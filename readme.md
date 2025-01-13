# Data Structure
## Disjoint Set Union
Initially there are n elements indexed from `0` to `n - 1`, each of them are in a set that contains only themselves. Disjoint set union is a data structure that can achieve following functionalities:
1. Merge the two sets that contain element u and v respectively.
2. Split element u from the set that contains it.
3. Find the set that contains element u.

`The merge operation can be weighted as a number r that creates a directed relationship from element u to element v (also from v to u will be -r).`
### methods
|methods | parameters|||description|complexity|
|:---|:---|:---|:---|:---|:---|
|constructor|`uint32_t size = 0`</br>the size of dsu||||O(n)|
|resize|`uint32_t size`</br>the new size of dsu||||O(n)|
|reset||||re-initialize the dsu|O(n)|
|get_father|`uint32_t u`</br>the index of the element|||returns the set which contains element u|O(log(n))|
|merge|`uint32_t u`</br>the set contains element u|`uint32_t v`</br>the set contains element v|`int32_t r = 0`</br>the weight from u to v|merge the two sets that contain element u and v respectively|O(log(n))|
|split|`uint32_t u`</br>the index of the element|||split element u from the set that contains it|O(1)|
### problems
[CF1620E](https://codeforces.com/problemset/problem/1620/E)
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
#include "templates/data_structure/segment_tree.h"
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