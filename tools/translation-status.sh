#!/bin/sh
# 检查英文版是否跟上了中文版。
#
# 每个英文文件的开头有一行，记下它是从哪个版本的中文翻译过来的：
#     % 译自: chapters/zh/ch01.tex @ 12b16dd:chapters/ch01.tex
# “@”前面是现在的中文文件，后面是翻译时那个中文文件在 git 里的版本（提交:路径）。
# 这个脚本把两者比较一下：一样，说明英文版是最新的；不一样，说明中文版后来改过，英文版要跟着改。
# 改完英文版以后，先提交中文版，再把“@”后面换成新的版本，例如 @ a1b2c3d:chapters/zh/ch01.tex
#
# 用法：sh tools/translation-status.sh        列出每个英文文件的状态
#       sh tools/translation-status.sh -d     同时显示中文版自翻译以来的改动
cd "$(dirname "$0")/.." || exit 1
show=0
[ "$1" = "-d" ] && show=1
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
stale=0

for en in $(find chapters/en backmatter/en frontmatter/en -name '*.tex' 2>/dev/null | sort); do
    line=$(grep -m1 '^% 译自:' "$en")
    if [ -z "$line" ]; then
        echo "??  $en：开头没有“% 译自:”一行"
        stale=1
        continue
    fi
    zh=$(echo "$line" | sed 's/^% 译自: *\([^ ]*\) *@.*/\1/')
    src=$(echo "$line" | sed 's/.*@ *//')
    if ! git cat-file -e "$src" 2>/dev/null; then
        echo "??  $en：git 里找不到 $src"
        stale=1
        continue
    fi
    # vimtex 的“%! TeX root”一行不算内容
    git show "$src" | grep -v '^%! TeX root' > "$tmp/old"
    grep -v '^%! TeX root' "$zh" > "$tmp/new"
    if cmp -s "$tmp/old" "$tmp/new"; then
        echo "ok  $en"
    else
        echo "旧  $en（$zh 自翻译以来改了 $(diff "$tmp/old" "$tmp/new" | grep -c '^[<>]') 行）"
        [ $show -eq 1 ] && diff -u --label "$src" --label "$zh" "$tmp/old" "$tmp/new"
        stale=1
    fi
done
exit $stale
