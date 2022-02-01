import asyncio
import requests
from datetime import datetime
import functools


class Token:

    def __init__(self, login, password):
        self.login = login
        self.password = password
        self.token = None
        self.expiration = None

        self.time_pattern = "%Y-%m-%dT%H:%M:%SZ"

    def t(self):
        if self.token is None or self.expiration is None or self.expiration < datetime.now():
            loop = asyncio.get_event_loop()
            loop.run_until_complete(self.__get_new_token())
        if self.expiration > datetime.now():
            return self.token

        #self.__get_new_token()

    
    async def __get_new_token(self):
        loop = asyncio.get_event_loop()
        #resp = requests.post('https://lpdaacsvc.cr.usgs.gov/appeears/api/login', auth=(self.login, self.password))
        future = loop.run_in_executor(None, functools.partial(requests.post, 'https://lpdaacsvc.cr.usgs.gov/appeears/api/login', auth=(self.login, self.password)))
        resp = await future
        json_resp = resp.json()
        msg = 'message'
        if msg in json_resp:
            print(json_resp[msg])
            self.token = None
            self.expiration = None
            return
        self.token = json_resp['token']
        self.expiration = datetime.strptime(json_resp['expiration'], self.time_pattern)




