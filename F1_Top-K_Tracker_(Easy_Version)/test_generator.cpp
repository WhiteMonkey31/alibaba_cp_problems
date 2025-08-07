// // optional: test case generator (can be .py too)

// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <random>
// #include <chrono>

// using namespace std;

// vector<int> hidden_perm;
// vector<int> pos_of;
// int n;

// // Utility to print top-k largest elements from a vector<int>, in increasing order
// vector<int> get_top_k(const vector<int>& vals, int k) {
//     vector<int> copy = vals;
//     sort(copy.begin(), copy.end());
//     if ((int)copy.size() > k)
//         copy.erase(copy.begin(), copy.end() - k);
//     return copy;
// }

// // Query Type 1: return top k in [pi1, pi2, ..., pim]
// void handle_query_type_1(const vector<int>& indices) {
//     vector<int> vals;
//     for (int idx : indices) vals.push_back(hidden_perm[idx - 1]);
//     vector<int> res = get_top_k(vals, min(60, (int)vals.size()));
//     for (int v : res) cout << v << " ";
//     cout << endl;
//     cout.flush();
// }

// // Query Type 2: return top k in [pos[i1], pos[i2], ..., pos[im]]
// void handle_query_type_2(const vector<int>& values) {
//     vector<int> vals;
//     for (int val : values) vals.push_back(pos_of[val]);
//     vector<int> res = get_top_k(vals, min(60, (int)vals.size()));
//     for (int v : res) cout << v << " ";
//     cout << endl;
//     cout.flush();
// }

// int main() {
//     int t;
//     cin >> t;
//     cout << t << endl; // echo test case count
//     for (int test = 0; test < t; ++test) {
//         cin >> n;
//         cout << n << endl;

//         // Generate hidden permutation with fixed seed
//         hidden_perm.resize(n);
//         for (int i = 0; i < n; ++i) hidden_perm[i] = i + 1;
//         unsigned seed = 1337 + test; // consistent for each test
//         shuffle(hidden_perm.begin(), hidden_perm.end(), default_random_engine(seed));

//         pos_of.assign(n + 1, 0);
//         for (int i = 0; i < n; ++i) pos_of[hidden_perm[i]] = i + 1;

//         string cmd;
//         while (cin >> cmd) {
//             if (cmd == "!") {
//                 vector<int> guess(n);
//                 for (int i = 0; i < n; ++i) cin >> guess[i];

//                 // Check answer
//                 bool correct = guess == hidden_perm;
//                 cout << (correct ? "Correct permutation!\n" : "Wrong permutation!\n");
//                 break;
//             } else {
//                 int m;
//                 cin >> m;
//                 vector<int> indices(m);
//                 for (int i = 0; i < m; ++i) cin >> indices[i];

//                 if (cmd == "1") handle_query_type_1(indices);
//                 else if (cmd == "2") handle_query_type_2(indices);
//                 else {
//                     cout << "Query type not implemented in judge.\n";
//                     cout.flush();
//                 }
//             }
//         }
//     }
//     return 0;
// }



#include <iostream>
#include <vector>
#include <algorithm>
#include <random>


const int MAX_K_TYPE_1_2 = 5;  // query indices limit for query type 1 & 2 // should be 60 ideally
const int MAX_K_TYPE_3_4 = 7;  // query indices limit for query type 1 & 2 // should be 300 ideally
bool used_type_3_or_4 = false;  // enforce one-time rule of query 3 and 4


            // function prototypes for handling different query types
void handle_query_by_value(const std::vector<int>& p, int m, const std::vector<int>& indices, int max_k);   // for query type 1 and 3
void handle_query_by_position(const std::vector<int>& pos, int m, const std::vector<int>& indices, int max_k);   // for query type 2 and 4



int main() {
    const int n = 10; // fixed size for prototype permutation // should be ideally 845
    std::vector<int> p(n); // the hidden permutation

    // Fill p with 1 to n 
    for (int i = 0; i < n; ++i) {
        p[i] = i + 1;
    }

    // Shuffle with fixed seed for deterministic behavior
    std::mt19937 rng(42); // any fixed number for reproducibility
    std::shuffle(p.begin(), p.end(), rng); // shuffled th permutation p randomly.   

    // Create position array: pos[i] = position of value i (0-based)
    std::vector<int> pos(n + 1); // 1-based index for values 1..n
    for (int i = 0; i < n; ++i) {
        pos[p[i]] = i + 1; // storing position as 1-based for easier mapping later
    }

    // Debug output (remove later in final judge)
    std::cout << "Generated Hidden Permutation (p): ";
    for (int i = 0; i < n; ++i) std::cout << p[i] << " ";  // prints the hidden permutation p on the screen
    std::cout << "\n";

    std::cout << "Position Array (pos): "; 
    for (int i = 1; i <= n; ++i) std::cout << pos[i] << " ";   // generate the position of the permutation elements inside the array or like indexing the array of permuntation
    std::cout << "\n";



    int query_type{};  // for checking the query type of the query
    int m{};        // for inputting the number of indices a user wants to enter
    std::cout<<"Enter Query Type (1-2): ";
    std::cin >> query_type;
    std::cout<<"Enter Number of Indices (m):";
    std::cin>>m;
    std::vector<int> indices(m);
    for (int i = 0; i < m; ++i) {   // for inputting the number of indices entered.
        std::cin >> indices[i];
    }


            // checking query type based on input data
    if (query_type == 1) {
        handle_query_by_value(p, m, indices,MAX_K_TYPE_1_2); // p:permutation, m:number of indices, indices:actual indies, MAX_K_TYPE_1_2: 60 indicdies that are allowed
    }else if (query_type == 2){
        handle_query_by_position(pos, m, indices,MAX_K_TYPE_1_2);
    } else if (query_type == 3) {
        if (used_type_3_or_4) { 
            std::cerr << "Type 3 or 4 already used!\n"; exit(1); 
        }
        used_type_3_or_4 = true;   // will turn to true after one time use and prevent 2nd time use
        handle_query_by_value(p, m, indices, MAX_K_TYPE_3_4);
    } else if (query_type == 4) {
        if (used_type_3_or_4) {
            std::cerr << "Type 3 or 4 already used!\n"; exit(1);   // exits if the query 3 or 4 is used 2nd time.
        }
        used_type_3_or_4 = true;
        handle_query_by_position(pos, m, indices, MAX_K_TYPE_3_4);
    }




    return 0;
}


    // function for handling query type 1 & 3
void handle_query_by_value(const std::vector<int>& p, int m, const std::vector<int>& indices, int max_k) {

                //p is the hidden permutation,
                // m is the number of indices,
                // indices are those individual indices that are sent to the judge/program for checking.
                // max_k is the max number of queries allowed by query type like 60 or 300

    std::vector<int> values;
    for (int idx : indices) {
        values.push_back(p[idx - 1]);  // convert to 0-based index
    }

    std::sort(values.begin(), values.end(), std::greater<int>()); // sort in descending order 
                                                                                    // it is done in desending order so that the extra number of indices don't exceed the allowed limit and can be removed.
    int k = std::min(max_k, m);                                 // MAX_K_TYPE_1_2 is the max number of indices allowed for type 1 and 2 query
    std::vector<int> top_k(values.begin(), values.begin() + k);         // top_k are the values that will be send back to the user/screen in reply of query
    std::sort(top_k.begin(), top_k.end()); // output must be in increasing order
    for (int val : top_k) {
        std::cout << val << " ";  // prints the top_k values to the screen.
    }
    std::cout << std::endl;
}


        // function for handling query type 2 and 4
void handle_query_by_position(const std::vector<int>& pos, int m, const std::vector<int>& indices, int max_k) {

                                    // pos is the index of hidden permutation p.
                                    // m is the number of indices that are given or entered.
                                    // indices are those individual values/input that are given to the system as a query, that comes after m in query
                                    // max_k is the max number of queries allowed by query type like 60 or 300
    std::vector<int> positions;
    for (int val : indices) {
        positions.push_back(pos[val]);  // pos is already 1-based
    }

    std::sort(positions.begin(), positions.end(), std::greater<int>());  // sorts in desending order
    int k = std::min(max_k, m);                                       // checking against the total number of indices allowed by query type 1 and 2
    std::vector<int> top_k(positions.begin(), positions.begin() + k);      // top_k contains the top-k elements selected from hidden permutation p
    std::sort(top_k.begin(), top_k.end()); // output must be in increasing order
    for (int val : top_k) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}




