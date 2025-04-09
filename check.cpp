#include <bits/stdc++.h>

const int N = 10;
const int K = 5;
const int MAX_STEP = 5;

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
	std::vector<int> state_space;
	for (int state = 0; state < (1 << N); state++) {
		if (pop_count(state) == K) {
			state_space.push_back(state);
		}
	}
	std::function<int(int)> trans_radix = [](int x) {
		int res = 0;
		for (int base = K + 1, p = 1; x; x >>= 1, p *= base) {
			res += (x & 1) * p;
		}
		return res;
	};
	std::vector<int> check_states(N, 0), values(N, 0);
	std::function<bool(int)> dfs = [&](int index) {
		if (index >= N) {
			int all_check = 0;
			for (auto check_state : check_states) {
				all_check |= check_state;
			}
			if (all_check < (1 << MAX_STEP) - 1) {
				return false;
			}
			for (int i = 0; i < N; i++) {
				values[i] = trans_radix(check_states[i]);
			}
			std::set<int> check_results;
			for (auto state : state_space) {
				int check_result = 0;
				for (int i = 0; i < N; i++) {
					check_result += ((state >> i) & 1) * values[i];
				}
				if (check_results.find(check_result) != check_results.end()) {
					return false;
				}
				check_results.insert(check_result);
			}
			return true;
		}
		if (index > 0) {
			check_states[index] = check_states[index - 1];
		}
		for (; check_states[index] < (1 << MAX_STEP); check_states[index]++) {
			if (dfs(index + 1)) {
				return true;
			}
		}
		return false;
	};
	std::cout << dfs(0) << std::endl;
	return 0;
}