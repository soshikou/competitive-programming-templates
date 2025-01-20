# Introduction
This is my personal template collection for online competitive programming contests, and it has absolutely no advantages but massive disadvantages comparing to other gods' library. Specifically, this is trash.

To somehow maybe accelerate the executing(surely failed), all templates in this shit use a self-defined struct Buffer to allocate memories, and there is literally no boundary checks(or any other checks). So it is necessary to make sure that the inputs and callings are vaild. Also, dont forget to set a proper size for Buffer(the default value is 128 megabytes), and reset the Buffer after each testcases unless its size is calculated enough.
# Data Structure
## Disjoint Set Union
Initially there are n elements indexed from `0` to `n - 1`, each of them are in a set that contains only themselves. Disjoint set union is a data structure that can achieve following functionalities:
1. Merge the two sets that contain element u and v respectively.
2. Split element u from the set that contains it.
3. Find the set that contains element u.

`The merge operation can be weighted as a number r that creates a directed relationship from element u to element v (also from v to u will be -r).`
### methods
|methods|parameters|||description|complexity|
|:---|:---|:---|:---|:---|:---|
|constructor|`uint32_t size = 0`</br>the size of dsu||||O(n)|
|reset|`uint32_t n`</br>the right boundary need to be reset|||reset the dsu for indexes in [0, n)|O(n)|
|get_father|`uint32_t u`</br>the index of the element|||returns the set which contains element u|O(log(n))|
|merge|`uint32_t u`</br>the set contains element u|`uint32_t v`</br>the set contains element v|`int32_t r = 0`</br>the weight from u to v|merge the two sets that contain element u and v respectively|O(log(n))|
|split|`uint32_t u`</br>the index of the element|||split element u from the set that contains it|O(1)|
### problems
|||||||||||
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|[CF1620E](https://codeforces.com/problemset/problem/1620/E)|[CF2060E](https://codeforces.com/problemset/problem/2060/E)||||||||
## Segment Tree
Segment tree is a data structure that can achieve following functionalities:
1. Apply some operations to all elements in some intervals of a sequence.
2. Calculate the answers of expressions that include all elements in some intervals. The expressions must satisfy associative law.

`In this context, an interval means an left closed right open interval.`
### methods
#### build
Build the segment tree from a given sequence.
```C++
template <typename T>
void build(T *src, int32_t l, int32_t r);
```
`T *src` is the base sequence on which to build the segment tree.

`int32_t l` indicates the start point of the sequence.

`int32_t r` indicates the end poine of the sequence.

#### update
Apply a update on an interval.
```C++
void update(int32_t l, int32_t r, const LazyType &lazy);
```
`int32_t l` indicates the start point of the interval on which to apply this update.

`int32_t r` indicates the end point of the interval on which to apply this update.

`const LazyType &lazy` describes how this update is applied.
#### query
Make a query on an interval.
```C++
ResponseType query(int32_t l, int32_t r);
```
Returns the answer of the expression that consists of the elements in interval [l, r).

`int32_t l` indicates the start point of the interval.

`int32_t r` indicates the end point of the interval.
### usage
To make use of this template, two structs's implementations as follows are required:
```C++
struct LazyType {	
    // some data members...
    void merge(const LazyType &rhs);
    void init();
};
struct ResponseType {
    // some data members...
    void merge(const LazyType &rhs, int32_t length);
    void merge(const ResponseType &rhs);
    template <typename T>
    void init(T *src, int32_t pos);
};
```
#### LazyType 
Struct LazyType is the lazy tag that will be added to the tree node that has same interval with the update operation after the original interval is distributed along the procedure. Also it is the description of updates.
##### merge
Describe how a newly added lazy tag impacts on the old one.

 `const LazyType &rhs` the newly added lazy tag.
 ##### init
Reset the data in current instance to default value.
#### ResponseType 
Struct ResponseType stores the answers of the queries.
##### merge
Describe how a lazy tag impacts on the answer of the interval.

`const LazyType &rhs` the description of the update, also will be stored as lazy tag attached on the intervals.

`int32_t length` the length of the interval.
##### merge
Describe how to calculate the final answer of the query on an interval from its two subintervals.

`const ResponseType &rhs` the answer of the query on the other subinterval.
##### init
Describe how to initialize the answer of intervals that have a length of 1.

`T *src` the source sequence.

`int32_t pos` the start point of the current interval of length 1.
### examples
Here is an example that implements a segment tree that allows add or multiply a number to all elements in an interval and calculates the sum of all elements in an interval:
```C++
#include <bits/stdc++.h>
#include "competitive-programming-templates/data_structure/segment_tree.h"
using namespace std;

int64_t k;

struct LazyType {
    int64_t m_add;
    int64_t m_mult;
    void merge(const LazyType &rhs) {
        if (rhs.m_mult != 1) {
            (m_mult *= rhs.m_mult) %= k;
            (m_add *= rhs.m_mult) %= k;
        }
        if (rhs.m_add) (m_add += rhs.m_add) %= k;
    }
    void init() { m_add = 0, m_mult = 1; }
};
struct ResponseType {
    int64_t m_sum;
    void merge(const LazyType &rhs, int32_t length) {
        if (rhs.m_mult != 1) (m_sum *= rhs.m_mult) %= k;
        if (rhs.m_add) (m_sum += (rhs.m_add * length)) %= k;
    }
    void merge(const ResponseType &rhs) { (m_sum += rhs.m_sum) %= k; }
    template <typename T>
    void init(T *src, int32_t pos) { m_sum = src[pos]; }
};

Rem::SegmentTree<LazyType, ResponseType> st;
int64_t a[1100000];

int main() {
    nTestCase = 1;
    //cin >> nTestCase;

    for (curTestCase = 1; curTestCase <= nTestCase; curTestCase++) {
        int n, m;
        cin >> n >> m >> k;
        for (int i = 1; i <= n;i++) cin >> a[i];
        st.build(a, 1, n + 1);
        LazyType lz;
        for (int i = 0; i < m;i++){
            int w, x, y, z;
            cin >> w;
            //multiply z to all numbers in interval [x, y + 1).
            if (w == 1) {
                cin >> x >> y >> z;
                lz.m_add = 0, lz.m_mult = z;
                st.update(x, y + 1, lz);
            }
            // add z to all numbers in interval [x, y + 1).
            if (w == 2) {
                cin >> x >> y >> z;
                lz.m_add = z, lz.m_mult = 1;
                st.update(x, y + 1, lz);
            }
            // calculate the sum of all numbers in interval [x, y + 1).
            if (w == 3) {
                cin >> x >> y;
                cout << st.query(x, y + 1).m_sum << endl;
            }
        }
    }
    return 0;
}
```
### problems
## Persistent Segment Tree
Persistent segment tree is such data structure that can achieve following functionalities:
1. Apply some operations to one element of a sequence from a history version, and generate a new version.
2. Calculate the answers of expressions that include all elements in some intervals. The expressions must satisfy associative law.

`In this context, an interval means an left closed right open interval.`
### methods
#### build
Build the persistent segment tree from a given sequence.
```C++
template <typename T>
void build(T *src, int32_t l, int32_t r);
```
`T *src` is the base sequence on which to build the persistent segment tree.

`int32_t l` indicates the start point of the sequence.

`int32_t r` indicates the end poine of the sequence.

#### update
Apply a update on one element.
```C++
void update(int32_t pos, const LazyType &lazy, uint32_t ver = 0);
```
`int32_t pos` indicates the position of the element on which to apply this update.

`const LazyType &lazy` describes how this update is applied.

`uint32_t ver` indicates the version from which to apply this update. The code of versions starts from 0, and increase by 1 after each update.
#### query
Make a query on an interval.
```C++
ResponseType query(int32_t l, int32_t r, uint32_t ver = 0);
```
Returns the answer of the expression that consists of the elements in interval [l, r).

`int32_t l` indicates the start point of the interval.

`int32_t r` indicates the end point of the interval.

`uint32_t ver` indicates the version from which to make this query.
### usage
To make use of this template, two structs's implementations as follows are required:
```C++
struct LazyType {	
    // some data members...
};
struct ResponseType {
    // some data members...
    void merge(const LazyType &rhs, int32_t length);
    void merge(const ResponseType &rhs);
    template <typename T>
    void init(T *src, int32_t pos);
};
```
#### LazyType
There is no lazy tag in persistent segment tree, this struct here is only used as a temporary variable that describes the updates. There is no necessary member under this circumstance.
#### ResponseType 
Struct ResponseType stores the answers of the queries.
##### merge
Describes how to apply an update on one element.

`const LazyType &rhs` the description of the update.

`int32_t length` the length of the interval.
##### merge
Describe how to calculate the final answer of the query on an interval from its two subintervals.

`const ResponseType &rhs` the answer of the query on the other subinterval.
##### init
Describe how to initialize the answer of intervals that have a length of 1.

`T *src` the source sequence.

`int32_t pos` the start point of the current interval of length 1.
### examples
### problems
# Graph
## Graph
Graph is the data structure used to save a 0-indexed graph, all other graph algorithm templates are based on it.
### class template parameters
|parameter|description|
|:---|:---|
|`typename Tv = int32_t`|the type of weight on vertexes|
|`typename Te = int32_t`|the type of weight on edges|
### methods
|methods|parameters|||description|complexity|
|:---|:---|:---|:---|:---|:---|
|constructor|`uint32_t v`</br>the number of vertexes|`uint32_t e`</br>the maximum number of edges|||O(v)|
|reset|||||O(v)|
|[]|`uint32_t i`</br>|||return the weight of vertex i|O(1)|
|size||||returns the number of vertexes|O(1)|
|add_edge|`uint32_t s`</br>the index of start vertex|`uint32_t t`</br>the index of end vertex|`Te w`</br>the weight the edge|add a directed edge from s to t with a weight of w|O(1)|
|freeze||||preprocess after done adding edges|O(v+e)|
|left_boundary|`uint32_t u`</br>the index of relative vertex|||return a pointer to the first edge starting from the vertex|O(1)|
|right_boundary|`uint32_t u`</br>the index of relative vertex|||return a pointer to the last edge starting from the vertex|O(1)|
### usage
1. The call of freeze() is needed after done adding edges.
2. The call of reset() is needed after each test case solved.

To iterate edges starting from vertex u:
```C++
auto lb = g.left_boundary(u), rb = g.right_boundary(u);
for (auto e = lb; e != rb; e++) {
    //e->m_t is the index of end vertex of the edge
    //e->m_w is the weight of the edge
}
```
### problems
|||||||||||
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
|[CF1092F](https://codeforces.com/problemset/problem/1092/F)||||||||||
### wtfs
1. after using `#pragma GCC optimize(2)`, it runs slower.
2. if pre-calculate all boundary pointers for later iteration instead of calculate the pointer each time called (by adding the bias to the start pointer), it runs slower.
3. placement new is faster than set all members manually.
# math
## functions
|function|parameters||||description|complexity|
|:---|:---|:---|:---|:---|:---|:---|
|euclid|`T a`|`T b`|||return the greatest common divisor of `a` and `b`|O(log(n))|
|extended_euclid|`T a`|`T b`|`T &x`|`T &y`|return the greatest common divisor `c` of `a` and `b`. Also set proper value to `x` and `y` that satisfy `ax + by = c`.|O(log(n))|
|exponentiation|`T n`|`T k`|||return `n ** k`|O(log(k))|
|exponentiation|`T n`|`T k`|`T p`||return `(n ** k) mod p`|O(log(k))|
|factorize|`T n`|`std::vector<std::pair<T, T>> &f`|||return `n`'s factors along with their expontent|O(sqrt(n))|
|get_divisor|`T n`||||get one divisor of `n`||
|get_divisors|`T n`|`std::vector<T> &r`|||get divisors of `n`|O(sqrt(n))|
|number_of_divisors|`T n`||||return number of divisors of `n`|O(sqrt(n))|
|sum_of_divisors|`T n`||||return number of divisors of `n`||
|euler|`T n`||||euler's totient function returns the number of `x` that is coprime with `n` from `1` to `n - 1`||
|euler|`T n`|`std::vector<T> &e`|||calculate euler's totient function for all numbers from `1` to `n`||
|primitive_root|`T n`||||get one primitive root of `n`, return `-1` if there is none.||
|primitive_root|`T n`||||get all primitive roots of `n`||
## modint
### methods
Let the current value be `x`, and current modulo be `p`.
|methods|parameters||description|complexity|
|:---|:---|:---|:---|:---|
|constructor|`T x = 0`|||O(1)|
|set_modulo|`T np`||set modulo|O(1)|
|get_modulo|||return the modulo|O(1)|
|val|||return the current value|O(1)|
|inverse|||return `y` that satisfies `x * y(mod p) = 1` when `x` and `p` are coprime.|O(log(p))|
|exponentiation|`T b`||return `x ** b` |O(log(b))|
|logarithm|`T a`||return `y` that satisfies `a ** y = x`||
|root|`T k`|`std::vector<T> &r`|return all `y` that satisfy `y ** k = x`||
# util
## Buffer
Buffer is used to manage memories.
### macros
`REM_MAX_BUFFER_SIZE`: defines the size(megabytes, default value is 128) of the Buffer.
### methods
|methods|parameters|description|complexity|
|:---|:---|:---|:---|
|reset|||O(1)|
|allocate<typename T>||return a pointer to the element|O(1)|
|allocate<typename T>|`uint32_t n`</br>the number of elements to allocate|return a pointer to the first element|O(1)|
|status||return the amount(megabytes) of memories used|O(1)|
