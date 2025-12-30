import json
import csv

# --- User Configuration ---
filename = "./hmhis.json"

# User specifies the desired attributes (keys) here:
desired_attributes = [
    "line", "G", "X", "Y", "Z", "laser", "PUIS_LASER", "VIT_TIR"
]

# Output CSV file
output_csv = "tuple_list.csv"
# -------------------------

# Load JSON data
with open(filename, 'r', encoding='utf-8') as f:
    raw_content = f.read()

data = json.loads(raw_content)

# Prepare CSV writer
with open(output_csv, 'w', newline='', encoding='utf-8') as csvfile:
    writer = csv.DictWriter(csvfile, fieldnames=desired_attributes)

    # Write header
    writer.writeheader()

    # Write rows
    for item in data:
        # Create a row with null for missing keys
        row = {key: item.get(key, "null") for key in desired_attributes}
        writer.writerow(row)

print(f"CSV successfully written to {output_csv}")
