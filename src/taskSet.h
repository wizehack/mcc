#ifndef TASK_SET_H_
#define TASK_SET_H_

#include "mcHubType.h"

namespace mcHubd {
    class TaskSet {
        public:
            ~TaskSet();
            static TaskSet* getInstance();
            void addWaitToReadyTask(std::string cKey);
            void rmWaitToReady(std::string& cKey);
            bool isWaitingTask(std::string& cKey);
            void clearWaitingTask();

        private:
            TaskSet();

        private:
            std::list <std::string> m_waitToReadyList; //client key list
            static std::atomic<TaskSet*> _singleton;
            static std::mutex _mutex;
    };
}

#endif /*TASK_SET_H_*/
