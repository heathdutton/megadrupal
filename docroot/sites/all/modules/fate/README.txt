Field API Tab Editor
--------------------
This module allows individual field values to be modified on their own custom
tabs on the entity's page. Each field must be enabled from the field's settings
page, then the new tab will show.


Features
------------------------------------------------------------------------------
The primary features include:

* Full support for (probably/hopefully) all fields.

* Support for revisions.

* Currently limited to node fields.

* Some modules will be excluded, in order to provide a simpler interface:
  * Field group
  * Metatag
  * Panelizer
  * Redirect

Note: Only nodes are fully supported, full support for all entities is
forthcoming.


Configuration
------------------------------------------------------------------------------
Open the "Manage fields" section for any content type, e.g.
  admin/structure/types/manage/article/fields

Open the 'edit' page for any field, enable the "Enable custom edit tab" option.
An extra field will be displayed allowing the label of the new edit tab to be
customized.


Credits / Contact
------------------------------------------------------------------------------
Maintained by Damien McKenna [1]. Based upon the Field API Pane Editor module
[2] by Earl Miles [3] and the References Manager module [4] by Damien McKenna.

Ongoing development is sponsored by Mediacurrent [5].

The best way to contact the author is to submit an issue, be it a support
request, a feature request or a bug report, in the project issue queue:
  http://drupal.org/project/issues/fate


References
------------------------------------------------------------------------------
1: http://drupal.org/user/108450
2: https://drupal.org/project/fape
3: https://drupal.org/user/26979
4: https://drupal.org/project/references_manager
5: http://www.mediacurrent.com/
