#!/usr/bin/env python3
"""Inspect a Keil .FLM flash programming algorithm (ELF32-LE-ARM).

Dumps the loadable segments, the exported flash function symbols and the
FlashDevice structure, so that the C++ parser in src/flash can be checked
against real files.

Usage:
    python flm_inspect.py <file.FLM> [...]
"""

import struct
import sys
import os

# ---------------------------------------------------------------------------
#  Minimal ELF32 little endian reader
# ---------------------------------------------------------------------------

SHT_SYMTAB = 2
PT_LOAD = 1


class Elf32(object):
    def __init__(self, data):
        self.data = data
        if data[:4] != b"\x7fELF":
            raise ValueError("not an ELF file")
        if data[4] != 1:
            raise ValueError("not ELF32")
        if data[5] != 1:
            raise ValueError("not little endian")

        self.machine = struct.unpack_from("<H", data, 0x12)[0]
        self.entry = struct.unpack_from("<I", data, 0x18)[0]
        e_phoff = struct.unpack_from("<I", data, 0x1C)[0]
        e_shoff = struct.unpack_from("<I", data, 0x20)[0]
        e_phentsize = struct.unpack_from("<H", data, 0x2A)[0]
        e_phnum = struct.unpack_from("<H", data, 0x2C)[0]
        e_shentsize = struct.unpack_from("<H", data, 0x2E)[0]
        e_shnum = struct.unpack_from("<H", data, 0x30)[0]
        e_shstrndx = struct.unpack_from("<H", data, 0x32)[0]

        self.phdrs = []
        for i in range(e_phnum):
            o = e_phoff + i * e_phentsize
            (p_type, p_offset, p_vaddr, p_paddr, p_filesz, p_memsz,
             p_flags, p_align) = struct.unpack_from("<8I", data, o)
            self.phdrs.append(dict(type=p_type, offset=p_offset, vaddr=p_vaddr,
                                   paddr=p_paddr, filesz=p_filesz, memsz=p_memsz,
                                   flags=p_flags, align=p_align))

        self.shdrs = []
        for i in range(e_shnum):
            o = e_shoff + i * e_shentsize
            (name, s_type, flags, addr, off, size, link, info, align,
             entsize) = struct.unpack_from("<10I", data, o)
            self.shdrs.append(dict(name=name, type=s_type, flags=flags, addr=addr,
                                   offset=off, size=size, link=link, info=info,
                                   align=align, entsize=entsize))

        if e_shstrndx < len(self.shdrs):
            s = self.shdrs[e_shstrndx]
            shstr = data[s["offset"]:s["offset"] + s["size"]]
            for sh in self.shdrs:
                end = shstr.find(b"\0", sh["name"])
                sh["sname"] = shstr[sh["name"]:end].decode("latin-1")
        self.symbols = self._read_symbols()

    def _cstr(self, blob, off):
        end = blob.find(b"\0", off)
        return blob[off:end].decode("latin-1")

    def _read_symbols(self):
        out = []
        for sh in self.shdrs:
            if sh["type"] != SHT_SYMTAB:
                continue
            strtab = self.shdrs[sh["link"]]
            strings = self.data[strtab["offset"]:strtab["offset"] + strtab["size"]]
            n = sh["size"] // 16
            for i in range(n):
                o = sh["offset"] + i * 16
                st_name, st_value, st_size, st_info, st_other, st_shndx = \
                    struct.unpack_from("<IIIBBH", self.data, o)
                if st_name == 0:
                    continue
                out.append(dict(name=self._cstr(strings, st_name),
                                value=st_value, size=st_size,
                                type=st_info & 0xF, bind=st_info >> 4,
                                shndx=st_shndx))
        return out

    def symbol(self, name):
        for s in self.symbols:
            if s["name"] == name:
                return s
        return None

    def read_file_offset(self, off, size):
        return self.data[off:off + size]

    def va_to_offset(self, addr):
        """Map a load address (p_paddr) to a file offset."""
        for p in self.phdrs:
            if p["type"] != PT_LOAD:
                continue
            base = p["paddr"] if p["paddr"] else p["vaddr"]
            if base <= addr < base + p["filesz"]:
                return p["offset"] + (addr - base)
        return None


# ---------------------------------------------------------------------------
#  FlashDevice (Keil FlashOS.h, VERS 1.x)
# ---------------------------------------------------------------------------

SECTOR_NUM = 512


def parse_flash_device(elf, addr):
    off = elf.va_to_offset(addr)
    if off is None:
        return None
    blob = elf.read_file_offset(off, 160 + SECTOR_NUM * 8)
    if len(blob) < 160:
        return None

    dev = {}
    dev["addr"] = addr
    dev["Vers"] = struct.unpack_from("<H", blob, 0)[0]
    raw_name = blob[2:130]
    dev["DevName"] = raw_name.split(b"\0")[0].decode("latin-1")
    dev["DevType"] = struct.unpack_from("<H", blob, 130)[0]
    (dev["DevAdr"], dev["szDev"], dev["szPage"], dev["Res"]) = \
        struct.unpack_from("<4I", blob, 132)
    dev["valEmpty"] = blob[148]
    dev["toProg"], dev["toErase"] = struct.unpack_from("<2I", blob, 152)

    dev["sectors"] = []
    for i in range(SECTOR_NUM):
        sz, adr = struct.unpack_from("<2I", blob, 160 + i * 8)
        if sz == 0xFFFFFFFF and adr == 0xFFFFFFFF:
            break
        dev["sectors"].append((adr, sz))
    return dev


FLASH_FUNCS = ["Init", "UnInit", "BlankCheck", "EraseChip", "EraseSector",
               "ProgramPage", "Verify"]


def inspect(path):
    print("=" * 78)
    print(os.path.basename(path))
    print("=" * 78)
    data = open(path, "rb").read()
    elf = Elf32(data)
    print("machine     : %d (40 = ARM)   entry: 0x%08X   size: %d bytes"
          % (elf.machine, elf.entry, len(data)))

    print("\n-- loadable segments (PT_LOAD) --")
    for p in elf.phdrs:
        if p["type"] != PT_LOAD:
            continue
        print("  offset 0x%06X  paddr 0x%08X  filesz %6d  memsz %6d  flags %d"
              % (p["offset"], p["paddr"], p["filesz"], p["memsz"], p["flags"]))

    print("\n-- flash function symbols --")
    found = {}
    for name in FLASH_FUNCS:
        s = elf.symbol(name)
        if s:
            found[name] = s["value"]
            print("  %-12s @ 0x%08X" % (name, s["value"]))
        else:
            print("  %-12s <missing>" % name)

    print("\n-- FlashDevice --")
    s = elf.symbol("FlashDevice")
    if not s:
        print("  symbol 'FlashDevice' not found (%d symbols in table)"
              % len(elf.symbols))
        names = [x["name"] for x in elf.symbols][:40]
        print("  first symbols:", names)
        return
    dev = parse_flash_device(elf, s["value"])
    if not dev:
        print("  could not read the structure at 0x%08X" % s["value"])
        return
    print("  address      : 0x%08X" % dev["addr"])
    print("  Vers         : 0x%04X" % dev["Vers"])
    print("  DevName      : %s" % dev["DevName"])
    print("  DevType      : %d" % dev["DevType"])
    print("  DevAdr       : 0x%08X" % dev["DevAdr"])
    print("  szDev        : %d (%.1f KB)" % (dev["szDev"], dev["szDev"] / 1024.0))
    print("  szPage       : %d" % dev["szPage"])
    print("  Res          : %d" % dev["Res"])
    print("  valEmpty     : 0x%02X" % dev["valEmpty"])
    print("  toProg       : %d ms" % dev["toProg"])
    print("  toErase      : %d ms" % dev["toErase"])
    print("  sectors      : %d entries" % len(dev["sectors"]))
    for adr, sz in dev["sectors"][:8]:
        print("      0x%08X  %8d bytes" % (adr, sz))
    if len(dev["sectors"]) > 8:
        print("      ...")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)
    for arg in sys.argv[1:]:
        inspect(arg)
