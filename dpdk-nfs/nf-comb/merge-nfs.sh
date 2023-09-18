check_folder() {
  if [ ! -d "$1" ]; then
    echo "Error: Folder $1 does not exist."
    exit 1
  fi
}

create_output_folder() {
  if [ ! -d "$1" ]; then
    mkdir -p "$1"
    echo "Merged nf location: $1"
  fi
}

dir_bdd1="$1"
dir_bdd2="$2"
dir_out="$3"

check_folder "$dir_bdd1"
check_folder "$dir_bdd2"
create_output_folder "$dir_out"

# NF-COMB Pipeline

echo "Running 'make bdd' in $dir_bdd1..."
(cd "$dir_bdd1" && make bdd > /dev/null 2>&1)
echo "Running 'make bdd' in $dir_bdd2..."
(cd "$dir_bdd2" && make bdd > /dev/null 2>&1)

echo "Running nf-comb..."
config_file="${4:-}"
if [ -n "$config_file" ] && [ -f "$config_file" ]; then
    nf-comb -bdd1 "$dir_bdd1/nf.bdd" -bdd2 "$dir_bdd2/nf.bdd" -config "$config_file" -out "$dir_out/nf" > "$dir_out/output" 2>&1
else
    echo "Warning: Running nf-comb without config file ($4)."
    nf-comb -bdd1 "$dir_bdd1/nf.bdd" -bdd2 "$dir_bdd2/nf.bdd" -out "$dir_out/nf" > "$dir_out/output" 2>&1
fi

bdd-to-c -in "$dir_out/nf.bdd" -out "$dir_out/comb.c" -target seq 

bundle_output=$(./../synthesized/tools/build_comb.py "$dir_out/comb.c" sequential "$dir_bdd1" "$dir_bdd2")
synthesized_file=$(echo "$bundle_output" | grep "Synthesized file:" | sed -e 's/Synthesized file: //')
synthesized_path="${synthesized_file%/nf.c}"

mv $synthesized_file "$dir_out/nf.c"
mv "$synthesized_path/Makefile" "$dir_out/Makefile"