# ============================================================================
#  MSYS2 静态 Qt 构建的公共设置（主程序与单元测试共用）
#
#  MSYS2 上用静态 Qt（qt5-static / qt6-static）链接时有两个坑：
#
#  1) 路径：MSYS 的 POSIX 路径（/mingw64）交给 Windows 原生的 g++ 后会被当成
#     “当前盘符下的相对路径”，头文件/库就找不到了。所以统一用 Windows 风格路径。
#
#  2) Qt6 静态库的链接顺序：harfbuzz 的依赖（graphite2 / usp10 / rpcrt4）、
#     freetype 的依赖（bz2）、glib 的依赖（iconv）都被排在了消费者“前面”，
#     而静态链接从左到右只扫描一遍，于是这些符号无法回头解析。
#     解决办法是用 -Wl,-u 把需要的符号提前置为 undefined，
#     等链接器扫到对应库时就会把目标文件拉进来（比 --whole-archive 省体积）。
#
#  用法：include($$PWD/qt6_static_link.pri)
# ============================================================================

# ---------------------------------------------------------------------------
#  MSYS2 前缀（Windows 风格路径）
#    1) 环境变量 MSYS_PREFIX_WIN（build.sh 会自动导出）
#    2) 调用 cygpath 转换
#    3) 回退到默认安装位置 D:/msys64/mingw64
# ---------------------------------------------------------------------------
MSYS_PREFIX = $$(MSYSTEM_PREFIX)
isEmpty(MSYS_PREFIX): MSYS_PREFIX = /mingw64

MSYS_PREFIX_WIN = $$(MSYS_PREFIX_WIN)
isEmpty(MSYS_PREFIX_WIN): MSYS_PREFIX_WIN = $$system(cygpath -m $$MSYS_PREFIX 2>/dev/null)
isEmpty(MSYS_PREFIX_WIN) {
    exists(D:/msys64/mingw64/include/hidapi/hidapi.h): MSYS_PREFIX_WIN = D:/msys64/mingw64
    else: MSYS_PREFIX_WIN = $$MSYS_PREFIX
}

# Qt5 静态编译需要大对象支持
QMAKE_CXXFLAGS += -Wa,-mbig-obj
QMAKE_CFLAGS   += -Wa,-mbig-obj

greaterThan(QT_MAJOR_VERSION, 5) {
    LIBS += -Wl,-u,gr_make_face_with_ops -Wl,-u,gr_face_destroy \
            -Wl,-u,gr_face_featureval_for_lang -Wl,-u,gr_face_find_fref \
            -Wl,-u,gr_featureval_destroy -Wl,-u,gr_fref_set_feature_value \
            -Wl,-u,gr_make_seg -Wl,-u,gr_seg_advance_X -Wl,-u,gr_seg_destroy \
            -Wl,-u,gr_seg_first_slot -Wl,-u,gr_seg_n_slots -Wl,-u,gr_slot_advance_Y \
            -Wl,-u,gr_slot_after -Wl,-u,gr_slot_before -Wl,-u,gr_slot_can_insert_before \
            -Wl,-u,gr_slot_gid -Wl,-u,gr_slot_next_in_segment \
            -Wl,-u,gr_slot_origin_X -Wl,-u,gr_slot_origin_Y
    LIBS += -Wl,-u,ScriptItemize -Wl,-u,ScriptPlace -Wl,-u,ScriptShape -Wl,-u,ScriptFreeCache
    LIBS += -Wl,-u,__imp_UuidCreate
    LIBS += -Wl,-u,BZ2_bzDecompressInit -Wl,-u,BZ2_bzDecompress -Wl,-u,BZ2_bzDecompressEnd
    LIBS += -Wl,-u,libiconv_open -Wl,-u,libiconv -Wl,-u,libiconv_close
    LIBS += -liconv
}
