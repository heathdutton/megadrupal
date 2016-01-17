
Drush extras
------------
Drush extras is a place where auxiliary drush commands may be found.
Typically, these "extra" commands are not be suitable for drush core
due to limitations; for example, some may only support certain platforms,
and others might require customization before use.

Drush extras welcomes contributions.


Installation Instructions
-------------------------
Use with drush-4.x or later.

    $ drush dl drush_extras

This will download drush_extras and place it in your $HOME/.drush
folder.  You may also download the release manually from:

    http://drupal.org/project/drush_extras

You may place drush_extras wherever you want, but if it is not
in a standard location for drush commands, you will need to add
it to your drush include file search path.  See examples/example.drushrc.php
in the drush project for more information.


Drush extras commands
---------------------
Here is a brief overview of the commands available in drush_extras.
Please see the help text for more information.


PUSHKEY

    drush pushkey user@host.domain.com

	Creates an ssh public/private key pair in $HOME/.ssh, if
	one does not already exist, and then pushes the public
	key to the specified remote account.  The password for the
	destination account must be entered once to push the
	key over; after the key has been stored on the remote
	system, subsequent ssh and remote drush commands may be
	executed using the public/private key pair for authentication.

	IN DRUSH EXTRAS because is is Linux / openssl-specific.


GREP

    drush grep '#regex#' --content-types=node

	Grep through a site's content using PCREs.

	IN DRUSH EXTRAS because it is only applicable to small sites
	(greping through enormous databases is impractically slow).


BLOCK-CONFIGURE

    drush block-configure --module=block --delta=0 --region=right --weight=10
    drush block-disable --module=block --delta=0
    drush block-show

  Configure, disable or show settings for particular blocks.

  Here, the --delta is the specific identifier for the
  desired block in the specified module. You can find
  it using `drush block-show`, or by inspecting the
  path in the block configuration form.

  IN DRUSH EXTRAS because site administration commands are not maintained in drush core.


GIVE

    drush give-node 27 bob
    drush give-comment 7 bob

	Change the ownership of a node or a comment.

	IN DRUSH EXTRAS because site administration commands are not maintained in drush core.


MENU-CREATE

    drush menu-create new_menu "New Menu" "Menu description."
    drush add-menu-item menu_name "New Link Title" "http://external.com/link/target"
    menu-list
    menu-links menu_name

  Create menus, add menu items, and list existing menus and items.

  IN DRUSH EXTRAS because site administration commands are not maintained in drush core.


SQL-HASH

    drush sql-hash
    drush sql-compare @site1 @site2

    	DEPRECATED

      This function is extremely inefficient.  If you'd like to determine
      whether the CONTENT of two sites has changed, use the following instead:

      $ drush @site sql-query --db-prefix 'select max(nid),max(changed) from {node}'

      Compare the output of this with the target site to see if anything changed.

      If you must use sql-hash or sql-compare, it is recommended to do so
      only with the --tables-list option with a small number of tables. For
      example:

      $ drush sql-compare @site1 @site2 --tables-list=users

      Output hash values for each table in the database, or compare two
        Drupal sites to determine which tables have different content.  Run
        before and after an operation on a Drupal site to track table usage.

	IN DRUSH EXTRAS because it is only nominally useful.  Likely to be removed
    in future releases.
