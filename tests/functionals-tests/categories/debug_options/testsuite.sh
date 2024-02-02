$BINARY -apv -c "echo hi" > /dev/null
rm -f ast.dot
$BINARY -oiu -c "echo hi" > /dev/null
$BINARY -c "echo hi" > /dev/full
