#include <iostream>
#include <stdexcept>
#include <chrono>

using namespace std;
using namespace std::chrono;

class HeapPriorityQueue {
private:
    struct Node {
        int priority;
        int value;
    };

    Node* heap;
    int capacity;
    int count;

    void resize() {
        int newCapacity = capacity * 2;
        Node* newHeap = new Node[newCapacity];
        for (int i = 0; i < count; ++i) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }

    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (heap[index].priority > heap[parent].priority) {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else break;
        }
    }

    void heapifyDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < count && heap[left].priority > heap[largest].priority)
                largest = left;
            if (right < count && heap[right].priority > heap[largest].priority)
                largest = right;

            if (largest != index) {
                swap(heap[index], heap[largest]);
                index = largest;
            }
            else break;
        }
    }

    int findIndex(int value) const {
        for (int i = 0; i < count; ++i) {
            if (heap[i].value == value) return i;
        }
        return -1;
    }

public:
    HeapPriorityQueue(int initialCapacity = 16) : capacity(initialCapacity), count(0) {
        heap = new Node[capacity];
    }

    ~HeapPriorityQueue() {
        delete[] heap;
    }

    void push(int value, int priority) {
        if (count >= capacity) resize();

        heap[count] = { priority, value };
        heapifyUp(count);
        count++;
    }

    void pop() {
        if (count == 0) throw runtime_error("Kolejka jest pusta");

        heap[0] = heap[count - 1];
        count--;
        heapifyDown(0);
    }

    int peek() const {
        if (count == 0) throw runtime_error("Kolejka jest pusta");
        return heap[0].value;
    }

    int size() const {
        return count;
    }

    void changePriority(int value, int newPriority) {
        int index = findIndex(value);
        if (index == -1) throw runtime_error("Element nie istnieje");

        int oldPriority = heap[index].priority;
        heap[index].priority = newPriority;

        if (newPriority > oldPriority)
            heapifyUp(index);
        else
            heapifyDown(index);
    }

    void clear() {
        count = 0;
    }

    int getPriority(int value) const {
        int index = findIndex(value);
        if (index == -1) throw runtime_error("Element nie istnieje");
        return heap[index].priority;
    }
};


long long measurePushSingle(HeapPriorityQueue& queue) {
    auto start = high_resolution_clock::now();
    queue.push(999999, rand() % 1000);
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

long long measureChange(HeapPriorityQueue& queue) {
    auto start = high_resolution_clock::now();
    queue.changePriority(500, 10000);
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

long long measurePeek(HeapPriorityQueue& queue) {
    auto start = high_resolution_clock::now();
    queue.peek();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

long long measurePopSingle(HeapPriorityQueue& queue) {
    auto start = high_resolution_clock::now();
    queue.pop();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

long long measureSize(HeapPriorityQueue& queue) {
    auto start = high_resolution_clock::now();
    queue.size();
    auto end = high_resolution_clock::now();
    return duration_cast<nanoseconds>(end - start).count();
}

int main() {
    HeapPriorityQueue queue;
    const int testCount = 10;
    const int baseSize = 25000;

    long long totalPushSingle = 0, totalPopSingle = 0, totalPeek = 0, totalChange = 0, totalSize = 0;

    for (int i = 0; i < testCount; ++i) {
        queue.clear();

        for (int j = 0; j < baseSize; ++j) {
            queue.push(j, rand() % 1000);
        }

        totalPushSingle += measurePushSingle(queue);
        totalChange += measureChange(queue);
        totalPeek += measurePeek(queue);
        totalPopSingle += measurePopSingle(queue);
        totalSize += measureSize(queue);
    }

    cout << "Kolejka kopcąca\n";
    cout << "Rozmiar tablicy:                 " << baseSize << "\n";
    cout << "Dodawanie pojedyncze (push):     " << totalPushSingle / testCount << " ns\n";
    cout << "Usuwanie (pop):                  " << totalPopSingle / testCount << " ns\n";
    cout << "Zwracanie rozmiaru:              " << totalSize / testCount << " us\n";
    cout << "Podejrzenie (peek):              " << totalPeek / testCount << " ns\n";
    cout << "Zmiana priorytetu:               " << totalChange / testCount << " us\n";

    return 0;
}