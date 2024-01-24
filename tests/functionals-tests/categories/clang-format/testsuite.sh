check_clang_format()
{
    # move to root of repo
    cd $REPO_ROOT

    # redirect clang format file
    clang_stderr="/tmp/.clang_stderr"

    # display clang-format
    sucess=true
    for source_header_file in $(find src -name *.[ch]); do
        TOTAL_RUN=$((TOTAL_RUN + 1))

        # check clang format
        clang-format -n $source_header_file 2> $clang_stderr

        # check if code is unclanged, test if clang file exist and not empty
        if [ -s $clang_stderr ]; then
            echo -ne "$BLUE-->>$WHITE $source_header_file...$WHITE"
            echo -e "$RED CLANG-FORMAT$WHITE"
            sucess=false
            TOTAL_FAIL=$((TOTAL_FAIL + 1))
        fi
    done

    if $sucess; then
        echo -ne "$BLUE-->>$WHITE clang-format...$WHITE"
        echo -e "$GREEN OK$WHITE"
    fi

    # go back previous dir
    cd - > /dev/null
}

check_clang_format
