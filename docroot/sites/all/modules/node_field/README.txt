-------------------------------------------------------------------------------
Node field for Drupal 7.x
	by ADCI, LLC team - www.adcillc.com
-------------------------------------------------------------------------------

Description:
Node field module allows you to add unique extra fields to single drupal nodes.
It's not connected to fields module, so different nodes of one content type can have absolutely different sets of fields.

You can add text field (with range also), long text field, link, radios, select, taxonomy terms, google map, files.
You can use date field, if you activate date and date_popup module (http://drupal.org/project/date).
You can use node reference field, if you activate node_reference module (https://drupal.org/project/references).
You can use select or other field, if you activate select_or_other module (https://drupal.org/project/select_or_other).

You can add node fields as a field in Views (v.3) for the node content. 

Installation:
1) Install Node field module.
2) Go to admin/config/node-field. Select node types to use node field module.
3) Now you can add unique extra fields to your nodes.

Module provides following hooks:

hook_node_fields_alter($node, $node_fields)
Allows to change node node_field param. 
Being called after node fields were loaded from DB.

hook_node_field_update($field)
Allows to add custom behaviour on node field update event.

hook_node_field_delete($field);
Allows to add custom behaviour on node field delete event.

hook_node_field_widget_alter($node_field, $form)
Allows to change field widget to change field value.

hook_node_field_settings_alter($node_field, $form)
Allows to change field type settings form.

hook_node_field_formatter_alter($node_field, $value)
Allows to change output of field value.

Support:
http://drupal.org/project/node_field