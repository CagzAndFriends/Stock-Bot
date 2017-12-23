'''
Created on Dec 20, 2017

@author: Andre
'''

from __future__ import print_function
import json
import datetime
import time
import requests

minute_api_url = "https://min-api.cryptocompare.com/data/histominute"
hour_api_url = "https://min-api.cryptocompare.com/data/histohour"
day_api_url = "https://min-api.cryptocompare.com/data/histoday"

minute = 60
hour = minute * 60
day = hour * 24

default_conversion = "USD"
default_exchange = "CCCAGG"

class CMCApiHandler():
    def __init__(self):
        pass
    
    def make_request(self, crypto, start, end, interval, conversion=default_conversion,
                      e=default_exchange):
        if isinstance(start, str):
            start = time.strptime(start, "")
        if isinstance(end, str):
            end = time.strptime(end, "")
        url = ""
        limit = 1
        if interval == "minute":
            url = minute_api_url
            limit = int((end - start) / minute)
        elif interval == "hour":
            url = hour_api_url
            limit = int((end - start) / hour)
        elif interval == "day":
            url = day_api_url
            limit = int((end - start) / day)
        else:
            #raise error
            return None
        
        params = {"fsym":crypto,
                  "tsym":conversion,
                  "e": e,
                  "limit":limit,
                  "toTs": end,
                  }
        return requests.get(url, params).json()
        
    def get_hourly(self,crypto, start, end):
        return self.make_request(crypto, start, end, "hour")
    
    def get_minute(self, crypto, start, end):
        return self.make_request(crypto, start, end, "minute")
        
    def get_daily(self, crypto, start, end):
        return self.make_request(crypto, start, end, "day")
    
    def get_all(self, crypto):
        url = day_api_url
        params = {"fsym":crypto,
                  "tsym":default_conversion,
                  "e": default_exchange,
                  "allData": True
                  }
        return requests.get(url, params)
    
class JsonToFile():
    
    def __init__(self):
        pass
    
    
    def to_csv(self, file_name, json):
        volume_key = "volumeto"
        time_format = "%Y-%m-%d"
        with open(file_name, "w") as csv_file:
            print("Date,Open,High,Low,Close,Volume", file=csv_file)
            data = json["Data"]
            for datum in data:
                info = []
                #time
                info.append(str(time.strftime(time_format,time.localtime(datum["time"]))))
                
                info.append(str(datum["open"]))
                info.append(str(datum["high"]))
                info.append(str(datum["low"]))
                info.append(str(datum["close"]))
                
                info.append(str(datum[volume_key]))
                
                
                print(",".join(info), file=csv_file)

def test_run():
    api = CMCApiHandler()
    file_handler = JsonToFile()
    test_symbol = "BTC"
    test_start = 1473307600
    test_end = 1513317600
    json = api.get_daily(test_symbol, test_start, test_end)
    print(json)
    file_handler.to_csv(test_symbol + ".csv", json)
    
if __name__ == "__main__":
    test_run()