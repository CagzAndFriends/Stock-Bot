import csv
import datetime as dt
import matplotlib.pyplot as plt
from matplotlib import style
import pandas as pd
import pandas_datareader.data as web
import forex_python
import numpy as np
from forex_python.converter import get_rate
from forex_python.bitcoin import BtcConverter

print("stocks")
style.use('ggplot')
start = dt.datetime(2000, 1, 1)
end = dt.datetime(2017, 12, 1)

df = web.DataReader('GOOGL', 'yahoo', start, end)
print(df.head(3))
print("\n")
print("forex")
t = dt.datetime(2001, 10, 18)
get_rate("USD", "EUR", t)
print("USD to EUR")
print(get_rate("USD", "EUR", t))
print("\n")

b = BtcConverter()
print("bitcoin")
print(b.get_latest_price('USD'))
print("USD\n")

#code starts now
print("code starts now\n")
with open("AllStocks1.csv", "r") as StocksFile:
	StocksFileReader = csv.reader(StocksFile)
	StocksList = []
	for row in StocksFileReader:
		if len (row) != 0:
			StocksList = StocksList + row
			#print(web.DataReader(row[0], 'yahoo', start, end))
			try:
				ddd = web.DataReader(row[0], 'yahoo', start, end)
				#print(ddd)
				thepath = 'C:\\Users\\Ryan\\Desktop\\rawstocks\\'
				ddd.to_csv(thepath + row[0] + '.csv', sep=',') 
			except:
				print("error @ " + row[0])
			
print("end of python")
			

