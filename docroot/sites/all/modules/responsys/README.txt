Responsys Integration module
============================

This module provides integration of Drupal and the
Responsys Web Marketing tool.

This is very much an API tool for developers.

All configuration must currently be done in the settings.php
file.

Only one list is currently supported per site.

Some RULES integration is provided to trigger syncs
between Drupal and Responsys and the other way.

Settings to add to settings.php
===============================

$conf['responsys_user_folder'] = 'your-folder-name';
$conf['responsys_user_list'] = 'your-list-name';
$conf['responsys_api_username'] = 'your-username';
$conf['responsys_api_password'] = 'your-password';

To provide a mapping between fields on a user object to
fields in Responsys list use the following mapping like this:

$conf['responsys_field_mappings'] = array(
  'your-folder-name' => array(
    'your-list-name' => array(
      'uid' => 'DRUPAL_ID',
      'mail' => 'EMAIL_ADDRESS_',
      'field_title' => 'TITLE',
      'field_first_name' => 'FIRST_NAME',
      'field_last_name' => 'LAST_NAME',
      'field_checkbox' => 'RESPONSYS_BOOLEAN_FIELD',
      // The next item is a taxonomy mapping 'tid' => 'RESPONSYS_FIELD'...
      'field_taxonomy_list' => array(
        '63' => 'RESPONSYS_FIELD_A', // Yes/No Boolean field.
        '64' => 'RESPONSYS_FIELD_B', // Yes/No Boolean field.
        '65' => 'RESPONSYS_FIELD_C', // Yes/No Boolean field.
      ),
    ),
  ),
);

In this way you can link checkbox fields, text fields and taxonomy
fields on the Drupal user object to fields in the Responsys list.
