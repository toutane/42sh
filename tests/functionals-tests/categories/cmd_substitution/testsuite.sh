# When command substitution is inside parenthesis
run_test_file basic_paren.test
run_test_file empty_paren.test
run_test_file paren_in_paren.sh
run_test_file multiple_paren.sh
run_test_file ambiguous.sh
run_test_file par_in_sub.sh
run_test_file par_in_quotes.sh
run_test_file paren_double_quoted.sh

# When command substitution is inside backticks
run_test_file basic_backtick.test
run_test_file empty_backtick.test
run_test_file backtick_in_backtick.sh
run_test_file backtick_double_quotes.sh
run_test_file multiple_back.sh

# Both parenthesis and backticks
run_test_file paren_and_back.sh
run_test_file many.sh
run_test_file error_in_sub.sh
