
INSTILLATION:

Simply copy the comment_tree folder to your sites/all/modules folder.
Then go to admin/modules and enable the module.

If you already have Existing Comments
Go to admin/config/development/maintenance
and put your site in maintenance mode.
This will prevent anyone from commenting on a node
while the comment tree is being built
(which could mess up the comment tree).

Then Generate the comment tree by going to:
admin/config/system/comment-tree

That's it! The comment tree will be maintained on
comment insert, update, and delete.
