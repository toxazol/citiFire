#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
using namespace std;
#define pb push_back
#define mkp make_pair
#define pii pair<int,int>
#define fi first
#define se second
enum dir { col, row };

int n, m, k, mx, i, j;
vector<pii> ign;
vector<pii> seg;

int max(int a, int b) { return a > b ? a : b; }
int notLess(int a, int a0 = 0) { return a > a0 ? a : a0; }
int notMore(int a, int a1) { return a < a1 ? a : a1; }

struct square
{
	int top, bottom, left, right;
	void add(const pii dot)
	{
		if (dot.fi > top || top == -1)
			top = dot.fi;
		if (dot.fi < bottom || bottom == -1)
			bottom = dot.fi;
		if (dot.se > right || right == -1)
			right = dot.se;
		if (dot.se < left || left == -1)
			left = dot.se;
	}
	pii getCenter()
	{
		return mkp((bottom + top) / 2, (left + right) / 2);
	}

	int getRadius()
	{
		return max((top - bottom + 1) / 2, (right - left + 1) / 2);
	}

	square() :top(-1), bottom(-1), left(-1), right(-1) {}
};

bool checkSide(dir d, int line, pii fromTo, int t, square* sq)
{
	if (line < 0) return true;
	switch (d)
	{
	case col: if (line >= m) return true; break;
	case row: if (line >= n) return true; break;
	}
	seg.clear();
	for (auto other : ign)
	{
		if (d == col && abs(line - other.se) <= t)
		{
			seg.pb(mkp(other.fi - t, 1));
			seg.pb(mkp(other.fi + t, -1));
		}
		else if (d == row && abs(line - other.fi) <= t)
		{
			seg.pb(mkp(other.se - t, 1));
			seg.pb(mkp(other.se + t, -1));
		}
	}
	int from, to;
	from = fromTo.fi; to = fromTo.se;

	if (seg.size() == 0)
	{
		switch (d)
		{
		case col:
			sq->add(mkp(from, line));
			sq->add(mkp(to, line));
			break;
		case row:
			sq->add(mkp(line, from));
			sq->add(mkp(line, to));
			break;
		}
		return false;
	}
	sort(seg.begin(), seg.end(), [](pii a, pii b)
	{
		return a.fi < b.fi;
	});

	int sum = 1;
	if (seg.front().fi > from) // excessive
	{
		switch (d)
		{
		case col:
			sq->add(mkp(seg.front().fi - 1, line));
			sq->add(mkp(from, line));
			break;
		case row:
			sq->add(mkp(line, seg.front().fi - 1));
			sq->add(mkp(line, from));
			break;
		}
	}
	if (seg.back().fi < to) // excessive
	{
		switch (d)
		{
		case col:
			sq->add(mkp(seg.back().fi + 1, line));
			sq->add(mkp(to, line));
			break;
		case row:
			sq->add(mkp(line, seg.back().fi + 1));
			sq->add(mkp(line, to));
			break;
		}
	}
	for (int s = 1; s < seg.size(); ++s) // it can still be before 'to'
	{
		sum += seg[s].se;
		if (sum == 0)
		{
			if (s + 1 < seg.size() && seg[s + 1].fi - seg[s].fi <= 1)
				continue;
			else if (seg[s].fi < to)
			{
				switch (d)
				{
				case col:
					sq->add(mkp(seg[s].fi + 1, line));
					if (s + 1 < seg.size())
						sq->add(mkp(seg[s + 1].fi - 1, line));
					break;
				case row:
					sq->add(mkp(line, seg[s].fi + 1));
					if (s + 1 < seg.size())
						sq->add(mkp(line, seg[s + 1].fi - 1));
					break;
				}
			}
			else return true;
		}
	}
	return false;
}

bool burn(int t)
{
	auto sq = new square();
	pii fromToCol, fromToRow;
	for (i = 0; i < k; ++i)
	{
		fromToCol = mkp(notLess(ign[i].fi - t - 1), notMore(ign[i].fi + t + 1, n - 1));
		fromToRow = mkp(notLess(ign[i].se - t - 1), notMore(ign[i].se + t + 1, m - 1));
		checkSide(col, ign[i].se + t + 1, fromToCol, t, sq);
		checkSide(col, ign[i].se - t - 1, fromToCol, t, sq);
		checkSide(row, ign[i].fi + t + 1, fromToRow, t, sq);
		checkSide(row, ign[i].fi - t - 1, fromToRow, t, sq);
	}
	fromToCol = mkp(0, n-1);
	fromToRow = mkp(0, m-1);
	checkSide(col, 0, fromToCol, t, sq);
	checkSide(col, m-1, fromToCol, t, sq);
	checkSide(row, 0, fromToRow, t, sq);
	checkSide(row, n-1, fromToRow, t, sq);
	int sqRad = sq->getRadius();
	if (sqRad > 0)
		return sqRad <= t;
	return true;
}

int binBurn()
{
	int ans;
	for (int t1 = 0, t2 = mx - 1;;)
	{
		int mt = (t1 + t2) / 2;
		if (burn(mt)) t2 = mt;
		else t1 = mt;
		if (t2 - t1 == 1)
		{
			if (burn(t1))
				ans = t1;
			else if (burn(t2))
				ans = t2;
			else
				throw "binBurn failed!";
			break;
		}
	}
	return ans;
}

int main()
{
	cin >> n >> m >> k;
	mx = max(n, m);
	for (i = 0; i < k; ++i)
	{
		int y, x;
		cin >> y >> x;
		ign.pb(mkp(y - 1, x - 1));
	}
	cout << binBurn() << endl;
	return 0;
}