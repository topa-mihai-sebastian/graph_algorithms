#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <utility>
#include <iostream>
using namespace std;
using pii = pair<int, int>;
/*nt main()
{
	// Deschidere fișiere
	ifstream fin("p2.in");
	ofstream fout("p2.out");

	int N, M;
	long long K;
	fin >> N >> M >> K;

	int NM = N * M;

	// 1) Citim matricea pe un vector 1D și colectăm valorile
	vector<long long> a(NM);
	vector<long long> vals;
	vals.reserve(NM);
	for (int i = 0; i < NM; ++i)
	{
		fin >> a[i];
		vals.push_back(a[i]);
	}

	// 2) Sortăm și păstrăm doar valorile unice
	sort(vals.begin(), vals.end());
	vals.erase(unique(vals.begin(), vals.end()), vals.end());
	int U = vals.size();

	// 3) Mapare valoare -> index în vals[] și liste de poziții
	vector<int> idxMat(NM);
	vector<vector<int>> pos_list(U);
	for (int id = 0; id < NM; ++id)
	{
		int idx = int(lower_bound(vals.begin(), vals.end(), a[id]) - vals.begin());
		idxMat[id] = idx;
		pos_list[idx].push_back(id);
	}

	// 4) Prefix-sum al numerelor de celule pentru fiecare idx
	vector<int> prefix_cnt(U + 1, 0);
	for (int i = 0; i < U; ++i)
	{
		prefix_cnt[i + 1] = prefix_cnt[i] + int(pos_list[i].size());
	}

	// 5) Construim lista de adiacență din grid 4-direcții
	vector<vector<int>> adj(NM);
	int dx[4] = {-1, 1, 0, 0}, dy[4] = {0, 0, -1, 1};
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			int u = i * M + j;
			for (int d = 0; d < 4; ++d)
			{
				int ni = i + dx[d];
				int nj = j + dy[d];
				if (ni < 0 || ni >= N || nj < 0 || nj >= M)
					continue;
				int v = ni * M + nj;
				adj[u].push_back(v);
			}
		}
	}

	// 6) Sliding-window + BFS cu timestamp pentru vizitare
	vector<int> vis(NM, 0);
	int ts = 1;
	int best_area = 0;
	int r = 0;

	for (int l = 0; l < U; ++l)
	{
		// Extindem capătul drept al ferestrei cât vals[r] - vals[l] ≤ K
		while (r < U && vals[r] - vals[l] <= K)
			++r;

		// Numărul total de celule în această fereastră
		int total = prefix_cnt[r] - prefix_cnt[l];
		if (total <= best_area)
		{
			++ts; // resetăm marcajele
			continue;
		}

		++ts; // începem un nou timestamp pentru vizitare
		// Pentru fiecare prag din fereastra [l..r):
		for (int idx = l; idx < r; ++idx)
		{
			for (int start : pos_list[idx])
			{
				if (vis[start] == ts)
					continue;

				// BFS pe componenta din care face parte start
				int area = 0;
				queue<int> q;
				q.push(start);
				vis[start] = ts;

				while (!q.empty())
				{
					int u = q.front();
					q.pop();
					++area;
					for (int v : adj[u])
					{
						if (vis[v] == ts)
							continue;
						int vidx = idxMat[v];
						if (vidx < l || vidx >= r)
							continue;
						vis[v] = ts;
						q.push(v);
					}
				}

				best_area = max(best_area, area);
				if (best_area == total)
					break;
			}
			if (best_area == total)
				break;
		}
	}

	fout << best_area;
	return 0;
}
*/

int main()
{
	// 1) Citire input
	ifstream fin("p2.in");
	ofstream fout("p2.out");
	int N, M;
	long long K;
	fin >> N >> M >> K;
	int NM = N * M;

	vector<long long> a(NM), vals;
	vals.reserve(NM);
	for (int i = 0; i < NM; ++i)
	{
		fin >> a[i];
		vals.push_back(a[i]);
	}

	// 2) Sortare + eliminare duplicate
	sort(vals.begin(), vals.end());
	vals.erase(unique(vals.begin(), vals.end()), vals.end());
	int U = vals.size();

	// 3) Mapare valoare → index în vals[] și listă de poziții
	vector<int> idxMat(NM);
	vector<vector<int>> pos_list(U);
	for (int i = 0; i < NM; ++i)
	{
		int idx = int(lower_bound(vals.begin(), vals.end(), a[i]) - vals.begin());
		idxMat[i] = idx;
		pos_list[idx].push_back(i);
	}

	// 4) Prefix-sum peste pos_list pentru pruning
	vector<int> prefix(U + 1, 0);
	for (int i = 0; i < U; ++i)
		prefix[i + 1] = prefix[i] + int(pos_list[i].size());

	// 5) Sliding-window + BFS cu timestamp și coadă manuală
	vector<int> vis(NM, 0);
	int ts = 1;
	int best_area = 0;
	vector<int> q(NM);

	for (int l = 0, r = 0; l < U; ++l)
	{
		// extindem capătul drept cât vals[r] - vals[l] ≤ K
		while (r < U && vals[r] - vals[l] <= K)
			++r;

		int total_cells = prefix[r] - prefix[l];
		if (total_cells <= best_area)
		{
			++ts; // "reset" vizitare
			continue;
		}

		++ts; // nou timestamp pentru această fereastră

		// încercăm BFS din fiecare punct nou
		for (int idx = l; idx < r; ++idx)
		{
			for (int start : pos_list[idx])
			{
				if (vis[start] == ts)
					continue;
				// BFS pe componentă
				int head = 0, tail = 0, area = 0;
				q[tail++] = start;
				vis[start] = ts;

				while (head < tail)
				{
					int u = q[head++];
					++area;
					int x = u / M, y = u % M;
					// sus
					if (x > 0)
					{
						int v = u - M;
						if (vis[v] != ts)
						{
							int vidx = idxMat[v];
							if (vidx >= l && vidx < r)
							{
								vis[v] = ts;
								q[tail++] = v;
							}
						}
					}
					// jos
					if (x + 1 < N)
					{
						int v = u + M;
						if (vis[v] != ts)
						{
							int vidx = idxMat[v];
							if (vidx >= l && vidx < r)
							{
								vis[v] = ts;
								q[tail++] = v;
							}
						}
					}
					// stânga
					if (y > 0)
					{
						int v = u - 1;
						if (vis[v] != ts)
						{
							int vidx = idxMat[v];
							if (vidx >= l && vidx < r)
							{
								vis[v] = ts;
								q[tail++] = v;
							}
						}
					}
					// dreapta
					if (y + 1 < M)
					{
						int v = u + 1;
						if (vis[v] != ts)
						{
							int vidx = idxMat[v];
							if (vidx >= l && vidx < r)
							{
								vis[v] = ts;
								q[tail++] = v;
							}
						}
					}
				}

				best_area = max(best_area, area);
				if (best_area == total_cells)
					break;
			}
			if (best_area == total_cells)
				break;
		}
	}

	fout << best_area;
	return 0;
}
