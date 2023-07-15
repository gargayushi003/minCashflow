#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> graph;
vector<string>pname;
void get_name();

void outputer(){
    string asciiArt = R"(

                                                                             /$$                       /$$       /$$
                                                                            | $$                      | $$      | $$
  /$$$$$$  /$$   /$$  /$$$$$$   /$$$$$$  /$$$$$$$   /$$$$$$$  /$$$$$$       | $$$$$$$  /$$   /$$  /$$$$$$$  /$$$$$$$ /$$   /$$
 /$$__  $$|  $$ /$$/ /$$__  $$ /$$__  $$| $$__  $$ /$$_____/ /$$__  $$      | $$__  $$| $$  | $$ /$$__  $$ /$$__  $$| $$  | $$
| $$$$$$$$ \  $$$$/ | $$  \ $$| $$$$$$$$| $$  \ $$|  $$$$$$ | $$$$$$$$      | $$  \ $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$
| $$_____/  >$$  $$ | $$  | $$| $$_____/| $$  | $$ \____  $$| $$_____/      | $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$
|  $$$$$$$ /$$/\  $$| $$$$$$$/|  $$$$$$$| $$  | $$ /$$$$$$$/|  $$$$$$$      | $$$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$|  $$$$$$$
 \_______/|__/  \__/| $$____/  \_______/|__/  |__/|_______/  \_______/      |_______/  \______/  \_______/ \_______/ \____  $$
                    | $$                                                                                             /$$  | $$
                    | $$                                                                                            |  $$$$$$/
                    |__/                                                                                             \______/

)";
    cout << asciiArt;
}

class people{
    public:
    string name;
    int netAmount;
    set<string> types;
};

int getMinIndex(people listOfNetAmounts[],int numPeople){
    int min=INT_MAX, minIndex=-1;
    for(int i=0;i<numPeople;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount < min){
            minIndex = i;
            min = listOfNetAmounts[i].netAmount;
        }
    }
    return minIndex;
}

int getSimpleMaxIndex(people listOfNetAmounts[],int numPeople){
    int max=INT_MIN, maxIndex=-1;
    for(int i=0;i<numPeople;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount > max){
            maxIndex = i;
            max = listOfNetAmounts[i].netAmount;
        }
    }
    return maxIndex;
}

pair<int,string> getMaxIndex(people listOfNetAmounts[],int numPeople,int minIndex,people input[],int maxNumTypes){
    int max=INT_MIN;
    int maxIndex=-1;
    string matchingType;

    for(int i=0;i<numPeople;i++){
        if(listOfNetAmounts[i].netAmount == 0) continue;

        if(listOfNetAmounts[i].netAmount < 0) continue;


        vector<string> v(maxNumTypes);
        vector<string>::iterator ls=set_intersection(listOfNetAmounts[minIndex].types.begin(),listOfNetAmounts[minIndex].types.end(), listOfNetAmounts[i].types.begin(),listOfNetAmounts[i].types.end(), v.begin());

        if((ls-v.begin())!=0 && max<listOfNetAmounts[i].netAmount ){
            max=listOfNetAmounts[i].netAmount;
            maxIndex=i;
            matchingType = *(v.begin());
        }
    }

    return make_pair(maxIndex,matchingType);
}

void printAns(vector<vector<pair<int,string>>> ansGraph, int numPeople,people input[]){

    cout<<"\nThe transactions for minimum cash flow are as follows : \n\n";
    for(int i=0;i<numPeople;i++){
        for(int j=0;j<numPeople;j++){

            if(i==j) continue;

            if(ansGraph[i][j].first != 0 && ansGraph[j][i].first != 0){

                if(ansGraph[i][j].first == ansGraph[j][i].first){
                    ansGraph[i][j].first=0;
                    ansGraph[j][i].first=0;
                }
                else if(ansGraph[i][j].first > ansGraph[j][i].first){
                    ansGraph[i][j].first -= ansGraph[j][i].first;
                    ansGraph[j][i].first =0;

                    cout<<input[i].name<<" pays " << ansGraph[i][j].first<< "to "<<input[j].name<<" using "<<ansGraph[i][j].second<<endl;
                }
                else{
                    ansGraph[j][i].first -= ansGraph[i][j].first;
                    ansGraph[i][j].first = 0;

                    cout<<input[j].name<<" pays "<< ansGraph[j][i].first<<" to "<<input[i].name<<" using "<<ansGraph[j][i].second<<endl;

                }
            }
            else if(ansGraph[i][j].first != 0){
                cout<<input[i].name<<" pays "<<ansGraph[i][j].first<<" to "<<input[j].name<<" using "<<ansGraph[i][j].second<<endl;

            }
            else if(ansGraph[j][i].first != 0){
                cout<<input[j].name<<" pays "<<ansGraph[j][i].first<<" to "<<input[i].name<<" using "<<ansGraph[j][i].second<<endl;

            }

            ansGraph[i][j].first = 0;
            ansGraph[j][i].first = 0;
        }
    }
    cout<<"\n";
}

void minimizeCashFlow(int numPeople,people input[],unordered_map<string,int>& indexOf,int numTransactions,vector<vector<int>>& graph,int maxNumTypes){

    //Find net amount of each person has
    people listOfNetAmounts[numPeople];

    for(int b=0;b<numPeople;b++){
        listOfNetAmounts[b].name = input[b].name;
        listOfNetAmounts[b].types = input[b].types;

        int amount = 0;
        //incoming edges
        //column traverse
        for(int i=0;i<numPeople;i++){
            amount += (graph[i][b]);
        }

        //outgoing edges
        //row traverse
        for(int j=0;j<numPeople;j++){
            amount += ((-1) * graph[b][j]);
        }

        listOfNetAmounts[b].netAmount = amount;
    }

    vector<vector<pair<int,string>>> ansGraph(numPeople,vector<pair<int,string>>(numPeople,{0,""}));//adjacency matrix


    //find min and max net amount
    int numZeroNetAmounts=0;

    for(int i=0;i<numPeople;i++){
        if(listOfNetAmounts[i].netAmount == 0) numZeroNetAmounts++;
    }
    while(numZeroNetAmounts!=numPeople){

        int minIndex=getMinIndex(listOfNetAmounts, numPeople);
        pair<int,string> maxAns = getMaxIndex(listOfNetAmounts, numPeople, minIndex,input,maxNumTypes);

        int maxIndex = maxAns.first;

        if(maxIndex == -1){

            (ansGraph[minIndex][0].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[minIndex][0].second) = *(input[minIndex].types.begin());

            int simpleMaxIndex = getSimpleMaxIndex(listOfNetAmounts, numPeople);
            (ansGraph[0][simpleMaxIndex].first) += abs(listOfNetAmounts[minIndex].netAmount);
            (ansGraph[0][simpleMaxIndex].second) = *(input[simpleMaxIndex].types.begin());

            listOfNetAmounts[simpleMaxIndex].netAmount += listOfNetAmounts[minIndex].netAmount;
            listOfNetAmounts[minIndex].netAmount = 0;

            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;

            if(listOfNetAmounts[simpleMaxIndex].netAmount == 0) numZeroNetAmounts++;

        }
        else{
            int transactionAmount = min(abs(listOfNetAmounts[minIndex].netAmount), listOfNetAmounts[maxIndex].netAmount);

            (ansGraph[minIndex][maxIndex].first) += (transactionAmount);
            (ansGraph[minIndex][maxIndex].second) = maxAns.second;

            listOfNetAmounts[minIndex].netAmount += transactionAmount;
            listOfNetAmounts[maxIndex].netAmount -= transactionAmount;

            if(listOfNetAmounts[minIndex].netAmount == 0) numZeroNetAmounts++;

            if(listOfNetAmounts[maxIndex].netAmount == 0) numZeroNetAmounts++;
        }

    }

    printAns(ansGraph,numPeople,input);
}

struct cashf
{
    int first,second,amt;
};

class personal{
    public:
        vector<pair<string,int>> debts;
        map<string,int> flow;
        int balance;

        void insert(int n){
            get_name();
            int flag=0,count=0;
            for(int i=0;i<n;i++){
                flag=0;
                cout<<"\t\t\t\t"<<pname[i]<<"'s Account"<<endl<<endl;
                for(int j=0;j<n;j++){
                    if(graph[i][j]){
                        flag=1;
                        flow.insert(make_pair(pname[j],graph[i][j]));
                        cout<<"\t\t\t\t   "<<pname[j]<<": "<<graph[i][j]<<endl;
                        count++;
                    }
                }
                if(flag==0){
                    cout<<"No debts present"<<endl;
                    string p;
                    cout<<endl<<"Enter any key to continue"<<endl;
                    cin>>p;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    system("cls");
                    continue;
                }
                cout<<endl;
                priority(count);
                cout<<"Enter the balance you have left"<<endl;
                cin>>balance;
                disp();
                cout<<endl;
                flow.clear();
                debts.clear();
            }
        }

        void priority(int count){
            int i=0;
            char pr;
            cout<<"Do you want to set priority?"<<endl;
            cin>>pr;
            if(pr=='y'){
                int n=0;
                string name;
                while(true){
                    cout<<"For how many people?"<<endl;
                    cin>>n;
                    if(cin.fail() || n>count){
                        cout<<"Not a valid number"<<endl<<endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        continue;
                    }
                    break;
                }
                cout<<"Enter priority: "<<endl;
                for(i=0;i<n;i++){
                    while(true){
                        cin>>name;
                        if(flow.find(name)==flow.end()){
                            cout<<"No such person exists in the group. Enter again"<<endl;
                            continue;
                        }
                        break;
                    }
                    debts.push_back(make_pair(name,flow[name]));
                    flow.erase(name);
                }
            }
            for(auto i:flow){
                debts.push_back(i);
            }

            quick_sort(debts,i,debts.size()-1);
        }

        void quick_sort(vector<pair<string,int>> &debts,int start,int end){
            if(start>=end) return;
            int pi=start+(rand()%(end-start+1));
            auto pivot=debts[pi];

            debts[pi]=debts[end];
            debts[end]=pivot;

            pi=start;
            for(int i=start;i<end;i++){
                if(pivot.second>debts[i].second){
                    auto temp=debts[pi];
                    debts[pi]=debts[i];
                    debts[i]=temp;
                    pi++;
                }
            }

            debts[end]=debts[pi];
            debts[pi]=pivot;
            quick_sort(debts,start,pi-1);
            quick_sort(debts,pi+1,end);
        }

        void disp(){
            cout<<endl;
            if(balance < debts[0].second){
                cout<<"No debt can be cleared"<<endl;
            }
            else{
                int i=0,flag=0;
                cout<<"To clear the maximum debt: "<<endl;
                while(balance>=0 && i<debts.size()){
                    flag= 0;
                    if(debts[i].second<=balance){
                        flag=1;
                        balance=balance-debts[i].second;
                        cout<<" "<<debts[i].first<<": "<<debts[i].second<<endl;
                        i++;
                    }
                    if(flag==0){
                        int p=0;
                        cout<<endl<<"Enter any key to continue"<<endl;
                        cin>>p;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        system("cls");
                        return;
                    }
                }
            }

            int p=0;
            cout<<endl<<"Enter any key to continue"<<endl;
            cin>>p;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            system("cls");
        }
};
personal x;

void get_name(){
    ifstream file;
    file.open("people.txt");
    string str;
    while(!file.eof()){
        file>>str;
        pname.push_back(str);
        getline(file,str);
    }
    file.close();
}

void update(){
    fstream file;
    file.open("people.txt",ios::in);
    string str,detail="";
    while(!file.eof()){
        getline(file,str);
        detail+=str + "0 \n";
    }
    file.close();

    file.open("people.txt",ios::out);
    file<<detail;
    file.close();
}

void disp_file(){
    ifstream file("people.txt");
    string str;
    cout<<"Listing"<<endl;
    while(!file.eof()){
        getline(file,str);
        cout<<str<<endl;

    }
}

void add_person(){
    ofstream file;
    file.open("people.txt",ios::app);
    if(file.is_open()){
        string name;
        cout<<"Enter the name of the person: ";
        getline(cin,name);
        file<<name<<" ";
        int num;
        fstream numfile("num.txt",ios::in);
        numfile>>num;
        numfile.close();
        for(int i=0;i<num;i++){
            file<<"0 ";
        }
        file.close();
        update();
        num++;
        numfile.open("num.txt",ios::out);
        numfile<<num;
        numfile.close();
    }

}

int* get_index(string debtor,string creditor){
    int *index=new int[2],i=1,flag=0;
    index[0]=index[1]=0;
    ifstream file;
    file.open("people.txt");
    string str;
    while(!file.eof()){
        file>>str;
        if(str==debtor){
            flag++;
            index[0]=i;
        }
        else if(str==creditor){
            flag++;
            index[1]=i;
        }
        if(flag==2){
            break;
        }
        getline(file,str);
        i++;
    }
    if(flag!=2){
        cout<<"Enter valid names"<<endl;
    }
    file.close();
    return index;
}

void add_amount(){
    string debtor,creditor;
    int amount;
    int* index;
    while(true){
        cout<<"Enter debtor(giver): ";
        getline(cin,debtor);
        cout<<"Enter creditor(receiver): ";
        getline(cin,creditor);
        if(debtor==creditor){
            cout<<"Both can't be the same"<<endl;
            continue;
        }
        index = get_index(debtor,creditor);
        if(index[0]==0 || index[1]==0){
            continue;
        }
        break;
    }

    while(true){
        cout<<"Enter the amount: ";
        cin>>amount;
        if(cin.fail()){
            cout<<"Not a valid number"<<endl<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        break;
    }

    string str,sp,detail="";
    int i=1;
    fstream file;
    file.open("people.txt",ios::in);
    while(!file.eof()){
        if(i==index[0]){
            for(int j=1;j<=index[1];j++){
                file>>str;
                detail+=str+" ";
            }
            file>>str;
            amount+=stoi(str);
            stringstream ss;
            ss<<amount;
            string amt;
            ss>>amt;
            detail+=amt;
            getline(file,str);
            detail+=str+"\n";
        }
        else{
            getline(file,str);
            if(str.empty()){
                break;
            }
            detail+=str+"\n";
        }
        i++;
    }
    file.close();

    file.open("people.txt",ios::out);
    file<<detail;
    file.close();
}

int get_graph(){
    ifstream file("people.txt");
    string str;
    vector<int> val;
    int value;
    while(!file.eof()){
        file>>str;
        if(str.empty()) continue;
        getline(file,str);
        stringstream ss(str);
        while(ss>>value){
            val.push_back(value);
        }
        graph.push_back(val);
        val.clear();
        str.clear();

    }
    file.close();
}

int get_min(int arr[],int NUM_PERSONS)
{
    int min_index = 0;
    for (int i=1; i<NUM_PERSONS; i++)
        if (arr[i] < arr[min_index])
            min_index = i;
    return min_index;
}

int get_max(int arr[], int NUM_PERSONS)
{
    int max_index = 0;
    for (int i=1; i<NUM_PERSONS; i++)
        if (arr[i] > arr[max_index])
            max_index = i;
    return max_index;
}

int get_min_of_two(int x, int y)
{
    return (x < y) ? x : y;
}

void calculate_min_cash_flow(int amount[],int NUM_PERSONS,auto &min_index)
{
    int max_credit_index = get_max(amount,NUM_PERSONS), max_debit_index = get_min(amount,NUM_PERSONS);

    if (amount[max_credit_index] == 0 && amount[max_debit_index] == 0)
        return;

    int min = get_min_of_two(-amount[max_debit_index], amount[max_credit_index]);
    amount[max_credit_index] -= min;
    amount[max_debit_index] += min;

    get_name();
    cout << pname[max_debit_index] << " pays " << min
         << " to " << pname[max_credit_index] << endl;

    min_index.push_back({max_debit_index,max_credit_index,min});
    calculate_min_cash_flow(amount,NUM_PERSONS,min_index);
}

void find_min_cash_flow(auto &min_index)
{
    int NUM_PERSONS = graph.size();
    int amount[NUM_PERSONS] = {0};

    for (int p=0; p<NUM_PERSONS; p++)
        for (int i=0; i<NUM_PERSONS; i++)
            amount[p] += (graph[i][p] - graph[p][i]);

    calculate_min_cash_flow(amount,NUM_PERSONS,min_index);
}

int comp(cashf a,cashf b){
    if(a.first==b.first) return (a.second)<(b.second);
    return (a.first)<(b.first);
}

void update_graph(vector<cashf>min_index){
    sort(min_index.begin(),min_index.end(),comp);

    int k=0;
    for(int i=0;i<graph.size();i++){
        for(int j=0;j<graph[i].size();j++){
            if(i==min_index[k].first && j==min_index[k].second){
                graph[i][j]=min_index[k].amt;
                k++;
            }
            else{
                graph[i][j]=0;
            }
        }
    }
}

void update_file(){
    get_name();
    fstream file;
    file.open("people.txt",ios::in);
    string info="",str;
    stringstream ss;
    for(int i=0;i<graph.size();i++){
        info+=pname[i]+" ";
        for(int j=0;j<graph[i].size();j++){
            ss<<graph[i][j]<<" ";
            ss>>str;
            info+=str+" ";
        }
        info+="\n";
        ss.clear();
    }
    file.close();
    file.open("people.txt",ios::out);
    file<<info;
    file.close();
}

void util_cashflow(){
    get_graph();
    vector<cashf> min_index;
    find_min_cash_flow(min_index);

    update_graph(min_index);
    update_file();
}

void payment()
{
    int numPeople;
    cout<<"Enter number of people in group: ";
    cin>>numPeople;
    people input[numPeople];
    unordered_map<string,int> indexOf;//stores index of a person

    cout<<endl;
    /*cout<<"Enter Person name ,number of payment modes it has and the payment modes.\n";
    cout<<"Person name and payment modes should not contain spaces\n";*/
    cout<<endl;
    int maxNumTypes;
    for(int i=0; i<numPeople;i++){
        if(i==0){
            cout<<"Person with all payment modes : ";
        }
        else{
            cout<<"Enter name of person"<<" : ";
        }
        cin>>input[i].name;
        indexOf[input[i].name] = i;
        int numTypes;
        cout<<"Enter number of payment methods available: ";
        cin>>numTypes;

        if(i==0) maxNumTypes = numTypes;

        string type;
        cout<<"Enter name of all methods available: ";
        while(numTypes--){
            cin>>type;

            input[i].types.insert(type);
        }
        cout<<endl;

    }

    cout<<"Enter number of transactions.\n";
    int numTransactions;
    cin>>numTransactions;

    vector<vector<int>> graph(numPeople,vector<int>(numPeople,0));//adjacency matrix


    cout<<endl;
    cout<<endl;
    for(int i=0;i<numTransactions;i++){
        cout<<"Transaction "<<(i+1);
        cout<<endl;
        string s1,s2;
        int amount;
        cout<<"Enter Debtor: ";
        cin>>s1;
        cout<<"Enter Creditor: ";
        cin>>s2;
        cout<<"Enter amount borrowed: ";
        cin>>amount;
        //cin >> s1>>s2>>amount;

        graph[indexOf[s1]][indexOf[s2]] = amount;
        cout<<endl;
    }

    //settle
    minimizeCashFlow(numPeople,input,indexOf,numTransactions,graph,maxNumTypes);
}
void findMin(vector<pair<int,int>> &d,int n,int v)
{

    //sort(d,d+n);
    sort(d.rbegin(),d.rend()); //decreasing
    vector<int> ans;
    for(int i=0;i<n;i++){
        while(v>=d[i].first && d[i].second!=0){
            v  = v - d[i].first;
            d[i].second--;
            ans.push_back(d[i].first);
        }
    }
    //print result
    for(int i=0;i<ans.size();i++){
        cout<<ans[i]<<" ";
    }
}


void input_denominations()
{
    int n;

    cout<<"Enter the no. of types of notes you have: ";
    cin>>n;
    cout<<"Enter the denominations you have and their count--- "<<endl;
    //int* denomination=new int[n];
    int d,counter;
    vector<pair<int,int>> denomination(n);
    for(int i=0;i<n;i++){
        cout<<"Entry "<<i+1<<": ";
        cin>>d>>counter;
        pair<int,int> p = make_pair(d,counter);
        denomination.push_back(p);
    }
    int amount;
    cout<<"Enter the amount: ";
    cin>>amount;
    cout<<"Minimal no. of change for "<<amount<<" is: ";
    findMin(denomination,n,amount);
}

void menu(){
    int choice1;
    outputer();
    cout << "\n\n\t\t\tEXPENSE BUDDY" << endl;
    cout << "\t\t\t   1. Limited number of payment methods? " << endl;
    cout << "\t\t\t   2. Splitwise" << endl;
    cout<<"Enter your choice: ";
    cin>>choice1;
    system("cls");
    switch(choice1)
    {
    case 1:
        payment();
        break;
    case 2:
    while (true) {
        outputer();
        cout << "\n\n\t\t\tGroup Formation" << endl;
        cout << "\t\t\t   1. Add a person" << endl;
        cout << "\t\t\t   2. Update existing balance" <<endl;
        cout << "\t\t\t   3. List all people" << endl<<endl<<endl;

        cout<<"\t\t\tFEATURES"<<endl;
        cout<<"\t\t\t   4. Minimise Cashflow"<<endl;
        cout<<"\t\t\t   5. Personal account"<<endl;
        cout<<"\t\t\t   6. Coinage"<<endl<<endl;
        cout<<"\t\t\t   7. Exit" << endl<<endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        system("cls");

        switch (choice) {
            case 1:
                add_person();
                break;
            case 2:
                add_amount();
                break;
            case 3:
                disp_file();
                break;

            case 4:
                util_cashflow();
                break;

            case 5:
                if(graph.size()==0) get_graph();
                x.insert(graph.size());
                break;

            case 6:
                input_denominations();
                break;

            case 7:
                return;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        int n=0;
        cout<<endl<<"Enter a key to go back to the main menu"<<endl;
        cin>>n;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        system("cls");
    }
    break;
    }
}

int main(){
    ifstream numfile;
    numfile.open("num.txt");
    if(!numfile){
        numfile.close();
        int n=0;
        ofstream numfile("num.txt");
        numfile<<n;
    }
    numfile.close();

    menu();
    return 0;
}
