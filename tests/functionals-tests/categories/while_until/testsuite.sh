run_test_file while_basic.test

#bad syntax:
run_test_file while_no_body.test
run_test_file while_no_do.test
run_test_file while_no_done.test
run_test_file while_do_done.test
run_test_file no_separator.test
run_test_file no_sec_separator.test

run_test_file until_basic.test

#bad syntax:
run_test_file until_no_body.test
run_test_file until_no_do.test
run_test_file until_no_done.test
run_test_file until_do_done.test
run_test_file no_separator_until.test
run_test_file no_separator_until_2.test

run_test_file neg_while_until.test
run_test_file two_loops.test
run_test_file multiple_lines.test

run_test_file while_break_cond.test
run_test_file until_break_cond.test
#run_test_file while_continue_cond.test


#run_test_file ./counter.test
#run_test_file ./user_input.test
#run_test_file ./file_reading.test
#run_test_file ./command_output.test
#run_test_file ./variables_condition.test
