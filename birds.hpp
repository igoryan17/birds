//
// Created by igoryan on 03.04.15.
//
#ifndef BIRDS_BIRDS_HPP
#define BIRDS_BIRDS_HPP

#include <string>
#include <sys/types.h>
#include <sys/ipc.h>
#include <assert.h>
#include <sys/sem.h>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
#include <assert.h>
#include <ctime>

const unsigned int second_on_minutes = 10;
const std::string part_name = "Sparrow number ";

class Sparrow;

class Grandmother;

typedef std::mutex my_mutex;

struct Argum {
public:
    int &_food;
    int &_lottery_number;
    const unsigned int _P;

    Argum(int &food, int &lottery_number, int P)
            : _food(food), _lottery_number(lottery_number), _P((unsigned int const) P) {
    }
};

template<class T>
class Clear {
public:
    Clear(std::thread *thread, T *pClass) : m_thread(thread), m_pClass(pClass) { }

    void erase() {
        m_thread->join();
        delete m_pClass;
        delete m_thread;
    }

    T *m_pClass;
private:
    std::thread *m_thread;
};

class Sparrow {
private:
    std::string m_name;
    int m_lottery_number_win;
    unsigned int m_crumbs = 0;
    Argum &m_data;
    my_mutex &m_block;
    static unsigned int count;
    bool m_exit = false;
public:
    bool fight();

    void fly_and_eat(int lottery_number);

    void leave();

    Sparrow(Argum &data, my_mutex &block) :
            m_data(data), m_block(block) {
        m_lottery_number_win = -1;
        m_crumbs = 0;
        count++;
        m_name = part_name + std::to_string(count);
    }

    bool check_exit() {
        return m_exit;
    }

    ~Sparrow();

    friend std::ostream &operator<<(std::ostream &os, const Sparrow *ptr);

    void arrive();

    Clear<Sparrow> *spawn() {
        std::thread *thread = new std::thread(&Sparrow::arrive, this);
        Clear<Sparrow> *clean_sparrow = new Clear<Sparrow>(thread, this);
        return clean_sparrow;
    }
};

class Grandmother {
private:
    const int m_Q;
    unsigned int m_N;
    int &m_food;
    const std::time_t m_start = std::time(NULL);
    std::time_t m_now;
    my_mutex &m_block;
    const unsigned int m_epsilon_sec = 5;
    bool m_exit = false;
public:
    Grandmother(int Q, int N, int &food, my_mutex &block) :
            m_Q(Q * second_on_minutes), m_N((unsigned int) N), m_food(food), m_block(block)
        {
        m_now = std::time(NULL) - m_start;
    }
    bool check_exit() {
        return m_exit;
    }
    ~Grandmother();

    void give_crumbs();

    Clear<Grandmother> *spawn() {
        std::thread *thread = new std::thread(&Grandmother::give_crumbs, this);
        Clear<Grandmother> *pClear = new Clear<Grandmother>(thread, this);
        return pClear;
    }
};

#endif //BIRDS_BIRDS_HPP