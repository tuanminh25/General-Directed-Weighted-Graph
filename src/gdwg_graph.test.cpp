#include "gdwg_graph.h"

#include <catch2/catch.hpp>

TEST_CASE("Default construction of graph") {
    SECTION("Create a graph object") {
        // Create an instance of graph
        gdwg::graph<int, int> g;
        REQUIRE(g.empty()); // Check if the graph is empty
    }
}

TEST_CASE("Constructor with initializer_list initializes graph with nodes && cannot have identical node") {
    SECTION("Graph initialized with a list of nodes") {
        // Initialize the graph with a list of nodes
        std::initializer_list<int> nodes = {1, 2, 3, 4, 5, 5};
        gdwg::graph<int, int> g(nodes);
        REQUIRE(!g.empty()); // Check if the graph is empty
        REQUIRE(g.nodes().size() == 5); // Ensure nodes
    }

}

TEST_CASE("Constructor graph(InputIt first, InputIt last) && cannot have identical node") {
	// Initialize the graph with a list of nodes
	std::initializer_list<int> nodes = {1, 2, 3, 4, 5, 5};
	gdwg::graph<int, int> g(nodes.begin(), nodes.end());
	REQUIRE(!g.empty()); // Check if the graph is empty
	REQUIRE(g.nodes().size() == 5); // Ensure nodes
}

TEST_CASE("Move constructor tests") {
    SECTION("Move constructor transfers ownership and leaves other empty") {
        // Initialize graph with some nodes and edges
        std::initializer_list<int> nodes = {1, 2, 3, 4, 5};
        gdwg::graph<int, int> g1(nodes);
        
        // Move construct g2 from g1
        gdwg::graph<int, int> g2(std::move(g1));
        
        // Test that g1 is empty after the move
        REQUIRE(g1.empty());
        
        // Test that g2 has the nodes and edges originally in g1
        REQUIRE_FALSE(g2.empty());
        REQUIRE(g2.nodes().size() == nodes.size());

    }
}

TEST_CASE("Move assignment operator tests") {
    SECTION("Move assignment transfers ownership and leaves other empty") {
        // Initialize graph with some nodes and edges
        std::initializer_list<int> nodes = {1, 2, 3, 4, 5};
        gdwg::graph<int, int> g1(nodes);
        
        // Create another graph with different nodes
        std::initializer_list<int> nodes2 = {6, 7, 8};
        gdwg::graph<int, int> g2(nodes2);

        // Move assign g2 to g1
        g1 = std::move(g2);

        // Test that g2 is empty after the move assignment
        REQUIRE(g2.empty());
        
        // Test that g1 has the nodes and edges originally in g2
        REQUIRE_FALSE(g1.empty());
        REQUIRE(g1.nodes().size() == nodes2.size());
    }
}

TEST_CASE("Copy constructor tests") {
    SECTION("Copy constructor duplicates nodes and edges") {
        // Initialize graph g1 with some nodes and edges
        std::initializer_list<int> nodes1 = {1, 2, 3, 4, 5};
		// Passing const to match with constructor
        const gdwg::graph<int, int> g1(nodes1);
        
        // Copy construct g2 from g1
        gdwg::graph<int, int> g2(g1);
        
        // Test that g1 and g2 are equal
        REQUIRE(g1.nodes() == g2.nodes());

        // Ensure that g1 and g2 are indeed equal in terms of their internal state
        REQUIRE(g1.empty() == g2.empty());
    }

	SECTION("Copy constructor duplicates nodes and edges even with non-const input") {
        // Initialize graph g1 with some nodes and edges
        std::initializer_list<int> nodes1 = {1, 2, 3, 4, 5};
        gdwg::graph<int, int> g1(nodes1);
        
        // Copy construct g2 from g1
        gdwg::graph<int, int> g2(g1);
        
        // Test that g1 and g2 are equal
        REQUIRE(g1.nodes() == g2.nodes());

        // Ensure that g1 and g2 are indeed equal in terms of their internal state
        REQUIRE(g1.empty() == g2.empty());
    }
}

TEST_CASE("Copy assignment operator tests") {
    SECTION("Copy assignment duplicates nodes and edges") {
        // Initialize graph g1 with some nodes and edges
        std::initializer_list<int> nodes1 = {1, 2, 3, 4, 5};
        gdwg::graph<int, int> g1(nodes1);
        
        // Initialize graph g2 with some other nodes and edges
        std::initializer_list<int> nodes2 = {6, 7, 8};
        gdwg::graph<int, int> g2(nodes2);

        // Copy assign g2 to g1
        g1 = g2;
        
        // Test that g1 and g2 are equal after the assignment
        REQUIRE(g1.nodes() == g2.nodes());

        // Ensure that g1 and g2 are indeed equal in terms of their internal state
        REQUIRE(g1.empty() == g2.empty());
    }
}

TEST_CASE("Edge print_edge function") {
    SECTION("Weighted edge string representation") {
        gdwg::weighted_edge<int, int> e1(1, 2, 10);
        REQUIRE(e1.print_edge() == "1 -> 2 | W | 10");
    }

    SECTION("Unweighted edge string representation") {
        gdwg::unweighted_edge<int, int> e2(3, 4);
        REQUIRE(e2.print_edge() == "3 -> 4 | U");
    }

    SECTION("Edge with different types") {
        gdwg::weighted_edge<std::string, double> e3("A", "B", 2.5);
        REQUIRE(e3.print_edge() == "A -> B | W | 2.5");

        gdwg::unweighted_edge<std::string, double> e4("C", "D");
        REQUIRE(e4.print_edge() == "C -> D | U");
    }
}

TEST_CASE("Edge is_weighted function") {
    SECTION("Weighted edge identification") {
        gdwg::weighted_edge<int, int> e1(1, 2, 10);
        REQUIRE(e1.is_weighted() == true);
    }

    SECTION("Unweighted edge identification") {
        gdwg::unweighted_edge<int, int> e2(3, 4);
        REQUIRE(e2.is_weighted() == false);
    }

    SECTION("Edge with different types") {
        gdwg::weighted_edge<std::string, double> e3("A", "B", 2.5);
        REQUIRE(e3.is_weighted() == true);

        gdwg::unweighted_edge<std::string, double> e4("C", "D");
        REQUIRE(e4.is_weighted() == false);
    }
}

TEST_CASE("Edge get_weight function") {
    SECTION("Weighted edge returns weight") {
        gdwg::weighted_edge<int, int> e1(1, 2, 10);
        REQUIRE(e1.get_weight().value() == 10);
    }

    SECTION("Unweighted edge returns std::nullopt") {
        gdwg::unweighted_edge<int, int> e2(3, 4);
        REQUIRE(e2.get_weight() == std::nullopt);
    }

    SECTION("Weighted edge with different types returns weight") {
        gdwg::weighted_edge<std::string, double> e3("A", "B", 2.5);
        REQUIRE(e3.get_weight().value() == 2.5);
    }

    SECTION("Unweighted edge with different types returns std::nullopt") {
        gdwg::unweighted_edge<std::string, double> e4("C", "D");
        REQUIRE(e4.get_weight() == std::nullopt);
    }
}

TEST_CASE("Edge get_nodes function") {
    SECTION("Weighted edge returns correct source and destination nodes") {
        gdwg::weighted_edge<int, int> e1(1, 2, 10);
        auto nodes = e1.get_nodes();
        REQUIRE(nodes.first == 1);
        REQUIRE(nodes.second == 2);
    }

    SECTION("Unweighted edge returns correct source and destination nodes") {
        gdwg::unweighted_edge<int, int> e2(3, 4);
        auto nodes = e2.get_nodes();
        REQUIRE(nodes.first == 3);
        REQUIRE(nodes.second == 4);
    }

    SECTION("Weighted edge with different types returns correct source and destination nodes") {
        gdwg::weighted_edge<std::string, double> e3("A", "B", 2.5);
        auto nodes = e3.get_nodes();
        REQUIRE(nodes.first == "A");
        REQUIRE(nodes.second == "B");
    }

    SECTION("Unweighted edge with different types returns correct source and destination nodes") {
        gdwg::unweighted_edge<std::string, double> e4("C", "D");
        auto nodes = e4.get_nodes();
        REQUIRE(nodes.first == "C");
        REQUIRE(nodes.second == "D");
    }
}

TEST_CASE("Insert node into graph") {
    gdwg::graph<int, int> g;

    SECTION("Inserting a new node") {
        REQUIRE(g.insert_node(1) == true);  // Node should be added
        REQUIRE(g.insert_node(1) == false); // Node should not be added again
        REQUIRE(g.insert_node(2) == true);  // Another node should be added
        REQUIRE(g.insert_node(3) == true);  // Another node should be added
    }

    SECTION("Inserting duplicate nodes") {
        g.insert_node(1);
        REQUIRE(g.insert_node(1) == false); // Duplicate node should not be added
    }

	SECTION("Working with lvalue") {
		int node1 = 1;
		int node2 = 2;
        REQUIRE(g.insert_node(node1) == true);  // Node should be added
        REQUIRE(g.insert_node(node1) == false); // Node should not be added again
        REQUIRE(g.insert_node(node2) == true);  // Another node should be added
    }
}

#include <catch2/catch.hpp>

TEST_CASE("Testing edge equality operator") {
    using N = int;
    using E = int;

    // Create weighted and unweighted edges
    gdwg::weighted_edge<N, E> edge1(1, 2, 10); // Example weighted edge with src=1, dst=2, weight=10
    gdwg::weighted_edge<N, E> edge2(1, 2, 10); // Same as edge1
    gdwg::weighted_edge<N, E> edge3(2, 1, 10); // Different src and dst
    gdwg::weighted_edge<N, E> edge4(1, 2, 20); // Same src and dst, different weight

    gdwg::unweighted_edge<N, E> edge5(1, 2); // Unweighted edge with src=1, dst=2
    gdwg::unweighted_edge<N, E> edge6(1, 2); // Same as edge5
    gdwg::unweighted_edge<N, E> edge7(2, 1); // Different src and dst

    // Test equality with same weighted edges
    REQUIRE(edge1 == edge2); // Expect true
    // Test equality with different src in weighted edge
    REQUIRE(!(edge1 == edge3)); // Expect false
    // Test equality with different dst in weighted edge
    REQUIRE(!(edge1 == edge3)); // Expect false
    // Test equality with different weight in weighted edge
    REQUIRE(!(edge1 == edge4)); // Expect false
    // Test equality with same unweighted edges
    REQUIRE(edge5 == edge6); // Expect true
    // Test equality with different src in unweighted edge
    REQUIRE(!(edge5 == edge7)); // Expect false
    // Test equality between weighted and unweighted edges
    REQUIRE(!(edge1 == edge5)); // Expect false
}

TEST_CASE("Insert edge tests") {
    gdwg::graph<float, int> g;

    SECTION("Insert unweighted edge") {
        g.insert_node(1);
        g.insert_node(2);
        REQUIRE(g.insert_edge(1, 2, std::nullopt) == true);
    }

    SECTION("Insert weighted edge") {
        g.insert_node(1);
        g.insert_node(2);
        REQUIRE(g.insert_edge(1, 2, 10) == true);
    }

    SECTION("Insert duplicate unweighted edge") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_edge(1, 2, std::nullopt); // First insertion
        REQUIRE(g.insert_edge(1, 2, std::nullopt) == false); // Duplicate insertion
    }

    SECTION("Insert duplicate weighted edge with same weight") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_edge(1, 2, 10); // First insertion
        REQUIRE(g.insert_edge(1, 2, 10) == false); // Duplicate insertion with same weight
    }

    SECTION("Insert edge with different weight to same src, dst") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_edge(1, 2, 10); // First insertion
        REQUIRE(g.insert_edge(1, 2, 20) == true); // New edge with different weight
    }

    SECTION("Insert edge with different weight to same src (reflexive)") {
        g.insert_node(1);
        REQUIRE(g.insert_edge(1, 1, 20) == true); 
        REQUIRE(g.insert_edge(1, 1, 20) == false); 
    }

    SECTION("Insert edge with non-existent node") {
        g.insert_node(1);
        REQUIRE_THROWS_AS(g.insert_edge(1, 2, std::nullopt), std::runtime_error);
    }
}

TEST_CASE("Replace node tests") {
    // Create a graph with integer nodes and integer edge weights
    gdwg::graph<int, int> g;

    // Add some nodes to the graph
    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_node(4);

    // Add some edges to the graph
    g.insert_edge(1, 2, 10);
    g.insert_edge(1, 3, 20);
    g.insert_edge(2, 3, 30);
    g.insert_edge(4, 1, 40);
    g.insert_edge(1, 1, 45);

    SECTION("Replace existing node successfully") {
        REQUIRE(g.replace_node(1, 5) == true);
        REQUIRE(g.is_node(5) == true);
        REQUIRE(g.is_node(1) == false);
		REQUIRE_THROWS_WITH(g.replace_node(1, 5),
					"Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
    }

    SECTION("Replace node with a value that already exists") {
        REQUIRE(g.replace_node(2, 3) == false);
        REQUIRE(g.is_node(2) == true);
        REQUIRE(g.is_node(3) == true);
    }

    SECTION("Replace non-existing node") {
        REQUIRE_THROWS_WITH(g.replace_node(6, 7),
                            "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
    }

    SECTION("Replace node with the same value") {
        REQUIRE(g.replace_node(3, 3) == false);
        REQUIRE(g.is_node(3) == true);
    }

	SECTION("Edges redirrected correctly") {
        REQUIRE(g.replace_node(1, 5) == true);
        REQUIRE(g.is_node(5) == true);
        REQUIRE(g.is_node(1) == false);
        REQUIRE_THROWS_WITH(g.replace_node(1, 5),
                            "Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
        // Check edges from 5 (previously from 1)
        auto edges_from_5 = g.edges(5, 3);
        REQUIRE(edges_from_5.size() == 1);
        REQUIRE(edges_from_5[0]->get_weight() == 20);

        edges_from_5 = g.edges(5, 2);
        REQUIRE(edges_from_5.size() == 1);
        REQUIRE(edges_from_5[0]->get_weight() == 10);

        auto edges_to_5 = g.edges(4, 5);
        REQUIRE(edges_to_5.size() == 1);
        REQUIRE(edges_to_5[0]->get_weight() == 40);
        REQUIRE(edges_to_5[0]->get_weight() == 40);

        auto edges_from_to_5 = g.edges(5, 5);
        REQUIRE(edges_from_to_5.size() == 1);
        REQUIRE(edges_from_to_5[0]->get_weight() == 45);

    }
}

TEST_CASE("merge_replace_node complex scenario") {
    gdwg::graph<std::string, int> g;

    // Insert nodes
    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));

    // Insert edges for A
    REQUIRE(g.insert_edge("A", "A", std::nullopt));
    REQUIRE(g.insert_edge("A", "A", 10));
    REQUIRE(g.insert_edge("A", "B", 2));
    REQUIRE(g.insert_edge("A", "B", std::nullopt));
    REQUIRE(g.insert_edge("A", "C", 3));

    // Insert edges for B
    REQUIRE(g.insert_edge("B", "B", std::nullopt));
    REQUIRE(g.insert_edge("B", "B", 9));
    REQUIRE(g.insert_edge("B", "A", 10));
    REQUIRE(g.insert_edge("B", "A", 1));
    REQUIRE(g.insert_edge("B", "C", 5));

    // Insert edges for C
    REQUIRE(g.insert_edge("C", "C", std::nullopt));
    REQUIRE(g.insert_edge("C", "C", 11));
    REQUIRE(g.insert_edge("C", "A", 4));
    REQUIRE(g.insert_edge("C", "B", 6));

    // Perform merge_replace
    g.merge_replace_node("B", "A");

    // Check results
    SECTION("Node B should not exist") {
        REQUIRE_FALSE(g.is_node("B"));
    }

    SECTION("Check edges from A to A") {
        auto edges = g.edges("A", "A");
        REQUIRE(edges.size() == 5);
        REQUIRE(edges[0]->get_weight() == std::nullopt);
        REQUIRE(edges[1]->get_weight().value() == 1);
        REQUIRE(edges[2]->get_weight().value() == 2);
        REQUIRE(edges[3]->get_weight().value() == 9);
        REQUIRE(edges[4]->get_weight().value() == 10);
    }

    SECTION("Check edges from A to C") {
        auto edges = g.edges("A", "C");
        REQUIRE(edges.size() == 2);
        REQUIRE(edges[0]->get_weight().value() == 3);
        REQUIRE(edges[1]->get_weight().value() == 5);
    }

    SECTION("Check edges from C to A") {
        auto edges = g.edges("C", "A");
        REQUIRE(edges.size() == 2);
        REQUIRE(edges[0]->get_weight().value() == 4);
        REQUIRE(edges[1]->get_weight().value() == 6);
    }

    SECTION("Check edges from C to C") {
        auto edges = g.edges("C", "C");
        REQUIRE(edges.size() == 2);
        REQUIRE(edges[0]->get_weight() == std::nullopt);
        REQUIRE(edges[1]->get_weight().value() == 11);
    }

    SECTION("Node replacing itself should not change the graph") {
        // Store the current state of the graph
        auto nodes_before = g.nodes();
        auto edges_A_A_before = g.edges("A", "A");
        auto edges_A_C_before = g.edges("A", "C");
        auto edges_C_A_before = g.edges("C", "A");

        // Perform merge_replace of A with itself
        g.merge_replace_node("A", "A");

        // Check that nothing has changed
        REQUIRE(g.nodes() == nodes_before);
        REQUIRE(g.edges("A", "A").size() == edges_A_A_before.size());
        REQUIRE(g.edges("A", "C").size() == edges_A_C_before.size());
        REQUIRE(g.edges("C", "A").size() == edges_C_A_before.size());

        // Double-check specific edges
        auto edges_A_A = g.edges("A", "A");
        REQUIRE(edges_A_A.size() == 5);
        REQUIRE(edges_A_A[0]->get_weight() == std::nullopt);
        REQUIRE(edges_A_A[1]->get_weight().value() == 1);
        REQUIRE(edges_A_A[2]->get_weight().value() == 2);
        REQUIRE(edges_A_A[3]->get_weight().value() == 9);
        REQUIRE(edges_A_A[4]->get_weight().value() == 10);

        auto edges_A_C = g.edges("A", "C");
        REQUIRE(edges_A_C.size() == 2);
        REQUIRE(edges_A_C[0]->get_weight().value() == 3);
        REQUIRE(edges_A_C[1]->get_weight().value() == 5);

        auto edges_C_A = g.edges("C", "A");
        REQUIRE(edges_C_A.size() == 2);
        REQUIRE(edges_C_A[0]->get_weight().value() == 4);
        REQUIRE(edges_C_A[1]->get_weight().value() == 6);
    }
}

TEST_CASE("Edges function tests 1 - type int") {
    gdwg::graph<int, int> g;

    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_node(4);

    g.insert_edge(1, 2, 5);
    g.insert_edge(1, 2, 10);
    g.insert_edge(2, 3, std::nullopt);
    g.insert_edge(2, 3, 15);

    SECTION("Retrieve edges between existing nodes - checking order") {
        auto edges_1_2 = g.edges(1, 2);
        REQUIRE(edges_1_2.size() == 2);
        REQUIRE(edges_1_2[0]->get_weight() == 5);
        REQUIRE(edges_1_2[1]->get_weight() == 10);
        
        auto edges_2_3 = g.edges(2, 3);
        REQUIRE(edges_2_3.size() == 2);
        REQUIRE(edges_2_3[0]->is_weighted() == false);
        REQUIRE(edges_2_3[1]->get_weight() == 15);
    }

    SECTION("Retrieve edges between nodes with no edges") {
        auto edges_3_4 = g.edges(3, 4);
        REQUIRE(edges_3_4.empty());
    }

    SECTION("Throw exception when src node doesn't exist") {
        REQUIRE_THROWS_WITH(g.edges(5, 2), "Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph");
    }

    SECTION("Throw exception when dst node doesn't exist") {
        REQUIRE_THROWS_WITH(g.edges(1, 5), "Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph");
    }

    SECTION("Throw exception when both src and dst nodes don't exist") {
        REQUIRE_THROWS_WITH(g.edges(5, 6), "Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph");
    }

    SECTION("Retrieve edges when src and dst are the same node - multiple edges") {
        g.insert_edge(1, 1, 20);
        g.insert_edge(1, 1);
        g.insert_edge(1, 1, 30);
        g.insert_edge(1, 1, 5);
        g.insert_edge(1, 1, 50);
        g.insert_edge(1, 1, 49);
        g.insert_edge(1, 1, 48);
        g.insert_edge(1, 1, 10);
        g.insert_edge(2, 1, 10);
        g.insert_edge(3, 1, 10);
        auto edges_1_1 = g.edges(1, 1);
        REQUIRE(edges_1_1[0]->get_weight() == std::nullopt);
        REQUIRE(edges_1_1[1]->get_weight() == 5);
    }
}

TEST_CASE("Edges function tests 2 - multiple type") {
    SECTION("Retrieve edges between existing nodes - checking order") {
        gdwg::graph<std::string, float> g;

        g.insert_node("A");
        g.insert_node("B");
        g.insert_node("C");
        g.insert_node("D");

        // Edges for A
        REQUIRE(g.insert_edge("A", "A", 10.0f));
        REQUIRE(g.insert_edge("A", "A"));
        REQUIRE(g.insert_edge("A", "B", 10.0f));
        REQUIRE(g.insert_edge("A", "B"));
        REQUIRE(g.insert_edge("A", "C", 20.0f));
        REQUIRE(g.insert_edge("A", "C"));

        // Edges for B
        REQUIRE(g.insert_edge("B", "B", 15.0f));
        REQUIRE(g.insert_edge("B", "B"));
        REQUIRE(g.insert_edge("B", "A", 15.0f));
        REQUIRE(g.insert_edge("B", "A"));
        REQUIRE(g.insert_edge("B", "C", 25.0f));
        REQUIRE(g.insert_edge("B", "C"));

        // Edges for C
        REQUIRE(g.insert_edge("C", "C", 30.0f));
        REQUIRE(g.insert_edge("C", "C"));
        REQUIRE(g.insert_edge("C", "A", 30.0f));
        REQUIRE(g.insert_edge("C", "A"));
        REQUIRE(g.insert_edge("C", "B", 35.0f));
        REQUIRE(g.insert_edge("C", "B"));

        // Attempting to insert identical edges (should return false)
        REQUIRE_FALSE(g.insert_edge("A", "A", 10.0f));
        REQUIRE_FALSE(g.insert_edge("A", "A"));
        REQUIRE_FALSE(g.insert_edge("A", "B", 10.0f));
        REQUIRE_FALSE(g.insert_edge("B", "C", 25.0f));

        // Check edges for A
        auto edges_A_A = g.edges("A", "A");
        REQUIRE(edges_A_A.size() == 2);
        REQUIRE(edges_A_A[0]->get_weight() == std::nullopt);
        REQUIRE(edges_A_A[1]->get_weight() == 10.0f);

        auto edges_A_B = g.edges("A", "B");
        REQUIRE(edges_A_B.size() == 2);
        REQUIRE(edges_A_B[0]->get_weight() == std::nullopt);
        REQUIRE(edges_A_B[1]->get_weight() == 10.0f);

        auto edges_A_C = g.edges("A", "C");
        REQUIRE(edges_A_C.size() == 2);
        REQUIRE(edges_A_C[0]->get_weight() == std::nullopt);
        REQUIRE(edges_A_C[1]->get_weight() == 20.0f);

        // Check edges for B
        auto edges_B_B = g.edges("B", "B");
        REQUIRE(edges_B_B.size() == 2);
        REQUIRE(edges_B_B[0]->get_weight() == std::nullopt);
        REQUIRE(edges_B_B[1]->get_weight() == 15.0f);

        auto edges_B_A = g.edges("B", "A");
        REQUIRE(edges_B_A.size() == 2);
        REQUIRE(edges_B_A[0]->get_weight() == std::nullopt);
        REQUIRE(edges_B_A[1]->get_weight() == 15.0f);

        auto edges_B_C = g.edges("B", "C");
        REQUIRE(edges_B_C.size() == 2);
        REQUIRE(edges_B_C[0]->get_weight() == std::nullopt);
        REQUIRE(edges_B_C[1]->get_weight() == 25.0f);

        // Check edges for C
        auto edges_C_C = g.edges("C", "C");
        REQUIRE(edges_C_C.size() == 2);
        REQUIRE(edges_C_C[0]->get_weight() == std::nullopt);
        REQUIRE(edges_C_C[1]->get_weight() == 30.0f);

        auto edges_C_A = g.edges("C", "A");
        REQUIRE(edges_C_A.size() == 2);
        REQUIRE(edges_C_A[0]->get_weight() == std::nullopt);
        REQUIRE(edges_C_A[1]->get_weight() == 30.0f);

        auto edges_C_B = g.edges("C", "B");
        REQUIRE(edges_C_B.size() == 2);
        REQUIRE(edges_C_B[0]->get_weight() == std::nullopt);
        REQUIRE(edges_C_B[1]->get_weight() == 35.0f);

        // Check that there are no edges to D
        REQUIRE(g.edges("A", "D").empty());
        REQUIRE(g.edges("B", "D").empty());
        REQUIRE(g.edges("C", "D").empty());
    }
}

TEST_CASE("is_connected functionality") {
    gdwg::graph<std::string, int> g;

    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));
    REQUIRE(g.insert_node("D"));

    REQUIRE(g.insert_edge("A", "B", 1));
    REQUIRE(g.insert_edge("A", "C", 2));
    REQUIRE(g.insert_edge("B", "D", 3));

    SECTION("Positive connections") {
        REQUIRE(g.is_connected("A", "B"));
        REQUIRE(g.is_connected("A", "C"));
        REQUIRE(g.is_connected("B", "D"));
    }

    SECTION("Negative connections") {
        REQUIRE_FALSE(g.is_connected("A", "D"));
        REQUIRE_FALSE(g.is_connected("B", "A"));
        REQUIRE_FALSE(g.is_connected("C", "B"));
        REQUIRE_FALSE(g.is_connected("D", "A"));
    }

    SECTION("Self connections") {
        REQUIRE(g.insert_edge("A", "A", 0));
        REQUIRE(g.is_connected("A", "A"));
        REQUIRE_FALSE(g.is_connected("B", "B"));
    }

    SECTION("Exception handling") {
        REQUIRE_THROWS_WITH(g.is_connected("E", "A"),
            "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
        REQUIRE_THROWS_WITH(g.is_connected("A", "E"),
            "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
        REQUIRE_THROWS_WITH(g.is_connected("E", "F"),
            "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
    }
}


TEST_CASE("connections functionality") {
    gdwg::graph<std::string, int> g;

    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));
    REQUIRE(g.insert_node("D"));
    REQUIRE(g.insert_node("E"));

    REQUIRE(g.insert_edge("A", "B", 1));
    REQUIRE(g.insert_edge("A", "C", 2));
    REQUIRE(g.insert_edge("A", "D", 3));
    REQUIRE(g.insert_edge("A", "B", 4));  // Duplicate destination
    REQUIRE(g.insert_edge("B", "C", 5));
    REQUIRE(g.insert_edge("D", "E", 6));
    REQUIRE(g.insert_edge("A", "A", 7));  // Self-loop

    SECTION("Connections from node with multiple outgoing edges") {
        auto connections_A = g.connections("A");
        REQUIRE(connections_A.size() == 4);
        REQUIRE(connections_A[0] == "A");
        REQUIRE(connections_A[1] == "B");
        REQUIRE(connections_A[2] == "C");
        REQUIRE(connections_A[3] == "D");
    }

    SECTION("Connections from node with single outgoing edge") {
        auto connections_B = g.connections("B");
        REQUIRE(connections_B.size() == 1);
        REQUIRE(connections_B[0] == "C");
    }

    SECTION("Connections from node with no outgoing edges") {
        auto connections_E = g.connections("E");
        REQUIRE(connections_E.empty());
    }

    SECTION("Connections are sorted") {
        REQUIRE(g.insert_edge("A", "E", 8));
        auto connections_A = g.connections("A");
        REQUIRE(connections_A.size() == 5);
        REQUIRE(std::is_sorted(connections_A.begin(), connections_A.end()));
    }

    SECTION("Connections return copies") {
        auto connections_A = g.connections("A");
        connections_A[0] = "Z";  // This shouldn't affect the graph
        auto new_connections_A = g.connections("A");
        REQUIRE(new_connections_A[0] != "Z");
    }

    SECTION("Exception for non-existent node") {
        REQUIRE_THROWS_WITH(g.connections("F"),
            "Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
    }
}

TEST_CASE("erase_node functionality") {
    gdwg::graph<std::string, int> g;

    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));
    REQUIRE(g.insert_node("D"));

    REQUIRE(g.insert_edge("A", "B", 1));
    REQUIRE(g.insert_edge("B", "C", 2));
    REQUIRE(g.insert_edge("C", "D", 3));
    REQUIRE(g.insert_edge("D", "A", 4));
    REQUIRE(g.insert_edge("A", "C", 5));

    SECTION("Erase existing node") {
        REQUIRE(g.erase_node("B") == true);
        REQUIRE(g.is_node("B") == false);
        REQUIRE(g.connections("A").size() == 1);
        REQUIRE(g.connections("C").size() == 1);
    }

    SECTION("Erase non-existing node") {
        REQUIRE(g.erase_node("E") == false);
        REQUIRE(g.nodes().size() == 4);
    }

    SECTION("Erase node with self-loop") {
        REQUIRE(g.insert_edge("A", "A", 6));
        REQUIRE(g.erase_node("A") == true);
        REQUIRE(g.is_node("A") == false);
        REQUIRE(g.connections("D").empty());
    }

    SECTION("Erase all nodes") {
        REQUIRE(g.erase_node("A") == true);
        REQUIRE(g.erase_node("B") == true);
        REQUIRE(g.erase_node("C") == true);
        REQUIRE(g.erase_node("D") == true);
        REQUIRE(g.nodes().empty());
    }
}

TEST_CASE("erase_edge functionality") {
    gdwg::graph<std::string, int> g;

    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));

    REQUIRE(g.insert_edge("A", "B", 1));
    REQUIRE(g.insert_edge("A", "C", 2));
    REQUIRE(g.insert_edge("B", "C", 3));
    REQUIRE(g.insert_edge("A", "B")); // Unweighted edge

    SECTION("Erase existing weighted edge") {
        REQUIRE(g.erase_edge("A", "B", 1) == true);
        REQUIRE(g.is_connected("A", "B") == true); // Unweighted edge still exists
        // Check that the weighted edge no longer exists
        auto edges = g.edges("A", "B");
        REQUIRE(std::none_of(edges.begin(), edges.end(), [](const auto& edge) {
            return edge->is_weighted() && edge->get_weight().value() == 1;
        }));
    }

    SECTION("Erase existing unweighted edge") {
        REQUIRE(g.erase_edge("A", "B") == true);
        REQUIRE(g.is_connected("A", "B") == true); // Weighted edge still exists
        // Check that the unweighted edge no longer exists
        auto edges = g.edges("A", "B");
        REQUIRE(std::none_of(edges.begin(), edges.end(), [](const auto& edge) {
            return !edge->is_weighted();
        }));
    }

    SECTION("Erase non-existing edge") {
        REQUIRE(g.erase_edge("A", "C", 1) == false);
        REQUIRE(g.erase_edge("B", "A") == false);
    }

    SECTION("Erase edge with non-existing nodes") {
        REQUIRE_THROWS_WITH(g.erase_edge("D", "A"),
            "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
        REQUIRE_THROWS_WITH(g.erase_edge("A", "D"),
            "Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
    }

    SECTION("Invalid itertor after erasing edge") {
        // Find the iterator for the edge we're going to erase
        auto it = g.find("A", "B", 1);
        auto next_it = g.find("A", "B", 1);
        
        REQUIRE(it != g.end());

        // Store the next edge for comparison
        ++next_it;
        (*it);

        // Erase the edge
        REQUIRE(g.erase_edge("A", "B", 1));
        // This one is to check that after deleting edge, dereferencing the iterator to that edge
        // will give out undefined behaviour -> so it is commented out
        // uncomment the line below will lead to a fail test report: SIGABRT - Abort (abnormal termination) signal
        // (*it);

    }
}


TEST_CASE("clear functionality") {
    gdwg::graph<std::string, int> g;

    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));

    REQUIRE(g.insert_edge("A", "B", 1));
    REQUIRE(g.insert_edge("B", "C", 2));
    REQUIRE(g.insert_edge("C", "A", 3));

    REQUIRE_FALSE(g.empty());

    g.clear();

    REQUIRE(g.empty());
    REQUIRE(g.nodes().empty());
    REQUIRE_FALSE(g.is_node("A"));
    REQUIRE_FALSE(g.is_node("B"));
    REQUIRE_FALSE(g.is_node("C"));
}

TEST_CASE("Graph iterator begin() with dereference operator") {
    gdwg::graph<int, std::string> g;
    
    // Add nodes and edges
    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_edge(1, 2, "Edge 1-2");
    g.insert_edge(2, 3, "Edge 2-3");
    g.insert_edge(1, 3, "Edge 1-3");

    // Test dereferencing the begin iterator
    auto it = g.begin();
    auto [from, to, weight] = *it;

    // Check if the first edge is correct
    REQUIRE(from == 1);
    REQUIRE(to == 2);
    REQUIRE(weight == "Edge 1-2");
}

TEST_CASE("Graph iterator equality comparison") {
    gdwg::graph<int, std::string> g;
    
    // Add some nodes and edges
    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_edge(1, 2, "Edge 1-2");
    g.insert_edge(2, 3, "Edge 2-3");

    SECTION("Two iterators pointing to the same edge") {
        auto it1 = g.begin();
        auto it2 = g.begin();
        REQUIRE(it1 == it2);
    }

    SECTION("Two iterators pointing to the end") {
        auto it1 = g.end();
        auto it2 = g.end();
        REQUIRE(it1 == it2);
    }
}

TEST_CASE("Graph iterator increment operator") {
    gdwg::graph<int, std::string> g;

    SECTION("Graph with one edge") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_edge(1, 2, "Edge 1-2");

        auto it = g.begin();
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == "Edge 1-2");

        ++it;
        REQUIRE(it == g.end());
    }

    SECTION("Graph with multiple edges - order checking") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_node(3);
        g.insert_edge(1, 2, "Edge 1-2");
        g.insert_edge(2, 3, "Edge 2-3");
        g.insert_edge(1, 3, "Edge 1-3");

        auto it = g.begin();
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == "Edge 1-2");

        ++it;
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "Edge 1-3");

        ++it;
        REQUIRE((*it).from == 2);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "Edge 2-3");

        ++it;
        REQUIRE(it == g.end());
    }

    SECTION("Graph with empty nodes between edges") {
        g.insert_node(1);
        g.insert_node(2);
        g.insert_node(3);
        g.insert_node(4);
        g.insert_edge(1, 2, "Edge 1-2");
        g.insert_edge(3, 4, "Edge 3-4");

        auto it = g.begin();
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == "Edge 1-2");

        ++it;
        REQUIRE((*it).from == 3);
        REQUIRE((*it).to == 4);
        REQUIRE((*it).weight == "Edge 3-4");

        ++it;
        REQUIRE(it == g.end());
    }
}

TEST_CASE("Graph iterator post-increment operator tests") {
    gdwg::graph<int, std::string> g;

    // Setup
    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_edge(1, 2, "1-2");
    g.insert_edge(1, 3, "1-3");
    g.insert_edge(2, 3, "2-3");

    SECTION("Post-increment returns copy of original iterator") {
        auto it = g.begin();
        auto original = it++;

        REQUIRE((*original).from == 1);
        REQUIRE((*original).to == 2);
        REQUIRE((*original).weight == "1-2");

        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "1-3");
    }

    SECTION("Multiple post-increments") {
        auto it = g.begin();
        auto first = it++;
        auto second = it++;
        auto third = it++;

        REQUIRE((*first).from == 1);
        REQUIRE((*first).to == 2);
        REQUIRE((*first).weight == "1-2");

        REQUIRE((*second).from == 1);
        REQUIRE((*second).to == 3);
        REQUIRE((*second).weight == "1-3");

        REQUIRE((*third).from == 2);
        REQUIRE((*third).to == 3);
        REQUIRE((*third).weight == "2-3");

        REQUIRE(it == g.end());
    }

    SECTION("Post-increment at the end") {
        auto it = g.begin();
        ++it; ++it; // Move to the last edge
        REQUIRE(it != g.end());
        
        auto last = it++;
        REQUIRE((*last).from == 2);
        REQUIRE((*last).to == 3);
        REQUIRE((*last).weight == "2-3");
        REQUIRE(it == g.end());
    }

    SECTION("Post-increment does not affect other iterators") {
        auto it1 = g.begin();
        auto it2 = g.begin();

        it1++;

        REQUIRE((*it2).from == 1);
        REQUIRE((*it2).to == 2);
        REQUIRE((*it2).weight == "1-2");

        REQUIRE((*it1).from == 1);
        REQUIRE((*it1).to == 3);
        REQUIRE((*it1).weight == "1-3");
    }

    SECTION("Chained post-increments") {
        auto it = g.begin();
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == "1-2");
        it++;
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "1-3");
    }
}

TEST_CASE("Graph iterator decrement operator tests") {
    gdwg::graph<char, std::string> g;

    // Setup the graph as described
    g.insert_node('A');
    g.insert_node('B');
    g.insert_node('C');
    g.insert_node('D');
    g.insert_node('E');
    g.insert_node('F');

    g.insert_edge('C', 'D', "C-D");
    g.insert_edge('D', 'C', "D-C");

    SECTION("Decrement from second edge to beginning") {
        auto it = g.begin();
        ++it; // Move to the second edge

        REQUIRE((*it).from == 'D');
        REQUIRE((*it).to == 'C');
        REQUIRE((*it).weight == "D-C");

        --it; // Decrement to the first edge

        REQUIRE((*it).from == 'C');
        REQUIRE((*it).to == 'D');
        REQUIRE((*it).weight == "C-D");

        REQUIRE(it == g.begin());
    }

    SECTION("Multiple decrements") {
        auto it = g.end();
        --it;
        REQUIRE((*it).from == 'D');
        REQUIRE((*it).to == 'C');
        REQUIRE((*it).weight == "D-C");
        --it;
        REQUIRE((*it).from == 'C');
        REQUIRE((*it).to == 'D');
        REQUIRE((*it).weight == "C-D");
        REQUIRE(it == g.begin());
    }

}

TEST_CASE("Graph iterator post-decrement operator tests") {
    gdwg::graph<int, std::string> g;

    // Setup
    g.insert_node(1);
    g.insert_node(2);
    g.insert_node(3);
    g.insert_edge(1, 2, "1-2");
    g.insert_edge(2, 3, "2-3");
    g.insert_edge(3, 1, "3-1");

    SECTION("Post-decrement returns copy of original iterator") {
        auto it = g.end();
        --it; // Move to last edge
        auto original = it--;

        REQUIRE((*original).from == 3);
        REQUIRE((*original).to == 1);
        REQUIRE((*original).weight == "3-1");

        REQUIRE((*it).from == 2);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "2-3");
    }

    SECTION("Multiple post-decrements") {
        auto it = g.end();
        auto third = it--;
        auto second = it--;
        auto first = it--;

        REQUIRE(third == g.end());

        REQUIRE((*second).from == 3);
        REQUIRE((*second).to == 1);
        REQUIRE((*second).weight == "3-1");

        REQUIRE((*first).from == 2);
        REQUIRE((*first).to == 3);
        REQUIRE((*first).weight == "2-3");

        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == "1-2");
    }

    SECTION("Post-decrement returns correct iterator and moves original") {
        auto it = g.end();
        --it; // Move to last edge
        
        auto returned_it = it--;

        // Check that returned iterator points to the original position
        REQUIRE((*returned_it).from == 3);
        REQUIRE((*returned_it).to == 1);
        REQUIRE((*returned_it).weight == "3-1");

        // Check that original iterator has moved to the previous edge
        REQUIRE((*it).from == 2);
        REQUIRE((*it).to == 3);
        REQUIRE((*it).weight == "2-3");

        // Verify that returned_it and it are different
        REQUIRE_FALSE(returned_it == it);
    }

}

TEST_CASE("Two empty graphs should be equal") {
    gdwg::graph<int, int> g1;
    gdwg::graph<int, int> g2;

    REQUIRE(g1 == g2); // Both graphs are empty, so they should be equal
}

TEST_CASE("Two graphs with same nodes but empty edges should be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into both graphs
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));
    REQUIRE(g2.insert_node("C"));

    REQUIRE(g1 == g2); // Both graphs have the same nodes and no edges, so they should be equal
}

TEST_CASE("Two graphs with same nodes and same edges should be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into both graphs
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));
    REQUIRE(g2.insert_node("C"));

    // Insert edges into both graphs
    REQUIRE(g1.insert_edge("A", "B", 1));
    REQUIRE(g1.insert_edge("B", "C", 2));
    REQUIRE(g1.insert_edge("C", "A", 3));

    REQUIRE(g2.insert_edge("A", "B", 1));
    REQUIRE(g2.insert_edge("B", "C", 2));
    REQUIRE(g2.insert_edge("C", "A", 3));

    REQUIRE(g1 == g2); // Both graphs have the same nodes and edges, so they should be equal
}

TEST_CASE("Graphs with different number of nodes should not be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into the first graph
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    // Insert nodes into the second graph
    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));

    REQUIRE_FALSE(g1 == g2); // The first graph has 3 nodes, the second graph has 2 nodes, so they should not be equal
}

TEST_CASE("Graphs with same number of nodes but different number of edges should not be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into both graphs
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));
    REQUIRE(g2.insert_node("C"));

    // Insert edges into the first graph
    REQUIRE(g1.insert_edge("A", "B", 1));
    REQUIRE(g1.insert_edge("B", "C", 2));

    // Insert edges into the second graph
    REQUIRE(g2.insert_edge("A", "B", 1));
    REQUIRE(g2.insert_edge("B", "C", 2));
    REQUIRE(g2.insert_edge("C", "A", 3));

    REQUIRE_FALSE(g1 == g2); // The first graph has 2 edges, the second graph has 3 edges, so they should not be equal
}

TEST_CASE("Graphs with same number of nodes and edges but different edge weights should not be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into both graphs
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));
    REQUIRE(g2.insert_node("C"));

    // Insert edges into the first graph with specific weights
    REQUIRE(g1.insert_edge("A", "B", 1));
    REQUIRE(g1.insert_edge("B", "C", 2));
    REQUIRE(g1.insert_edge("C", "A", 3));

    // Insert edges into the second graph with different weights
    REQUIRE(g2.insert_edge("A", "B", 10));
    REQUIRE(g2.insert_edge("B", "C", 20));
    REQUIRE(g2.insert_edge("C", "A", 30));

    REQUIRE_FALSE(g1 == g2); // The graphs have different edge weights, so they should not be equal
}

TEST_CASE("Graphs with same number of nodes and edges but different edge types (weighted vs. unweighted) should not be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into both graphs
    REQUIRE(g1.insert_node("A"));
    REQUIRE(g1.insert_node("B"));
    REQUIRE(g1.insert_node("C"));

    REQUIRE(g2.insert_node("A"));
    REQUIRE(g2.insert_node("B"));
    REQUIRE(g2.insert_node("C"));

    // Insert edges into the first graph (all weighted)
    REQUIRE(g1.insert_edge("A", "B", 1));
    REQUIRE(g1.insert_edge("B", "C", 2));
    REQUIRE(g1.insert_edge("C", "A", 3));

    // Insert edges into the second graph (all unweighted)
    REQUIRE(g2.insert_edge("A", "B", std::nullopt));
    REQUIRE(g2.insert_edge("B", "C", std::nullopt));
    REQUIRE(g2.insert_edge("C", "A", std::nullopt));

    REQUIRE_FALSE(g1 == g2); // The first graph has weighted edges, the second graph has unweighted edges, so they should not be equal
}

TEST_CASE("Graphs with same number of nodes but different node names should not be equal") {
    gdwg::graph<std::string, int> g1;
    gdwg::graph<std::string, int> g2;

    // Insert nodes into the first graph with specific names
    REQUIRE(g1.insert_node("Node1"));
    REQUIRE(g1.insert_node("Node2"));
    REQUIRE(g1.insert_node("Node3"));

    // Insert nodes into the second graph with different names
    REQUIRE(g2.insert_node("NodeA"));
    REQUIRE(g2.insert_node("NodeB"));
    REQUIRE(g2.insert_node("NodeC"));

    // Insert edges into both graphs
    REQUIRE(g1.insert_edge("Node1", "Node2", 5));
    REQUIRE(g1.insert_edge("Node2", "Node3", 10));
    REQUIRE(g1.insert_edge("Node3", "Node1", 15));

    REQUIRE(g2.insert_edge("NodeA", "NodeB", 5));
    REQUIRE(g2.insert_edge("NodeB", "NodeC", 10));
    REQUIRE(g2.insert_edge("NodeC", "NodeA", 15));

    REQUIRE_FALSE(g1 == g2); // The graphs have different node names, so they should not be equal
}

TEST_CASE("Graph output operator test") {
    SECTION("Graph with isolated nodes") {
        gdwg::graph<std::string, double> g;

        // Insert nodes
        REQUIRE(g.insert_node("A"));
        REQUIRE(g.insert_node("B"));
        REQUIRE(g.insert_node("C"));

        // Insert one edge
        REQUIRE(g.insert_edge("A", "B", 1.5));

        std::ostringstream oss;
        oss << g;

        std::string expected_output = R"(A (
  A -> B | W | 1.5
)
B (
)
C (
)
)";

        REQUIRE(oss.str() == expected_output);
    }

    SECTION("Empty graph") {
        gdwg::graph<char, int> g;

        std::ostringstream oss;
        oss << g;

        std::string expected_output = "";

        REQUIRE(oss.str() == expected_output);
    }



    SECTION("Graph with multiple edges between same nodes") {
        gdwg::graph<int, int> g;

        // Insert nodes
        REQUIRE(g.insert_node(1));
        REQUIRE(g.insert_node(2));

        // Insert multiple edges between same nodes
        REQUIRE(g.insert_edge(1, 2));         // Unweighted edge
        REQUIRE(g.insert_edge(1, 2, 5));      // Weighted edge
        REQUIRE(g.insert_edge(1, 2, 3));      // Another weighted edge

        std::ostringstream oss;
        oss << g;

        std::string expected_output = R"(1 (
  1 -> 2 | U
  1 -> 2 | W | 3
  1 -> 2 | W | 5
)
2 (
)
)";
        REQUIRE(oss.str() == expected_output);
    }

     SECTION("Graph with multiple nodes and mixed edge types") {
        gdwg::graph<int, int> g;

        // Insert nodes
        REQUIRE(g.insert_node(1));
        REQUIRE(g.insert_node(2));
        REQUIRE(g.insert_node(3));
        REQUIRE(g.insert_node(4));
        REQUIRE(g.insert_node(5));

        // Insert edges
        REQUIRE(g.insert_edge(1, 2, 10));     // Weighted edge
        REQUIRE(g.insert_edge(1, 3));         // Unweighted edge
        REQUIRE(g.insert_edge(2, 3, 5));      // Weighted edge
        REQUIRE(g.insert_edge(2, 4));         // Unweighted edge
        REQUIRE(g.insert_edge(3, 4, 7));      // Weighted edge
        REQUIRE(g.insert_edge(4, 5));         // Unweighted edge
        REQUIRE(g.insert_edge(5, 1, 3));      // Weighted edge

        std::ostringstream oss;
        oss << g;

        std::string expected_output = R"(1 (
  1 -> 2 | W | 10
  1 -> 3 | U
)
2 (
  2 -> 3 | W | 5
  2 -> 4 | U
)
3 (
  3 -> 4 | W | 7
)
4 (
  4 -> 5 | U
)
5 (
  5 -> 1 | W | 3
)
)";

        REQUIRE(oss.str() == expected_output);
    }

}

TEST_CASE("Graph find function tests") {
    gdwg::graph<std::string, int> g;

    // Setup
    REQUIRE(g.insert_node("A"));
    REQUIRE(g.insert_node("B"));
    REQUIRE(g.insert_node("C"));
    REQUIRE(g.insert_edge("A", "B", 10));
    REQUIRE(g.insert_edge("A", "C"));  // Unweighted edge

    SECTION("Find non-existing edge should return end()") {
        auto it = g.find("A", "D");  // "D" doesn't exist
        REQUIRE(it == g.end());

        auto it2 = g.find("B", "A");  // Edge exists in opposite direction
        REQUIRE(it2 == g.end());

        auto it3 = g.find("A", "B", 20);  // Edge exists but with different weight
        REQUIRE(it3 == g.end());
    }

    SECTION("Find existing edge, dereferencing iterator should return correct edge properties") {
        auto it = g.find("A", "B", 10);
        REQUIRE(it != g.end());

        auto edge = *it;
        REQUIRE(edge.from == "A");
        REQUIRE(edge.to == "B");
        REQUIRE(edge.weight.has_value());
        REQUIRE(edge.weight.value() == 10);

        auto it2 = g.find("A", "C");  // Unweighted edge
        REQUIRE(it2 != g.end());

        auto it3 = g.find("A", "C");
        edge = *it3;
        REQUIRE(edge.from == "A");
        REQUIRE(edge.to == "C");
        REQUIRE_FALSE(edge.weight.has_value());
    }

    SECTION("Two finds to the same edge should result in iterators with the same dereferenced value") {
        auto it1 = g.find("A", "B", 10);
        auto it2 = g.find("A", "B", 10);

        REQUIRE(it1 != g.end());
        REQUIRE(it2 != g.end());

        auto edge1 = *it1;
        auto edge2 = *it2;

        REQUIRE(edge1.from == edge2.from);
        REQUIRE(edge1.to == edge2.to);
        REQUIRE(edge1.weight == edge2.weight);

        // Check that the iterators are distinct objects
        ++it1;
        REQUIRE((*it1).to != (*it2).to); // C vs B
        REQUIRE((*it1).weight != (*it2).weight); // Weighted vs unweighted
    }
}

TEST_CASE("Additional Graph find function tests") {
    gdwg::graph<int, double> g;

    // Setup
    REQUIRE(g.insert_node(1));
    REQUIRE(g.insert_node(2));
    REQUIRE(g.insert_node(3));
    REQUIRE(g.insert_node(4));
    REQUIRE(g.insert_edge(1, 2, 1.5));
    REQUIRE(g.insert_edge(1, 3));  // Unweighted edge
    REQUIRE(g.insert_edge(2, 3, 2.5));
    REQUIRE(g.insert_edge(3, 4, 3.5));
    REQUIRE(g.insert_edge(4, 1, 4.5));

    SECTION("Find edge with default weight (std::nullopt)") {
        auto it = g.find(1, 3);
        REQUIRE(it != g.end());
        auto edge = *it;
        REQUIRE(edge.from == 1);
        REQUIRE(edge.to == 3);
        REQUIRE_FALSE(edge.weight.has_value());
    }

    SECTION("Find edge after node replacement") {
        REQUIRE(g.replace_node(2, 5));
        auto it = g.find(1, 5, 1.5);
        REQUIRE(it != g.end());
        auto edge = *it;
        REQUIRE(edge.from == 1);
        REQUIRE(edge.to == 5);
        REQUIRE(edge.weight == 1.5);
    }

    SECTION("Find edge after inserting multiple edges between same nodes") {
        auto it = g.find(1, 2, 1.5);
        REQUIRE(g.insert_edge(1, 2, 2.5));
        auto it1 = g.find(1, 2, 1.5);
        auto it2 = g.find(1, 2, 2.5);
        REQUIRE(it != g.end());
        REQUIRE((*it).from == 1);
        REQUIRE((*it).to == 2);
        REQUIRE((*it).weight == 1.5);
        REQUIRE(it1 != g.end());
        REQUIRE(it2 != g.end());
        REQUIRE((*it1).from == 1);
        REQUIRE((*it1).to == 2);
        REQUIRE((*it1).weight == 1.5);
        REQUIRE((*it2).from == 1);
        REQUIRE((*it2).to == 2);
        REQUIRE((*it2).weight == 2.5);
    }

    SECTION("Find edge after erasing another edge") {
        auto it0 = g.find(1, 2, 1.5);
        REQUIRE_FALSE(it0 == g.end());
        REQUIRE(g.erase_edge(1, 2, 1.5));
        auto it = g.find(1, 2, 1.5);
        REQUIRE(it == g.end());
        auto it2 = g.find(1, 3);
        REQUIRE(it2 != g.end());
    }

    SECTION("Find after clear") {
        auto it0 = g.find(1, 2, 1.5);
        REQUIRE_FALSE(it0 == g.end());
        g.clear();
        auto it = g.find(1, 2, 1.5);
        REQUIRE(it == g.end());
    }

}

TEST_CASE("erase_edge iterator i") {
    SECTION("erase_edge iterator i at the beginning") {
        auto g = gdwg::graph<int, std::string>{1, 2, 3};
        g.insert_edge(1, 2);
        g.insert_edge(1, 3);
        auto it = (g.erase_edge(g.begin()));
        CHECK(it == g.begin());
        CHECK_FALSE(g.is_connected(1, 2));
        CHECK(g.is_connected(1, 3));
        auto g2 = gdwg::graph<std::string, int>{"A", "B", "C"};
        g2.insert_edge("A", "B");
        g2.insert_edge("A", "C");
        auto it2 = g2.erase_edge(g2.begin());
        CHECK(g2.begin() == it2);
        CHECK_FALSE(g2.is_connected("A", "B"));
        CHECK(g2.is_connected("A", "C"));
    }
}


TEST_CASE("erase_edge iterator i with end() iterator") {
    auto g = gdwg::graph<int, std::string>{1, 2, 3};
    g.insert_edge(1, 2);
    g.insert_edge(1, 3);
    CHECK(g.erase_edge(g.end()) == g.end());
    CHECK(g.is_connected(1, 2));
    CHECK(g.is_connected(1, 3));
}

TEST_CASE("erase_edge iterator i at the end") {
    auto g = gdwg::graph<int, std::string>{1, 2, 3};
    g.insert_edge(1, 2);
    g.insert_edge(1, 3);
    CHECK(g.erase_edge(--g.end()) == g.end());
    CHECK(g.is_connected(1, 2));
    CHECK_FALSE(g.is_connected(1, 3));

    auto g2 = gdwg::graph<std::string, int>{"A", "B", "C"};
    g2.insert_edge("A", "B");
    g2.insert_edge("A", "C");
    CHECK(g2.erase_edge(--g2.end()) == g2.end());
    CHECK(g2.is_connected("A", "B"));
    CHECK_FALSE(g2.is_connected("A", "C"));
}

TEST_CASE("erase_edge range of iterators [i, s)") {
    // Test case with integer nodes and string edges
    auto g = gdwg::graph<int, std::string>{1, 2, 3, 4};
    g.insert_edge(1, 2, "a");
    g.insert_edge(1, 3, "b");
    g.insert_edge(2, 4, "c");

    // Erase edges between 1 and the edge between nodes (2, 4)
    auto start_it = g.begin();
    auto end_it = g.find(2, 4, "c");

    CHECK(g.erase_edge(start_it, end_it) == end_it);

    // Check that the edge between (1, 2) and (1, 3) were erased, but (2, 4) remains
    CHECK_FALSE(g.is_connected(1, 2));
    CHECK_FALSE(g.is_connected(1, 3));
    CHECK(g.is_connected(2, 4));

    // Test case with string nodes and integer edges
    auto g2 = gdwg::graph<std::string, int>{"A", "B", "C", "D"};
    g2.insert_edge("A", "B", 1);
    g2.insert_edge("A", "C", 2);
    g2.insert_edge("B", "D", 3);

    // Erase edges between "A" and the edge between nodes ("B", "D")
    auto start_it2 = g2.begin();
    auto end_it2 = g2.find("B", "D", 3);

    CHECK(g2.erase_edge(start_it2, end_it2) == end_it2);

    // Check that the edge between ("A", "B") and ("A", "C") were erased, but ("B", "D") remains
    CHECK_FALSE(g2.is_connected("A", "B"));
    CHECK_FALSE(g2.is_connected("A", "C"));
    CHECK(g2.is_connected("B", "D"));
}


TEST_CASE("basic test") {
	// These are commented out right now
	//  because withour your implementation
	//  it will not compile. Uncomment them
	//  once you've done the work
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}
