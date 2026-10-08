# -*- coding: utf-8 -*-
"""
Fill the Qt translation file with the Simplified Chinese translations.

English is the source language of every tr() string in the source code, so only
translations/app_zh_CN.ts needs a translation; switching the UI to English just
removes the installed translator (see LanguageManager).

The dictionary below must cover *every* message that lupdate extracts from the
project. Run the script after lupdate: any source text that is not in the
dictionary is reported as MISSING (and left unfinished, so it shows up as
English in the UI), and any dictionary entry that no longer exists in the
source is reported as UNUSED.

Usage:
    python tools/i18n_fill.py <project directory>
"""
import sys
import os
import xml.etree.ElementTree as ET

# English source text -> Simplified Chinese translation
ZH = {
    # ---------------------------------------------------------------- AlgoCatalog
    "Keil built-in algorithms": "Keil 内置算法",
    "Additional folder": "附加目录",
    "Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)":
        "下载算法 (*.flm *.FLM *.flx *.xldr);;所有文件 (*.*)",

    # ------------------------------------------------------------ AlgorithmPicker
    "Select algorithm ...": "选择算法...",
    "Filter algorithms ...": "筛选算法...",

    # ---------------------------------------------------------------- DapTransport
    # The transport kind is spelled "USB HID" / "USB bulk"; the prefix question
    # is a framing detail, not a protocol version (see src/dap/probes.h). The
    # spec-conformant bulk framing carries the bare CMSIS-DAP packet - the same
    # bytes a v1 HID report carries - and the length-prefixed form is a
    # tool-side convention, so neither is called "v1"/"v2" here.
    "USB HID (CMSIS-DAP v1)": "USB HID（CMSIS-DAP v1）",
    "USB bulk (WinUSB, CMSIS-DAP v2)": "USB 批量（WinUSB，CMSIS-DAP v2）",
    "CMSIS-DAP bulk transport, spec framing (no length prefix)":
        "CMSIS-DAP 批量传输，规范帧（无长度前缀）",
    "CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)":
        "CMSIS-DAP 批量传输，长度前缀帧（2 字节前缀）",
    "The probe did not answer DAP_Info. Check that the right interface of the "
    "device was selected and that the firmware runs a CMSIS-DAP implementation.":
        "探针未应答 DAP_Info。请确认已选择设备的正确接口，且其固件实现了 CMSIS-DAP。",
    "unknown HID error": "未知 HID 错误",
    "The HID probe could not be opened. It is probably used by another program "
    "(Keil, OpenOCD, a debug session).":
        "无法打开 HID 探针，它可能已被其他程序占用（Keil、OpenOCD 或调试会话）。",
    "HID write failed: %1": "HID 写入失败：%1",
    "HID read failed: %1": "HID 读取失败：%1",
    "The probe did not answer (timeout).": "探针无应答（超时）。",
    "The USB (libusb) layer could not be initialised.": "USB (libusb) 层初始化失败。",
    "USB enumeration failed.": "USB 枚举失败。",
    "The USB probe was not found any more. Replug it and refresh the list.":
        "已找不到该 USB 探针，请重新插拔后刷新列表。",
    "Access to the USB probe was denied. It is either used by another program, "
    "or no WinUSB driver is bound to it.":
        "USB 探针访问被拒绝：要么已被其他程序占用，要么未绑定 WinUSB 驱动。",
    "The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, "
    "or select the same probe over its USB HID (CMSIS-DAP v1) interface.":
        "该探针未绑定 WinUSB（或 libusbK）驱动。请安装驱动，"
        "或改用它的 USB HID（CMSIS-DAP v1）接口选择同一个探针。",
    "Opening the USB probe failed: %1": "打开 USB 探针失败：%1",
    "Claiming the USB interface failed: %1": "占用 USB 接口失败：%1",
    "USB bulk write failed: %1": "USB 批量写入失败：%1",
    "USB bulk read failed: %1": "USB 批量读取失败：%1",
    "The probe sent a malformed packet.": "探针返回了格式错误的报文。",
    "The command does not fit into one packet (%1 > %2 bytes).":
        "命令超出单个报文长度（%1 > %2 字节）。",

    # ------------------------------------------------------------------ CmsisDap
    "The DP did not acknowledge the power up request within %1 ms.":
        "DP 未在 %1 毫秒内确认上电请求。",

    # ------------------------------------------------------------ LanguageManager
    "Follow System(&A)": "跟随系统(&A)",
    "English(&E)": "English(&E)",
    "Simplified Chinese(&C)": "简体中文(&C)",
    "Simplified Chinese": "简体中文",
    "English": "English",

    # ---------------------------------------------------------------- MainWindow
    # window title / about
    "WeAct Studio DAP Flasher": "WeAct Studio DAP 烧录工具",
    "<h3>WeAct Studio DAP Flasher</h3><p>Version: %1</p><p><b>WeAct Studio</b><br>"
    "Copyright (C) 2026 WeAct Studio. All rights reserved.</p>"
    "<p>Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or USB bulk / "
    "WinUSB) using the Keil .FLM flash algorithms.</p>"
    "<p>This program is free software: you can redistribute it and/or modify it "
    "under the terms of the GNU General Public License as published by the Free "
    "Software Foundation, either version 3 of the License, or (at your option) any "
    "later version. It is distributed in the hope that it will be useful, but "
    "WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
    "FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more "
    "details.</p>"
    "<p>License: GPL-3.0-or-later<br>"
    "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
    "https://www.gnu.org/licenses/gpl-3.0.html</a></p>"
    "<p>Built with Qt %2, statically compiled with MSYS2 (MINGW64).</p>":
        "<h3>WeAct Studio DAP 烧录工具</h3><p>版本：%1</p>"
        "<p><b>WeAct Studio</b><br>"
        "Copyright (C) 2026 WeAct Studio 保留所有权利。</p>"
        "<p>通过 CMSIS-DAP 探针（USB HID 或 USB 批量 / WinUSB），"
        "使用 Keil .FLM 下载算法烧录 Cortex-M 器件。</p>"
        "<p>本程序为自由软件：您可以依据自由软件基金会发布的 GNU 通用公共许可证"
        "第 3 版或（按您选择的）任何更新版本的条款重新发布和修改本程序。"
        "发布本程序是希望它能有所用处，但不提供任何担保，甚至不隐含"
        "适销性或特定用途适用性的担保。详见 GNU 通用公共许可证。</p>"
        "<p>许可协议：GPL-3.0-or-later<br>"
        "<a href='https://www.gnu.org/licenses/gpl-3.0.html'>"
        "https://www.gnu.org/licenses/gpl-3.0.html</a></p>"
        "<p>基于 Qt %2 开发，使用 MSYS2 (MINGW64) 静态编译。</p>",

    # section titles
    "Debug Probe": "调试探针",
    "Flash Algorithm": "下载算法",
    "Firmware File": "固件文件",
    "Options": "选项",
    "Log": "日志",

    # probe / target row
    "Probe:": "探针：",
    "Connection:": "连接状态：",
    "Target:": "目标：",
    "(no probe found)": "（未发现探针）",
    "Connect a CMSIS-DAP probe and press Refresh.":
        "请连接 CMSIS-DAP 探针，然后点击“刷新”。",
    "<font color='%1'>No probe found</font>":
        "<font color='%1'>未发现探针</font>",
    "<font color='%1'>Connected (%2)</font>":
        "<font color='%1'>已连接（%2）</font>",
    "<font color='%1'>Probe present, target not identified</font>":
        "<font color='%1'>探针已连接，但未能识别目标</font>",
    "SWD": "SWD",
    "Identifying the target over %1 ...": "正在通过 %1 识别目标 ...",
    "CMSIS-DAP firmware: %1": "CMSIS-DAP 固件版本：%1",
    "CPUID %1, DP IDCODE %2, core %3": "CPUID %1，DP IDCODE %2，内核 %3",
    "halted": "已暂停",
    "running": "运行中",
    "unknown": "未知",

    # algorithm row
    "Algorithm File:": "算法文件：",
    "Select a Keil Flash Algorithm": "选择 Keil 下载算法",
    "<font color='%1'>No algorithm selected</font>":
        "<font color='%1'>尚未选择算法</font>",
    "<b>%1</b><br>%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors":
        "<b>%1</b><br>%2，Flash 0x%3 - 0x%4（%5），页 %6 字节，%7 个扇区",
    "<br><font color='%1'>This algorithm has no chip erase; the sectors are "
    "erased instead.</font>":
        "<br><font color='%1'>该算法不支持整片擦除，将改为按扇区擦除。</font>",
    "The algorithm %1 could not be loaded: %2": "无法加载算法 %1：%2",
    "Algorithm loaded: %1 (%2)": "已加载算法：%1（%2）",

    # algorithm scan log
    "Scanning flash algorithms (%1 pack folders) ...":
        "正在扫描下载算法（%1 个 pack 目录）...",
    "%1 flash algorithms found in %2 groups.":
        "共找到 %1 个下载算法，分布在 %2 个分组中。",
    "%1 file(s) were skipped: they carry no usable FlashDevice structure.":
        "已跳过 %1 个文件：其中没有可用的 FlashDevice 结构。",
    "Additional folder: %1": "附加目录：%1",
    "Recently used": "最近使用",
    "Last used": "上次使用",

    # firmware row
    "File Path:": "文件路径：",
    "Firmware:": "固件：",
    "Select or drop a .hex / .bin firmware file": "选择或直接拖入 .hex / .bin 固件文件",
    "Select Firmware File": "选择固件文件",
    "Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
    "Binary Files (*.bin);;All Files (*.*)":
        "固件文件 (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;"
        "二进制文件 (*.bin);;所有文件 (*.*)",
    "<font color='%1'>No valid firmware file selected</font>":
        "<font color='%1'>尚未选择有效的固件文件</font>",
    "%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6":
        "%1　大小：%2（%3 个数据块）　地址：0x%4 - 0x%5　校验和：0x%6",
    "Intel HEX": "Intel HEX",
    "BIN": "BIN",
    "BIN Load Address: 0x": "BIN 烧录地址：0x",
    "Loading address used for .bin files (ignored for .hex)":
        ".bin 文件使用的烧录起始地址（.hex 忽略此项）",

    # options row
    "SWD Clock:": "SWD 时钟：",
    "Algorithm RAM: 0x": "算法 RAM：0x",
    "RAM area the flash algorithm is copied to while it runs. 0x20000000 is "
    "correct for most Cortex-M devices.":
        "下载算法运行时被复制到的 RAM 区域。大多数 Cortex-M 器件填 0x20000000 即可。",
    "Whole chip erase": "整片擦除",
    "Verify": "校验",
    "Reset and run": "复位并运行",
    "Reset Mode:": "复位模式：",
    "Reset used by the Reset button and by \"Reset and run\". Software reset "
    "needs no extra wire, hardware reset pulses the nRESET line, automatic "
    "uses nRESET when the probe has one.":
        "复位按钮与「复位并运行」使用的复位方式。软件复位不需要额外接线；"
        "硬件复位拉低 nRESET 线；自动在探针有 nRESET 线时使用它。",

    # buttons
    "Refresh(&R)": "刷新(&R)",
    "Browse(&B)...": "浏览(&B)...",
    "Cancel(&C)": "取消(&C)",
    "Start Programming(&S)": "开始烧录(&S)",
    "Halt(&P)": "暂停(&P)",
    "Run(&R)": "运行(&R)",
    "Reset(&T)": "复位(&T)",
    "Exit(&X)": "退出(&X)",

    # menus
    "File(&F)": "文件(&F)",
    "Open Firmware(&O)...": "打开固件(&O)...",
    "Select Algorithm(&A)...": "选择算法(&A)...",
    "Tools(&T)": "工具(&T)",
    "Refresh Probes(&R)": "刷新探针(&R)",
    "Rescan Algorithms(&L)": "重新扫描算法(&L)",
    "Show All USB HID Devices(&H)": "显示所有 USB HID 设备(&H)",
    "Clear Log(&C)": "清空日志(&C)",
    "View(&V)": "视图(&V)",
    "Language(&L)": "语言(&L)",
    "Help(&H)": "帮助(&H)",
    "About(&A)": "关于(&A)",

    # messages
    "Ready": "就绪",
    "Preparing ...": "准备中 ...",
    "Cancelling ...": "正在取消 ...",
    "Programming complete": "烧录完成",
    "Complete": "完成",
    "The firmware was programmed successfully.\n\n%1":
        "固件烧录成功。\n\n%1",
    "Cancelled": "已取消",
    "Programming failed": "烧录失败",
    "Failed": "失败",
    "Programming in Progress": "正在烧录",
    "Programming is still running; stopping now can leave the device with an "
    "incomplete firmware.\n\nExit anyway?":
        "烧录仍在进行中，现在退出可能使设备固件不完整。\n\n仍要退出吗？",
    "About": "关于",

    # dialog / status bar texts
    "-": "-",
    "<font color='%1'>%2</font>": "<font color='%1'>%2</font>",
    "No Probe": "未发现探针",
    "Select a CMSIS-DAP probe first.": "请先选择一个 CMSIS-DAP 探针。",
    "Select a debug probe first.": "请先选择一个调试探针。",
    "Halt Failed": "暂停失败",
    "The core is halted.": "内核已暂停。",
    "Run Failed": "运行失败",
    "The core is running.": "内核正在运行。",
    "Reset Failed": "复位失败",
    "The target was reset and is running.": "目标已复位并开始运行。",
    "No Algorithm": "未选择算法",
    "Select the Keil .FLM flash algorithm that matches the device.":
        "请选择与目标器件匹配的 Keil .FLM 下载算法。",
    "No Firmware": "未选择固件",
    "Select a valid firmware file first.": "请先选择一个有效的固件文件。",

    # ------------------------------------------------------------------- Probes
    "HID device": "HID 设备",
    " (in use)": "（被占用）",
    "The HID interface could not be initialised.": "HID 接口初始化失败。",
    "HID init failed: %1": "HID 初始化失败：%1",
    "HID init failed.": "HID 初始化失败。",
    "Transport: %1": "传输方式：%1",
    "Product: %1": "产品名称：%1",
    "Manufacturer: %1": "厂商：%1",
    "Serial: %1": "序列号：%1",
    "USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5":
        "USB：总线 %1 地址 %2，接口 %3，端点 IN 0x%4 / OUT 0x%5",
    "Interface: %1": "接口：%1",
    "Usage page: 0x%1  Usage: 0x%2": "Usage Page：0x%1　Usage：0x%2",

    # -------------------------------------------------- CmsisDap / CortexM (SWD)
    "No probe is open.": "没有打开的探针。",
    "The probe sent an empty answer.": "探针返回了空应答。",
    "The probe answered with a malformed DAP_Info response.":
        "探针对 DAP_Info 的应答格式错误。",
    "The probe sent a truncated DAP_Info response.": "探针返回的 DAP_Info 应答被截断。",
    "The probe answered with a malformed DAP_Connect response.":
        "探针对 DAP_Connect 的应答格式错误。",
    "The probe refused the SWD connection.": "探针拒绝了 SWD 连接。",
    "Setting the SWD clock failed.": "设置 SWD 时钟失败。",
    "Configuring the DAP transfer failed.": "配置 DAP 传输失败。",
    "The probe answered with a malformed DAP_SWJ_Pins response.":
        "探针对 DAP_SWJ_Pins 的应答格式错误。",
    "The probe answered with a malformed DAP_Reset_Target response.":
        "探针对 DAP_Reset_Target 的应答格式错误。",
    "The SWJ sequence failed.": "SWJ 序列执行失败。",
    "The SWD clock could not be set: %1": "无法设置 SWD 时钟：%1",
    "SWD clock: %1": "SWD 时钟：%1",
    "The SWD DP did not answer, trying to revive the link.":
        "SWD DP 无应答，正在尝试重新建立连接。",
    "The SWD link came back at %1 Hz.": "SWD 连接已恢复（%1 Hz）。",
    "Holding nRESET low and connecting under reset.":
        "保持 nRESET 为低，改为在复位状态下连接。",
    "The SWD link came back under reset at %1 Hz.":
        "SWD 连接已在复位状态下恢复（%1 Hz）。",
    "Connecting under reset did not bring the DP back either.":
        "复位状态下连接同样没能唤回 DP。",
    "The SWD DP did not answer while nRESET was being driven.":
        "驱动 nRESET 期间 SWD DP 仍无应答。",
    "This probe does not drive an nRESET line.":
        "该探针不驱动 nRESET 线。",
    "The target program was holding the debug access; "
    "the core is now halted under reset.":
        "目标程序占住了调试访问；现已在内核复位状态下将其暂停。",
    "The SWD DP reported an invalid IDCODE.": "SWD DP 返回了无效的 IDCODE。",
    "The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), "
    "the target power supply and the SWD clock. A target program that "
    "switched the debug interface off, or that put the core into a low "
    "power state, only comes back on a power cycle. If it comes back "
    "with BOOT0 held high, that program is what takes the debug pins "
    "over: start the ROM bootloader that way, which leaves the debug "
    "pins alone, and erase the chip from there.":
        "SWD DP 无应答。请检查接线（SWDIO、SWCLK、GND）、目标板供电以及 SWD 时钟。"
        "若目标程序关闭了调试接口，或让内核进入了低功耗状态，则只有重新上电才能恢复。"
        "如果拉高 BOOT0 后能连上，说明正是该程序占用了调试引脚："
        "用这种方式启动 ROM 引导程序（它不会动调试引脚），再从那里整片擦除。",
    "The DP read returned no data.": "DP 读取没有返回数据。",
    "The AP read returned no data.": "AP 读取没有返回数据。",
    "Empty transfer.": "传输内容为空。",
    "Internal error: the write value count does not match the request count.":
        "内部错误：写入值的数量与请求数量不一致。",
    "The probe answered with a truncated transfer response.":
        "探针返回的传输应答被截断。",
    "The probe answered with a malformed DAP_Transfer response.":
        "探针对 DAP_Transfer 的应答格式错误。",
    "SWD fault (the target did not acknowledge the access).":
        "SWD 故障（目标未应答本次访问）。",
    "Protocol error during the SWD transfer.": "SWD 传输过程中出现协议错误。",
    "The probe performed %1 of %2 transfers.": "探针只完成了 %2 次传输中的 %1 次。",
    "The probe sent a truncated transfer payload.": "探针返回的传输数据被截断。",
    "The SWD transfer was retried too often (WAIT response).":
        "SWD 传输重试次数过多（WAIT 应答）。",
    "Empty block transfer.": "块传输内容为空。",
    "The probe answered with a truncated block transfer response.":
        "探针返回的块传输应答被截断。",
    "The probe answered with a malformed DAP_TransferBlock response.":
        "探针对 DAP_TransferBlock 的应答格式错误。",
    "SWD fault during the block transfer.": "块传输过程中发生 SWD 故障。",
    "Protocol error during the block transfer.": "块传输过程中出现协议错误。",
    "The probe performed %1 of %2 block transfers.":
        "探针只完成了 %2 次块传输中的 %1 次。",
    "The probe sent a truncated block transfer payload.":
        "探针返回的块传输数据被截断。",

    "Unknown Cortex-M core": "未知的 Cortex-M 内核",
    "No Cortex-M core answered at CPUID (0x%1).":
        "在 CPUID (0x%1) 处没有 Cortex-M 内核应答。",
    "The core is locked up (the flash algorithm crashed).":
        "内核已锁死（下载算法崩溃）。",
    "The core did not halt within %1 ms.": "内核在 %1 毫秒内没有暂停。",
    "The probe has no nRESET line; use the software reset instead.":
        "该探针没有 nRESET 线，请改用软件复位。",
    "Software reset": "软件复位",
    "Hardware reset": "硬件复位",
    "Automatic": "自动（优先硬件复位）",
    "Reset: %1": "复位方式：%1",
    "Reset: %1 (the DP did not answer, so this was a pin reset only).":
        "复位方式：%1（DP 无应答，因此只脉冲了复位线）。",
    "The core register transfer timed out.": "内核寄存器传输超时。",
    "Internal error: no register buffer.": "内部错误：缺少寄存器缓冲区。",

    # -------------------------------------------------------------- ElfFile
    "The algorithm file could not be opened: %1": "无法打开算法文件：%1",
    "The file is too small to be an ELF image.": "文件太小，不可能是 ELF 映像。",
    "The file is not an ELF image (a Keil .FLM algorithm is expected).":
        "该文件不是 ELF 映像（这里需要 Keil .FLM 算法）。",
    "Only 32 bit ELF images are supported (this one is 64 bit).":
        "仅支持 32 位 ELF 映像（该文件是 64 位）。",
    "Only little endian ELF images are supported.": "仅支持小端 ELF 映像。",
    "The image is not an ARM image (e_machine = %1).":
        "该映像不是 ARM 映像（e_machine = %1）。",
    "The image is truncated (segment %1 leaves the file).":
        "映像被截断（第 %1 段超出文件末尾）。",
    "The image carries no loadable segment.": "映像中没有可加载的段。",

    # -------------------------------------------------------------- FlashAlgo
    "The algorithm does not export an Init() function.":
        "该算法没有导出 Init() 函数。",
    "The algorithm does not export a ProgramPage() function.":
        "该算法没有导出 ProgramPage() 函数。",
    "The algorithm does not export EraseSector() nor EraseChip().":
        "该算法既没有导出 EraseSector() 也没有导出 EraseChip()。",
    "The FlashDevice structure could not be located in the algorithm.":
        "无法在算法中定位 FlashDevice 结构。",
    "The FlashDevice structure at offset 0x%1 is not valid.":
        "偏移 0x%1 处的 FlashDevice 结构无效。",
    "The algorithm carries an empty sector table.": "该算法的扇区表为空。",
    "Unknown": "未知",
    "On-chip flash": "片内 Flash",
    "External flash, 8 bit bus": "片外 Flash，8 位总线",
    "External flash, 16 bit bus": "片外 Flash，16 位总线",
    "External flash, 32 bit bus": "片外 Flash，32 位总线",
    "External SPI flash": "片外 SPI Flash",

    # ------------------------------------------------------------ FlashWorker
    "Loading the flash algorithm ...": "正在加载下载算法 ...",
    "Algorithm %1: %2, %3, page %4 bytes, %5 sectors":
        "算法 %1：%2，%3，页 %4 字节，%5 个扇区",
    "%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6":
        "%1：%2（%3 个数据块），0x%4 - 0x%5，校验和 0x%6",
    "Firmware %1": "固件 %1",
    "The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm "
    "(0x%3 - 0x%4). Check the loading address.":
        "固件（0x%1 - 0x%2）超出了算法对应的 Flash 范围（0x%3 - 0x%4）。"
        "请检查烧录地址。",
    "The algorithm has no sector at 0x%1.": "该算法在 0x%1 处没有对应的扇区。",
    "Cancelled.": "已取消。",
    "Probe: %1": "探针：%1",
    "CMSIS-DAP firmware: %1": "CMSIS-DAP 固件版本：%1",
    "Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)":
        "目标：%1（CPUID 0x%2，DP IDCODE 0x%3）",
    "Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.":
        "烧录完成，耗时 %1 秒：%2 页、%3 字节，擦除 %4 个扇区。",
    "No firmware given: the whole chip is erased and the target is left halted.":
        "未提供固件：将整片擦除，并把目标保持在内核暂停状态。",
    "The whole chip was erased in %1 s; the target is left halted.":
        "整片擦除完成，耗时 %1 秒；目标保持在内核暂停状态。",
    "The algorithm used 0x%1 of RAM at %2.":
        "该算法占用了 %2 处的 0x%1 字节 RAM。",
    "The SWD link failed at %1; retrying at %2.":
        "SWD 链路在 %1 下失败，改用 %2 重试。",
    "Retrying at a slower clock ...": "正在以更低的时钟重试 ...",

    # --------------------------------------------------------------- Flasher
    "The algorithm image is empty.": "算法映像为空。",
    "The algorithm reports an unusable page size (%1 bytes).":
        "算法报告的页大小不可用（%1 字节）。",
    "The RAM base address is too high for the algorithm.":
        "RAM 基地址对该算法来说过高。",
    "The algorithm could not be written to the target RAM at %1: "
    "%2 of %3 bytes came back different, the first one at %4 "
    "(wrote 0x%5, read 0x%6). The base address has to be RAM the "
    "debug unit can write to; if it is, the probe did not move the "
    "data reliably.":
        "无法把算法写入目标 RAM 的 %1 处：回读后有 %2/%3 字节不一致，"
        "第一个不一致在 %4（写入 0x%5，读回 0x%6）。该基地址必须是调试单元"
        "可写的 RAM；如果确实是 RAM，那就是探针没有可靠地搬运数据。",
    "RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, "
    "command packet %3 bytes.":
        "RAM 传输：每次 DAP_TransferBlock 传送 %1 个字（%2 字节），命令包 %3 字节。",
    "Writing the algorithm to the RAM failed, attempt %1 of %2: %3":
        "把算法写入 RAM 失败，第 %1/%2 次尝试：%3",
    "The algorithm does not provide %1().": "该算法没有提供 %1()。",
    "%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)":
        "%1() 没有正常返回：%2（PC 0x%3，DFSR 0x%4，CFSR 0x%5，HFSR 0x%6）",
    "%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).":
        "%1() 停在了 0x%2，而没有返回 0x%3（DFSR = 0x%4）。",
    "Erasing the whole chip ...": "正在整片擦除 ...",
    "EraseChip() failed with code %1.": "EraseChip() 失败，错误码 %1。",
    "Chip erase done.": "整片擦除完成。",
    "The algorithm provides neither EraseChip() nor EraseSector().":
        "该算法既没有提供 EraseChip() 也没有提供 EraseSector()。",
    "Address 0x%1 is not covered by the sector table of the algorithm.":
        "地址 0x%1 不在该算法的扇区表范围内。",
    "Nothing to erase.": "没有需要擦除的内容。",
    "Erasing sector %1 of %2 (0x%3) ...": "正在擦除第 %1/%2 个扇区（0x%3）...",
    "Erasing the sector at 0x%1 failed with code %2.":
        "擦除 0x%1 处的扇区失败，错误码 %2。",
    "The firmware file carries no data.": "固件文件中没有数据。",
    "Programming 0x%1 (%2 / %3 bytes) ...": "正在烧录 0x%1（%2 / %3 字节）...",
    "Programming the page at 0x%1 failed with code %2.":
        "烧录 0x%1 处的页失败，错误码 %2。",
    "The algorithm has no Verify() routine; the flash is read back through the "
    "debug port instead.":
        "该算法没有 Verify() 例程，将改由调试口回读 Flash 进行校验。",
    "Verifying 0x%1 ...": "正在校验 0x%1 ...",
    "Verification failed at 0x%1 (code %2).": "校验失败于 0x%1（错误码 %2）。",
    "Reading back 0x%1 failed; the verification was skipped.":
        "回读 0x%1 失败，已跳过校验。",
    "Verification failed at 0x%1: expected 0x%2, read 0x%3.":
        "校验失败于 0x%1：期望 0x%2，实际读到 0x%3。",
    "No flash algorithm is selected.": "尚未选择下载算法。",
    "No firmware data to program.": "没有需要烧录的固件数据。",
    "Algorithm: %1 (%2, %3 bytes)": "算法：%1（%2，%3 字节）",
    "Algorithm loaded at 0x%1 (%2 bytes of RAM).":
        "算法已加载到 0x%1（占用 %2 字节 RAM）。",
    "Init() failed with code %1.": "Init() 失败，错误码 %1。",
    "Init() did not complete, restarting the algorithm (%1 of %2): %3":
        "Init() 未完成，正在重启算法（第 %1 次，共 %2 次）：%3",
    "The core could not be halted (%1). It has been reset and halted on the reset vector instead (%2).":
        "内核无法暂停（%1）。已改为复位目标，并在复位向量处将其暂停（%2）。",
    "Initialised.": "初始化完成。",
    "UnInit() failed: %1": "UnInit() 失败：%1",
    "UnInit() returned code %1.": "UnInit() 返回错误码 %1。",
    "Resetting the target failed: %1": "复位目标失败：%1",
    "Done.": "完成。",

    # ------------------------------------------------------------- ThemeManager
    "Follow System(&S)": "跟随系统(&S)",
    "Light(&L)": "浅色(&L)",
    "Dark(&D)": "深色(&D)",
}


def fill(path, translator):
    """Fill every <translation> of the given .ts file using translator().

    Returns (missing, used): the source texts that had no translation, and the
    set of dictionary keys that were actually consumed.
    """
    tree = ET.parse(path)
    root = tree.getroot()
    missing = []
    used = set()

    for ctx in root:
        if ctx.tag != 'context':
            continue
        name_el = ctx.find('name')
        name = name_el.text if name_el is not None else '?'
        for msg in ctx.findall('message'):
            src = msg.find('source')
            if src is None:
                continue
            source = src.text or ''
            tr = msg.find('translation')
            if tr is None:
                tr = ET.SubElement(msg, 'translation')

            text = translator(source)
            if text is None:
                missing.append((name, source))
                tr.text = ''
                tr.set('type', 'unfinished')
            else:
                used.add(source)
                tr.attrib.pop('type', None)
                tr.text = text

    with open(path, 'wb') as f:
        f.write(b'<?xml version="1.0" encoding="utf-8"?>\n<!DOCTYPE TS>\n')
        tree.write(f, encoding='utf-8', xml_declaration=False)

    return missing, used


def main():
    root_dir = sys.argv[1] if len(sys.argv) > 1 else '.'
    base = os.path.join(root_dir, 'translations')

    missing, used = fill(os.path.join(base, 'app_zh_CN.ts'), lambda s: ZH.get(s))

    print('zh_CN: filled, missing %d' % len(missing))
    for ctx, src in missing:
        print('  [MISSING][%s] %s' % (ctx, src.replace('\n', '\\n')))

    unused = sorted(set(ZH) - used)
    if unused:
        print('  [UNUSED] %d dictionary entr(ies) not present in the sources:' % len(unused))
        for src in unused:
            print('    %s' % src.replace('\n', '\\n'))


if __name__ == '__main__':
    main()
