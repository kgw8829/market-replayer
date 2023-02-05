Market replayer replays trade tick data from binance

data source: https://www.binance.com/en/landing/data

1) download data and store in .csv/trade directory
2) fileformat {SYMBOL}-{YEAR}-{MONTH}-{DAY}.csv
3) edit src/replay.cc to load data

USAGE ./replayer {YEAR}-{MONTH}-{DAY}
