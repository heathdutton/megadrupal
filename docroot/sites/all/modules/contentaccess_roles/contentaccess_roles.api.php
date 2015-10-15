<?php


/**
 * @file
 * Hooks provided by the contentaccess roles module.
 */

 /**
  * Provide information about fields that are related to contentaccess roles.
  *
  * Using this info, Contentaccess roles is aware of the fields,
  * and allows adding them to the correct bundle.
  *
  * - type: Array with the values "node". To define to
  *   which bundles the field may be attached.
  * - Description: The description of the field.
  * - field: The field info array as will be passed to field_create_field().
  * - instance: The field instance array as will be passed to
  *   field_info_instance().
  */
function hook_contentaccess_roles_fields_info() {
  $items = array();
  $items[CONTENTACCESS_ROLES_FIELD] = array(
    'type' => array('node'),
    'description' => t('Show this node/content only for the selected role(s). If you select no roles, the content/node will be visible to all users.'),
    'field' => array(
      'field_name' => CONTENTACCESS_ROLES_FIELD,
      'no_ui' => TRUE,
      'type' => CONTENTACCESS_ROLES_FIELD_WIDGET,
      'cardinality' => FIELD_CARDINALITY_UNLIMITED,
      'settings' => array(CONTENTACCESS_ROLES_FIELD => array()),
      'default_widget' => 'options_select',
      'default_formatter' => 'contentaccess_roles_default',
      'property_type' => 'integer',
    ),
    'instance' => array(
      'label' => t('Show content for specific roles'),
      'description' => t('Show this node/content only for the selected role(s). If you select no roles, the content/node will be visible to all users.'),
      'widget_type' => CONTENTACCESS_ROLES_FIELD_WIDGET,
      'view modes' => array(
        'full' => array(
          'label' => t('Full'),
          'type' => 'select',
          'custom settings' => FALSE,
        ),
        'teaser' => array(
          'label' => t('Teaser'),
          'type' => 'select',
          'custom settings' => FALSE,
        ),
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
    ),
  );
  return $items;
}

/**
 * TODO.
 */
function hook_contentaccess_roles_fields_info_alter(&$fields_info) {

}
