import os
import pathlib
import sys

# Cut 6-tuples into 5-tuple and filter out duplicates if occured
USAGE = """Usage: python convert_txt_rules.py <input_dir> <output_dir>:"""


def process_file(input_file, target_file):
    print(f'Processing `{input_file}` -> `{target_file}`:')

    with open(input_file) as input_fs:
        lines = input_fs.readlines()

    lines = [line.rstrip().split('\t') for line in lines]

    if len(lines[0]) == 5:
        print(f'{input_file} already has 5 fields, only remove duplicates')
    elif len(lines[0]) == 6:
        lines = [line[:5] for line in lines]
    else:
        assert False, f'{lines[0]} in {input_file}'

    lines = ['\t'.join(line) for line in lines]

    # Remove duplicates & preserve order
    seen = set()
    result = []
    for line in lines:
        if line not in seen:
            result.append(line)
            seen.add(line)
    
    print(f'Removed {len(lines) - len(result)} / {len(lines)} duplicates')
    with open(target_file, 'w') as target_fs:
        for line in lines:
            target_fs.write(line)
            target_fs.write('\n')
        


def main():
    if len(sys.argv) != 3:
        print(USAGE)
        return

    input_dir = pathlib.Path(sys.argv[1])
    output_dir = pathlib.Path(sys.argv[2])
    os.makedirs(output_dir, exist_ok=True)

    for f in os.listdir(input_dir):
        if f.endswith('.txt'):
            target_name = f.replace('.txt', '.rules')
            process_file(input_dir / f, output_dir / target_name)



if __name__ == '__main__':
    main()
