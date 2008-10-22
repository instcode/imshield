#ifndef IMSHIELD_H
#define IMSHIELD_H

#include <windows.h>

const LPCTSTR PING_EVENT						= "PingEvent";
const LPCTSTR ALIVE_EVENT						= "AliveEvent";
const LPCTSTR STOP_MONITORING_EVENT				= "StopMonitoringEvent";
const LPCTSTR PROCESS_DETACH_EVENT				= "ProcessDetachEvent";
const LPCTSTR IM_SERVICE_EVENT					= "IMServiceEvent";
const LPCTSTR INCOMMING_BUFFER_AVAILABLE_EVENT	= "IncommingBufferAvailableEvent";
const LPCTSTR OUTGOING_BUFFER_AVAILABLE_EVENT	= "OutgoingBufferAvailableEvent";

#define REQUEST_TIME_WAIT			10000
#define PING_TIME_WAIT				5000

#endif