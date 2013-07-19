#include "twitter_functions.h"

void tweet_hello_table()
{
    char tweet[256];
    time_t mytime;
    mytime = time(NULL);
    sprintf(tweet,"python2 dp_tweet.py daft punk table started: %s\n",ctime(&mytime));
    system(tweet);
}

void check_new_mentions()
{
    char tweet[256];
    sprintf(tweet,"python2 dp_mentions.py\n");
    system(tweet);
    char line[512];
    FILE *file_;
    file_ = fopen("mentions.txt","r" );
    while (fgets(line,512, file_)!=NULL)
    {
        if(strlen(line)>10)
        {
			scroll_message("    ",50);
			scroll_message(line,133);
			scroll_message("    ",50);
            char * pch;
			pch = strtok (line,":");
			if (pch!=NULL)
			{
				char message[512];
				sprintf(message,"python2 dp_tweet.py %s thanks for your message!\n",pch);
				printf(message);
				system(message);
			}
        }
    }
    fclose(file_);
}
