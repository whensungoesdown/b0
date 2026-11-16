#!/bin/sh
# Check the example programs of Appendix C. Usage: make test
fail=0

# The good triangle and the evil triangle must print exactly the same thing
[ "$(./triangle_good)" = "$(./triangle_evil)" ] || { echo "FAIL: the two triangles differ"; fail=1; }
[ "$(./triangle_good | wc -l)" -eq 5 ] || { echo "FAIL: the triangle should have 5 rows"; fail=1; }

# The trick in admin.c: everybody becomes an administrator
[ "$(./admin | grep -c 'admin? yes')" -eq 3 ] || { echo "FAIL: admin should make all 3 users administrators"; fail=1; }

# The trick in vote.c: every vote goes to the cat
./vote | grep -q "Cat: 8 votes, dog: 0 votes" || { echo "FAIL: wrong result from vote"; fail=1; }

# The hello, world of 1984
[ "$(./anonymous)" = "hello, world!" ] || { echo "FAIL: anonymous should print hello, world!"; fail=1; }

# 2008 winner: after redaction, the number of 0s still gives away how many digits each number had
./redact secret.ppm 0 0 4 2 | sed -n 5p | grep -q '^0 0 0   000 000 000   00 00 00   000 000 255$' \
    || { echo "FAIL redact: wrong output"; fail=1; }

# 2015 winner: the fake warhead passes the inspection too
./inspect | grep -q 'Fake warhead: PASS' || { echo "FAIL inspect: the fake warhead should fool the check"; fail=1; }
./inspect | grep -q 'Real warhead: PASS' || { echo "FAIL inspect: the real warhead should pass"; fail=1; }

# -Wall catches both tricks
gcc -std=c99 -Wall -c admin.c -o /dev/null 2>&1 | grep -q misleading-indentation \
    || { echo "FAIL: -Wall did not catch the problem in admin.c"; fail=1; }
gcc -std=c99 -Wall -c vote.c -o /dev/null 2>&1 | grep -q Wparentheses \
    || { echo "FAIL: -Wall did not catch the problem in vote.c"; fail=1; }

[ $fail -eq 0 ] && echo "All tests passed"
exit $fail
