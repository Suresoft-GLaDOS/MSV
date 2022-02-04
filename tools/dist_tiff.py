import logging
import numpy as np
import re
import argparse
from os.path import isfile
from Levenshtein import distance,hamming
from os import environ


def dist_tiff(exp_img,out_img,debug=None):
    if debug is not None:
        log_level = getattr(logging, debug, None)
        if not isinstance(log_level, int):
            raise ValueError('Invalid log level: %s' % log_level)

    exp_file=open(exp_img,'rb')
    exp_data=exp_file.read()
    exp_file.close()

    out_file=open(out_img,'rb')
    out_data=out_file.read()
    out_file.close()

    if len(exp_data)>len(out_data):
        out_data=out_data.ljust(len(exp_data),b'0')
    else:
        exp_data=exp_data.ljust(len(out_data),b'0')

    return hamming(exp_data,out_data)

def dist_pdf(exp_pdf,out_pdf,debug=None):
    if debug is not None:
        log_level = getattr(logging, debug, None)
        if not isinstance(log_level, int):
            raise ValueError('Invalid log level: %s' % log_level)
    
    exp_file=open(exp_pdf,'r')
    exp_data=exp_file.read()
    exp_file.close()

    out_file=open(out_pdf,'r')
    out_data=out_file.read()
    out_file.close()

    return distance(exp_data,out_data)
    
if __name__=='__main__':
    parser = argparse.ArgumentParser('dist_tiff')
    parser.add_argument('is_text', metavar='Mode of datas', help='1 if is test(pdf), or 0 if it is tiff')
    parser.add_argument('exp_file', metavar='Expected file', help='the expected file path')
    parser.add_argument('out_file', metavar='Actual file', help='the actual file path')
    parser.add_argument('--log', metavar='LOG', default=None,
                        choices=['DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL'],
                        help='set the logging level')
    args = parser.parse_args()


    output_file_name=environ['MSV_OUTPUT_DISTANCE_FILE']
    if args.is_text=='1':
        dist=dist_pdf(args.exp_file,args.out_file)
    else:
        dist=dist_tiff(args.exp_file,args.out_file)
    
    with open(output_file_name,'w') as output_file:
        output_file.write(str(dist))
