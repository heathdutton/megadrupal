<?php

/**
 * @file
 * Rules integration for User addressbook.
 *
 * @addtogroup rules
 * @{
 */


/**
 * Implements hook_rules_action_info().
 */
function user_addressbook_rules_action_info() {
  $actions = array();

  $actions['user_addressbook_set_default_address'] = array(
    'label' => t('Set the user address as default of the given type'),
    'parameter' => array(
      'user_address' => array(
        'type' => 'user_address',
        'label' => t('User address to be set as default'),
      ),
      'type' => array(
        'label' => t('Default type'),
        'type' => 'list<text>',
        'description' => t('The default type(s) for addresses.'),
        'options list' => 'user_addressbook_address_types_options_list',
      ),
    ),
    'group' => t('User address'),
    'callbacks' => array(
      'execute' => 'user_addressbook_set_default_address',
    ),
  );

  return $actions;
}

/**
 * Options list callback: address fields for the address comparison condition.
 */
function user_addressbook_address_types_options_list() {
  $options = array();

  // Retrieve a list of all user address type defined.
  $user_address_types = user_addressbook_address_types();

  // Loop over every type and build the list options.
  foreach ($user_address_types as $type => $user_address_type) {
    $options[$type] = $user_address_type['title'];
  }

  return $options;
}
