#include "numbers.dat"
#include <iostream>

const int MIN_VAL = 0;
const int MAX_VAL = 100000;

constexpr bool isprime(int val);
constexpr int prime_count(int min, int max);

const int prime_arr_size = prime_count(MIN_VAL, MAX_VAL);
int* gen_prime_arr(int min, int max);

int binary_search(const int* arr, int size, int val);
int find_bound(int val, char mode);
int count_prime_val(int* prime_arr, int l_bound, int r_bound);

int main(int argc, char** argv) {
    if(!(argc % 2) || argc == 1) {
        return -1;
    }
    int* prime_arr = gen_prime_arr(MIN_VAL,MAX_VAL);
    for(int i = 1; i < argc; i = i + 2) {

        int l_bound = std::atoi(argv[i]);
        int r_bound = std::atoi(argv[i+1]);
        if(l_bound > r_bound){
            std::cout<<int(0)<<std::endl;
            return 0;
        }
        std::cout<<count_prime_val(prime_arr, l_bound, r_bound)<<std::endl;
    }
    return 0;
}

constexpr bool isprime(int val) {
    for(int i = 2; i * i <= val; ++i) {
        if(!(val % i)) {
            return false;
        }
    }
    return true;
}
constexpr int prime_count(int min, int max){
    int count = 0;
    for(int i = min; i <= max; ++i) {
        if(isprime(i) && i && i != 1) {
            ++count;
        }
    }
    return count;
}
int* gen_prime_arr(int min, int max) {
    int* prime_arr = new int[prime_arr_size];
    int curr_idx = 0;
    for(int i = min; i <= max; ++i) {
        if(isprime(i) && i && i != 1) {
            prime_arr[curr_idx] = i;
            ++curr_idx;
        }
    }
    return prime_arr;
}
int binary_search(const int* arr, int size, int val) {
    int l = -1;
    int r = size;
    while(l < r - 1) {
        int x = (l + r) / 2;
        if(arr[x] < val) {
            l = x;
        } else {
            r = x;
        }
    }
    return r == size ? r - 1 : r;
}
int find_bound(int val, char mode) {
    int idx = binary_search(Data, Size, val);
    if(Data[idx] != val) {
        return -1;
    }
    if(mode == 'l') {
        while((idx > 0) && (Data[idx] == Data[idx-1])) {
            --idx;
        }
    } else {
        while((idx < Size - 1) && (Data[idx] == Data[idx+1])) {
            ++idx;
        }
    }
    return idx;
}
int count_prime_val(int* prime_arr, int l_bound, int r_bound) {
    int left_idx = find_bound(l_bound, 'l');
    int right_idx = find_bound(r_bound, 'r');
    if(left_idx == -1 || right_idx == -1) {
        return 0;
    }
    int count = 0;
    int prime_idx = binary_search(prime_arr, prime_arr_size, l_bound);
    while(left_idx <= right_idx) {
        if(Data[left_idx] == prime_arr[prime_idx]) {
            ++count;
            ++left_idx;
        } else if (Data[left_idx] > prime_arr[prime_idx]) {
            ++prime_idx;
        } else {
            ++left_idx;
        }
    }
    return count;
}