/* Given a number N, find the smallest greater number than N with the same set of digits (as N).
   If N is the greatest possible number with given digits, then print "NOT POSSIBLE TO FIND GREATER NUMBER"
   and stop */
   
 /* Logic: 
	Start from rightmost digit.
	1. Find the smallest greater digit to the right of current position
	2. If not found, go one position left
		2.1 If at leftmost position, print "NOT POSSIBLE TO FIND GREATER NUMBER" and stop
		2.2 Else, goto step 1
	3. If you find one, swap that digit with current one and call "sortAscending" and the set
	   of digits to its right. Print the resulting number as answer and stop
	
	Complexity: Traversal should take worst case O(N) and sortAscending() should take worst case
	O((n-1)log(n-1)). So 
	worst case complexity : O((n-1)log(n-1))
	Best case complexity  : O(1)
	Average case complexity : ???
*/