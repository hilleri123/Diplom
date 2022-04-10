
import requests

import asyncio
import functools
from clint.textui import progress
import json


class Products:
    __singleton = None
    #__url = 'https://lpdaacsvc.cr.usgs.gov/appeears/api/product'
    __url = "https://appeears.earthdatacloud.nasa.gov/api/product"
    __json_tokens = {'Description':'Elevation', 'Available':True}
    def __init__(self, limit=10, max_offset = 10000):
        if self.__singleton:
            #self.instance()
            return
        loop = asyncio.get_event_loop()
        loop.run_until_complete(self.__fill_products(limit, max_offset))

    async def __fill_products(self, limit, max_offset):
        loop = asyncio.get_event_loop()
        def params_gen():
            params = {'limit': limit, 'offset': 0}
            for off in range(max_offset // limit):
                params['offset'] = off * limit
                yield params
            params['offset'] += limit
            params['limit'] = max_offset % limit
            yield params

        self.products = {}

        tmp = []
        futures = []
        for params in progress.bar(params_gen(), expected_size=max_offset // limit):
            future = loop.run_in_executor(None, functools.partial(requests.get, self.__url, params = params))
            futures.append(future)

        for future in progress.bar(futures):
            resp = await future
            #resp = requests.get(self.__url, params = params)
            if resp.status_code == 200:
                json_data = resp.json()
                for prd in json_data:
                    if all([prd[key] == val for key, val in self.__json_tokens.items()]):
                        tmp.append(prd)
        for item in tmp:
            product_id = item['ProductAndVersion']
            resp = requests.get(self.__url + f'/{product_id}')
            json_data = resp.json()
            self.products[product_id] = json_data

    
    @classmethod
    def instance(cls, size=10000):
        if not cls.__singleton:
            cls.__singleton = Products(max_offset=size)
        return cls.__singleton

