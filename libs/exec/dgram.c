#if 0

int main(argc, argv) int argc;
char *argv[];
{
    int sock;
    struct sockaddr_un name;

    /* Create socket on which to send. */
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("opening datagram socket");
        exit(1);
    }
    /* Construct name of socket to send to. */
    name.sun_family = AF_UNIX;
    bzero(name.sun_path, sizeof(name.sun_path));
    strcpy(name.sun_path + 1, argv[1]);
    name.sun_path[0] = 0;
    /* Send message. */
    int i;
    for (i = 0; i < 1000000; i++)
    {
        printf("sending %8d\r", i);
        if (sendto(sock, DATA, sizeof(DATA), 0,
                   (struct sockaddr *)&name, sizeof(struct sockaddr_un)) < 0)
        {
            perror("sending datagram message");
        }
    }
    close(sock);
    return 0;
}

int main()
{
    int sock, length;
    struct sockaddr_un name;
    char buf[1024];

    /* Create socket from which to read. */
    sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("opening datagram socket");
        exit(1);
    }

    /* Create name. */
    bzero(name.sun_path, sizeof(name.sun_path));
    name.sun_family = AF_UNIX;
    strcpy(name.sun_path + 1, NAME);
    name.sun_path[0] = 0;

    /* Bind the UNIX domain address to the created socket */
    if (bind(sock, (struct sockaddr *)&name, sizeof(struct sockaddr_un)))
    {
        perror("binding name to datagram socket");
        exit(1);
    }
    printf("socket -->%s\n", NAME);
    sleep(20);
    int i = 0;
    /* Read from the socket */
    while (1)
    {
        if (read(sock, buf, 10) < 0)
            perror("receiving datagram packet");
        buf[9] = 0;
        printf("%8d: -->%s\n", ++i, buf);
    }
    close(sock);
    unlink(NAME);
}

#endif