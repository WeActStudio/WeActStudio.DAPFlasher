# Target identification

[简体中文](mcu-identification_zh_CN.md)

The "Target" row in the window used to show nothing but a core name (`Cortex-M7`), and
an H742, H743, H750 and H753 all answer with the same one. What tells them apart is the
vendor's **device id register** next to the debug unit.

## How it works

| Item | Value |
|---|---|
| Register | `DBGMCU_IDCODE` |
| Address (later H7 parts) | `0x5C001000` (APB3 / D3 domain) |
| Address (every other STM32) | `0xE0042000` |
| Fields | `[11:0]` = DEV_ID, `[31:16]` = REV_ID |

The addresses are tried in the order the table lists them. A part that does not own an
address faults the read - `readMem32()` clears the sticky error itself - and the probe
moves on. If no address answers, or the id is not in the table, the window keeps showing
the core name, exactly as it did before the table existed.

## The data file: `mcu-ids.txt`

**The table is not compiled in; it is `mcu-ids.txt` next to the executable** (UTF-8).
It is re-read on every identify, so an edit takes effect on the next Refresh - no
restart.

```
# One device per line, fields separated by '|'
#     <register address> | <value> | <vendor> | <device name> [ | <mask> ]
0x5C001000 | 0x450 | STMicroelectronics | STM32H742/743/750/753
0x10000100 | 0x52840 | Nordic Semiconductor | nRF52840 | 0x1FFFF
```

- `#` starts a comment, blank lines are ignored;
- **The mask is optional and defaults to `0xFFF`** (ST's 12 bit DEV_ID). Nordic's part
  code is 20 bits (`0x1FFFF`), Artery's fills the whole register (mask the range that
  identifies the family). The register is compared as
  `(read & mask) == (value & mask)`;
- Addresses are probed **in the order they appear**, so put the ones that hit most often
  first - a miss costs a round trip;
- The device name **must not contain `|`**; spaces and `/` are fine;
- **Without the file the program uses a short built-in list** (the common STM32 parts,
  nRF52840/832/833/820, AT32F403A). It does not create the file, and it does not stop
  identifying anything either;
- The full table ships next to the executable (source copy in the repository root as
  `mcu-ids.txt`). Add a part by editing that file.

To try a different table without touching the installed one:

```
set DAPFLASHER_MCU_DB=I:\la_cache\mcu-ids-test.txt
```

The code is `src/target/mcudb.cpp` (parser and probe); the built-in copy is
`kDefaultTable` in that file.

## How precise it is

**Sub-family, not orderable part number.** `0x450` covers the STM32H743VIT6, the
STM32H743ZIT6 and the STM32H750VBT6 alike - package, pin count and flash size are not in
`DBGMCU_IDCODE`:

- Size lives in each family's flash-size register (`0x1FFFF7E0` on F1, `0x1FFF7A22` on
  F2/F4; the H7 has no such register and keeps a 96 bit UID at `0x1FF1E880`). The address
  differs per family, and the payoff is small;
- Pin count and package are not exposed anywhere - only the marking or the project files
  know them.

That is why the window shows `STM32H743/753 (Cortex-M7)`: sub-family, with the core in
brackets.

## Why the flash algorithm is not the source

`FlashDevice.DeviceName` in a `.FLM` is the name of the **algorithm**, not of the chip:

```
STM32H7x_2048.FLM  -> DeviceName = "STM32H7x_2048"   (H7 family, 2 MB)
STM32F4xx_512.FLM  -> DeviceName = "STM32F4xx_512"
```

Two problems with using it:

1. It is coarser than DEV_ID - `H7x` covers the H742, H743, H750 and H753 all at once;
2. There is no algorithm name before the user picks an algorithm, and identifying the
   target is exactly what happens before that.

The other direction is useful: the sub-family cross-checks the algorithm. `STM32H743/753`
together with `STM32H7x_1024` means the wrong size was picked.

## Debug port id (DP IDCODE)

The first thing that answers is the **debug port**, not the chip. Its IDCODE is ADIv5's
DPIDR:

| Bits | Meaning |
|---|---|
| `[0]` | always 1 (tells a register apart from a dead bus) |
| `[11:1]` | designer, JEP106 code, `0x23B` = ARM Ltd |
| `[19:12]` | part number; SW-DP answers `0x01` (ADIv5.1) or `0x02` (ADIv5.2, multidrop) |
| rest | per-vendor revision coding, not standardised, **not guessed at** |

Measured on this H743: `0x6BA02477` -> `SW-DP v2, ARM, part 0x02`. An older Cortex-M3/M4
target answers `0x2BA01477` -> `SW-DP v1, ARM, part 0x01`.

**It cannot identify the part** - every part of a family with the same core generation
returns the same value. What it answers is "what is on the other end of this wire": a
designer other than `0x23B`, or a part number outside 0x01/0x02, means the target is not
a plain SW-DP (a JTAG-DP, or nothing wired correctly) - which is worth knowing before
reading any memory.

Code: `describeDpIdCode()` in `src/target/mcudb.cpp`. The command line puts it right
after `DP IDCODE:`; the window shows it in the hover text of the "Target" row
(`Debug port: ...`).

## Known entries (STM32)

This is the initial content of `mcu-ids.txt`. The values are the `DBGMCU_IDCODE` fields
from the ST reference manuals (RM0433 for the H7, the per-family manuals for the rest),
cross-checked against community-maintained tables. **These are sub-families, not part
numbers.**

### H7 (address `0x5C001000`)

| DEV_ID | Device |
|---|---|
| `0x450` | STM32H742/743/750/753 |
| `0x480` | STM32H7A3/7B3 |
| `0x483` | STM32H723/725/730/733/735 |

### F0 / F1 / F2 / F3

| DEV_ID | Device |
|---|---|
| `0x440` | STM32F030x8/F05x |
| `0x444` | STM32F030x4/6 |
| `0x442` | STM32F030xC/F09x |
| `0x445` | STM32F04x/F070x6 |
| `0x448` | STM32F070xB/F071/F072 |
| `0x410` | STM32F10x medium density |
| `0x412` | STM32F10x low density |
| `0x414` | STM32F10x high density |
| `0x418` | STM32F105/107 |
| `0x420` | STM32F100 value line |
| `0x428` | STM32F100 high density |
| `0x430` | STM32F10x XL density |
| `0x411` | STM32F2xx |
| `0x432` | STM32F373/378 |
| `0x422` | STM32F302/303/358 |
| `0x439` | STM32F301/302x6/318 |
| `0x438` | STM32F303x6/8, F334/328 |
| `0x446` | STM32F302xD/E, F303xD/E, F398 |

### F4 / F7

| DEV_ID | Device |
|---|---|
| `0x413` | STM32F405/407/415/417 |
| `0x419` | STM32F42x/43x |
| `0x423` | STM32F401xB/C |
| `0x433` | STM32F401xD/E |
| `0x458` | STM32F410 |
| `0x431` | STM32F411 |
| `0x441` | STM32F412 |
| `0x421` | STM32F446 |
| `0x434` | STM32F469/479 |
| `0x463` | STM32F413/423 |
| `0x449` | STM32F74x/75x |
| `0x451` | STM32F76x/77x |
| `0x452` | STM32F72x/73x |

### L0 / L1 / L4

| DEV_ID | Device |
|---|---|
| `0x457` | STM32L01x/02x |
| `0x425` | STM32L031/041 |
| `0x417` | STM32L05x/06x |
| `0x447` | STM32L07x/08x |
| `0x416` | STM32L1xx medium density |
| `0x429` | STM32L1xx medium density+ |
| `0x427` | STM32L1xx category 2 |
| `0x436` | STM32L1xx high density |
| `0x437` | STM32L1xx XL density |
| `0x415` | STM32L47x/48x |
| `0x435` | STM32L43x/44x |
| `0x462` | STM32L45x/46x |
| `0x461` | STM32L496/4A6 |
| `0x470` | STM32L4Rx/4Sx |
| `0x471` | STM32L4P5/Q5 |

### G0 / G4 / U5

| DEV_ID | Device |
|---|---|
| `0x460` | STM32G0x1 |
| `0x466` | STM32G03x/04x |
| `0x468` | STM32G431/441 |
| `0x469` | STM32G47x/48x |
| `0x479` | STM32G491/4A1 |
| `0x482` | STM32U535/545/575/585 |

## Other vendors

### Nordic nRF52

The register is FICR's `INFO.PART` (FICR at `0x10000000`, offset `0x100`), a 20 bit part
code:

| Value | Device |
|---|---|
| `0x52840` | nRF52840 |
| `0x52833` | nRF52833 |
| `0x52832` | nRF52832 |
| `0x52820` | nRF52820 |
| `0x52811` | nRF52811 |
| `0x52810` | nRF52810 |

The same FICR holds `INFO.FLASH` (`0x110`), `INFO.RAM` (`0x10C`) and `INFO.PACKAGE`
(`0x108`) - KB figures and a package code, if showing the size ever becomes worth one
more read.

### Artery AT32

`DEBUG_IDCODE` is at `0xE0042000`, 32 bits: the upper half is the family, the lower half
carries size and package:

| Value (mask `0xFFFF0000`) | Device |
|---|---|
| `0x70050000` | AT32F403A/407 (`0x70050242` is an AT32F403ARCT7) |

Telling individual parts apart needs the 32 bits from `0xE0042000` combined with the
project id at `0x1FFFF7F3` into a 64 bit code, exactly as Artery's own AN does with
`AT32_MCU_ID_TABLE`. That is far finer than this tool needs - family level is enough to
pick an algorithm.

### Deliberately absent

- **GD32**: `DBG_ID` sits at the same address and carries the same values as the STM32 it
  replaces - a GD32F303 and an STM32F405 both answer `0x413`, a GD32F103 and an
  STM32F103 both answer `0x414`. Picking one would be a coin flip; a family name is
  better than a wrong part number.
- **NXP LPC / Kinetis**: the `SIM_SDID` address differs per series, and most LPC parts
  have no equivalent register at all.
- **WCH CH32V**: RISC-V, so there is no ARM `DBGMCU`; the `ESIG` area around
  `0x1FFFF7E0` is laid out differently per series, and this tool only programs Cortex-M.
- **Raspberry Pi RP2040**: `SYSINFO_CHIP_ID` reads back 0, which fails the "not zero, not
  all ones" test that every other probe relies on.

## Adding a vendor

Edit `mcu-ids.txt`; no rebuild needed:

1. Find the vendor's device id register address and value width in the reference manual;
2. Add a row as `address | value | vendor | device name | mask`. Rows that share an
   address share one probe.

A width other than 12 bits is fine - that is what the mask column is for. The one case
that needs code is an id that has to be shifted or stitched from two registers (Artery's
64 bit code, or a vendor that puts the id in the high byte).

## Verification

WeAct-MiniSTM32H7xx (STM32H743VIT6), identical output with and without `mcu-ids.txt`:

```
Target: Cortex-M7
CPUID: 0x411fc271
DP IDCODE: 0x6ba02477 (SW-DP v2, ARM, part 0x02)
MCU: STM32H742/743/750/753 (STMicroelectronics, device id 0x00000450, rev 0x00002003)
State: running
```

The window's "Target" row shows `STM32H742/743/750/753 (Cortex-M7)`; its hover text
carries `CPUID ... , DP IDCODE ... , core ...` plus `Debug port: ...` and
`Vendor device id ...`.
