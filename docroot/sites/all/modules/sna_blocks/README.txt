This module provide simple node archive style option for Views.

The archive blocks can be theme using Jquery Menu Module.
For the archive blocks you can set the number of nodes will display under month.

This module depends on views(https://drupal.org/project/views) module.

-- INSTALLATION --

  Install module like normal

-- CONFIGURATION --

  Go to your edit view. Select format as 'Simple Node Archive' and then settings.
  Note: As the archive settings for each block is unique.
  It is recommended to override view "Block: Style options" for blocks.
  1.
    * Archived field name:
      Achived block will created based on this field.
      Default value is node posted date "node_created" and for custom field use Date module(http://drupal.org/project/date).
      Remember to add this field in views "FIELDS" list.
  2. A page is required to show the archive result. Need to create a view page and then use below setting for page.
    * View machine name :
      The machine name of the view whose page is used to display archive result.
    * View page display I D:
      The view page display id, e.g. page_1
    * sna_view_page_url:
      Page url.
  3. Theme archive block.
    * Use Jquerymenu Module:
      Check this box if you want to use Jquerymenu module to theme
      archive blocks.
