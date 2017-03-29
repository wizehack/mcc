#ifndef MCHUBD_TYPE_H_
#define MCHUBD_TYPE_H_

/* data structure */
#include <map>
#include <vector>
#include <list>
#include <algorithm>

/* json */
#include <json-c/json.h>
#include <json-c/bits.h>
#include <json-c/linkhash.h> //lh_entry

/* utils */
#include <string>
#include <memory>

/* singleton */
#include <mutex>
#include <atomic>

/* thread */
#include <chrono>
#include <thread>

/* shared memory */
#include <sys/ipc.h>
#include <sys/shm.h>

/* log */
#include <iostream>
#include "testStub.h"

namespace mcHubd {
    typedef enum mType{
        REQ_REG_CLIENT = 0,
        REQ_REG_CHANNEL,
        REQ_DEL_CLIENT,
        REQ_DEL_CHANNEL,
        REQ_GET_CHANNEL
    } MSGTYPE;

    typedef enum reason {
        NOTI_CHANNEL_OPEN,
        NOTI_CHANNEL_READY,
        NOTI_CHANNEL_CLOSE
    } CONTRACTREASON;

    typedef enum status {
        OPEN = 0,
        READY,
        CLOSE
    } STATUS;

    typedef enum respCode {
        MCHUBD_OK = 0,
        MCHUBD_INVALID_MSG,
        MCHUBD_NOT_ACCEPTABLE_KEY,
        MCHUBD_IS_CREATED_KEY,
        MCHUBD_IS_AVAILABLE_KEY,
        MCHUBD_IS_NOT_AVAILABLE_KEY,
        MCHUBD_CREATE_CHANNEL_ERROR,
        MCHUBD_INFORM_CHANNEL_ERROR,
        MCHUBD_INTERNAL_ERROR,
        MCHUBD_EXCEEDED_MAXIMUM_CLIENT_KEY
    } RESPCODE;
}

#endif /* MCHUBD_TYPE_H_ */
