#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MSG_SIZE 256

#define MSG_REGISTER 1
#define MSG_LOGIN 2
#define MSG_LIST_CLUBS 3
#define MSG_JOIN_CLUB 4
#define MSG_LEAVE_CLUB 5
#define MSG_VIEW_SCHEDULE 6
#define MSG_QUIT 7

#define RESP_OK 0
#define RESP_ERROR -1

struct message {
    int type;
    char data[MSG_SIZE];
};

struct response {
    int status;
    char data[MSG_SIZE];
};

#endif