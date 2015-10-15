<?php

/**
 * @file
 * Configuration page for Webmaster menu
 */

/**
 * Configuration form for Webmaster menu.
 */
function webmaster_menu_config_form($form, &$form_state) {

  // Get menus for the menu dropdown.
  $result = db_select('menu_custom')
    ->fields('menu_custom', array('menu_name', 'title'))
    ->orderBy('menu_name')
    ->execute();

  $menus = array();

  while ($record = $result->fetchAssoc()) {
    $menus[$record['menu_name']] = $record['title'];
  }

  $form['menu'] = array(
    '#type' => 'select',
    '#title' => t('Menu to use for the Webmaster toolbar'),
    '#options' => $menus,
    '#default_value' => variable_get('webmaster_menu_menu', NULL),
  );

  $form['add_home'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add home link to the Webmaster toolbar'),
    '#default_value' => variable_get('webmaster_menu_add_home', TRUE),
  );

  $form['home_menu'] = array(
    '#type' => 'select',
    '#title' => t('Add menu to home link'),
    '#options' => array_merge(array('_no_menu' => t('Do not add menu to home link')), $menus),
    '#default_value' => variable_get('webmaster_menu_home_menu', '_no_menu_'),
    '#states' => array(
      'invisible' => array(
        ':input[name="add_home"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['add_logout'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add logout link to the Webmaster toolbar (will only show when user is logged in)'),
    '#default_value' => variable_get('webmaster_menu_add_logout', TRUE),
  );

  $form['extra_menu'] = array(
    '#type' => 'select',
    '#title' => t('Add an extra menu to the Webmaster toolbar'),
    '#options' => array_merge(array('_no_menu' => t('Do not add extra menu to toolbar')), $menus),
    '#default_value' => variable_get('webmaster_menu_extra_menu', '_no_menu_'),
  );

  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the menu for the following roles:'),
    '#options' => array_map('check_plain', user_roles(FALSE)),
    '#default_value' => variable_get('webmaster_menu_roles', array()),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Submit handler for the Webmaster configuration form.
 */
function webmaster_menu_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $roles = $values['roles'];
  // $roles is a map. Keys: every role id. Values: same as key, when selected,
  // 0 otherwise.
  // We prefer a simpler datastructure: An array of role ids.
  // array_filter() is used to remove the key/value pairs with value==0.
  // This leaves an array like this: array(3 => "3", 4 => "5").
  // array_keys() is used to make it like this: array(3, 4).
  $roles = array_keys(array_filter($roles));

  // Now save all the values with variable_set.
  // We could almost have used system_settings_form - but not quite, because we
  // wanted to store the roles as a simple array of role ids.
  variable_set('webmaster_menu_roles', $roles);
  variable_set('webmaster_menu_menu', $values['menu']);
  variable_set('webmaster_menu_add_home', $values['add_home']);
  variable_set('webmaster_menu_home_menu', $values['home_menu']);
  variable_set('webmaster_menu_add_logout', $values['add_logout']);
  variable_set('webmaster_menu_extra_menu', $values['extra_menu']);
}
