#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

namespace Util
{
  vector<vector<int> > AddMatrix(vector<vector<int> > a, vector<vector<int> > b, int multiplier = 1)
  {
    int size = a.size();
  
    for (int i = 0; i < size; i++)
    {
      for (int j = 0; j < size; j++)
      {
        a[i][j] = a[i][j] + (multiplier * b[i][j]);
      }
    }
    return a;
  }

  void DisplayMatrix(vector<vector<int> >& resultMatrix)
  {
    for (int i = 0; i < resultMatrix.size(); i++)
    {
      for (int j = 0; j < resultMatrix[0].size(); j++)
      {
        cout << resultMatrix[i][j] << "|";
      }
      cout << "\n";
    }
  }
}
/**
 * This holds true for square matrices
 * c at ij => summation(k = 0 to k < size) c of ij + a at ik* b at kj
 * */
vector<vector<int> > SquareIterativeMulitplyMatrix(vector<vector<int> > &matrixA, vector<vector<int> > &matrixB)
{
  int size = matrixA.size();
  vector<int> inner(size, 0);
  vector<vector<int> > c(size, inner);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {

      for (int k = 0; k < size; k++)
      {
        c[i][j] = c[i][j] + matrixA[i][k] * matrixB[k][j];
      }
    }
  }

  return c;
}

// recursive approach!!!
/**
 * Assume n is always of the power of 2 for now
 * Partition A, B, and C into four n/2 × n/2 matrices
 * Page 15 https://www.cl.cam.ac.uk/teaching/1415/AdvAlgo/lec4_ann.pdf
 * */
vector<vector<int> > RecursiveMultiplyMatrix(vector<vector<int> > matrixA, vector<vector<int> > matrixB)
{
  int col1 = matrixA[0].size();
  int col2 = matrixB[0].size();

  int row1 = matrixA.size();
  int row2 = matrixB.size();

  if (col1 != row2)
  {
    vector<vector<int> > nope;
    return nope;
  }

  vector<int> resultRow(col1, 0);
  vector<vector<int> > resultMatrix(row1, resultRow);

  if (col1 == 1)
  {
    resultMatrix[0][0] = matrixA[0][0] * matrixB[0][0];
    return resultMatrix;
  }
  else
  {
    int splitIdx = col1 / 2;

    vector<int> rowVector(splitIdx, 0);

    // create 4 partitions of n / 2 * n / 2 for a & b
    vector<vector<int> > a00(splitIdx, rowVector);
    vector<vector<int> > a01(splitIdx, rowVector);
    vector<vector<int> > a10(splitIdx, rowVector);
    vector<vector<int> > a11(splitIdx, rowVector);

    vector<vector<int> > b00(splitIdx, rowVector);
    vector<vector<int> > b01(splitIdx, rowVector);
    vector<vector<int> > b10(splitIdx, rowVector);
    vector<vector<int> > b11(splitIdx, rowVector);

    // populate the n/2 matrices of a and b
    for (int i = 0; i < splitIdx; i++)
    {
      for (int j = 0; j < splitIdx; j++)
      {
        a00[i][j] = matrixA[i][j];
        a01[i][j] = matrixA[i][j + splitIdx];
        a10[i][j] = matrixA[splitIdx + i][j];
        a11[i][j] = matrixA[i + splitIdx][j + splitIdx];

        b00[i][j] = matrixB[i][j];
        b01[i][j] = matrixB[i][j + splitIdx];
        b10[i][j] = matrixB[splitIdx + i][j];
        b11[i][j] = matrixB[i + splitIdx][j + splitIdx];
      }
    }

    auto result00 = Util::AddMatrix(RecursiveMultiplyMatrix(a00, b00), RecursiveMultiplyMatrix(a01, b10));
    auto result01 = Util::AddMatrix(RecursiveMultiplyMatrix(a00, b01), RecursiveMultiplyMatrix(a01, b11));
    auto result10 = Util::AddMatrix(RecursiveMultiplyMatrix(a10, b00), RecursiveMultiplyMatrix(a11, b10));
    auto result11 = Util::AddMatrix(RecursiveMultiplyMatrix(a10, b01), RecursiveMultiplyMatrix(a11, b11));

    // now copy the values from result00 and others into main returing result vector
    for (int i = 0; i < splitIdx; i++)
    {
      for (int j = 0; j < splitIdx; j++)
      {
        resultMatrix[i][j] = result00[i][j];
        resultMatrix[i][j + splitIdx] = result01[i][j];
        resultMatrix[splitIdx + i][j] = result10[i][j];
        resultMatrix[i + splitIdx][j + splitIdx] = result11[i][j];
      }
    }

    return resultMatrix;
  }
}

vector<vector<int> > StrassensMultiplication(vector<vector<int> > a, vector<vector<int> > b)
{
  int colA = a[0].size();
  int rowA = a.size();
  int colB = b[0].size();
  int rowB = b.size();
  
  if (colA != rowB) {
    return {};
  }

  vector<int> resultMatrixRow(colA, 0);
  vector<vector<int> > resultMatrix(rowA, resultMatrixRow);


  if (colA == 1)
  {
    resultMatrix[0][0] = a[0][0] * b[0][0];
    return resultMatrix;
  }
  // divide in 4 n/2 x n/2 submatrices, the resutXX's will hold the result for the combined
  // parts and then have to be merged
  int nx2 = colA / 2;
  vector<int> nx2Row(nx2, 0);

  // now split matrix a and matrix b into 4 n/2 x n/2 matrices each
  vector<vector<int> > a00(nx2, nx2Row);
  vector<vector<int> > a01(nx2, nx2Row);
  vector<vector<int> > a10(nx2, nx2Row);
  vector<vector<int> > a11(nx2, nx2Row);
  vector<vector<int> > b00(nx2, nx2Row);
  vector<vector<int> > b01(nx2, nx2Row);
  vector<vector<int> > b10(nx2, nx2Row);
  vector<vector<int> > b11(nx2, nx2Row);

  for (int i = 0; i < nx2; i++)
  {
    for (int j = 0; j < nx2; j++)
    {
      a00[i][j] = a[i][j];
      a10[i][j] = a[i + nx2][j];
      a01[i][j] = a[i][j + nx2];
      a11[i][j] = a[i + nx2][j + nx2];

      b00[i][j] = b[i][j];
      b10[i][j] = b[i + nx2][j];
      b01[i][j] = b[i][j + nx2];
      b11[i][j] = b[i + nx2][j + nx2];
    }
  }

  // p = a00 * (b01 + b11)
  vector<vector<int> > p(StrassensMultiplication(a00, Util::AddMatrix(b01, b11, -1)));

  // q = b11 * (a00 + a01)
  vector<vector<int> > q(StrassensMultiplication(Util::AddMatrix(a00, a01), b11));

  // r = b00 * (a10 + a11)
  vector<vector<int> > r(StrassensMultiplication(Util::AddMatrix(a10, a11), b00));

  // s = a11 * (b10 + b00)
  vector<vector<int> > s(StrassensMultiplication(a11, Util::AddMatrix(b10, b00, -1)));

  // t = (a00 + a11) * (b00 + b11)
  vector<vector<int> > t(StrassensMultiplication(Util::AddMatrix(a00, a11), Util::AddMatrix(b00, b11)));

  // u = (a01 + a11) * (b10 + b11)
  vector<vector<int> > u(StrassensMultiplication(Util::AddMatrix(a01, a11, -1), Util::AddMatrix(b10, b11)));

  // v = (a00 + a10) * (b00 + b01)
  vector<vector<int> > v(StrassensMultiplication(Util::AddMatrix(a00, a10, -1), Util::AddMatrix(b00, b01)));

  // Computing the values of the 4 quadrants of the final matrix c

  auto result00(Util::AddMatrix(Util::AddMatrix(Util::AddMatrix(t, s), u), q, -1));

  auto result01(Util::AddMatrix(p, q));

  auto result10(Util::AddMatrix(r, s));

  auto result11(Util::AddMatrix(Util::AddMatrix(Util::AddMatrix(t, p), r, -1), v, -1));

  // now combine resultXX into one big result and return it
  for (int i = 0; i < nx2; i++)
  {
    for (int j = 0; j < nx2; j++)
    {
      resultMatrix[i][j] = result00[i][j];
      resultMatrix[i][j + nx2] = result01[i][j];
      resultMatrix[nx2 + i][j] = result10[i][j];
      resultMatrix[i + nx2][j + nx2] = result11[i][j];
    }
  }

  return resultMatrix;
}

int main()
{
  vector<vector<int> > matrixA = {{1, 1, 1, 1},
                                  {2, 2, 2, 2},
                                  {3, 3, 3, 3},
                                  {2, 2, 2, 2}};

  vector<vector<int> > matrixB = {{1, 1, 1, 1},
                                  {2, 2, 2, 2},
                                  {3, 3, 3, 3},
                                  {2, 2, 2, 2}};

  cout << "Let Matrix A \n";

  Util::DisplayMatrix(matrixA);

  cout << "Let Matrix B \n";

  Util::DisplayMatrix(matrixB);

  vector<vector<int> > iterMatrix = SquareIterativeMulitplyMatrix(matrixA, matrixB);
  
  cout << "Iterative Multiplication: \n";

  Util::DisplayMatrix(iterMatrix);
  
  vector<vector<int> > recurMatrix = RecursiveMultiplyMatrix(matrixA, matrixB);
  
  cout << "Recursive Multiplication: \n";

  Util::DisplayMatrix(recurMatrix);
  
  vector<vector<int> > strassenMatrix = StrassensMultiplication(matrixA, matrixB);
  
  cout << "Strassen's Multiplication: \n";

  Util::DisplayMatrix(strassenMatrix);
}
