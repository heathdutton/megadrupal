File Entity Revisions for Drupal 7.x
Including: Uploading Tax Paths

INTRODUCTION
------------
This combo module provides file entity revisioning capabilities along with
persistent file paths and custom path placement by taxonomy value.  There is
also a Moderation component handled by Workflow fields.  This module will not work
without Workflow, and may produce unpredicable results if it is not configured
correctly for all three modules working together.

REQUIREMENTS
------------
Workflow 7.x-2.5
Workflow Field 7.x-2.5 (submodule of Workflow)
File Entity 7.x-2.0-beta1
File Admin 7.x-1.0-beta8

INSTALLATION
------------
Follow the contrib instructions for installing all dependencies.  Then enable
both File Entity Revisions and Uploading Tax Paths modules.
   
CONFIGURATION
-------------
* Setup file types with workflow fields, see contrib instructions for how to do
that.

* Add a taxonomy field to your file type, then check the box on it that says
"Use field's value to determine upload sub-path."  You can then put in the
path the files should be located in, using %1 as a replacement token.

* /admin/config/workflow/file_entity_revisions (Configuration -> Workflow ->
File Entity Revisions) This page lets you tell the module how Workflow is
setup, as it doesn't know by default.  You will want to tell it which field is
the workflow field, and also which actions are publish actions / unpublish
actions.  The fallback action is the state that will be set when File Entity
Revisions has to default to published or unpublished, but usually this is
modified by Workflow.

* /admin/config/workflow/workflow (Configuration -> Workflow -> Workflows) From
this page, click 'actions' on any of the workflows listed.  Here you will want
to set the file action of 'Publish File Entity' or 'Unpublish File Entity' on
any trigger that involves your file type going through a transition that would
publish or unpublish that file.

* /admin/config/workflow/uploading_tax_paths (Configuration -> Workflow ->
Uploading Tax Paths) Finally check this page to ensure that your path is
configured correctly for the file types you are using.

* Be sure to set New Revision checked by default when setting up your file
types because this new version disables the ability to check the box manually
and would cause the module to work incorrectly.

TROUBLESHOOTING
---------------

Q: The revisions tab isn't showing after I upload a file.
A: The revisions tab will not show until there are two or more revisions.
After updating the file again, the tab should now appear.

Q: My file is published, but when I go to edit it, it says it is unpublished.
A: Editing a file creates a new draft.  New drafts will always start out
unpublished.  Use moderation to publish files, not the published box option.

MAINTAINERS
-----------
Current Maintainers:
 * Rick Tilley (pcrats33) - https://www.drupal.org/u/pcrats33
 * Kenneth Lancaster (kenneth-lancaster) - https://www.drupal.org/u/kenneth-lancaster

