#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <math.h>
#include <sstream>


using namespace std;

struct Stock
{
    public:
    string symbol;
    string name;
    string start_date;
    int number_days;
    float prices[16384];

};

float correlation(Stock A, int inc_a, Stock B, int inc_b, int modifier)
{
    //find k = number of values to compare
    int k = 0;
    if (A.number_days/inc_a < (B.number_days-modifier)/inc_b)
    {
        k = A.number_days/inc_a;
    }
    else
    {
        k = (B.number_days - modifier)/inc_b;
    }

    //perason's correlation algorithm
    float xbar = 0;
    float ybar = 0;
    float counter = 0;

    for (int i = 0; i <= k; i++)
    {
        xbar += A.prices[A.number_days -1 - (i*inc_a)];
        ybar += B.prices[B.number_days -1 - modifier - (i*inc_b)];
    }

    xbar = xbar/(k+1);
    ybar = ybar/(k+1);

    //calculate sums
    float xmxbar = 0;
    float ymybar = 0;
    float xmxymy = 0;
    float xx = 0;
    float yy = 0;


    for (int i = 0; i <= k; i++)
    {
        xmxbar += A.prices[A.number_days - (i*inc_a)] - xbar;
        ymybar += B.prices[B.number_days - modifier - (i*inc_b)] - ybar;
        xmxymy += (A.prices[A.number_days - (i*inc_a)] - xbar) * (B.prices[B.number_days - modifier - (i*inc_b)] - ybar);
        xx += (A.prices[A.number_days - (i*inc_a)] - xbar)*(A.prices[A.number_days - (i*inc_a)] - xbar);
        yy += (B.prices[B.number_days - modifier - (i*inc_b)] - ybar)*(B.prices[B.number_days - modifier - (i*inc_b)] - ybar);
    }

    float S = sqrt(xx*yy);
    return xmxymy/S;
}

struct Row
{
    Stock A;
    int inc_a;
    Stock B;
    int inc_b;
    int modifier;
    float r_value;

};

struct Compare_Row
{
    bool operator()(Row const &A, Row const &B)
    {
        return A.r_value < B.r_value;
    }
};

vector<Row> get_rows(Stock A, Stock B, int modifier)
{
    vector<Row> rows;

    string asymbol = "C:\\Users\\Ryan\\Desktop\\rawstocks\\" + A.symbol + ".csv";
    string bsymbol = "C:\\Users\\Ryan\\Desktop\\rawstocks\\" + B.symbol + ".csv";
    char *asym = new char[asymbol.length()+1];
    char *bsym = new char[bsymbol.length()+1];
    strcpy(asym, asymbol.c_str());
    strcpy(bsym, bsymbol.c_str());
    ifstream ip(asym);
    ifstream ip2(bsym);

    if (ip.is_open() && ip2.is_open())
    {
        int counter = 0;
        string date, price, high, low, close, adjclose, volume, nothing;

        //take first line of csv file 1
        getline(ip, nothing, ',');
        getline(ip, nothing, ',');
        getline(ip, nothing, ',');
        getline(ip, nothing, ',');
        getline(ip, nothing, ',');
        getline(ip, nothing, ',');
        getline(ip, nothing, '\n');

        while (ip.good())
        {
            getline(ip, date, ',');
            getline(ip, price, ',');
            A.prices[counter] = atof(price.c_str());
            getline(ip, high, ',');
            getline(ip, low, ',');
            getline(ip, close, ',');
            getline(ip, adjclose, ',');
            getline(ip, volume, '\n');
            counter++;

        }
        ip.close();
        A.number_days = counter;
        counter = 0;

        //take first line of csv file 2
        getline(ip2, nothing, ',');
        getline(ip2, nothing, ',');
        getline(ip2, nothing, ',');
        getline(ip2, nothing, ',');
        getline(ip2, nothing, ',');
        getline(ip2, nothing, ',');
        getline(ip2, nothing, '\n');

        while (ip2.good())
        {
            getline(ip2, date, ',');
            getline(ip2, price, ',');
            B.prices[counter] = atof(price.c_str());
            getline(ip2, high, ',');
            getline(ip2, low, ',');
            getline(ip2, close, ',');
            getline(ip2, adjclose, ',');
            getline(ip2, volume, '\n');
            counter++;

        }
        ip2.close();
        B.number_days = counter;

        int inc_a_range = 1;
        int inc_b_range = 1;
        Row row;

        for (int i = 1; i <= inc_a_range; i++)
        {
            for (int j = 1; j <= inc_b_range; j++)
            {
                float cs = correlation(A, i, B, j, modifier);
                row.A = A;
                row.inc_a = i;
                row.B = B;
                row.inc_b = j;
                row.modifier = modifier;
                row.r_value = cs;
                rows.push_back(row);
            }
        }


    }//end if (ip.is_open() && ip2.is_open())
    delete [] asym;
    delete [] bsym;

    return rows;
}

int main()
{
    //change this value here
    int modifier = 1;

    stringstream ss;
    ss << modifier;
    string outputname = "output"  + ss.str() + ".txt";
    string output2name = "results" + ss.str() + ".csv";
    char *ot = new char[outputname.length()+1];
    char *ot2 = new char[output2name.length()+1];
    strcpy(ot, outputname.c_str());
    strcpy(ot2, output2name.c_str());

    ofstream output(ot);
    ofstream output2(ot2);
    cout << "start" << endl;
    Stock A;
    Stock B;
    priority_queue<Row, vector<Row>, Compare_Row> pq;
    vector<string> stocks;

    ifstream AllStocks("JustSymbols.csv");
    if (!AllStocks.is_open()) cout << "Error File AllStocks open" << endl;

    while (AllStocks.good())
    {
        string SSS;
        getline(AllStocks, SSS);
        stocks.push_back(SSS);
    }
    AllStocks.close();

/*
    cout << "part 1 test MMM && ABT" << endl;
    A.symbol = "MMM";
    B.symbol = "ABT";
    vector<Row> rows = get_rows(A, B);
    for (int i = 0; i < rows.size(); i++)
    {
        cout << rows[i].A.symbol << " && " << rows[i].B.symbol << " (" << rows[i].inc_a << ", " << rows[i].inc_b << ") -" << rows[i].modifier << ":  " << rows[i].r_value << endl;
    }
*/

    cout << "part 1 test all" << endl;
    for (int i = 0; i < stocks.size(); i++)
    {
        cout << "Stock " << stocks[i] << endl;
        for (int j = 0; j < stocks.size(); j++)
        {
            if (stocks[i] != stocks[j])
            {
                A.symbol = stocks[i];
                B.symbol = stocks[j];
                vector<Row> rows = get_rows(A, B, modifier);
                for (int l = 0; l < rows.size(); l++)
                {
                    //pq.push(rows[l]);
                    output << rows[l].A.symbol << " && " << rows[l].B.symbol << " ("<< rows[l].inc_a << ", " << rows[l].inc_b << ") -" << rows[l].modifier <<":  " << rows[l].r_value << endl;
                    output2 << rows[l].A.symbol << "," << rows[l].B.symbol << ","<< rows[l].inc_a << "," << rows[l].inc_b << "," << rows[l].modifier <<"," << rows[l].r_value << endl;
                }
            }
        }
    }



    cout << "part 2" << endl;

/*
    while (!pq.empty())
    {
        Row top = pq.top();
        cout << top.A.symbol << endl;
        output << top.A.symbol << " && " << top.B.symbol << " ("<< top.inc_a << ", " << top.inc_b << ") -" << top.modifier <<":  " << top.r_value << endl;
        pq.pop();
    }
*/
    cout << "end" << endl;

    output.close();
    output2.close();

    return 0;
}
