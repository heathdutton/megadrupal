
***********************************************************
Installation instructions
***********************************************************

- Place the FX.php API in the sites/all/libraries folder.

  sites/all/libraries/fxphp/
  sites/all/libraries/fxphp/FX.php
  
- Install the FileMaker and Libraries module.

  sites/all/modules/libraries
  sites/all/modules/filemaker

- Enable the modules at admin/build/modules.

- Configure permissions.

***********************************************************
Requirements (FileMaker)
***********************************************************

- FileMaker server, v7 or above
- FileMaker user account with with XML extended privileges
- An open port pointing to FileMaker (usually 443 or 80)
- FX.php API (available from https://github.com/yodarunamok/fxphp/zipball/master)

***********************************************************
Dependencies (Drupal)
***********************************************************

- Libraries module

***********************************************************
Features
***********************************************************

FileMaker features:

- Create, find, browse, and edit modes
- Value lists
- Unlimited number of reusable connections to FileMaker databases

Drupal features and integration:

- Rich set of permissions
- Choice of interface for anonymous users

***********************************************************
Create connection(s) to FileMaker database(s)
***********************************************************

To interact with FileMaker you must first add a FileMaker connection at admin/config/development/filemaker/connection, accessible via the Drupal menu at:

Configuration > Development > FileMaker Settings.

The username and password are the FileMaker username and password, and you will only have access to the FileMaker elements that the FileMaker account has.

The user account you use must have fmphp Extended Privileges and the file must be hosted on a FileMaker server.

Do not use the .fp7 suffix for your database name. my_database.fp7 should be entered as my_database.

You may add as many connections, to as many databases, as you want. Each node will be based on exactly one connection.

***********************************************************
Prepare FileMaker for the web
***********************************************************

Create layouts in FileMaker which include the fields you want to appear on the web.

Every filemaker node is tied to a single layout and will only be able to interact with elements on that layout.

Be thoughtful when adding elements to the layout in FileMaker, as performance will decrease dramatically if you add too many.

It will almost certainly be better to have separate layouts for each node than to try to reuse a layout by adding more elements to it.

***********************************************************
Create filemaker nodes and interact with FileMaker:
***********************************************************

Create a filemaker node, like any other piece of content (node/add).

The only additional field on the node creation screen is a select field so you choose a connection (see above) and save the node. 

Once the node is saved, visit the 'Layout Mode' tab on the node, choose which FileMaker layout the node will be based on, and add the fields you want. 

That's it. Now you can enter 'Find Mode' and search for records, 'Create Mode' to add records, or 'Browse Mode' to view and edit records.
