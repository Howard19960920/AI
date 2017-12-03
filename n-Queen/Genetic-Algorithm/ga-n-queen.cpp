#include <bits/stdc++.h>
using namespace std;


typedef pair<int, vector<int>> chromosome;


#define MAX_BOARD_SIZE 256


int nQueen, pplSize, tGen, runs, seeder = 0, nkid, success = 0;
float xOver;
stack<int> sexPool;
vector<chromosome> ppl;



/* 計算每個個體 fitness 的值, time complexity O(n^2) */

__attribute__((always_inline)) inline void getfitness(chromosome & x)
{
  int cnter = 0;
  for(int i = 0; i < nQueen; i++)
  {
    for(int j = i + 1; j < nQueen; j++)
    {
      // 右上對角
      if(x.second[i] - j + i == x.second[j])
        cnter++;
      // 右橫列
      if(x.second[i] == x.second[j])
        cnter++;
      // 右下對角
      if(x.second[i] + j - i == x.second[j])
        cnter++;
    }
  }
  x.first = -cnter;
}


/* ***********************************************************************
 *        Time Complexity O(Generation * (populations * crossRate) * n^2)
 *************************************************************************
 *  1. 產生 crossOver rate * population size 個子代, 2-tournament 選擇親代
 *  2. 隨機產生個體變異 P = 1 / numQueen
 *  3. 透過 fitness 淘汰最差的 k 個直到 population size 回到初始值
 *  4. Goto 1.
 * ***********************************************************************/

void Genetic_Algorithm()
{
  srand(time(NULL) + (seeder++));

  int idx1, idx2, cutedge;
  chromosome temp;

  for(int t = 0; t < tGen; t++)
  {
    // 根據 crossOver rate 決定每一 generation 產生子代的數目 nkid
    for(int i = 0; i < nkid; i++)
    {
      // 親代選拔 2-tournament
      while(sexPool.size() != 2)
      {
        idx1 = rand() % nQueen;
        idx2 = rand() % nQueen;
        if(ppl[idx1].first > ppl[idx2].first)
          sexPool.push(idx1);
        else
          sexPool.push(idx2);
      }

      // Single point crossover
      temp.second.clear();
      cutedge = rand() % nQueen;

      for(int i = 0; i < cutedge; i++)
        temp.second.push_back(ppl.at(sexPool.top()).second.at(i));
      sexPool.pop();

      for(int i = cutedge; i < nQueen; i++)
        temp.second.push_back(ppl.at(sexPool.top()).second.at(i));
      sexPool.pop();

      // 子代變異 Mutation, rate: 1/nQueen
      temp.second.at(rand() % nQueen) = rand() % nQueen;
      // 計算生存值
      getfitness(temp);
      ppl.push_back(temp);
    }

    // 競爭 fitness-based 留下最好的前 pplSize 個子代
    sort(ppl.begin(), ppl.end());
    ppl.erase(ppl.begin(), ppl.begin() + (ppl.size() - pplSize));
    if(ppl.at(ppl.size() - 1).first == 0)
    {
      success++;
      for(int i = 0; i < nQueen; i++)
      {
        for(int j = 0; j < nQueen; j++)
        {
          if(ppl.at(ppl.size() - 1).second.at(j) == i)
            printf(" 1");
          else
            printf(" 0");
        }
        printf("\n");
      }
      return;
    }
    random_shuffle(ppl.begin(), ppl.end());
    assert(pplSize == ppl.size());
  }
}


void initialize()
{
  // 清空變數
  ppl.clear();
  while(not sexPool.empty())
    sexPool.pop();

  srand(time(NULL) + (seeder++));

  // 隨機產生大小為 population size 的族群
  chromosome temp;

  for(int i = 0; i < pplSize; i++)
  {
    temp.second.clear();

    for(int j = 0; j < nQueen; j++)
    {
      temp.second.push_back(rand() % nQueen);
    }
    // 計算 fitness 值
    getfitness(temp);
    ppl.push_back(temp);
  }
}


int main(int argc, char **argv)
{
  printf("plz input number of queen, population size, crossover rate, generation, and runs in order\n");
  scanf("%d%d%f%d%d", &nQueen, &pplSize, &xOver, &tGen, &runs);
  // 每一世代需透過crossover產生的個體數目
  nkid = floor(xOver * nQueen);
  assert(nQueen > 0 and 0 < pplSize and 0 < xOver and 0 < tGen and 0 < runs and nQueen <= pplSize);
  for(int r = 0; r < runs; r++)
  {
    initialize();
    Genetic_Algorithm();
  }
  printf("Success Rate: %d / %d\n", success, runs);
  return 0;
}
