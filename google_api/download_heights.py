#!/usr/bin/python3

import requests
from requests import get, post
from pandas.io.json import json_normalize
from clint.textui import progress
from math import pi
import math
import time


file_name = '../tables/api_table_040.csv'

def download(query):
    r = None
    while r is None:
        try:
            r = get(query, timeout = 20)
        except requests.exceptions.Timeout:
            pass
    return r

def get_elevation(lat = None, lon = None, delta = 0):
    '''
        script for returning elevation in m from lat, long
    '''
    if lat is None or lon is None: return None
    lat = lat / pi * 180 + delta
    lon = lon / pi * 180 + delta

    key = None
    with open('./key/key.txt', 'r') as f_key:
        key = f_key.readline()[:-1]
    
    query = ('https://api.open-elevation.com/api/v1/lookup'
             f'?locations={lat},{lon}')
    if key:
        query += f'&key={key}'
        
    with open('log.txt', 'a') as f_log:
        f_log.write("'"+query+"'"+'\n')
    #print(query)
    
    # Request with a timeout for slow responses
    r = download(query)


    # Only get the json response in case of 200 or 201
    if r.status_code == 200 or r.status_code == 201:
        elevation = json_normalize(r.json(), 'results')['elevation'].values[0]
    else: 
        elevation = None
    return elevation


        #double dlat = 0.10;
        #double dlon = 0.10;
        #for (double lat = -pi/2; lat < pi/2.; lat += lat+dlat >= pi/2. ? pi/2.-lat : dlat) {
                #for (double lon = 0; lon < 2*pi; lon += lon+dlon >= 2*pi ? 2*pi-lon : dlon) {


def psevdo_google(lat = None, lon = None, delta = 0):
    '''
        script for returning elevation in m from lat, long
    '''
    if lat is None or lon is None: return None
    lat = lat / pi * 180 + delta
    lon = lon / pi * 180 + delta

    headers = [
                #r"POST /elevation_data/elevation.json HTTP/1.1",
                r"Host:www.gpsvisualizer.com" ,
                r"Connection:keep-alive" ,
                r"Content-Length:40" ,
                r"sec-ch-ua:\" Not A;Brand\";v=\"99\", \"Chromium\";v=\"96\", \"Google Chrome\";v=\"96\"" ,
                r"sec-ch-ua-mobile:?0" ,
                r"User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36" ,
                r"sec-ch-ua-platform:\"Linux\"" ,
                r"Content-Type:text/plain;charset=UTF-8" ,
                r"Accept:*/*" ,
                r"Origin:https://www.gpsvisualizer.com" ,
                r"Sec-Fetch-Site:same-origin" ,
                r"Sec-Fetch-Mode:cors" ,
                r"Sec-Fetch-Dest:empty" ,
                r"Referer:https://www.gpsvisualizer.com/elevation" ,
                r"Accept-Encoding:gzip,deflate,br" ,
                r"Accept-Language:ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7" ,
                r"Cookie:_ga=GA1.2.1340973456.1642942926; _gid=GA1.2.701304413.1642942926"]
    h = {i.split(':')[0]:i.split(':')[1] for i in headers}
    query = 'https://www.gpsvisualizer.com/elevation_data/elevation.json'
    #r = post(query, headers=h, data=f'return_source=1&coords={lat}%2C{lon}', timeout = 20)
    r = post(query, data=f'return_source=1&coords={lat}%2C{lon}', timeout = 20)
    print(json_normalize(r.json()))
    if r.status_code == 200 or r.status_code == 201:
        print(json_normalize(r.json()))
        elevation = json_normalize(r.json())[0].values[3]
    else: 
        elevation = None
    return elevation


def RandGen(start, end, step):
    yield start
    while start < end:
        if start+step >= end:
            yield end
            break
        start += step
        yield start
        


class GeneratorLen(object):
    def __init__(self, gen, length):
        self.gen = gen
        self.length = length

    def __len__(self): 
        return self.length

    def __iter__(self):
        return self.gen


def main():
    with open(file_name, 'w') as f:
        pass
    delta = 0.0
    lat_s = -pi/2
    lat_e = pi/2
    dlat = 0.04
    lon_s = 0
    lon_e = pi*2
    dlon = 0.04
    lat_len = math.ceil((lat_e-lat_s)/dlat)
    for lat in progress.bar(GeneratorLen(RandGen(lat_s, lat_e, dlat), lat_len)):
        for lon in RandGen(lon_s, lon_e, dlon):
            h = get_elevation(lat, lon, delta)
            #h = psevdo_google(lat, lon, delta)
            time.sleep(1)
            with open(file_name, 'a') as f:
                f.write(f"{lat-delta},{lon-delta},{h}\n")

if __name__ == "__main__":
    main()



