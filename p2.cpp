#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>
#include <utility>
#include <iostream>
using namespace std;

int main()
{
	ifstream fin("p2.in");
	ofstream fout("p2.out");
	int N, M;
	long long K;
	fin >> N >> M >> K;
	int NM = N * M;

	vector<long long> a(NM);
	for (int i = 0; i < NM; i++)
		fin >> a[i];

	// 1) facem S = [(valoare, idx1D)] și sortăm după valoare
	vector<pair<long long, int>> S;
	S.reserve(NM);
	for (int i = 0; i < NM; i++)
		S.emplace_back(a[i], i);
	sort(S.begin(), S.end());

	// 2) allowed[i] = dacă celula i e în fereastra curentă de valori
	vector<char> allowed(NM, 0);
	// 3) vizitare cu timestamp și coadă
	vector<int> vis(NM, 0), q(NM);
	int ts = 1, best_area = 0;

	// 4) sliding window
	for (int l = 0, r = 0; l < NM; ++l)
	{
		// extindem r cât putem
		while (r < NM && S[r].first - S[l].first <= K)
		{
			allowed[S[r].second] = 1;
			++r;
		}

		int total_cells = r - l;
		if (total_cells <= best_area)
		{
			// scoatem elementul l din fereastră și mergem mai departe
			allowed[S[l].second] = 0;
			continue;
		}

		// avem șansa să găsim ceva mai bun
		++ts;
		// BFS doar din fiecare poziție nou intrată (S[l..r))
		for (int t = l; t < r && best_area < total_cells; ++t)
		{
			int start = S[t].second;
			if (!allowed[start] || vis[start] == ts)
				continue;

			// BFS
			int head = 0, tail = 0, area = 0;
			q[tail++] = start;
			vis[start] = ts;

			while (head < tail)
			{
				int u = q[head++];
				++area;
				int x = u / M, y = u % M;

				// cei patru vecini
				if (x > 0)
				{
					int v = u - M;
					if (allowed[v] && vis[v] != ts)
					{
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (x + 1 < N)
				{
					int v = u + M;
					if (allowed[v] && vis[v] != ts)
					{
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (y > 0)
				{
					int v = u - 1;
					if (allowed[v] && vis[v] != ts)
					{
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (y + 1 < M)
				{
					int v = u + 1;
					if (allowed[v] && vis[v] != ts)
					{
						vis[v] = ts;
						q[tail++] = v;
					}
				}
			}

			best_area = max(best_area, area);
		}

		// scoatem valoarea l înainte de a trece la l+1
		allowed[S[l].second] = 0;

		// dacă am găsit deja întreaga matrice, putem opri tot
		if (best_area == NM)
			break;
	}

	fout << best_area << "\n";
	return 0;
}
