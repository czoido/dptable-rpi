import random
import os
import time
import sys

from tweepy import (API, BasicAuthHandler, OAuthHandler, Friendship, Cursor,
                    MemoryCache, FileCache)

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

file=open('mentions.txt', 'w')
file_timestamp=open('last_mention.txt', 'r+')
timestamp = file_timestamp.readline()
last_showed_mention_date = time.strptime(timestamp, "%b %d %Y %H:%M:%S")
file_timestamp.close()

#read latest mentions, store them in a file, then the C program will read them from the file
#store the date of the last showed message
mentions = api.mentions_timeline()
for status in reversed(mentions):
	mention_date = time.strptime(status.created_at.strftime("%b %d %Y %H:%M:%S"), "%b %d %Y %H:%M:%S")
	if (last_showed_mention_date<mention_date) and (status.user.screen_name!='dp_table'):
		print >>file,  'from @' + status.user.screen_name + ': ' + status.text 
		file_timestamp=open('last_mention.txt', 'w')
		file_timestamp.write(status.created_at.strftime("%b %d %Y %H:%M:%S"))
		file_timestamp.close()
