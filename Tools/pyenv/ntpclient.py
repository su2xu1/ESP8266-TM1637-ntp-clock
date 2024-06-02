#!/usr/local/bin/python3

import os
import sys
import pprint
sys.path.append(os.path.join(os.path.dirname(__file__), 'lib/python3.8/site-packages'))

##pprint.pprint(sys.path)


import ntplib
from time import ctime

def get_time_from_nist():
    try:
        client = ntplib.NTPClient()
        response = client.request('time.nist.gov')
        return ctime(response.tx_time)
    except Exception as e:
        print(f"Error: {e}")
        return None

if __name__ == "__main__":
    current_time = get_time_from_nist()
    if current_time:
        print(f"The current time from NIST is: {current_time}")
    else:
        print("Failed to get time from NIST.")

