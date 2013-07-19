import random
from time import sleep
import os
import sys

from tweepy import (API, BasicAuthHandler, OAuthHandler, Friendship, Cursor,
                    MemoryCache, FileCache)


message = ''
for arg in sys.argv[1:]:
	message = message + ' '  +  arg

username = os.environ.get('TWITTER_USERNAME', 'replace-with-your-own')
oauth_consumer_key = os.environ.get('CONSUMER_KEY', 'replace-with-your-own')
oauth_consumer_secret = os.environ.get('CONSUMER_SECRET', 'replace-with-your-own')
oauth_token = os.environ.get('ACCESS_KEY', 'replace-with-your-own')
oauth_token_secret = os.environ.get('ACCESS_SECRET', 'replace-with-your-own')
auth = OAuthHandler(oauth_consumer_key, oauth_consumer_secret)
auth.set_access_token(oauth_token, oauth_token_secret)
api = API(auth)
api.retry_count = 2
api.retry_delay = 5
update = api.update_status(message)


