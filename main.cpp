#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

struct Node {
    string symbol;
    double probability;
    string code;
};

bool compare(Node a, Node b) {
    return a.probability > b.probability;
}

void shannonFano(vector<Node>& nodes, int start, int end) {
    if (start >= end) return;
    double total = 0;
    for (int i = start; i <= end; i++) {
        total += nodes[i].probability;
    }
    double half = total / 2;
    double acc = 0;
    int split = start;
    for (int i = start; i <= end; i++) {
        acc += nodes[i].probability;
        if (acc >= half) {
            split = i;
            break;
        }
    }
    for (int i = start; i <= split; i++) {
        nodes[i].code += "0";
    }
    for (int i = split + 1; i <= end; i++) {
        nodes[i].code += "1";
    }
    shannonFano(nodes, start, split);
    shannonFano(nodes, split + 1, end);
}

int main() {
    int q;
    cout << "Nhap so luong tin q (<12): ";
    cin >> q;
    if (q >= 12) {
        cout << "So luong tin phai nho hon 12." << endl;
        return 1;
    }

    vector<Node> nodes(q);
    double totalProbability = 0;

    for (int i = 0; i < q; i++) {
        cout << "Nhap tin thu " << i + 1 << ": ";
        cin >> nodes[i].symbol;
        cout << "Nhap xac suat cua tin " << nodes[i].symbol << ": ";
        cin >> nodes[i].probability;
        if (nodes[i].probability >= 1) {
            cout << "Xac suat cua moi tin phai nho hon 1." << endl;
            return 1;
        }
        totalProbability += nodes[i].probability;
    }

    if (totalProbability != 1) {
        cout << "Tong xac suat cac tin phai bang 1." << endl;
        return 1;
    }

    sort(nodes.begin(), nodes.end(), compare);

    shannonFano(nodes, 0, q - 1);

    cout << "Bang ma Shannon-Fano:" << endl;
    for (const auto& node : nodes) {
        cout << "Tin: " << node.symbol << " - Ma: " << node.code << endl;
    }

    return 0;
}
