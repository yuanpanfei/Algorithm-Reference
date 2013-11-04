struct SBTNode {
	int k; // key
	int s; // size
	union {
		int c[2]; // children
		struct { int l, r; }; // left and right child
	};
	void new_node(int x) {
		k = x;
		s = 1;
		r = l = 0;
	}
}
SBTNode nodes[MAX_NODE_NUM];
int count;
int root;
void init() {
	root = 0;
	count = 0;
}
void rotate(int & x, bool r) {
	int y = nodes[x].child[!r];
	nodes[x].c[!r] = nodes[y].c[r];
	nodes[y].c[r] = x;
	nodes[y].s = nodes[x].s;
	nodes[x].s = nodes[nodes[x].l].s + 1 + nodes[nodes[x].r].s;
	x = y;
}
int find(int x, int k) {
	return x && nodes[x].k != k ? find(nodes[x].c[k>=nodes[x].k], k) : x;
}
void maintain(int & x, bool r) {
	if (nodes[nodes[nodes[x].c[r]].c[r]].s > nodes[nodes[x].c[!r]].s) {
		rotate(x, !r);
	} else if (nodes[nodes[nodes[x].c[r]].c[!r]].s > nodes[nodes[x].c[!r]].s) {
		rotate(nodes[x].c[r], r);
		rotate(x, !r);
	} else {
		return;
	}
	maintain(nodes[x].l, false);
	maintain(nodes[x].r, true);
	maintain(x, false);
	maintain(x, true);
}
void insert(int & t, int v) {
	if (t == 0) {
		t = ++count;
		nodes[t].new_node(v);
		return;
	}
	++nodes[t].s;
	if (v < nodes[t].k) {
		insert(nodes[t].l, v);
	} else {
		insert(nodes[t].r, v);
	}
	maintain(t, v>=nodes[t].k);
}
int erase(int & t, int v) {
	--nodes[t].s;
	if (v==nodes[t].k || (v<nodes[t].k&&!nodes[t].l) || (v>nodes[t].k&&!nodes[t].r)) {
		if (nodes[t].l && nodes[t].r) {
			int p = erase(nodes[t].l, v+1);
			nodes[t].k = nodes[p].k;
			return p;
		} else {
			int p = t;
			t = nodes[t].l + nodes[t].r;
			return p;
		}
	} else {
		return erase(v<nodes[t].k ? nodes[t].l : nodes[t].r, v);
	}
}
int pred(int t,int v) {
	if (!t) {
		return -1;
	} else if (v <= nodes[t].k) {
		return pred(nodes[t].l, v);
	} else {
		return max(nodes[t].k, pred(nodes[t].r, v));
	}
}
int succ(int t, int v) {
	if (!t) {
		return INF;
	} else if (v >= nodes[t].k) {
		return succ(nodes[t].r, v);
	} else {
		return min(nodes[t].k, succ(nodes[t].l, v));
	}
}
// return $k$-small element in the tree which root is $t$
int select(int t, int k) {
	if (k==nodes[nodes[t].l].s+1) {
		return nodes[t].k;
	} else if (k<=nodes[nodes[t].l].s) {
		return select(nodes[t].l, k);
	} else {
		return select(nodes[t].r, k-1-nodes[nodes[t].l].s);
	}
}
// return $x$ that $x$ is $k$-small element in the tree which root is $t$
int rank(int t, int k) {
	if (t==0) {
		return 1;
	} else if (k<=nodes[t].k) {
		return rank(nodes[t].l,k);
	} else {
		return nodes[nodes[t].l].s + 1 + rank(nodes[t].r, k);
	}
}
