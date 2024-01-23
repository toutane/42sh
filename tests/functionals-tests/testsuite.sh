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

# path to working directory and mirror morking dir
WORKING_DIR=$(realpath "$(dirname "$0")/working-dir")
WORKING_DIR_REF="${WORKING_DIR}/../working-dir_ref"
WORKING_DIR_MY="${WORKING_DIR}/../working-dir_my"

# time out delay
timeout_time=2s

line_by_line=0
full_file=1

# execute the command by passing it to a string
run_string()
{
    # build mirror ref and my dir
    #build_mirror_dir

    # Execute testsuite in working dir ref
    cd $WORKING_DIR_REF

    bash --posix -c "$1" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    # Go back to previous dir
    cd - > /dev/null

    # Execute testsuite in working dir my
    cd $WORKING_DIR_MY

    timeout $timeout_time $BINARY -c "$1" > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    # Go back to previous dir
    cd - > /dev/null

    INPUT="string"
}

# execute the command by passing it to a file script
run_file()
{
    # build mirror ref and my dir
    #build_mirror_dir

    # Execute testsuite in working dir ref
    cd $WORKING_DIR_REF

    bash --posix "$1" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    # Go back to previous dir
    cd - > /dev/null

    # Execute testsuite in working dir my
    cd $WORKING_DIR_MY

    timeout $timeout_time $BINARY "$1" > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    INPUT="file"

    # Go back to previous dir
    cd - > /dev/null
}

# execute the command by passing it to stdin
run_stdin()
{
    # build mirror ref and my dir
    #build_mirror_dir

    # Execute testsuite in working dir ref
    cd $WORKING_DIR_REF

    bash --posix < "$1" > $ref_stdout 2> $ref_stderr
    REF_CODE=$?

    # Go back to previous dir
    cd - > /dev/null

    # Execute testsuite in working dir my
    cd $WORKING_DIR_MY

    timeout $timeout_time $BINARY < "$1" > $my_stdout 2> $my_stderr
    MY_CODE=$?
    # WARNING, if program return 124 without timeout he will be considered as timeout
    [ $MY_CODE -eq 124 ] && WAS_TIMEOUT=1

    INPUT="stdin"

    # Go back to previous dir
    cd - > /dev/null
}

# check the differences between ref and my
check_diff()
{
    diff --color=always -u $ref_stdout $my_stdout > $1.diff
    DIFF_CODE=$?

    diff --color=always -u -r $WORKING_DIR_REF $WORKING_DIR_MY > $1_working_dir.diff
    DIFF_CODE_DIR=$?

    grep -q $GREP_PATTERN $my_stderr
    GREP_CODE=$?

    # check timeouts
    if [ $WAS_TIMEOUT -eq 1 ]; then
        echo -ne "$RED TIMEOUT($INPUT)$WHITE"
        sucess=false
    fi

    # check both working-dir for ref and my
    if [ $DIFF_CODE_DIR -eq 1 ]; then
        echo -ne "$RED WORKING-DIR($INPUT)$WHITE"
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

    # check if stderr exists and is not empty
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

        # remove mirror directories and rebuild it
        clear_mirror_dir
        build_mirror_dir

        echo -e "\n$YELLOW$2$WHITE"
        if [ $REF_CODE != $MY_CODE ] && [ $WAS_TIMEOUT -eq 0 ]; then
            echo -e "ref return code: $REF_CODE\nmy return code: $MY_CODE$WHITE"
        fi

        # Check diff between stdout of ref and my
        if [ -s $( realpath $1.diff ) ]; then
            echo -e "$(cat $(realpath $1.diff))$WHITE"
        fi

        # Check diff between ref and my working-dir
        if [ -s $( realpath $1_working_dir.diff ) ]; then
            echo -e "$(cat $(realpath $1_working_dir.diff))$WHITE"
        fi

        TOTAL_FAIL=$((TOTAL_FAIL + 1))
    fi

    rm -f *.diff
}

run_test_file()
{
    [ -e $1 ] || echo "Missing file $1" 1>&2

    WAS_TIMEOUT=0
    TOTAL_RUN=$((TOTAL_RUN + 1))

    echo -ne "$BLUE-->>$WHITE $1...$WHITE"
    string="$(cat "$1")"
    file=$(realpath "$1")

    sucess=true
    run_string "$string"
    check_diff "$file" "$string"
    if $sucess; then
        run_file "$file"
        check_diff "$file" "$string"
        if $sucess; then
            run_stdin "$file" 
            check_diff "$file" "$string"
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
    while read string; do
        WAS_TIMEOUT=0
        TOTAL_RUN=$((TOTAL_RUN + 1))

        echo -ne "$BLUE-->>$WHITE $1 - test $counter...$WHITE"
        file="script_file.tmp"
        echo "$string" > $file
        file=$(realpath $file)

        sucess=true
        run_string "$string"
        check_diff "$1_$counter" "$string"
        if $sucess; then
            run_file "$file"
            check_diff "$1_$counter" "$string"
            if $sucess; then
                run_stdin "$file"
                check_diff "$1_$counter" "$string"
                if $sucess; then
                    echo -e "$GREEN OK$WHITE"
                fi
            fi
        fi

        rm -f "$file"
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

    # go to categories dir to execute testsuite.sh
    save_cur_dir=$(pwd)
    cd $1
    source ./testsuite.sh
    cd $save_cur_dir >/dev/null
}

run_testsuite()
{
    # build mirror dir
    build_mirror_dir

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

    # clear mirror dir
    clear_mirror_dir
}

build_mirror_dir()
{
    # build ref mirror dir
    cp -r $WORKING_DIR $WORKING_DIR_REF
    # build my mirror dir
    cp -r $WORKING_DIR $WORKING_DIR_MY
}

clear_mirror_dir()
{
    rm -rf $WORKING_DIR_REF $WORKING_DIR_MY
}

#### MAIN ####

# path to sources files
if [ $# -eq 0 ]; then
    echo -e "${RED}ERROR, no source files dir  where given$WHITE"
    exit 1
else
    REPO_ROOT=$(realpath "$1")
    #SOURCE_FILE_PATH="$1"
    shift
fi

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
