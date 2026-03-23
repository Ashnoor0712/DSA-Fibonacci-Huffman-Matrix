// ashnoor
// csci 115
//lab 10

// huffman coding

#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <queue>
#include <map>

using namespace std;
using namespace std::chrono;


// part 1 a
// recursive function of fib
int fib_recursive(int n) {
    return (n <= 1) ? n : fib_recursive(n - 1) + fib_recursive(n - 2);
}

//Memoization fibonacci
unordered_map<int, int> fib_hash;
int fib_memoization(int n) {
    if (n <= 1) return n;
    if (fib_hash.count(n)) return fib_hash[n];
    fib_hash[n] = fib_memoization(n - 1) + fib_memoization(n - 2);
    return fib_hash[n];
}


// compares the two fib functions
void compare_fibs(int n) {
    cout << "The number: " << n << endl;

    auto start1 = high_resolution_clock::now();
    int result1 = fib_recursive(n);
    auto stop1 = high_resolution_clock::now();
    auto duration1 = duration_cast<milliseconds>(stop1 - start1);
    cout << "Recursive result: " << result1 << " in " << duration1.count() << " ms\n";

    auto start2 = high_resolution_clock::now();
    int result2 = fib_memoization(n);
    auto stop2 = high_resolution_clock::now();
    auto duration2 = duration_cast<milliseconds>(stop2 - start2);
    cout << "Memoization result: " << result2 << " in " << duration2.count() << " ms\n\n";
}

// part b

// matrix multiplication

class Matrix {
public:
    vector<vector<int>> mat_rix;
    int rows, columns;
    

    // constructor to
    Matrix(int r, int c) : rows(r), columns(c) {
        mat_rix.assign(r, vector<int>(c, 0));
    }

    void input() {
        cout << "Enter elements of " << rows << "x" << columns << " matrix:\n";
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                cin >> mat_rix[i][j];
    }

    Matrix multiply(Matrix &mat2) {
        if (columns != mat2.rows) {
            cout << "Matrix multiplication not possible!\n";
            return Matrix(0, 0);
        }
        
        // getting result by multiplication
        Matrix result_mul(rows, mat2.columns);
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < mat2.columns; ++j)
                for (int k = 0; k < columns; ++k)
                    result_mul.mat_rix[i][j] += mat_rix[i][k] * mat2.mat_rix[k][j];

        return result_mul;
    }

    void print() {
        for (auto &row : mat_rix) {
            for (auto &val : row)
                cout << val << " ";
            cout << endl;
        }
    }
};


// part 2

class HuffmanNode {
public:
    char ch;
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// campare function
struct Compare {
    bool operator()(HuffmanNode *a, HuffmanNode *b) {
        return a->freq > b->freq;// returns true if a has higher frequency than b-- thus min heap behavior
    }
};

void assignbinary(HuffmanNode *root, string str, map<char, string> &codes) {
    if (!root) return;
    if (!root->left && !root->right)
        codes[root->ch] = str;

    assignbinary(root->left, str + "0", codes);
    assignbinary(root->right, str + "1", codes);
}

void freeTree(HuffmanNode *root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

string encode(const string &text, map<char, string> &codes) {
    string encoded = "";
    for (char ch : text)
        encoded += codes[ch];
    return encoded;
}

string decode(HuffmanNode *root, const string &encoded) {
    string decoded = "";
    HuffmanNode *curr = root;
    for (char bit : encoded) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            decoded += curr->ch;
            curr = root;
        }
    }
    return decoded;
}

void huffman_demo(string text) {
    map<char, int> freq;
    for (char ch : text) freq[ch]++;

    // Loop through each character-frequency pair in the frequency map
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto &entry : freq)
        pq.push(new HuffmanNode(entry.first, entry.second));

    while (pq.size() > 1) {
        HuffmanNode *a = pq.top(); pq.pop();
        HuffmanNode *b = pq.top(); pq.pop();
        HuffmanNode *merged = new HuffmanNode('\0', a->freq + b->freq);
        merged->left = a;
        merged->right = b;
        pq.push(merged);
    }

    HuffmanNode *root = pq.top();
    map<char, string> codes;
    assignbinary(root, "", codes);

    string encoded = encode(text, codes);
    string decoded = decode(root, encoded);
    
    cout<<"HUFFMAN CODING EXAMPLE: "<< endl;

    cout << "Original text: \n" << text << endl;
    cout << "\nEncoded text:\n" << encoded << endl;
    cout << "\nDecoded text:\n" << decoded << endl;

    freeTree(root);
}

int main() {
    // Fibonacci Comparison
    compare_fibs(17);

    // Matrix Multiplication 2*2
    Matrix A(2, 2), B(2, 2);
    A.input();
    B.input();
    Matrix C = A.multiply(B);
    cout << "Result of Matrix Multiplication:\n";
    C.print();

    // Huffman Coding Demo with a paragraph
    string text = R"(Over hill, over dale,
Thorough bush, thorough brier,
Over park, over pale,
Thorough flood, thorough fire!
I do wander everywhere,
Swifter than the moon's sphere;
And I serve the Fairy Queen,
To dew her orbs upon the green;
The cowslips tall her pensioners be;
In their gold coats spots you see; Those be rubies, fairy favours;
In those freckles live their savours;
I must go seek some dewdrops here, And hang a pearl in every cowslip's ear.)";

    huffman_demo(text);

    return 0;
}

