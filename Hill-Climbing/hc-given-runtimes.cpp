#include <bits/stdc++.h>
using namespace std;


#define MAX_BOARD_SIZE 256


struct ds_candidate
{
  int numAttk, xaxis, yaxis;
};


int nQueen, board[MAX_BOARD_SIZE][MAX_BOARD_SIZE], qlist[MAX_BOARD_SIZE], curAttk, idx, seeder, runs, success;
vector<ds_candidate> candidate;


void init()
{
  assert(nQueen < MAX_BOARD_SIZE);
  srand(time(NULL) + (seeder++));

  // 每一 Column 皇后初始位置
  for(int i = 0; i < nQueen; i++)
    qlist[i] = rand() % nQueen;
  // 計算初始的衝突數
  curAttk = 0;
  for(int i = 0; i < nQueen; i++)
  {
    for(int j = i + 1; j < nQueen; j++)
    {
      // 右上對角
      if(qlist[i] - j + i == qlist[j])
        curAttk++;
      // 右橫列
      if(qlist[i] == qlist[j])
        curAttk++;
      // 右下對角
      if(qlist[i] + j - i == qlist[j])
        curAttk++;
    }
  }
}


void hill_climbing()
{
  srand(time(NULL) + (seeder++));
  while(true)
  {
    // 計算相對衝突變量  O(k * N^2), where k is a constant
    memset(&board[0][0], 0, sizeof(board));

    for(int i = 0; i < nQueen; i++)
    {
      // 左橫列
      for(int j = i - 1; j >= 0; j--)
        board[qlist[i]][j]++;
      // 右橫列
      for(int j = i + 1; j < nQueen; j++)
        board[qlist[i]][j]++;
      // 右上對角
      for(int j = i + 1; j < nQueen && 0 <= qlist[i] - j + i; j++)
        board[qlist[i] - j + i][j]++;
      // 右下對角
      for(int j = i + 1; j < nQueen && qlist[i] + j - i < nQueen; j++)
        board[qlist[i] + j - i][j]++;
      // 左上對角
      for(int j = i - 1; j >= 0 && 0 <= qlist[i] + j - i; j--)
        board[qlist[i] + j - i][j]++;
      // 左下對角
      for(int j = i - 1; j >= 0 && qlist[i] - j + i < nQueen; j--)
        board[qlist[i] - j + i][j]++;
    }

    // 建立下一步棋的候選表
    candidate.clear();
    candidate.push_back({curAttk, -1, -1});

    for(int i = 0; i < nQueen; i++)
    {
      for(int j = 0; j < nQueen; j++)
      {
        // 找到更好的選擇
        if(curAttk - board[qlist[i]][i] + board[j][i] < candidate[0].numAttk)
        {
          candidate.clear();
          candidate.push_back({curAttk - board[qlist[i]][i] + board[j][i], j, i});
        }
        // 找到一樣好的選擇
        else if(curAttk - board[qlist[i]][i] + board[j][i] == candidate[0].numAttk)
        {
          candidate.push_back({candidate[0].numAttk, j, i});
        }
      }
    }
    // 已是 local(global) optimal solution
    if(candidate[0].numAttk == curAttk)
    {
      break;
    }
    // 在順位相同的這些候選名單中, 隨機選擇一個 node 去擴展
    else
    {
      idx = rand() % candidate.size();
      curAttk = candidate[idx].numAttk;
      qlist[candidate[idx].yaxis] = candidate[idx].xaxis;
    }
  }
  // 計算找到最佳解的次數
  if(curAttk == 0)
    success++;
}



int main(int argc, char **argv)
{
  if(argc != 3)
  {
    fprintf(stderr, "Too few or too many arguments\n");
    exit(1);
  }

  nQueen = (int) atoi(argv[1]);
  runs = (int) atoi(argv[2]);
  assert(nQueen > 0 and 0 < runs);
  success = 0;
  seeder = 0;

  for(int i = 0; i < runs; i++)
  {
    init();
    hill_climbing();
  }

  printf("Rate: %d / %d\n", success, runs);
  return 0;
}
