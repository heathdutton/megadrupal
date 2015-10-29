<?php

/**
 * @file
 */

/**
 * Implementation of hook_ec_charge_filter_info().
 */
function user_ec_charge_filter_info() {
  return array(
    'roles' => array(
      'name' => t('Roles'),
      'description' => t('Filter based upon the customers role.'),
      'module' => 'user_roles',
      'file' => 'user.inc',
      'core' => TRUE,
    ),
  );
}

function user_roles_filter_form(&$form_state, $settings = array()) {
  $settings += array('roles' => array());
  $form = array();

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => $settings['roles'],
    '#options' => user_roles(),
    '#description' => t('Select the roles which will be applicable to this charge.'),
  );

  return $form;
}

function user_roles_filter_settings() {
  return array('roles');
}

function user_roles_filter_process($type, $settings, $object) {
  $$type = & $object;

  switch ($type) {
    case 'txn':
      $account = user_load(ec_customer_get_uid($txn->ecid));
      break;

    case 'node':
      global $user;

      $account = $user;
      break;
  }

  $roles = array_keys($account->roles);

  $intersect = array_intersect($settings['roles'], $roles);
  return !empty($intersect);
}
