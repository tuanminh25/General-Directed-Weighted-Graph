<h1 class="title" style="text-align:center; font-weight:bold">General Directed Weighted Graph</h1>

## **Overview**  
This project implements a **flexible and efficient graph representation** with an emphasis on **modularity and extendability**. It provides a well-defined **edge class hierarchy** supporting both **weighted and unweighted edges**, along with comprehensive **access and modification methods**. The project also includes an **iterator interface** for traversing the graph structure and ensures **const-correctness** for safe and predictable usage.

## **Features**  
- **Graph Representation**: Implements a structured approach to defining graphs, including **constructors, accessors, and modifiers**.  
- **Edge Class Hierarchy**:  
  - **`edge`**: Base class for all edges.  
  - **`weighted_edge`**: Extends `edge` to support weights.  
  - **`unweighted_edge`**: Extends `edge` but support no weights.   
- **Iterator Support**:  
  - Provides **iterator constructors, traversal methods, and comparison operators** for efficient graph traversal.  
- **Comparison & Extraction**:  
  - Defines **comparison operators** for edges and iterators.  
  - Implements **extractor methods** for retrieving edge properties.  
- **Const-Correctness**:  
  - Adheres to **const-correct programming principles** to ensure immutability where required.  

## **Project Structure**  
- **Change Log** – Tracks updates and modifications.  
- **The Task** – General project description and requirements.  
  - **Definitions** – Key terms and structures used in the implementation.  
  - **Constructors** – Methods for creating instances of the graph and its components.  
- **Edge Class Hierarchy** – Organized edge structures supporting different types.  
- **Graph Interface**  
  - **Modifiers** – Functions to modify graph properties and structure.  
  - **Accessors** – Methods to retrieve graph information.  
  - **Iterator Access** – Mechanisms for iterating over graph elements.  
  - **Comparisons** – Operators for comparing different graph components.  
  - **Extractors** – Functions to extract details from graph objects.  
- **Iterator Implementation** – Handles traversal and element access in the graph.  
- **Compulsory Internal Representation** – Defines the internal data structure requirements.  
- **Other Notes**  
  - **Const-Correctness** – Enforces safe and efficient usage of `const` where applicable.  

## **Usage**  
This project serves as a foundation for working with **graph structures**, making it useful for applications in **network analysis, route optimization, and algorithm development**.  


# 1 Change Log <a name="1-change-log"></a>

- 17/07/2024 Correct statement of unweighted edge, fix typo in `insert_edge` 
- 18/07/2024 Clarification on order of unweighted/weighted edges
- 18/07/2024 Explicit assumption for *N* and *E*
- 20/07/2024 Member functions `nodes()`, `edges()` and `connections()` return copied data
- 20/07/2024 Smart pointers now not required in internal representation
- 21/07/2024 Correct return type of `edges()` & `edge` not required for internal representation in `gdwg::graph`
- 21/07/2024 Added `operator==` for `edge` & clearer requirement of `unweighted_edge`, `weighted_edge`'s constructor to be `public`
- 21/07/2024 Starter code: Get rid of `edge` type alias & Fix outdated code in `client.cpp`
- 21/07/2024 Remove lexicographic in describing order `nodes`, `edges` & correct expression on order
- 22/07/2024 Correct `print_edge` output to be single space
- 23/07/2024 Correct output in 2.8 extractor & fix wrong expression in `insert_edge`
- 23/07/2024 Change `edge` class pure virtual function requirement to use them when appropriate.
- 23/07/2024 Clarification on `edge` equality
- 24/07/2024 clarification on `edge` member functions can be not `virtual`; derived classes `unweighted_edge`, `weighted_edge` from `edge` can store member parameters as `non-private`; add argument order for derived classes
- 26/07/2024 `print_edge` not required in `operator<<` for efficiency purpose
- 26/07/2024 Fix error in `client.cpp` & extractor output
- 26/07/2024 Remove note for modifier: not required to handle edges
- 26/07/2024 Clarification on template typename order of `edge`
- 26/07/2024 Allowed to add non-member operator

# 2 The Task

I will implement a generic directed weighted graph (GDWG) with value-semantics in C++. Node data and edge weights are parameterized types (e.g., `gdwg::graph<std::string, int>`).  Nodes and edges are assumed copyable, comparable, streamable, and hashable.

Nodes are unique.  Incoming/outgoing edges and in/out-degree are defined as usual. Graphs can contain both weighted and unweighted edges (at most one unweighted edge per source/destination pair).

Edges are ordered by source, destination, and then weight (for weighted edges). Unweighted edges precede weighted edges with the same source/destination. Edges with equal source, destination, and weight are equal.

Dynamic polymorphism is used for the `edge` class hierarchy:

*   `edge`: Abstract base class.
*   `weighted_edge`: Represents a weighted edge.
*   `unweighted_edge`: Represents an unweighted edge.

Edges can be reflexive. The graph is multi-edged (multiple edges between the same source/destination with *different* weights allowed).

# 2.1 Definitions

Some words have special meaning in this document. This section precisely defines those words.

* *Preconditions*: the conditions that the function assumes to hold whenever it is called; violation of any preconditions results in undefined behaviours.
* *Effects*: the actions performed by the function.
* *Postconditions*: the conditions (sometimes termed observable results) established by the function.
* *Returns*: a description of the value(s) returned by the function.
* *Throws*: any exceptions thrown by the function, and the conditions that would cause the exception.
* *Complexity*: the time and/or space complexity of the function.
* *Remarks*: additional semantic constraints on the function.
* *Unspecified*: the implementation is allowed to make its own decisions regarding what is unspecified, provided that it still follows the explicitly specified wording.
* An *Effects* element may specify semantics for a function `F` in code using the term *Equivalent to*. The semantics for `F` are interpreted as follows:
  * All of the above terminology applies to the provided code, whether or not it is explicitly specified.
  * If there is not a *Returns* element, and `F` has a non-`void` return type, all the return statements are in the code block.
  * *Throws*, *Postconditions*, and *Complexity* elements always have priority over the code block.
* Specified complexity requirements are upper bounds, and implementations that provide better complexity guarantees meet the requirements.
* The class synopsis is the minimum text your header requires to compile most tests (this doesn’t mean that it will necessarily link or run as expected).
* Blue text in code will link to C++ Reference or to another part of this document.
* This section makes use of [stable.names]. A stable name is a short name for a (sub)section, and isn’t supposed to change. We will use these to reference specific sections of the document.

# 2.2 Constructors 

**It’s very important your constructors work. If we can’t validly construct your objects, we can’t test any of your other functions.**

```cpp
graph();
```

1. *Effects*: <a href="https://en.cppreference.com/w/cpp/language/value_initialization">Value initialises</a> all members.

2. *Throws*: Nothing.

```cpp
graph(std::initializer_list<N> il);
```
3. *Effects*: Equivalent to: `graph(il.begin(), il.end());`

```cpp
template<typename InputIt>
graph(InputIt first, InputIt last);
```
4. *Preconditions*: Type `InputIt` models *<a href="https://en.cppreference.com/w/cpp/named_req/InputIterator">Cpp17 Input Iterator</a>* and is indirectly readable as type `N`.

5. *Effects*: Initialises the graph’s node collection with the range `[first, last)`.
```cpp
graph(graph&& other) noexcept;
```
6. *Postconditions*:
  `*this` is equal to the value `other` had before this constructor’s invocation.
  `other.empty()` is `true`.
  All iterators pointing to elements owned by `*this` prior to this constructor’s invocation are invalidated.
  All iterators pointing to elements owned by `other` prior to this constructor’s invocation remain valid, but now point to the elements owned by `*this`.
```cpp
auto operator=(graph&& other) noexcept -> graph&;
```
7. *Effects*: All existing nodes and edges are either move-assigned to, or are destroyed.

8. *Postconditions*:
    * `*this` is equal to the value `other` had before this operator’s invocation.
    * `other.empty()` is `true`.
    * All iterators pointing to elements owned by `*this` prior to this operator’s invocation are invalidated.
    * All iterators pointing to elements owned by `other` prior to this operator’s invocation remain valid, but now point to the elements owned by `*this`.

9. *Returns*: `*this`.

```cpp
graph(graph const& other);
```
10. *Postconditions*: `*this == other` is `true`.

```cpp
auto operator=(graph const& other) -> graph&;
```
11. *Postconditions*:
    * `*this == other` is `true`.
    * All iterators pointing to elements owned by `*this` prior to this operator’s invocation are invalidated.
    * *Returns*: `*this`.

# 2.3 Edge Class Hierachy

## 2.3.1 edge

The `edge` class should be an abstract **BASE** class, which means at least **one** of its member functions must be declared as **pure** virtual. `edge` class takes template typenames in the same order as `graph` class.

[Note: Of the member functions you are required to implement for edge, there is at least one natural candidate which should be pure virtual. You should consider declaring a function as **pure** virtual when its behavior must be implemented differently by each derived class. —end note] 

You will note that **ONLY** the member functions listed below can be specified as public in edge or its derived classes. You are free to create other private functions to help with the implementation of the derived classes and the features required for `gdwg::graph`.

NOTE: We didn't specify the keywords for functions such as `const`, `virtual`, `override`, or `noexcept`, this is intentional. You should use them where appropriate.


```cpp
auto print_edge() -> std::string;
```
  1. *Effects*: Returns a string representation of the edge.
  2. *Returns*: A string representation of the edge.
  3. *Remarks*: The format of the string representation is `src -> dst | W | weight` if the edge is weighted, and `src -> dst | U` if the edge is unweighted, there should not be a newline character `\n` in the end.
  * Note: `print_edge` can be used in the `operator<<` overload for the `graph` class.
```cpp
auto is_weighted() -> bool;
```
  4. *Effects*: identify whether the edge is weighted.
  5. *Returns*: `true` if the edge is weighted, and `false` otherwise.

```cpp
auto get_weight() -> std::optional<E>;
```
  7. *Effects*: Returns the weight of the edge, `std::nullopt` if the edge is unweighted.
  8. *Returns*: The weight of the edge.

```cpp
auto get_nodes() -> std::pair<N, N>;
```
  9. *Effects*: Returns the source and destination nodes of the edge.
  10. *Returns*: A pair of the source and destination nodes of the edge.

```cpp
auto operator==(edge const& other) -> bool;
```
  11. *Returns*: Returns true if two edges are equal, false otherwise

**As a polymorphic base class, `edge` should also have a public virtual destructor.**

## 2.3.2 weighted_edge

* The `weighted_edge` class should have a **public** constructor that takes the `src`, `dst` node, and `weight` as parameters and initialises the corresponding member variables.

```cpp
weighted_edge(N const& src, N const& dst, E const& weight);
```

* The `weighted_edge` class inherits from `edge` and represents a weighted edge in the graph.

* It **MUST** implement the `edge` class’s virtual functions to provide appropriate funtionality for weighted edges.



## 2.3.3 unweighted_edge

* Similar to the `weighted_edge` class, the `unweighted_edge` class should have a **public**
constructor that takes the `src` node and `dst` node as parameters and initialises the corresponding member variables.

```cpp
unweighted_edge(N const& src, N const& dst);
```

* The `unweighted_edge` class inherits from `edge` and represents an unweighted edge in the graph.

* It **MUST** implement the `edge` class’s virtual functions to provide appropriate functionality for unweighted edges.

# 2.4 Modifiers

```cpp
auto insert_node(N const& value) -> bool;
```
1. *Effects*: Adds a new node with value `value` to the graph if, and only if, there is no node equivalent to `value` already stored.

2. *Postconditions*: All iterators are invalidated.

3. *Returns*: `true` if the node is added to the graph and `false` otherwise.

```cpp
auto insert_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool;
```

4. *Effects*: Adds a new edge representing `src` → `dst` with an optional `weight`. If weight is `std::nullopt`, an `unweighted_edge` is created. Otherwise, a `weighted_edge` with the specified weight is created. The edge is only added if there is no existing edge between `src` and `dst` with the same weight.

    [*Note*:⁠ Nodes are allowed to be connected to themselves. —*end note*]

5. *Postconditions*: All iterators are invalidated.

6. *Returns*: `true` if the edge is added to the graph and `false` otherwise.

7. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does not exist")` if either of `is_node(src)` or `is_node(dst)` are `false`.

    [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

```cpp
auto replace_node(N const& old_data, N const& new_data) -> bool;
```

8. *Effects*: Replaces the original data, `old_data`, stored at this particular node by the replacement data, `new_data`. Does nothing if `new_data` already exists as a node.

9. *Postconditions*: All iterators are invalidated.

10. *Returns*: `false` if a node that contains value `new_data` already exists and `true` otherwise.

11. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist")` if `is_node(old_data)` is `false`.

    [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

```cpp
auto merge_replace_node(N const& old_data, N const& new_data) -> void;
```

12. *Effects*: The node equivalent to `old_data` in the graph are replaced with instances of `new_data`. After completing, every incoming and outgoing edge of `old_data` becomes an incoming/ougoing edge of `new_data`, except that duplicate edges shall be removed.

13. *Postconditions*: All iterators are invalidated.

14. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they don't exist in the graph")` if either of `is_node(old_data)` or `is_node(new_data)` are `false`.

    [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

15. *Note*: The following examples use the format <span class="math inline">(*N*<sub>*s**r**c*</sub>, *N*<sub>*d**s**t*</sub>, *E*)</span>.

    Basic example.
  	* Operation: `merge_replace_node(A, B)`
    * Graph before: <span class="math inline">(*A*, *B*, 1), (*A*, *C*, 2), (*A*, *D*, 3)</span>
    * Graph after : <span class="math inline">(*B*, *B*, 1), (*B*, *C*, 2), (*B*, *D*, 3)</span>


    Duplicate edge removed example.
    * Operation: `merge_replace_node(A, B)`
    * Graph before: <span class="math inline">(*A*, *B*, 1), (*A*, *C*, 2), (*A*, *D*, 3), (*B*, *B*, 1)</span>
    * Graph after : <span class="math inline">(*B*, *B*, 1), (*B*, *C*, 2), (*B*, *D*, 3)</span>
    —*end example*]

    Diagrammatic example.

<img src="https://i.imgur.com/gCDHqrD.png" />

<p><br/></p>

```cpp
auto erase_node(N const& value) -> bool;
```

16. *Effects*: Erases node equivalent to `value`, including all incoming and outgoing edges.

17. *Returns*: `true` if `value` was removed; `false` otherwise.
18. *Postconditions*: All iterators are invalidated.

```cpp
auto erase_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool;
```

20. *Effects*: Erases the edge representing `src` → `dst` with the specified weight. If weight is `std::nullopt`, it erases the `unweighted_edge` between `src` and `dst`. If weight has a value, it erases the weighted_edge between src and dst with the specified weight.
21. *Returns*: `true` if an edge was removed; `false` otherwise.
22. *Postconditions*: All iterators are invalidated.
23. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist in the graph")` if either `is_node(src)` or `is_node(dst)` is `false`. [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]
24. *Complexity*: *O*(log( *n*) + *e*), where *n* is the total number of stored nodes and *e* is the total number of stored edges.

```cpp
auto erase_edge(iterator i) -> iterator;
```

25. *Effects*: Erases the edge pointed to by `i`.
26. *Complexity*: *O*(log (*n*) + *e*), where *n* is the total number of stored nodes and *e* is the total number of stored edges. [*Note*: This complexity requirement is slightly weaker than a real-world container to help make the assignment easier. —*end note*]
27. *Returns*: An iterator pointing to the element immediately after `i` prior to the element being erased. If no such element exists, returns `end()`.
28. *Postconditions*: All iterators are invalidated. [*Note*: The postcondition is slightly stricter than a real-world container to help make the assignment easier (i.e. we won’t be testing any iterators post-erasure). —*end note*]

```cpp
auto erase_edge(iterator i, iterator s) -> iterator;
```

29. *Effects*: Erases all edges between the iterators `[i, s)`.

30. *Complexity* *O*(*d*(log( *n*) + *e*)), where *d* = `std::distance(i, s)`. [*Note*: This complexity requirement is slightly weaker than a real-world container to help make the assignment easier. —*end note*]

31. *Returns*: An iterator equivalent to `s` prior to the items iterated through being erased. If no such element exists, returns `end()`.

32. *Postconditions*: All iterators are invalidated. [*Note*: The postcondition is slightly stricter than a real-world container to help make the assignment easier (i.e. we won’t be testing any iterators post-erasure). —*end note*]

```cpp
auto clear() noexcept -> void;
```

33. *Effects*: Erases all nodes from the graph.
34. *Postconditions*: `empty()` is `true`.

# 2.5 Accessors

```cpp
[[nodiscard]] auto is_node(N const& value) -> bool;
```

1. *Returns*: `true` if a node equivalent to `value` exists in the graph, and `false` otherwise.

2. *Complexity*: *O*(log *n*) time.

```cpp
[[nodiscard]] auto empty() -> bool;
```

3. *Returns*: `true` if there are no nodes in the graph, and `false` otherwise.

```cpp
[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool;
```

4. *Returns*: `true` if an edge `src` → `dst` exists in the graph, and `false` otherwise.
5. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph")` if either of `is_node(src)` or `is_node(dst)` are `false`. [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

```cpp
[[nodiscard]] auto nodes() -> std::vector<N>;
```

6. *Returns*: A vector of all stored nodes, sorted in ascending order. This returns **copies** of the specified data.

7. *Complexity*: *O*(*n*), where *n* is the number of stored nodes.

```cpp
[[nodiscard]] auto edges(N const& src, N const& dst) -> std::vector<std::unique_ptr<edge<N,E>>>;
```

8. *Returns*: All edges from `src` to `dst`, start with the unweighted edge (if exists), then the rest of the weighted edges are sorted in ascending order by edge weights. This returns **copies** of the specified data.

9. *Complexity*: *O*(log( *n*) + *e*), where *n* is the number of stored nodes and *e*is the number of stored edges.

10. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the graph")` if either of `is_node(src)` or `is_node(dst)` are `false`. [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

```cpp
[[nodiscard]] auto find(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> iterator;
```

11. *Returns*: An iterator pointing to an edge equivalent to the specified `src`, `dst`, and `weight`. If weight is `std::nullopt`, it searches for an `unweighted_edge` between `src` and `dst`. If weight has a value, it searches for a `weighted_edge` between `src` and `dst` with the specified weight. Returns `end()` if no such edge exists.
12. *Complexity*:  O(log(*n*) + *e*), where n is the number of stored nodes and e is the number of edges (either weighted or unweighted) between src and dst.

```cpp
[[nodiscard]] auto connections(N const& src) -> std::vector<N>;
```

13. *Returns*: All nodes (found from any immediate outgoing edge) connected to `src`, sorted in ascending order. This returns **copies** of the specified data.

14. *Complexity*: *O*(log (*n*) + *e*), where *e* is the number of outgoing edges associated with `src`.

15. *Throws*: `std::runtime_error("Cannot call gdwg::graph<N, E>::connections if src doesn't exist in the graph")` if `is_node(src)` is `false`. [*Note*: Unlike Assignment 2, the exception message must be used verbatim. —*end note*]

# 2.6 Iterator Access

```cpp
[[nodiscard]] auto begin() const -> iterator;
```

1. *Returns*: An iterator pointing to the first element in the container.

```cpp
[[nodiscard]] auto end() const -> iterator;
```

2. *Returns*: An iterator denoting the end of the iterable list that `begin()` points to.
3. *Remarks*: `[begin(), end())` shall denote a valid iterable list.

# 2.7 Comparisons

```cpp
[[nodiscard]] auto operator==(graph const& other) -> bool;
```

1. *Returns*: `true` if `*this` and `other` contain exactly the same nodes and edges, and `false` otherwise.

2. *Complexity*: *O*(*n* + *e*) where *n* is the sum of stored nodes in `*this` and `other`, and *e* is the sum of stored edges in `*this` and `other`.

# 2.8 Extractor

```cpp
friend auto operator<<(std::ostream& os, graph const& g) -> std::ostream&;
```

1. *Effects*: Behaves as a <a href="https://en.cppreference.com/w/cpp/named_req/FormattedOutputFunction">formatted output function</a> of `os`.

2. *Returns*: `os`.

3. *Remarks*: The format is specified thusly:

<blockquote>
[source_node<sub>1</sub>] [edges<sub>1</sub>] <br/>
[source_node<sub>2</sub>] [edges<sub>2</sub>] <br/>
... <br/>
[source_node<sub>n</sub>] [edges<sub>n</sub>] <br/>
</blockquote>

<p>[source_node<sub>1</sub>], …, [source_node<sub>n</sub>] are placeholders for all nodes that the graph stores, sorted in ascending order. [edges<sub>1</sub>], …, [edges<sub>n</sub>] are placeholders for the below output format:</p>

<blockquote>
( <br />
  [node<sub>1</sub> -> node<sub>1</sub>] | U<br />
  [node<sub>n</sub> -> node<sub>1</sub>] | W | [weight] <br />
  [node<sub>n</sub> -> node<sub>2</sub>] | W | [weight] <br />
  ... <br />
  [node<sub>n</sub> -> node<sub>n</sub>] | W | [weight] <br />
)
</blockquote>

<p>where <code>[node<span class="math inline"><sub>n</sub></span> -> node<span class="math inline"><sub>1</sub></span>] | U</code>, …, <code>[node<span class="math inline"><sub>n</sub></span> -> node<span class="math inline"><sub>n</sub></span>] | W | [weight]</code> are placeholders for each node’s connections edge type and the corresponding weight(if exists) the edge has, always start with unweighted edges, then weighted edges, sorted in ascending order separately.</p>

[*Note*: If a node doesn’t have any connections, the corresponding field will be represented as a line-separated pair of parentheses —*end note*]

[*Example*:
```cpp
using graph = gdwg::graph<int, int>;
auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
    {4, 1, -4},
    {3, 2, 2},
    {2, 4, std::nullopt},
    {2, 4, 2},
    {2, 1, 1},
    {4, 1, std::nullopt},
    {6, 2, 5},
    {6, 3, 10},
    {1, 5, -1},
    {3, 6, -8},
    {4, 5, 3},
    {5, 2, std::nullopt},
};

auto g = graph{};
for (const auto& [from, to, weight] : v) {
    g.insert_node(from);
    g.insert_node(to);
    if (weight.has_value()) {
        g.insert_edge(from, to, weight.value());
    } else {
        g.insert_edge(from, to);
    }
}
g.insert_node(64);

auto out = std::ostringstream{};
out << g;
auto const expected_output = std::string_view(R"(
1 (
  1 -> 5 | W | -1
)
2 (
  2 -> 1 | W | 1
  2 -> 4 | U
  2 -> 4 | W | 2
)
3 (
  3 -> 2 | W | 2
  3 -> 6 | W | -8
)
4 (
  4 -> 1 | U
  4 -> 1 | W | -4
  4 -> 5 | W | 3
)
5 (
  5 -> 2 | U
)
6 (
  6 -> 2 | W | 5
  6 -> 3 | W | 10
)
64 (
)
)");
CHECK(out.str() == expected_output);
```

<p>—*end example* ]</p>

# 2.9 Iterator

Note that the `iterator` has a different value type compared with `gdwg::edge`. 

```cpp
template<typename N, typename E>
class graph<N, E>::iterator {
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
  iterator() = default;

  // Iterator source
  auto operator*() -> reference;

  // Iterator traversal
  auto operator++() -> iterator&;
  auto operator++(int) -> iterator;
  auto operator--() -> iterator&;
  auto operator--(int) -> iterator;

  // Iterator comparison
  auto operator==(iterator const& other) -> bool;
private:
  explicit iterator(unspecified);
};
```

1. Nodes without any connections are not traversed.
2. [*Note*: `gdwg::graph<N, E>::iterator` models <a href="https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator">`std::bidirectional_iterator`</a>. —*end note*]

## 2.9.1 Iterator constructor

```cpp
iterator();
```

1. *Effects*: Value-initialises all members.
2. *Remarks*: Pursuant to the requirements of <a href="https://en.cppreference.com/w/cpp/iterator/forward_iterator">`std::forward_iterator`</a>, two value-initialised iterators shall compare equal.

```cpp
explicit iterator(unspecified);
```

3. *Effects*: Constructs an iterator to a specific element in the graph.
4. *Remarks*: There may be multiple constructors with a non-zero number of parameters.

## 2.9.2 Iterator source

```cpp
auto operator*() -> reference;
```

1. *Effects*: Returns the current `from`, `to`, and `weight`.

## 2.9.3 Iterator traversal

```cpp
auto operator++() -> iterator&;
```
1. *Effects*: Advances `*this` to the next element in the iterable list.

[*Example*: In this way, your iterator will iterate through a graph like the one below producing the following tuple values when deferenced each time:

```cpp
gdwg::graph<int, int>
```

<p><img src="https://qph.fs.quoracdn.net/main-qimg-2ea8bf9286505bf2ccd63893e05eb5f9" /></p>

```
(1, 7, 4)
(1, 12, 3)
(1, 21, 12)
(7, 21, 13)
(12, 19, 16)
(14, 14, 0)
(19, 1, 3)
(19, 21, 2)
(21, 14, 23)
(21, 31, 14)
```

<p>—*end example*]</p>

2. *Returns*: `*this`.

```cpp
auto operator++(int) -> iterator;
```

3. *Effects*: Equivalent to:

```cpp
auto temp = *this;
++*this;
return temp;
```

```cpp
auto operator--() -> iterator&;
```
4. *Effects*: Advances `*this` to the previous element in the iterable list.
5. *Returns*: `*this`.

```cpp
auto operator--(int) -> iterator;
```

6. *Effects*: Equivalent to:

```cpp
auto temp = *this;
--*this;
return temp;
```

## 2.9.4 Iterator comparison

```cpp
auto operator==(iterator const& other) -> bool;
```

1. *Returns*: `true` if `*this` and `other` are pointing to the same elements in the same iterable list, and `false` otherwise.

# 2.10 Compulsory internal representation

Your graph is **required** to own the resources (nodes and edge weights) that are passed in through the insert functions. This means creating memory on the heap and doing a proper copy of the values from the caller. This is because resources in your graph should outlive the caller’s resouce that was passed in in case it goes out of scope. For example, we want the following code to be valid.

```cpp
auto main() -> int {
  gdwg::graph<std::string, int> g;
  {
    std::string s1{"Hello"};
    g.insert_node(s1);
  }

  // Even though s1 has gone out of scope, g has its own
  //  copied resource that it has stored, so the node
  //  will still be in here.
  std::cout << g.is_node("Hello") << "\n"; // prints 'true';
}
```

Your graph must not internally store redundant copies of nodes or edges. You may wish to use smart pointers (e.g. std::unique_ptr and std::shared_ptr) to achieve this, but you are not required to.

1. For each node, you are only allowed to have one underlying resource (heap) stored in your graph for it.

2. For each edge, you are not required to use `edge` for internal representation, however no matter it's weighted or unweighted, you should avoid using unnecessary additional memory wherever possible.

# 2.11 Other notes

You must:
* Include a header guard in `src/gdwg_graph.h`
* Use C++20 style and methods where appropriate
* Make sure that *all appropriate member functions* are `const`-qualified
* Leave a moved-from object in a state with no nodes.
* Implement this class within the namespace `gdwg`.
* **do not** implement an overload for `operator!=` in Assignment 3.

You must **NOT**:

* Write to any files that aren’t provided in the repo (e.g. storing your vector data in an auxilliary file)
* Add additional members to the <b style="color: red;">public</b> interface. The **only** exception is you can add friend non-member operators


You:

* Should try to mark member functions that will not throw exceptions with `noexcept`
* Are not required to make any member function `explicit` unless directly asked to in the spec.

## 2.11.1 `const`-correctness [const.correctness]<a name="2.11.1-const-correctness-constcorrectness"></a>

We have deliberately removed the `const`-qualifiers for most member functions from the specification. **You are required to work out which functions must be `const`-qualified.** You must ensure that each operator and member function appropriately either has:

* A `const` member function; or
* A non-`const` member function; or
* Both a `const` and non-const member function

Please think carefully about this. The function declarations intentionally do not specify their constness, except for the `begin()` and `end()` member functions. These are `const`-qualified to help you out.

In most cases you will only need a single function in the overload set.

As noted in <a href="#29-compulsory-internal-representation-gdwginternal">the compulsory internal representation</a> section, you are unlikely to want to use this directly within your representation. However, it is used by the `iterator` type, and is good practice to have for a container.
