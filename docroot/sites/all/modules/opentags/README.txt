INTRODUCTION
------------
This module provides a rich user experience to insert AGLS metadata into Drupal
contents via the Metatag module.

OpenTags empowers your content creators to easily and intuitively specify
metadata tags when creating or updating content. OpenTags is pre-configured to
satisfy the AGLS requirements for Australian Government websites – aimed at
facilitating AGLS compliance across departments and agencies.

 * For a full description of the module, visit the project page:
   https://www.drupal.org/sandbox/yangli0516/2536148

 * To submit bug reports and feature suggestions, or to track changes:
   https://www.drupal.org/project/issues/2536148


REQUIREMENTS
------------
This module requires the Metatag module(https://www.drupal.org/project/metatag).

Please note that this module doesn't work together with the Metatag: Dublin Core
module and AGLS Metadata module due to tag name conflicts.


INSTALLATION
------------
 * Install and enable the OpenTags module.

 * Go to page http://[your site]/opentags/create-taxonomy and follow the 
   instructions to create required taxonomy terms. (Note: this step is necessary
   because it creates selectable tag sources used by the OpenTags widgets).


HOW TO USE
----------
OpenTags is a submodule of Metatag module. Therefore, it utilises all the
features of Metatag module.

To add OpenTags metadata, go to "Meta tags" tab at bottom of the content editing
page. Then expand "OpenTags" tab.

The OpenTags module provides two types of widget:

 * Drop down list. This widget is used for single value metadata.

 * Auto combo widget. This widget is similar to a combination of auto complete
   widget and drop down list widget. It allows you to select values from a list
   as well as auto complete as you type.

   - Click "Browse", single click on the element to expand sub categories terms
     and double click to select the term. Some element may not be selectable
     (Dark grey background), as they are only used for categorisation purposes.

   - Type the term value in the text box, the auto complete feature will list
     suggested terms based on the value you typed. The suggest list will also
     show the categories of the terms if the terms have a multi-level hierarchy.
     Select the value from the suggest list or type in the full term and press
     Enter key.

   - To delete a term, click the delete button (X) on the term tag.

To apply default metadata settings, go to page admin/config/search/metatags.
Please check the Metatag module documentation for more details.
