#!/usr/bin/env python3

from urllib.parse import parse_qs, urlparse
import sys
from bs4 import BeautifulSoup
import requests
import time


# This script is used to collect urls from google search results.
#   Later these urls will be used to check if there is a readable .git folder using C++ app.

# The reason why python is being used for this is becasuse C++ doesn't like Google responses and has some issues with the ...
#    .. Brotli encoding that i didn't have time to figure out and also python libraries are better at parsing html.

# Google headers
headers = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3'
}

# Google cookies
cookies = {
    "GOOGLE_ABUSE_EXEMPTION": "ID=0620f683f15943de:TM=1764526563:C=R:IP=109.236.81.180-:S=JKPKvGD27amb-3A4BCrrmfg", # VPN was used
    "NID": "526=f6TmG9gYM_BYrusx59NHhrH7zksB2wDik09ULcOowKjEiRcSk75DzatcYr-nY4Ae0DSMS7q3tJ28TcmON6TIINycbSSsAr1OoqrkfaAeRkuKwx26mv8vSYJKQlxJ0Ro0Kje6IuX3YbKgWuzfCqpujMQ26UfRb_DhbfLklNyROcPmLYxAD5Ywqxcoh0QlNjyZN_C5-Rfl83E8PsF0ePBGIKo-RUHjoJ_YcdkBc2lVaHwxjD1iweSgYwdWjDdieowZVgQ",
    "SOCS": "CAESHAgCEhJnd3NfMjAyNTExMjUtMF9SQzEaAmx2IAEaBgiAkq7JBg",
}


# Using dictionary because it stores only unique values
filtered = set()

query = "*.lv"

# start_num=0
result_max_count=500
timeout_seconds=15

def extract_original_url(google_redirect_url):
    """
    Extract original url from google search result redirect url.

    Args:
        google_redirect_url (str): Redirect url from google search result.

    Returns:
        str: Original url if found, otherwise None.
    """
    parsed = urlparse(google_redirect_url)
    if 'google.com' in google_redirect_url:
        return None
    query_params = parse_qs(parsed.query)
    if 'url' in query_params and query_params['url']:
        domain = urlparse(query_params['url'][0]).netloc.lower()
        return domain.removeprefix('www.')
    return None


def collect_urls(start_num=0):
    """
    Collect urls from google search results.

    Args:
        start_num (int): The page number to start from. Defaults to 0.

    Returns:
        None

    Raises:
        Exception: If any exception occurs during the execution of the function.
        KeyboardInterrupt: If the user interrupts the execution of the function.
    """
    try:
        if start_num != 0:
            print(f"Starting of {start_num / 10} page: {start_num}")
        while(len(filtered) <= result_max_count):
            r = requests.get(f'https://www.google.com/search?q=site:{query}&start={start_num}&tbs=qdr:m', headers=headers, cookies=cookies)
            if "Our systems have detected unusual traffic from your computer network." in r.text:
                print(cookies)
                print("Captcha alert")
                continueprompt = input("Do you want to continue ? (y/n)")
                if continueprompt != "y":
                    # printing out current timestamp
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    # writing to file
                    with open('urls.txt.crashed.{timestamp}', 'a') as f:
                        f.write(timestamp + '\n')
                    sys.exit(1)
                # asking for a new SOCS cookie
                GOOGLE_ABUSE_EXEMPTION = input("provide a new GOOGLE_ABUSE_EXEMPTION cookie to continue: ")
                cookies["GOOGLE_ABUSE_EXEMPTION"] = GOOGLE_ABUSE_EXEMPTION
                # repeat the request
                collect_urls(start_num)
            soup = BeautifulSoup(r.content, 'html.parser')
            urls = [link.get('href') for link in soup.find_all('a', href=True)]
            for url in urls:
                url = extract_original_url(url)
                if url is not None:
                    filtered.add(url)
            start_num += 10
            print(len(filtered))
            time.sleep(timeout_seconds)
            print(start_num)

        # writing to file on every line
        with open('urls.txt', 'w') as f:
            for url in filtered:
                f.write(url + '\n')
    except Exception as e:
        print(e)
    except KeyboardInterrupt:
        # printing out current timestamp
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
        # writing to file
        with open('urls.txt.crashed.{timestamp}', 'a') as f:
            for url in filtered:
                f.write(url + '\n')

if __name__ == "__main__":
    # Collecting urls
    collect_urls()

    # Printing urls
    print(filtered)