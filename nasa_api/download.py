#!/usr/bin/python3

import nasa_api
import password

import requests
import asyncio

def name_gen():
    count = 0
    while True:
        yield f"s004_{count}"
        count += 1

def main():
    token = nasa_api.Token(password.login, password.password)
    downloader = nasa_api.DownloadTasks(token, r's004_(\d*)')
    downloader.wait_until_complete()



if __name__ == '__main__':
    main()


