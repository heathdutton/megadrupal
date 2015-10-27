Vocabulary Queue
================

Vocabulary Queue creates a nodequeue for a defined set of vocabularies and creates subqueues for each taxonomy term from the selected vocabularies have their own unique subqueue.  You can place nodes into any of these subqueues.

Vocabulary Queue is build using Nodequeue modules Smartqueue API and the implementation based on smartqueue.module (part of Nodequeue module).

Vocabulary Queue is similar in functionality to smartqueue.module and the code based on code from smartqueue.module.

They differ in that you can add nodes to any subqueue associated with the vocabulary and not just the subqueues matching the taxonomy terms of the node.

Vocabulary Queue also creates subqueues when creating taxonomy terms and not like smartqueue.module when trying to use the subqueue.
