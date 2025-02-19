#include "gdwg_graph.h"

#include <iostream>
#include <string>

auto main() -> int {
	// These are commented out right now
	//  because withour your implementation
	//  it will not compile. Uncomment them
	//  once you've done the work
	/*auto g = gdwg::graph<std::string, int>{};
	g.insert_node("hello");
	g.insert_node("how");
	g.insert_node("are");
	g.insert_node("you?");

	g.insert_edge("hello", "how", 5);
	g.insert_edge("hello", "are", 8);
	g.insert_edge("hello", "are", 2);

	g.insert_edge("how", "you?", 1);
	g.insert_edge("how", "hello", 4);

	g.insert_edge("are", "you?", 3);

	std::cout << g << "\n";

	auto g2 = gdwg::graph<std::string, int>(g);

	std::cout << g2 << "\n";

	// This is a structured binding.
	// https://en.cppreference.com/w/cpp/language/structured_binding
	// It allows you to unpack your tuple.
	for (auto const& [from, to, weight] : g) {
    	std::cout << from << " -> " << to << " ";
    	if (weight.value()) {
        	std::cout << "(weight " << weight << ")\n";
    	} else {
        	std::cout << "(no weight)\n";
    	}
	}*/
}
