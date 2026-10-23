<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>AlgoCatalog</name>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="453" />
        <source>Keil built-in algorithms</source>
        <translation>Keil 内置算法</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="459" />
        <source>Additional folder</source>
        <translation>附加目录</translation>
    </message>
    <message>
        <location filename="../src/flash/algocatalog.cpp" line="597" />
        <source>Flash Algorithms (*.flm *.FLM *.flx *.xldr);;All Files (*.*)</source>
        <translation>下载算法 (*.flm *.FLM *.flx *.xldr);;所有文件 (*.*)</translation>
    </message>
</context>
<context>
    <name>AlgorithmPicker</name>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="135" />
        <location filename="../src/algorithmpicker.cpp" line="380" />
        <source>Filter algorithms ...</source>
        <translation>筛选算法...</translation>
    </message>
    <message>
        <location filename="../src/algorithmpicker.cpp" line="368" />
        <source>Select algorithm ...</source>
        <translation>选择算法...</translation>
    </message>
</context>
<context>
    <name>CmsisDap</name>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="195" />
        <source>No probe is open.</source>
        <translation>没有打开的探针。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="206" />
        <source>The probe sent an empty answer.</source>
        <translation>探针返回了空应答。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="234" />
        <source>The probe answered with a malformed DAP_Info response.</source>
        <translation>探针对 DAP_Info 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="245" />
        <source>The probe sent a truncated DAP_Info response.</source>
        <translation>探针返回的 DAP_Info 应答被截断。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="299" />
        <source>The probe answered with a malformed DAP_Connect response.</source>
        <translation>探针对 DAP_Connect 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="305" />
        <source>The probe refused the SWD connection.</source>
        <translation>探针拒绝了 SWD 连接。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="425" />
        <source>Setting the SWD clock failed.</source>
        <translation>设置 SWD 时钟失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="448" />
        <source>Configuring the DAP transfer failed.</source>
        <translation>配置 DAP 传输失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="468" />
        <source>The probe answered with a malformed DAP_SWJ_Pins response.</source>
        <translation>探针对 DAP_SWJ_Pins 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="488" />
        <source>The probe answered with a malformed DAP_Reset_Target response.</source>
        <translation>探针对 DAP_Reset_Target 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="584" />
        <source>The SWJ sequence failed.</source>
        <translation>SWJ 序列执行失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="619" />
        <source>SWD clock: %1</source>
        <translation>SWD 时钟：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="624" />
        <source>The SWD clock could not be set: %1</source>
        <translation>无法设置 SWD 时钟：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="649" />
        <source>The SWD DP did not answer, trying to revive the link.</source>
        <translation>SWD DP 无应答，正在尝试重新建立连接。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="706" />
        <source>The SWD link came back at %1 Hz.</source>
        <translation>SWD 连接已恢复（%1 Hz）。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="725" />
        <source>The SWD DP does not answer. Check the wiring (SWDIO, SWCLK, GND), the target power supply and the SWD clock. A target program that switched the debug interface off, or that put the core into a low power state, only comes back on a power cycle. If it comes back with BOOT0 held high, that program is what takes the debug pins over: start the ROM bootloader that way, which leaves the debug pins alone, and erase the chip from there.</source>
        <translation>SWD DP 无应答。请检查接线（SWDIO、SWCLK、GND）、目标板供电以及 SWD 时钟。若目标程序关闭了调试接口，或让内核进入了低功耗状态，则只有重新上电才能恢复。如果拉高 BOOT0 后能连上，说明正是该程序占用了调试引脚：用这种方式启动 ROM 引导程序（它不会动调试引脚），再从那里整片擦除。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="764" />
        <source>Holding nRESET low and connecting under reset.</source>
        <translation>保持 nRESET 为低，改为在复位状态下连接。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="799" />
        <source>The SWD link came back under reset at %1 Hz.</source>
        <translation>SWD 连接已在复位状态下恢复（%1 Hz）。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="813" />
        <source>Connecting under reset did not bring the DP back either.</source>
        <translation>复位状态下连接同样没能唤回 DP。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="757" />
        <source>This probe does not drive an nRESET line.</source>
        <translation>该探针不驱动 nRESET 线。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="378" />
        <source>The DP did not acknowledge the power up request within %1 ms.</source>
        <translation>DP 未在 %1 毫秒内确认上电请求。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="818" />
        <source>The SWD DP did not answer while nRESET was being driven.</source>
        <translation>驱动 nRESET 期间 SWD DP 仍无应答。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="832" />
        <source>The SWD DP reported an invalid IDCODE.</source>
        <translation>SWD DP 返回了无效的 IDCODE。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="873" />
        <source>The DP read returned no data.</source>
        <translation>DP 读取没有返回数据。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="902" />
        <source>The AP read returned no data.</source>
        <translation>AP 读取没有返回数据。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="945" />
        <source>Empty transfer.</source>
        <translation>传输内容为空。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="959" />
        <source>Internal error: the write value count does not match the request count.</source>
        <translation>内部错误：写入值的数量与请求数量不一致。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="980" />
        <source>The probe answered with a truncated transfer response.</source>
        <translation>探针返回的传输应答被截断。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="990" />
        <source>The probe answered with a malformed DAP_Transfer response.</source>
        <translation>探针对 DAP_Transfer 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1009" />
        <source>SWD fault (the target did not acknowledge the access).</source>
        <translation>SWD 故障（目标未应答本次访问）。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1015" />
        <source>Protocol error during the SWD transfer.</source>
        <translation>SWD 传输过程中出现协议错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1021" />
        <source>The probe performed %1 of %2 transfers.</source>
        <translation>探针只完成了 %2 次传输中的 %1 次。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1030" />
        <source>The probe sent a truncated transfer payload.</source>
        <translation>探针返回的传输数据被截断。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1040" />
        <source>The SWD transfer was retried too often (WAIT response).</source>
        <translation>SWD 传输重试次数过多（WAIT 应答）。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1051" />
        <source>Empty block transfer.</source>
        <translation>块传输内容为空。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1078" />
        <source>The probe answered with a truncated block transfer response.</source>
        <translation>探针返回的块传输应答被截断。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1084" />
        <source>The probe answered with a malformed DAP_TransferBlock response.</source>
        <translation>探针对 DAP_TransferBlock 的应答格式错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1098" />
        <source>SWD fault during the block transfer.</source>
        <translation>块传输过程中发生 SWD 故障。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1104" />
        <source>Protocol error during the block transfer.</source>
        <translation>块传输过程中出现协议错误。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1110" />
        <source>The probe performed %1 of %2 block transfers.</source>
        <translation>探针只完成了 %2 次块传输中的 %1 次。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1119" />
        <source>The probe sent a truncated block transfer payload.</source>
        <translation>探针返回的块传输数据被截断。</translation>
    </message>
    <message>
        <location filename="../src/dap/cmsisdap.cpp" line="1297" />
        <source>The probe answered a poll with no data.</source>
        <translation>探针回应了轮询，但没有数据。</translation>
    </message>
</context>
<context>
    <name>CortexM</name>
    <message>
        <location filename="../src/target/cortexm.cpp" line="78" />
        <source>Unknown Cortex-M core</source>
        <translation>未知的 Cortex-M 内核</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="101" />
        <source>No Cortex-M core answered at CPUID (0x%1).</source>
        <translation>在 CPUID (0x%1) 处没有 Cortex-M 内核应答。</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="302" />
        <source>The core is locked up (the flash algorithm crashed).</source>
        <translation>内核已锁死（下载算法崩溃）。</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="320" />
        <source>The core did not halt within %1 ms.</source>
        <translation>内核在 %1 毫秒内没有暂停。</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="460" />
        <source>The probe has no nRESET line; use the software reset instead.</source>
        <translation>该探针没有 nRESET 线，请改用软件复位。</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="578" />
        <source>The core register transfer timed out.</source>
        <translation>内核寄存器传输超时。</translation>
    </message>
    <message>
        <location filename="../src/target/cortexm.cpp" line="585" />
        <source>Internal error: no register buffer.</source>
        <translation>内部错误：缺少寄存器缓冲区。</translation>
    </message>
</context>
<context>
    <name>DapTransport</name>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="123" />
        <source>unknown HID error</source>
        <translation>未知 HID 错误</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="169" />
        <source>The HID probe could not be opened. It is probably used by another program (Keil, OpenOCD, a debug session).</source>
        <translation>无法打开 HID 探针，它可能已被其他程序占用（Keil、OpenOCD 或调试会话）。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="206" />
        <location filename="../src/dap/probes.cpp" line="321" />
        <source>USB HID (CMSIS-DAP v1)</source>
        <translation>USB HID（CMSIS-DAP v1）</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="228" />
        <source>HID write failed: %1</source>
        <translation>HID 写入失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="249" />
        <source>HID read failed: %1</source>
        <translation>HID 读取失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="254" />
        <location filename="../src/dap/daptransport.cpp" line="588" />
        <source>The probe did not answer (timeout).</source>
        <translation>探针无应答（超时）。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="321" />
        <source>The USB (libusb) layer could not be initialised.</source>
        <translation>USB (libusb) 层初始化失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="328" />
        <source>USB enumeration failed.</source>
        <translation>USB 枚举失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="343" />
        <source>The USB probe was not found any more. Replug it and refresh the list.</source>
        <translation>已找不到该 USB 探针，请重新插拔后刷新列表。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="364" />
        <source>Access to the USB probe was denied. It is either used by another program, or no WinUSB driver is bound to it.</source>
        <translation>USB 探针访问被拒绝：要么已被其他程序占用，要么未绑定 WinUSB 驱动。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="369" />
        <source>The probe is not bound to a WinUSB (or libusbK) driver. Install the driver, or select the same probe over its USB HID (CMSIS-DAP v1) interface.</source>
        <translation>该探针未绑定 WinUSB（或 libusbK）驱动。请安装驱动，或改用它的 USB HID（CMSIS-DAP v1）接口选择同一个探针。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="517" />
        <source>CMSIS-DAP bulk transport, length-prefixed framing (2 byte prefix)</source>
        <translation>CMSIS-DAP 批量传输，长度前缀帧（2 字节前缀）</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="375" />
        <source>Opening the USB probe failed: %1</source>
        <translation>打开 USB 探针失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="387" />
        <source>Claiming the USB interface failed: %1</source>
        <translation>占用 USB 接口失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="512" />
        <source>CMSIS-DAP bulk transport, spec framing (no length prefix)</source>
        <translation>CMSIS-DAP 批量传输，规范帧（无长度前缀）</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="559" />
        <source>USB bulk write failed: %1</source>
        <translation>USB 批量写入失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="592" />
        <source>USB bulk read failed: %1</source>
        <translation>USB 批量读取失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="601" />
        <location filename="../src/dap/daptransport.cpp" line="611" />
        <source>The probe sent a malformed packet.</source>
        <translation>探针返回了格式错误的报文。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="683" />
        <source>The probe did not answer DAP_Info. Check that the right interface of the device was selected and that the firmware runs a CMSIS-DAP implementation.</source>
        <translation>探针未应答 DAP_Info。请确认已选择设备的正确接口，且其固件实现了 CMSIS-DAP。</translation>
    </message>
    <message>
        <location filename="../src/dap/daptransport.cpp" line="723" />
        <source>The command does not fit into one packet (%1 &gt; %2 bytes).</source>
        <translation>命令超出单个报文长度（%1 &gt; %2 字节）。</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="320" />
        <source>USB bulk (WinUSB, CMSIS-DAP v2)</source>
        <translation>USB 批量（WinUSB，CMSIS-DAP v2）</translation>
    </message>
</context>
<context>
    <name>ElfFile</name>
    <message>
        <location filename="../src/flash/elffile.cpp" line="61" />
        <source>The algorithm file could not be opened: %1</source>
        <translation>无法打开算法文件：%1</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="69" />
        <source>The file is too small to be an ELF image.</source>
        <translation>文件太小，不可能是 ELF 映像。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="74" />
        <source>The file is not an ELF image (a Keil .FLM algorithm is expected).</source>
        <translation>该文件不是 ELF 映像（这里需要 Keil .FLM 算法）。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="79" />
        <source>Only 32 bit ELF images are supported (this one is 64 bit).</source>
        <translation>仅支持 32 位 ELF 映像（该文件是 64 位）。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="84" />
        <source>Only little endian ELF images are supported.</source>
        <translation>仅支持小端 ELF 映像。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="90" />
        <source>The image is not an ARM image (e_machine = %1).</source>
        <translation>该映像不是 ARM 映像（e_machine = %1）。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="123" />
        <source>The image is truncated (segment %1 leaves the file).</source>
        <translation>映像被截断（第 %1 段超出文件末尾）。</translation>
    </message>
    <message>
        <location filename="../src/flash/elffile.cpp" line="133" />
        <source>The image carries no loadable segment.</source>
        <translation>映像中没有可加载的段。</translation>
    </message>
</context>
<context>
    <name>FlashAlgo</name>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="162" />
        <source>The algorithm does not export an Init() function.</source>
        <translation>该算法没有导出 Init() 函数。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="168" />
        <source>The algorithm does not export a ProgramPage() function.</source>
        <translation>该算法没有导出 ProgramPage() 函数。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="174" />
        <source>The algorithm does not export EraseSector() nor EraseChip().</source>
        <translation>该算法既没有导出 EraseSector() 也没有导出 EraseChip()。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="209" />
        <source>The FlashDevice structure could not be located in the algorithm.</source>
        <translation>无法在算法中定位 FlashDevice 结构。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="217" />
        <source>The FlashDevice structure at offset 0x%1 is not valid.</source>
        <translation>偏移 0x%1 处的 FlashDevice 结构无效。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="300" />
        <source>The algorithm carries an empty sector table.</source>
        <translation>该算法的扇区表为空。</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="339" />
        <location filename="../src/flash/flashalgo.cpp" line="347" />
        <source>Unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="340" />
        <source>On-chip flash</source>
        <translation>片内 Flash</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="341" />
        <source>External flash, 8 bit bus</source>
        <translation>片外 Flash，8 位总线</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="342" />
        <source>External flash, 16 bit bus</source>
        <translation>片外 Flash，16 位总线</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="343" />
        <source>External flash, 32 bit bus</source>
        <translation>片外 Flash，32 位总线</translation>
    </message>
    <message>
        <location filename="../src/flash/flashalgo.cpp" line="344" />
        <source>External SPI flash</source>
        <translation>片外 SPI Flash</translation>
    </message>
</context>
<context>
    <name>FlashWorker</name>
    <message>
        <location filename="../src/flashworker.cpp" line="72" />
        <source>Loading the flash algorithm ...</source>
        <translation>正在加载下载算法 ...</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="81" />
        <source>Algorithm %1: %2, %3, page %4 bytes, %5 sectors</source>
        <translation>算法 %1：%2，%3，页 %4 字节，%5 个扇区</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="104" />
        <source>%1: %2 in %3 blocks, 0x%4 - 0x%5, checksum 0x%6</source>
        <translation>%1：%2（%3 个数据块），0x%4 - 0x%5，校验和 0x%6</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="105" />
        <source>Intel HEX</source>
        <translation>Intel HEX</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="105" />
        <source>BIN</source>
        <translation>BIN</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="112" />
        <source>Firmware %1</source>
        <translation>固件 %1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="117" />
        <source>The firmware (0x%1 - 0x%2) does not fit into the flash of the algorithm (0x%3 - 0x%4). Check the loading address.</source>
        <translation>固件（0x%1 - 0x%2）超出了算法对应的 Flash 范围（0x%3 - 0x%4）。请检查烧录地址。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="127" />
        <source>The algorithm has no sector at 0x%1.</source>
        <translation>该算法在 0x%1 处没有对应的扇区。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="132" />
        <source>No firmware given: the whole chip is erased and the target is left halted.</source>
        <translation>未提供固件：将整片擦除，并把目标保持在内核暂停状态。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="137" />
        <location filename="../src/flashworker.cpp" line="244" />
        <location filename="../src/flashworker.cpp" line="245" />
        <source>Cancelled.</source>
        <translation>已取消。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="155" />
        <source>Probe: %1</source>
        <translation>探针：%1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="159" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>CMSIS-DAP 固件版本：%1</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="183" />
        <source>Target: %1 (CPUID 0x%2, DP IDCODE 0x%3)</source>
        <translation>目标：%1（CPUID 0x%2，DP IDCODE 0x%3）</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="254" />
        <source>The whole chip was erased in %1 s; the target is left halted.</source>
        <translation>整片擦除完成，耗时 %1 秒；目标保持在内核暂停状态。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="257" />
        <source>Programming finished in %1 s: %2 pages, %3 bytes, %4 sectors erased.</source>
        <translation>烧录完成，耗时 %1 秒：%2 页、%3 字节，擦除 %4 个扇区。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="264" />
        <source>The algorithm used 0x%1 of RAM at %2.</source>
        <translation>该算法占用了 %2 处的 0x%1 字节 RAM。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="300" />
        <source>The SWD link failed at %1; retrying at %2.</source>
        <translation>SWD 链路在 %1 下失败，改用 %2 重试。</translation>
    </message>
    <message>
        <location filename="../src/flashworker.cpp" line="306" />
        <source>Retrying at a slower clock ...</source>
        <translation>正在以更低的时钟重试 ...</translation>
    </message>
</context>
<context>
    <name>Flasher</name>
    <message>
        <location filename="../src/flash/flasher.cpp" line="110" />
        <source>The algorithm image is empty.</source>
        <translation>算法映像为空。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="116" />
        <source>The algorithm reports an unusable page size (%1 bytes).</source>
        <translation>算法报告的页大小不可用（%1 字节）。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="128" />
        <source>The RAM base address is too high for the algorithm.</source>
        <translation>RAM 基地址对该算法来说过高。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="230" />
        <source>The algorithm could not be written to the target RAM at %1: %2 of %3 bytes came back different, the first one at %4 (wrote 0x%5, read 0x%6). The base address has to be RAM the debug unit can write to; if it is, the probe did not move the data reliably.</source>
        <translation>无法把算法写入目标 RAM 的 %1 处：回读后有 %2/%3 字节不一致，第一个不一致在 %4（写入 0x%5，读回 0x%6）。该基地址必须是调试单元可写的 RAM；如果确实是 RAM，那就是探针没有可靠地搬运数据。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="260" />
        <source>The algorithm does not provide %1().</source>
        <translation>该算法没有提供 %1()。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="293" />
        <source>%1() did not return: %2 (PC 0x%3, DFSR 0x%4, CFSR 0x%5, HFSR 0x%6)</source>
        <translation>%1() 没有正常返回：%2（PC 0x%3，DFSR 0x%4，CFSR 0x%5，HFSR 0x%6）</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="313" />
        <source>%1() stopped at 0x%2 instead of returning to 0x%3 (DFSR = 0x%4).</source>
        <translation>%1() 停在了 0x%2，而没有返回 0x%3（DFSR = 0x%4）。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="340" />
        <source>Erasing the whole chip ...</source>
        <translation>正在整片擦除 ...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="351" />
        <source>EraseChip() failed with code %1.</source>
        <translation>EraseChip() 失败，错误码 %1。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="356" />
        <source>Chip erase done.</source>
        <translation>整片擦除完成。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="361" />
        <source>The algorithm provides neither EraseChip() nor EraseSector().</source>
        <translation>该算法既没有提供 EraseChip() 也没有提供 EraseSector()。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="376" />
        <source>Address 0x%1 is not covered by the sector table of the algorithm.</source>
        <translation>地址 0x%1 不在该算法的扇区表范围内。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="396" />
        <source>Nothing to erase.</source>
        <translation>没有需要擦除的内容。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="402" />
        <location filename="../src/flash/flasher.cpp" line="471" />
        <location filename="../src/flash/flasher.cpp" line="585" />
        <source>Cancelled.</source>
        <translation>已取消。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="411" />
        <source>Erasing sector %1 of %2 (0x%3) ...</source>
        <translation>正在擦除第 %1/%2 个扇区（0x%3）...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="425" />
        <source>Erasing the sector at 0x%1 failed with code %2.</source>
        <translation>擦除 0x%1 处的扇区失败，错误码 %2。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="455" />
        <source>The firmware file carries no data.</source>
        <translation>固件文件中没有数据。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="507" />
        <source>Programming 0x%1 (%2 / %3 bytes) ...</source>
        <translation>正在烧录 0x%1（%2 / %3 字节）...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="533" />
        <source>Programming the page at 0x%1 failed with code %2.</source>
        <translation>烧录 0x%1 处的页失败，错误码 %2。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="569" />
        <source>The algorithm has no Verify() routine; the flash is read back through the debug port instead.</source>
        <translation>该算法没有 Verify() 例程，将改由调试口回读 Flash 进行校验。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="605" />
        <source>Verifying 0x%1 ...</source>
        <translation>正在校验 0x%1 ...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="622" />
        <source>Verification failed at 0x%1 (code %2).</source>
        <translation>校验失败于 0x%1（错误码 %2）。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="630" />
        <source>Reading back 0x%1 failed; the verification was skipped.</source>
        <translation>回读 0x%1 失败，已跳过校验。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="644" />
        <source>Verification failed at 0x%1: expected 0x%2, read 0x%3.</source>
        <translation>校验失败于 0x%1：期望 0x%2，实际读到 0x%3。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="671" />
        <source>No flash algorithm is selected.</source>
        <translation>尚未选择下载算法。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="681" />
        <source>No firmware data to program.</source>
        <translation>没有需要烧录的固件数据。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="693" />
        <source>Preparing ...</source>
        <translation>准备中 ...</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="696" />
        <source>Algorithm: %1 (%2, %3 bytes)</source>
        <translation>算法：%1（%2，%3 字节）</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="738" />
        <source>RAM transfer: %1 words (%2 bytes) per DAP_TransferBlock, command packet %3 bytes.</source>
        <translation>RAM 传输：每次 DAP_TransferBlock 传送 %1 个字（%2 字节），命令包 %3 字节。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="751" />
        <source>Writing the algorithm to the RAM failed, attempt %1 of %2: %3</source>
        <translation>把算法写入 RAM 失败，第 %1/%2 次尝试：%3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="763" />
        <source>Algorithm loaded at 0x%1 (%2 bytes of RAM).</source>
        <translation>算法已加载到 0x%1（占用 %2 字节 RAM）。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="782" />
        <source>Init() did not complete, restarting the algorithm (%1 of %2): %3</source>
        <translation>Init() 未完成，正在重启算法（第 %1 次，共 %2 次）：%3</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="807" />
        <source>Init() failed with code %1.</source>
        <translation>Init() 失败，错误码 %1。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="816" />
        <source>Initialised.</source>
        <translation>初始化完成。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="854" />
        <source>UnInit() failed: %1</source>
        <translation>UnInit() 失败：%1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="856" />
        <source>UnInit() returned code %1.</source>
        <translation>UnInit() 返回错误码 %1。</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="865" />
        <source>Resetting the target failed: %1</source>
        <translation>复位目标失败：%1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="729" />
        <location filename="../src/flash/flasher.cpp" line="867" />
        <source>Reset: %1</source>
        <translation>复位方式：%1</translation>
    </message>
    <message>
        <location filename="../src/flash/flasher.cpp" line="874" />
        <source>Done.</source>
        <translation>完成。</translation>
    </message>
</context>
<context>
    <name>LanguageManager</name>
    <message>
        <location filename="../src/languagemanager.cpp" line="129" />
        <source>Follow System(&amp;A)</source>
        <translation>跟随系统(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="130" />
        <source>English(&amp;E)</source>
        <translation>English(&amp;E)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="131" />
        <source>Simplified Chinese(&amp;C)</source>
        <translation>简体中文(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="132" />
        <source>Russian(&amp;R)</source>
        <translation>俄语(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="133" />
        <source>German(&amp;G)</source>
        <translation>德语(&amp;G)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="134" />
        <source>Spanish(&amp;S)</source>
        <translation>西班牙语(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="178" />
        <source>Simplified Chinese</source>
        <translation>简体中文</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="180" />
        <source>Russian</source>
        <translation>俄语</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="182" />
        <source>German</source>
        <translation>德语</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="184" />
        <source>Spanish</source>
        <translation>西班牙语</translation>
    </message>
    <message>
        <location filename="../src/languagemanager.cpp" line="187" />
        <source>English</source>
        <translation>English</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../src/mainwindow.cpp" line="479" />
        <source>WeAct Studio DAP Flasher</source>
        <translation>WeAct Studio DAP 烧录工具</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="481" />
        <source>Debug Probe</source>
        <translation>调试探针</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="482" />
        <source>Flash Algorithm</source>
        <translation>下载算法</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="483" />
        <source>Firmware File</source>
        <translation>固件文件</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="484" />
        <source>Options</source>
        <translation>选项</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="486" />
        <source>Probe:</source>
        <translation>探针：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="487" />
        <source>Connection:</source>
        <translation>连接状态：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="488" />
        <source>Target:</source>
        <translation>目标：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="489" />
        <source>Algorithm File:</source>
        <translation>算法文件：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="490" />
        <source>File Path:</source>
        <translation>文件路径：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="491" />
        <source>Firmware:</source>
        <translation>固件：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="492" />
        <source>BIN Load Address: 0x</source>
        <translation>BIN 烧录地址：0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="493" />
        <source>SWD Clock:</source>
        <translation>SWD 时钟：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="494" />
        <source>Algorithm RAM: 0x</source>
        <translation>算法 RAM：0x</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="495" />
        <source>Reset Mode:</source>
        <translation>复位模式：</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="497" />
        <source>Refresh(&amp;R)</source>
        <translation>刷新(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="498" />
        <location filename="../src/mainwindow.cpp" line="499" />
        <source>Browse(&amp;B)...</source>
        <translation>浏览(&amp;B)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="501" />
        <source>Whole chip erase</source>
        <translation>整片擦除</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="502" />
        <source>Verify</source>
        <translation>校验</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="503" />
        <source>Reset and run</source>
        <translation>复位并运行</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Cancel(&amp;C)</source>
        <translation>取消(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="513" />
        <source>Start Programming(&amp;S)</source>
        <translation>开始烧录(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="514" />
        <source>Halt(&amp;P)</source>
        <translation>暂停(&amp;P)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="515" />
        <source>Run(&amp;R)</source>
        <translation>运行(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="516" />
        <source>Reset(&amp;T)</source>
        <translation>复位(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="517" />
        <location filename="../src/mainwindow.cpp" line="536" />
        <source>Exit(&amp;X)</source>
        <translation>退出(&amp;X)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="519" />
        <source>Select or drop a .hex / .bin firmware file</source>
        <translation>选择或直接拖入 .hex / .bin 固件文件</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="523" />
        <source>RAM area the flash algorithm is copied to while it runs. 0x20000000 is correct for most Cortex-M devices.</source>
        <translation>下载算法运行时被复制到的 RAM 区域。大多数 Cortex-M 器件填 0x20000000 即可。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="525" />
        <source>Loading address used for .bin files (ignored for .hex)</source>
        <translation>.bin 文件使用的烧录起始地址（.hex 忽略此项）</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="526" />
        <source>Reset used by the Reset button and by "Reset and run". Software reset needs no extra wire, hardware reset pulses the nRESET line, automatic uses nRESET when the probe has one.</source>
        <translation>复位按钮与「复位并运行」使用的复位方式。软件复位不需要额外接线；硬件复位拉低 nRESET 线；自动在探针有 nRESET 线时使用它。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="530" />
        <source>Log</source>
        <translation>日志</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="533" />
        <source>File(&amp;F)</source>
        <translation>文件(&amp;F)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="534" />
        <source>Open Firmware(&amp;O)...</source>
        <translation>打开固件(&amp;O)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="535" />
        <source>Select Algorithm(&amp;A)...</source>
        <translation>选择算法(&amp;A)...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="538" />
        <source>Tools(&amp;T)</source>
        <translation>工具(&amp;T)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="539" />
        <source>Refresh Probes(&amp;R)</source>
        <translation>刷新探针(&amp;R)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="540" />
        <source>Rescan Algorithms(&amp;L)</source>
        <translation>重新扫描算法(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="541" />
        <source>Show All USB HID Devices(&amp;H)</source>
        <translation>显示所有 USB HID 设备(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="542" />
        <source>Clear Log(&amp;C)</source>
        <translation>清空日志(&amp;C)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="544" />
        <source>View(&amp;V)</source>
        <translation>视图(&amp;V)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="545" />
        <source>Language(&amp;L)</source>
        <translation>语言(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="546" />
        <source>Help(&amp;H)</source>
        <translation>帮助(&amp;H)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="547" />
        <source>About(&amp;A)</source>
        <translation>关于(&amp;A)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="713" />
        <source>(no probe found)</source>
        <translation>（未发现探针）</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="792" />
        <source>Identifying the target over %1 ...</source>
        <translation>正在通过 %1 识别目标 ...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="863" />
        <source>&lt;font color='%1'&gt;Identifying the target ...&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;正在识别目标 ...&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="930" />
        <source>&lt;font color='%1'&gt;No probe found&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;未发现探针&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="931" />
        <source>Connect a CMSIS-DAP probe and press Refresh.</source>
        <translation>请连接 CMSIS-DAP 探针，然后点击“刷新”。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="865" />
        <location filename="../src/mainwindow.cpp" line="932" />
        <location filename="../src/mainwindow.cpp" line="958" />
        <source>-</source>
        <translation>-</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="939" />
        <source>&lt;font color='%1'&gt;Connected (%2)&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;已连接（%2）&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="941" />
        <source>SWD</source>
        <translation>SWD</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="943" />
        <source>CMSIS-DAP firmware: %1</source>
        <translation>CMSIS-DAP 固件版本：%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="944" />
        <source>unknown</source>
        <translation>未知</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="949" />
        <source>CPUID %1, DP IDCODE %2, core %3</source>
        <translation>CPUID %1，DP IDCODE %2，内核 %3</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>halted</source>
        <translation>已暂停</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="952" />
        <source>running</source>
        <translation>运行中</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="955" />
        <source>&lt;font color='%1'&gt;Probe present, target not identified&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;探针已连接，但未能识别目标&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1251" />
        <source>Select a Keil Flash Algorithm</source>
        <translation>选择 Keil 下载算法</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1380" />
        <source>%1  Size: %2 in %3 blocks  Address: 0x%4 - 0x%5  Checksum: 0x%6</source>
        <translation>%1　大小：%2（%3 个数据块）　地址：0x%4 - 0x%5　校验和：0x%6</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1025" />
        <source>Scanning flash algorithms (%1 pack folders) ...</source>
        <translation>正在扫描下载算法（%1 个 pack 目录）...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1066" />
        <source>%1 flash algorithms found in %2 groups.</source>
        <translation>共找到 %1 个下载算法，分布在 %2 个分组中。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1071" />
        <source>%1 file(s) were skipped: they carry no usable FlashDevice structure.</source>
        <translation>已跳过 %1 个文件：其中没有可用的 FlashDevice 结构。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1179" />
        <source>Additional folder: %1</source>
        <translation>附加目录：%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1158" />
        <source>Recently used</source>
        <translation>最近使用</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1206" />
        <source>Last used</source>
        <translation>上次使用</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1287" />
        <source>The algorithm %1 could not be loaded: %2</source>
        <translation>无法加载算法 %1：%2</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1290" />
        <source>Algorithm loaded: %1 (%2)</source>
        <translation>已加载算法：%1（%2）</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1301" />
        <location filename="../src/mainwindow.cpp" line="1371" />
        <source>&lt;font color='%1'&gt;%2&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;%2&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1308" />
        <source>&lt;font color='%1'&gt;No algorithm selected&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;尚未选择算法&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1314" />
        <source>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2, flash 0x%3 - 0x%4 (%5), page %6 bytes, %7 sectors</source>
        <translation>&lt;b&gt;%1&lt;/b&gt;&lt;br&gt;%2，Flash 0x%3 - 0x%4（%5），页 %6 字节，%7 个扇区</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1324" />
        <source>&lt;br&gt;&lt;font color='%1'&gt;This algorithm has no chip erase; the sectors are erased instead.&lt;/font&gt;</source>
        <translation>&lt;br&gt;&lt;font color='%1'&gt;该算法不支持整片擦除，将改为按扇区擦除。&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1342" />
        <source>Select Firmware File</source>
        <translation>选择固件文件</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1344" />
        <source>Firmware Files (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;Binary Files (*.bin);;All Files (*.*)</source>
        <translation>固件文件 (*.hex *.bin *.ihex);;Intel HEX (*.hex *.ihex);;二进制文件 (*.bin);;所有文件 (*.*)</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1362" />
        <source>&lt;font color='%1'&gt;No valid firmware file selected&lt;/font&gt;</source>
        <translation>&lt;font color='%1'&gt;尚未选择有效的固件文件&lt;/font&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1381" />
        <source>Intel HEX</source>
        <translation>Intel HEX</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1381" />
        <source>BIN</source>
        <translation>BIN</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1445" />
        <location filename="../src/mainwindow.cpp" line="1480" />
        <location filename="../src/mainwindow.cpp" line="1506" />
        <location filename="../src/mainwindow.cpp" line="1567" />
        <source>No Probe</source>
        <translation>未发现探针</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1445" />
        <location filename="../src/mainwindow.cpp" line="1480" />
        <location filename="../src/mainwindow.cpp" line="1506" />
        <source>Select a debug probe first.</source>
        <translation>请先选择一个调试探针。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1465" />
        <source>Halt Failed</source>
        <translation>暂停失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1469" />
        <source>The core is halted.</source>
        <translation>内核已暂停。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1491" />
        <source>Run Failed</source>
        <translation>运行失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1495" />
        <source>The core is running.</source>
        <translation>内核正在运行。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1517" />
        <source>Reset Failed</source>
        <translation>复位失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1521" />
        <source>The target was reset and is running.</source>
        <translation>目标已复位并开始运行。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1568" />
        <source>Select a CMSIS-DAP probe first.</source>
        <translation>请先选择一个 CMSIS-DAP 探针。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1573" />
        <source>No Algorithm</source>
        <translation>未选择算法</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1574" />
        <source>Select the Keil .FLM flash algorithm that matches the device.</source>
        <translation>请选择与目标器件匹配的 Keil .FLM 下载算法。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1580" />
        <source>No Firmware</source>
        <translation>未选择固件</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1581" />
        <source>Select a valid firmware file first.</source>
        <translation>请先选择一个有效的固件文件。</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1602" />
        <source>Preparing ...</source>
        <translation>准备中 ...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1627" />
        <source>Cancelling ...</source>
        <translation>正在取消 ...</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1650" />
        <location filename="../src/mainwindow.cpp" line="1687" />
        <source>Programming complete</source>
        <translation>烧录完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1656" />
        <source>Complete</source>
        <translation>完成</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1657" />
        <source>The firmware was programmed successfully.

%1</source>
        <translation>固件烧录成功。

%1</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1660" />
        <source>Cancelled</source>
        <translation>已取消</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1664" />
        <location filename="../src/mainwindow.cpp" line="1690" />
        <source>Programming failed</source>
        <translation>烧录失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1666" />
        <source>Failed</source>
        <translation>失败</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1694" />
        <source>Ready</source>
        <translation>就绪</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1761" />
        <source>About</source>
        <translation>关于</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1765" />
        <source>&lt;h3&gt;WeAct Studio DAP Flasher&lt;/h3&gt;&lt;p&gt;Version: %1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio. All rights reserved.&lt;/p&gt;&lt;p&gt;Programs Cortex-M devices through a CMSIS-DAP probe (USB HID or USB bulk / WinUSB) using the Keil .FLM flash algorithms.&lt;/p&gt;&lt;p&gt;This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. It is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.&lt;/p&gt;&lt;p&gt;License: GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;Built with Qt %2, statically compiled with MSYS2 (MINGW64).&lt;/p&gt;</source>
        <translation>&lt;h3&gt;WeAct Studio DAP 烧录工具&lt;/h3&gt;&lt;p&gt;版本：%1&lt;/p&gt;&lt;p&gt;&lt;b&gt;WeAct Studio&lt;/b&gt;&lt;br&gt;Copyright (C) 2026 WeAct Studio 保留所有权利。&lt;/p&gt;&lt;p&gt;通过 CMSIS-DAP 探针（USB HID 或 USB 批量 / WinUSB），使用 Keil .FLM 下载算法烧录 Cortex-M 器件。&lt;/p&gt;&lt;p&gt;本程序为自由软件：您可以依据自由软件基金会发布的 GNU 通用公共许可证第 3 版或（按您选择的）任何更新版本的条款重新发布和修改本程序。发布本程序是希望它能有所用处，但不提供任何担保，甚至不隐含适销性或特定用途适用性的担保。详见 GNU 通用公共许可证。&lt;/p&gt;&lt;p&gt;许可协议：GPL-3.0-or-later&lt;br&gt;&lt;a href='https://www.gnu.org/licenses/gpl-3.0.html'&gt;https://www.gnu.org/licenses/gpl-3.0.html&lt;/a&gt;&lt;/p&gt;&lt;p&gt;基于 Qt %2 开发，使用 MSYS2 (MINGW64) 静态编译。&lt;/p&gt;</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1821" />
        <source>Programming in Progress</source>
        <translation>正在烧录</translation>
    </message>
    <message>
        <location filename="../src/mainwindow.cpp" line="1822" />
        <source>Programming is still running; stopping now can leave the device with an incomplete firmware.

Exit anyway?</source>
        <translation>烧录仍在进行中，现在退出可能使设备固件不完整。

仍要退出吗？</translation>
    </message>
</context>
<context>
    <name>Probes</name>
    <message>
        <location filename="../src/dap/probes.cpp" line="73" />
        <source>The HID interface could not be initialised.</source>
        <translation>HID 接口初始化失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="82" />
        <source>HID init failed: %1</source>
        <translation>HID 初始化失败：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="83" />
        <source>HID init failed.</source>
        <translation>HID 初始化失败。</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="287" />
        <source>HID device</source>
        <translation>HID 设备</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="307" />
        <source> (in use)</source>
        <translation>（被占用）</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="318" />
        <source>Transport: %1</source>
        <translation>传输方式：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="324" />
        <source>Product: %1</source>
        <translation>产品名称：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="326" />
        <source>Manufacturer: %1</source>
        <translation>厂商：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="328" />
        <source>Serial: %1</source>
        <translation>序列号：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="331" />
        <source>USB: bus %1 address %2, interface %3, EP IN 0x%4 / OUT 0x%5</source>
        <translation>USB：总线 %1 地址 %2，接口 %3，端点 IN 0x%4 / OUT 0x%5</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="339" />
        <source>Interface: %1</source>
        <translation>接口：%1</translation>
    </message>
    <message>
        <location filename="../src/dap/probes.cpp" line="340" />
        <source>Usage page: 0x%1  Usage: 0x%2</source>
        <translation>Usage Page：0x%1　Usage：0x%2</translation>
    </message>
</context>
<context>
    <name>ResetMode</name>
    <message>
        <location filename="../src/target/resetmode.cpp" line="18" />
        <source>Hardware reset</source>
        <translation>硬件复位</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="20" />
        <source>Automatic</source>
        <translation>自动（优先硬件复位）</translation>
    </message>
    <message>
        <location filename="../src/target/resetmode.cpp" line="25" />
        <source>Software reset</source>
        <translation>软件复位</translation>
    </message>
</context>
<context>
    <name>TargetSession</name>
    <message>
        <location filename="../src/targetsession.cpp" line="30" />
        <source>Probe: %1</source>
        <translation>探针：%1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="83" />
        <source>The target program was holding the debug access; the core is now halted under reset.</source>
        <translation>目标程序占住了调试访问；现已在内核复位状态下将其暂停。</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="191" />
        <source>Reset: %1</source>
        <translation>复位方式：%1</translation>
    </message>
    <message>
        <location filename="../src/targetsession.cpp" line="225" />
        <source>Reset: %1 (the DP did not answer, so this was a pin reset only).</source>
        <translation>复位方式：%1（DP 无应答，因此只脉冲了复位线）。</translation>
    </message>
</context>
<context>
    <name>ThemeManager</name>
    <message>
        <location filename="../src/thememanager.cpp" line="166" />
        <source>Follow System(&amp;S)</source>
        <translation>跟随系统(&amp;S)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="168" />
        <source>Light(&amp;L)</source>
        <translation>浅色(&amp;L)</translation>
    </message>
    <message>
        <location filename="../src/thememanager.cpp" line="171" />
        <source>Dark(&amp;D)</source>
        <translation>深色(&amp;D)</translation>
    </message>
</context>
</TS>