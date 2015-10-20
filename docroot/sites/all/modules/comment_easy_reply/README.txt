CONTENTS OF THIS FILE
---------------------

 * Overview
 * Features
 * Supported modules
 * Installation
 * How to use
 * How to use with Views
 * How to use with Quote
 * Author

OVERVIEW
-------------

The Comment Easy Reply module provides a simpler way to users to reply to
existent comments.

FEATURES
-------------

 * Comment numeration: each comment in the same node is numbered.
 * Use tags to reply: reply to a comment inserting a simple tag in comment body.
 * (e.g.: @#1, @#2, ...).
 * Reply to multiple comments at the same time: users can insert as many
   comment tags as they likes in comment body.
 * Themeable comment tags: each tag in a comment body will be rendered as a
   themeable link.
 * (two choices for default: #COMMENT_NUMBER and @COMMENT_AUTHOR.
 * E.g.: #1, #2.1, @myname, ...)
 * Tooltips on comment tags: each link to a referred comment will show the
   referred comment body in a tooltip, for a better discussion reading
 * Support for flat and threaded comments.
 * Activatable for specific node types: activate module's features for the
   needed node types only.
 * Content-type specific settings allowed.

SUPPORTED MODULES
-----------------

 * CKEditor
 * Ideal Comments
 * Quote
 * Token
 * Views
 * WYSIWYG
 
INSTALLATION
------------

1) Copy the Comment Easy Reply folder to the modules folder in your 
   installation.

2) Enable Comment Easy Reply folder modules using 
   Administration -> Modules(/admin/modules).

3) Configure module using Administration -> Configuration -> Content authoring 
   -> Comment Easy Reply settings (admin/config/content/comment-easy-reply).

4) Enable module's features for the needed node types using Administration ->
   Configuration -> Content authoring -> Comment Easy Reply settings -> Status
  (admin/config/content/comment-easy-reply/status).

5) (Optional) Override module's settings for specific node types in node type
   configuration page (admin/structure/types/manage/<node_type>).

HOW TO USE
-------------

The Comment Easy Reply module assigns a number to each comment of the same
node, respecting the comments order.
To reply to a comment, click on the comment number: a special tag will be added
to the comment form's textarea.
The special tag in your comment body will be converted into a link displaying
the number of the referred comment or its author.
Passing the mouse over the link will show the referred content body inside a
tooltip.

HOW TO USE WITH VIEWS
---------------------
New comment fields are provided to be used with Views:
 * Comment: Comment Easy Reply Comment number
   The permalink showing the comment number. A reply tooltip can be added to
   the link using field settings inside the View.
 * Comment: Comment
   This is the standard comment body field. A new option is added on field
   settings to activate Comment Easy Reply markup on comment body tags.
 * Comment: Comment Easy Reply Reply-to link
   A reply link capable to ajax-reply to the comment inserting the special
   tag inside comment form.
 * Comment: Comment Easy Reply Quote link
   A quote link capable to ajax-reply to the comment inserting the quoted
   comment into comment form.

The common use for a Views with Comment Easy Reply is in combination with
Panels module, creating a page on node/%nid made by node fields, a comment
form and a View showing current node's comments with Comment Easy Reply fields
inside.


HOW TO USE WITH QUOTE
---------------------
Enable Quote link on comments, Comment Easy Reply module will automatically add
ajax functionality to the link.

AUTHOR
------

Alberto Colella
http://drupal.org/user/460740

Contacs:
http://drupal.org/user/460740/contact
kongoji@gmail.com
