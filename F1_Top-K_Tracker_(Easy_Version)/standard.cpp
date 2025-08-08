// main optimal solution


#include <iostream>
#include <vector>
#include <string>
#include <sstream>

// Function to send query to judge and receive response
std::vector<int> send_query(int type, const std::vector<int>& indices) {
    int m = indices.size();

    // Send query
    std::cout << type << " " << m;
    for (int i = 0; i < m; ++i)
        std::cout << " " << indices[i];
    std::cout << std::endl;
    std::cout.flush();

    // Read response
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    std::vector<int> response;
    int x;
    while (ss >> x)
        response.push_back(x);

    return response;
}

void submit_final_guess(const std::vector<int>& permutation) {
    std::cout << "! ";
    for (int i = 0; i < permutation.size(); ++i)
        std::cout << permutation[i] << " ";
    std::cout << std::endl;
    std::cout.flush();
}

int main() {


    const int n = 845;
    std::vector<int> all_indices(n);
    std::iota(all_indices.begin(), all_indices.end(), 1);

    // Step 1: Use Type 3 query to get top-7 values
    std::vector<int> top_k = send_query(3, all_indices); // single use only
    std::set<int> top_k_set(top_k.begin(), top_k.end());

    // Step 2: Guess array to build permutation
    std::vector<int> guess(n, -1);

    // Step 3: Break into 60-sized chunks and locate top-k values
    for (int i = 0; i < n; i += 60) {
        std::vector<int> chunk;
        for (int j = i; j < std::min(i + 60, n); ++j)
            chunk.push_back(j + 1);  // 1-based

        std::vector<int> response = send_query(1, chunk); // max 60 indices
        std::set<int> found(response.begin(), response.end());

        for (int pos : chunk) {
            if (guess[pos - 1] != -1) continue;

            std::vector<int> test = {pos};
            std::vector<int> res = send_query(1, test);
            if (top_k_set.count(res[0])) {
                guess[pos - 1] = res[0];
            }
        }
    }

    // Step 4: Deduce remaining values by elimination
    std::set<int> all_vals;
    for (int i = 1; i <= n; ++i) all_vals.insert(i);
    for (int x : guess) if (x != -1) all_vals.erase(x);
    for (int i = 0; i < n; ++i)
        if (guess[i] == -1) {
            guess[i] = *all_vals.begin();
            all_vals.erase(all_vals.begin());
        }

    // Step 5: Submit the guess
    submit_final_guess(guess);
    
    return 0;
}





    // int n = 10;

    // // Step 1: Get the top-k elements using Type 3 query (allowed once)
    // std::vector<int> all_indices(n);
    // for (int i = 0; i < n; ++i)
    //     all_indices[i] = i + 1;

    // std::vector<int> top_k = send_query(3, all_indices);  // Type 3, all positions

    // std::cout << "[Debug] Top K elements: ";
    // for (int x : top_k) std::cout << x << " ";
    // std::cout << "\n";

    // // Step 2: Create a map to place known values in their correct positions
    // std::vector<int> guess(n, -1);   // Final guess array, -1 means unknown
    // std::set<int> top_k_set(top_k.begin(), top_k.end()); // For fast lookup

    // // Step 3: Scan all positions using Type 1 queries in chunks of 5
    // for (int i = 0; i < n; i += 5) {
    //     std::vector<int> chunk;
    //     for (int j = i; j < n && j < i + 5; ++j) {
    //         chunk.push_back(j + 1);  // 1-based index
    //     }
    //     std::vector<int> response = send_query(1, chunk);

    //     // We know the chunk, and we know the response is sorted top-k values
    //     for (int val : response) {
    //         for (int pos : chunk) {
    //             if (guess[pos - 1] == -1) {  // not guessed yet
    //                 std::vector<int> test_query = {pos};
    //                 std::vector<int> val_res = send_query(1, test_query);
    //                 if (val_res[0] == val) {
    //                     guess[pos - 1] = val;
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }

    // // Step 4: Fill missing values by elimination
    // std::set<int> all_vals;
    // for (int i = 1; i <= n; ++i) all_vals.insert(i);
    // for (int x : guess) all_vals.erase(x);

    // for (int i = 0; i < n; ++i) {
    //     if (guess[i] == -1) {
    //         guess[i] = *all_vals.begin();  // just insert remaining
    //         all_vals.erase(all_vals.begin());
    //     }
    // }

    // // Final guess
    // submit_final_guess(guess);





    // int n = 10;  // will be 845 in final version

    // // DEMO: Just query index 1 using type 1
    // std::vector<int> sample_indices = {1};  // position to query
    // std::vector<int> result = send_query(1, sample_indices);

    // std::cout << "[Debug] Response: ";
    // for (int val : result)
    //     std::cout << val << " ";
    // std::cout << std::endl;

    // // (Just to show format) Submit dummy permutation
    // std::vector<int> dummy_guess(n);
    // for (int i = 0; i < n; ++i) dummy_guess[i] = i + 1;
    // submit_final_guess(dummy_guess);

//     return 0;
// }
