#!/bin/sh
# 检查附录 C 的示例程序。用法：make test
fail=0

# 好的三角形和坏的三角形，输出必须一模一样
[ "$(./triangle_good)" = "$(./triangle_evil)" ] || { echo "FAIL: 两个三角形不一样"; fail=1; }
[ "$(./triangle_good | wc -l)" -eq 5 ] || { echo "FAIL: 三角形应当有 5 行"; fail=1; }

# admin.c 的“坏心眼”：每个人都成了管理员
[ "$(./admin | grep -c '是管理员吗？是')" -eq 3 ] || { echo "FAIL: admin 应当让 3 个人都成为管理员"; fail=1; }

# vote.c 的“坏心眼”：所有的票都算给了猫
./vote | grep -q "猫：8 票，狗：0 票" || { echo "FAIL: vote 的结果不对"; fail=1; }

# 1984 年的 hello, world
[ "$(./anonymous)" = "hello, world!" ] || { echo "FAIL: anonymous 应当打印 hello, world!"; fail=1; }

# 2008 年冠军：涂黑以后，0 的个数还保留着原来数字的位数
./redact secret.ppm 0 0 4 2 | sed -n 5p | grep -q '^0 0 0   000 000 000   00 00 00   000 000 255$' \
    || { echo "FAIL redact: 输出不对"; fail=1; }

# 2015 年冠军：假弹头也通过了检查
./inspect | grep -q '假弹头：通过' || { echo "FAIL inspect: 假弹头应当骗过检查"; fail=1; }
./inspect | grep -q '真弹头：通过' || { echo "FAIL inspect: 真弹头应当通过"; fail=1; }

# -Wall 能发现这两个坏心眼
gcc -std=c99 -Wall -c admin.c -o /dev/null 2>&1 | grep -q misleading-indentation \
    || { echo "FAIL: -Wall 没有发现 admin.c 的问题"; fail=1; }
gcc -std=c99 -Wall -c vote.c -o /dev/null 2>&1 | grep -q Wparentheses \
    || { echo "FAIL: -Wall 没有发现 vote.c 的问题"; fail=1; }

[ $fail -eq 0 ] && echo "全部通过"
exit $fail
