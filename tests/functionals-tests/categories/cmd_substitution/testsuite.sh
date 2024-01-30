# When command substitution is inside parenthesis
run_test_file basic_paren.test
run_test_file empty_paren.test

# When command substitution is inside backticks
run_test_file basic_backtick.test
run_test_file empty_backtick.test

# Both parenthesis and backticks
run_test_file recursive1.test
