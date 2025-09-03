#pragma once
#ifndef DEQUE_H
#define DEQUE_H

#include "Sequences.h"
#include <random>
#include <algorithm>

template <class T>
class Deque : public ArraySequence<T> {
private:
    Sequence<T>* sequence;
public:
    Deque() : sequence(new ArraySequence<T>()) {}

    Deque(T* items, int count) : sequence(new ArraySequence<T>(items, count)) {}

    Deque(const Deque<T>& other) : sequence(other.sequence->Clone()) {}

    ~Deque() {
        delete sequence;
    }

    bool IsEmpty() const {
        return sequence->GetSize() == 0;
    }

    int GetSize() const {
        return sequence->GetSize();
    }

    void PushFront(T item) {
        sequence->Prepend(item);
    }

    void PushBack(T item) {
        sequence->Append(item);
    }

    void Clear() {
        delete this->sequence;
        this->sequence = new ArraySequence<T>();
    }

    T PopFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = sequence->GetFirst();
        Sequence<T>* newSequence = sequence->GetSubSequence(1, sequence->GetSize() - 1);
        delete sequence;
        sequence = newSequence;
        return item;
    }

    T PopBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = sequence->GetLast();
        Sequence<T>* newSequence = sequence->GetSubSequence(0, sequence->GetSize() - 2);
        delete sequence;
        sequence = newSequence;
        return item;
    }

    T PeekFront() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return sequence->GetFirst();
    }

    T PeekBack() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return sequence->GetLast();
    }

    T Get(int index) const {
        return sequence->Get(index);
    }

    void Sort(std::function<bool(const T&, const T&)> comparator = [](const T& a, const T& b) { return a < b; }) {
        if (IsEmpty()) return;

        int size = GetSize();
        for (int i = 0; i < size - 1; ++i) {
            for (int j = 0; j < size - i - 1; ++j) {
                T current = Get(j);
                T next = Get(j + 1);

                if (!comparator(current, next)) {
                    T temp = current;
                    (*sequence)[j] = next;
                    (*sequence)[j + 1] = temp;
                }
            }
        }
    }

    T& operator[](int index) {
        return (*sequence)[index];
    }

    const T& operator[](int index) const {
        return (*sequence)[index];
    }

    Deque<T> GetSubDeque(int startIndex, int endIndex) const {
        Deque<T> result;
        Sequence<T>* subSeq = this->sequence->GetSubSequence(startIndex, endIndex);
        for (int i = 0; i < subSeq->GetSize(); ++i) {
            result.PushBack(subSeq->Get(i));
        }
        delete subSeq;
        return result;
    }

    Deque<T>* Concat(Deque<T>* other) {
        Sequence<T>* concatenated = sequence->Concat(other->sequence);
        Deque<T>* result = new Deque<T>();
        delete result->sequence;
        result->sequence = concatenated;
        return result;
    }

    void Append(T item) override {
        PushBack(item);
    }

    void Prepend(T item) override {
        PushFront(item);
    }

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new Deque<T>(*this);
    }

    void Print() const {
        for (int i = 0; i < GetSize(); ++i) {
            std::cout << Get(i).ToString() << " ";
        }
        std::cout << std::endl;
    }

    std::string ToString() const {
        std::string result;
        for (int i = 0; i < GetSize(); ++i) {
            if (i != 0) result += " ";
            result += std::to_string(Get(i));
        }
        return result;
    }

    int CountInversionsMultiPass() const {
        int count = 0;
        for (int i = 0; i < GetSize(); ++i) {
            for (int j = i + 1; j < GetSize(); ++j) {
                if (Get(i) > Get(j)) {
                    count++;
                }
            }
        }
        return count;
    }

    int CountInversionsSinglePass() const {
        int count = 0;
        LinkedList<T> sortedList;

        for (int i = 0; i < GetSize(); ++i) {
            T current = Get(i);
            int pos = 0;
            bool inserted = false;

            for (auto node = sortedList.GetHead(); node != nullptr; node = node->next) {
                if (node->data > current) {
                    count += sortedList.GetSize() - pos;
                    sortedList.Insert(current, pos);
                    inserted = true;
                    break;
                }
                pos++;
            }

            if (!inserted) {
                sortedList.Append(current);
            }
        }

        return count;
    }

    static Deque<int> Range(int l, int h) {
        Deque<int> result;
        for (int i = l; i <= h; ++i) {
            result.PushBack(i);
        }
        return result;
    }

    Deque<T> Where(std::function<bool(T)> predicate) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); ++i) {
            T item = Get(i);
            if (predicate(item)) {
                result.PushBack(item);
            }
        }
        return result;
    }

    static Deque<int> Factorize(int n) {
        Deque<int> factors;
        if (n < 2) return factors;

        for (int i = 2; i <= n; ++i) {
            while (n % i == 0) {
                factors.PushBack(i);
                n /= i;
            }
        }
        return factors;
    }

    static bool IsPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }

    static Deque<int> PrimeFactorization(int n) {
        Deque<int> factors;
        if (n < 2) return factors;

        while (n % 2 == 0) {
            factors.PushBack(2);
            n /= 2;
        }

        for (int i = 3; i * i <= n; i += 2) {
            while (n % i == 0) {
                factors.PushBack(i);
                n /= i;
            }
        }

        if (n > 2) {
            factors.PushBack(n);
        }

        return factors;
    }

    static Deque<int> SieveOfEratosthenes(int limit) {
        Deque<int> primes;
        if (limit < 2) return primes;

        bool* isPrime = new bool[limit + 1];
        for (int i = 0; i <= limit; ++i) {
            isPrime[i] = true;
        }

        for (int p = 2; p * p <= limit; ++p) {
            if (isPrime[p]) {
                for (int i = p * p; i <= limit; i += p) {
                    isPrime[i] = false;
                }
            }
        }

        for (int p = 2; p <= limit; ++p) {
            if (isPrime[p]) {
                primes.PushBack(p);
            }
        }

        delete[] isPrime;
        return primes;
    }

    static Deque<T> GenerateRandomPermutation(int n) {
        Deque<T> deque;
        if (n <= 0) return deque;

        for (T i = 1; i <= n; ++i) {
            deque.PushBack(i);
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = n; i > 1; --i) {
            std::uniform_int_distribution<> distr(0, i - 1);
            int j = distr(gen);

            if (j == i - 1) continue;

            Deque<T> leftPart = deque.GetSubDeque(0, j - 1);
            Deque<T> middlePart = deque.GetSubDeque(j + 1, i - 2);
            Deque<T> rightPart = deque.GetSubDeque(i, n - 1);
            T elem_j = deque.GetSubDeque(j, j).PeekFront();
            T elem_last = deque.GetSubDeque(i - 1, i - 1).PeekFront();

            deque.Clear();

            while (!leftPart.IsEmpty()) {
                deque.PushBack(leftPart.PopFront());
            }

            deque.PushBack(elem_last);

            while (!middlePart.IsEmpty()) {
                deque.PushBack(middlePart.PopFront());
            }

            deque.PushBack(elem_j);

            while (!rightPart.IsEmpty()) {
                deque.PushBack(rightPart.PopFront());
            }
        }

        return deque;
    }

    Sequence<Sequence<T>>* GetAllSubsequences() const {
        ArraySequence<Sequence<T>>* result = new ArraySequence<Sequence<T>>();
        int n = GetSize();

        result->Append(ArraySequence<T>());

        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                ArraySequence<T>* subseq = new ArraySequence<T>();
                for (int k = i; k <= j; ++k) {
                    subseq->Append(Get(k));
                }
                result->Append(*subseq);
                delete subseq;
            }
        }

        return result;
    }
};

template <typename T>
class SegmentedBufferDeque : public SegmentedList<T> {
public:
    SegmentedBufferDeque() : SegmentedList<T>() {}

    SegmentedBufferDeque(T* items, int count) : SegmentedList<T>(items, count) {}

    SegmentedBufferDeque(const SegmentedBufferDeque<T>& other) : SegmentedList<T>(other) {}

    ~SegmentedBufferDeque() = default;

    bool IsEmpty() const {
        return this->GetSize() == 0;
    }

    void PushFront(const T& item) {
        this->Prepend(item);
    }

    void PushBack(const T& item) {
        this->Append(item);
    }

    SegmentedBufferDeque<T> GetSubSegmentedBufferDeque(int startIndex, int endIndex) {
        SegmentedBufferDeque<T> result;
        Sequence<T>* subSeq = this->GetSubSequence(startIndex, endIndex);
        for (int i = 0; i < subSeq->GetSize(); ++i) {
            result.PushBack(subSeq->Get(i));
        }
        delete subSeq;
        return result;
    }

    T PopFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = this->GetFirst();
        Sequence<T>* newSequence = this->GetSubSequence(1, this->GetSize() - 1);

        this->Clear();
        for (int i = 0; i < newSequence->GetSize(); ++i) {
            this->Append(newSequence->Get(i));
        }

        delete newSequence;
        return item;
    }

    T PopBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = this->GetLast();
        Sequence<T>* newSequence = this->GetSubSequence(0, this->GetSize() - 2);

        this->Clear();
        for (int i = 0; i < newSequence->GetSize(); ++i) {
            this->Append(newSequence->Get(i));
        }

        delete newSequence;
        return item;
    }

    T PeekFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return this->GetFirst();
    }

    T PeekBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return this->GetLast();
    }

    void Sort(std::function<bool(const T&, const T&)> comparator = [](const T& a, const T& b) { return a < b; }) {
        for (int i = 0; i < this->GetSize() - 1; i++) {
            for (int j = 0; j < this->GetSize() - i - 1; j++) {
                if (!comparator(this->Get(j), this->Get(j + 1))) {
                    T temp = this->Get(j);
                    (*this)[j] = this->Get(j + 1);
                    (*this)[j + 1] = temp;
                }
            }
        }
    }

    Sequence<T>* Map(std::function<T(T)> func) override {
        SegmentedBufferDeque<T>* result = new SegmentedBufferDeque<T>();
        for (int i = 0; i < this->GetSize(); i++) {
            result->PushBack(func(this->Get(i)));
        }
        return result;
    }

    SegmentedBufferDeque<T> Where(std::function<bool(T)> predicate) const {
        SegmentedBufferDeque<T> result;
        for (int i = 0; i < this->GetSize(); ++i) {
            T item = this->Get(i);
            if (predicate(item)) {
                result.PushBack(item);
            }
        }
        return result;
    }

    T Reduce(std::function<T(T, T)> func, T initial) const {
        T result = initial;
        for (int i = 0; i < this->GetSize(); i++) {
            result = func(result, this->Get(i));
        }
        return result;
    }

    Optional<int> FindSubsequence(const SegmentedBufferDeque<T>& sub) const {
        if (sub.IsEmpty() || sub.GetSize() > this->GetSize()) {
            return Optional<int>::None();
        }

        for (int i = 0; i <= this->GetSize() - sub.GetSize(); i++) {
            bool match = true;
            for (int j = 0; j < sub.GetSize(); j++) {
                if (this->Get(i + j) != sub.Get(j)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return Optional<int>(i);
            }
        }
        return Optional<int>::None();
    }

    SegmentedBufferDeque<T> Merge(const SegmentedBufferDeque<T>& other) const {
        SegmentedBufferDeque<T> result;
        int i = 0, j = 0;
        while (i < this->GetSize() && j < other.GetSize()) {
            result.PushBack(this->Get(i++));
            result.PushBack(other.Get(j++));
        }
        while (i < this->GetSize()) {
            result.PushBack(this->Get(i++));
        }
        while (j < other.GetSize()) {
            result.PushBack(other.Get(j++));
        }
        return result;
    }

    void Print() const {
        for (int i = 0; i < this->GetSize(); ++i) {
            std::cout << this->Get(i) << " ";
        }
        std::cout << std::endl;
    }

    std::string ToString() const {
        std::string result;
        for (int i = 0; i < this->GetSize(); ++i) {
            if (i != 0) result += " ";
            result += std::to_string(this->Get(i));
        }
        return result;
    }

    int CountInversionsMultiPass() const {
        int count = 0;
        for (int i = 0; i < this->GetSize(); ++i) {
            for (int j = i + 1; j < this->GetSize(); ++j) {
                if (this->Get(i) > this->Get(j)) {
                    count++;
                }
            }
        }
        return count;
    }

    int CountInversionsSinglePass() const {
        int count = 0;
        LinkedList<T> sortedList;

        for (int i = 0; i < this->GetSize(); ++i) {
            T current = this->Get(i);
            int pos = 0;
            bool inserted = false;

            for (auto node = sortedList.GetHead(); node != nullptr; node = node->next) {
                if (node->data > current) {
                    count += sortedList.GetSize() - pos;
                    sortedList.Insert(current, pos);
                    inserted = true;
                    break;
                }
                pos++;
            }

            if (!inserted) {
                sortedList.Append(current);
            }
        }

        return count;
    }

    static SegmentedBufferDeque<int> Range(int l, int h) {
        SegmentedBufferDeque<int> result;
        for (int i = l; i <= h; ++i) {
            result.PushBack(i);
        }
        return result;
    }

    static SegmentedBufferDeque<int> Factorize(int n) {
        SegmentedBufferDeque<int> factors;
        if (n < 2) return factors;

        for (int i = 2; i <= n; ++i) {
            while (n % i == 0) {
                factors.PushBack(i);
                n /= i;
            }
        }
        return factors;
    }

    static bool IsPrime(int n) {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;

        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0) {
                return false;
            }
        }
        return true;
    }

    static SegmentedBufferDeque<int> PrimeFactorization(int n) {
        SegmentedBufferDeque<int> factors;
        if (n < 2) return factors;

        while (n % 2 == 0) {
            factors.PushBack(2);
            n /= 2;
        }

        for (int i = 3; i * i <= n; i += 2) {
            while (n % i == 0) {
                factors.PushBack(i);
                n /= i;
            }
        }

        if (n > 2) {
            factors.PushBack(n);
        }

        return factors;
    }

    static SegmentedBufferDeque<int> SieveOfEratosthenes(int limit) {
        SegmentedBufferDeque<int> primes;
        if (limit < 2) return primes;

        bool* isPrime = new bool[limit + 1];
        for (int i = 0; i <= limit; ++i) {
            isPrime[i] = true;
        }

        for (int p = 2; p * p <= limit; ++p) {
            if (isPrime[p]) {
                for (int i = p * p; i <= limit; i += p) {
                    isPrime[i] = false;
                }
            }
        }

        for (int p = 2; p <= limit; ++p) {
            if (isPrime[p]) {
                primes.PushBack(p);
            }
        }

        delete[] isPrime;
        return primes;
    }

    static SegmentedBufferDeque<int> GenerateRandomPermutation(int n) {
        SegmentedBufferDeque<int> deque;
        if (n <= 0) return deque;
        for (int i = 1; i <= n; ++i) {
            deque.PushBack(i);
        }

        std::random_device rd;
        std::mt19937 gen(rd());

        for (int i = n; i > 1; --i) {
            std::uniform_int_distribution<> distr(0, i - 1);
            int j = distr(gen);

            if (j == i - 1) continue;

            auto leftPart = deque.GetSubSegmentedBufferDeque(0, j - 1);
            auto middlePart = deque.GetSubSegmentedBufferDeque(j + 1, i - 2);
            auto rightPart = deque.GetSubSegmentedBufferDeque(i, n - 1);
            int elem_j = deque.GetSubSegmentedBufferDeque(j, j).PeekFront();
            int elem_last = deque.GetSubSegmentedBufferDeque(i - 1, i - 1).PeekFront();

            deque.Clear();

            while (!leftPart.IsEmpty()) {
                deque.PushBack(leftPart.PopFront());
            }

            deque.PushBack(elem_last);

            while (!middlePart.IsEmpty()) {
                deque.PushBack(middlePart.PopFront());
            }

            deque.PushBack(elem_j);

            while (!rightPart.IsEmpty()) {
                deque.PushBack(rightPart.PopFront());
            }
        }

        return deque;
    }

    Sequence<Sequence<T>>* GetAllSubsequences() const {
        ArraySequence<Sequence<T>>* result = new ArraySequence<Sequence<T>>();
        int n = this->GetSize();

        result->Append(ArraySequence<T>());

        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                ArraySequence<T>* subseq = new ArraySequence<T>();
                for (int k = i; k <= j; ++k) {
                    subseq->Append(this->Get(k));
                }
                result->Append(*subseq);
                delete subseq;
            }
        }

        return result;
    }
};

#endif // DEQUE_H