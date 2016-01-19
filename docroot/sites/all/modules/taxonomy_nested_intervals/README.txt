The Taxonomy Nested Intervals module maintains a list of nested intervals for taxonomy terms. It is fully multiparent compatible.

Currently it overrides the taxonomy/term/% page (incl. feed) and also reintroduces the /all depth modifier. There's a patch bundled with the module, which allows global override of taxonomy_get_tree().

Views integration is on the todo list.

Implementation description
Attempt at nested intervals implementation. I view this as a mix between materialized path and nested sets.

(Nested sets, farey fractions, matrix encoding, möbius transformation, etc).

Essentially, the process is to take an enumerated path and turn it into a unique fraction.

The module implements 2 variants. The one proposed by Tropashko (MatrixTreeNode) which is a reverse order on every other depth, and the one proposed by Hazel (MatrixTreeNodeModified) which does not have this issue, but which has the disadvantage of using more data per node (since it basically just uses every other depth). The Hazel variant is a much simpler implementation and also supports sorted trees. The Tropashko is a bit more complex and does not support sorted trees out of the box. I believe it is possible to compensate for this, but the logic will be extensive, so I have not done this (yet).

Below is a verbose description of the Hazel variant (MatrixTreeNodeModified implementation).

First we convert a enumerated path into matrices:

<?php
$path = "5.6.7";
$matrices = MatrixTreeNodeModified::pathToMatrices($path);
MatrixTreeNodeModified::printMatrices($matrices);
?>

+---+---+  +---+---+  +---+---+  +---+---+  
|  0|  1|  |  1|  1|  |  1|  1|  |  1|  1|
|  1|  0|  |  5|  6|  |  6|  7|  |  7|  8|
+---+---+  +---+---+  +---+---+  +---+---+ 

The process for generating the matrices for e.g. path x.y.z is:
+---+---+  +---+---+  +---+---+  +---+---+  
|  0|  1|  |  1|  1|  |  1|  1|  |  1|  1|
|  1|  0|  |  x|x+1|  |  y|y+1|  |  z|z+1|
+---+---+  +---+---+  +---+---+  +---+---+ 
Then we multiply the matrices:

<?php
$matrix = MatrixTreeNodeModified::product($matrices);
MatrixTreeNodeModified::printMatrices(array($matrix));
?>

and get:
+---+---+  
|370|417|
| 63| 71|
+---+---+  
Explanation:

+---+----+  
|num|snum|
|den|sden|
+---+----+  
Where:

num: Numerator of the left value
den: Denominator of the left value
snum: Numerator of the right value (sibling numerator)
sden: Denominator of the right value (sibling denominator)
Left = 370 / 63 = 5.8730158730159
Right = 417 / 71 = 5.8732394366197

And there you have it. A unique left and right value for an enumerated path.

The reason for storing the numerator and denominator rather than the left and right values, is for easier manipulation of the paths (e.g. move subtrees, etc.).

It's also possible to convert the numerator and denominator back to an enumerated path.

<?php
$node = new MatrixTreeNodeModified(370, 63, 417, 71);
print $node->getPath() . "\n";
?>

5.6.7
To relocate a subtree, we "simply" substract the old parent and add the new.
Here we move all children from parent "5" to a new parent "6", (for simplicity we only have 1 child) e.g. "5.6.7" => "6.6.7"

First we find the fraction for the old parent:

<?php
$parent = MatrixTreeNodeModified::pathToMatrix("5");
?>

+---+---+  
|  5|  6|
|  1|  1|
+---+---+  
Then we find the fraction for the new parent:

<?php
$parent = MatrixTreeNodeModified::pathToMatrix("6");
?>

+---+---+  
|  6|  7|
|  1|  1|
+---+---+  
Then we relocate all of our children of parent "5" (in this case "5.6.7"):

+---+---+  +---+---+  +---+---+  +---+---+  
|  6|  7|  |  1|  0|  | -1|  6|  |370|417|
|  1|  1|  |  0|  1|  |  1| -5|  | 63| 71|
+---+---+  +---+---+  +---+---+  +---+---+  

Resulting in:
+---+---+  
|433|488|
| 63| 71|
+---+---+  

And converting back to enumerated path:
<?php
$node = new MatrixTreeNodeModified(433, 63, 488, 71);
print $node->getPath() . "\n";
?>

6.6.7
Voilá!

Explanation:
There are four matrices in the above equation.

Matrix of parent
Sibling relocator. In this case we did not relocate the top most siblings (5.6.7 => 6.6.7)
Inverse matrix of old parent
Matrix of child (remember this equation is per child that should be moved)
Another view of what's happening when moving a subtree (move 1.3.1 to 1.2.2):

1
1.1
1.2
1.2.1
1.3
1.3.1
1.3.1.1
1.3.1.2
1.3.1.3

becomes:

1
1.1
1.2.1
1.2.2
1.2.2.1
1.2.2.2
1.2.2.3
1.3

Parent to detach:

+---+---+  +---+---+  +---+---+             +---+---+
|  0|  1|  |  1|  1|  |  1|  1|     ===     |  7|  9|
|  1|  0|  |  1|  2|  |  3|  4|     ===     |  4|  5|
+---+---+  +---+---+  +---+---+             +---+---+
Parent to attach:

+---+---+  +---+---+  +---+---+             +---+---+
|  0|  1|  |  1|  1|  |  1|  1|     ===     |  5|  7|
|  1|  0|  |  1|  2|  |  2|  3|     ===     |  3|  4|
+---+---+  +---+---+  +---+---+             +---+---+
Siblings to move (+1, e.g. subtree 1.1 becomes 2.1 etc., remember these are enumerated paths we're moving. In the Drupal taxonomy data, we have to locate the insertion point, and bump the older siblings (not covered here))

+---+---+             +---+---+
|  1|  0|     ===     |  1|  0|
|  d|  1|     ===     |  1|  1|
+---+---+             +---+---+

Where d is the difference in the sibling enumeration (destination sibling number minus source sibling number, here 2 - 1 = 1 derived from 1.3.1 moved to 1.2.2).
Add them all together (process each child of 1.3.1 inclusive):

1.3.1 => 1.2.2
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
|  5|  7|  |  1|  0|  | -5|  9|  | 16| 25|     ===     | 19| 26|
|  3|  4|  |  1|  1|  |  4| -7|  |  9| 14|     ===     | 11| 15|
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
1.3.1.1 => 1.2.2.1
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
|  5|  7|  |  1|  0|  | -5|  9|  | 41| 66|     ===     | 45| 71|
|  3|  4|  |  1|  1|  |  4| -7|  | 23| 37|     ===     | 26| 41|
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
1.3.1.2 => 1.2.2.2
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
|  5|  7|  |  1|  0|  | -5|  9|  | 66| 91|     ===     | 71| 97|
|  3|  4|  |  1|  1|  |  4| -7|  | 37| 51|     ===     | 41| 56|
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
1.3.1.3 => 1.2.2.3
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
|  5|  7|  |  1|  0|  | -5|  9|  | 91|116|     ===     | 97|123|
|  3|  4|  |  1|  1|  |  4| -7|  | 51| 65|     ===     | 56| 71|
+---+---+  +---+---+  +---+---+  +---+---+             +---+---+
Since the old parent and then new parent and the sibling relocation are always the same, this can be simplified to:

+---+---+  +---+---+  +---+---+             +---+---+
|  5|  7|  |  1|  0|  | -5|  9|     ===     |-32| 59|
|  3|  4|  |  1|  1|  |  4| -7|     ===     |-19| 35|
+---+---+  +---+---+  +---+---+             +---+---+
Further simplifying the actual move:

1.3.1 => 1.2.2
+---+---+  +---+---+             +---+---+
|-32| 59|  | 16| 25|     ===     | 19| 26|
|-19| 35|  |  9| 14|     ===     | 11| 15|
+---+---+  +---+---+             +---+---+
1.3.1.1 => 1.2.2.1
+---+---+  +---+---+             +---+---+
|-32| 59|  | 41| 66|     ===     | 45| 71|
|-19| 35|  | 23| 37|     ===     | 26| 41|
+---+---+  +---+---+             +---+---+
1.3.1.2 => 1.2.2.2
+---+---+  +---+---+             +---+---+
|-32| 59|  | 66| 91|     ===     | 71| 97|
|-19| 35|  | 37| 51|     ===     | 41| 56|
+---+---+  +---+---+             +---+---+
1.3.1.3 => 1.2.2.3
+---+---+  +---+---+             +---+---+
|-32| 59|  | 91|116|     ===     | 97|123|
|-19| 35|  | 51| 65|     ===     | 56| 71|
+---+---+  +---+---+             +---+---+
Look at the MatrixTreeNode.class.php for how to handle the tree nodes.


