int main(){
/* Defining Variables */
	int sockfd;
	int nsockfd;
	//int num;
	size_t sin_size;
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	char revbuf[LENGTH]; // Receiver buffer

	/* Get the Socket file descriptor */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		exit(1);
	}
	else
		printf("[Server] Obtaining socket descriptor successfully.\n");

	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Family
	addr_local.sin_port = htons(PORT); // Port number
	addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
	bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

	/* Bind a special Port */
	if ( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		exit(1);
	}
	else
		printf("[Server] Binded tcp port %d in addr 127.0.0.1 sucessfully.\n", PORT);

	/* Listen remote connect/calling */
	if (listen(sockfd, BACKLOG) == -1)
	{
		fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
		exit(1);
	}
	else
		printf ("[Server] Listening the port %d successfully.\n", PORT);

	int success = 0;
	while (success == 0)
	{
		sin_size = sizeof(struct sockaddr_in);

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1)
		{
			fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		else
			printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

		/*Receive File from Client */
		char* fr_name = (char*)"./receive.txt";
		FILE *fr = fopen(fr_name, "w");
		if (fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{
			bzero(revbuf, LENGTH);
			int fr_block_sz = 0;
			while ((fr_block_sz = recv(nsockfd, revbuf, LENGTH, 0)) > 0)
			{
				int write_sz = fwrite(revbuf, sizeof(char), fr_block_sz, fr);
				if (write_sz < fr_block_sz)
				{
					error("File write failed on server.\n");
				}
				bzero(revbuf, LENGTH);
				if (fr_block_sz == 0 || fr_block_sz != 512)
				{
					break;
				}
			}
			if (fr_block_sz < 0)
			{
				if (errno == EAGAIN)
				{
					printf("recv() timed out.\n");
				}
				else
				{
					fprintf(stderr, "recv() failed due to errno = %d\n", errno);
					exit(1);
				}
			}
			printf("Ok received from client!\n");
			fclose(fr);
		}

		/* Call the Script */
		system("chmod +x ./script.sh ; ./script.sh");

		/* Send File to Client */
		//if(!fork())
		//{
		char* fs_name = (char*)"./output.txt";
		char sdbuf[LENGTH]; // Send buffer
		printf("[Server] Sending %s to the Client...", fs_name);
		FILE *fs = fopen(fs_name, "r");
		if (fs == NULL)
		{
			fprintf(stderr, "ERROR: File %s not found on server. (errno = %d)\n", fs_name, errno);
			exit(1);
		}

		bzero(sdbuf, LENGTH);
		int fs_block_sz;
		while ((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
		{
			if (send(nsockfd, sdbuf, fs_block_sz, 0) < 0)
			{
				fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
				exit(1);
			}
			bzero(sdbuf, LENGTH);
		}
		printf("Ok sent to client!\n");
		success = 1;
		close(nsockfd);
		printf("[Server] Connection with Client closed. Server will wait now...\n");
		while (waitpid(-1, NULL, WNOHANG) > 0);
		//}
	}
	
}