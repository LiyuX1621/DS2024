#include <iostream>
#include <vector>
#include <complex>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <chrono>

class Complex {
public:
    double real, imag;

    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    double modulus() const {
        return std::sqrt(real * real + imag * imag);
    }

    bool operator==(const Complex& other) const {
        return real == other.real && imag == other.imag;
    }

    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};
void shuffle(std::vector<Complex>& vec) {
    std::random_shuffle(vec.begin(), vec.end());
}



void unique(std::vector<Complex>& vec) {
    std::sort(vec.begin(), vec.end(), [](const Complex& a, const Complex& b) {
        return (a.real < b.real) || (a.real == b.real && a.imag < b.imag);
        });
    auto last = std::unique(vec.begin(), vec.end());
    vec.erase(last, vec.end());
}

std::vector<Complex> search(const std::vector<Complex>& vec, const Complex& target) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        if (c == target) result.push_back(c);
    }
    return result;
}

void insert(std::vector<Complex>& vec, const Complex& c) {
    vec.push_back(c);
}

Complex remove(std::vector<Complex>& vec, size_t index) {
    if (index < vec.size()) {
        Complex removed = vec[index];
        vec.erase(vec.begin() + index);
        return removed;
    }
    return Complex();
}

void bubbleSort(std::vector<Complex>& vec) {
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (vec[j].modulus() > vec[j + 1].modulus() ||
                (vec[j].modulus() == vec[j + 1].modulus() && vec[j].real > vec[j + 1].real)) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void bubbleSort_desc(std::vector<Complex>& vec) {//降序
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        for (size_t j = 0; j < vec.size() - i - 1; ++j) {
            if (vec[j].modulus() < vec[j + 1].modulus() ||
                (vec[j].modulus() == vec[j + 1].modulus() && vec[j].real < vec[j + 1].real)) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Complex> L(vec.begin() + left, vec.begin() + mid + 1);
    std::vector<Complex> R(vec.begin() + mid + 1, vec.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].modulus() < R[j].modulus() ||
            (L[i].modulus() == R[j].modulus() && L[i].real < R[j].real)) {
            vec[k++] = L[i++];
        }
        else {
            vec[k++] = R[j++];
        }
    }

    while (i < n1) vec[k++] = L[i++];
    while (j < n2) vec[k++] = R[j++];
}

void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        merge(vec, left, mid, right);
    }
}

std::vector<Complex> rangeSearch(const std::vector<Complex>& vec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& c : vec) {
        double mod = c.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(c);
        }
    }
    return result;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(0)));

    // 随机生成复数向量
    std::vector<Complex> vec;
    for (int i = 0; i < 20; ++i) {
        vec.emplace_back(static_cast<double>(std::rand() % 10), static_cast<double>(std::rand() % 10));
    }
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    // 测试置乱、查找、插入、删除和唯一化
    shuffle(vec);
    std::cout << "Shuffled Vector: ";
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    Complex target(2, 3); // 示例查找目标
    auto found = search(vec, target);
    std::cout << "Found: ";
    for (const auto& c : found) std::cout << c << " ";
    std::cout << "\n";

    insert(vec, Complex(2, 3)); // 插入元素
    std::cout << "After Insertion: ";
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    remove(vec, 0); // 删除第一个元素
    std::cout << "After Removal: ";
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    unique(vec);
    std::cout << "After Unique: ";
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    // 排序效率比较
    shuffle(vec);
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";
    std::vector<Complex> vec1 = vec;
    std::vector<Complex> vec2 = vec;
    std::vector<Complex> vec3 = vec;
    bubbleSort_desc(vec3);
    std::cout << "After Descending: ";
    for (const auto& c : vec3) std::cout << c << " ";
    std::cout << "\n";
    std::vector<Complex> vec4 = vec3;
    std::vector<Complex> vec5 = vec3;
    auto start = std::chrono::high_resolution_clock::now();//初次使用时
    bubbleSort(vec4);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;//初次使用
    std::cout << "Bubble Sort Time(desc): " << elapsed.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    mergeSort(vec5, 0, vec5.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Merge Sort Time(desc): " << elapsed.count() << " seconds\n";
    /*clock_t start = clock();
    bubbleSort(vec1);
    for (const auto& c : vec1) std::cout << c << " ";
    std::cout << "\n";
    clock_t end = clock();
    std::cout << "Bubble Sort Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";

    start = clock();
    for (const auto& c : vec2) std::cout << c << " ";
    std::cout << "\n";
    mergeSort(vec2, 0, vec2.size() - 1);
    end = clock();
    std::cout << "Merge Sort Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << " seconds\n";
  */
    start = std::chrono::high_resolution_clock::now();
    bubbleSort(vec1);
    end = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> 
    elapsed = end - start;
    std::cout << "Bubble Sort Time(unsort): " << elapsed.count() << " seconds\n";

    start = std::chrono::high_resolution_clock::now();
    mergeSort(vec2, 0, vec2.size() - 1);
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Merge Sort Time(unsort): " << elapsed.count() << " seconds\n";
    // 区间查找
    double m1 = 2.0, m2 = 5.0;
    auto rangeResult = rangeSearch(vec, m1, m2);
    std::cout << "Range Search Result: ";
    for (const auto& c : rangeResult) std::cout << c << " ";
    std::cout << "\n";

    return 0;
}
