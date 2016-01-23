--------------
Per Page Queue
--------------

Per Page Queue provides a nodequeue service that can display a different
subqueue for each page, based on that page's path.

It allows users with permission to create, edit and delete subqueues based on
path aliases; By adding the provided block to the site, visitors can view these
subqueues when visiting these paths.


Creating a Per Page Queue
-------------------------
To create a Per Page Queue, navigate to the admin structure nodequeue page and
click the "add per page queue" link. Configure the nodequeue, minding the two
configurations that this module adds:

Use Tokens
----------
Checking this box will enable the use of tokenized paths as opposed to just
simple paths. Tokenized paths work well with the Pathauto module. For example,
assume we have a content type "news" that has a pathauto settings of
"news/[year]/[month]/[day]/[title]" and several nodes that created these path
aliases:
news/2011/08/15/lorem
news/2011/08/25/ipsum
news/2011/09/01/dolor
news/2011/09/05/sit
news/2011/09/15/amet

If you do not check the "Use Tokens" box, you will have to create a single
subqueue for each news item that you want. If you DO check the "User Tokens"
box, you can create a subqueue that will work on a specific tokenized path, so
the subqueue whose path is "news/2011/09/%/%" (where (%) is the token sign)
means it will be displayed on "dolor", "sit" and "amet", while the subqueue
whose path is "news/2011/%/15/%" will be displayed on "lorem" and "amet".

In cases where several subqueues might match the current page (say, we have
three subqueues whose paths are "news/2011/%/15/%", "news/2011/09/%/%" and
"news/2011/09/15/amet"), only one of the subqueues will be displayed in the
following precedence:
# The non-tokenized path is always first (e.g. "news/2011/09/15/amet") or ;
# If there are several tokenized paths, prefere the one with the rightmost
  tokens (e.g. "news/2011/09/%/%" takes precedence over "news/2011/%/15/%".)

Display View
------------
If you have the Views module installed and enabled, you will be given an option
to select a View to display in a block. This View must display nodes (not users
or any other type of object that Views support,) must have a defined
relationship with the queue, and expect the first argument to be a Subqueue ID.

Once selected, you can use the Block admin screen to place this block anywhere.
When viewing a page that contains this block, the block will look for the top
matching path (as described above in Use Tokens,) and, if found, will display
the View's content.

The View can have several displays (several pages and blocks, for example.)
When selecting which view to display, the form will list all the potential
displays, with "Master" being the default display.


Admin Block
-----------
The module provides a block for admins that provides a form to add a new
subqueue for the currently visited page. If there is already a subqueue for the
visited page it will display a link to edit that subqueue.


Not doing it with Views
-----------------------
If you prefer another solution to Views, for each path you can get the top
matching subqueue by calling the function perpagequeue_load_subqueue_by_path().
