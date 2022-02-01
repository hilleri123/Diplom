#!/usr/bin/python3

import nasa_api


def main():
    token = nasa_api.Token('hilleri123', 'W2gYqs$nxF62c-r')
    print(token.t())


if __name__ == '__main__':
    main()


