#!/bin/bash
SCRIPT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
NF_SAMPLES=$SCRIPT_DIR/examples
NF_SAMPLES_COMB=$SCRIPT_DIR/examples-comb
NF_CONFIGS=$SCRIPT_DIR/configs

combinations=(
  "fw_wan_1 pol_wan_0_lan_1 config_mixed.json"
  "fw_wan_1 pol_wan_1_lan_0 config_mixed.json"
  "nat_wan_1 pol_wan_0_lan_1 config_mixed.json"
  "nat_wan_1 pol_wan_1_lan_0 config_mixed.json"
  # Add more combinations here as needed
)

cleanup_examples_comb() {
  if [ -d "$NF_SAMPLES_COMB" ]; then
    echo "Removing all folders inside 'examples-comb'..."
    find "$NF_SAMPLES_COMB" -mindepth 1 -maxdepth 1 -type d -exec rm -rf {} +
  fi
}

run_combination() {

  if [ $# -lt 1 ]; then
    echo "Please provide at least one combination."
    exit 1
  fi

  rm -f $NF_SAMPLES_COMB/exec_time.dat
  touch $NF_SAMPLES_COMB/exec_time.dat

  for combination in "${@}"; do

    bdd1=""
    bdd2=""
    config_file=""
    
    read -r bdd1 bdd2 config_file <<< "$combination"

    bdd1_dir="$NF_SAMPLES/$bdd1"
    bdd2_dir="$NF_SAMPLES/$bdd2"

    if [ ! -d "$bdd1_dir" ] || [ ! -d "$bdd2_dir" ]; then
      echo "Error parsing NFs directory".
      continue
    fi

    if [ -n "$config_file" ] && [ ! -f "$NF_CONFIGS/$config_file" ]; then
      echo "Configuration file '$config_file' not found."
      continue
    fi

    # Create the output folder with the combined names
    if [ -n "$config_file" ]; then
    	output_folder="${bdd1}_${bdd2}_${config_file%.json}"
    else
    	output_folder="${bdd1}_${bdd2}"
    fi

    output_dir="$NF_SAMPLES_COMB/$output_folder"
    mkdir -p "$output_dir"

    echo "Running combination of '$bdd1' and '$bdd2' using config file '$config_file'..."
    start_time=$(date +%s%N)
    ./nf-comb.sh $bdd1_dir $bdd2_dir $output_dir "$NF_CONFIGS/$config_file"
    end_time=$(date +%s%N)

    elapsed_time=$((end_time - start_time))
    real_time=$(echo "scale=2; $elapsed_time / 60000000000" | bc)

    echo "$output_folder  $real_time" >> $NF_SAMPLES_COMB/exec_time.dat

  done
}

build_examples() {

  example_folders=($(find "$NF_SAMPLES" -maxdepth 1 -mindepth 1 -type d))

  for folder in "${example_folders[@]}"; do
    echo "Running make in $folder..."
    (cd "$folder" && make bdd > /dev/null 2>&1)
  done

  echo "Build complete!"
}

cleanup_examples_comb
#build_examples
run_combination "${combinations[@]}"

echo "Complete!"