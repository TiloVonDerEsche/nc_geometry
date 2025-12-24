# import json;
# with open('./../data/hmhis.json') as hmhis:
#     data = json.load(hmhis)


import json
import os

# === CONFIG ===
filename = "./hmhis.json"  # Change this to your actual file path

# === STEP 1: Check if file exists and has content ===
if not os.path.exists(filename):
    print(f"ERROR: File '{filename}' does not exist!")
    exit(1)

if os.path.getsize(filename) == 0:
    print(f"ERROR: File '{filename}' is empty!")
    exit(1)

# === STEP 2: Read and inspect raw content ===
try:
    with open(filename, 'r', encoding='utf-8') as f:
        raw_content = f.read()
except Exception as e:
    print(f"ERROR: Could not read file: {e}")
    exit(1)

print("=== RAW FILE CONTENT (first 500 chars) ===")
print(repr(raw_content[:500]))
if len(raw_content) > 500:
    print("... (truncated)")
print("=== END RAW CONTENT ===\n")

# === STEP 3: Strip whitespace and validate JSON ===
raw_content = raw_content.strip()
if not raw_content:
    print("ERROR: After stripping, file content is empty!")
    exit(1)

# === STEP 4: Try to parse JSON ===
try:
    data = json.loads(raw_content)
    print("SUCCESS: JSON parsed successfully!")
    print(f"Loaded {len(data)} objects.")

    for item in data:
        print(item)

except json.JSONDecodeError as e:
    print(f"JSON DECODE ERROR: {e}")
    print(f"Error at line {e.lineno}, column {e.colno}")
    exit(1)
except Exception as e:
    print(f"Unexpected error: {e}")
    exit(1)
