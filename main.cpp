#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

struct Node
{
    string symbol;
    double probability;
    string code;
};

bool compare(Node a, Node b)
{
    return a.probability > b.probability;
}

void calculatePrefixSum(vector<Node> &nodes, vector<double> &prefixSum)
{
    prefixSum[0] = nodes[0].probability;
    for (int i = 1; i < nodes.size(); i++)
    {
        prefixSum[i] = prefixSum[i - 1] + nodes[i].probability;
    }
}

double getEntropy(vector<Node> &nodes)
{
    double entropy = 0;
    for (const auto &node : nodes)
    {
        entropy += node.probability * log2(1 / node.probability);
    }
    return entropy;
}

double getAverageLength(vector<Node> &nodes)
{
    double averageLength = 0;
    for (const auto &node : nodes)
    {
        averageLength += node.probability * node.code.length();
    }
    return averageLength;
}

double getExpectedLength(double prob) {
    return -log2(prob);
}

// Hàm thực hiện thuật toán Shannon-Fano để mã hóa các ký tự
void shannonFano(vector<Node> &nodes, int start, int end, vector<double> &prefixSum)
{
    // Điều kiện dừng: nếu chỉ còn 1 phần tử hoặc không còn phần tử nào
    if (start >= end)
        return;

    // cout << "--------------------------------------------------------" << endl;
    // cout << "Doan: " << start << " -> " << end << endl;

    // Tính tổng xác suất của các phần tử trong đoạn hiện tại
    double total = prefixSum[end] - (start > 0 ? prefixSum[start - 1] : 0);
    // cout << "Tong xac suat: " << total << endl;

    // Tìm điểm chia để tổng xác suất hai nhóm gần bằng nhau nhất
    double half = total / 2;
    // cout << "Nua tong xac suat: " << half << endl;
    int split = start;
    double minDiff = total;
    for (int i = start; i < end; i++)
    {
        double leftSum = prefixSum[i] - (start > 0 ? prefixSum[start - 1] : 0);
        // double rightSum = suffixSum[i + 1];
        double diff = abs(leftSum - half);
        if (diff < minDiff)
        {
            minDiff = diff;
            split = i;
        }
    }
    // cout << "Diem chia: " << split << endl;
    // cout << "2 nua tong tai diem chia: " << prefixSum[split] - (start > 0 ? prefixSum[start - 1] : 0) << " va " << suffixSum[split + 1] << endl;

    // Gán mã 0 cho nhóm bên trái (có xác suất lớn hơn)
    for (int i = start; i <= split; i++)
    {
        nodes[i].code += "0";
    }
    // Gán mã 1 cho nhóm bên phải (có xác suất nhỏ hơn)
    for (int i = split + 1; i <= end; i++)
    {
        nodes[i].code += "1";
    }

    // Đệ quy thực hiện tiếp cho hai nhóm con
    shannonFano(nodes, start, split, prefixSum);
    shannonFano(nodes, split + 1, end, prefixSum);
}

int main()
{
    map<string, int> frequencies;
    string symbol;
    int frequency;

    cout << "Nhap ky hieu va tan suat (ket thuc voi EOF):" << endl;
    while (cin >> symbol >> frequency)
    {
        frequencies[symbol] = frequency;
    }

    int totalFrequency = 0;
    for (const auto &pair : frequencies)
    {
        totalFrequency += pair.second;
    }

    vector<Node> nodes;
    for (const auto &pair : frequencies)
    {
        nodes.push_back({pair.first, static_cast<double>(pair.second) / totalFrequency, ""});
    }

    sort(nodes.begin(), nodes.end(), compare);

    vector<double> prefixSum(nodes.size());
    calculatePrefixSum(nodes, prefixSum);
    cout << endl;

    shannonFano(nodes, 0, nodes.size() - 1, prefixSum);
    
    cout << "Bang ma Shannon-Fano:" << endl;

    cout << left << setw(10) << "Ky hieu" << setw(10) << "Tan suat" << setw(15) << "Xac suat" << setw(10) << "Ma" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (const auto &node : nodes)
    {
        cout << left << setw(10) << node.symbol << setw(10) << frequencies[node.symbol] << setw(15) << node.probability << setw(10) << node.code << endl;
    }

    cout << "--------------------------------------------------------" << endl;
    cout << "Entropy: " << getEntropy(nodes) << endl;
    cout << "Do dai trung binh: " << getAverageLength(nodes) << endl << endl;

    return 0;
}
