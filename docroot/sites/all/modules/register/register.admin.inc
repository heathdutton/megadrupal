<?php

/**
 * @file
 * Code for Register administration pages.
 */

/**
 * Administrative configuration form.
 */
function register_admin_form($form, $form_state) {
  $form = array();

  // Membersonly filters out the anonymous role.
  $roles = user_roles($membersonly = TRUE);

  // We may as well filter out authenticated too.
  unset($roles[2]);

  $form['register_user_status'] = array(
    '#title' => t('User Status'),
    '#description' => t('Initial status of users from this form.'),
    '#type' => 'radios',
    '#options' => array(
      'blocked' => t('Blocked'),
      'active' => t('Active'),
      'default' => t('System default status (active only if visitor registration is enabled)'),
    ),
    '#default_value' => variable_get('register_user_status', 'default'),
  );

  $form['register_roles'] = array(
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#title' => t('Role Assignment'),
    '#description' => t('Which roles (if any) should be assigned to users who complete this form?'),
    '#default_value' => variable_get('register_roles', array()),
  );

  $form['register_path'] = array(
    '#title' => t('Registration Path'),
    '#type' => 'textfield',
    '#description' => t('What path do you want this form to be displayed on?'),
    '#default_value' => variable_get('register_module_register_path', 'user/register/default'),
  );

  if (module_exists('profile2')) {
    // Add bundle info but bypass entity_load() as we cannot use it here.
    $types_obj = db_select('profile_type', 'p')
        ->fields('p')
        ->execute()
        ->fetchAllAssoc('type');

    $types = array();
    foreach ($types_obj AS $type => $info) {
      $types[$type] = $info->label;
    }

    $form['register_profiles'] = array(
      '#type' => 'checkboxes',
      '#options' => $types,
      '#title' => 'Profile Assignment',
      '#description' => t('Which profiles should we show on this registration form?'),
      '#default_value' => variable_get('register_profiles', array()),
    );
  }
  
  $form['#submit'][] = 'register_admin_form_set_path';

  return system_settings_form($form);
}

function register_admin_form_set_path($form, &$form_state){
    $path = variable_get('register_module_register_path', 'user/register/default');
    if($form_state['values']['register_path'] != $path){ //if something changed
      variable_set('register_module_register_path', $form_state['values']['register_path']); //set new path
      menu_rebuild();
    }
    unset($form_state['values']['register_path']); //make sure we don't save this again
}
