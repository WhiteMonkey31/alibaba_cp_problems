
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <sstream>
#include <set>

const int MAX_QUERIES {30};   // maximum number of queries allowed for each permutation
const int MAX_K_TYPE_1_2 {5};  // query indices limit for query type 1 & 2 // should be 60 ideally
const int MAX_K_TYPE_3_4 {7};  // query indices limit for query type 1 & 2 // should be 300 ideally



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


    int total_queries{0};   // to calculate the number of queries entered
    bool used_type_3_or_4 {false};  // enforce one-time rule of query 3 and 4

    bool invalid_query = false;  // for checking invalid query and skipping it
    std::string line;
    std::cout<<"Enter Query: ";
    std::cout.flush();
    while (std::getline(std::cin, line)) {  // keep taking query in loop until 30 queries
        std::stringstream ss(line);  // ss is made for validating query if valid query has been passsed or not?
        std::string cmd;
        ss >> cmd;   // forwarding first character to cmd for validation

        if (cmd == "!") {  // if query has first character (!) instead of 1-4, then it means the user is trying to guess the number instead of forwarding query
            std::vector<int> guess(n);
            for (int i = 0; i < n; ++i)
                ss >> guess[i];
            if (guess == p) {   // if guess is correct
                std::cout << "Correct!" << std::endl;
                break;
            } else {
                std::cout << "Wrong!" << std::endl;
                std::cout.flush();
                invalid_query=true;
                // break;
            }
        }

        int type = std::stoi(cmd);  // for validating the query type by first charachter (1-4)
        if (type < 1 || type > 4) {
            std::cout << "Invalid query type.\n";
            std::cout.flush();
            continue;
            // invalid_query=true;
        }

        int m;
        ss >> m;  // for validating the m that should be greater than 0 and less than number of permutation (n)
        if (m <= 0 || m > n) {
            std::cout << "Invalid m.\n";
            std::cout.flush();
            continue;
            // invalid_query=true;
        }

        std::vector<int> indices(m);  // the indices sent in query will be stored here
        std::set<int> unique_check;   // for checking the uniqueness of the indices sent in the query
        for (int i = 0; i < m; ++i) {
            ss >> indices[i];
            if (indices[i] < 1 || indices[i] > n) {  // checking range of indices
                std::cout << "Index out of range.\n";
                std::cout.flush();
                invalid_query = true;
            }
            if (!unique_check.insert(indices[i]).second) {   // checking uniqueness of indices
                std::cout << "Duplicate index detected: " << indices[i] << "\n";
                std::cout.flush();
                invalid_query = true;
            }   
        }

        // Enforce query limits
        total_queries++;
        if (total_queries > MAX_QUERIES) {
            std::cout << "Query limit exceeded.\n";

            break;
        }
        if ((type == 3 || type == 4) && used_type_3_or_4) {
            std::cout << "Type 3 or 4 already used.\n";
            std::cout.flush();
            invalid_query=true;
            // break;
            
        }
        if (type == 3 || type == 4) {
            used_type_3_or_4 = true;
        }
          // allowing indices limit based on query type
        int k = (type == 3 || type == 4) ? std::min(MAX_K_TYPE_3_4, m) : std::min(MAX_K_TYPE_1_2, m);
        std::vector<int> values; // for temperory storing the values of th etop-k in unsorted form

        if (type == 1 || type == 3) {
            for (int i : indices) {
                values.push_back(p[i - 1]); // put th emaching indices top-k elements in values
            }
        } else {
            for (int i : indices) {
                values.push_back(pos[i]);
            }
        }

        // sorting the top_k values to give output to user
        std::sort(values.begin(), values.end(), std::greater<int>());
        std::vector<int> top_k(values.begin(), values.begin() + k);
        std::sort(top_k.begin(), top_k.end()); // Output must be increasing

            // outputting the query top-k 
        for (int x : top_k){
            std::cout << x << " ";
            std::cout.flush();
        }
        std::cout << std::endl;
        std::cout.flush();

        if(total_queries<30){
            std::cout<<"Enter Query: ";
            std::cout.flush();
        }

        if(invalid_query){
            continue;
        }

            // incase ss silently fails due to invalid input
        if (ss.fail()) {
            std::cout << "Invalid input format.\n";
            std::cout.flush();
            continue;
        }
    
        continue;
    }

}