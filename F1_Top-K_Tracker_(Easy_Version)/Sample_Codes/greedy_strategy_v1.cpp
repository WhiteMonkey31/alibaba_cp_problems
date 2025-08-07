// #include<iostream>
// #include<vector>
// #include<algorithm>
// int main(){
//     int n{};
//     std::cin>>n;
//     std::vector<std::pair<int,int>>activities(static_cast<std::size_t>(n));
//     for(int i{0};i<n;i++){
//         std::cin>>activities[i].first>>activities[i].second;
//     }
//     std::sort(activities.begin(),activities.end(),[](auto& a,auto& b){
//         return a.second<b.second;
//     });
//     int count{0};
//     int last_end{0};
//     for(auto[start,end]:activities){
//         if(start>=last_end){
//             count++;
//             last_end=end;
//         }
//     }
//     std::cout<<count<<std::endl;
//     return 0;
// }