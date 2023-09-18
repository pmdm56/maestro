#!/bin/bash

combinations=(
  "fwd_nf drop_nf config_bdd1.json"
  "fwd_nf drop_nf config_bdd2.json"
  "fwd_nf drop_nf"
  "fwd_nf drop_nf config_mixed.json"
  "fwd_nf bcast_nf config_mixed.json"
  "drop_nf bcast_nf config_mixed.json"
  # Add more combinations here as needed
)


#!/bin/bash

# Function to remove all folders inside "examples-comb"
cleanup_examples_comb() {
  examples_comb_dir="examples-comb"

  if [ -d "$examples_comb_dir" ]; then
    echo "Removing all folders inside 'examples-comb'..."
    find "$examples_comb_dir" -mindepth 1 -maxdepth 1 -type d -exec rm -rf {} +
  fi
}

run_combination() {

  if [ $# -lt 1 ]; then
    echo "Please provide at least one combination."
    exit 1
  fi

  for combination in "${@}"; do

    bdd1=""
    bdd2=""
    config_file=""
    
    read -r bdd1 bdd2 config_file <<< "$combination"

    bdd1_dir="examples/$bdd1"
    bdd2_dir="examples/$bdd2"
    config_dir="configs"

    if [ ! -d "$bdd1_dir" ] || [ ! -d "$bdd2_dir" ]; then
      echo "Error parsing NFs directory".
      continue
    fi

    if [ -n "$config_file" ] && [ ! -f "$config_dir/$config_file" ]; then
      echo "Configuration file '$config_file' not found."
      continue
    fi

    # Create the output folder with the combined names
    if [ -n "$config_file" ]; then
    	output_folder="${bdd1}_${bdd2}_${config_file%.json}"
    else
    	output_folder="${bdd1}_${bdd2}"
    fi
    output_dir="examples-comb/$output_folder"
    mkdir -p "$output_dir"

    echo "Running combination of '$bdd1' and '$bdd2' using config file '$config_file'..."

    if [ -n "$config_file" ]; then
        nf-comb -bdd1 "$bdd1_dir/nf.bdd" -bdd2 "$bdd2_dir/nf.bdd" -config "$config_dir/$config_file" -out "$output_dir/nf" > "$output_dir/output" 2>&1
    else 
        nf-comb -bdd1 "$bdd1_dir/nf.bdd" -bdd2 "$bdd2_dir/nf.bdd" -out "$output_dir/nf" > "$output_dir/output" 2>&1
    fi

    if [ -f "$output_dir/nf.bdd" ]; then 
      echo "Synthesizing code on ${output_dir}"
      bdd-to-c -in "$output_dir/nf.bdd" -out "$output_dir/comb.c" -target seq > /dev/null 2>&1

      #bundle code
      ./../synthesized/tools/build.py "$output_dir/comb.c" sequential "$output_dir" > /dev/null 2>&1
      mv ../synthesized/build "$output_dir/build"
      
    fi

  done
}

build_examples() {

  example_folders=($(find "examples/" -maxdepth 1 -mindepth 1 -type d))

  for folder in "${example_folders[@]}"; do
    echo "Running make in $folder..."
    (cd "$folder" && make bdd > /dev/null 2>&1)
  done

  echo "Build complete!"
}

cleanup_examples_comb
#build_examples
#run_combination "${combinations[@]}"

#echo "Complete!"