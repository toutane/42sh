echo "The number of files in the current directory is: $(ls | wc -l)"

current_dir=$(dirname pwd)
echo "Current directory is: $current_dir"

for file in $(ls env_var); do
    echo "Processing file: $file"
done

echo "Today is $(date)"

number_of_lines=$(wc -l < env_var/echo_sharp_sign.sh)
echo "The file has $number_of_lines lines."

echo "The file has $(wc -l < env_var/echo_sharp_sign.sh) lines."

