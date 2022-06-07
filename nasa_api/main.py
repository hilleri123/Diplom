#!/usr/bin/python3

import nasa_api
import password

def name_gen():
    count = 0
    while True:
        yield f"s001_{count}"
        count += 1

def main():
    token = nasa_api.Token(password.login, password.password)
    #print(token.t())
    r = 0.002
    size = 1000
    nasa_api.Sphere.lat_start = 0.5292036732051038- 0.12
    nasa_api.Sphere.lon_start = 0.65
    nasa_api.Sphere.lat_end = 0.5292036732051038+ 0.12
    nasa_api.Sphere.lon_end = 2
    s = nasa_api.Sphere(r, r, size)
    #p = nasa_api.Products.instance(10000).products
    #prod_id = next(iter(p))
    #layer_id = next(iter(p[prod_id]))
    prod_id = 'ASTGTM_NC.003'
    layer_id = 'ASTER_GDEM_DEM'
    print(prod_id, layer_id)
    task = nasa_api.Task({"id":prod_id, "layer":layer_id}, token, name_gen())
    for i in s:
        task.create_task(i, size)
        #print(i)
        #count = 0
        #for j in i:
            #print(j)
            #count += 1
        #print(count)
    task.wait_until_complete()


if __name__ == '__main__':
    main()


