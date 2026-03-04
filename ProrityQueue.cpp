#include <iostream>
using namespace std;

class Node {
public:
    int value;
    Node* next;
    Node* prev;

    Node(int v) {
        value = v;
        next = nullptr;
        prev = nullptr;
    }
};

class PriorityQueue {
private:
    Node* head = nullptr;  // largest value
    Node* tail = nullptr;  // smallest value

public:
    bool empty() {
        return head == nullptr;
    }

    // Insert while keeping list sorted (descending order)
    void push(int x) {
        Node* temp = new Node(x);

        if (head == nullptr) {
            head = tail = temp;
        }
        // Insert at beginning (largest priority)
        else if (x > head->value) {
            temp->next = head;
            head->prev = temp;
            head = temp;
        }
        // Insert at end (smallest priority)
        else if (x <= tail->value) {
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        else {
            Node* current = head->next;

            while (x <= current->value) {
                current = current->next;
            }

            temp->prev = current->prev;
            temp->next = current;
            current->prev->next = temp;
            current->prev = temp;
        }
    }

    int top() {
        if (empty()) {
            cout << "Queue is empty\n";
            return -1;
        }
        return head->value; // highest priority
    }

    void pop() {
        if (empty()) {
            cout << "Queue is empty\n";
            return;
        }

        Node* temp = head;
        head = head->next;

        if (head != nullptr)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete temp;
    }

    void print() {
        for (Node* k = head; k != nullptr; k = k->next) {
            cout << k->value << " ";
        }
        cout << endl;
    }

    ~PriorityQueue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    PriorityQueue pq;

    pq.push(10);
    pq.push(40);
    pq.push(15);
    pq.push(30);
    pq.push(50);

    pq.print();

    while (!pq.empty()) {
        cout << pq.top() << " ";
        pq.pop();
    }

    return 0;
}
