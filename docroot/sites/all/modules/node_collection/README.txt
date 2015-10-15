Installing the module
=====================

1. Login to your Drupal installation as an admin.
2. Go into Modules and enable Node Collection.
3. The module adds the following permissions.
 1. Administer Node Collections - Administer node collection so that you can
    create, edit or delete parent child relationships.
 2. Administer Node Contents  - Add/Edit/Delete node contents for the
    relationships already defined.

Add Relationship
================

1. Login to your Drupal installation as an admin
2. Go to Configuration => Content authoring => Define a Parent > Child
   relationship.
3. Add a relationship name, select the parent content type and the child content
   types and press "Save relationship".
   Note: A content type can only be added once as the parent.

Edit or Delete relationship
===========================

1. Login to your Drupal installation as an admin
2. Go to Configuration => Content authoring => Edit or Delete relationship

Template overrides for node collections
=========================================

Page / Page container :-
    Common for all node collections : page--node_container_default.tpl.php
    Parent type specific : page--node_container_<parent_node_type>.tpl.php
    Ex: If book is a parent of some relationship page-book.tpl.php can be
        overridden by adding page--node_container_book.tpl.php without
        effecting page-book.tpl.php

Node :-
    Child nodes templates can be overridden per relationship as follows
    Ex: node--<parent_nodetype>-<child_nodetype>.tpl.php
    In a parent-child relationship book is a parent node type amd article is
    a child node type article inside book can be overridden using the following
    template node--book-article.tpl.php

Template variables available :-
    page : parent_node_object
           is_node_collection_container


    node :  is_node_collection
            is_node_parent
            node_collection_parent_node_id
            parent_node_object
