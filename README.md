<div align="left">
  <h1>Interval Trees</h1>
  <h3>A C project that solves different tasks with the use of Interval Trees</h3>
</div>
<br/>
<br/>

## Contents of the project ##
* arbori_intervale.c and arbori_intervale.h implement the general functions used to create an Interval Tree
* intersectii.c and intersectii.h implement the functions used to efficiently solve the problem of
determining the number of vertical/horizontal segments that intersect in a 2D plane
* pozitie_libera.c and pozitie_libera.h implement the functions used to efficiently solve the following problem:
given a list of numbers, create another list in the following way:
  * go through each number of the first list
  * try to add it in the created list on the position indicated by the number itself
  * if the position is empty, add the number there
  * if not, add it in the first empty position starting from that position to 0
  * if no position is empty, then discard the current number
