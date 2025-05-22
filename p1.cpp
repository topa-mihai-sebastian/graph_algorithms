#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ifstream fin("p1.in");
	ofstream fout("p1.out");
	int N;
	fin >> N;
	vector<int> v(N + 1);
	for (int i = 1; i <= N; i++)
	{
		fin >> v[i];
	}

	if (v[1] != 0)
	{
		fout << -1;
		return 0;
	}

	int max_dist = -1;
	for (int i = 1; i <= N; i++)
	{
		max_dist = max(max_dist, v[i]);
		if (v[i] == 0 && i != 1)
		{
			fout << -1;
			return 0;
		}
	}
	vector<pair<int, int>> edges;
	vector<vector<int>> layer(N + 1);
	for (int i = 1; i <= N; i++)
	{
		layer[v[i]].push_back(i);
	}
	for (int d = 1; d <= max_dist; d++)
	{
		if (layer[d].empty())
			continue;
		if (layer[d - 1].empty())
		// nu ai cum sa legi nodul
		{
			fout << -1;
			return 0;
		}
		int parents = layer[d - 1].size();
		for (int i = 0; i < (int)layer[d].size(); i++)
		{
			int node = layer[d][i];
			int parent = layer[d - 1][i % parents];
			pair<int, int> aux = {parent, node};
			edges.push_back(aux);
		}
	}
	fout << edges.size() << endl;
	for (auto e : edges)
	{
		fout << e.first << " " << e.second << endl;
	}
	fin.close();
	fout.close();
	return 0;
}