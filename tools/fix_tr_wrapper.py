# -*- coding: utf-8 -*-
"""
Expand the local tr_() helper into explicit QCoreApplication::translate() calls.

lupdate only extracts strings that are passed to translate() *directly* as a
literal, so every message that goes through a wrapper function like

    QString tr_(const char *text) { return QCoreApplication::translate("X", text); }

is invisible to it and never reaches the .ts file. This script rewrites the
call sites, so lupdate sees them again.

Usage:
    python tools/fix_tr_wrapper.py <project directory> [--dry-run]
"""
import os
import re
import sys

# file -> context used by the old tr_() helper
FILES = {
    'src/dap/cmsisdap.cpp': 'CmsisDap',
    'src/flash/elffile.cpp': 'ElfFile',
    'src/flash/flashalgo.cpp': 'FlashAlgo',
    'src/flash/flasher.cpp': 'Flasher',
    'src/flashworker.cpp': 'FlashWorker',
    'src/target/cortexm.cpp': 'CortexM',
    'src/targetsession.cpp': 'TargetSession',
}

# The definition itself is dropped, the calls carry the context now
DEF_RE = re.compile(
    r'QString tr_\(const char \*text\)\s*\{\s*'
    r'return QCoreApplication::translate\("[^"]+", text\);\s*\}',
    re.S)


def find_call_end(text, open_idx):
    """Return the index just past the ')' that closes the '(' at open_idx.

    String and character literals are skipped, so parentheses inside a message
    (and escaped quotes) do not confuse the scan.
    """
    depth = 0
    i = open_idx
    n = len(text)
    while i < n:
        c = text[i]
        if c == '"':
            i += 1
            while i < n:
                if text[i] == '\\':
                    i += 2
                    continue
                if text[i] == '"':
                    break
                i += 1
        elif c == "'":
            i += 1
            while i < n:
                if text[i] == '\\':
                    i += 2
                    continue
                if text[i] == "'":
                    break
                i += 1
        elif c == '(':
            depth += 1
        elif c == ')':
            depth -= 1
            if depth == 0:
                return i + 1
        i += 1
    raise ValueError('unbalanced parentheses')


def convert(path, context, dry_run):
    with open(path, encoding='utf-8') as f:
        text = f.read()

    text, n_def = DEF_RE.subn('', text)
    if n_def != 1:
        print('  ! %s: helper definition not found (%d)' % (path, n_def))
        return 0

    count = 0
    while True:
        idx = text.find('tr_(')
        if idx < 0:
            break
        # tr_ is never a suffix of a longer identifier here
        end = find_call_end(text, idx + 3)
        inner = text[idx + 4:end - 1]
        replacement = 'QCoreApplication::translate("%s", %s)' % (context, inner)
        text = text[:idx] + replacement + text[end:]
        count += 1

    # drop the now blank line the removed definition left behind
    text = re.sub(r'\n{3,}', '\n\n', text)

    if not dry_run:
        with open(path, 'w', encoding='utf-8', newline='') as f:
            f.write(text)
    return count


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    dry_run = '--dry-run' in sys.argv
    root = args[0] if args else '.'

    total = 0
    for rel, context in FILES.items():
        path = os.path.join(root, rel)
        if not os.path.isfile(path):
            print('  ? %s: missing' % rel)
            continue
        n = convert(path, context, dry_run)
        total += n
        print('  %-30s %-14s %3d call(s)' % (rel, context, n))

    print('total: %d call(s)%s' % (total, ' (dry run)' if dry_run else ''))


if __name__ == '__main__':
    main()
