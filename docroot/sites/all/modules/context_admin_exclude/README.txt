Context Admin Exclude
---------------------
Out of the box, contexts set to display Sitewide without specific URLs will get executed on every
page, including admin pages. Sometimes the context doesn't really need to be executed on admin pages, such as node/add.
These contexts cause Drupal to bootstrap and do some work that is not really needed. If you want
to change the list (/admin/structure/context/settings), you can implement the hook_context_admin_exclude_admin_paths_alter().

Installation
------------
Install the module as usual

Configuration
-------------
Go to /admin/structure/context/settings
If you need a certain context to run on admin paths, you can edit the context and tick "Execute this context on Admin paths".
