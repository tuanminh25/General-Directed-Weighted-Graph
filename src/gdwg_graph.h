#ifndef GDWG_GRAPH_H
#define GDWG_GRAPH_H
#include <initializer_list>
#include <algorithm>
#include <optional>
#include <set>
#include <vector>
#include <utility>
#include <iostream>
#include <memory>
#include <sstream>
#include <map>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <ranges>
#include <typeinfo>
namespace gdwg {
	// General template for to_string
	template <typename T>
	std::string to_string(T const& value) {
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}

	// Specialization for std::string
	template <>
	std::string to_string<std::string>(std::string const& value) {
		return value;
	}

	// Forward declaration of graph
	template<typename N, typename E>
	class graph;

	template<typename N, typename E>
	class edge {
		public:
			/**
			* Effects: Returns a string representation of the edge.
			* 
			* Returns: A string representation of the edge.
			* 
			* Remarks: The format of the string representation is src -> dst | W |  weight if the edge is weighted
			*  src -> dst | U if the edge is unweighted.
			* 
			* Note: print_edge will be used in the operator<< overload for the graph class.
			*/
			virtual auto print_edge() const noexcept -> std::string = 0;

			/**
			* Effects: identify whether the edge is weighted.
			* 
			* Returns: true if the edge is weighted, and false otherwise.
			*
			*/
			virtual auto is_weighted() const noexcept -> bool = 0;

			/**
			* Effects: Returns the weight of the edge, std::nullopt if the edge is unweighted.
			* 
			* Returns: The weight of the edge.
			*
			*/
			virtual auto get_weight() const noexcept -> std::optional<E> = 0;

			/**
			* Effects: Returns the source and destination nodes of the edge.
			* 
			* Returns: A pair of the source and destination nodes of the edge.
			*
			*/
			virtual auto get_nodes() const noexcept -> std::pair<N, N> {
				return std::pair(this->src_, this->dst_);
			};

			/**
			* Returns: Returns true if two edges are equal, false otherwise
			*/
			auto operator==(edge<N, E> const& other) const -> bool {
				// Compare nodes
				if (this->src_ != other.src_ or this->dst_ != other.dst_) {
					return false;
				}

				// If both are weighted, compare weights
				if (this->is_weighted() and other.is_weighted()) {
					return this->get_weight() == other.get_weight();
				}
				
				// If either is unweighted, weights should not be compared
				return !this->is_weighted() and !other.is_weighted();
			};

			/**
			* Destructor
			*/
			virtual ~edge() = default;

		protected:
			auto change_src(N const& new_src) -> void {
				src_ = new_src;
			}

			auto change_dst(N const& new_dst) -> void {
				dst_ = new_dst;
			}

			edge(N src, N dst, E weight) : src_(src), dst_(dst), weight_(weight) {};
			N src_;
			N dst_;
			E weight_;
	 	private:
			friend class graph<N, E>;
	};

	template<typename N, typename E>
	class unweighted_edge : public gdwg::edge<N, E> {
		public:
			/**
			* Constructor for unweighted_edge
			* 
			* @param src The source node of the edge.
			* @param dst The destination node of the edge.
			*/
			unweighted_edge(N const& src, N const& dst) : gdwg::edge<N, E>{src, dst, E{1}} {}

			/**
			* Effects: Returns a string representation of the edge.
			* 
			* Returns: A string representation of the edge.
			* 
			* Remarks: src -> dst | U if the edge is unweighted.
			* 
			* Note: print_edge will be used in the operator<< overload for the graph class.
			*/
			auto print_edge() const noexcept -> std::string override {
				return to_string(this->src_) + " -> " + to_string(this->dst_) + " | U";
			};

			/**
			* Effects: identify whether the edge is weighted.
			* 
			* Returns: true if the edge is weighted, and false otherwise.
			*
			*/
			auto is_weighted() const noexcept -> bool override {
				return false;
			};

			/**
			* Effects: Returns the weight of the edge, std::nullopt if the edge is unweighted.
			* 
			* Returns: The weight of the edge.
			*
			*/
			auto get_weight() const noexcept -> std::optional<E> override {
				return std::nullopt;
			};
	};

	template<typename N, typename E>
	class weighted_edge : public gdwg::edge<N, E> {
		public:
			/**
			* Constructor for weighted edge
			* 
			* @param src The source node of the edge.
			* @param dst The destination node of the edge.
			*/
			weighted_edge(N const& src, N const& dst, E const& weight) : gdwg::edge<N, E>{src, dst, weight} {}

			/**
			* Effects: Returns a string representation of the edge.
			* 
			* Returns: A string representation of the edge.
			* 
			* Remarks: The format of the string representation is src -> dst | W |  weight if the edge is weighted
			* 
			* Note: print_edge will be used in the operator<< overload for the graph class.
			*/
			auto print_edge() const noexcept -> std::string override {
				return to_string(this->src_) + " -> " + to_string(this->dst_) + " | W | " + to_string(this->weight_);
			};

			/**
			* Effects: identify whether the edge is weighted.
			* 
			* Returns: true if the edge is weighted, and false otherwise.
			*
			*/
			auto is_weighted() const noexcept -> bool override {
				return true;
			};

			/**
			* Effects: Returns the weight of the edge, std::nullopt if the edge is unweighted.
			* 
			* Returns: The weight of the edge.
			*
			*/
			auto get_weight() const noexcept -> std::optional<E> override {
				return this->weight_;
			};
	};

	template<typename N, typename E>
	class graph {
		using edge_ = gdwg::edge<N, E>;
		using edge_ptr = std::shared_ptr<edge_>;
		struct EdgePtrComparator {
			bool operator()(const edge_ptr& a, const edge_ptr& b) const {					
				// 0. Check if they are the same
				if (a == b) {
					return false;
				}

				// 1. Order by src
				if (a->get_nodes().first != b->get_nodes().first) {
					return a->get_nodes().first < b->get_nodes().first;
				}

				// 2. Order by dst
				if (a->get_nodes().second != b->get_nodes().second) {
					return a->get_nodes().second < b->get_nodes().second;
				}

				// 3. Order by unweighted first
				if (a->is_weighted() != b->is_weighted()) {
					return !a->is_weighted();
				}

				// 4. Order by weight 
				if (a->is_weighted() and b->is_weighted()) {
					if (a->get_weight() != b->get_weight()) {
						return a->get_weight() < b->get_weight();
					}
				}

				// 5. Order by memory address
				return std::less<const edge_*>{}(a.get(), b.get());
			}
		};
		using edge_ptr_set = std::set<edge_ptr, EdgePtrComparator>;
		using edge_ptr_set_pair = std::pair<edge_ptr_set, edge_ptr_set>;

	 	public:
			// Forward declaration of iterator
			class iterator;
			
			/**
			* Default constructor for graph
			*/
			graph() noexcept = default;

			/**
			 * Equivalent to: graph(il.begin(), il.end());
			 */
			graph(std::initializer_list<N> il) noexcept {
				std::for_each(il.begin(), il.end(), [this](const N& node) {
					g_[node] = edge_ptr_set_pair();
				});
			}

			/**
			 * Precondition: InputIt models Cpp17 Input Iterator 
			 * Preconditon: InputIt indirectly readable as type N
			 * 
			 * Initialises the graph’s node collection with the range [first, last)
			 */
			template<typename InputIt>
			graph(InputIt first, InputIt last) noexcept {
				std::for_each(first, last, [this](const N& node) {
					g_[node] = edge_ptr_set_pair();
				});
			}

			/**
			 * Postcondition: *this is equal to the value other had before this constructor’s invocation
			 * 
			 * Postcondition: other.empty() is true
			 * 
			 * Postcondition: All iterators pointing to elements owned by *this 
			 * prior to this constructor’s invocation are invalidated
			 * 
			 * Postcondition: All iterators pointing to elements owned by other 
			 * prior to this constructor’s invocation remain valid, 
			 * but now point to the elements owned by *this
			 *
			 */
			graph(graph&& other) noexcept {
				g_ = std::exchange(other.g_, {});
			};

			/**
			 * Effects: All existing nodes and edges are either move-assigned to, or are destroyed
			 *
			 * Postconditions:
			 * - *this is equal to the value other had before this operator’s invocation.
			 * - All iterators pointing to elements owned by *this prior to this operator’s invocation are invalidated.
			 * - All iterators pointing to elements owned by other prior to this operator’s invocation remain valid, 
			 * but now point to the elements owned by *this.
			 *
			 * Returns: *this.
			 */
			auto operator=(graph&& other) noexcept -> graph& {
				if (this == &other) {
					return *this;
				}
				g_ = std::exchange(other.g_, {});
				return *this;
			};

			/**
			 * Postconditions: *this == other is true
			 */
			graph(graph const& other) noexcept : g_(std::move(other.g_)) {};

			/**
			 * Postconditions
			 * - *this == other is true
			 * - All iterators pointing to elements owned by *this prior to this operator’s invocation are invalidated
			 * 
			 * Returns: *this
			 */
			auto operator=(graph const& other) -> graph& {
				g_ = std::move(other.g_);
				return *this;
			};

			// Iterator related function:


			// Your member functions go here
			/**
			* Effects: Adds a new node with value value to the graph if, and only if, there is no node equivalent to value already stored.
			* Postconditions: All iterators are invalidated.
			* Returns: true if the node is added to the graph and false otherwise.
			*/
			auto insert_node(N const& value) noexcept -> bool {
				if (is_node(value)) {
					return false;
				}
				g_[value] = edge_ptr_set_pair();
				return true;
			};

			/**
			* Effects: Adds a new edge representing src → dst with an optional weight. 
			* 
			* - If weight is std::nullopt, an unweighted_edge is created. 
			* - Otherwise, a weighted_edge with the specified weight is created. 
			* - The edge is only added if there is no existing edge between src and dst with the same weight.
			* [Note:⁠ Nodes are allowed to be connected to themselves. —end note]
			* Postconditions: All iterators are invalidated.
			*
			* Returns: true if the node is added to the graph and false otherwise.
			*
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist") if either of is_node(src) or is_node(dst) are false.
			* [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			*/
			auto create_edge_ptr (N const& src, N const& dst, std::optional<E> weight = std::nullopt) const -> std::shared_ptr<edge_> {
				if (weight == std::nullopt) {
					return std::make_shared<gdwg::unweighted_edge<N, E>>(src, dst);
				} else {
					return std::make_shared<gdwg::weighted_edge<N, E>>(src, dst, weight.value());
				}
			}

			// Find based on edges from given node
			auto existed_edge(const std::set<std::shared_ptr<edge_>, EdgePtrComparator>& edges_ptr, 
									const std::shared_ptr<edge_>& new_edge_ptr) const -> bool {
				
				// Define a lambda to compare edges based on their content, not their address
				auto edge_content_equal = [&new_edge_ptr](const edge_ptr& e) {
					return *e == *new_edge_ptr;
				};

				auto it = std::find_if(edges_ptr.begin(), edges_ptr.end(), edge_content_equal);
				if (it == edges_ptr.end()) {
					return false;
				} else {
					return true;
				}
			}

			auto insert_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
				// Check src and dst existence first
				if (!is_node(src) or !is_node(dst)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist");
				}

				auto new_edge_ptr = create_edge_ptr(src, dst, weight);
				if (existed_edge(g_[src].first, new_edge_ptr) or existed_edge(g_[dst].second, new_edge_ptr)) {
					return false;
				}
				
				// The set will automatically order the edges based on the custom comparator
				return g_[src].first.insert(new_edge_ptr).second and
 				g_[dst].second.insert(new_edge_ptr).second;
			};

			/**
			* Effects: Replaces the original data, old_data, stored at this particular node by the replacement data, new_data. 
			* Does nothing if new_data already exists as a node.
			*
			* Postconditions: All iterators are invalidated.
			*
			* Returns: false if a node that contains value new_data already exists and true otherwise.
			*
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist") if is_node(old_data) is false.
			*
			* [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			*
			*/
			auto move_node_data(N const& old_node, N const& new_node) -> void {
				auto insert_new_edge = [this](const auto& edge_ptr, const N& src, const N& dst) {
					edge_ptr->is_weighted()
						? this->insert_edge(src, dst, edge_ptr->get_weight())
						: this->insert_edge(src, dst);
				};

				// Handle outgoing edges
				for (const auto& edge : g_[old_node].first)
					insert_new_edge(edge, new_node, edge->get_nodes().second);

				// Handle incoming edges
				for (const auto& edge : g_[old_node].second)
					insert_new_edge(edge, edge->get_nodes().first, new_node);

				g_.erase(old_node);
			}

			auto replace_node(N const& old_data, N const& new_data) -> bool {
				// Check src and dst existence first
				if (!is_node(old_data)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
				}
				
				if (is_node(new_data)) {
					return false;
				}
				
				insert_node(new_data);
				move_node_data(old_data, new_data);
				return true;
			};

			/**
			* Effects: The node equivalent to old_data in the graph are replaced with instances of new_data. 
			* After completing, every incoming and outgoing edge of old_data becomes an incoming/ougoing edge of new_data, 
			* except that duplicate edges shall be removed.
			* 
			* Postconditions: All iterators are invalidated.
			* 
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph") 
			* if either of is_node(old_data) or is_node(new_data) are false.
			* 
			* 
			*/
			auto merge_replace_node(N const& old_data, N const& new_data) -> void {
				// Check src and dst existence first
				if (!is_node(old_data) or !is_node(new_data)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph");
				}
					
				if (old_data == new_data) {
					return;
				}
				move_node_data(old_data, new_data);
			}; 

			/**
			* Effects: Erases node equivalent to value, including all incoming and outgoing edges.
			* Returns: true if value was removed; false otherwise.
			* Postconditions: All iterators are invalidated.
			*/
			auto erase_node(N const& value) -> bool {
				// Check if the node exists
				if (!is_node(value)) {
					return false;
				}

				// 1. Erase all incoming and outgoing edges of the node
				g_[value].first.clear();  // Clear outgoing edges
				g_[value].second.clear(); // Clear incoming edges

				// 2. Go through all other nodes and remove edges associated with the deleted node
				for (auto& [node, edge_sets] : g_) {
					if (node != value) {
						std::erase_if(edge_sets.first, 
							[&value](const auto& edge) { return edge->get_nodes().second == value; });
						std::erase_if(edge_sets.second, 
							[&value](const auto& edge) { return edge->get_nodes().first == value; });
					}
				}

				// 3. Finally, erase the node itself
				g_.erase(value);

				return true;
			}


			/**
			* Effects: Erases the edge representing src → dst with the specified weight. 
			* If weight is std::nullopt, it erases the unweighted_edge between src and dst. 
			* If weight has a value, it erases the weighted_edge between src and dst with the specified weight.
			* 
			* Returns: true if an edge was removed; false otherwise.
			* Postconditions: All iterators are invalidated.
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph") if either is_node(src) or is_node(dst) is false.
			* [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			* Complexity: O(log( n) + e), where n is the total number of stored nodes and e is the total number of stored edges.
			*/
			auto erase_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
				// Check if src and dst exist in the graph
				if (!is_node(src) or !is_node(dst)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph");
				}

				// Handling out going edge
				// Create a temporary edge for comparison
				auto temp_edge_ptr = create_edge_ptr(src, dst, weight);

				// Try to erase from outgoing edges of src
				auto& outgoing_edges = g_[src].first;

				// Define a lambda to compare edges based on their content, not their address
				auto edge_content_equal = [&temp_edge_ptr](const edge_ptr& e) {
					return *e == *temp_edge_ptr;
				};

				// Find the iterator to the edge we want to remove
				auto it = std::find_if(outgoing_edges.begin(), outgoing_edges.end(), edge_content_equal);
				if (it != outgoing_edges.end()) {
					outgoing_edges.erase(it);
				} else {
					return false;
				}

				// Handling incomming edges
				// Now erase from incoming edges of dst
				auto& incoming_edges = g_[dst].second;
				auto in_it = std::find_if(incoming_edges.begin(), incoming_edges.end(), edge_content_equal);
				if (in_it != incoming_edges.end()) {
					incoming_edges.erase(in_it);
				} else {
					return false;
				}

				return true;
			}


			/**
			* Effects: Erases the edge pointed to by i.
			*
			* Complexity: O(log (n) + e), where n is the total number of stored nodes 
			* and e is the total number of stored edges. 
			* [Note: This complexity requirement is slightly weaker 
			* than a real-world container to help make the assignment easier. —end note]
			*
			* Returns: An iterator pointing to the element immediately after i 
			* prior to the element being erased. 
			* If no such element exists, returns end().
			*
			* Postconditions: All iterators are invalidated. 
			* [Note: The postcondition is slightly stricter than a real-world container to help make the assignment easier (i.e. we won’t be testing any iterators post-erasure). —end note]
			*/
			auto erase_edge(iterator i) -> iterator {
				if (i == end()) {
        			return end();
    			}
				// Copy the edge where the iterator points to
				auto src = (*i).from;
				auto dst = (*i).to;
				auto weight = (*i).weight;

				// Get the iterator to the element after i
				auto next = i;
				++next;
				// Erase the edge
				if (erase_edge(src, dst, weight)) {
					// If successful, return the iterator to the element after i
					return next;
				} else {
					// If the edge wasn't found (which shouldn't happen), return the original iterator
					return i;
				}
			};

			/**
			* Effects: Erases all edges between the iterators [i, s).
			* Complexity O(d(log( n) + e)), where d = std::distance(i, s). 
			* 
			* Returns: An iterator equivalent to s prior to the items iterated through being erased. If no such element exists, returns end().
			* 
			* Postconditions: All iterators are invalidated. [Note: The postcondition is slightly stricter than a real-world container to help make the assignment easier (i.e. we won’t be testing any iterators post-erasure). —end note]
			*/
			auto erase_edge(iterator i, iterator s) -> iterator {
				if (i == s) {
					return s;
				}
				if (i == end()) {
					return i;
				}
				// Loop until see s
				while (i != s) {
					i = erase_edge(i);
					if (i == end()) {
            			return end();
        			}
				}
				return i;
			};

			/**
			* Effects: Erases all nodes from the graph.
			* Postconditions: empty() is true.
			*/
			auto clear() noexcept -> void {
				g_.clear();
			};
			
			// Returns: An iterator pointing to the first element in the container.
			[[nodiscard]] auto begin() const -> iterator {
				// auto it = iterator();
				// it = it.begin(this);
				// return it;
				return graph<N, E>::iterator::begin(this);
			};

			/**
			* Returns: An iterator denoting the end of the iterable list 
			* that begin() points to.
			* Remarks: [begin(), end()) shall denote a valid iterable list. 
			*/
			[[nodiscard]] auto end() const -> iterator {
				// auto it = iterator();
				// it = it.end(this);
				// return it;
				return graph<N, E>::iterator::end(this);
			};


			/**
			* Returns: true if a node equivalent to value exists in the graph, and false otherwise.
			* Complexity: O(log n) time.
			*/
	 		[[nodiscard]] auto is_node(N const& value) const noexcept -> bool {
				return g_.find(value) != g_.end();
			};
		
			/**
			 * Returns: true if there are no nodes in the graph, and false otherwise
			 */
			[[nodiscard]] auto empty() const noexcept -> bool {
				return g_.empty();
			};

			/**
			* Returns: true if an edge src → dst exists in the graph, and false otherwise.
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph") 
			* if either of is_node(src) or is_node(dst) are false. [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			*/
			[[nodiscard]] auto is_connected(N const& src, N const& dst) const -> bool {
				// Check src and dst existence first
				if (!is_node(src) or !is_node(dst)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
				}

				return std::any_of(g_.at(src).first.begin(), g_.at(src).first.end(),
					[&dst](const auto& edge_ptr) {
						return edge_ptr->get_nodes().second == dst;
					}
				);
			};

			/**
			* Returns: A sequence of all stored nodes, sorted in ascending order.
			* Complexity: O(n), where n is the number of stored nodes. 
			*/
			[[nodiscard]] auto nodes() const noexcept -> std::vector<N> {
				std::vector<N> nodes_vector;
				nodes_vector.reserve(g_.size());
				std::for_each(g_.begin(), g_.end(),
					[&nodes_vector](const auto& pair) {
						nodes_vector.push_back(pair.first);
					}
				);
				return nodes_vector;
			};

			/**
			* Returns: A sequence of edges from src to dst, start with the unweighted edge (if exists), 
			* then the rest of the weighted edges are sorted in ascending order.
			* 
			* Complexity: O(log n + e), where n is the number of stored nodes and eis the number of stored edges.
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph") if either of is_node(src) or is_node(dst) are false. [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			*/
			[[nodiscard]] auto edges(N const& src, N const& dst) const -> std::vector<std::unique_ptr<edge<N,E>>> {
				// Check src and dst existence first
				if (!is_node(src) or !is_node(dst)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph");
				}
				std::vector<std::unique_ptr<edge<N,E>>> edges_ptr_vector;
				std::for_each(g_.at(src).first.begin(), g_.at(src).first.end(), [&](const auto& edge_ptr) {
					if (edge_ptr->get_nodes().second == dst) {
						if (edge_ptr->is_weighted()) {
							edges_ptr_vector.push_back(std::make_unique<gdwg::weighted_edge<N, E>>(
								edge_ptr->get_nodes().first, edge_ptr->get_nodes().second, edge_ptr->get_weight().value()));
						} else {
							edges_ptr_vector.push_back(std::make_unique<gdwg::unweighted_edge<N, E>>(
								edge_ptr->get_nodes().first, edge_ptr->get_nodes().second));
						}
					}
				});
				return edges_ptr_vector;
			};

			/**
			* Returns: An iterator pointing to an edge equivalent to the specified src, dst, and weight. 
			* If weight is std::nullopt, it searches for an unweighted_edge between src and dst. 
			* If weight has a value, it searches for a weighted_edge between src and dst with the specified weight. Returns end() if no such edge exists.
			* Complexity:  O(log(n) + e), where n is the number of stored nodes and e is the number of edges (either weighted or unweighted) between src and dst.
			* Assume that dst and src given are valid
			*/
			[[nodiscard]] auto find(N const& src, N const& dst, std::optional<E> weight = std::nullopt) const noexcept -> iterator {
				// Define a lambda to compare edges based on their content, not their address
				if (!is_node(src) or !is_node(dst)) {
					return end();
				}
				const auto& edges_ptr_set = g_.find(src)->second.first;
				const auto find_edge_ptr = create_edge_ptr(src, dst, weight);
				const auto edge_content_equal = [&find_edge_ptr](const edge_ptr& e) {
					return *e == *find_edge_ptr;
				};

				auto it = std::find_if(edges_ptr_set.begin(), edges_ptr_set.end(), edge_content_equal);
				if (it == edges_ptr_set.end()) {
					return end();
				} else {
					return iterator(g_.find(src), it , this);
				}
			};

			/**
			* Returns: All nodes (found from any immediate outgoing edge) connected to src, sorted in ascending order. This returns copies of the specified data.
			*
			* Complexity: O(log (n) + e), where e is the number of outgoing edges associated with src.
			* Throws: std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph") if is_node(src) is false. [Note: Unlike Assignment 2, the exception message must be used verbatim. —end note]
			*/
			[[nodiscard]] auto connections(N const& src) const -> std::vector<N> {
				// Check if src exists in the graph
				if (!is_node(src)) {
					throw std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph");
				}

				// Set to store unique destinations
				std::set<N> unique_dests;

				// Iterate through outgoing edges
				std::transform(
					g_.at(src).first.begin(),
					g_.at(src).first.end(),
					std::inserter(unique_dests, unique_dests.end()),
					[](const auto& edge_ptr) { return edge_ptr->get_nodes().second; }
				);

				// Convert set to vector
				return std::vector<N>(unique_dests.begin(), unique_dests.end());
			};

			[[nodiscard]] auto operator==(graph const& other) const -> bool {
				// Compare the size of the two maps first
				if (g_.size() != other.g_.size()) {
					return false;
				}

				// Create 2 iterators one from g and one from other
				auto g_it = begin();
				auto other_it = other.begin();

				// Check nodes
				const auto vec1 = nodes();
				const auto vec2 = other.nodes();
				for (size_t i = 0; i < vec1.size(); ++i) {
					if (vec1.at(i) != vec2.at(i)) {
						return false;  // Return false immediately if a mismatch is found
					}
				}
				 
				// Handle the case when one of the iterators is already at end
				if (g_it == end() and other_it == other.end()) {
					return true;
				}
				if (g_it == end() or other_it == other.end()) {
					return false;
				}

				// Handle other case
				while (g_it != end() and other_it != end()) {
			        // Dereference iterators to get the value_type structs
					auto value1 = *g_it;
					auto value2 = *other_it;

					// Compare the individual fields
					if (value1.from != value2.from or
						value1.to != value2.to or
						value1.weight != value2.weight) {
						return false; // One or more fields are not equal
					}
					++g_it;
        			++other_it;
				}
				// If one iterator is not at end, the graphs are not equal
				return g_it == end() and other_it == other.end();
			};

			/**
			* Effects: Behaves as a formatted output function of os.
			* Returns: os.
			*/
			friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream& {
				for (const auto& [node, edge_sets] : g.g_) {
					os << node << " (\n";
					const auto& outgoing_edges = edge_sets.first;
					std::for_each(outgoing_edges.begin(), outgoing_edges.end(),
								[&os](const auto& edge) {
									os << "  " << edge->print_edge() << "\n";
								});
					os << ")\n";
				}
				return os;
			}

		private:
			// The graph will be a map with key is node
			// for each key, we store a pair of set of edges where
			// the first set is outgoing edges and the second set is incomming edges
			// reflexive edge will be stored on both of the set
			std::map<N, edge_ptr_set_pair> g_;
	};

	template<typename N, typename E>
	class graph<N, E>::iterator {
		private:
			using graph_iterator = typename std::map<N, edge_ptr_set_pair>::const_iterator;
			using edge_iterator = typename edge_ptr_set::const_iterator;
			graph_iterator current_node_;
			edge_iterator current_edge_;
			// Raw graph ptr
			const graph* graph_ptr_;

			// Create a iterator based on given entities
			explicit iterator(graph_iterator node, 
							edge_iterator edge, 
							const graph* g_ptr) : 
			current_node_(node), current_edge_(edge), graph_ptr_(g_ptr) {};

			// Store inside private because we are not required to implement this operator
			// So I do not want accidentally to write my test which include this operator
			// If my test using = operator is ever used against other people code, it will not work !
			auto operator=(iterator const& other) noexcept -> iterator& {
				if (this != &other) {
					current_node_ = other.current_node_;
					current_edge_ = other.current_edge_;
					graph_ptr_ = other.graph_ptr_;
				}
				return *this;
			}
			
			// Public factory method for begin iterator
			static iterator begin(const graph* g) noexcept {
				if (g->g_.empty()) {
					return end(g);
				}
				auto node_it = g->g_.begin();
				while (node_it != g->g_.end() and node_it->second.first.empty()) {
					++node_it;
				}
				if (node_it == g->g_.end()) {
					return end(g);
				}
				return iterator(node_it, node_it->second.first.begin(), g);
			}

			static iterator end(const graph *g) noexcept {
				return iterator(g->g_.end(), edge_iterator(), g);
			}

		public:
			using value_type = struct {
				N from;
				N to;
				std::optional<E> weight;
			};
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			// Iterator constructor
			iterator() : current_node_(), current_edge_(), graph_ptr_(nullptr) {}
			
			iterator(const iterator& other)
				: current_node_(other.current_node_)
				, current_edge_(other.current_edge_)
				, graph_ptr_(other.graph_ptr_) {}

			// Iterator source
			auto operator*() const noexcept -> reference {
				// Dereferencing an end vector case
				// throw error

				const auto& edge = *current_edge_;
				return  value_type {
					// current_node_->first;
					edge->get_nodes().first,
					edge->get_nodes().second,
					edge->is_weighted() ? std::optional<E>{edge->get_weight()} : std::nullopt
				};
			};

			// Iterator traversal
			// Precondition: never add after end
			auto operator++() noexcept -> iterator& {
				++current_edge_;
				if (current_edge_ == current_node_->second.first.end()) {
					do {
						++current_node_;
						if (current_node_ == graph_ptr_->g_.end()) {
							*this = iterator::end(graph_ptr_);
							return *this;
						}
					} while (current_node_->second.first.empty());
					current_edge_ = current_node_->second.first.begin();
				}
				return *this;
			}

			// Precondition: never add after end
			auto operator++(int) noexcept -> iterator {
				auto temp = *this;
				++*this;
				return temp;
			};

			// Precondition: never minus before start
			auto operator--() -> iterator& {
				// Check if we're at the end iterator
				if (*this == iterator::end(graph_ptr_)) {

					if (graph_ptr_->g_.empty()) {
						throw std::out_of_range("Cannot decrement end iterator of an empty graph");
					}

					// Move to the last valid edge
					current_node_ = std::prev(graph_ptr_->g_.end());
					while (current_node_->second.first.empty()) {
						if (current_node_ == graph_ptr_->g_.begin()) {
							throw std::out_of_range("Cannot decrement: all nodes are empty");
						}

						--current_node_;
					}
					current_edge_ = std::prev(current_node_->second.first.end());
					return *this;
				}

				// Check if we're at the beginning of the graph
				if (current_node_ == graph_ptr_->g_.begin() and current_edge_ == current_node_->second.first.begin()) {
					throw std::out_of_range("Cannot decrement iterator before beginning");
				}

				if (current_edge_ == current_node_->second.first.begin()) {
					// Move to the previous node
					do {
						--current_node_;
					} while (current_node_->second.first.empty() and current_node_ != graph_ptr_->g_.begin());

					// Set to the last edge of the non-empty node we found
					current_edge_ = std::prev(current_node_->second.first.end());
				} else {
					--current_edge_;
				}
				return *this;
			}
			
			auto operator--(int) noexcept -> iterator {
				auto temp = *this;
				--*this;
				return temp;
			};

			// Iterator comparison
			auto operator==(iterator const& other) const noexcept -> bool {
				if (graph_ptr_ != other.graph_ptr_) {
					return false;
				}

				// Check if both current_node_ iterators are at end
				if (current_node_ == graph_ptr_->g_.end() and other.current_node_ == other.graph_ptr_->g_.end()) {
					return true;
				}

				// If only one is at end, they're not equal
				if (current_node_ == graph_ptr_->g_.end() or other.current_node_ == other.graph_ptr_->g_.end()) {
					return false;
				}

				// Compare dereferenced current_node_
				if (*current_node_ != *other.current_node_) {
					return false;
				}

				// Check if both current_edge_ iterators are at end
				if (current_edge_ == current_node_->second.first.end() and 
					other.current_edge_ == other.current_node_->second.first.end()) {
					return true;
				}

				// If only one is at end, they're not equal
				if (current_edge_ == current_node_->second.first.end() or 
					other.current_edge_ == other.current_node_->second.first.end()) {
					return false;
				}
				
				// Compare dereferenced current_edge_
				return **current_edge_ == **other.current_edge_;
			};

		friend class graph;
	};

} // namespace gdwg

#endif // GDWG_GRAPH_H
