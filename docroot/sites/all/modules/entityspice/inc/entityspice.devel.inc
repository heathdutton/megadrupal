<?php

/**
 * Returns menu item for hook_menu of entityspice related to development stuff.
 *
 * Purpose of menu items: a page where developers can generate user entities
 * who their names will be a, b, c, ...
 * Good for quickly loging in as another user.
 */
function _entityspice_devel_menu_options() {
  $file_path =
    drupal_get_path('module', 'entityspice') . '/inc/entityspice.devel.inc';

  $items['admin/config/development/create_abc_users'] = array(
    'title' => 'Create ABC users',
    'description' => 'Create users with names from a to z',
    'page arguments' => array('_entityspice_devel_create_abc_users_form'),
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer site configurations'),
    'file' => $file_path,
  );

  return $items;
}

/**
 * Form callback for menu path admin/config/development/create_abc_users.
 *
 * @see _entityspice_devel_menu_options().
 */
function _entityspice_devel_create_abc_users_form($f, &$fs) {
  // Indicates which form mst be shown; create user or the form after:
  // confirm_form.
  $fs['create_user_ok'] = isset($fs['create_user_ok']) ?: FALSE;

  // Indicated user has confirmed the action or not.
  $confirm = isset($fs['values']['confirm']) ? $fs['values']['confirm'] : 0;

  // If action is confitmed in confirm_form.
  if($confirm) {
    // Check to see user with same name does not exist.
    // range(97, 122) is ASCII a-z.
    $names = array_map('chr', range(97, 122));
    $existing_names = db_query('SELECT name FROM {users} where name IN (:names)', array(':names' => $names))
      ->fetchCol();
    // Remove names which already exist.
    $names = array_diff($names, $existing_names);

    // Total number of users created after this action.
    $number_of_users = 0;
    foreach ($names as $c) {
      $fields = array(
        'name' => $c,
        'mail' => "$c@$c.$c",
        'pass' => $c, 'status' => 1,
        'roles' => array(DRUPAL_AUTHENTICATED_RID => 'authenticated user'),
      );
      user_save('', $fields);
      $number_of_users++;
    }

    drupal_set_message(t('Number of users created: @num', array('@num' => $number_of_users)));
    drupal_set_message(t('These users where created: @names', array('@names' => implode(', ', $names))));
    drupal_set_message(t('Skiped these names because they already existed: @names', array('@names' => implode(', ', $existing_names))));
  }
  // Else if user has clicked create_user, Show the confirm form.
  elseif($fs['create_user_ok']) {
    $path = 'admin/config/development/create_abc_users';
    $desc = t('All users will be activated and have a one letter password same as their name, This is considered a security risk and must not be done on production site.');
    $f = confirm_form($f, t('Are you sure?'), $path, $desc);
    return $f;
  }
  // Show the first form.
  else {
    $f['create_abc_users'] = array(
      '#markup' => 'Create users with names from a to z',
    );

    $submit = array('_entityspice_devel_create_abc_users_form_submit');
    $f['actions'] = _entityspice_f_actions();
    $f['actions']['submit'] = _entityspice_f_submit('', NULL);

    return $f;
  }
}

/**
 * Form submit callback for _entityspice_devel_create_abc_users_form.
 *
 * @see _entityspice_devel_menu_options().
 */
function _entityspice_devel_create_abc_users_form_submit(&$f, &$fs) {
  $fs['create_user_ok'] = TRUE;
  $fs['rebuild'] = TRUE;
  return $f;
}

function _entityspice_hook_info_page($etype = NULL) {
  $info = _entityspice_getinfo($etype);
  $default = _entityspice_info_defaults('default');
  $menu = array();
  $entityinfo = array();

  foreach (array_keys($info) as $module) {
    $hook_menu = $module . '_menu';
    $menu[$module] = $hook_menu();

    $hook_info = $module . '_entity_info';
    $entityinfo[$module] = $hook_info();
  }

  ksort($info);
  ksort($menu);

  $info = array(
    'default info' => $default,
    'entityspice info' => $info,
    'menu items' => $menu,
    'entity info' => $entityinfo,
  );

  return kprint_r($info, TRUE);
};
