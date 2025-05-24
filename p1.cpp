#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
	ifstream fin("p1.in");
	ofstream fout("p1.out");
	int N;
	fin >> N;
	vector<int> v(N + 1);
	int max_dist = -1;
	for (int i = 1; i <= N; i++) {
		fin >> v[i];
		if (v[1] != 0) {
			fout << -1;
			return 0;
		}
		if (v[i] == 0 && i != 1) {
			fout << -1;
			return 0;
		}
		max_dist = max(max_dist, v[i]);
	}
	vector<vector<int>> layer(max_dist + 1);
	vector<pair<int, int>> edges;

	for (int i = 1; i <= N; i++) {
		layer[v[i]].push_back(i);
	}

	for (int i = 1; i <= max_dist; i++) {
		if (layer[i].empty())
			continue;
		if (layer[i - 1].empty()) {
			// nu ai la ce sa te conectezi
			fout << -1;
			return 0;
		}
		// nr de parinti din stratul i-1
		int parents = (int)layer[i - 1].size();
		for (int d = 0; d < (int)layer[i].size(); d++) {
			int node = layer[i][d];
			int parent = layer[i - 1][d % parents];
			pair<int, int> aux = {parent, node};
			edges.push_back(aux);
		}
	}

	fout << edges.size() << endl;
	for (auto e : edges) {
		fout << e.first << " " << e.second << endl;
	}
	fin.close();
	fout.close();
	return 0;
}
