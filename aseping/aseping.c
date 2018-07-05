/*

    Copyright 2004,2005,2006 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl.txt

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #include <winsock.h>
    #include "winerr.h"

    #define close   closesocket
    #define sleep   Sleep
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <arpa/inet.h>
    #include <netdb.h>
#endif



#define VER     "0.1.2"
#define BUFFSZ  2048
#define TIMEOUT 5



void ase_show(u_char *data, u_int size);
int timeout(int sock);
u_int resolv(char *host);
void std_err(void);



int main(int argc, char *argv[]) {
    int         sd,
                len,
                psz;
    u_short     port;
    struct  sockaddr_in peer;
    u_char      *buff;

#ifdef WIN32
    WSADATA    wsadata;
    WSAStartup(MAKEWORD(1,0), &wsadata);
#endif

            /*     aseping[] = { 'p', 's', 'g', 'v' };                  */
            /* 'p' is used only for ping, 'g' only to know the server's */
            /* game and 'v' to verify the remote server (useless, it is */
            /* used only by scanner servers)                            */


    setbuf(stdout, NULL);

    fputs("\n"
        "ASE Ping "VER"\n"
        "by Luigi Auriemma\n"
        "e-mail: aluigi@autistici.org\n"
        "web:    aluigi.org\n"
        "\n", stdout);

    if(argc < 3) {
        printf("\n"
            "Usage: %s <server> <port>\n"
            "\n"
            "Usually the port is the default game port plus 123\n"
            "Note: this tool doesn't support multiple ping replies (I'm too lazy)\n"
            "\n", argv[0]);
        exit(1);
    }

    port                 = atoi(argv[2]);
    peer.sin_addr.s_addr = resolv(argv[1]);
    peer.sin_port        = htons(port);
    peer.sin_family      = AF_INET;
    psz                  = sizeof(peer);

    printf("\nTarget %s:%hu\n",
        inet_ntoa(peer.sin_addr), port);

    fputs("\n- Getting informations using ASE ping:\n\n", stdout);

    sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sd < 0) std_err();

    if(sendto(sd, "s", 1, 0, (struct sockaddr *)&peer, psz)
      < 0) std_err();

    if(timeout(sd) < 0) {
        fputs("\nError: socket timeout\n", stdout);
        exit(1);
    }

    buff = malloc(BUFFSZ);
    if(!buff) std_err();

    len = recvfrom(sd, buff, BUFFSZ, 0, (struct sockaddr *)&peer, &psz);
    if(len < 0) std_err();
    close(sd);

    ase_show(buff, len);

    return(0);
}



void ase_show(u_char *data, u_int size) {
    u_char  *limit,
            *players_limit,
            bitnum,
            i;
    int     num = 0,
            players = 1;

    if(memcmp(data, "EYE1", 4)) {
        fwrite(data + 1, size - 1, 1, stdout);
        fputc('\n', stdout);
        return;
    }
    limit = data + size;
    for(players_limit = limit; players_limit != data; players_limit--) {
        if(*players_limit == 0x01) break;
    }
    if(players_limit == data) players_limit = limit;
    data += 4;

    fputs("Informations\n------------", stdout);
    while(data < players_limit) {
        switch(num) {
            case 0: fputs("\nRunning game:   ", stdout); break;
            case 1: fputs("\nGame port:      ", stdout); break;
            case 2: fputs("\nServer name:    ", stdout); break;
            case 3: fputs("\nBattle mode:    ", stdout); break;
            case 4: fputs("\nMap name:       ", stdout); break;
            case 5: fputs("\nVersion:        ", stdout); break;
            case 6: fputs("\nPassword:       ", stdout); break;
            case 7: fputs("\nPlayers:        ", stdout); break;
            case 8: fputs("\nMax players:    ", stdout); break;
            default: {
                if(num & 1) fputc('\n', stdout);
                  else fputs(":   ", stdout);
                } break;
        }

        fwrite(data + 1, *data - 1, 1, stdout);
        data += *data;
        num++;
    }

    fputs("\n\nPlayers\n-------\n", stdout);
    data++;
    while(data < limit) {
        bitnum = *data++;
        printf("%d:  ", players);
        i = 0;
        while(1) {
            if(bitnum & (1 << i)) {
                fwrite(data + 1, *data - 1, 1, stdout);
                data += *data;
            }
            if(++i == 6) break;
            fputs(" \t", stdout);
        }
        players++;
        fputc('\n', stdout);
    }
    fputc('\n', stdout);
}



int timeout(int sock) {
    struct  timeval tout;
    fd_set  fd_read;
    int     err;

    tout.tv_sec = TIMEOUT;
    tout.tv_usec = 0;
    FD_ZERO(&fd_read);
    FD_SET(sock, &fd_read);
    err = select(sock + 1, &fd_read, NULL, NULL, &tout);
    if(err < 0) std_err();
    if(!err) return(-1);
    return(0);
}



u_int resolv(char *host) {
    struct hostent *hp;
    u_int host_ip;

    host_ip = inet_addr(host);
    if(host_ip == INADDR_NONE) {
        hp = gethostbyname(host);
        if(!hp) {
            printf("\nError: Unable to resolv hostname (%s)\n", host);
            exit(1);
        } else host_ip = *(u_int *)hp->h_addr;
    }
    return(host_ip);
}



#ifndef WIN32
    void std_err(void) {
        perror("\nError");
        exit(1);
    }
#endif
