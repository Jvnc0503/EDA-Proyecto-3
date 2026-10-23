# BST implícito

**time limit per test:** 6 s.

**memory limit per test:** 512 MB

El usar un arreglo como estructura para almacenar datos no suele ser recomendable si dichos datos van a cambiar constantemente a lo largo del tiempo, ya que requiere de $\mathcal{O}(n)$ de trabajo para buscar/insertar/eliminar un valor y $\mathcal{O}(1)$ para acceder a la información de una posición. Por otro lado, la lista es el caso contrario — $\mathcal{O}(1)$ para insertar/eliminar un valor pero $\mathcal{O}(n)$ para buscar y acceder a la información de una posición.

Naturalmente, almacenar datos en un árbol binario de búsqueda (BST) suele ser la opción correcta si se garantiza cierta complejidad (sea normal o amortizada), pero a veces los cambios realizados sobre los datos no dependen de sus valores directamente, sino de sus posiciones. Para intentar resolver este tipo de problemas, algunos BST tienen la capacidad de "separar" la información que almacenan en dos BST diferentes respecto a un punto de corte según el *inorder traversal* — a estos BST se les suele agregar la palabra "implícito", ya que las "llaves" no son almacenadas en los nodos, sino que están representadas por la posición de cada nodo en el *inorder traversal* del BST.

Ya que tengo solo un problema de optimalidad dinámica de buena calidad, decidí reservármelo para el examen parcial. Por lo tanto, tu tarea es implementar un BST implícito que inicialmente contenga los elementos $a_1, \dots, a_n$ y soporte las siguientes operaciones:

* **`A l r x`**: Agregar $x$ a todos los elementos cuya posición esté entre $l$ y $r$; es decir, $a_i \leftarrow a_i + x$ para todo $i \in [l, r]$.
Por ejemplo, si se tiene $l=2$, $r=4$ y $x=7$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{1, 9, 10, 11, 5\}$$


* **`R l r`**: Revertir el rango de posiciones $[l, r]$.
Por ejemplo, si se tiene $l=2$ y $r=4$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{1, 4, 3, 2, 5\}$$


* **`O l r k`**: Rotar hacia la derecha el rango de posiciones $[l, r]$ exactamente $k$ veces.
Por ejemplo, si se tiene $l=2$, $r=4$ y $k=1$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{1, 4, 2, 3, 5\}$$


* **`I k x`**: Insertar el valor $x$ a la derecha de $a_k$ si $k > 0$, o al inicio de la secuencia si $k = 0$.
Por ejemplo, si se tiene $k=0$ y $x=7$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{7, 1, 4, 2, 3, 5\}$$



Por otro lado, si $k=4$ y $x=7$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{1, 2, 3, 4, 7, 5\}$$


* **`E k`**: Eliminar el valor en la posición $k$.
Por ejemplo, si se tiene $k=4$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, este cambia de la siguiente manera:

$$\{1, 2, 3, 4, 5\} \to \{1, 2, 3, 5\}$$


* **`M l r`**: Calcular el valor mínimo de los elementos cuya posición esté entre $l$ y $r$; es decir, calcular el valor $\min_{i=l}^r \{a_i\}$.
Por ejemplo, si se tiene $l=2$ y $r=4$ sobre el arreglo $a=\{1, 2, 3, 4, 5\}$, la respuesta sería $2$.

> **Nota:** Este proyecto tiene fecha límite el día 1 de noviembre a las 8:00 p.m. Además, se debe presentar un reporte de 2-3 páginas explicando el diseño de implementación, la correctitud algorítmica y el análisis de complejidad de la estructura.
> **Nota 2:** Esta vez el proyecto va a retar un poco más su implementación, así que pueden usar cualquier estructura que crean conveniente siempre y cuando pase el tiempo y la memoria límite que se establezca en cada grupo de casos de prueba.

---

## Input

La primera línea de entrada contiene dos enteros $n$ y $q$ ($1 \le n, q \le 5 \cdot 10^5$) — La cantidad de elementos iniciales de la secuencia y la cantidad de consultas.

La segunda línea de entrada contiene $n$ enteros $a_i$ ($\vert{}a_i\vert{} \le 10^9$) — El $i$-ésimo entero es el valor del $i$-ésimo elemento de $a$.

Las siguientes $q$ líneas de entrada contienen la descripción de $q$ consultas según lo establecido en el enunciado:

* Si la $i$-ésima consulta es de tipo `A`, se garantiza que $1 \le l \le r \le \vert{}a\vert{}$ y $\vert{}x\vert{} \le 10^9$.
* Si la $i$-ésima consulta es de tipo `R`, se garantiza que $1 \le l \le r \le \vert{}a\vert{}$.
* Si la $i$-ésima consulta es de tipo `O`, se garantiza que $1 \le l \le r \le \vert{}a\vert{}$ y $0 \le k \le r - l$.
* Si la $i$-ésima consulta es de tipo `I`, se garantiza que $0 \le k \le \vert{}a\vert{}$ y $\vert{}x\vert{} \le 10^9$.
* Si la $i$-ésima consulta es de tipo `E`, se garantiza que $1 \le k \le \vert{}a\vert{}$.
* Si la $i$-ésima consulta es de tipo `M`, se garantiza que $1 \le l \le r \le \vert{}a\vert{}$.

Se garantiza que la única operación que se realizará sobre una secuencia vacía es la de tipo `I`.

---

## Output

Para cada consulta de tipo `M`, imprime el mínimo valor en el rango de posiciones correspondiente en una nueva línea.

---

## Example

### Input

```text
10 15
1 2 3 4 5 6 7 8 9 10
A 4 8 3
M 5 7
M 7 10
R 2 5
M 2 6
M 2 3
I 3 4
M 3 4
M 5 10
E 6
M 3 5
M 4 4
O 3 6 3
M 5 8
M 7 10

```

### Output

```text
8
9
2
7
4
2
3
4
7
9

```

---

## Note

Para el caso de ejemplo, la secuencia es la siguiente:

* **Antes de las consultas:**
$a = \{1, 2, 3, 4, 5, 6, 7, 8, 9, 10\}$
* **Después de la consulta 1:**
$a = \{1, 2, 3, 7, 8, 9, 10, 11, 9, 10\}$
* **Consulta 2:** Mínimo en $[5, 7]$ — $8$.
$a = \{1, 2, 3, 7, 8, 9, 10, 11, 9, 10\}$
* **Consulta 3:** Mínimo en $[7, 10]$ — $9$.
$a = \{1, 2, 3, 7, 8, 9, 10, 11, 9, 10\}$
* **Después de la consulta 4:**
$a = \{1, 8, 7, 3, 2, 9, 10, 11, 9, 10\}$
* **Consulta 5:** Mínimo en $[2, 6]$ — $2$.
$a = \{1, 8, 7, 3, 2, 9, 10, 11, 9, 10\}$
* **Consulta 6:** Mínimo en $[2, 3]$ — $7$.
$a = \{1, 8, 7, 3, 2, 9, 10, 11, 9, 10\}$
* **Después de la consulta 7:**
$a = \{1, 8, 7, 4, 3, 2, 9, 10, 11, 9, 10\}$
* **Consulta 8:** Mínimo en $[3, 4]$ — $4$.
$a = \{1, 8, 7, 4, 3, 2, 9, 10, 11, 9, 10\}$
* **Consulta 9:** Mínimo en $[5, 10]$ — $2$.
$a = \{1, 8, 7, 4, 3, 2, 9, 10, 11, 9, 10\}$
* **Después de la consulta 10:**
$a = \{1, 8, 7, 4, 3, 9, 10, 11, 9, 10\}$
* **Consulta 11:** Mínimo en $[3, 5]$ — $3$.
$a = \{1, 8, 7, 4, 3, 9, 10, 11, 9, 10\}$
* **Consulta 12:** Mínimo en $[4, 4]$ — $4$.
$a = \{1, 8, 7, 4, 3, 9, 10, 11, 9, 10\}$
* **Después de la consulta 13:**
$a = \{1, 8, 4, 3, 9, 7, 10, 11, 9, 10\}$
* **Consulta 14:** Mínimo en $[5, 8]$ — $7$.
$a = \{1, 8, 4, 3, 9, 7, 10, 11, 9, 10\}$
* **Consulta 15:** Mínimo en $[7, 10]$ — $9$.
$a = \{1, 8, 4, 3, 9, 7, 10, 11, 9, 10\}$
