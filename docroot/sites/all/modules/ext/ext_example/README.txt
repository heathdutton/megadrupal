REQUIREMENTS

The ext module.

INSTALLATION

1. Ensure that Ext module is enabled, Ext JS library has been placed in sites/all/libraries/ext, and enable the Ext Example module in the usual way.

2. Ensure that some nodes are being created (it is not a blank Drupal site!). Create nodes in node/add if this is not so.

3. Goto /admin/build/ext. Check the Enable check box. If it is already checked, uncheck it and click "Save configuration", then
   check it and click "Save configuration" again. Goto the directory 
   {$public_file_dir}/ext/{Ext application name}, by default, this is usually 
   sites/default/files/ext/DrupalExt.  You should find some JS files with the directory structure:
   
   DrupalExt
     |-Application.js
     |-controller
     |    |- node
     |	      |-FieldValue.js
     |- model
     |    |- {.JS files with name of Entities}
     |- override
     |- store
     |    |- {.JS files with name of Entities}     
     |- view
          |- model
          |   |- FieldValues.js
          |- node
          |   |- Grid.js
          |- Viewport.js
          
4. Clear cache in admin/config/development/performance.

5. Goto /ext/page. You should see the Ext page being generated.

