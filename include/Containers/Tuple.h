#pragma once

template <typename T1, typename T2>
struct Tuple {
    
    Tuple() {

    }

    Tuple (T1 t1, T2 t2) {
        this->t1 = t1;
        this->t2 = t2;
    }

    T1 t1;
    T2 t2;
};