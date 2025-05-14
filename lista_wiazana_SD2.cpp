#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

template <typename T>
class PriorityQueue {
private:
    struct Node {
        T value;
        int priority;
        Node* next;
        Node(T v, int p) : value(v), priority(p), next(nullptr) {}
    };

    Node* head;
    int count;

public:
    PriorityQueue() : head(nullptr), count(0) {}

    ~PriorityQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T value, int priority) {
        Node* newNode = new Node(value, priority);
        if (!head || priority > head->priority) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next && current->next->priority >= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        count++;
    }

    void dequeue() {
        if (isEmpty()) {
            std::cerr << "Kolejka jest pusta!\n";
            return;
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
    }

    int size() const {
        return count;
    }

    T peek() const {
        if (isEmpty()) {
            throw std::runtime_error("Kolejka jest pusta!");
        }
        return head->value;
    }

    bool changePriority(T value, int newPriority) {
        Node* prev = nullptr;
        Node* current = head;

        while (current && current->value != value) {
            prev = current;
            current = current->next;
        }

        if (!current) return false;

        if (prev) prev->next = current->next;
        else head = current->next;
        delete current;
        count--;

        enqueue(value, newPriority);
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void print() const {
        Node* current = head;
        while (current) {
            std::cout << "(" << current->value << ", " << current->priority << ") -> ";
            current = current->next;
        }
        std::cout << "NULL\n";
    }
};

int main() {
    const int numTests = 100;
    const int sizeOfQueue = 10000;
    long long totalEnqueue = 0, totalDequeue = 0, totalSize = 0, totalPeek = 0, totalChangePriority = 0;

    srand(time(nullptr));

    for (int test = 0; test < numTests; ++test) {
        PriorityQueue<int> pq;

        for (int i = 0; i < sizeOfQueue; i++) {
            int priority = rand() % sizeOfQueue + 1;
            pq.enqueue(i, priority);
        }

        auto startEnqueue = high_resolution_clock::now();
        pq.enqueue(3, sizeOfQueue / 2);
        auto endEnqueue = high_resolution_clock::now();
        totalEnqueue += duration_cast<nanoseconds>(endEnqueue - startEnqueue).count();

        auto startDequeue = high_resolution_clock::now();
        pq.dequeue();
        auto endDequeue = high_resolution_clock::now();
        totalDequeue += duration_cast<nanoseconds>(endDequeue - startDequeue).count();

        auto startSize = high_resolution_clock::now();
        pq.size();
        auto endSize = high_resolution_clock::now();
        totalSize += duration_cast<nanoseconds>(endSize - startSize).count();

        auto startPeek = high_resolution_clock::now();
        pq.peek();
        auto endPeek = high_resolution_clock::now();
        totalPeek += duration_cast<nanoseconds>(endPeek - startPeek).count();

        auto startChange = high_resolution_clock::now();
        pq.changePriority(sizeOfQueue / 2, sizeOfQueue + 5);
        auto endChange = high_resolution_clock::now();
        totalChangePriority += duration_cast<nanoseconds>(endChange - startChange).count();
    }

    cout << "Średni czas dodawania elementu: " << (totalEnqueue / numTests) << " ns\n";
    cout << "Średni czas usuwania elementu: " << (totalDequeue / numTests) << " ns\n";
    cout << "Średni czas zwracania rozmiaru: " << (totalSize / numTests) << " ns\n";
    cout << "Średni czas podejrzenia elementu (peek): " << (totalPeek / numTests) << " ns\n";
    cout << "Średni czas zmiany priorytetu: " << (totalChangePriority / numTests) << " ns\n";

    return 0;
}