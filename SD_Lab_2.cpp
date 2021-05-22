#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <sstream>
#include <fstream>

using namespace std;

struct element
{
    string value;
    int prior;
};

template <typename T>
string toString(T val)
{
    ostringstream oss;
    oss << val;
    return oss.str();
}

void parser(string exp, element* arr,int* size, bool* identity)
{
    int previousPriority = 0;
    string number;
    int j = 0;
    for (int i = 0; i <= exp.length(); i++)
    {
        switch (exp[i])
        {
        case '+':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 2;
            j++;
            break;
        case '-':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
                arr[j].value = exp[i];
                arr[j].prior = 2;
                j++;
                break;
            }
            else
            {
                number = number + exp[i];
                break;
            }
        case '*':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 3;
            j++;
            break;
        case '/':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 3;
            j++;
            break;
        case '(':
            if (!number.empty())
            {
                cout << "error" << endl;
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 1;
            j++;
            break;
        case ')':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 1;
            j++;
            break;
        case '=':
            if (!number.empty())
            {
                arr[j].value = number;
                arr[j].prior = 0;
                j++;
                number.clear();
            }
            arr[j].value = exp[i];
            arr[j].prior = 5;
            j++;
            *identity = true;
            break;
        default:
            number = number + exp[i];
            break;
        }
    }
    if (number[0]!='\0')
    {
        arr[j].value = number;
        arr[j].prior = 0;
    }
    else
    {
        j--;
    }
    *size = j;
}

void translationAlgorithm(element *arr, queue <element>* q, stack <element>* s,int* j)
{
    int lastPrior = 0;
    string lastValue;
    element transit;
  for (int i = 0; i <= *j; i++)
  {
      switch (arr[i].prior)
      {
      case 0:
          q->push(arr[i]);
          lastPrior = arr[i].prior;
          lastValue = arr[i].value;
          break;
      case 1:
          if (arr[i].value == "(")
          {
              s->push(arr[i]);
              break;
          }
          else
          {
              transit = s->top();
              lastValue = transit.value;
              while ((lastValue != "(")and(!s->empty()))
              {
                  q->push(s->top());
                  s->pop();
                  if (!s->empty())
                  {
                      transit = s->top();
                      lastValue = transit.value;
                  }
              }
              s->pop();
              break;
          }
      case 2:
          if (s->empty())
          {
              s->push(arr[i]);
              break;
          }
          else
          {
              transit=s->top();
              if (arr[i].prior > transit.prior)
              {
                  s->push(arr[i]);
                  break;
              }
              else
              {
                  while ((transit.prior >= arr[i].prior)and(!s->empty()))
                  {
                      q->push(s->top());
                      s->pop();
                      if(!s->empty())
                      transit = s->top();
                  }
                  s->push(arr[i]);
                  break;
              }
          }
          break;
      case 3: 
          if (s->empty())
          {
              s->push(arr[i]);
              break;
          }
          else
          {
              transit = s->top();
              if (arr[i].prior > transit.prior)
              {
                  s->push(arr[i]);
                  break;
              }
              else
              {
                  while ((transit.prior >= arr[i].prior) and (!s->empty()))
                  {
                      q->push(s->top());
                      s->pop();
                      if (!s->empty())
                          transit = s->top();
                  }
                  s->push(arr[i]);
                  break;
              }
          }
          break;
      default:
          cout << "default" << endl;
          break;
      }

  }
  while (!s->empty())
  {
      q->push(s->top());
      s->pop();
  }
}

float stackMachine(queue <element>* q)
{
    stack <element> s;
    element transit,order;
    while (!q->empty())
    {
        order = q->front();
        if (order.prior == 0)
        {
            s.push(order);
            q->pop();
        }
        else
        {
            float a, b;
            transit = s.top();
            a = stof(transit.value);
            s.pop();
            transit = s.top();
            b = stof(transit.value);
            s.pop();
            if (order.value == "+")
            {
                transit.value = toString(a+b);
                transit.prior = 0;
                s.push(transit);
                q->pop();
            }
            else if (order.value == "-")
            {
                transit.value = toString(b-a);
                transit.prior = 0;
                s.push(transit);
                q->pop();
            }
            else if (order.value == "*")
            {
                transit.value = toString(a * b);
                transit.prior = 0;
                s.push(transit);
                q->pop();
            }
            else if (order.value == "/")
            {
                transit.value = toString(b/a);
                transit.prior = 0;
                s.push(transit);
                q->pop();
            }
        }
    }
    transit = s.top();
    s.pop();
    float result = stof(transit.value);
    return result;
}

int main()
{
    string expression;
    bool identityCheck = false;
    queue <element> q;
    stack <element> s;
    cout << "Enter math expression" << endl;
    cin >> expression;
    int j = 0;
    element* arr = new element[expression.length()];
    parser(expression, arr,&j,&identityCheck);
    if (identityCheck)
    {
        ofstream outf("Test.txt",ios::app);
        if (!outf)
        {
            cout << "Eror opening file!" << endl;
            exit(1);
        }
        outf << expression << endl;
        outf.close();
        cout << "information written to file" << endl;
    }
    else
    {
        ifstream inf("Test.txt");
        if (!inf)
        {
            cout << "Eror opening file!" << endl;
            exit(1);
        }
        while (inf)
        {
            string strInput;
            inf >> strInput;
            cout << strInput << endl;
            string variable;
            int strLong = strInput.size();
            variable = strInput[0];
            for (int i = 0; i <= j; i++)
            {
                if (arr[i].prior == 0)
                {
                    if (arr[i].value == variable)
                    {
                        cout << "we can swap" << endl;
                        int m = 2;
                        string swap;
                        while (m<=strLong)
                        {
                            swap+= strInput[m];
                            m++;
                        }
                        arr[i].value = swap; 
                    }
                }
            }
        }
        inf.close();
        translationAlgorithm(arr, &q, &s, &j);
        queue<element>qu = q;
        while (!q.empty())
        {
            element transit;
            transit = q.front();
            cout << transit.value << " ";
            q.pop();
        }
        cout << endl;
        cout << stackMachine(&qu);
    }
    delete[]arr;
}