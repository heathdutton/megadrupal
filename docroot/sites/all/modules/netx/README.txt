DEPENDENCIES

Media module requires patch from https://www.drupal.org/node/2378973.

INSTALLATION

1) Install as usual, see https://www.drupal.org/node/895232 for further information.

2) Enable the module.

3) Go to admin/config/media/netx and configure authentication

HOW-TO CREATE METADATA VIEWS

1) Enable netx_devel module for getting /netx-metadata-demo-view demo view as example.
For creating something similar you have to create view of type File. Also there is a
"File metadata" type available if you don't need relations with file_managed table and
want to deal with metadata only.

2) file_metadata table is not normalized, so newly created view
should build with a grouping field by name for avoiding duplicates. Use Settings menu
under "Format:" within created view.

3) file_metadata value column contains serialized data, so we are using core views
views_handler_field_serialized handler for ability to display non serialized data.
For approaching that you have to know exactly array key for data you are trying
to display.
For selecting proper key from value column with stored array you have to select
human readable "Display format" tempodary for a field, called "Full data(unserialized)"
(use /admin/structure/views/nojs/config-item/netx_files/page/field/value_1 from
netx_devel for example) and you should get something like

  Array ( [0] => [1] => 11/18/2014 [2] => Completed )

in the auto preview panel for your field. Here are numeric keys within brackets you may use
after changing back to "A certain key" display format for a current field.
Do not enter brackets there.
If You are trying to view non array data, you should select "Full data(unserialized)"
display format by default. Make sure your data displayed in a right way by using
preview views panel at the bottom of the page.
Don't forget to add human readable label for selected field data afterwards using
"Create label" checkbox in field settings.

4) Do create filter for limiting out needed attribute names only. Use "OR" if you need to
display more than one attribute. For obtaining available attribute names add a field
"File metadata: File metadata name." temporary. After filter creation just remove it
if you don't need it for current view.
