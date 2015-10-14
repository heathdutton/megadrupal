
    =====   USAGE   =====

This module provides a default plugin argument for views,
with which you can get the parent terms associated with the current node.

It is very useful to get related content from a higher taxonomic level.

Using it in conjunction with the argument "Taxonomy Term: Parent Term" you
can get all those nodes for which the parent node's terms is the same as
in the current node.

You can get this argument creating the relationship
"Content: Taxonomy terms on node"


    =====   EXAMPLE   =====

Suppose we have a Taxonomy Tree like these:

  |- Parent_Term_1
  |--- Term_A   --   Associated to Node_1 and Node_2
  |--- Term_B   --   Associated to Node_3
  |--- Term_C   --   Associated to Node_4, Node_5 and Node_6
  |
  |- Parent_Term_2
  |--- Term_Z
  |--- Term_Y
  |
  |- Parent_Term_3
  |--- Term_J
  |--- Term_K
  |--- Term_L

Views provide a custom default argument to get content related to taxonomy
terms associated to current node. In the example above, Node_3 would not have
related content, Node_1 would have Node_2 as related content and Node_4 would
have Node_5 and Node_6 as related content.

However, this module provides another default plugin argument, to get
associated parent terms. In the example above, Node_3 would have all nodes as
related content. Is useful when there are few content associated with a
taxonomy term.
