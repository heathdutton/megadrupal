<?php
/**
 * @file
 * Party info hook include.
 */

/**
 * Implements hook_party_data_set_info()
 */
function party_user_party_data_set_info() {
  $sets['user'] = array(
    'label' => t("User account"),
    'entity type' => 'user',
    'class' => 'PartyUserDataSet',
    'singleton' => TRUE,
    'max cardinality' => 1,
    'admin' => array(
      'edit' => 'admin/config/people/accounts',
      'manage fields' => 'admin/config/people/accounts/fields',
      'manage display' => 'admin/config/people/accounts/display',
    ),
    'form callback' => 'party_user_form_user',
    // Provide a piece corresponding to the data set.
    'piece' => array(
      'path' => 'user',
      'maker' => 'core',
      // @todo: these don't have any effect yet.
      // @see http://drupal.org/node/1669774.
      'weight' => -8,
    ),
    'actions' => array(
      'attach' => array(
        'controller' => 'PartyUserDataSetUIAttach',
        'action label' => 'Attach existing user',
      ),
      'add' => array(
        'controller' => 'PartyUserDataSetUIAdd',
        'action label' => 'Add a new user',
      ),
    ),
    // Some of our permissions require a warning.
    'permissions' => array(
      // Attaching a user account to a party potentially grants a new user
      // access to data on that party.
      'attach' => array(
        'restrict access' => TRUE,
      ),
      'edit' => array(
        'restrict access' => TRUE,
      ),
    ),
  );
  return $sets;
}

/**
 * Implements hook_party_primary_fields_sources_alter().
 */
function party_user_party_primary_fields_sources_alter(&$sources) {
  // Allow uid to be converted to a username.
  if (isset($sources['user']['sources']['user:uid'])) {
    $sources['user']['sources']['user:uid']['callbacks']['uid_to_username'] = array(
      'label' => t('Formatted username'),
      'callback' => 'party_user_primary_fields_callback_uid_to_format_username',
      'type' => 'text',
      'file' => array(
        'type' => 'inc',
        'module' => 'party_user',
        'name' => 'party_user.party',
      ),
    );
  }
}
