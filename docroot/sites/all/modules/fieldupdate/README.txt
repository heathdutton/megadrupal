Introduction:
-------------
* This module allow to update default value for all old/existing field without
  updating node publish/last change date. For now this is not compatible with 
  date, image and file type fields.

* This works only with field with node.

* I have a problem with one of my live project, where we need to add new field
  in our existing content type having lots of existing node. Client want to
  update this field with a default value for all old/existing nodes.

* For a full description of the module, visit the project page:
  https://www.drupal.org/sandbox/raj_visu/2418411

Requirements:
------------
No special requirements.

Recommended modules:
--------------------
None.

Installation:
-------------
* Install as usual, see http://drupal.org/node/895232 for further information.

CONFIGURATION
------------
1. Install your module as usual. See more details on:
   https://drupal.org/documentation/install/modules-themes/modules-7
2. Go to /admin/structure/types/manage/<content-type>/fields and edit field 
   that you want to update
3. Enable check box under default value.
