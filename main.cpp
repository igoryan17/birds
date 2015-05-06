#include <iostream>
#include <algorithm>
#include "birds.hpp"

const unsigned int Q = 1;
const unsigned int N = 3;
const unsigned int P = 1;
const unsigned int M = 2;
const unsigned int MaxTime = second_on_minutes * 5;
const unsigned int Sleep = 40;
const unsigned int epsilon = 4;

int main() {
    my_mutex block;
    int food = 0;
    int lottery_number;
    lottery_number = 0;
    Argum data(food, lottery_number, P);
    //
    //initialisation data
    //
    //make repository for threads
    std::vector<Clear<Sparrow> *> threads;
    //add M sparrow
    const std::time_t start = std::time(NULL);
    std::time_t now = std::time(NULL) - start;
    for (int i = 0; i < (int) M; i++) {
        sleep(second_on_minutes);
        Sparrow *spar = new Sparrow(data, block);
        threads.push_back(spar->spawn());
    }
    //initialization Grandmother
    Grandmother *grandmother;
    grandmother = new Grandmother(Q, N, food, block);
    //do method give crumbs
    Clear<Grandmother> *grand = grandmother->spawn();
    //add new sparrow when time changed
    while (now < MaxTime && data._food >= 0) {
        now = std::time(NULL) - start;
        if (epsilon > now % second_on_minutes) {
            Sparrow *tmp;
            tmp = new Sparrow(data, block);
            threads.push_back(tmp->spawn());
        }
        sleep(Sleep);
    }
    std::vector<Clear<Sparrow> *>::iterator i = threads.begin();
    while (threads.size() > 0) {
        if (i == threads.end()) {
            if (threads.size() > 0) {
                i = threads.begin();
            }
            else {

            }
        }
        if ((*i)->m_pClass->check_exit()) {
            (*i)->erase();
            delete (*i);
            threads.erase(i);
            if (threads.size() <= 0) {
                break;
            }
            i = threads.begin();
            sleep(Q * second_on_minutes * P - epsilon);
        }
        else {
            i++;
        }
    }
    grand->erase();
    delete grand;
    return 0;
}