CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration
  * Known issues


INTRODUCTION
------------

This module is an enhancement to the media module. It adds a new plugin so that
you can use a view to list and organize your files. The main advantage of this
is that you can add custom fields to your file entities, and then add those
fields to the media browser view. When managing files, you can filter your files
via any exposed filters in the view.

Sample use case: Filtering by taxonomy 

INSTALLATION
------------

1. Make sure you have met all the module dependencies.

   The following modules and their dependencies need to be present before you 
   can enable this module:
   - Media (+ File Entity)
   - Views (+ CTools)
   This module has been tested with Media 7.x-2.x-dev and Views 7.x-3.0-rc1

2. At admin/build/modules, enable the views_media_browser module, which is
   located in the Media group.

3. Verify that the media browser view was reverted correctly at: 
   /admin/content/media_browser
 
   There should be links labeled "Thumbnails" and "List" and you are looking
   at the "Thumbnails" page by default.

CONFIGURATION
-------------

These steps will guide you through one of the possible use cases. In this use 
case, we will add a term reference (taxonomy) filter to the media browser view: 

1. Media browser does not play well without clean URLs. Take a moment and make
   sure you have them turned on.

2. Add a field to your image content type (or any other file type).

   To do that, go to /admin/config/media/file-types and click on manage fields.

3. Once you added this field, you will be able to use it when you edit files via
   /admin/content/media and clicking the edit link. Your new field should show
   up in the edit form.

4. If you have the views_ui module enabled, then you can customize the view at
   /admin/structure/views/view/views_media_browser/edit and add the field that 
   you added in step 2 as an exposed filter. If the field is multi-select for 
   users, make sure that you check the reduce duplicates, or your file will 
   appear more than once.

   Save the view.

5. Go back to the media browser at /admin/content/media_browser. You should now
   have a nice exposed filter to narrow down the selection of your files.

KNOWN ISSUES
------------
1. Clean URLs need to be turned on for the media browser to work correctly.
