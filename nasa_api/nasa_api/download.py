import asyncio
import requests
from datetime import datetime
import functools
import typing
import numpy as np
import nptyping as npt
import math
import pandas as pd
import json
import os
import re
from clint.textui import progress

from .sphere import Point
from .nasa_token import Token

class DownloadTasks:

    __url = 'https://appeears.earthdatacloud.nasa.gov/api/task'
    __file_url = 'https://appeears.earthdatacloud.nasa.gov/api/bundle/{0}'
    __download_url = 'https://appeears.earthdatacloud.nasa.gov/api/bundle/{0}/{1}'
    #__time_pattern = "%Y-%m-%d"
    #__time_pattern = "%m-%d-%Y"
    __status = 'done'
    __dest_dir = "/home/shurik/Work/Diplom/Diplom/nasa_api/results/"

    def __init__(self, token:Token, task_name_template:str):
        self.__token = token
        self.__task_name_template = task_name_template
        self.__futures = []
        self.__tasks_ids = pd.DataFrame(columns=['task_name', 'task_id', 'file_id'])


    def __list_tasks(self):
        headers = {'Authorization': 'Bearer {0}'.format(self.__token.t())}
        resp = requests.get(self.__url, headers=headers)
        js = resp.json()
        loop = asyncio.get_event_loop()
        for task in js:
            if not re.search(self.__task_name_template, task['task_name']) is None and task['status'] == self.__status:
                self.__tasks_ids = self.__tasks_ids.append(pd.DataFrame({'task_name':task['task_name'], 'task_id':task['task_id']}, index=[task['task_id']]))
                url = self.__file_url.format(task['task_id'])
                future = loop.run_in_executor(None, functools.partial(requests.get, url, headers = headers))
                self.__futures.append(future)
        print(self.__tasks_ids)

    async def __complete_list(self):
        for idx, future in enumerate(progress.bar(self.__futures)):
            resp = await future
            file_id = None
            js = resp.json()
            for file_js in js['files']:
                if file_js['file_type'] == 'csv':
                    file_id = file_js['file_id']
            self.__tasks_ids.loc[js['task_id']]['file_id'] = file_id
        self.__futures.clear()
        print(self.__tasks_ids)


    def __download_completed_tasks(self):
        loop = asyncio.get_event_loop()
        headers = {'Authorization': 'Bearer {0}'.format(self.__token.t())}
        for _, row in self.__tasks_ids.iterrows():
            url = self.__download_url.format(row['task_id'],row['file_id'])
            future = loop.run_in_executor(None, functools.partial(requests.get, url, headers = headers, allow_redirects=True, stream=True))
            self.__futures.append((row['task_name'],future))


    def wait_until_complete(self):
        loop = asyncio.get_event_loop()
        self.__list_tasks()
        loop.run_until_complete(self.__complete_list())
        self.__download_completed_tasks()
        loop.run_until_complete(self.__wait_until_complete())

    async def __wait_until_complete(self):
        statuses = {}
        for idx, (name, future) in enumerate(progress.bar(self.__futures)):
            resp = await future

            #resp = requests.get(self.__url, params = params)
            statuses[idx] = resp.status_code
            #print(resp.json())
            try:
                json_data = resp.json()
                with open(f'jsons/{idx}.json', 'w') as f:
                    json.dump(json_data, f)

            except:
                pass
            file_name = f'{name}.csv'
            try:
                filepath = os.path.join(self.__dest_dir, file_name)
                os.makedirs(os.path.dirname(filepath), exist_ok=True)

                with open(filepath, 'wb') as f:
                    for data in resp.iter_content(chunk_size=8192):
                        f.write(data)

            except Exception as exception:
                print(f'file {file_name} faild {exception}')
        print(statuses)
        self.__futures.clear()


        

