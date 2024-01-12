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

run_test_file()
{
  [ -e $1 ] || echo "Missing file $1" 1>&2
  sucess=true

  TOTAL_RUN=$((TOTAL_RUN + 1))

  echo -ne "$BLUE-->>$WHITE $1 - test on file $1...$WHITE"
  bash --posix "$1" > $ref_stdout 2> $ref_stderr
  REF_CODE=$?

  $BINARY "$1" > $my_stdout 2> $my_stderr
  MY_CODE=$?

  diff --color=always -u $ref_stdout $my_stdout > $1.diff
  DIFF_CODE=$?

  # check if the error code is the same
  if [ $REF_CODE != $MY_CODE ]; then
    echo -ne "$RED RETURN$WHITE"
    sucess=false
  fi

  # check if stdout is the same
  if [ $DIFF_CODE != 0 ]; then
    echo -ne "$RED STDOUT$WHITE"
    sucess=false
  fi

  # check if stderr exists
  if { [ -s $ref_stderr ] && [ ! -s $my_stderr ]; } ||
    { [ ! -s $ref_stderr ] && [ -s $my_stderr ]; }; then
      echo -ne "$RED STDERR$WHITE"
      sucess=false
  fi

  # check if tests were sucess or not
  if $sucess; then
    echo -e "$GREEN OK$WHITE"
    rm -f $1.diff
  else
    echo -ne "$YELLOW\n$1$WHITE"
    [ -s $( realpath $1.diff ) ] && echo -ne "\n$(cat $(realpath $1.diff))$WHITE"
    echo
    TOTAL_FAIL=$((TOTAL_FAIL + 1))
    rm -f $1.diff
  fi;

  echo
}

run_test()
{
  [ -e $1 ] || echo "Missing file $1" 1>&2
  sucess=true

  counter=1

  while read line; do
    TOTAL_RUN=$((TOTAL_RUN + 1))

    echo -ne "$BLUE-->>$WHITE $1 - test $counter...$WHITE"
    bash --posix -c "$line" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    $BINARY -c "$line" > $my_stdout 2> $my_stderr
    MY_CODE=$?

    diff --color=always -u $ref_stdout $my_stdout > $1_$counter.diff
    DIFF_CODE=$?

    # check if the error code is the same
    if [ $REF_CODE != $MY_CODE ]; then
      echo -ne "$RED RETURN$WHITE"
      sucess=false
    fi

    # check if stdout is the same
    if [ $DIFF_CODE != 0 ]; then
      echo -ne "$RED STDOUT$WHITE"
      sucess=false
    fi

    # check if stderr exists
    if { [ -s $ref_stderr ] && [ ! -s $my_stderr ]; } ||
      { [ ! -s $ref_stderr ] && [ -s $my_stderr ]; }; then
        echo -ne "$RED STDERR$WHITE"
        sucess=false
    fi

    # check if tests were sucess or not
    if $sucess; then
      echo -e "$GREEN OK$WHITE"
      rm -f $1_$counter.diff
    else
      echo -ne "$YELLOW\n$line$WHITE"
      [ -s $( realpath $1_$counter.diff ) ] && echo -ne "\n$(cat $(realpath $1_$counter.diff))$WHITE"
      echo
      TOTAL_FAIL=$((TOTAL_FAIL + 1))
      rm -f $1_$counter.diff
    fi;

    counter=$((counter + 1))
  done < "$1"
  echo
}

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
    [ $category = $SCRIPT_LOCATION ] && continue

    echo -e "$TURQUOISE==========================================="
    printf "$WHITE%-36s $TURQUOISE%s\n" "${category#*/}"
    echo -e "$TURQUOISE-------------------------------------------"

    run_category $category
    echo
  done
}


#### MAIN ####

# path tho program
if [ $# -eq 0 ]; then
    echo -e "${RED}ERROR, no program where given$WHITE"
    exit 1
else
    BINARY="$1"
    shift
fi

# tests that need to be run by the testsuite
SCRIPT_LOCATION="$(dirname "$0")"
TEST_TO_RUN=$(find $SCRIPT_LOCATION -type d)

# if tests is specified run only thoses tests
if [ $# -ne 0 ]; then
    TEST_TO_RUN=$*
fi

# run the testsuite
run_testsuite $TEST_TO_RUN

# display results
PERCENT_SUCCES=$(((TOTAL_RUN - TOTAL_FAIL)*100/TOTAL_RUN))

echo -e "$BLUE==========================================="
echo -e "$WHITE RECAP: $([ $PERCENT_SUCCES = 100 ] && echo $GREEN || echo $RED) $PERCENT_SUCCES%"
echo -e "$BLUE===========================================$WHITE"
