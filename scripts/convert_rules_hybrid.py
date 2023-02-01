import argparse
import ipaddress
import os
import pathlib
import sys

def create_argument_parser():
    parser = argparse.ArgumentParser(
        description = 'Convert first two fields from 123.123.123.123/12-like format to ternary ("1010101011********"-like)'
    )
    parser.add_argument('input_dir')
    parser.add_argument('output_dir')
    parser.add_argument('--reverse', action='store_true', default=False, required=False, help='If passed, convert backwards')
    return parser


def to_ternary(s):
    ip_part, mask_part = s.split('/')
    prefix_len = int(mask_part)

    ip = ipaddress.IPv4Address(ip_part)
    ip_bin = '{:#b}'.format(ip)[2:]  # Cut '0b'

    result = ip_bin[:prefix_len] + '*' * (32 - prefix_len)
    return result


def from_ternary(s):
    assert len(s) == 32
    stars = s.count('*')
    s = s.replace('*', '0')
    bin_octets = s[:8], s[8:16], s[16:24], s[24:32]
    natural_address = '.'.join(str(int(octet, base=2)) for octet in bin_octets)

    result = natural_address + '/' + str(32 - stars)
    return result


def process_line(line, reverse=False):
    tokens = line.rstrip().split('\t')
    
    if not reverse:
        tokens[0] = to_ternary(tokens[0][1:])  # Cut '@'
        tokens[1] = to_ternary(tokens[1])
        result = '$' + '\t'.join(tokens)
    else:
        tokens[0] = from_ternary(tokens[0][1:])  # Cut '$'
        tokens[1] = from_ternary(tokens[1])
        result = '@' + '\t'.join(tokens)
    return result


def process_file(input_file, target_file, reverse=False):
    print(f'Processing `{input_file}` -> `{target_file}`:')

    with open(input_file) as input_fs:
        lines = input_fs.readlines()

    lines = [process_line(line, reverse=reverse) for line in lines]    
    
    with open(target_file, 'w') as target_fs:
        for line in lines:
            target_fs.write(line)
            target_fs.write('\n')


def main():
    parser = create_argument_parser()
    args = parser.parse_args()

    input_dir = pathlib.Path(args.input_dir)
    output_dir = pathlib.Path(args.output_dir)
    os.makedirs(output_dir, exist_ok=True)

    reverse = args.reverse
    
    input_ext, target_ext = '.txt', '.hybrid'
    if reverse:
        input_ext, target_ext = target_ext, input_ext  

    for f in os.listdir(input_dir):
        if f.endswith(input_ext):
            target_name = f.replace(input_ext, target_ext)
            process_file(input_dir / f, output_dir / target_name, reverse=reverse)



if __name__ == '__main__':
    main()
