/**
 * Problema: BST implícito (CS3014 - Proyecto 3)
 * Estructura: Implicit Treap con Lazy Propagation y Pool Estático
 * Complejidad Temporal: O(n) construcción inicial, O(log n) esperado por consulta
 * Complejidad Espacial: O(n + q)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdint>

using namespace std;

const int MAX_NODES = 1000005; // Soporta n + q <= 10^6 nodos

struct Node {
    long long val;       // Valor actual del elemento
    long long mn;        // Mínimo en el subárbol
    long long lazy_add;  // Suma diferida (lazy) para el subárbol
    uint32_t prio;       // Prioridad aleatoria del Treap
    int l, r;            // Índices de los hijos izquierdo y derecho (0 = nulo)
    int sz;              // Cantidad de nodos en el subárbol
    bool rev;            // Flag de reversión diferida (lazy)
};

Node tree[MAX_NODES];
int node_cnt = 0;
int root = 0;

mt19937 rng(1337); // Generador de números pseudoaleatorios de alta velocidad

// Crea un nuevo nodo en el pool estático en O(1)
int new_node(long long v) {
    ++node_cnt;
    tree[node_cnt].val = v;
    tree[node_cnt].mn = v;
    tree[node_cnt].lazy_add = 0;
    tree[node_cnt].prio = rng();
    tree[node_cnt].l = 0;
    tree[node_cnt].r = 0;
    tree[node_cnt].sz = 1;
    tree[node_cnt].rev = false;
    return node_cnt;
}

inline int get_sz(int t) {
    return t ? tree[t].sz : 0;
}

// Aplica suma diferida al nodo t en O(1)
inline void apply_add(int t, long long x) {
    if (!t || x == 0) return;
    tree[t].val += x;
    tree[t].mn += x;
    tree[t].lazy_add += x;
}

// Aplica reversión diferida al nodo t en O(1)
inline void apply_rev(int t) {
    if (!t) return;
    tree[t].rev ^= 1;
    swap(tree[t].l, tree[t].r);
}

// Propaga las etiquetas lazy hacia los hijos antes de descender
inline void push(int t) {
    if (!t) return;
    if (tree[t].rev) {
        apply_rev(tree[t].l);
        apply_rev(tree[t].r);
        tree[t].rev = false;
    }
    if (tree[t].lazy_add != 0) {
        apply_add(tree[t].l, tree[t].lazy_add);
        apply_add(tree[t].r, tree[t].lazy_add);
        tree[t].lazy_add = 0;
    }
}

// Recalcula el tamaño y el mínimo del subárbol desde los hijos
inline void pull(int t) {
    if (!t) return;
    tree[t].sz = 1;
    tree[t].mn = tree[t].val;
    if (tree[t].l) {
        tree[t].sz += tree[tree[t].l].sz;
        tree[t].mn = min(tree[t].mn, tree[tree[t].l].mn);
    }
    if (tree[t].r) {
        tree[t].sz += tree[tree[t].r].sz;
        tree[t].mn = min(tree[t].mn, tree[tree[t].r].mn);
    }
}

// Divide el Treap 't' en 'l' (primeros k elementos) y 'r' (el resto)
void split(int t, int k, int &l, int &r) {
    if (!t) {
        l = r = 0;
        return;
    }
    push(t);
    int left_sz = get_sz(tree[t].l);
    if (left_sz >= k) {
        split(tree[t].l, k, l, tree[t].l);
        r = t;
    } else {
        split(tree[t].r, k - left_sz - 1, tree[t].r, r);
        l = t;
    }
    pull(t);
}

// Une dos Treaps 'l' y 'r' asumiendo que todas las posiciones de 'l' van antes que 'r'
int merge(int l, int r) {
    if (!l || !r) return l ? l : r;
    if (tree[l].prio > tree[r].prio) {
        push(l);
        tree[l].r = merge(tree[l].r, r);
        pull(l);
        return l;
    } else {
        push(r);
        tree[r].l = merge(l, tree[r].l);
        pull(r);
        return r;
    }
}

// Operación A l r x: Suma x a todos los elementos en el rango [l, r]
void range_add(int l, int r, long long x) {
    int t1, t2, t3;
    split(root, r, t2, t3);
    split(t2, l - 1, t1, t2);
    apply_add(t2, x);
    root = merge(merge(t1, t2), t3);
}

// Operación R l r: Revierte el rango de posiciones [l, r]
void range_reverse(int l, int r) {
    int t1, t2, t3;
    split(root, r, t2, t3);
    split(t2, l - 1, t1, t2);
    apply_rev(t2);
    root = merge(merge(t1, t2), t3);
}

// Operación O l r k: Rota hacia la derecha el rango [l, r] exactamente k veces
void range_rotate(int l, int r, int k) {
    int len = r - l + 1;
    k %= len;
    if (k == 0) return;

    int t1, t2, t3, left_part, right_part;
    split(root, r, t2, t3);
    split(t2, l - 1, t1, t2);
    // Separa el subárbol del rango [l, r] en los primeros (len - k) y los últimos k elementos
    split(t2, len - k, left_part, right_part);
    // Intercambia el orden de ambos bloques para efectuar la rotación circular derecha
    t2 = merge(right_part, left_part);
    root = merge(merge(t1, t2), t3);
}

// Operación I k x: Inserta x a la derecha de la posición k (o al inicio si k = 0)
void insert_at(int k, long long x) {
    int t1, t2;
    split(root, k, t1, t2);
    root = merge(merge(t1, new_node(x)), t2);
}

// Operación E k: Elimina el elemento en la posición k
void erase_at(int k) {
    int t1, t2, t3;
    split(root, k, t2, t3);
    split(t2, k - 1, t1, t2);
    root = merge(t1, t3);
}

// Operación M l r: Retorna el mínimo en el rango [l, r]
long long query_min(int l, int r) {
    int t1, t2, t3;
    split(root, r, t2, t3);
    split(t2, l - 1, t1, t2);
    long long ans = tree[t2].mn;
    root = merge(merge(t1, t2), t3);
    return ans;
}

int main() {
    // Optimización de entrada/salida estándar
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    for (int i = 0; i < n; ++i) {
        long long a_i;
        cin >> a_i;
        root = merge(root, new_node(a_i));
    }

    while (q--) {
        char type;
        cin >> type;
        if (type == 'A') {
            int l, r;
            long long x;
            cin >> l >> r >> x;
            range_add(l, r, x);
        } else if (type == 'R') {
            int l, r;
            cin >> l >> r;
            range_reverse(l, r);
        } else if (type == 'O') {
            int l, r, k;
            cin >> l >> r >> k;
            range_rotate(l, r, k);
        } else if (type == 'I') {
            int k;
            long long x;
            cin >> k >> x;
            insert_at(k, x);
        } else if (type == 'E') {
            int k;
            cin >> k;
            erase_at(k);
        } else if (type == 'M') {
            int l, r;
            cin >> l >> r;
            cout << query_min(l, r) << "\n";
        }
    }

    return 0;
}
