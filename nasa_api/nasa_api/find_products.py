
import requests




class Products:
    __singleton = None
    __url = 'https://lpdaacsvc.cr.usgs.gov/appeears/api/product'
    __json_tokens = {'Description':'Elevation', 'Available':True}
    def __init__(self, limit=10, max_offset = 200):
        if self.__singleton:
            #self.instance()
            return
        
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
        for params in params_gen():
            resp = requests.get(self.__url, params = params)
            json = resp.json()
            for prd in json:
                if all([prd[key] == val for key, val in self.__json_tokens.items()]):
                    tmp.append(prd)
        for item in tmp:
            product_id = item['ProductAndVersion']
            resp = requests.get(self.__url + f'/{product_id}')
            json = resp.json()
            self.products[product_id] = json

    
    @classmethod
    def instance(cls):
        if not cls.__singleton:
            cls.__singleton = Products()
        return cls.__singleton

