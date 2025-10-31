import yfinance as yf
import pandas as pd
import os

# --- Settings ---
TICKER = "NVDA"
PERIOD = "5y"      # How much data to get (e.g., "1y", "5y", "max")
INTERVAL = "1d"    # What timeframe (e.g., "1d" for daily, "1h" for hourly)
OUTPUT_FILE = "NVDA.csv"
# --- End Settings ---

print(f"Downloading {TICKER} data ({PERIOD} period, {INTERVAL} interval)...")

try:
    # 1. Download the data from Yahoo Finance
    data = yf.download(TICKER, period=PERIOD, interval=INTERVAL)

    if data.empty:
        print(f"No data found for {TICKER}. Check the ticker symbol.")
    else:
        # 2. Clean the data for the C++ parser
        
        # Drop columns we don't need
        if 'Adj Close' in data.columns:
            data = data.drop(columns=['Adj Close'])
        if 'Volume' in data.columns:
            data = data.drop(columns=['Volume'])
        
        # Reset the index to turn the 'Date' (which is the index) into a column
        data = data.reset_index()

        # Convert the 'Date' (datetime object) to a Unix Timestamp (long long)
        # Your C++ code expects a 'long long', so this is perfect.
        data['Timestamp'] = (data['Date'].astype('int64') // 10**9).astype('int64')

        # 3. Reorder columns to exactly match the C++ parser's expected format
        # Your C++ parser expects: Timestamp,Open,High,Low,Close
        final_data = data[['Timestamp', 'Open', 'High', 'Low', 'Close']]

        # 4. Save to the CSV file
        # index=False tells pandas not to write the row numbers (0, 1, 2...)
        final_data.to_csv(OUTPUT_FILE, index=False, float_format='%.2f')
        
        print(f"Successfully saved data to {os.path.abspath(OUTPUT_FILE)}")
        print(f"Total data points: {len(final_data)}")

except Exception as e:
    print(f"An error occurred: {e}")