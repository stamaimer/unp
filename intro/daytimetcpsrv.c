#include "unp.h"
#include <time.h>

int main(int argc, char **argv)
{
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	char *time_str[27];
	time_t ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9999); /* daytime server */

	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for (;;)
	{
		connfd = Accept(listenfd, (SA *)NULL, NULL);

		ticks = time(NULL);

		// The ctime() function adjusts the time value for the current time zone, in the same manner as localtime().  It returns a
		// pointer to a 26-character string of the form:
		// Thu Nov 24 18:22:48 1986\n\0
		memcpy(time_str, ctime(&ticks), 26);
		time_str[26] = '\0';
		time_str[25] = '\n';
		time_str[24] = '\r';

		char *time_ptr = time_str;
		while (*time_ptr != '\0')
		{
			snprintf(buff, sizeof(buff), "%c", *time_ptr);
			Write(connfd, buff, strlen(buff));
			time_ptr++;
		}

		Close(connfd);
	}
}
