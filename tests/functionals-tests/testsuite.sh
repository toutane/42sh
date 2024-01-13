#!/bin/bash

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

# grep regex to match memory leaks
GREP_PATTERN="LeakSanitizer"

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

  # Run ref and our program
  echo -ne "$BLUE-->>$WHITE $1 - test on file $1...$WHITE"
  bash --posix "$1" > $ref_stdout 2> $ref_stderr
  REF_CODE=$?

  $BINARY "$1" > $my_stdout 2> $my_stderr
  MY_CODE=$?

  # Compare ref and our program return stdout stderr
  diff --color=always -u $ref_stdout $my_stdout > $1.diff
  DIFF_CODE=$?

  grep -q $GREP_PATTERN $my_stderr
  GREP_CODE=$?

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

  #check memory leaks
  if [ $GREP_CODE -eq 0 ]; then
      echo -ne "$RED MEMORY_LEAKS$WHITE"
      sucess=false
  fi

  # check if tests were sucess or not
  if $sucess; then
      echo -e "$GREEN OK$WHITE"
      rm -f $1.diff
  else
      echo -ne "$YELLOW\n$1$WHITE"
      [ -s $( realpath $1.diff ) ] && echo -ne "\n$(cat $(realpath $1.diff))$WHITE"
      [ $REF_CODE != $MY_CODE ] && echo -ne "\nref return code: $REF_CODE\nmy return code: $MY_CODE"
      echo
      TOTAL_FAIL=$((TOTAL_FAIL + 1))
      rm -f $1.diff
  fi

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

        grep -q $GREP_PATTERN $my_stderr
        GREP_CODE=$?

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

        #check memory leaks
        if [ $GREP_CODE -eq 0 ]; then
            echo -ne "$RED MEMORY_LEAKS$WHITE"
            sucess=false
        fi

        # check if tests were sucess or not
        if $sucess; then
            echo -e "$GREEN OK$WHITE"
            rm -f $1_$counter.diff
        else
            echo -ne "$YELLOW\n$line$WHITE"
            [ -s $( realpath $1_$counter.diff ) ] && echo -ne "\n$(cat $(realpath $1_$counter.diff))$WHITE"
            [ $REF_CODE != $MY_CODE ] && echo -ne "\nref return code: $REF_CODE\nmy return code: $MY_CODE"
            echo
            TOTAL_FAIL=$((TOTAL_FAIL + 1))
            rm -f $1_$counter.diff
        fi

        counter=$((counter + 1))
    done < "$1"
    echo
}

run_category()
{
    # check if file exist
    if [ ! -e $1 ]; then
        echo -e "${RED}UNKNOWN directory tests \"$1\"$WHITE"
        return
    fi

    cd $1
    source ./testsuite.sh
    cd - >/dev/null
}

run_testsuite()
{
    for category in $@; do
        [ $category = "." ] && continue
        [ $category = $SCRIPT_LOCATION ] && continue
        filename=$(basename $category)

        echo -e "$TURQUOISE==========================================="
        printf "$WHITE%-36s $TURQUOISE%s\n" "${filename#*/}"
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

# path to the script
SCRIPT_LOCATION="$(dirname "$0")/categories"
cd $SCRIPT_LOCATION

# tests relative path
TEST_TO_RUN=$(find . -type d)

# if tests is specified run only thoses tests
if [ $# -ne 0 ]; then
    TEST_TO_RUN=$*
fi

# run the testsuite
run_testsuite $TEST_TO_RUN

# display results
if [ $TOTAL_RUN -eq 0 ]; then
    PERCENT_SUCCES=0 
else
    PERCENT_SUCCES=$(((TOTAL_RUN - TOTAL_FAIL)*100/TOTAL_RUN))
fi

echo -e "$BLUE==========================================="
echo -e "$WHITE RECAP: $([ $PERCENT_SUCCES = 100 ] && echo $GREEN || echo $RED) $PERCENT_SUCCES%"
echo -e "$BLUE===========================================$WHITE"
