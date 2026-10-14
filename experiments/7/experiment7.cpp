#include "experiment7.hpp"
#include "hash.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <random>
#include <iomanip>

constexpr unsigned seed = 169;

std::vector<std::string> gen_candidates() {
	std::vector<std::string> candidates;
	candidates.reserve(10000);
	for (int i = 0; i < 10000; ++i) {
		std::ostringstream oss;
		oss << std::setw(4) << std::setfill('0') << i;
		candidates.push_back(oss.str());
	}
	return candidates;
}

//be druskos
void part1(const std::vector<std::string>& candidates, const std::string& target_input) {
	std::cout << "1 dalis: perrinkimas be salt\n";

	std::string target_hash = to_hex(hash_block(target_input));
	std::cout << "tiksline maisa: " << target_hash << "\n";

	auto start = std::chrono::steady_clock::now();

	long long attempts = 0;
	std::vector<std::string> matches;
	for (const auto& c : candidates) {
		++attempts;
		std::string c_hash = to_hex(hash_block(c));
		if (c_hash == target_hash) {
			matches.push_back(c);
		}
	}

	auto end = std::chrono::steady_clock::now();
	double seconds = std::chrono::duration<double>(end - start).count();

	std::cout << "bandymu sk.: " << attempts << "\n";
	std::cout << "laikas: " << seconds << "s\n";
	std::cout << "sutampanciu kandidatu sk.: " << matches.size() << "\n";
	for (const auto& m : matches) {
		std::cout << "\"" << m << "\"" << (m == target_input ? " - rasta, tai yra pradine ivestis" : "klaida") << "\n";
	}
	std::cout << "\n";
}

//viesa druska
void part2(const std::vector<std::string>& candidates, const std::string& target_input) {
	std::cout << "2 dalis: perrinkimas su viesa salt\n";

	//druska - 32 char hex
	std::string h_salt = salt(16);
	std::cout << "druska: " << h_salt << "\n";

	std::string target_hash = to_hex(hash_block_salted(target_input, h_salt));
	std::cout << "tiksline maisa su salt: " << target_hash << "\n\n";

	//is anksto apskaiciuotu maisu lentele be druskos (palyginimui)
	std::unordered_map<std::string, std::string> pre_nosalt;
	for (const auto& c : candidates) {
		std::string c_hash = to_hex(hash_block(c));
		pre_nosalt[c_hash] = c;
	}

	auto start = std::chrono::steady_clock::now();
	auto it = pre_nosalt.find(target_hash);
	auto end = std::chrono::steady_clock::now();
	double seconds = std::chrono::duration<double>(end - start).count();

	std::cout << "paieska is anksto apskaiciuotoje lenteleje:\n";
	if (it != pre_nosalt.end()){
		std::cout << "rastas kandidatas: " << it->second << "\n";
	}
	else {
		std::cout << "tiksline maisa nerasta\n";
	}
	std::cout << "paieskos laikas: " << seconds << "s\n\n";

	//su viesa druska

	start = std::chrono::steady_clock::now();
	long long attempts = 0;
	std::vector<std::string> matches;

	for (const auto& c : candidates) {
		++attempts;
		std::string c_hash = to_hex(hash_block_salted(c, h_salt));

		if (c_hash == target_hash) {
			matches.push_back(c);
		}
	}

	end = std::chrono::steady_clock::now();
	seconds = std::chrono::duration<double>(end - start).count();

	std::cout << "bandymu sk.: " << attempts << "\n";
	std::cout << "laikas: " << seconds << "s\n";
	std::cout << "sutampanciu kandidatu sk.: " << matches.size() << "\n\n";
}

//slaptas atsitiktinumas
void part3(const std::string& target_input) {
	std::cout << "3 dalis: slaptas atsitiktinumas\n";

	std::string r = salt(16);
	std::cout << "slaptas r: " << r << "\n\n";

	//maisa paskelbta, r slaptas (isipareigojimas)
	std::string hash_committed = to_hex(hash_block_salted(target_input, r));
	std::cout << "hash, kurio input zino tik siuntejas: " << hash_committed << "\n";

	std::string recomputed = to_hex(hash_block_salted(target_input, r));
	std::cout << "atskleista input: \"" << target_input << "\"\n";
	std::cout << "atskleistas r: " << r << "\n";
	std::cout << "perskaiciuota H(input || r): " << recomputed << "\n";
	std::cout << "paskelbtas commitment:        " << hash_committed << "\n";
	std::cout << "sutampa? " << (recomputed == hash_committed ? "sutampa, patikrinimas sekmingas" : "nesutampa,klaida") << "\n\n";
}

void run_experiment7() {
	std::cout << "7 eksperimentas: spejimas, viesa druska, slaptas atsitiktinumas\n";

	std::vector<std::string> candidates = gen_candidates();

	std::mt19937 rand(seed);
	std::uniform_int_distribution<int> target_dist(0, 9999);
	int target_index = target_dist(rand);
	std::string target_input = candidates[target_index];

	std::cout << "tiksline ivestis (slapta nuo atakos): " << target_input << "\n\n";

	part1(candidates, target_input);
	part2(candidates, target_input);
	part3(target_input);

	std::cout << "daugiau info README.md\n";

	std::cout << "\n7 eksperimentas baigtas\n";
}