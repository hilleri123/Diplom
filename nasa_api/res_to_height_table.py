#!/usr/bin/python3

import pandas as pd
from os import listdir
from os.path import isfile, join
import re

mypath = '/home/shurik/Work/Diplom/Diplom/nasa_api/results'
res_path = '/home/shurik/Work/Diplom/Diplom/tables/sector_table.csv'

template = 's004_(\d*)'

def ok_clms(clm_name):
    return clm_name.lower() == 'latitude' or clm_name.lower() == 'longitude' or clm_name.lower().endswith('dem')

def main():
    only_files = [f for f in listdir(mypath) if isfile(join(mypath, f)) and re.search(template, f)]
    print(only_files)
    height_table = pd.DataFrame()
    for file_name in only_files:
        tmp = pd.read_csv(join(mypath, file_name))
        for clm_name in tmp.columns:
            if not ok_clms(clm_name):
                tmp.drop(clm_name, inplace=True, axis=1)
        height_table = pd.concat([height_table, tmp], ignore_index=True)
        print(height_table)
    height_table.to_csv(res_path, header=False, index=False)



if __name__ == "__main__":
    main()

