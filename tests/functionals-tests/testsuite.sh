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

# grep regex to match memory leaks
GREP_PATTERN="LeakSanitizer"

# redirect files
ref_stdout=/tmp/.ref_stdout
ref_stderr=/tmp/.ref_stderr
my_stdout=/tmp/.my_stdout
my_stderr=/tmp/.my_stderr

# time out delay
timeout_time=2s

line_by_line=0
full_file=1

# execute the command by passing it to a string
run_string()
{
    bash --posix -c "$1" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    timeout $timeout_time $BINARY -c "$1" > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    INPUT="string"
}

# execute the command by passing it to a file script
run_file()
{
    bash --posix "$1" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    timeout $timeout_time $BINARY "$1" > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    INPUT="file"
}

# execute the command by passing it to stdin
run_stdin()
{
    echo "$1" | bash --posix > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    timeout $timeout_time echo "$1" | $BINARY > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    INPUT="stdin"
}

# check the differences between ref and my
check_diff()
{
    diff --color=always -u $ref_stdout $my_stdout > $1.diff
    DIFF_CODE=$?

    grep -q $GREP_PATTERN $my_stderr
    GREP_CODE=$?

    # check timeouts
    if [ $WAS_TIMEOUT -eq 1 ]; then
        echo -ne "$RED TIMEOUT($INPUT)$WHITE"
        sucess=false
    fi
    
    #check memory leaks
    if [ $GREP_CODE -eq 0 ]; then
        echo -ne "$RED MEMORY_LEAKS($INPUT)$WHITE"
        sucess=false
    fi

    # check if stdout is the same
    if { [ $DIFF_CODE != 0 ] && [ $WAS_TIMEOUT -eq 0 ]; }; then
        echo -ne "$RED STDOUT($INPUT)$WHITE"
        sucess=false
    fi

    # check if stderr exists
    if { [ -s $ref_stderr ] && [ ! -s $my_stderr ] && [ $WAS_TIMEOUT -eq 0 ]; } ||
        { [ ! -s $ref_stderr ] && [ -s $my_stderr ]; }; then
        echo -ne "$RED STDERR($INPUT)$WHITE"
        sucess=false
    fi
    
    # check if the error code is the same
    if { [ $REF_CODE != $MY_CODE ] && [ $WAS_TIMEOUT -eq 0 ]; }; then
        echo -ne "$RED RETURN($INPUT)$WHITE"
        sucess=false
    fi

    # check if tests fail
    if ! $sucess; then
        echo -e "\n$YELLOW$2$WHITE"
        if [ $REF_CODE != $MY_CODE ] && [ $WAS_TIMEOUT -eq 0 ]; then
            echo -e "ref return code: $REF_CODE\nmy return code: $MY_CODE$WHITE"
        fi
        [ -s $( realpath $1.diff ) ] && echo -e "$(cat $(realpath $1.diff))$WHITE"

        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi

    rm -f $1.diff
}

run_test_file()
{
    [ -e $1 ] || echo "Missing file $1" 1>&2

    WAS_TIMEOUT=0
    TOTAL_RUN=$((TOTAL_RUN + 1))

    echo -ne "$BLUE-->>$WHITE $1...$WHITE"
    file_to_string="$(cat "$1")"

    sucess=true
    run_string "$file_to_string"
    check_diff "$1" "$1"
    if $sucess; then
        run_file "$1"
        check_diff "$1" "$1"
        if $sucess; then
            run_stdin "$file_to_string" 
            check_diff "$1" "$1"
            if $sucess; then
                echo -e "$GREEN OK$WHITE"
            fi
        fi
    fi
}

run_test_line()
{
    [ -e $1 ] || echo "Missing file $1" 1>&2

    counter=1
    while read line; do
        WAS_TIMEOUT=0
        TOTAL_RUN=$((TOTAL_RUN + 1))

        echo -ne "$BLUE-->>$WHITE $1 - test $counter...$WHITE"
        string_to_file="script_file.tmp"
        echo "$line" > $string_to_file

        sucess=true
        run_string "$line"
        check_diff "$1_$counter" "$line"
        if $sucess; then
            run_file "$string_to_file"
            check_diff "$1_$counter" "$line"
            if $sucess; then
                run_stdin "$line"
                check_diff "$1_$counter" "$line"
                if $sucess; then
                    echo -e "$GREEN OK$WHITE"
                fi
            fi
        fi

        rm -f "$string_to_file"
        counter=$(($counter + 1))
    done < "$1"
}

# run testsuite in directory
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
