from bluetooth import *
from pprint import pprint

devices = discover_devices()
for dev in devices:
    print(dev)
    for serv in find_service(address = dev):
        print(serv)

