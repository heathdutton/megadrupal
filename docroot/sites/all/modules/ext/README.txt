OVERVIEW

Provides integration between Drupal and Ext JS 
(http://www.sencha.com/products/extjs).

This module provides several major features:

* Loading of the Ext JS library files.

* Automatic generation of JS code for Ext Model and Store definitions.

* An API to allow modules to provide Model and Store definitions,
  also allowing modules to alter definitions and generated code.
  
* A RESTful services API providing CRUD operations for all Models and Stores.
  The generated JS Model and Store definitions are automatically configured to
  use this API.
  
* Model definitions are provided for nodes (one for each node type), users,
  and View displays that use a fields row style.
  
* Store definitions are provided that are backed by Views.


Features still to come (this requires more thought and will likely change at least in details):

* Provide useful off-the-shelf functionality for non-developers.

  * Allow rendering some elements using Ext components: menus, grids from defined stores, trees from defined model relationships....
  
  * Allow specifying if links/buttons should be modified to prevent reloading the entire page, ie create ajax request using link address or form submission and replace xtype panel content with result (instead of placing content in an iframe).

* Provide API and UI to create Ext UI components from Drupal content/objects

  * Create plug-in API to allow defining new xtypes based on Drupal content/objects, eg page content, blocks, menus.
  
  * Provide plug-in for various types of content, with config option to specify id of content (eg page address, node ID, block ID, menu name).

  * Provide unified RPC API to get various content: pages, nodes, blocks, menus.



REQUIREMENTS

Ext JS 4.x.


INSTALLATION

1. Download and extract the Ext JS library to sites/all/libraries, such that 
   the file ext-all.js is located at sites/all/libraries/ext/ext-all.js

2. Install and enable this module in the normal way.

3. The module may be configured at Admin > Site configuration > Ext. 
 
 
DEBUGING TIPS
If you encounter problem in using the Ext module, check the following:

* Try the Ext Example module accompanying this module first before porting your existing Ext JS file over. Read its README.txt
  file for more information.

* If Ext page in /ext/page is not being generated, ensure that the Ext JS library is placed in the correct location:
  Try to view the source of the /ext/page in the browser, then find the url with 
  /sites/all/libraries/ext/resources/css/ext-all.css and ensure this file exists (the content of the
  CSS file should show up when click on it). Ext page will not show up if the Ext JS library does not exist 
  or is placed in the incorrect location.

* Try regenerating the Ext JS files. Goto /admin/build/ext, uncheck the "Enable" box and click "Save configuration", then
  check it and click "Save configuration" again. This is to ensure Ext JS files are being regenerated.
  To check this is so, Goto the directory {$public_file_dir}/ext/{Ext application name}, by default, this is usually 
  sites/default/files/ext/DrupalExt, make sure that the "Application.js", together "controller", "model", "store" and "view"
  directories are generated.

* Clear the cache before accessing the Ext page: under admin/config/development/performance, click on Clear all caches.

* If no node information appear in your Ext JS application, try accessing /ext/rest/store/{Entity}.js from the browser,
  where {Entity} is the entity enabled in admin/build/ext/stores page. For example, /ext/rest/store/Node.js,
  if "Node" is enabled in admin/build/ext/stores. If Ext module is working, you should see the node information in JSON format.
  Ensure that the user has appropriate permission to the nodes in order to generate the node.


  