-------------------------------------------------------------------------------
Sky field for Drupal 7.x
	by ADCI, LLC team - www.adcillc.com
-------------------------------------------------------------------------------

Description:

Sky field module has grown up from Node field module (http://drupal.org/project/node_field).
It provides wider and more flexible functionality.

Sky field module allows you to add unique extra fields to single drupal entity.
It's not connected to fields module, so different entities of one entity type can have absolutely different sets of fields.

You can add text fields, long text fields, links, radios, select, checkbox, taxonomy terms.
You can use date fields, if you activate date and date_popup module (http://drupal.org/project/date).

Installation:
1) Install Sky field module.
2) Go to admin/config/sky-field. Select entity types to use sky field module.
3) Now you can add unique extra fields to your entities.

UI is made the same way as in fields module. Many hooks were added, so now it can be used as a base for more complex modules.
You can add new sky field types using hook_sky_field_info_alter. You can change fields widgets and formatters. And so on!
We have tried to make it as flexible as it's possible!

It's beta version now, so we are grateful for any feedback!

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