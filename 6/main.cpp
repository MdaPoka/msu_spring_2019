#include <thread>
#include <fstream>
#include <string>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <cstdio>

const int max_numbers_to_read = 250000;
/* С учетом того, что std::sort может потребоваться дополнительная память,
количество считываемых тредом чисел уменьшено вдвое */


bool file_exist(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

auto read_numbers(std::ifstream &file, int amount){
    std::vector<uint64_t> vec;
    uint64_t tmp;
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
    uint64_t x, y;
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
} // Слияние 2-х файлов с отсортированными числами в один отсортированный файл.

void partial_sort(std::ifstream &file, std::mutex &mtx) {
    std::thread::id thread_id = std::this_thread::get_id();
    int number_of_files = 0;
    
    std::ostringstream ss;
    ss<<thread_id<<"_";
    
    while(!file.eof()){
        std::unique_lock<std::mutex> lock(mtx);
        
        std::vector<uint64_t> vec = read_numbers(file, max_numbers_to_read);
        if(vec.empty()) {
            break;
        }
        lock.unlock();
        std::sort(vec.begin(), vec.end());

        std::ofstream out(ss.str()+std::to_string(++number_of_files), std::ios::binary);
        out.write((char*)vec.data(), vec.size()*sizeof(uint64_t));
        std::this_thread::yield();
    }

    while (number_of_files > 1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::string f1 = ss.str()+std::to_string(number_of_files--);
        std::string f2 = ss.str()+std::to_string(number_of_files);
        std::string tmp = ss.str()+std::string("tmp");
        concat(std::ref(f1),std::ref(f2),std::ref(tmp));
        lock.unlock();
        std::remove(f1.c_str());
        std::remove(f2.c_str());
        std::rename(tmp.c_str(), f2.c_str());
        std::this_thread::yield();
    }
}

std::string get_part_name(std::thread::id id) {
    std::ostringstream ss;
    ss<<id<<"_1";
    return ss.str();
} //Возвращет имя файла, который оставил после себя поток.

int main(int argc, char **argv)
{
    char* filename = argv[1];


    // char filename[] = "hello";
    // std::ofstream mda(filename, std::ios::binary);
    // for(uint64_t i = 26; i > 0; --i) {
    //     mda.write((char*)(&i), sizeof(i));
    // }
    // mda.close();


    std::ifstream file(filename, std::ios::binary);
    std::mutex mtx;
    std::thread t1(partial_sort, std::ref(file), std::ref(mtx));
    std::thread t2(partial_sort, std::ref(file), std::ref(mtx));
    
    std::string part_1 = get_part_name(t1.get_id());
    std::string part_2 = get_part_name(t2.get_id());
    std::string res("result");

    t1.join();
    t2.join();

    file.close();
    if(file_exist(part_1) && file_exist(part_2)) {
        concat(part_1, part_2, res);
        std::remove(part_1.c_str());
        std::remove(part_2.c_str());
    } else {
        if(file_exist(part_1)) {
            std::rename(part_1.c_str(), res.c_str());
        } else {
            std::rename(part_2.c_str(), res.c_str());
        }
    }
    return 0;
}