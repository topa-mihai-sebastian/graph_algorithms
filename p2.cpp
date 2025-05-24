#include <vector>
#include <fstream>
#include <algorithm>
#include <utility>

using namespace std;

int main() {
	ifstream fin("p2.in");
	ofstream fout("p2.out");
	int N, M;
	long long K;
	fin >> N >> M >> K;
	int NM = N * M;

	// nu imi trecea ultimul test cu coada normala

	vector<long long> a(NM);
	for (int i = 0; i < NM; i++)
		fin >> a[i];

	// sort sorteaza mai intai dupa prima valoare din pereche
	// apoi dupa a doua daca primele sunt egale
	vector<pair<long long, int>> S;
	S.reserve(NM);
	for (int i = 0; i < NM; i++)
		S.emplace_back(a[i], i);
	sort(S.begin(), S.end());

	// allowed[i] = daca celula i e în fereastra curentă de valori
	vector<int> allowed(NM, 0);
	//  vizitare cu timestamp si coada
	vector<int> vis(NM, 0), q(NM);
	int ts = 1, best_area = 0;

	//  fereastra
	for (int l = 0, r = 0; l < NM; ++l) {
		// extindem r cat timp diferenta dintre min si max ramane <= K
		while (r < NM && S[r].first - S[l].first <= K) {
			allowed[S[r].second] = 1;
			++r;
		}
		// numarul de celule din fereastra
		int total_cells = r - l;
		if (total_cells <= best_area) {
			// scoatem elementul l din fereastra si
			// mergem mai departe pentru ca nu putem depasi
			// best_area
			allowed[S[l].second] = 0;
			continue;
		}

		// se poate gasi un numar mai mare
		++ts;
		// BFS doar din fiecare pozitie nou intrată (S[l..r))
		for (int t = l; t < r && best_area < total_cells; ++t) {
			int start = S[t].second;
			if (!allowed[start] || vis[start] == ts)
				continue;

			// BFS
			int head = 0, tail = 0, area = 0;
			q[tail++] = start;
			vis[start] = ts;

			while (head < tail) {
				// u este urmatorul nod din coada
				int u = q[head++];
				++area;
				int x = u / M, y = u % M;

				// cei patru vecini
				if (x > 0) {
					int v = u - M;
					if (allowed[v] && vis[v] != ts) {
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (x + 1 < N) {
					int v = u + M;
					if (allowed[v] && vis[v] != ts) {
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (y > 0) {
					int v = u - 1;
					if (allowed[v] && vis[v] != ts) {
						vis[v] = ts;
						q[tail++] = v;
					}
				}
				if (y + 1 < M) {
					int v = u + 1;
					if (allowed[v] && vis[v] != ts) {
						vis[v] = ts;
						q[tail++] = v;
					}
				}
			}

			best_area = max(best_area, area);
		}

		// scoatem valoarea l inainte de a trece la l+1
		allowed[S[l].second] = 0;

		// daca aria este fix cat aria totala se opreste
		if (best_area == NM)
			break;
	}
	fout << best_area;
	return 0;
}
