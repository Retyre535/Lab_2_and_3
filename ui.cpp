#include <wx/wx.h>
#include <wx/listctrl.h>
#include "Sequences.h"
#include "Deques.h"

class SequenceTesterApp : public wxApp {
public:
    virtual bool OnInit();
};

class SequenceTesterFrame : public wxFrame {
public:
    SequenceTesterFrame(const wxString& title);

private:
    wxChoice* sequenceTypeChoice;
    wxTextCtrl* inputText;
    wxButton* testButton;
    wxListCtrl* resultList;

    void OnTestButtonClick(wxCommandEvent& event);
    void TestSequence(wxString sequenceType, wxString input);

    void AddResult(wxString operation, wxString result);
    void AddIntResult(wxString operation, int result);
    void AddBoolResult(wxString operation, bool result);

    DECLARE_EVENT_TABLE()
};

enum {
    ID_TestButton = 1
};

BEGIN_EVENT_TABLE(SequenceTesterFrame, wxFrame)
EVT_BUTTON(ID_TestButton, SequenceTesterFrame::OnTestButtonClick)
END_EVENT_TABLE()

bool SequenceTesterApp::OnInit() {
    SequenceTesterFrame* frame = new SequenceTesterFrame("Sequence Tester");
    frame->Show(true);
    return true;
}

SequenceTesterFrame::SequenceTesterFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(1000, 700)) {

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxStaticText* typeLabel = new wxStaticText(panel, wxID_ANY, "Sequence Type:");
    sequenceTypeChoice = new wxChoice(panel, wxID_ANY);
    sequenceTypeChoice->Append("DynamicArray");
    sequenceTypeChoice->Append("LinkedList");
    sequenceTypeChoice->Append("ArraySequence");
    sequenceTypeChoice->Append("ListSequence");
    sequenceTypeChoice->Append("AdaptiveSequence");
    sequenceTypeChoice->Append("SegmentedList");
    sequenceTypeChoice->Append("MutableArraySequence");
    sequenceTypeChoice->Append("ImmutableArraySequence");
    sequenceTypeChoice->Append("MutableListSequence");
    sequenceTypeChoice->Append("ImmutableListSequence");
    sequenceTypeChoice->Append("Deque");
    sequenceTypeChoice->Append("SegmentedBufferDeque");
    sequenceTypeChoice->SetSelection(0);

    wxBoxSizer* typeSizer = new wxBoxSizer(wxHORIZONTAL);
    typeSizer->Add(typeLabel, 0, wxRIGHT, 10);
    typeSizer->Add(sequenceTypeChoice, 1);
    vbox->Add(typeSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxStaticText* inputLabel = new wxStaticText(panel, wxID_ANY, "Input (comma separated integers):");
    inputText = new wxTextCtrl(panel, wxID_ANY, "1,2,3,4,5", wxDefaultPosition, wxDefaultSize);
    vbox->Add(inputLabel, 0, wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(inputText, 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

    testButton = new wxButton(panel, ID_TestButton, "Run Tests");
    vbox->Add(testButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    resultList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    resultList->InsertColumn(0, "Operation", wxLIST_FORMAT_LEFT, 300);
    resultList->InsertColumn(1, "Result", wxLIST_FORMAT_LEFT, 600);
    vbox->Add(resultList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vbox);
    Center();
}

void SequenceTesterFrame::OnTestButtonClick(wxCommandEvent& event) {
    resultList->DeleteAllItems();

    wxString sequenceType = sequenceTypeChoice->GetStringSelection();
    wxString input = inputText->GetValue();

    TestSequence(sequenceType, input);
}

void SequenceTesterFrame::TestSequence(wxString sequenceType, wxString input) {
    wxArrayString inputItems = wxSplit(input, ',');
    if (inputItems.IsEmpty()) {
        wxMessageBox("Please enter some numbers separated by commas", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int* items = new int[inputItems.GetCount()];
    for (size_t i = 0; i < inputItems.GetCount(); i++) {
        long value;
        if (!inputItems[i].ToLong(&value)) {
            wxMessageBox("Invalid number: " + inputItems[i], "Error", wxOK | wxICON_ERROR);
            delete[] items;
            return;
        }
        items[i] = (int)value;
    }

    try {
        if (sequenceType == "DynamicArray") {
            DynamicArray<int> arr(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", arr.GetSize());
            AddIntResult("Element at index 0", arr.Get(0));
            AddIntResult("Element at index 1", arr.Get(1));

            arr.Append(10);
            AddIntResult("After Append(10) size", arr.GetSize());
            AddIntResult("Last element", arr.Get(arr.GetSize() - 1));

            arr.Prepend(0);
            AddIntResult("After Prepend(0) size", arr.GetSize());
            AddIntResult("First element", arr.Get(0));

            arr.Insert(-99, 2);
            AddIntResult("After Insert(-99, 2) size", arr.GetSize());
            AddIntResult("Element at index 2", arr.Get(2));

            arr.Resize(3);
            AddIntResult("After Resize(3) size", arr.GetSize());

            arr.Set(1, 42);
            AddIntResult("After Set(1, 42)", arr.Get(1));

            DynamicArray<int> arrCopy(arr);
            AddIntResult("Copy constructor size", arrCopy.GetSize());
            AddIntResult("Copy element at 0", arrCopy.Get(0));

            arr[0] = 100;
            AddIntResult("After operator[] set", arr[0]);

            try {
                arr.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "LinkedList") {
            LinkedList<int> list(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", list.GetSize());
            AddIntResult("First element", list.GetFirst());
            AddIntResult("Last element", list.GetLast());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());
            AddIntResult("Last element", list.GetLast());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());
            AddIntResult("First element", list.GetFirst());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
            AddIntResult("Element at index 2", list.Get(2));

            LinkedList<int>* subList = list.GetSubList(1, 3);
            AddIntResult("SubList(1,3) size", subList->GetSize());
            delete subList;

            LinkedList<int> listCopy(list);
            AddIntResult("Copy constructor size", listCopy.GetSize());
            AddIntResult("Copy first element", listCopy.GetFirst());

            list[0] = 100;
            AddIntResult("After operator[] set", list[0]);

            LinkedList<int>* concatList = list.Concat(&listCopy);
            AddIntResult("Concat size", concatList->GetSize());
            delete concatList;

            try {
                list.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "ArraySequence") {
            ArraySequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ArraySequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            try {
                seq.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "ListSequence") {
            ListSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ListSequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            try {
                seq.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "AdaptiveSequence") {
            AdaptiveSequence<int> seq(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());
            AddIntResult("Last element", seq.GetLast());

            seq.Append(10);
            AddIntResult("After Append(10) size", seq.GetSize());
            AddIntResult("Last element", seq.GetLast());

            seq.Prepend(0);
            AddIntResult("After Prepend(0) size", seq.GetSize());
            AddIntResult("First element", seq.GetFirst());

            seq.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq.GetSize());
            AddIntResult("Element at index 2", seq.Get(2));

            if (seq.GetSize() < AdaptiveSequenceSwitch) {
                for (int i = 0; i < AdaptiveSequenceSwitch; i++) {
                    seq.Append(i);
                }
                AddIntResult("After force switch size", seq.GetSize());
                AddResult("Switch check", "Should have switched to list");
            }

            Sequence<int>* subSeq = seq.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            AdaptiveSequence<int> seqCopy(seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            seq[0] = 100;
            AddIntResult("After operator[] set", seq[0]);

            Sequence<int>* concatSeq = seq.Concat(&seqCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            Sequence<int>* mappedSeq = seq.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            int value;
            bool found = seq.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            found = seq.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            Sequence<int>* fromSeq = seq.From(seq);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            Sequence<int>* zippedSeq = seq.Zip(seq);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            try {
                seq.Get(100000);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "SegmentedList") {
            SegmentedList<int> list;
            for (size_t i = 0; i < inputItems.GetCount(); i++) {
                list.Append(items[i]);
            }

            AddIntResult("Initial size", list.GetSize());
            AddIntResult("First element", list.GetFirst());
            AddIntResult("Last element", list.GetLast());

            list.Append(10);
            AddIntResult("After Append(10) size", list.GetSize());
            AddIntResult("Last element", list.GetLast());

            list.Prepend(0);
            AddIntResult("After Prepend(0) size", list.GetSize());
            AddIntResult("First element", list.GetFirst());

            list.Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", list.GetSize());
            AddIntResult("Element at index 2", list.Get(2));

            for (int i = 0; i < 50; i++) {
                list.Append(i);
            }
            AddIntResult("After multiple appends size", list.GetSize());

            Sequence<int>* subSeq = list.GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            SegmentedList<int> listCopy(list);
            AddIntResult("Copy constructor size", listCopy.GetSize());
            AddIntResult("Copy first element", listCopy.GetFirst());

            list[0] = 100;
            AddIntResult("After operator[] set", list[0]);

            Sequence<int>* concatSeq = list.Concat(&listCopy);
            AddIntResult("Concat size", concatSeq->GetSize());
            delete concatSeq;

            Sequence<int>* mappedSeq = list.Map([](int x) { return x * 2; });
            AddIntResult("Map (*2) first element", mappedSeq->GetFirst());
            delete mappedSeq;

            int value;
            bool found = list.TryGet(1, value);
            AddBoolResult("TryGet(1) success", found);
            if (found) AddIntResult("TryGet(1) value", value);

            found = list.TryFind([](int x) { return x == 99; }, value);
            AddBoolResult("TryFind(x == 99) success", found);
            if (found) AddIntResult("TryFind value", value);

            Sequence<int>* fromSeq = list.From(list);
            AddIntResult("From size", fromSeq->GetSize());
            delete fromSeq;

            Sequence<int>* zippedSeq = list.Zip(list);
            AddIntResult("Zip size", zippedSeq->GetSize());
            delete zippedSeq;

            try {
                list.Get(100000);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test", "Correctly caught IndexOutOfRange");
            }
        }
        else if (sequenceType == "MutableArraySequence") {
            MutableArraySequence<int>* seq = new MutableArraySequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            seq->Append(10);
            AddIntResult("After Append(10) size", seq->GetSize());
            AddIntResult("Last element", seq->GetLast());

            seq->Prepend(0);
            AddIntResult("After Prepend(0) size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());

            seq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq->GetSize());
            AddIntResult("Element at index 2", seq->Get(2));

            MutableArraySequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is same object", (instance == seq));

            Sequence<int>* cloned = seq->Clone();
            AddIntResult("Clone size", cloned->GetSize());
            AddIntResult("Clone first element", cloned->GetFirst());
            delete cloned;

            delete seq;
        }
        else if (sequenceType == "ImmutableArraySequence") {
            ImmutableArraySequence<int>* seq = new ImmutableArraySequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            Sequence<int>* appendedSeq = seq->Clone();
            appendedSeq->Append(10);
            AddIntResult("After Append(10) size", appendedSeq->GetSize());
            AddIntResult("Last element after append", appendedSeq->GetLast());
            delete appendedSeq;

            Sequence<int>* prependedSeq = seq->Clone();
            prependedSeq->Prepend(0);
            AddIntResult("After Prepend(0) size", prependedSeq->GetSize());
            AddIntResult("First element after prepend", prependedSeq->GetFirst());
            delete prependedSeq;

            Sequence<int>* insertedSeq = seq->Clone();
            insertedSeq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", insertedSeq->GetSize());
            AddIntResult("Element at index 2 after insert", insertedSeq->Get(2));
            delete insertedSeq;

            Sequence<int>* subSeq = seq->GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ImmutableArraySequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is different object", (instance != seq));
            delete instance;

            delete seq;
        }
        else if (sequenceType == "MutableListSequence") {
            MutableListSequence<int>* seq = new MutableListSequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            seq->Append(10);
            AddIntResult("After Append(10) size", seq->GetSize());
            AddIntResult("Last element", seq->GetLast());

            seq->Prepend(0);
            AddIntResult("After Prepend(0) size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());

            seq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", seq->GetSize());
            AddIntResult("Element at index 2", seq->Get(2));

            MutableListSequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is same object", (instance == seq));

            Sequence<int>* cloned = seq->Clone();
            AddIntResult("Clone size", cloned->GetSize());
            AddIntResult("Clone first element", cloned->GetFirst());
            delete cloned;

            delete seq;
        }
        else if (sequenceType == "ImmutableListSequence") {
            ImmutableListSequence<int>* seq = new ImmutableListSequence<int>(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", seq->GetSize());
            AddIntResult("First element", seq->GetFirst());
            AddIntResult("Last element", seq->GetLast());

            Sequence<int>* appendedSeq = seq->Clone();
            appendedSeq->Append(10);
            AddIntResult("After Append(10) size", appendedSeq->GetSize());
            AddIntResult("Last element after append", appendedSeq->GetLast());
            delete appendedSeq;

            Sequence<int>* prependedSeq = seq->Clone();
            prependedSeq->Prepend(0);
            AddIntResult("After Prepend(0) size", prependedSeq->GetSize());
            AddIntResult("First element after prepend", prependedSeq->GetFirst());
            delete prependedSeq;

            Sequence<int>* insertedSeq = seq->Clone();
            insertedSeq->Insert(99, 2);
            AddIntResult("After Insert(99, 2) size", insertedSeq->GetSize());
            AddIntResult("Element at index 2 after insert", insertedSeq->Get(2));
            delete insertedSeq;

            Sequence<int>* subSeq = seq->GetSubSequence(1, 3);
            AddIntResult("SubSequence(1,3) size", subSeq->GetSize());
            delete subSeq;

            ImmutableListSequence<int> seqCopy(*seq);
            AddIntResult("Copy constructor size", seqCopy.GetSize());
            AddIntResult("Copy first element", seqCopy.GetFirst());

            Sequence<int>* instance = seq->Instance();
            AddBoolResult("Instance is different object", (instance != seq));
            delete instance;

            delete seq;
        }
        else if (sequenceType == "Deque") {
            Deque<int> deque(items, (int)inputItems.GetCount());

            AddIntResult("Initial size", deque.GetSize());
            AddBoolResult("IsEmpty", deque.IsEmpty());
            AddIntResult("First element (PeekFront)", deque.PeekFront());
            AddIntResult("Last element (PeekBack)", deque.PeekBack());

            //Deque<int> randomPerm = Deque<int>::GenerateRandomPermutation(5);
            //AddResult("Random permutation of size 5", randomPerm.ToString().c_str());

            deque.PushFront(0);
            AddIntResult("After PushFront(0) size", deque.GetSize());
            AddIntResult("First element after PushFront", deque.PeekFront());

            deque.PushBack(10);
            AddIntResult("After PushBack(10) size", deque.GetSize());
            AddIntResult("Last element after PushBack", deque.PeekBack());

            int front = deque.PopFront();
            AddIntResult("Popped front element", front);
            AddIntResult("Size after PopFront", deque.GetSize());

            int back = deque.PopBack();
            AddIntResult("Popped back element", back);
            AddIntResult("Size after PopBack", deque.GetSize());

            AddIntResult("Element at index 1 (Get)", deque.Get(1));
            AddIntResult("Element at index 1 (operator[])", deque[1]);

            Deque<int> subDeque = deque.GetSubDeque(1, deque.GetSize() - 1);
            AddIntResult("SubDeque(1, size-1) size", subDeque.GetSize());

            Deque<int> otherDeque(items, 2);
            Deque<int>* concatDeque = deque.Concat(&otherDeque);
            AddIntResult("ConcatDeque size", concatDeque->GetSize());
            delete concatDeque;

            Deque<int> mappedDeque = deque;
            for (int i = 0; i < mappedDeque.GetSize(); i++) {
                mappedDeque[i] = mappedDeque[i] * 2;
            }
            AddResult("Mapped (*2)", mappedDeque.ToString().c_str());

            Deque<int> filtered = deque.Where([](int x) { return x % 2 == 0; });
            AddResult("Filtered (even)", filtered.ToString().c_str());

            int sum = 0;
            for (int i = 0; i < deque.GetSize(); i++) {
                sum += deque.Get(i);
            }
            AddIntResult("Reduce (sum)", sum);

            deque.Sort();
            AddResult("After default sorting", deque.ToString().c_str());
            deque.Sort([](const int& a, const int& b) { return a > b; });
            AddResult("After descending sorting", deque.ToString().c_str());

            AddIntResult("Inversions (Multi-Pass)", deque.CountInversionsMultiPass());
            AddIntResult("Inversions (Single-Pass)", deque.CountInversionsSinglePass());

            Deque<int> range = Deque<int>::Range(1, 20);
            AddResult("Range(1,20)", range.ToString().c_str());

            Deque<int> primes = range.Where([](int n) { return Deque<int>::IsPrime(n); });
            AddResult("Primes in range", primes.ToString().c_str());

            int numToFactor = 60;
            Deque<int> factors = Deque<int>::Factorize(numToFactor);
            AddResult(wxString::Format("Factors of %d", numToFactor), factors.ToString().c_str());

            Deque<int> primeFactors = Deque<int>::PrimeFactorization(numToFactor);
            AddResult(wxString::Format("Prime factors of %d", numToFactor), primeFactors.ToString().c_str());

            int sieveLimit = 30;
            Deque<int> sievePrimes = Deque<int>::SieveOfEratosthenes(sieveLimit);
            AddResult(wxString::Format("Primes up to %d (Sieve)", sieveLimit), sievePrimes.ToString().c_str());

            deque.Clear();
            AddIntResult("After Clear size", deque.GetSize());
            AddBoolResult("IsEmpty after Clear", deque.IsEmpty());

            try {
                deque.PopFront();
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test PopFront", "Correctly caught IndexOutOfRange on empty deque");
            }

            try {
                deque.PopBack();
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test PopBack", "Correctly caught IndexOutOfRange on empty deque");
            }

            try {
                deque.PeekFront();
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test PeekFront", "Correctly caught IndexOutOfRange on empty deque");
            }

            try {
                deque.PeekBack();
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test PeekBack", "Correctly caught IndexOutOfRange on empty deque");
            }

            try {
                deque.Get(100);
            }
            catch (const IndexOutOfRange& e) {
                AddResult("Exception test Get", "Correctly caught IndexOutOfRange");
            }
            }
        else if (sequenceType == "SegmentedBufferDeque") {
                SegmentedBufferDeque<int> deque;
                for (size_t i = 0; i < inputItems.GetCount(); i++) {
                    deque.PushBack(items[i]);
                }


                AddIntResult("Initial size", deque.GetSize());
                AddBoolResult("IsEmpty", deque.IsEmpty());
                AddIntResult("First element (PeekFront)", deque.PeekFront());
                AddIntResult("Last element (PeekBack)", deque.PeekBack());
                auto randomPermSeg = SegmentedBufferDeque<int>::GenerateRandomPermutation(5);
                AddResult("Random permutation of size 5", randomPermSeg.ToString().c_str());

                deque.PushFront(0);
                AddIntResult("After PushFront(0) size", deque.GetSize());
                AddIntResult("First element after PushFront", deque.PeekFront());

                deque.PushBack(10);
                AddIntResult("After PushBack(10) size", deque.GetSize());
                AddIntResult("Last element after PushBack", deque.PeekBack());

                int front = deque.PopFront();
                AddIntResult("Popped front element", front);
                AddIntResult("Size after PopFront", deque.GetSize());

                int back = deque.PopBack();
                AddIntResult("Popped back element", back);
                AddIntResult("Size after PopBack", deque.GetSize());

                AddIntResult("Element at index 1 (Get)", deque.Get(1));
                AddIntResult("Element at index 1 (operator[])", deque[1]);

                auto subDeque = deque.GetSubSegmentedBufferDeque(1, deque.GetSize() - 1);
                AddIntResult("SubDeque(1, size-1) size", subDeque.GetSize());

                auto mapped = deque.Map([](int x) { return x * 2; });
                auto mappedDeque = dynamic_cast<SegmentedBufferDeque<int>*>(mapped);
                if (mappedDeque) {
                    AddResult("Mapped (*2)", mappedDeque->ToString().c_str());
                }
                delete mapped;

                auto filtered = deque.Where([](int x) { return x % 2 == 0; });
                AddResult("Filtered (even)", filtered.ToString().c_str());

                int sum = deque.Reduce([](int a, int b) { return a + b; }, 0);
                AddIntResult("Reduce (sum)", sum);

                deque.Sort();
                AddResult("After default sorting", deque.ToString().c_str());
                deque.Sort([](const int& a, const int& b) { return a > b; });
                AddResult("After descending sorting", deque.ToString().c_str());

                AddIntResult("Inversions (Multi-Pass)", deque.CountInversionsMultiPass());
                AddIntResult("Inversions (Single-Pass)", deque.CountInversionsSinglePass());

                auto numbers = SegmentedBufferDeque<int>::Range(1, 20);
                AddResult("Range(1,20)", numbers.ToString().c_str());

                auto primes = numbers.Where([](int n) {
                    return SegmentedBufferDeque<int>::IsPrime(n);
                    });
                AddResult("Primes in range", primes.ToString().c_str());

                auto factors = SegmentedBufferDeque<int>::Factorize(60);
                AddResult("Factors of 60", factors.ToString().c_str());

                auto primeFactors = SegmentedBufferDeque<int>::PrimeFactorization(60);
                AddResult("Prime factors of 60", primeFactors.ToString().c_str());

                auto sieve = SegmentedBufferDeque<int>::SieveOfEratosthenes(100);
                AddResult("Primes up to 100 (Sieve)", sieve.ToString().c_str());

                deque.Clear();
                AddIntResult("After Clear size", deque.GetSize());
                AddBoolResult("IsEmpty after Clear", deque.IsEmpty());

                try {
                    deque.PopFront();
                }
                catch (const IndexOutOfRange& e) {
                    AddResult("Exception test PopFront", "Correctly caught IndexOutOfRange on empty deque");
                }

                try {
                    deque.PopBack();
                }
                catch (const IndexOutOfRange& e) {
                    AddResult("Exception test PopBack", "Correctly caught IndexOutOfRange on empty deque");
                }

                try {
                    deque.PeekFront();
                }
                catch (const IndexOutOfRange& e) {
                    AddResult("Exception test PeekFront", "Correctly caught IndexOutOfRange on empty deque");
                }

                try {
                    deque.PeekBack();
                }
                catch (const IndexOutOfRange& e) {
                    AddResult("Exception test PeekBack", "Correctly caught IndexOutOfRange on empty deque");
                }

                try {
                    deque.Get(100);
                }
                catch (const IndexOutOfRange& e) {
                    AddResult("Exception test Get", "Correctly caught IndexOutOfRange");
                }
                }
    }
    catch (const IndexOutOfRange& e) {
        AddResult("Error", e.what());
    }
    catch (const std::exception& e) {
        AddResult("Error", e.what());
    }

    delete[] items;
}

void SequenceTesterFrame::AddResult(wxString operation, wxString result) {
    long index = resultList->InsertItem(resultList->GetItemCount(), operation);
    resultList->SetItem(index, 1, result);
}

void SequenceTesterFrame::AddIntResult(wxString operation, int result) {
    AddResult(operation, wxString::Format("%d", result));
}

void SequenceTesterFrame::AddBoolResult(wxString operation, bool result) {
    AddResult(operation, result ? "true" : "false");
}

wxIMPLEMENT_APP(SequenceTesterApp);