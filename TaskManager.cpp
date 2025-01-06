//
// Created by moham on 04/01/2025.
//

#include "TaskManager.h"

void TaskManager::assignTask(const string &personName, const Task &task) {
    bool added = false;
    Task result(task.getPriority(), task.getType(), task.getDescription());
    for (int i = 0; i < 10; ++i) {
        if (persons[i].getName() == personName) {
            added = true;
            result.setId(id);
            id += 1;
            persons[i].assignTask(result);
            break;
        }
        if (persons[i].getName().empty()) {
            Person newPerson(personName);
            persons[i] = newPerson;
            i--;
        }
    }
    if (!added) {
        throw std::runtime_error("There is no place to add");
    }
}

void TaskManager::completeTask(const string &personName) {
    for (int i = 0; i < 10; ++i) {
        if (persons[i].getName() == personName) {
            persons[i].completeTask();
            break;
        }
    }
}


void TaskManager::bumpPriorityByType(TaskType type, int priorityUp) {
    if (priorityUp <= 0) {
        return;
    }

    for (int i = 0; i < 10; ++i) {
        SortedList<Task> updatedTasks = persons[i].getTasks().apply([type, priorityUp](const Task& task) {
            if (task.getType() == type) {
                int newPriority = task.getPriority() + priorityUp;
                if (newPriority > 100) {
                    newPriority = 100;
                }
                Task result(newPriority, task.getType(), task.getDescription());
                result.setId(task.getId());
                return result;
            }
            return task;
        });
        persons[i].setTasks(updatedTasks);
    }
}




void TaskManager::printAllEmployees() const {

    for(int i = 0; i < 10 && !persons[i].getName().empty(); i++){
        std::cout << persons[i] << std::endl;
    }
}

void TaskManager::printAllTasks() const {
    SortedList<Task> allTasks;
    int i = 0;
    while (i < 10 && !persons[i].getName().empty()) {
        SortedList<Task>::ConstIterator it = persons[i].getTasks().begin();
        while (it != persons[i].getTasks().end()) {
            allTasks.insert(*it);
            ++it;
        }
        i++;
    }
    SortedList<Task>::ConstIterator it = allTasks.begin();
    while (it != allTasks.end()) {
        std::cout << *it << std::endl;
        ++it;
    }
}


void TaskManager::printTasksByType(TaskType type) const {
    SortedList<Task> allTasks;

    for (int i = 0; i < 10 && !persons[i].getName().empty(); ++i) {
        SortedList<Task> filteredTasks = persons[i].getTasks().filter([type](const Task& task) {
            return task.getType() == type;
        });

        for (SortedList<Task>::ConstIterator it = filteredTasks.begin(); it != filteredTasks.end(); ++it) {
            allTasks.insert(*it);
        }
    }

    for (SortedList<Task>::ConstIterator it = allTasks.begin(); it != allTasks.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

