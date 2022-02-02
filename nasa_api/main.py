#!/usr/bin/python3

import nasa_api
import password


def main():
    #token = nasa_api.Token(password.login, password.password)
    #print(token.t())
    r = 0.1
    s = nasa_api.Sphere(r, r, 5000)
    for i in s:
        print(i)
        count = 0
        for j in i:
            print(j)
            count += 1
        print(count)


if __name__ == '__main__':
    main()


