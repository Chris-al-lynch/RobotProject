#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "Logger.h"

using namespace std;

typedef enum messageType
{
   INVALID_MESSAGE = -1,
   TEST_MESSAGE,
   MOVEMENT_MESSAGE,
   SENSE_MESSAGE,
   RESPONSE_MESSAGE
} messageType_t;

class Message
{
    private:
        static int messageIdentifier;
        static const int MSG_HEADER_MARKER = 0x0BADDAD0;
        static const int MSG_TRAILER_MARKER = 0x0DADBAD0;

    protected:
        typedef struct msgHeader
        {
            int msgLength;
            int msgType;
            int id;
            int marker;
        } msgHeader;

        typedef struct msgTrailer
        {
            int marker;
        } msgTrailer;

        char *messageBuffer;

        Logger *logger;
        static int getMessageIdForNewMessage();

        static void generateMessageHeader( msgHeader *hdr, int length,
                                           messageType_t type );
        static void generateResponseHeader( msgHeader *hdr, int length,
                                            messageType_t type, int id );
        static void generateTrailer( msgTrailer *trailer );

    public:
        Message( char *messageBuffer );
        messageType_t getType();
        int getId();
        int getLength();
        bool isMessageValid();
        virtual char *processMessage(){ return nullptr; }
};

#endif
