#include "experiment6.hpp"
#include "hash.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdint>
#include <algorithm>
#include <limits>

const int ascii_min = 33;
const int ascii_max = 126;
const int seed = 67;

const int pair_num = 100000;
const std::vector<size_t> lengths = { 10, 100, 500, 1000 };
//po lygiai paskirstyti
const int pairs_per_len = pair_num / static_cast<int>(lengths.size());

std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
	std::vector<uint8_t> bytes;
	bytes.reserve(hex.size() / 2);
	for (size_t i = 0; i + 1 < hex.size(); i += 2) {
		uint8_t byte = static_cast<uint8_t>(std::stoi(hex.substr(i, 2), nullptr, 16));
		bytes.push_back(byte);
	}
	return bytes;
}

int popcount_byte(uint8_t x) {
	int count = 0;
	while (x) {
		count += x & 1;
		x >>= 1;
	}
	return count;
}

double bit_diff(const std::string& hex_a, const std::string& hex_b) {
	std::vector<uint8_t> bytes_a = hex_to_bytes(hex_a);
	std::vector<uint8_t> bytes_b = hex_to_bytes(hex_b);

	int diff_bits = 0;
	for (size_t i = 0; i < bytes_a.size(); ++i) {
		uint8_t x = bytes_a[i] ^ bytes_b[i];
		diff_bits += popcount_byte(x);
	}
	int total_bits = static_cast<int>(bytes_a.size()) * 8;
	return 100.0 * diff_bits / total_bits;
}

double hex_diff(const std::string& hex_a, const std::string& hex_b) {
	int diff = 0;
	for (size_t i = 0; i < hex_a.size(); ++i) {
		if (hex_a[i] != hex_b[i]) ++diff;
	}
	return 100.0 * diff / hex_a.size();
}

struct Stats {
	double min_val = std::numeric_limits<double>::max();
	double max_val = std::numeric_limits<double>::lowest();
	double sum = 0.0;
	long count = 0;

	void add(double v) {
		min_val = std::min(min_val, v);
		max_val = std::max(max_val, v);
		sum += v;
		++count;
	}

	double avg() const { return count > 0 ? sum / count : 0.0; }
};

void print_stats(const std::string& label, const Stats& s) {
	std::cout << "  " << label << ": min=" << s.min_val
		<< "%  max=" << s.max_val
		<< "%  vid=" << s.avg() << "%\n";
}

// ---------------------------------------------------------------------
// Ivesciu generavimas
// ---------------------------------------------------------------------

std::string generate_text(size_t len, std::mt19937& rand) {
	std::uniform_int_distribution<int> dist(ascii_min, ascii_max);
	std::string s;
	s.reserve(len);
	for (size_t i = 0; i < len; ++i) s += static_cast<char>(dist(rand));
	return s;
}

//pora: originalas ir su random pakeista ascii char
std::pair<std::string, std::string> gen_changed_pair(size_t len, std::mt19937& rand) {
	std::string original = generate_text(len, rand);
	std::string modified = original;

	std::uniform_int_distribution<size_t> index_dist(0, len - 1);
	std::uniform_int_distribution<int> char_dist(ascii_min, ascii_max);

	size_t idx = index_dist(rand);
	char new_char;
	do {
		new_char = static_cast<char>(char_dist(rand));
	} while (new_char == original[idx]); // garantuotai skirtingas simbolis

	modified[idx] = new_char;
	return { original, modified };
}

//pora: originalas ir su random apverstu bitu
std::pair<std::vector<uint8_t>, std::vector<uint8_t>> gen_flipped_pair(size_t len, std::mt19937& rand) {
	std::string text = generate_text(len, rand);
	std::vector<uint8_t> original(text.begin(), text.end());
	std::vector<uint8_t> modified = original;

	size_t total_bits = len * 8;
	std::uniform_int_distribution<size_t> bit_dist(0, total_bits - 1);
	size_t bit_pos = bit_dist(rand);

	size_t byte_idx = bit_pos / 8;
	size_t bit_in_byte = bit_pos % 8;
	modified[byte_idx] ^= static_cast<uint8_t>(1u << bit_in_byte);

	return { original, modified };
}

void print_histogram(const std::vector<double>& values, int bucket_count = 20) {
	std::vector<int> buckets(bucket_count, 0);
	double bucket_width = 100.0 / bucket_count;

	for (double v : values) {
		int idx = static_cast<int>(v / bucket_width);
		if (idx >= bucket_count) idx = bucket_count - 1; // 100.0% riba
		if (idx < 0) idx = 0;
		++buckets[idx];
	}

	int max_count = *std::max_element(buckets.begin(), buckets.end());
	const int max_bar_width = 50;

	std::cout << "bitu skirtumo histograma:\n";
	for (int i = 0; i < bucket_count; ++i) {
		double range_start = i * bucket_width;
		double range_end = (i + 1) * bucket_width;
		int bar_len = max_count > 0 ? (buckets[i] * max_bar_width / max_count) : 0;

		std::cout << "    " << range_start << "-" << range_end << "%: ";
		std::cout << std::string(bar_len, '*');
		std::cout << " (" << buckets[i] << ")\n";
	}
}

//simbolio pakeitimo testas
void run_char_change_test(std::mt19937& rand) {
	std::cout << "1 ascii char pakeitimas\n\n";

	Stats bits_stats, hex_stats;
	std::vector<double> histo_bit_stats;

	for (size_t len : lengths) {
		Stats bit_len, hex_len;

		for (int i = 0; i < pairs_per_len; ++i) {
			auto [a, b] = gen_changed_pair(len, rand);

			std::string hash_a = to_hex(hash_block(a));
			std::string hash_b = to_hex(hash_block(b));

			double bit_d = bit_diff(hash_a, hash_b);
			double hex_d = hex_diff(hash_a, hash_b);

			bit_len.add(bit_d);
			hex_len.add(hex_d);
			bits_stats.add(bit_d);
			hex_stats.add(hex_d);
			histo_bit_stats.push_back(bit_d);
		}

		std::cout << "ilgis: " << len << " baitu ir " << pairs_per_len << " poru\n";
		print_stats("bitu skirtumas", bit_len);
		print_stats("hex skirtumas", hex_len);
		std::cout << "\n";
	}

	std::cout << "bendri rezultatai, poru sk:" << pair_num << "\n";
	print_stats("bitu skirtumas", bits_stats);
	print_stats("hex skirtumas", hex_stats);

	print_histogram(histo_bit_stats);
	std::cout << "\n";
}

//1 bito apvertimas
void run_flipped_test(std::mt19937& rand) {
	std::cout << "1 random ivesties bito apvertimas\n\n";

	Stats bits_stats, hex_stats; //final result

	for (size_t len : lengths) {
		Stats bit_len, hex_len;

		for (int i = 0; i < pairs_per_len; ++i) {
			auto [a, b] = gen_flipped_pair(len, rand);

			std::string hash_a = to_hex(hash_block(a));
			std::string hash_b = to_hex(hash_block(b));

			double bit_d = bit_diff(hash_a, hash_b);
			double hex_d = hex_diff(hash_a, hash_b);

			bit_len.add(bit_d);
			hex_len.add(hex_d);
			bits_stats.add(bit_d);
			hex_stats.add(hex_d);
		}

		std::cout << "ilgis: " << len << " baitu\n";
		print_stats("bitu skirtumas", bit_len);
		print_stats("hex skirtumas", hex_len);
		std::cout << "\n";
	}

	std::cout << "rezultatai:\n";
	print_stats("bitu skirtumas", bits_stats);
	print_stats("hex skirtumas", hex_stats);
	std::cout << "\n\n";
}

void run_experiment6() {
	std::cout << "6 eksperimentas: lavinos efekto matavimas\n";

	std::mt19937 rand(seed);

	run_char_change_test(rand);
	run_flipped_test(rand);

	std::cout << "\n6 eksperimentas baigtas\n";
}