//
// Created by igoryan on 03.04.15.
//
#include "birds.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>

std::mutex output;

unsigned int Sparrow::count = 0;

std::ostream &operator<<(std::ostream &os, const Sparrow *ptr) {
    os << ptr->m_name << std::endl;
    os << "crumbs:" << ptr->m_crumbs << std::endl;
    os << "lot number_w:" << ptr->m_lottery_number_win << std::endl;
    os << "lot_number:" << ptr->m_data._lottery_number << std::endl;
    return os;
}

Sparrow::~Sparrow() {
    count--;
    output.lock();
    std::cout << m_name << " destructor call" << std::endl;
    std::cout << std::endl;
    output.unlock();
}

bool Sparrow::fight() {
    output.lock();
    std::cout << "call function fight\n";
    std::cout << m_name << std::endl;
    std::cout << "process number:" << std::this_thread::get_id() << std::endl;
    std::cout << std::endl;
    output.unlock();
    if (m_data._food == 0)
        return true;
    if (m_data._food < 0) {
        output.lock();
        std::cout << "no has food\n";
        std::cout << std::endl;
        output.unlock();
        return false;
    }
    if (m_lottery_number_win == m_data._lottery_number) {
        output.lock();
        std::cout << "he is winner last fighting\n";
        std::cout << std::endl;
        output.unlock();
        return false;
    }
    m_data._lottery_number++;
    output.lock();
    std::cout << m_name << " win lottery" << std::endl;
    std::cout << std::endl;
    output.unlock();
    m_data._food--;
    fly_and_eat(m_data._lottery_number);
    return true;
}

void Sparrow::fly_and_eat(int lottery_number) {
    m_crumbs++;
    m_lottery_number_win = lottery_number;
}

void Sparrow::leave() {
    output.lock();
    std::cout << "call function leave crumbs=" << m_crumbs << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << std::endl;
    output.unlock();
    output.lock();
    std::cout << "leave:\n" << this << "leave end" << std::endl;
    std::cout << std::endl;
    output.unlock();
    m_exit = true;
}

void Sparrow::arrive() {
    output.lock();
    std::cout << m_name << " arrived" << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << std::endl;
    output.unlock();
    while (1) {
        if (m_crumbs >= m_data._P) {
            output.lock();
            std::cout << "crumbs>=P:" << this << std::endl;
            std::cout << std::this_thread::get_id() << std::endl;
            std::cout << std::endl;
            output.unlock();
            leave();
            return;
        }
        if (m_data._food > 0 && m_lottery_number_win != m_data._lottery_number) {
            if (m_block.try_lock()) {
                if (!fight()) {
                    m_block.unlock();
                    leave();
                    return;
                }
                m_block.unlock();
            }
        }
        m_block.lock();
        if (m_data._food < 0) {
            output.lock();
            std::cout << "food end" << std::endl;
            std::cout << std::this_thread::get_id() << std::endl;
            std::cout << std::endl;
            output.unlock();
            m_block.unlock();
            leave();
            return;
        }
        m_block.unlock();
    }
    m_exit = true;
}

Grandmother::~Grandmother() {
    output.lock();
    std::cout << "grandmother finished give crumbs" << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << std::endl;
    output.unlock();
}

void Grandmother::give_crumbs() {
    while (1) {
        m_now = std::time(NULL) - m_start;
        //output.lock();
        //std::cout <<"now=" << m_now <<std::endl;
        //output.unlock();
        if ((m_now % m_Q) <= m_epsilon_sec && m_N > 0) {
            output.lock();
            std::cout << "babka has " << m_N << " crumbs" << std::endl;
            std::cout << std::this_thread::get_id() << std::endl;
            std::cout << std::endl;
            output.unlock();
            m_food++;
            output.lock();
            std::cout << "foods value became " << m_food << std::endl;
            std::cout << std::this_thread::get_id() << std::endl;
            std::cout << std::endl;
            output.unlock();
            m_N--;
            sleep(m_Q - m_epsilon_sec);
        }
        if (m_N <= 0 && m_food <= 0) {
            m_food = -1;
            m_exit = true;
            return;
        }
    }
    m_exit = true;
}