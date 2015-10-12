This module looks like the PM functions of Drush, allows us to download most of
popular WYSIWYG editors.

--------------------------------------------------------------------------------
Integration:
--------------------------------------------------------------------------------

Feel free to integrate your editor using my API. There are only 2 functions
you should know
- hook_drush_editor(): Register the list of your editors.
- hook_drush_editor_alter(): Change the list of registered editors.

You can see the file drush_editor.api.php for more information.

--------------------------------------------------------------------------------
Dependencies
--------------------------------------------------------------------------------

- Drush

--------------------------------------------------------------------------------
Supported Editors
--------------------------------------------------------------------------------

- CKEditor
- TinyMCE

--------------------------------------------------------------------------------
Installation
--------------------------------------------------------------------------------

Download the module and simply copy it into your contributed modules folder:
[for example, your_drupal_path/sites/all/modules] and enable it from the
modules administration/management page.
More information at: Installing contributed modules (Drupal 7).

Note: If you want to install the command to drush for global use. Just copy
the 'editor' folder to drush_base_dir/commands/ and run `drush cc drush` to
clear drush cache.

--------------------------------------------------------------------------------
How to use
--------------------------------------------------------------------------------

The idea and logic are from Drush PM, so there is no doubt that you understand
all of this very fast. Just run `drush help eddl` and see.

--------------------------------------------------------------------------------
Roadmap
--------------------------------------------------------------------------------

Currently the functionalities are quite simple, you can only download the
editors. My goal is providing some highly useful drush commands for the editors,
so here is the list of features in future releases :

(order by descending priority)
- Update/Delete editors.
- Support svn/git with submodules. (this is tough)
- Customize (this is the hardest part).
