#!/bin/sh

# colors for display
RED="\e[31m"
GREEN="\e[32m"
YELLOW="\e[33m"
BLUE="\e[34m"
TURQUOISE="\e[36m"
WHITE="\e[0m"

# global variable about tests runs
TOTAL_RUN=0
TOTAL_FAIL=0

# redirect files
ref_stdout=/tmp/.ref_stdout
ref_stderr=/tmp/.ref_stderr
my_stdout=/tmp/.my_stdout
my_stderr=/tmp/.my_stderr

run_category()
{
  cd $1
  source ./testsuite.sh
  cd - >/dev/null
}

run_testsuite()
{
  for category in $@; do
    [ $category = "." ] && continue

    echo -e "$TURQUOISE==========================================="
    printf "$WHITE%-36s $TURQUOISE%s\n" "${category#*/}"
    echo -e "$TURQUOISE==========================================="

    run_category $category
  done
}

run_testsuite $(find . -type d)

# display results
PERCENT_SUCCES=$(((TOTAL_RUN - TOTAL_FAIL)*100/TOTAL_RUN))

echo -e "$BLUE==========================================="
echo -e "$WHITE RECAP: $([ $PERCENT_SUCCES = 100 ] && echo $GREEN || echo $RED) $PERCENT_SUCCES%"
echo -e "$BLUE==========================================="
