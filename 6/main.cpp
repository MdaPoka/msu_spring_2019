#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <cstdio>

const int magic_numb = 4;

auto read_numbers(std::ifstream &file, int amount){
    std::vector<int64_t> vec;
    int64_t tmp;
    for(int i = 0; i < amount; ++i) {
        file.read((char*)(&tmp), sizeof(tmp));
        if(file.eof()) {
            return vec;
        }
        vec.push_back(tmp);
    }
    return vec;
}

void concat(std::string &file_name1, std::string &file_name2, std::string &result) {
    std::ifstream f1(file_name1, std::ios::binary);
    std::ifstream f2(file_name2, std::ios::binary);
    std::ofstream res(result, std::ios::binary);
    int64_t x, y;
    f1.read((char*)(&x), sizeof(x));
    f2.read((char*)(&y), sizeof(y));

    while(!f1.eof() && !f2.eof()) {
        if(x > y) {
            res.write((char*)(&y), sizeof(y));
            f2.read((char*)(&y), sizeof(y));
        } else {
            res.write((char*)(&x), sizeof(x));
            f1.read((char*)(&x), sizeof(x));
        }
    }
    std::ifstream* final = &f1;
    if(f1.eof()) {
        final = &f2;
        std::swap(x,y);
    }
    while(!(*final).eof()) {
        res.write((char*)(&x), sizeof(x));
        (*final).read((char*)(&x), sizeof(x));
    }
    f1.close();
    f2.close();
    res.close();
}

void partial_sort(std::ifstream &file, std::mutex &mtx, std::condition_variable &cv) {
    std::thread::id thread_id = std::this_thread::get_id();
    int number_of_files = 0;
    
    std::ostringstream ss;
    ss<<thread_id<<"_";
    
    while(!file.eof()){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return true;});
        
        std::vector<int64_t> vec = read_numbers(file, magic_numb);
        if(!vec.size()) {
            lock.unlock();
            cv.notify_one();
            break;
        }
        std::sort(vec.begin(), vec.end());
        lock.unlock();
        cv.notify_one();

        std::ofstream out(ss.str()+std::to_string(++number_of_files), std::ios::binary);
        for(int i = 0 ;i < vec.size(); ++i) {
            int64_t x = vec[i];
            out.write((char*)(&x), sizeof(x));
        }
        std::this_thread::yield();
    }

    while (number_of_files > 1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return true;});
        std::string f1 = ss.str()+std::to_string(number_of_files--);
        std::string f2 = ss.str()+std::to_string(number_of_files);
        std::string tmp = ss.str()+std::string("tmp");
        concat(std::ref(f1),std::ref(f2),std::ref(tmp));
        lock.unlock();
        cv.notify_one();   
        std::remove(f1.c_str());
        std::remove(f2.c_str());
        std::rename(tmp.c_str(), f2.c_str());
        std::this_thread::yield();
    }
}

int main(int argc, char **argv)
{
    // char* filename = argv[1];
    char filename[] = "hello";
    std::ofstream mda(filename, std::ios::binary);
    for(int64_t i = 26; i > 0; --i) {
        mda.write((char*)(&i), sizeof(i));
    }
    mda.close();


    std::ifstream file(filename, std::ios::binary);
    std::mutex mtx;
    std::condition_variable cv;
    std::thread t1(partial_sort, std::ref(file), std::ref(mtx), std::ref(cv));
    std::thread t2(partial_sort, std::ref(file), std::ref(mtx), std::ref(cv));
    
    std::ostringstream ss;
    ss<<t1.get_id()<<"_1";
    std::string part_1 = ss.str();
    ss.str("");
    ss<<t2.get_id()<<"_1";
    std::string part_2 = ss.str();
    std::string res("result");

    t1.join();
    t2.join();

    file.close();
    concat(part_1, part_2, res);
    std::remove(part_1.c_str());
    std::remove(part_2.c_str());
    
    std::remove(filename);
    return 0;
}
