#include <bits/stdc++.h>

const int N = 10;
const int K = 5;

int pop_count(int x) {
	int cnt = 0;
	for (; x; x = x & (x - 1), cnt += 1);
	return cnt;
}

void binary_output(int x) {
	for (int i = N - 1; i >= 0; i--) {
		putchar('0' + ((x >> i) & 1));
	}
	putchar('\n');
}

int main() {
	std::function<std::tuple<int, int, double>(std::vector<int>)> dfs = [&](std::vector<int> state_space) {
		if (state_space.size() == 1) {
			return std::make_tuple(0, 0, 0.0);
		}
		double maxq = 0;
		int best_mask = 0;
		for (int mask = 1; mask < (1 << N); mask++) {
			std::vector<int> cnt(N + 1);
			for (auto state : state_space) {
				cnt[pop_count(state & mask)]++;
			}
			double q = 0;
			for (int i = 0; i <= N; i++) {
				if (cnt[i] == 0) {
					continue;
				}
				double p = 1.0 * cnt[i] / state_space.size();
				q -= p * log2(p);
			}
			if (q > maxq) {
				maxq = q;
				best_mask = mask;
			}
		}
		std::vector<std::vector<int>> sub_state_space(N + 1);
		for (auto state : state_space) {
			sub_state_space[pop_count(state & best_mask)].push_back(state);
		}
		int step = 0;
		for (int i = 0; i <= N; i++) {
			if (sub_state_space[i].empty() || sub_state_space[i].size() == state_space.size()) {
				continue;
			}
			auto res = dfs(sub_state_space[i]);
			step = std::max(step, std::get<0>(res) + 1);
		}
		return std::make_tuple(step, best_mask, maxq);
	};
	std::vector<int> state_space;
	for (int state = 0; state < (1 << N); state++) {
		if (pop_count(state) == K) {
			state_space.push_back(state);
		}
	}

	int sum;
	while (state_space.size() > 1) {
		auto ans = dfs(state_space);
		int step = std::get<0>(ans);
		int mask = std::get<1>(ans);
		double q = std::get<2>(ans);
		std::cout << "最坏情况下还需步数(worst remain step): " << step << std::endl;
		std::cout << "当前信息量(current amount of information): " << log2(1.0 * state_space.size()) << std::endl;
		std::cout << "该次称量的信息熵(information entropy): " << q << std::endl;
		binary_output(mask);

		std::cin >> sum;
		std::vector<int> sub_state_space;
		for (auto state : state_space) {
			if (pop_count(state & mask) == sum) {
				sub_state_space.push_back(state);
			}
		}
		state_space = sub_state_space;
	}
	binary_output(state_space[0]);
	return 0;
}