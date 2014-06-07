#ifndef __STUBMTP_SMTP_PROTOCOLDEF_H__
#define __STUBMTP_SMTP_PROTOCOLDEF_H__

#define SMTP_CMDLEN 4

#define SMTP_CMD_EHLO "EHLO"
#define SMTP_CMD_MAIL "MAIL"
#define SMTP_CMD_RCPT "RCPT"
#define SMTP_CMD_DATA "DATA"
#define SMTP_CMD_QUIT "QUIT"

#define SMTP_NEWLINE  "\r\n"

#define SMTP_DATA_END SMTP_NEWLINE "." SMTP_NEWLINE

#endif // __STUBMTP_SMTP_PROTOCOLDEF_H__
