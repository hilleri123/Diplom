#!/usr/bin/python3

from requests import get
from pandas.io.json import json_normalize
from clint.textui import progress
from math import pi


file_name = '../tables/api_table.csv'

def get_elevation(lat = None, lon = None):
    '''
        script for returning elevation in m from lat, long
    '''
    if lat is None or lon is None: return None
    lat = lat / pi * 180
    lon = lon / pi * 180
    
    query = ('https://api.open-elevation.com/api/v1/lookup'
             f'?locations={lat},{lon}')
    
    # Request with a timeout for slow responses
    r = get(query, timeout = 20)

    # Only get the json response in case of 200 or 201
    if r.status_code == 200 or r.status_code == 201:
        elevation = json_normalize(r.json(), 'results')['elevation'].values[0]
    else: 
        elevation = None
    return elevation


def main():
    with open(file_name, 'w') as f:
        for lat in progress.bar(range(-10,10,1)):
            for lon in range(-10,10,1):
                h = get_elevation(lat/10, lon/10)
                f.write(f"{lat/10},{lon/10},{h}\n")

if __name__ == "__main__":
    main()



