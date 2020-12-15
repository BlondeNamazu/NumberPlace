/*{{{*/
#include<bits/stdc++.h>
#include<unistd.h>

typedef long long ll;

#define REP(i,n) for(ll i=0;i<n;i++)
#define FOR(i,a,b) for(ll i=a;i<b;i++)
#define FORR(i,a,b) for(ll i=b-1;i>=a;i--)

#define vl vector<ll>
#define pll pair<ll,ll>

#define DEBUG(x) cout<<#x<<": "<<x<<endl

using namespace std;
/*}}}*/

vector<vl> number(9,vl(9,0));
// candMap[i][j][k] = number[i][j] can be k (k is 1-origin)
vector<vector<vector<bool>>> candMap(9,vector<vector<bool>>(9,vector<bool>(10,true)));
queue<vl> que; // {x, y, k}, which can be filled
stack<pair<string,vl>> st; // {encoded_matrix, {cand_x, cand_y, cand_k}}
vector<vl> queLog; // {x, y, k} k is filling number. if popping, k = 0.
stack<vl> actionStack; // {x, y, k} k is filling number. if popping, k = 0.
void print() {
  cout << "-------------" << endl;
  REP(i,9) {
    cout << '|';
    REP(j,9) {
      if(number[i][j]==0) cout << ' ';
      else cout << number[i][j];
      if(j%3==2) cout << '|';
    }
    cout << endl;
    if(i%3==2) cout << "-------------" << endl;
  }
}
void printCandMap() {
  cout << "=====================================" << endl;
  REP(i,9) {
    REP(di,3){
      cout << '|';
      REP(j,9) {
        REP(dj,3){
          ll k = di*3+dj+1;
          if(candMap[i][j][k]) cout << k;
          else cout << ' ';
        }
        if(j%3==2) cout << '|';
        else cout << ':';
      }
      cout << endl;
    }
    if(i%3==2) cout << "=====================================" << endl;
    else cout << "-------------------------------------" << endl;
  }
}
void fillCandMap() {
  REP(i,9) REP(j,9) FOR(k,1,10) candMap[i][j][k] = true;
  REP(ui,9) {
    REP(uj,9){
      // update number of (ui,uj)

      // if already filled, no need to update
      if(number[ui][uj]!=0) {
        candMap[ui][uj] = vector<bool>(10,false);
        candMap[ui][uj][number[ui][uj]] = true;
        //continue;
      }

      // horizontal
      REP(k,9) {
        ll n = number[ui][k];
        candMap[ui][uj][n] = false;
      }
      // vertical
      REP(k,9) {
        ll n = number[k][uj];
        candMap[ui][uj][n] = false;
      }
      // box
      ll bi = ui/3;
      ll bj = uj/3;
      REP(i,3) {
        REP(j,3) {
          ll n = number[bi*3+i][bj*3+j];
          candMap[ui][uj][n] = false;
        }
      }
    }
  }
  //printCandMap();
}
void insertQueue() {
  // to avoid duplicate action in queue
  vector<vector<bool>> alreadyFilled = vector<vector<bool>>(9,vector<bool>(9,false));
  REP(i,9) REP(j,9) alreadyFilled[i][j] = number[i][j] != 0;

  // only 1 candidate in a mesh
  REP(i,9){
    REP(j,9){
      vl cands;
      FOR(k,1,10){
        if(candMap[i][j][k]) cands.push_back(k);
      }
      if(cands.size()==1 && alreadyFilled[i][j] && number[i][j]==0) {
        cout << "Candidate unique in the mesh: (" << i << "," << j << ") = " << cands[0] << endl;
        que.push({i,j,cands[0]});
        alreadyFilled[i][j] = true;
      }
    }
  }
  // only 1 mesh in the box can fill a number
  REP(bi,3){
    REP(bj,3){
      // for box[bi][bj]
      FOR(k,1,10){
        // only one mesh in the box can be k?
        vector<vl> cands;
        REP(ui,3){
          REP(uj,3){
            ll i = bi*3+ui;
            ll j = bj*3+uj;
            if(candMap[i][j][k]) cands.push_back({i,j,k});
          }
        }
        if(cands.size()==1 && !alreadyFilled[cands[0][0]][cands[0][1]]){
          cout << "Candidate unique in the box: (" << cands[0][0] << "," << cands[0][1] << ") = " << k << endl;
          que.push(cands[0]);
          alreadyFilled[cands[0][0]][cands[0][1]] = true;
        }
      }
    }
  }
  // only 1 mesh in the horizontal line can fill a number
  REP(i,9){
    FOR(k,1,10){
      vector<vl> cands;
      REP(j,9){
        if(candMap[i][j][k]) cands.push_back({i,j,k});
      }
      if(cands.size()==1 && !alreadyFilled[cands[0][0]][cands[0][1]]){
        cout << "Candidate unique in the horizontal: (" << cands[0][0] << "," << cands[0][1] << ") = " << k << endl;
        que.push(cands[0]);
        alreadyFilled[cands[0][0]][cands[0][1]] = true;
      }
    }
  }
  // only 1 mesh in the vertical line can fill a number
  REP(j,9){
    FOR(k,1,10){
      vector<vl> cands;
      REP(i,9){
        if(candMap[i][j][k]) cands.push_back({i,j,k});
      }
      if(cands.size()==1 && !alreadyFilled[cands[0][0]][cands[0][1]]){
        cout << "Candidate unique in the vertical: (" << cands[0][0] << "," << cands[0][1] << ") = " << k << endl;
        que.push(cands[0]);
        alreadyFilled[cands[0][0]][cands[0][1]] = true;
      }
    }
  }
}
void fillNumber(ll i, ll j, ll k){
  number[i][j] = k;
  //cout << "Fill number: (" << i << "," << j << ") = " << k << endl;
}
bool shouldContinue(){
  ll count = 0;
  REP(i,9) REP(j,9) if(number[i][j]==0) count++;
  return count>0;
}
string encodeMatrix(vector<vl> m) {
  string encoded = "";
  REP(i,9){
    REP(j,9){
      char c = '0'+number[i][j];
      encoded += c;
    }
  }
  return encoded;
}
vector<vl> decodeMatrix(string code) {
  vector<vl> decoded(9,vl(9,0));
  REP(i,9){
    REP(j,9){
      ll id = i*9+j;
      decoded[i][j] = code[id]-'0';
    }
  }
  return decoded;
}
void stackCandidateMatrix(){
  vector<vl> cand;
  REP(i,9){
    REP(j,9){
      FOR(k,1,10){
        if(candMap[i][j][k]) cand.push_back({i,j,k});
      }
      if(cand.size()>0) break;
    }
    if(cand.size()>0) break;
  }
  string encoded = encodeMatrix(number);
  REP(i,cand.size()){
    st.push({encoded,cand[i]});
  }
  if(cand.size()==0){
    cout << "No validate candidate found: pop the stack!" << endl;
  } else {
    //cout << "Fill candidate at (" << cand[0][0] << "," << cand[0][1] << ")" << endl;
  }
}
void popCandidateMatrix(){
  string encoded = st.top().first;
  vl vec = st.top().second;
  st.pop();
  cout << "start popping!" << endl;
  print();
  string saveBeforeNumber = encodeMatrix(number);
  number = decodeMatrix(encoded);
  cout << "pop actions until number becomes:" << endl;
  print();
  number = decodeMatrix(saveBeforeNumber);
  while(encodeMatrix(number)!=encoded){
    vl tmp = actionStack.top();
    actionStack.pop();
    cout << "pop action: (" << tmp[0] << "," << tmp[1] << ")<-" << tmp[2] << endl;
    fillNumber(tmp[0],tmp[1],0);
    queLog.push_back({tmp[0],tmp[1],0});
    print();
  }
  actionStack.push(vec);
  fillNumber(vec[0],vec[1],vec[2]);
  queLog.push_back({vec[0],vec[1],vec[2]});
}
bool checkValidation(){
  bool result = true;
  // horizontal limitation
  REP(i,9){
    FOR(k,1,10){
      ll count = 0;
      REP(j,9){
        if(number[i][j]==k) count++;
      }
      result &= count<=1;
    }
  }
  // vertical limitation
  REP(j,9){
    FOR(k,1,10){
      ll count = 0;
      REP(i,9){
        if(number[i][j]==k) count++;
      }
      result &= count<=1;
    }
  }
  // vertical limitation
  REP(bi,3){
    REP(bj,3){
      FOR(k,1,10){
        ll count = 0;
        REP(ui,3){
          REP(uj,3){
            ll i = bi*3+ui;
            ll j = bj*3+uj;
            if(number[i][j]==k) count++;
          }
        }
        result &= count<=1;
      }
    }
  }
  //// no candidate is validate
  //fillCandMap();
  //REP(i,9) REP(j,9) {
  //  ll validCount = 0;
  //  FOR(k,1,10) if(candMap[i][j][k]) validCount++;
  //  if(validCount==0) cout << "(" << i << "," << j << ") has no valid candidate!" << endl; 
  //  result &= validCount>0;
  //}
  return result;
}
int main() {
  cout << "input problem!" << endl;
  REP(i,9) {
    string s;
    cin >> s;
    if(s.length()!=9){
      cout << "invalid line input: " << s << endl;
    }
    REP(j,9) number[i][j] = s[j]-'0';
  }
  cout << "given matrix:" << endl;
  print();
  string givenMatrixString = encodeMatrix(number);
  if(!checkValidation()){
    cout << "validation failed for default input!" << endl;
    print();
    return 1;
  }
  while(shouldContinue() || !checkValidation()){
    if(!checkValidation()){
      //cout << "Validation failure: try another caididate!" << endl;
      if(st.size()==0) {
        //cout << "Error: stack is empty!" << endl;
        return 1;
      }
      popCandidateMatrix();
      //cout << "Promote with:" << endl;
      //print();
    }
    fillCandMap();
    insertQueue();
    if(que.empty()){
      //cout << "No certain move found: try with one of the candidate" << endl;
      //cout << "Current map:" << endl;
      //print();
      stackCandidateMatrix();
      if(st.size()==0) {
        cout << "Error: stack is empty!" << endl;
        return 1;
      }
      popCandidateMatrix();
      //cout << "Promote with:" << endl;
      //print();
    }
    while(!que.empty()) {
      vl vec = que.front();
      que.pop();
      actionStack.push(vec);
      fillNumber(vec[0],vec[1],vec[2]);
      queLog.push_back({vec[0],vec[1],vec[2]});
    }
  }
  cout << "action log:" << endl;
  number = decodeMatrix(givenMatrixString);
  REP(i,queLog.size()){
    cout << "(" << queLog[i][0] << "," << queLog[i][1] << "): " << queLog[i][2] << endl;
    fillNumber(queLog[i][0],queLog[i][1],queLog[i][2]);
    print();
    usleep(100000);
  }
  cout << "answer:" << endl;
  print();
  return 0;
}
