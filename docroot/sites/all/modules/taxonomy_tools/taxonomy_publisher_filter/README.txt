This module allows you to limit the taxonomy terms listed in a term reference 
form element or in a views exposed filter.

Description

Based on a status field disabled terms will be removed from the options by a
 hook_field_widget_form_alter() implementation for term reference field widgets
and by a hook_form_FORM_ID_alter() for views exposed forms.
The module allows you to enable/disable this limitation with permissions
for each vocabulary.

Dependencies

It depends on the Taxonomy Publisher module 
(now part of the Taxonomy tools project), which
attaches a status field to the taxonomy terms of the vocabularies you select.

Versions v1.0

Version 1.0 supports select and checkbox widgets 
for term reference fields.

Versions v2.0

The current version supports select, checkbox/radio and autocomplete widgets 
for term reference fields.
Provide a better administration UI and cache functionality.

How to use

download the module and place it under 'sites/all/modules/contrib' folder
with Drush use: drush dl taxonomy_publisher_filter
enable the module from the modules page: 'admin/build/modules'
enable taxonomy publisher on the vocabularies you wish to limit
enable/disable the terms in the above vocabularies configure roles 
that show see the limited list only under admin/people/permission

Custom Form

_taxonomy_publisher_filter_custom_form($vid, $settings) function is helpful to 
filter your select or checkbox/radio field in custom forms.
The function will expect a vocabulary id by default 
and will return the filtered option list.

Documentation: http://drupal.org/node/1895610
