#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

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

void calculateSuffixSum(vector<Node> &nodes, vector<double> &suffixSum)
{
    suffixSum[nodes.size() - 1] = nodes[nodes.size() - 1].probability;
    for (int i = nodes.size() - 2; i >= 0; i--)
    {
        suffixSum[i] = suffixSum[i + 1] + nodes[i].probability;
    }
}

// Hàm thực hiện thuật toán Shannon-Fano để mã hóa các ký tự
void shannonFano(vector<Node> &nodes, int start, int end, vector<double> &prefixSum, vector<double> &suffixSum)
{
    // Điều kiện dừng: nếu chỉ còn 1 phần tử hoặc không còn phần tử nào
    if (start >= end)
        return;

    // Tính tổng xác suất của các phần tử trong đoạn hiện tại
    double total = prefixSum[end] - (start > 0 ? prefixSum[start - 1] : 0);

    // Tìm điểm chia để tổng xác suất hai nhóm gần bằng nhau nhất
    double half = total / 2;
    int split = start;
    double minDiff = total;
    for (int i = start; i < end; i++)
    {
        double leftSum = prefixSum[i] - (start > 0 ? prefixSum[start - 1] : 0);
        double rightSum = suffixSum[i + 1];
        double diff = abs(leftSum - rightSum);
        if (diff < minDiff)
        {
            minDiff = diff;
            split = i;
        }
    }

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
    shannonFano(nodes, start, split, prefixSum, suffixSum);
    shannonFano(nodes, split + 1, end, prefixSum, suffixSum);
}

int main()
{
    int q;
    cout << "Nhap so luong tin q (<12): ";
    cin >> q;
    if (q >= 12)
    {
        cout << "So luong tin phai nho hon 12." << endl;
        return 1;
    }

    vector<Node> nodes(q);
    vector<double> prefixSum(q);
    vector<double> suffixSum(q);

    double totalProbability = 0;

    for (int i = 0; i < q; i++)
    {
        cout << "Nhap tin thu " << i + 1 << ": ";
        cin >> nodes[i].symbol;
        cout << "Nhap xac suat cua tin " << nodes[i].symbol << ": ";
        cin >> nodes[i].probability;
        if (nodes[i].probability >= 1)
        {
            cout << "Xac suat cua moi tin phai nho hon 1." << endl;
            return 1;
        }
        totalProbability += nodes[i].probability;
    }

    if (totalProbability - 1.0 > 1e-6)
    {
        cout << "Tong xac suat cac tin phai bang 1." << endl;
        return 1;
    }

    sort(nodes.begin(), nodes.end(), compare);
    calculatePrefixSum(nodes, prefixSum);
    calculateSuffixSum(nodes, suffixSum);

    shannonFano(nodes, 0, q - 1, prefixSum, suffixSum);

    cout << "Bang ma Shannon-Fano:" << endl;
    for (const auto &node : nodes)
    {
        cout << "Tin: " << node.symbol << " - Ma: " << node.code << endl;
    }

    return 0;
}
