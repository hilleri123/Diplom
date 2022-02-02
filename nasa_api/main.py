#!/usr/bin/python3

import nasa_api
import password


def main():
    token = nasa_api.Token(password.login, password.password)
    print(token.t())


if __name__ == '__main__':
    main()


