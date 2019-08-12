#include <iostream>
#include <tuple>
#include <map>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus
{
    NEW,         // новая
    IN_PROGRESS, // в разработке
    TESTING,     // на тестировании
    DONE         // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks
{
  public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo &GetPersonTasksInfo(const string &person) const{
        return db.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string &person){
        db[person][TaskStatus::NEW] += 1;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count){
        TasksInfo upd;
        TasksInfo nupd = db[person];
        
        if (db.count(person) == 0){
            return make_tuple(upd, nupd);
        }

        TasksInfo& t = db[person];

        int task_num = t[TaskStatus::NEW] + t[TaskStatus::IN_PROGRESS] + t[TaskStatus::TESTING];
        task_count = min(task_count, task_num);

        for(int i = 0; i < 3; ++i){
            TaskStatus cur = (TaskStatus)i;
            TaskStatus next = (TaskStatus)(i+1);
            while(task_count > 0 && nupd[cur] > 0){
                --t[cur];
                ++t[next];
                ++upd[next];
                --nupd[cur];
                --task_count;
            }
        }
        clean(t);
        clean(upd);
        clean(nupd);
        nupd.erase(TaskStatus::DONE);
        return make_tuple(upd, nupd);
    }

  private:
    void clean(TasksInfo tasks){
        for(int i = 0; i < 4; ++i){
            TaskStatus ts = (TaskStatus)i;
            if (tasks[ts] == 0){
                tasks.erase(ts);
            }
        }
    }
    map<string, TasksInfo> db;
};