# -*- coding: utf-8 -*-
"""
Generate the Windows icon and the UI image from the source logo:

    resources/icons/logo.png        source logo (1080x1080 RGBA)
        -> resources/icons/logo.ico      multi size icon (Windows resource,
                                         embedded into the executable)
        -> resources/icons/logo_128.png  image used by the About dialog

Usage (requires Pillow):
    python tools/make_icon.py <project directory>
"""
import os
import sys

from PIL import Image

# Common Windows icon sizes (256 included, so Explorer large icons stay sharp)
SIZES = [(16, 16), (20, 20), (24, 24), (32, 32), (40, 40),
         (48, 48), (64, 64), (128, 128), (256, 256)]


def main(project_dir):
    icon_dir = os.path.join(project_dir, "resources", "icons")
    src = os.path.join(icon_dir, "logo.png")

    if not os.path.isfile(src):
        print("Source image not found: %s" % src)
        return 1

    img = Image.open(src).convert("RGBA")
    print("Source: %s (%dx%d %s)" % (src, img.width, img.height, img.mode))

    ico = os.path.join(icon_dir, "logo.ico")
    img.save(ico, format="ICO", sizes=SIZES)
    print("Written: %s (%d bytes, %d sizes)" % (ico, os.path.getsize(ico), len(SIZES)))

    preview = os.path.join(icon_dir, "logo_128.png")
    img.resize((128, 128), Image.LANCZOS).save(preview)
    print("Written: %s" % preview)

    return 0


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python tools/make_icon.py <project directory>")
        sys.exit(1)
    sys.exit(main(sys.argv[1]))
