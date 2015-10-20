<?php

/**
 * @file
 * Integration with Variable module.
 */

/**
 * Implements hook_variable_info().
 */
function entity_abuse_variable_info($options) {

  $variable['entity_abuse_own_content'] = array(
    'type' => 'boolean',
    'title' => t('Allow user to abuse on his own content'),
    'description' => t('Defines if user may complain on the own content'),
    'default' => ENTITY_ABUSE_OWN_CONTENT_DISALLOWED,
    'group' => 'entity_abuse',
  );

  $variable['entity_abuse_reason_field'] = array(
    'type' => 'select',
    'title' => t('Reason field'),
    'description' => t('Setting for adding reason textarea when user submits his complain.'),
    'default' => ENTITY_ABUSE_REASON_FIELD_OPTIONAL,
    'group' => 'entity_abuse',
    'options' => array(
      ENTITY_ABUSE_REASON_FIELD_DISABLED => t('Disabled'),
      ENTITY_ABUSE_REASON_FIELD_OPTIONAL => t('Optional'),
      ENTITY_ABUSE_REASON_FIELD_REQUIRED => t('Required'),
    ),
  );

  $variable['entity_abuse_link_submit_title'] = array(
    'type' => 'string',
    'title' => t('Submit abuse link title', array(), $options),
    'description' => t('Title of the "submit abuse" link which displayed on the content.', array(), $options),
    'default' => ENTITY_ABUSE_LINK_SUBMIT_ABUSE_TITLE,
    'group' => 'entity_abuse',
    'localize' => TRUE,
  );

  $variable['entity_abuse_link_cancel_title'] = array(
    'type' => 'string',
    'title' => t('Cancel abuse link title', array(), $options),
    'description' => t('Title of the "cancel abuse" link which displayed on the content.', array(), $options),
    'default' => ENTITY_ABUSE_LINK_CANCEL_ABUSE_TITLE,
    'group' => 'entity_abuse',
    'localize' => TRUE,
  );

  return $variable;
}

/**
 * Implements hook_variable_group_info().
 */
function entity_abuse_variable_group_info() {

  $groups['entity_abuse'] = array(
    'title' => t('Entity abuse'),
    'description' => t('Variables for Entity abuse configuration.'),
    'access' => 'manage entity abuses',
    'path' => array('admin/config/content/entity_abuse'),
  );

  return $groups;
}
