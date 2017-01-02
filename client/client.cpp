#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <curl/curl.h>
#include <unistd.h> 	/* getpass(), sleep() */
#include <errno.h>
#include <fcntl.h>

static size_t resultIntoString(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

bool checkStringContainsDigit(std::string string_to_test)
{
	return std::string::npos != string_to_test.find_first_of("0123456789");
}

int main(void)
{
	int fd;
	std::string username;
	std::string string_result;
	CURL *curl;
	CURLcode res = CURLE_OK;

	/* List of default imap address */
	std::vector<std::string> imap_list { 
		"imaps://imap.gmail.com:993",
		"imaps://imap.mail.yahoo.com:993",
		"imaps://imap.1and1.com:993",
	};

	/* Get config of account */
	std::cout << "Enter username : " << std::endl;
	std::cin >> username;
	std::string password(getpass("Enter password: "));

	std::string imap;
	std::cout << "Select or enter imap : " << std::endl;
	std::cout << "List of predefined imap " << std::endl;
	for(int i=0; i<imap_list.size(); ++i)
  		std::cout << "index " << i << " " << imap_list[i] << std::endl;

  	std::cout << "Enter index or imap address (add folder if you enter address /INBOX for example) : " << std::endl;
  	std::cin >> imap;

  	if (checkStringContainsDigit(imap))
  		imap = imap_list.at(std::stoi(imap)) + "/INBOX";

  	fd = open("/dev/email_check", O_RDWR);
   	if (fd < 0){
		perror("Failed to open /dev/email_check : ");
		return errno;
   	}

  	/* Check email every minute */
  	while(true)
	{
   		int ret;
		curl = curl_easy_init();
	  	if(curl) 
	  	{
	    	curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
	    	curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str() );
	    	curl_easy_setopt(curl, CURLOPT_URL, imap.c_str());
	    	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "SEARCH UNSEEN");

	    	// get curl result into std::string
	    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, resultIntoString);
	    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &string_result);

	    	res = curl_easy_perform(curl);

	    	if(res != CURLE_OK)
	      		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

	      	/* if string contains any digits, new mail is waiting in INBOX 
	      	   so call driver by syscalls */
	    	if (checkStringContainsDigit(string_result))
	    	{
	    		/* Warns user by light up LED 10 second */
				ret = write(fd, "1", 1); 
			    if (ret < 0){
					perror("Failed to write in /dev/email_check : ");
					return errno;
				}

				sleep(10);

				ret = write(fd, "0", 1); 
			    if (ret < 0){
					perror("Failed to write in /dev/email_check : ");
					return errno;
				}

	    	}

	    	close(fd);
	    	curl_easy_cleanup(curl);
	  	}
	  	else
			std::cerr << "Failed to init curl" << std::endl;

		sleep(60);
	}

  	return (int)res;
}

