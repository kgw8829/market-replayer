import pandas as pd

book_csv = './data/book.csv'
trade_csv = './data/trade.csv'

if __name__ == '__main__':
    book_df = pd.read_csv(book_csv, header=None)
    trade_df = pd.read_csv(trade_csv, header=None)
    book_df.sort_values(by=book_df.columns[0], inplace=True)
    trade_df.sort_values(by=trade_df.columns[0], inplace=True)
    book_df.to_csv('./data/book_sorted.csv', header=False, index=False)
    trade_df.to_csv('./data/trade_sorted.csv', header=False, index=False)
    print ('data files are sorted successfuly.')
