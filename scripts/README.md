# Usage
### Data conversion
#### 6-tuple processing **[Outdated]**
1. Put 6-tuple .txt rule files into `./filters_original` folder.
2. Run script from root folder:
`python3 ./scripts/convert_txt_rules.py ./filters_original ./filters_data`

#### Convert data to hybrid format:
1. `python ./scripts/convert_rules_to_hybrid.py ./filters_original ./filters_hybrid`

#### Convert back
1. `python ./scripts/convert_rules_hybrid.py ./filters_hybrid ./filters_reverse --reverse`