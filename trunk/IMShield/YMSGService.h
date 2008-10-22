#ifndef _YMSGSERVICE_H_
#define _YMSGSERVICE_H_

//0x59 0x4D 0x53 0x47 byte ymsg[4]; // "YMSG"
//0x00 0x0F	byte version[2]; // 2 version number of protocol
//0x00 0x00 byte vendorID;[2] // 2 vendor ID: 0-Windows client, 100-Mac client
//0x26 0xEE	byte len[2]; // 2 length of the data payload
//0x00 0xF1	byte command[2]; // 2 request command
//0x00 0x00 0x00 0x09 byte status[4]; // 4 status of command
//0xA1 0x4A 0xB7 0x56 byte sessionId[4]; // 4;

#ifndef YAHOO_YES_HFILE
#define YAHOO_YES_HFILE

// Define the request header sent with all requests
#define YES_REQUEST_HEADER			"YMSG"
#define YES_RESPONSE_HEADER			"YMSG"
#define YES_PACKET_HEADER_SIZE		20
#define YPAGER_PROTOCOL_VERSION		11
// Size of username/id strings
#define NTFY_KEY_SIZE				35
// Client Request parameters
#define MAX_CLIENT_REQ_SIZE			4096 // Max size of Client Req struct
#define MAX_CLIENT_DATA_SIZE		1980 // Must be greater than MAX_MESSAGE_SIZE
#define MAX_MESSAGE_SIZE			512	 // Max client mesg size
#define HEADERLENGTH (sizeof(YMsgData) - MAX_CLIENT_DATA_SIZE)

// Define the separator for fields all over
#define YES_SEPARATOR			'\001'
#define YES_SEPARATOR_STR		"\001"
// Special char to mark active IDs
#define YES_MARKER				'\002'
#define YES_MARKER_STR			"\002"
#define UTF_NULL				"\300\200"

// User request modes
#define YES_USER_LOGIN				1
#define YES_USER_LOGOFF				2
#define YES_USER_AWAY				3
#define YES_USER_BACK				4
#define YES_USER_GET_MSGS			5
#define YES_USER_HAS_MSG			6
#define YES_ACTIVATE_ID				7
#define YES_DEACTIVATE_ID			8
#define YES_GET_USER_STATUS			10
#define YES_USER_HAS_MAIL			11
#define YES_START_CONFERENCE		12
#define YES_CALENDAR_ALERT			13
#define YES_USER_PERSONAL_MESSAGE	14
#define YES_UPDATE_BUDDY_LIST		15
#define YES_UPDATE_ID_LIST			16
#define YES_UPDATE_IGNORE_LIST		17
#define YES_PING					18
#define YES_UPDATE_GROUP			19
#define YES_SYSTEM_MESSAGE			20
#define YES_CLIENT_STATS			21
#define YES_CLIENT_ALERT_STATS		22
#define YES_GROUP_MESSAGE			23
#define YES_HOST_CONFERENCE			24
#define YES_JOIN_CONFERENCE			25
#define YES_DECLINE_CONFERENCE		26
#define YES_LEAVE_CONFERENCE		27
#define YES_INVITE_CONFERENCE		28
#define YES_SAY_CONFERENCE			29
#define YES_CHAT_LOGIN				30
#define YES_CHAT_LOGOFF				31
#define YES_CHAT_MSG				32
#define YES_GAMES_USER_LOGIN		40
#define YES_GAMES_USER_LOGOFF		41
#define YES_GAMES_USER_HAS_MSG		42
#define YES_NET2PHONE_STATS			44
#define YES_ADDRESSBOOK_ALERT		51
#define YES_AUCTION_ALERT			60
#define YES_USER_FT					70
#define YES_USER_FT_REPLY			71
#define YES_USER_CONVERSE			72
#define YES_USER_WEBTOUR			73
#define YES_IM_ENABLE_VOICE			74
#define YES_SEND_PORT_CHECK			76
//messenger-game integration
#define YES_USER_SEND_MESG			75
#define YES_SEND_DATA_THRU			77
#define YES_P2P_START				79
// Webcam commands
#define YES_MSGR_WEBCAM_TOKEN		80
#define YES_STATS					81
#define YES_USER_LOGIN_2			84
#define YES_PRELOGIN_DATA			85
#define YES_GET_COOKIE_DATA			86
#define YES_HELO					87
#define YES_FEATURE_NOT_SUPPORTED	88
#define YES_NEWS_ALERTS				300
#define YES_SYMANTEC_MSGS			500
// SMS Messages
#define YES_MOBILE_SEND_SMS_MESSAGE	746
#define YES_MOBILE_SMS_LOGIN		748//0 - user Login to Mobile
#define YES_MOBILE_SMS_NUMBER		749 //1 - user configured SMS device from web and server send this notify
// Anonymous Messages
#define YES_ANON_LOGOFF				802
#define YES_ANON_HAS_MSG			806
// Buddy management.
#define YES_ADD_BUDDY				131
#define YES_REMOVE_BUDDY			132
#define YES_MODIFY_IGNORE_LIST		133
#define YES_DENY_BUDDY_ADD			134
#define YES_RENAME_GROUP			137
#define YES_KEEP_ALIVE				138
#define YES_YPC_ADD_FRIEND_APPROVAL	139
#define YES_CHALENGE				140
#define YES_ADD_BUDDY_INSTANT_APPROVAL	141
// Command ids for messenger/chat integration.
#define YES_CHAT_MSGR_USER_LOGIN		150
#define YES_CHAT_GOTO_USER				151
#define YES_CHAT_ROOM_JOIN				152
#define YES_CHAT_ROOM_PART				155
#define YES_CHAT_ROOM_INVITE			157
#define YES_CHAT_MSGR_USER_LOGOFF		160
#define YES_CHAT_PING					161
#define YES_CHAT_WEBCAM_TOKEN			167
#define YES_CHAT_PUBLIC_MSG				168
#define YES_CHAT_ROOM_CREATE			169
// New commands for 6.0
#define YES_GAMES_INVITE				183
#define YES_GAMES_SEND_DATA				184
#define YES_EDIT_INVISIBLE_TO_LIST		185
#define YES_EDIT_VISIBLE_TO_LIST		186
#define YES_ANTIBOT						187
#define YES_AVATAR_CHANGED				188
#define YES_FRIEND_ICON					189
#define YES_FRIEND_ICON_DOWNLOAD		190
#define YES_AVATAR_GET_FILE				191
#define YES_AVATAR_GET_HASH				192
#define YES_DISPLAY_TYPE_CHANGED		193
#define YES_FRIEND_ICON_FT				194
#define YES_GET_COOKIE					195
#define YES_ADDRESS_BOOK_CHANGED		196
#define YES_SET_VISIBILITY				197
#define YES_SET_AWAY_STATUS				198
#define YES_AVATAR_PREFS				199
#define YES_VERIFY_USER					200
#define YES_AUDIBLE						208

// Client originated response..
#define YES_CLIENT_NETSTAT				1000
#define YES_P2P_USER					1001
#define YES_P2P_STATE					1002

// Response status codes
#define YES_STATUS_ERR				-1
#define YES_STATUS_DUPLICATE		-3
#define YES_STATUS_OK				0
#define YES_STATUS_NOTIFY			1
#define YES_STATUS_NOT_AVAILABLE	2 // Target user not available
#define YES_STATUS_NEW_BUDDYOF		3 // Unsolicited: user has been added as buddy
#define YES_STATUS_PARTIAL_LIST		5
#define YES_STATUS_SAVED_MESG		6
#define YES_STATUS_BUDDYOF_DENIED	7
#define YES_STATUS_INVALID_USER		8
#define YES_STATUS_INVITED			11
#define YES_STATUS_DONT_DISTURB		12
#define YES_STATUS_DISTURB_ME		13
#define YES_STATUS_NEW_BUDDYOF_AUTH	15
#define YES_STATUS_WEB_MESG			16
#define YES_STATUS_REQUEST			17
#define YES_STATUS_SPECIFIC_SNDR	22 /* Send to target as specific user */
#define YES_STATUS_SMS_CARRIER		29// SMS Carrier changed for one phone number
#define YES_STATUS_ISGROUP_IM		33 /* As a part of multiple user IM */
#define YES_STATUS_INCOMP_VERSION	24 /* target user using old version or Mac/Unix/Palm messenger*/
#define YES_STATUS_CMD_SENT_ACK		1000 /* Is known only to the client */
#define YES_STATUS_FT_REPLY			0
#define YES_STATUS_FT_ERROR			-1
#define YES_STATUS_FT_NOTIFY		1
#define YES_STATUS_FT_NOTIFY_SAVED	2
#define YES_STATUS_WEBTOUR_OK		1
#define YES_STATUS_CONVERSE_OK		1
#define YES_STATUS_UNKNOWN_USER		1515563605 /* IM for an unknown user, to be checked in UDBS*/
#define YES_STATUS_KNOWN_USER		1515563606 /* IM for an known user, to be saved */
#define YES_STATUS_ACK				18

// Corporate messenger.
#define YES_CORP_USER_LOGIN			450
#define YES_EE_LOGIN				450 // same as YES_CORP_USER_LOGIN
#define YES_MSGREE_LOGIN			451
#define YES_CORP_ID_COPRP2PINIT		452
#define YES_EE_P2P_INIT				452 // same as YES_CORP_ID_COPRP2PINIT
#define YES_CORP_CHAT_MSG			453
#define YES_CORP_GAMES_USER_HAS_MSG	454

/* Secure Messenger client request code */
#define YES_SECURE_USER_LOGIN			460
#define YES_SECURE_IM_MSG				461
#define YES_SECURE_CHAT_SAY_MESG		463
#define YES_SECURE_GAMES_USER_HAS_MSG	464

// WPARAM for the msg from dll to client.
#define MSG_START_NUMBER	500
#define MSG_SOCKET_SERVER	MSG_START_NUMBER+1
#define MSG_HTTP_SERVER		MSG_START_NUMBER+2
#define MSG_STATUS_INT		MSG_START_NUMBER+3
#define MSG_STATUS_STRING	MSG_START_NUMBER+4
#define MSG_STATUS_DONE		MSG_START_NUMBER+5
#define MSG_NET_LAYER		MSG_START_NUMBER+6

// Display message IDs
#define STATUS_NOT_CONNECTED		0
#define STATUS_CONNECTING			1
#define STATUS_CONNECTED			2
#define STATUS_PRELOGIN_SUCCEEDED	100
#define STATUS_SERVER_CONNECTED		101
#define STATUS_FD_CONNECT_SUCCESS	102

enum SERVERTYPE
{
	YAHOO_SERVER_SOCKET = 0,
	YAHOO_SERVER_HTTP = 1,
	YAHOO_SSL_SERVER_SOCKET= 2
};

enum ErrorSeverityLevel
{
	LOG_ERROR,
	LOG_WARNING,
	LOG_VIP_INFORMATION,
	LOG_INFORMATION
};

enum ConnectionID
{
	ConnectionNoProxy = 0,
	ConnectionFirewallNoProxy,
	ConnectionProxy,
	ConnectionNoNetDetection
};

#define ERR_FMT				-0x02 /* general format error */
#define ERR_COMM_FAIL		-0x03 /* internal comm failure error */
#define ERR_REFUSED			-0x04 /* request refused */
#define ERR_INVALID_ROOM	-0x05 /* the room name is invalid */
#define ERR_NOSUCHROOM		-0x06 /* same thing as above, anachronism */
#define ERR_TALK_NOTINROOM	-0x07 /* have to be in room to talk there */
#define ERR_MSG_N			-0x08 /* person not accepting messages */
#define ERR_NO_VOICE		-0x09 /* user doesn't have voice */
#define ERR_TOLONG			-0x0a /* request too long */
#define ERR_NOTONLINE		-0x0b /* user not online */
#define ERR_NOTINROOM		-0x0c /* self not in room. used for leaves */
#define ERR_ROOMNAME_LEN	-0x0d /* room name is too long */
#define ERR_USERNAME_LEN	-0x0e /* username is too long */
#define ERR_INVALID_USER	-0x0f /* the user name is invalid */
#define ERR_NOTINAUDITORIUM	-0x10 /* user isn't in an auditorium */

/* joins and leaves */
#define ERR_JOIN_REFUSED	-0x20 /* plain join refusal */
#define ERR_JOIN_PRIVATE	-0x21 /* join refused, private */
#define ERR_JOIN_FULL		-0x22 /* the room is full. tey another */
#define ERR_JOIN_FULL_RETRY	-0x23 /* full, retry again */
#define ERR_JOIN_TOOMANY	-0x24 /* person is already in too many rooms */
#define ERR_JOIN_TOOLONG	ERR_ROOMNAME_LEN
#define SUCC_JOIN_ALREADY	0x10 /* user is already in room */
#define SUCC_INVITE_ALREADY 0x11 /* already invited to or in this room */
/* say, tell, emote */
#define ERR_SAY_NOTONLINE		ERR_NOTONLINE
#define ERR_SAY_REFUSED			ERR_MSG_N
#define ERR_SAY_VOICE			ERR_NO_VOICE
#define ERR_SAY_LEN				ERR_TOLONG
#define ERR_SAY_NOTINROOM		ERR_TALK_NOTINROOM
#define ERR_EMOTE_NOEMOTION		-0x30 /* that emotion doesn't exist */
#define ERR_EMOTE_NOPERSON		-0x31 /* that emotion requires a person */
#define ERR_EMOTE_NOTONLINE		ERR_NOTONLINE
#define ERR_EMOTE_REFUSED		ERR_MSG_N
#define ERR_EMOTE_VOICE			ERR_NO_VOICE
#define ERR_EMOTE_LEN			ERR_TOLONG
#define ERR_EMOTE_NOTINROOM		ERR_TALK_NOTINROOM
#define GATO_ROOM_FLAG_FILTERED		0x0001 /* was PRIVATE...reclaimed for filtering purposes.. means roomname has already been filtered */
#define GATO_ROOM_FLAG_PRIVATE		0x0006 /* secret and invite only */
#define GATO_ROOM_FLAG_SECRET		0x0002 /* not visible in roomlist */
#define GATO_ROOM_FLAG_INVITEONLY	0x0004 /* must be invited */
#define GATO_ROOM_FLAG_MODERATED	0x0008 /* voice=0 by default */
#define GATO_ROOM_FLAG_Y_CONFERENCE	0x0010 /* room is for Y! Pager */
#define GATO_ROOM_FLAG_STAGE		0x0020 /* room is a stage */
#define GATO_ROOM_FLAG_AUDITORIUM	0x0040 /* room is an auditorium */
#define GATO_ROOM_FLAG_Y_CLUBS		0x0080 /* room is for Y! Clubs */

/* room behaviour */
#define GATO_ROOM_FLAG_USER_CREATED		0x0100 /* duh */
#define GATO_ROOM_FLAG_AUTODUPLICATE	0x0200 /* ditto */
#define GATO_ROOM_FLAG_PERMANENT		0x0400 /* don't die when empty */
#define GATO_ROOM_FLAG_TOPIC_SETTABLE	0x0800 /* admins can change topic */
#define GATO_ROOM_FLAG_URL_SETTABLE		0x1000 /* admins can change url */
#define GATO_ROOM_FLAG_TRANSCRIPT		0x2000 /* save transcripts */
#define GATO_ROOM_FLAG_YAHOOLIGANS		0x4000 /* */
#define GATO_ROOM_FLAG_ADULT			0x08000 /* Adult */
#define GATO_ROOM_FLAG_VOICE			0x0010000 /* Voice chat */
#define GATO_ROOM_FLAG_GUEST_OK			0x0020000 /* Guest users can join */
#define GATO_FLAG_WEBCAM				0x10 /* Indicates Webcam status */
#define GATO_FLAG_MALE					0x8000 /* Male chatter */
#define GATO_FLAG_FEMALE				0x10000 /* Female chatter */

/* List Manipulation error codes */
#define ERR_SUCCESS					0
#define ERR_GENERAL_FAILURE			1
#define ERR_USER_ALREADY_EXISTS		2
#define ERR_NOT_EXISTS				3
#define ERR_COOKIE_ERROR			4
#define ERR_NEED_RESEND				5
#define ERR_MAX_BUDDIES				6
#define ERR_MAX_GROUPS				7
#define ERR_GROUP_EXISTS			8
#define ERR_BUDDY_NOT_DEL			9
#define ERR_MAX_IGNORE_USERS		10
#define ERR_RESERVED_ID				11
#define ERR_IGNORE_USER_NOT_DEL		12
#define ERR_WRONG_PASSWORD			13
#define ERR_ACCOUNT_LOCKED			14
#define ERR_SMS_NOT_DELIVERED		15
#define ERR_SMS_PHONENO_BLOCKED		16
#define ERR_SMS_SYSTEM_PAUSED		17
#define ERR_SMS_SYSTEM_BUSY			18
#define ERR_SMS_WAIT_RESPONSE		19
#define YES_USER_NOT_ALLOWED		20
#define YES_USER_NOT_ONLINE			21
#define ERR_YPC_ACCOUNT_LOCKED		22
#define ERR_YPC_PARENT_AUTH_FAILED	23
#define ERR_SMS_PHONENO_INVALID		24
#define ERR_ANTIBOT					29
//app name in YES_USER_SEND_MESG
#define APPNAME_GAME				"GAME"
#define APPNAME_TYPING				"TYPING"
#define APPNAME_CONTACTINFO			"CONTACTINFO"
#define APPNAME_INVITE_VIEW_WEBCAM	"WEBCAMINVITE"
#define APPNAME_IMVIRONMENT			"IMVIRONMENT"
#define APPNAME_FILEXFER			"FILEXFER"
#define APPNAME_P2P					"PEERTOPEER"
// dummy, until server implements the actual Invite & SendData commands.
#define APPNAME_GAMES_INVITE		"GAMESINVITE"
#define APPNAME_GAMES_SEND_DATA		"GAMESSENDDATA"
#define STAT_TYPE_P2P				1
#define STAT_TYPE_IMV				2

enum
{
	CONNSTATE_UNINIT = 0,
	CONNSTATE_FAILED,
	CONNSTATE_SUCCESS_1,
	CONNSTATE_SUCCESS_2,
	CONNSTATE_INITIATED,
	CONNSTATE_RESPONDED
};

// Globally used constants
#define MAX_64BYTES				64
#define MAX_128BYTES			128
#define MAX_256BYTES			256
#define MAX_512BYTES			512
#define MAX_1024BYTES			1024
#define MAX_2048BYTES			2048
#define MAX_USER_NAME			128
#define MAX_GROUP_NAME			128
#define MAX_IDS					7
#define MAX_TOTAL_IDS_LEN		(MAX_USER_NAME+3)*MAX_IDS
#define MAX_AWAY_MSG			1024
#define MAX_IM_MSG				2048
#define MAX_IMV_ID				38
#define MAX_CONF_ROOMNAME		200
#define MAX_EMAIL_ID			128
#define MAX_MAIL_SUBJECT		128
#define MAX_BIZ_MAIL_TEXT		128
#define MAX_URL					1024
#define MAX_CALENDAR_ALERT_MSG	128
#define MAX_USER_TYPED_MSG		128
#define MAX_GROUP_NAME			128
#define MAX_CONF_TOPIC			64
#define MAX_SERVER_MSG			512
#define MAX_AUCTION_ITEM		128
#define MAX_WEBCAM_TOKEN		128
#define MAX_SERVER_NAME			256
#define MAX_DATE_STR			64
#define MAX_CHALLENGE			512
#define MAX_ALERT_DOC_TITLE		512
#define MAX_ALERT_TYPE			64
#define MAX_COUNTRY				64
#define MAX_SMS_CARRIER_CODE	64
#define MAX_CHAT_ROOM_NAME		128
#define MAX_CHAT_ROOM_CATEGORY	128
#define MAX_CHAT_ROOM_TOPIC		128
#define MAX_APPNAME				64
#define MAX_GAME_MSG			256
#define MAX_DISP_NAME			512
#define MAX_GAMES_DATA			740

#define YES_IM_PANEL_FEATURE	210
#define YES_SHARE_CONTACTS		211
#define YES_IM_SESSION			212
#define YES_SUBSCRIPTION		213
#define YES_ADD_BUDDY_AUTHORIZE	214
#define YES_PHOTO_ADD			215
#define YES_PHOTO_SELECT		216
#define YES_PHOTO_DELETE		217
#define YES_PHOTO_FILE_REQUEST	218
#define YES_PHOTO_POINTER		219
#define YES_FXFER_INVITE		220
#define YES_FXFER_SEND			221
#define YES_FXFER_RECEIVE		222
#define YES_UPDATE_CAPABILITY	223
#define YES_REPORT_SPIM			224
#define YES_MINGLE_DATA			225
#define YES_ALERT				226
#define YES_APP_REGISTRY		227
#define YES_NEW_USER			228
#define YES_ACCEPT_MSGR_INVITE	229
#define YES_MSGR_USAGE			230
#define YES_BUDDY_MOVE			231
#define YES_GET_VOICE_CRUMB		232
#endif // YAHOO_YES_HFILE

///////////////////////////////////////////////////////

#ifndef __YMSG_FLD_DEFS_H
#define __YMSG_FLD_DEFS_H
/* definition for flags for login */
#define YMSG_FLAG_HIDE_ID 0x0001
#define YMSG_FLD_TYPE_INT1
#define YMSG_FLD_TYPE_LONG2
#define YMSG_FLD_TYPE_CHAR3
/* user's presence flags */
#define YMSG_FLAG_USER_IN_PAGER0x00000001
#define YMSG_FLAG_USER_IN_CHAT0x00000002
#define YMSG_FLAG_USER_IN_GAMES0x00000004
#define YMSG_FLAG_VOICE_CONF0x00000100
#define YMSG_FLD_END -1
/* Key values */
#define YMSG_FLD_USER_NAME				0/* User Name (string) */
#define YMSG_FLD_CURRENT_ID				1/* Current user id (string) */
#define YMSG_FLD_ACTIVE_ID				2/* Active id (string) */
#define YMSG_FLD_USER_ID				3/* User ID (string) */
#define YMSG_FLD_SENDER					4/* Sender's name (string) */
#define YMSG_FLD_TARGET_USER			5/* Target user name (string) */
#define YMSG_FLD_PASSWORD				6/* Password (string) */
#define YMSG_FLD_BUDDY					7/* Buddy's name (string) */
#define YMSG_FLD_NUM_BUDDIES			8/* Number of buddies (integer) */
#define YMSG_FLD_NUM_EMAILS				9/* Number of emails (integer) */
#define YMSG_FLD_AWAY_STATUS			10/* Away status code (integer) */
#define YMSG_FLD_SESSION_ID				11/* Session id (long) */
#define YMSG_FLD_IP_ADDRESS				12/* IP adderss (long) */
#define YMSG_FLD_FLAG					13/* Flag (long) */
#define YMSG_FLD_MSG					14/* Message for user (string )*/
#define YMSG_FLD_TIME					15/* Time (long) */
#define YMSG_FLD_ERR_MSG				16/* System error message (string) */
#define YMSG_FLD_PORT					17/* Port number (integer) */
#define YMSG_FLD_MAIL_SUBJECT			18/* Mail subject (string) */
#define YMSG_FLD_AWAY_MSG				19/* Away custom message */
#define YMSG_FLD_URL					20/* Alert URL (string) */
#define YMSG_FLD_ALERT_TIME				21/* ALERT time (string) */
#define YMSG_FLD_NEWS					22/* News (string) */
#define YMSG_FLD_DEV_SPEED				23/* Device speed (int) */
#define YMSG_FLD_WEB_ID					24/* Web id (string) */
#define YMSG_FLD_USER_ALERT_STATS		25 /* User alert status (string) */
#define YMSG_FLD_STATS_DATA				26 /* status (string) */
#define YMSG_FLD_FILE_NAME				27 /* File Name (string) */
#define YMSG_FLD_FILE_SIZE				28 /* File Size (integer) */
#define YMSG_FLD_FILE_DATA				29/* File Data */
#define YMSG_FLD_SYMANTEC_IPADDR		30/* Symantec user IP address */
#define YMSG_FLD_COMMAND				31/* Command (string) */
#define YMSG_FLD_STATUS					32/* Status (integer) */
#define YMSG_FLD_NUM_NEWS				33/* Number of news (integer) */
#define YMSG_FLD_NUM_MSGS				34/* Number of Messages (integer) */
#define YMSG_FLD_ITEM					35/* Autcion Item (string) */
#define YMSG_FLD_OLD_GRP_NAME			36/* Old group name (string) */
#define YMSG_FLD_NEW_GRP_NAME			37/* New group name (string) */
#define YMSG_FLD_EXPIRATION_TIME		38/* File transfer Expiration time */
#define YMSG_FLD_NUM_PERSONAL_MSGS		39/* Number of personal messages */
#define YMSG_FLD_SYS_MSG_CODE			40 /* Sys msg code (integer) */
#define YMSG_FLD_MSG_NUM_DUMMY			41 /* Code for a system message, before v5.6 1349 build */
#define YMSG_FLD_FROM_EMAIL				42 /* From user email addess (string) */
#define YMSG_FLD_FROM_NAME				43 /* From user name (string) */
#define YMSG_FLD_ADD_ID					44/* ID to be added (string) */
#define YMSG_FLD_DELETE_ID				45/* ID to be deleted (string) */
#define YMSG_FLD_DEBUG_INFO				46/* User's debug info (string) */
#define YMSG_FLD_CUSTOM_DND_STATUS		47/* Do Not Disturb flag with custom message status */
#define YMSG_FLD_CONTAINS_TAGS			48/* Contains some special taga */
#define YMSG_FLD_APPNAME				49/* app name (string) for general message YES_USER_SEND_MESG*/
#define YMSG_FLD_NET2PHONE_CALL_LEN		50/* lenght of the phone call */
#define YMSG_FLD_AD_SPACE_ID			51/* ad space id */
#define YMSG_FLD_USES_IMIP_CLIENT		52/* this buddy is logged in using another imip client */
#define YMSG_FLD_SHORTCUT				53/* p2p filexfer shortcut name which uniquely identify the file */
#define YMSG_FLD_FEED_VER				54/* p2p filexfer feed version required to be generated by YServer */
/* conferencing fields (50-69) */
#define YMSG_FLD_INVITOR_NAME			50/* Invitor user name (string) */
#define YMSG_FLD_INVITEE_NAME			51/* Invitee user name (string) */
#define YMSG_FLD_INVITED_USER			52/* Invited user name (string) */
#define YMSG_FLD_JOINED_USER			53/* Joined user name (string) */
#define YMSG_FLD_DECLINED_USER			54/* Declined user name (string) */
#define YMSG_FLD_UNAVAILABLE_USER		55/* Unavailable user name (string) */
#define YMSG_FLD_LEFT_USER				56/* Name of user that left conf (string) */
#define YMSG_FLD_ROOM_NAME				57/* Name of conf room (string) */
#define YMSG_FLD_CONF_TOPIC				58/* Conference topic (string) */
#define YMSG_FLD_COOKIE					59 /* <Cookie Name>\t<Cookie data>*/
#define YMSG_FLD_DEVICE_TYPE			60/* for Mobile buddies(int) */
#define YMSG_FLD_USER_TYPE				60/* its ok to reuse the above field */
#define YMSG_FLD_WEBCAM_TOKEN			61
#define YMSG_FLD_WEBCAM_STATUS			62
#define YMSG_FLD_TIMED_P2P_CONN_FLG		61/* Repeat. but its ok */
#define YMSG_FLD_IMV_ID					63/* IMVironments fields */
#define YMSG_FLD_IMV_FLAG				64
#define YMSG_FLD_BUDDY_GRP_NAME			65
#define YMSG_FLD_ERROR_CODE				66
#define YMSG_FLD_NEWBUDDYGRP_NAME		67
//SMS message
#define YMSG_FLD_PHONE_CARRIER_CODE		68 /* SMS msg from pc client to phone */
#define YMSG_FLD_SCREEN_NAME			69 /* SMS msg pc client sndr's screen name */
#define YMSG_FLD_CONVERSE_COMMAND		70
#define YMSG_FLD_CONVERSE_IDENTITY		71
#define YMSG_FLD_CONVERSE_OTHERGUY		72
#define YMSG_FLD_CONVERSE_TOPIC			73
#define YMSG_FLD_CONVERSE_COMMENT		74
#define YMSG_FLAG_CONVERSE_MAX			75
#define YMSG_FLAG_CONVERSE_URL			76
#define YMSG_FLAG_CONVERSE_YOURCOMMENT	77
#define YMSG_FLD_STAT_TYPE				78/* Type of Stat info */
#define YMSG_FLD_IMIP_SERVICE			79/* IMIP Service name */
#define YMSG_FLD_IMIP_LOGIN				80/* Loging in or out */
//Alert
#define YMSG_FLD_ALERT_TYPEID			81//int
#define YMSG_FLD_ALERT_SUBTYPEID		82
#define YMSG_FLD_ALERT_DOC_TITLE		83
#define YMSG_FLD_ALERT_PRIO_LEVEL		84
#define YMSG_FLD_ALERT_TYPE				85//string
#define YMSG_FLD_ALERT_COUNTRY			86
// Prelogin info fields.
#define YMSG_FLD_BUDDY_LIST				87 /* Buddy list */
#define YMSG_FLD_IGNORE_LIST			88 /* Ignore list */
#define YMSG_FLD_IDENTITY_LIST			89 /* Ignore list */
#define YMSG_FLD_HAS_MAIL				90 /* Does user have a email account */
#define YMSG_FLD_CONVERSE_CMD_DECTEXT	90
#define YMSG_FLD_SMS_PHONE				70
#define YMSG_FLD_ANON_NAME				91/* Anonymous message fields */
#define YMSG_FLD_ANON_ID				92
#define YMSG_T_COOKIE_EXPIRE			93
#define YMSG_FLD_CHALLENGE				94
#define YMSG_FLD_OLD_PASSWORD			96
#define YMSG_FLD_UTF8_FLAG				97
#define YMSG_FLD_COUNTRY_CODE			98
#define YMSG_FLD_COBRAND_CODE			99
#define YMSG_FLD_DATE					100
#define YMSG_FLD_IMV_DATA				101
#define YMSG_FLD_WEBCAM_FARM			102
// Field ids for messenger/chat integration.
#define YMSG_FLD_CHAT_IGNORE_USER		103
#define YMSG_FLD_CHAT_ROOM_NAME			104
#define YMSG_FLD_CHAT_ROOM_TOPIC		105
#define YMSG_FLD_CHAT_ROOM_URL			106
#define YMSG_FLD_CHAT_ROOM_PARAMETER	107
#define YMSG_FLD_CHAT_NUM_USERS			108
#define YMSG_FLD_CHAT_ROOM_USER_NAME	109
#define YMSG_FLD_CHAT_ROOM_USER_AGE		110
#define YMSG_FLD_CHAT_ROOM_USER_GENDER	111
#define YMSG_FLD_CHAT_ROOM_USER_TIMESTAMP	112
#define YMSG_FLD_CHAT_ROOM_USER_FLAG	113
#define YMSG_FLD_CHAT_ERR_NO			114
#define YMSG_FLD_CHAT_SIMILAR_ROOM		115
#define YMSG_FLD_CHAT_EMOT_MSG			116
#define YMSG_FLD_CHAT_MSG				117
#define YMSG_FLD_CHAT_INVITED_USER		118
#define YMSG_FLD_CHAT_INVITER			119
#define YMSG_FLD_CHAT_EXTENDED_DATA_ID	120
#define YMSG_FLD_CHAT_EXTENDED_DATA		121
#define YMSG_FLD_CHAT_USER_SETTINGS		122
#define YMSG_FLD_CHAT_LOGOFF_MSG		123
#define YMSG_FLD_CHAT_MSG_TYPE			124
#define YMSG_FLD_CHAT_FRAME_NAME		125
#define YMSG_FLD_CHAT_FLG				126
#define YMSG_FLD_CHAT_ROOM_TYPE			127
#define YMSG_FLD_CHAT_ROOM_CATEGORY		128
#define YMSG_FLD_CHAT_ROOM_SPACEID		129
#define YMSG_FLD_CHAT_VOICE_AUTH		130
#define YMSG_FLD_ALERT_BUTTONLABEL		131
#define YMSG_FLD_ALERT_BUTTONLINK		132
#define YMSG_FLD_ALERT_MIN_DIMENSION	133
#define YMSG_FLD_BIZ_MAIL_TEXT			134
#define YMSG_FLD_VERSION				135
#define YMSG_FLD_COBRAND_ROOM_INFO		136
#define YMSG_FLD_IDLE_TIME				137
#define YMSG_FLD_NO_IDLE_TIME			138
#define YMSG_FLD_CHAT_USER_NICKNAME		141
#define YMSG_FLD_CHAT_USER_LOCATION		142
#define YMSG_FLD_PING_INTERVAL			143
#define YMSG_FLD_KEEP_ALIVE_INTERVAL	144
#define YMSG_FLD_CPU_TYPE				145
#define YMSG_FLD_OS_VERSION				146
#define YMSG_FLD_TIME_ZONE				147
#define YMSG_FLD_TIME_BIAS				148
#define YMSG_FLD_BLINDED_USERID			149
#define YMSG_FLD_CACHE_CRYPTO_KEY		150
#define YMSG_FLD_LOCAL_CRYPTO_KEY		151
#define YMSG_FLD_YPC_PREFS				153
#define YMSG_FLD_PARENT_ID				154
#define YMSG_FLD_MSG_NUM				159 /* Code used for a system message, apply to v5.6 1349 build and after */
// New fields for 6.0 - Start
#define YMSG_FLD_GAME_ID				180
#define YMSG_FLD_GAME_NAME				181
#define YMSG_FLD_GAME_DATA				182
#define YMSG_FLD_GAME_URL				183
#define YMSG_FLD_STATUS_DATA			184
#define YMSG_FLD_INVISIBLE_TO			185
#define YMSG_FLD_VISIBLE_TO				186
#define YMSG_FLD_STATUS_LINK_TYPE		187
#define YMSG_FLD_AVATAR_FLAG			190
#define YMSG_FLD_AVATAR_MOOD_ID			191
#define YMSG_FLD_ICON_CHECKSUM			192
#define YMSG_FLD_ICON_DATA				193
#define YMSG_FLD_SEQUENCE_NO			194
#define YMSG_FLD_MAX_SEQUENCE_NO		195
#define YMSG_FLD_ANTIBOT_TEXT			196
#define YMSG_FLD_AVATAR_HASH			197
#define YMSG_FLD_AVATAR_USER			198
#define YMSG_FLD_WIDTH					199
#define YMSG_FLD_HEIGHT					200
#define YMSG_FLD_ALERT_DATA				203
#define YMSG_FLD_AVATAR_DEFMOOD			204
#define YMSG_FLD_AVATAR_ZOOM			205
#define YMSG_FLD_DISPLAY_TYPE			206
#define YMSG_FLD_BTUSER_ID				207/* User BT ID (string) */
#define YMSG_FLD_T_COOKIE				208
#define YMSG_FLD_STATS_BUFFER			211
#define YMSG_FLD_APPLY_TO_ALL			212
#define YMSG_FLD_SHOW_MY_AVATAR_IN_FRIEND_TREE	213
#define YMSG_FLD_GAME_PROWLER_PREF		214
#define YMSG_FLD_VAS_USER				215
#define YMSG_FLD_NICKNAME				216
#define YMSG_FLD_YPM_KEY				217
#define YMSG_FLD_ANTIBOT_URL			225
#define YMSG_FLD_ANTIBOT_SECRET			226
#define YMSG_FLD_ANTIBOT_RESPONSE		227
#define YMSG_FLD_AUDIBLE_ID				230
#define YMSG_FLD_AUDIBLE_TEXT			231
#define YMSG_FLD_AUDIBLE_HASH			232
// New fields for 6.0 - End
#define YMSG_FLD_EE_CONFIRM_DELIVERY	160
#define YMSG_FLD_EE_SENDER				161
#define YMSG_FLD_EE_NONCE				162
#define UTF_NULL						"\300\200"
#define YMSG_FLD_FEATURE_ID				221
#define YMSG_FLD_ACTION_TYPE			222
#define YMSG_FLD_UNAUTH					223
#define YMSG_FLD_GROUP					224
#define YMSG_FLD_IGNORED_USER			236
#define YMSG_FLD_PROFILE_ID				237
#define YMSG_FLD_INVISIBLE_TO_FRIEND	238
#define YMSG_FLD_VISIBLE_TO_FRIEND		239
#define YMSG_FLD_CONTACT_INFO			240
#define YMSG_FLD_CLOUD_ID				241
#define YMSG_FLD_BRANDING_ID			242
#define YMSG_FLD_NUM_ATTRIBUTED_BUDDIES 243
#define YMSG_FLD_CAPABILITY_MATRIX		244
#define YMSG_FLD_OBJECT_ID				245
#define YMSG_FLD_OBJECT_NAME			246
#define YMSG_FLD_META_DATA				247
#define YMSG_FLD_OBJECT_SIZE			248
#define YMSG_FLD_TRANSFER_TYPE			249
#define YMSG_FLD_TRANSFER_TAG			250
#define YMSG_FLD_TOKEN					251
#define YMSG_FLD_HASH					252
#define YMSG_FLD_CHECKSUM				253
#define YMSG_FLD_LASTNAME				254
#define YMSG_FLD_INITIATOR				256
#define YMSG_FLD_DATA					257
#define YMSG_FLD_APP_ID					258
#define YMSG_FLD_INSTANCE_ID			259
#define YMSG_FLD_ALERT_ID				260
#define YMSG_FLD_OPI_STATUS				261
#define YMSG_FLD_APP_REGISTER			262
#define YMSG_FLD_CHECKLOGIN_STATUS		263
#define YMSG_FLD_TARGET_GROUP			264
#define YMSG_FLD_FT_SESSION_ID			265
#define YMSG_FLD_TOTAL_FILE_COUNT		266
#define YMSG_FLD_THUMBNAIL				267
#define YMSG_FLD_FILE_INFO				268
#define YMSG_FLD_SPAMMER_ID				269
#define YMSG_FLD_INITIATOR				270
#define YMSG_FLD_FT_ONE_FILE_DONE		271
#define YMSG_FLD_XPOS					272
#define YMSG_FLD_YPOS					273
#define YMSG_FLD_MSG_RECORD				274
#define YMSG_FLD_FLAG_MINGLE_USER		275
#define YMSG_FLD_ABUSE_SIGNATURE		276
#define YMSG_FLD_LOGIN_Y_COOKIE			277
#define YMSG_FLD_LOGIN_T_COOKIE			278
#define YMSG_FLD_LOGIN_CRUMB			279
#define YMSG_FLD_BUDDY_DETAIL			280
#define YMSG_FLD_VALID_CLIENT_COOKIES	281
#define YMSG_FLD_NUM_LCS_BUDDIES		282
#define YMSG_FLD_IS_RELOGIN				283
// Put new iceman ids above this
#define YMSG_FLD_START_OF_RECORD		300
#define YMSG_FLD_END_OF_RECORD			301
#define YMSG_FLD_START_OF_LIST			302
#define YMSG_FLD_END_OF_LIST			303
// PSTN related data
#define YMSG_FLD_COUNTRYCODE			304
#define YMSG_FLD_PSTN_DID				305
#define YMSG_FLD_PREMIUM_FLAG			306
#endif /* __YMSG_FLD_DEFS_H */

#define MAX_YMSG_FIELD_KNOWN_COUNT 307

static const char* YMSG_FLD_NAME_MAPPING[] = {
	"YMSG_FLD_USER_NAME", //0
	"YMSG_FLD_CURRENT_ID", //1
	"YMSG_FLD_ACTIVE_ID", //2
	"YMSG_FLD_USER_ID", //3
	"YMSG_FLD_SENDER", //4
	"YMSG_FLD_TARGET_USER", //5
	"YMSG_FLD_PASSWORD", //6
	"YMSG_FLD_BUDDY", //7
	"YMSG_FLD_NUM_BUDDIES", //8
	"YMSG_FLD_NUM_EMAILS", //9
	"YMSG_FLD_AWAY_STATUS", //10
	"YMSG_FLD_SESSION_ID", //11
	"YMSG_FLD_IP_ADDRESS", //12
	"YMSG_FLD_FLAG", //13
	"YMSG_FLD_MSG", //14
	"YMSG_FLD_TIME", //15
	"YMSG_FLD_ERR_MSG", //16
	"YMSG_FLD_PORT", //17
	"YMSG_FLD_MAIL_SUBJECT", //18
	"YMSG_FLD_AWAY_MSG", //19
	"YMSG_FLD_URL", //20
	"YMSG_FLD_ALERT_TIME", //21
	"YMSG_FLD_NEWS", //22
	"YMSG_FLD_DEV_SPEED", //23
	"YMSG_FLD_WEB_ID", //24
	"YMSG_FLD_USER_ALERT_STATS", //25
	"YMSG_FLD_STATS_DATA", //26
	"YMSG_FLD_FILE_NAME", //27
	"YMSG_FLD_FILE_SIZE", //28
	"YMSG_FLD_FILE_DATA", //29
	"YMSG_FLD_SYMANTEC_IPADDR", //30
	"YMSG_FLD_COMMAND", //31
	"YMSG_FLD_STATUS", //32
	"YMSG_FLD_NUM_NEWS", //33
	"YMSG_FLD_NUM_MSGS", //34
	"YMSG_FLD_ITEM", //35
	"YMSG_FLD_OLD_GRP_NAME", //36
	"YMSG_FLD_NEW_GRP_NAME", //37
	"YMSG_FLD_EXPIRATION_TIME", //38
	"YMSG_FLD_NUM_PERSONAL_MSGS", //39
	"YMSG_FLD_SYS_MSG_CODE", //40
	"YMSG_FLD_MSG_NUM_DUMMY", //41
	"YMSG_FLD_FROM_EMAIL", //42
	"YMSG_FLD_FROM_NAME", //43
	"YMSG_FLD_ADD_ID", //44
	"YMSG_FLD_DELETE_ID", //45
	"YMSG_FLD_DEBUG_INFO", //46
	"YMSG_FLD_CUSTOM_DND_STATUS", //47
	"YMSG_FLD_CONTAINS_TAGS", //48
	"YMSG_FLD_APPNAME", //49
	"YMSG_FLD_NET2PHONE_CALL_LEN/YMSG_FLD_INVITOR_NAME", //50
	"YMSG_FLD_AD_SPACE_ID/YMSG_FLD_INVITEE_NAME", //51
	"YMSG_FLD_USES_IMIP_CLIENT/YMSG_FLD_INVITED_USER", //52
	"YMSG_FLD_SHORTCUT/YMSG_FLD_JOINED_USER", //53
	"YMSG_FLD_FEED_VER/YMSG_FLD_DECLINED_USER", //54
	"YMSG_FLD_UNAVAILABLE_USER", //55
	"YMSG_FLD_LEFT_USER", //56
	"YMSG_FLD_ROOM_NAME", //57
	"YMSG_FLD_CONF_TOPIC", //58
	"YMSG_FLD_COOKIE", //59
	"YMSG_FLD_DEVICE_TYPE/YMSG_FLD_USER_TYPE", //60
	"YMSG_FLD_WEBCAM_TOKEN/YMSG_FLD_TIMED_P2P_CONN_FLG", //61
	"YMSG_FLD_WEBCAM_STATUS", //62
	"YMSG_FLD_IMV_ID", //63
	"YMSG_FLD_IMV_FLAG", //64
	"YMSG_FLD_BUDDY_GRP_NAME", //65
	"YMSG_FLD_ERROR_CODE", //66
	"YMSG_FLD_NEWBUDDYGRP_NAME", //67
	"YMSG_FLD_PHONE_CARRIER_CODE", //68
	"YMSG_FLD_SCREEN_NAME", //69
	"YMSG_FLD_CONVERSE_COMMAND/YMSG_FLD_SMS_PHONE", //70
	"YMSG_FLD_CONVERSE_IDENTITY", //71
	"YMSG_FLD_CONVERSE_OTHERGUY", //72
	"YMSG_FLD_CONVERSE_TOPIC", //73
	"YMSG_FLD_CONVERSE_COMMENT", //74
	"YMSG_FLAG_CONVERSE_MAX", //75
	"YMSG_FLAG_CONVERSE_URL", //76
	"YMSG_FLAG_CONVERSE_YOURCOMMENT", //77
	"YMSG_FLD_STAT_TYPE", //78
	"YMSG_FLD_IMIP_SERVICE", //79
	"YMSG_FLD_IMIP_LOGIN", //80
	"YMSG_FLD_ALERT_TYPEID", //81
	"YMSG_FLD_ALERT_SUBTYPEID", //82
	"YMSG_FLD_ALERT_DOC_TITLE", //83
	"YMSG_FLD_ALERT_PRIO_LEVEL", //84
	"YMSG_FLD_ALERT_TYPE", //85
	"YMSG_FLD_ALERT_COUNTRY", //86
	"YMSG_FLD_BUDDY_LIST", //87
	"YMSG_FLD_IGNORE_LIST", //88
	"YMSG_FLD_IDENTITY_LIST", //89
	"YMSG_FLD_HAS_MAIL/YMSG_FLD_CONVERSE_CMD_DECTEXT", //90
	"YMSG_FLD_ANON_NAME", //91
	"YMSG_FLD_ANON_ID", //92
	"YMSG_T_COOKIE_EXPIRE", //93
	"YMSG_FLD_CHALLENGE", //94
	"YMSG_FLD_UNKNOWN", //95
	"YMSG_FLD_OLD_PASSWORD", //96
	"YMSG_FLD_UTF8_FLAG", //97
	"YMSG_FLD_COUNTRY_CODE", //98
	"YMSG_FLD_COBRAND_CODE", //99
	"YMSG_FLD_DATE", //100
	"YMSG_FLD_IMV_DATA", //101
	"YMSG_FLD_WEBCAM_FARM", //102
	"YMSG_FLD_CHAT_IGNORE_USER", //103
	"YMSG_FLD_CHAT_ROOM_NAME", //104
	"YMSG_FLD_CHAT_ROOM_TOPIC", //105
	"YMSG_FLD_CHAT_ROOM_URL", //106
	"YMSG_FLD_CHAT_ROOM_PARAMETER", //107
	"YMSG_FLD_CHAT_NUM_USERS", //108
	"YMSG_FLD_CHAT_ROOM_USER_NAME", //109
	"YMSG_FLD_CHAT_ROOM_USER_AGE", //110
	"YMSG_FLD_CHAT_ROOM_USER_GENDER", //111
	"YMSG_FLD_CHAT_ROOM_USER_TIMESTAMP", //112
	"YMSG_FLD_CHAT_ROOM_USER_FLAG", //113
	"YMSG_FLD_CHAT_ERR_NO", //114
	"YMSG_FLD_CHAT_SIMILAR_ROOM", //115
	"YMSG_FLD_CHAT_EMOT_MSG", //116
	"YMSG_FLD_CHAT_MSG", //117
	"YMSG_FLD_CHAT_INVITED_USER", //118
	"YMSG_FLD_CHAT_INVITER", //119
	"YMSG_FLD_CHAT_EXTENDED_DATA_ID", //120
	"YMSG_FLD_CHAT_EXTENDED_DATA", //121
	"YMSG_FLD_CHAT_USER_SETTINGS", //122
	"YMSG_FLD_CHAT_LOGOFF_MSG", //123
	"YMSG_FLD_CHAT_MSG_TYPE", //124
	"YMSG_FLD_CHAT_FRAME_NAME", //125
	"YMSG_FLD_CHAT_FLG", //126
	"YMSG_FLD_CHAT_ROOM_TYPE", //127
	"YMSG_FLD_CHAT_ROOM_CATEGORY", //128
	"YMSG_FLD_CHAT_ROOM_SPACEID", //129
	"YMSG_FLD_CHAT_VOICE_AUTH", //130
	"YMSG_FLD_ALERT_BUTTONLABEL", //131
	"YMSG_FLD_ALERT_BUTTONLINK", //132
	"YMSG_FLD_ALERT_MIN_DIMENSION", //133
	"YMSG_FLD_BIZ_MAIL_TEXT", //134
	"YMSG_FLD_VERSION", //135
	"YMSG_FLD_COBRAND_ROOM_INFO", //136
	"YMSG_FLD_IDLE_TIME", //137
	"YMSG_FLD_NO_IDLE_TIME", //138
	"YMSG_FLD_UNKNOWN", //139
	"YMSG_FLD_UNKNOWN", //140
	"YMSG_FLD_CHAT_USER_NICKNAME", //141
	"YMSG_FLD_CHAT_USER_LOCATION", //142
	"YMSG_FLD_PING_INTERVAL", //143
	"YMSG_FLD_KEEP_ALIVE_INTERVAL", //144
	"YMSG_FLD_CPU_TYPE", //145
	"YMSG_FLD_OS_VERSION", //146
	"YMSG_FLD_TIME_ZONE", //147
	"YMSG_FLD_TIME_BIAS", //148
	"YMSG_FLD_BLINDED_USERID", //149
	"YMSG_FLD_CACHE_CRYPTO_KEY", //150
	"YMSG_FLD_LOCAL_CRYPTO_KEY", //151
	"YMSG_FLD_UNKNOWN", //152
	"YMSG_FLD_YPC_PREFS", //153
	"YMSG_FLD_PARENT_ID", //154
	"YMSG_FLD_UNKNOWN", //155
	"YMSG_FLD_UNKNOWN", //156
	"YMSG_FLD_UNKNOWN", //157
	"YMSG_FLD_UNKNOWN", //158
	"YMSG_FLD_MSG_NUM", //159
	"YMSG_FLD_EE_CONFIRM_DELIVERY", //160
	"YMSG_FLD_EE_SENDER", //161
	"YMSG_FLD_EE_NONCE", //162
	"YMSG_FLD_UNKNOWN", //163
	"YMSG_FLD_UNKNOWN", //164
	"YMSG_FLD_UNKNOWN", //165
	"YMSG_FLD_UNKNOWN", //166
	"YMSG_FLD_UNKNOWN", //167
	"YMSG_FLD_UNKNOWN", //168
	"YMSG_FLD_UNKNOWN", //169
	"YMSG_FLD_UNKNOWN", //170
	"YMSG_FLD_UNKNOWN", //171
	"YMSG_FLD_UNKNOWN", //172
	"YMSG_FLD_UNKNOWN", //173
	"YMSG_FLD_UNKNOWN", //174
	"YMSG_FLD_UNKNOWN", //175
	"YMSG_FLD_UNKNOWN", //176
	"YMSG_FLD_UNKNOWN", //177
	"YMSG_FLD_UNKNOWN", //178
	"YMSG_FLD_UNKNOWN", //179
	"YMSG_FLD_GAME_ID", //180
	"YMSG_FLD_GAME_NAME", //181
	"YMSG_FLD_GAME_DATA", //182
	"YMSG_FLD_GAME_URL", //183
	"YMSG_FLD_STATUS_DATA", //184
	"YMSG_FLD_INVISIBLE_TO", //185
	"YMSG_FLD_VISIBLE_TO", //186
	"YMSG_FLD_STATUS_LINK_TYPE", //187
	"YMSG_FLD_UNKNOWN", //188
	"YMSG_FLD_UNKNOWN", //189
	"YMSG_FLD_AVATAR_FLAG", //190
	"YMSG_FLD_AVATAR_MOOD_ID", //191
	"YMSG_FLD_ICON_CHECKSUM", //192
	"YMSG_FLD_ICON_DATA", //193
	"YMSG_FLD_SEQUENCE_NO", //194
	"YMSG_FLD_MAX_SEQUENCE_NO", //195
	"YMSG_FLD_ANTIBOT_TEXT", //196
	"YMSG_FLD_AVATAR_HASH", //197
	"YMSG_FLD_AVATAR_USER", //198
	"YMSG_FLD_WIDTH", //199
	"YMSG_FLD_HEIGHT", //200
	"YMSG_FLD_UNKNOWN", //201
	"YMSG_FLD_UNKNOWN", //202
	"YMSG_FLD_ALERT_DATA", //203
	"YMSG_FLD_AVATAR_DEFMOOD", //204
	"YMSG_FLD_AVATAR_ZOOM", //205
	"YMSG_FLD_DISPLAY_TYPE", //206
	"YMSG_FLD_BTUSER_ID", //207
	"YMSG_FLD_T_COOKIE", //208
	"YMSG_FLD_UNKNOWN", //209
	"YMSG_FLD_UNKNOWN", //210
	"YMSG_FLD_STATS_BUFFER", //211
	"YMSG_FLD_APPLY_TO_ALL", //212
	"YMSG_FLD_SHOW_MY_AVATAR_IN_FRIEND_TREE", //213
	"YMSG_FLD_GAME_PROWLER_PREF", //214
	"YMSG_FLD_VAS_USER", //215
	"YMSG_FLD_NICKNAME", //216
	"YMSG_FLD_YPM_KEY", //217
	"YMSG_FLD_UNKNOWN", //218
	"YMSG_FLD_UNKNOWN", //219
	"YMSG_FLD_UNKNOWN", //220
	"YMSG_FLD_FEATURE_ID", //221
	"YMSG_FLD_ACTION_TYPE", //222
	"YMSG_FLD_UNAUTH", //223
	"YMSG_FLD_GROUP", //224
	"YMSG_FLD_ANTIBOT_URL", //225
	"YMSG_FLD_ANTIBOT_SECRET", //226
	"YMSG_FLD_ANTIBOT_RESPONSE", //227
	"YMSG_FLD_UNKNOWN", //228
	"YMSG_FLD_UNKNOWN", //229
	"YMSG_FLD_AUDIBLE_ID", //230
	"YMSG_FLD_AUDIBLE_TEXT", //231
	"YMSG_FLD_AUDIBLE_HASH", //232
	"YMSG_FLD_UNKNOWN", //233
	"YMSG_FLD_UNKNOWN", //234
	"YMSG_FLD_UNKNOWN", //235
	"YMSG_FLD_IGNORED_USER", //236
	"YMSG_FLD_PROFILE_ID", //237
	"YMSG_FLD_INVISIBLE_TO_FRIEND", //238
	"YMSG_FLD_VISIBLE_TO_FRIEND", //239
	"YMSG_FLD_CONTACT_INFO", //240
	"YMSG_FLD_CLOUD_ID", //241
	"YMSG_FLD_BRANDING_ID", //242
	"YMSG_FLD_NUM_ATTRIBUTED_BUDDIES", //243
	"YMSG_FLD_CAPABILITY_MATRIX", //244
	"YMSG_FLD_OBJECT_ID", //245
	"YMSG_FLD_OBJECT_NAME", //246
	"YMSG_FLD_META_DATA", //247
	"YMSG_FLD_OBJECT_SIZE", //248
	"YMSG_FLD_TRANSFER_TYPE", //249
	"YMSG_FLD_TRANSFER_TAG", //250
	"YMSG_FLD_TOKEN", //251
	"YMSG_FLD_HASH", //252
	"YMSG_FLD_CHECKSUM", //253
	"YMSG_FLD_LASTNAME", //254
	"YMSG_FLD_UNKNOWN", //255
	"YMSG_FLD_INITIATOR", //256
	"YMSG_FLD_DATA", //257
	"YMSG_FLD_APP_ID", //258
	"YMSG_FLD_INSTANCE_ID", //259
	"YMSG_FLD_ALERT_ID", //260
	"YMSG_FLD_OPI_STATUS", //261
	"YMSG_FLD_APP_REGISTER", //262
	"YMSG_FLD_CHECKLOGIN_STATUS", //263
	"YMSG_FLD_TARGET_GROUP", //264
	"YMSG_FLD_FT_SESSION_ID", //265
	"YMSG_FLD_TOTAL_FILE_COUNT", //266
	"YMSG_FLD_THUMBNAIL", //267
	"YMSG_FLD_FILE_INFO", //268
	"YMSG_FLD_SPAMMER_ID", //269
	"YMSG_FLD_INITIATOR", //270
	"YMSG_FLD_FT_ONE_FILE_DONE", //271
	"YMSG_FLD_XPOS", //272
	"YMSG_FLD_YPOS", //273
	"YMSG_FLD_MSG_RECORD", //274
	"YMSG_FLD_FLAG_MINGLE_USER", //275
	"YMSG_FLD_ABUSE_SIGNATURE", //276
	"YMSG_FLD_LOGIN_Y_COOKIE", //277
	"YMSG_FLD_LOGIN_T_COOKIE", //278
	"YMSG_FLD_LOGIN_CRUMB", //279
	"YMSG_FLD_BUDDY_DETAIL", //280
	"YMSG_FLD_VALID_CLIENT_COOKIES", //281
	"YMSG_FLD_NUM_LCS_BUDDIES", //282
	"YMSG_FLD_IS_RELOGIN", //283
	"YMSG_FLD_UNKNOWN", //284
	"YMSG_FLD_UNKNOWN", //285
	"YMSG_FLD_UNKNOWN", //286
	"YMSG_FLD_UNKNOWN", //287
	"YMSG_FLD_UNKNOWN", //288
	"YMSG_FLD_UNKNOWN", //289
	"YMSG_FLD_UNKNOWN", //290
	"YMSG_FLD_UNKNOWN", //291
	"YMSG_FLD_UNKNOWN", //292
	"YMSG_FLD_UNKNOWN", //293
	"YMSG_FLD_UNKNOWN", //294
	"YMSG_FLD_UNKNOWN", //295
	"YMSG_FLD_UNKNOWN", //296
	"YMSG_FLD_UNKNOWN", //297
	"YMSG_FLD_UNKNOWN", //298
	"YMSG_FLD_UNKNOWN", //299
	"YMSG_FLD_START_OF_RECORD", //300
	"YMSG_FLD_END_OF_RECORD", //301
	"YMSG_FLD_START_OF_LIST", //302
	"YMSG_FLD_END_OF_LIST", //303
	"YMSG_FLD_COUNTRYCODE", //304
	"YMSG_FLD_PSTN_DID", //305
	"YMSG_FLD_PREMIUM_FLAG", //306
};

static const char* YES_NAME_MAPPING_BASIC[] = {
	"YES_INVALID", //0
	"YES_USER_LOGIN", //1
	"YES_USER_LOGOFF", //2
	"YES_USER_AWAY", //3
	"YES_USER_BACK", //4
	"YES_USER_GET_MSGS", //5
	"YES_USER_HAS_MSG", //6
	"YES_ACTIVATE_ID", //7
	"YES_DEACTIVATE_ID", //8
	"YES_UNKNOWN", //9
	"YES_GET_USER_STATUS", //10
	"YES_USER_HAS_MAIL", //11
	"YES_START_CONFERENCE", //12
	"YES_CALENDAR_ALERT", //13
	"YES_USER_PERSONAL_MESSAGE", //14
	"YES_UPDATE_BUDDY_LIST", //15
	"YES_UPDATE_ID_LIST", //16
	"YES_UPDATE_IGNORE_LIST", //17
	"YES_PING", //18
	"YES_UPDATE_GROUP", //19
	"YES_SYSTEM_MESSAGE", //20
	"YES_CLIENT_STATS", //21
	"YES_CLIENT_ALERT_STATS", //22
	"YES_GROUP_MESSAGE", //23
	"YES_HOST_CONFERENCE", //24
	"YES_JOIN_CONFERENCE", //25
	"YES_DECLINE_CONFERENCE", //26
	"YES_LEAVE_CONFERENCE", //27
	"YES_INVITE_CONFERENCE", //28
	"YES_SAY_CONFERENCE", //29
	"YES_CHAT_LOGIN", //30
	"YES_CHAT_LOGOFF", //31
	"YES_CHAT_MSG", //32
	"YES_UNKNOWN", //33
	"YES_UNKNOWN", //34
	"YES_UNKNOWN", //35
	"YES_UNKNOWN", //36
	"YES_UNKNOWN", //37
	"YES_UNKNOWN", //38
	"YES_UNKNOWN", //39
	"YES_GAMES_USER_LOGIN", //40
	"YES_GAMES_USER_LOGOFF", //41
	"YES_GAMES_USER_HAS_MSG", //42
	"YES_UNKNOWN", //43
	"YES_NET2PHONE_STATS", //44
	"YES_UNKNOWN", //45
	"YES_UNKNOWN", //46
	"YES_UNKNOWN", //47
	"YES_UNKNOWN", //48
	"YES_UNKNOWN", //49
	"YES_UNKNOWN", //50
	"YES_ADDRESSBOOK_ALERT", //51
	"YES_UNKNOWN", //52
	"YES_UNKNOWN", //53
	"YES_UNKNOWN", //54
	"YES_UNKNOWN", //55
	"YES_UNKNOWN", //56
	"YES_UNKNOWN", //57
	"YES_UNKNOWN", //58
	"YES_UNKNOWN", //59
	"YES_AUCTION_ALERT", //60
	"YES_UNKNOWN", //61
	"YES_UNKNOWN", //62
	"YES_UNKNOWN", //63
	"YES_UNKNOWN", //64
	"YES_UNKNOWN", //65
	"YES_UNKNOWN", //66
	"YES_UNKNOWN", //67
	"YES_UNKNOWN", //68
	"YES_UNKNOWN", //69
	"YES_USER_FT", //70
	"YES_USER_FT_REPLY", //71
	"YES_USER_CONVERSE", //72
	"YES_USER_WEBTOUR", //73
	"YES_IM_ENABLE_VOICE", //74
	"YES_USER_SEND_MESG", //75
	"YES_SEND_PORT_CHECK", //76
	"YES_SEND_DATA_THRU", //77
	"YES_UNKNOWN", //78
	"YES_P2P_START", //79
	"YES_MSGR_WEBCAM_TOKEN", //80
	"YES_STATS", //81
	"YES_UNKNOWN", //82
	"YES_UNKNOWN", //83
	"YES_USER_LOGIN_2", //84
	"YES_PRELOGIN_DATA", //85
	"YES_GET_COOKIE_DATA", //86
	"YES_HELO", //87
	"YES_FEATURE_NOT_SUPPORTED", //88
};

#define YES_NAME_MAPPING_ADVANCED_BEGIN 131
#define YES_NAME_MAPPING_ADVANCED_END 232

static const char* YES_NAME_MAPPING_ADVANCED[] = {
	"YES_ADD_BUDDY", //131
	"YES_REMOVE_BUDDY", //132
	"YES_MODIFY_IGNORE_LIST", //133
	"YES_DENY_BUDDY_ADD", //134
	"YES_UNKNOWN", //135
	"YES_UNKNOWN", //136
	"YES_RENAME_GROUP", //137
	"YES_KEEP_ALIVE", //138
	"YES_YPC_ADD_FRIEND_APPROVAL", //139
	"YES_CHALENGE", //140
	"YES_ADD_BUDDY_INSTANT_APPROVAL", //141
	"YES_UNKNOWN", //142
	"YES_UNKNOWN", //143
	"YES_UNKNOWN", //144
	"YES_UNKNOWN", //145
	"YES_UNKNOWN", //146
	"YES_UNKNOWN", //147
	"YES_UNKNOWN", //148
	"YES_UNKNOWN", //149
	"YES_CHAT_MSGR_USER_LOGIN", //150
	"YES_CHAT_GOTO_USER", //151
	"YES_CHAT_ROOM_JOIN", //152
	"YES_UNKNOWN", //153
	"YES_UNKNOWN", //154
	"YES_CHAT_ROOM_PART", //155
	"YES_UNKNOWN", //156
	"YES_CHAT_ROOM_INVITE", //157
	"YES_UNKNOWN", //158
	"YES_UNKNOWN", //159
	"YES_CHAT_MSGR_USER_LOGOFF", //160
	"YES_CHAT_PING", //161
	"YES_UNKNOWN", //162
	"YES_UNKNOWN", //163
	"YES_UNKNOWN", //164
	"YES_UNKNOWN", //165
	"YES_UNKNOWN", //166
	"YES_CHAT_WEBCAM_TOKEN", //167
	"YES_CHAT_PUBLIC_MSG", //168
	"YES_CHAT_ROOM_CREATE", //169
	"YES_UNKNOWN", //170
	"YES_UNKNOWN", //171
	"YES_UNKNOWN", //172
	"YES_UNKNOWN", //173
	"YES_UNKNOWN", //174
	"YES_UNKNOWN", //175
	"YES_UNKNOWN", //176
	"YES_UNKNOWN", //177
	"YES_UNKNOWN", //178
	"YES_UNKNOWN", //179
	"YES_UNKNOWN", //180
	"YES_UNKNOWN", //181
	"YES_UNKNOWN", //182
	"YES_GAMES_INVITE", //183
	"YES_GAMES_SEND_DATA", //184
	"YES_EDIT_INVISIBLE_TO_LIST", //185
	"YES_EDIT_VISIBLE_TO_LIST", //186
	"YES_ANTIBOT", //187
	"YES_AVATAR_CHANGED", //188
	"YES_FRIEND_ICON", //189
	"YES_FRIEND_ICON_DOWNLOAD", //190
	"YES_AVATAR_GET_FILE", //191
	"YES_AVATAR_GET_HASH", //192
	"YES_DISPLAY_TYPE_CHANGED", //193
	"YES_FRIEND_ICON_FT", //194
	"YES_GET_COOKIE", //195
	"YES_ADDRESS_BOOK_CHANGED", //196
	"YES_SET_VISIBILITY", //197
	"YES_SET_AWAY_STATUS", //198
	"YES_AVATAR_PREFS", //199
	"YES_VERIFY_USER", //200
	"YES_UNKNOWN", //201
	"YES_UNKNOWN", //202
	"YES_UNKNOWN", //203
	"YES_UNKNOWN", //204
	"YES_UNKNOWN", //205
	"YES_UNKNOWN", //206
	"YES_UNKNOWN", //207
	"YES_AUDIBLE", //208
	"YES_UNKNOWN", //209
	"YES_IM_PANEL_FEATURE", //210
	"YES_SHARE_CONTACTS", //211
	"YES_IM_SESSION", //212
	"YES_SUBSCRIPTION", //213
	"YES_ADD_BUDDY_AUTHORIZE", //214
	"YES_PHOTO_ADD", //215
	"YES_PHOTO_SELECT", //216
	"YES_PHOTO_DELETE", //217
	"YES_PHOTO_FILE_REQUEST", //218
	"YES_PHOTO_POINTER", //219
	"YES_FXFER_INVITE", //220
	"YES_FXFER_SEND", //221
	"YES_FXFER_RECEIVE", //222
	"YES_UPDATE_CAPABILITY", //223
	"YES_REPORT_SPIM", //224
	"YES_MINGLE_DATA", //225
	"YES_ALERT", //226
	"YES_APP_REGISTRY", //227
	"YES_NEW_USER", //228
	"YES_ACCEPT_MSGR_INVITE", //229
	"YES_MSGR_USAGE", //230
	"YES_BUDDY_MOVE", //231
	"YES_GET_VOICE_CRUMB", //232
};

#endif