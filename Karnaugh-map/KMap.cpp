#include <iostream>
#include<string>
#include <vector>
using namespace std;

struct Cell{
  bool data;
  int number;
  Cell *left, *down, *right;
  Cell(bool passed_data, int no, Cell* l, Cell* r, Cell* d)
  {
    data = passed_data;
    left = l;
    right = r;
    down = d;
    number = no;
  }
};
vector < vector<Cell> > total_terms;

bool checkMatched(Cell* cell, int pair_size)
{
  for(int i = 0; i<total_terms.size(); i++)
  {
		for(int j = 0; j<total_terms[i].size(); j++)
    {
      if(total_terms[i][j].number == cell->number && total_terms[i].size() == pair_size)
        return true;
    }
  }
  return false;
}
bool checkColumn(Cell* cell)
{
  return cell->data && cell->down->data;
}
bool checkFoursRows(Cell* cell)
{
	int i =0;
  Cell* current_cell = cell;
  vector<Cell> matched;
  while(i<4)
  {
    if(!current_cell->data)
      return false;
    i+=1;
    matched.push_back(*current_cell);
    current_cell = current_cell->right;
  }

	total_terms.push_back(matched);
  return true;
}

bool checkFoursRight(Cell* cell)
{

  if(checkColumn(cell) && checkColumn(cell->right))
  {
    vector<Cell> matched;
   	Cell* rightcell = cell->right;
    matched.push_back(*cell);
    matched.push_back(*rightcell);
    matched.push_back(*cell->down);
    matched.push_back(*rightcell->down);

    total_terms.push_back(matched);
    return true;
  }
}

bool checkFoursLeft(Cell* cell)
{
  if(checkColumn(cell) && checkColumn(cell->left))
  {
    vector<Cell> matched;
   	Cell* leftcell = cell->left;
    matched.push_back(*cell);
    matched.push_back(*leftcell);
    matched.push_back(*cell->down);
    matched.push_back(*leftcell->down);

    total_terms.push_back(matched);
    return true;
  }

}

bool checkFours(Cell* cell)
{
        if(checkFoursRows(cell))
            return true;
        if(checkFoursRight(cell))
            return true;
        if(checkFoursLeft(cell))
            return true;
        return false;
	
}
bool checkTwos(Cell* cell)
{
  // cases of twos
  // case1: the cell we are standing on, matched with the cell on our right.
  // case2: the cell we are standing on, matched with the cell under us.
    Cell* rightcell = cell->right;
    Cell* downcell = cell->down;
    vector<Cell> matched;
    if(cell->data && rightcell->data && !checkMatched(rightcell,2)) // we do not check that the rightcell is matched in a pair of fours, because the redundant case will only appear if the rightcell is in a pairs of two
    {
        matched.push_back(*rightcell);
        matched.push_back(*cell);
        total_terms.push_back(matched);
        return true;
    }
    else
        if(cell->data && downcell->data && !checkMatched(downcell,2))
        {
            matched.push_back(*downcell);
            matched.push_back(*cell);
            total_terms.push_back(matched);
            return true;
        }
    return false;
}

void match_any(Cell* cell) // checks on the three neighbouring cells, if anyone of them is equal to 1 --> match them together else, match it as single term.
{
    vector<Cell> matched;
    matched.push_back(*cell);
    if(cell->right->data)
    {
        matched.push_back(*cell->right);
    }
    else
    if(cell->down->data)
    {
        matched.push_back(*cell->down);
    }
    else
    if(cell->left->data)
    {
        matched.push_back(*cell->left);
    }
    total_terms.push_back(matched);
}
void print_Kmap(vector <Cell*> Kmap)
{
    Cell current_cell = *Kmap[0]; //first rpw
    for(int row =0; row < 2; row++)
    {
        for(int column = 0; column < 4; column++)
        {
            cout<<current_cell.data<<" ";
            current_cell = *(current_cell.right);
        }
        current_cell = *Kmap[0]->down; //second row
        cout<<"\n";
    }
    cout<<endl;
}

void print_terms()
{
    for(int i =0; i<total_terms.size(); i++)
    {
        for(int j=0; j<total_terms[i].size(); j++)
        {
            cout<<total_terms[i][j].number<<" ";
        }
        cout<<endl;
    }
}

void terms_to_expression()
{
  		// minterms = [0,1,4,5,2,6]
  		// total_terms =[[0,4,1,5],
  		//							 [0,4,2,6]]

		// case 0 1 4 5
    // 0: 0000 0000 .... 0000 0000
    // 1: 0000 0000 .... 0000 0001
    // 3: 0000 0000 .... 0000 0100
    // 5: 0000 0000 .... 0000 0101

    // case 0 1 3 2
    // 0: 0000 0000 .... 0000 0000
    // 1: 0000 0000 .... 0000 0001
    // 3: 0000 0000 .... 0000 0011
    // 2: 0000 0000 .... 0000 0010

    /// case 5 7
    // 5: 0000 0000 .... 0000 0101
    // 7: 0000 0000 .... 0000 0111

    // 5: 0000 0000 .... 0000 0101
    // 1: 0000 0000 .... 0000 1010

  		int base_mask = 1;
  		// mask = 0000 0000 .... 0000 0001
      for(int term = 0; term < total_terms.size(); term++)
      {

       string expression = "";
        for(int i = 0; i<3; i++)
        {

          int bit_in_question = base_mask << i;
          /*
            bit_in_question:
            first iteration i = 0
            base_mask (0000 .... 0001) << 0;
            bit_in_question = 0000 0000 .... 0001

            second iteration i = 1
            base_mask << 1;
            bit_in_question = 0000 0000 .... 0010

            third iteration i = 2
            base_mask << 2;
            bit_in_question = 0000 0000 .... 0100
            */


          vector<Cell> current_term = total_terms[term]; // term we are looking at.
          int current_bit_value = bit_in_question & current_term[0].number; // first cell in current term.

          bool flag = true;
          for(int current_cell = 0; current_cell < total_terms[term].size(); current_cell++)
          {

            if(current_bit_value == (current_term[current_cell].number &  bit_in_question) )
              continue;
            else
            {
              flag = false;
              break;
            }
          }

          if(flag )
          {
            switch(i)
            {
              case 0: // I am looking at the C bit.
                if(current_bit_value == 0) // Bit was 0
                  expression += "C'";
                else
                  expression += "C";
                break;
              case 1: // I am looking at the B bit.
                if( current_bit_value == 0 )
                  expression += "B'";
                else
                  expression += "B";
            		break;
              case 2:
                	if(current_bit_value == 0)
                    expression += "A'";
                	else
                    expression += "A";
                break;
            }

          }

        }
        cout<<expression<<endl;
        if(term != total_terms.size() - 1)
          cout<<" + ";
      }
}

int main()
{
  int minTer;
  vector <int> minterms;
  cout<<"Enter n# of Minterms ";
  cin>>minTer;
  if(minTer == 0)
  {
    cout << 0;
    return 0;
  }
  if(minTer == 8)
  {
    cout<< 1;
    return 0;
  }

  for (int i =0; i<minTer; i++)
  {
    int x;
    cin>>x;
    minterms.push_back(x);
  }

	vector <Cell*> Kmap;
  for(int i = 0; i < 8; i++)
      Kmap.push_back( new Cell(false,i,NULL,NULL,NULL));
  for (int i =0; i < 6; i++)
  	{
    	int cellnumber = minterms[i];
    	Kmap[cellnumber]->data = true;
  	}

  int a = 2;
  int b = 1;
  // a = 0000 0000 .... 0000 0010
  // b = 0000 0000 .... 0000 0001
  // a & b = 0000 0000 .... 0000 0000
  // a | b = 0000 0000 .... 0000 0011
  // a << 1 0000 0000 .... 0000 0100
  // a << 2 0000 0000 .... 0000 1000


  Kmap[0]->right = Kmap[1];
  Kmap[0]->down = Kmap[4];
  Kmap[0]->left = Kmap[2];

  Kmap[1]->right = Kmap[3];
  Kmap[1]->left = Kmap[0];
  Kmap[1]->down = Kmap[5];

  Kmap[3]->right = Kmap[2];
  Kmap[3]->down = Kmap[7];
  Kmap[3]->left = Kmap[1];

  Kmap[2]->right = Kmap[0];
  Kmap[2]->down = Kmap[6];
  Kmap[2]->left = Kmap[3];

  Kmap[4]->right = Kmap[5];
  Kmap[4]->down = Kmap[0];
  Kmap[4]->left = Kmap[6];

  Kmap[5]->right = Kmap[7];
  Kmap[5]->down = Kmap[1];
  Kmap[5]->left = Kmap[4];

  Kmap[7]->right = Kmap[6];
  Kmap[7]->down = Kmap[3];
  Kmap[7]->left = Kmap[5];

  Kmap[6]->right = Kmap[4];
  Kmap[6]->down = Kmap[2];
  Kmap[6]->left = Kmap[7];

  for(int i=0;i<Kmap.size(); i++)
  {

    if( !Kmap[i]->data )
        continue;

    if(checkMatched(Kmap[i],4))
      continue;
    if(checkFours(Kmap[i]))
      continue;

    if(checkMatched(Kmap[i],2))
        continue;
    checkTwos(Kmap[i]);
  }
  // after the first loop, if a cell isn't matched, there are only 2 cases:
  // case1: we suspected that it was redundant.
  // case2: it is isolated.
for(int i=0; i<Kmap.size(); i++)
    {
        if(!Kmap[i]->data)
            continue;
        if(checkMatched(Kmap[i],2) || checkMatched(Kmap[i],4))
            continue;
        match_any(Kmap[i]);
    }
  print_Kmap(Kmap);
  print_terms();
	terms_to_expression();

return 0;
}

