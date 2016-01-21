-- SUMMARY --

Goto Last Page defaults to the last page of comments (instead of the first one)
for configurable node types.
It also allows adding a custom fragment (like #comments) to pager links, so
the user will go straight to comments section while paging through comments.


-- DESCRIPTION --

If "page" query parameter is not set, Drupal defaults to the first page
of a pager; but this is not always the best choice.

Example
-------
Node: http://example.org/mynode
Node comments: 500
Comments / page: 25
Comments sorting: Threaded list - Expanded / Date - Oldest first

When a visitor opens http://example.org/mynode , Drupal will return the node
together with the first page of comments (1 to 25).
But these comments are the oldest, so the user must click again on the comments
pager to see the more recent ones... and this could require more than one click.

Goto Last Page module changes this behavior (on configurable node types)
by returning the last page by default.


-- WHAT CHANGES? --

Let's see how rendered pager links will be changed by Goto Last Page module
when an user request the same URI as above.

* Without Goto Last Page module
URL:        http://example.org/mynode
Returned comments page: first
First page: http://example.org/mynode
Prev. page: http://example.org/mynode
Page 2:     http://example.org/mynode?page=1
Page 3:     http://example.org/mynode?page=2
Next page:  http://example.org/mynode?page=1
Last page:  http://example.org/mynode?page=19

* With Goto Last Page module
URL:        http://example.org/mynode
Returned comments page: last
First page: http://example.org/mynode?page=0
Prev. page: http://example.org/mynode?page=18
Page 2:     http://example.org/mynode?page=1
Page 3:     http://example.org/mynode?page=2
Next page:  http://example.org/mynode
Last page:  http://example.org/mynode

As you can see now the default page is the last; to see the first one "page=0"
query parameter must be added to the URI.


-- REQUIREMENTS --

Comment core module must be enabled.


-- INSTALLATION --

Install as usual, see https://drupal.org/node/895232 for further information.


-- CONFIGURATION --

By default the module is active on all node types.
Open configuration page (admin/config/system/gotolastpage) to change it.


-- PERMISSIONS --

By default only admin can access the configuration of this module.
If you need to give this permission to other users, go to the permissions page:
  admin/people/permissions#module-gotolastpage


-- CONTACT --

Current maintainer:
- Claudio Nicora (nicorac) - https://drupal.org/user/235024
