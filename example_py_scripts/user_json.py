#minimal example for usage
import json

filename = "./../data/hmhis.json"

with open(filename, 'r', encoding='utf-8') as f:
    raw_content = f.read()

data = json.loads(raw_content)

for item in data:
    print(item)
