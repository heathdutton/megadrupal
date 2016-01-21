<?php

/**
 * Mosaik admin page.
 */
function mosaik_page($form, &$form_state, $mosaik = NULL) {
  $clone = FALSE;

  if (is_string($mosaik) && ($mosaik = mosaik_load($mosaik))) {
    $mosaik['name'] = 'clone_of_' . $mosaik['name'];
    $clone = TRUE;
  }

  if (!isset($form_state['mosaik'])) {
    $form_state['mosaik'] = $mosaik;
  }
  else {
    $mosaik = $form_state['mosaik'];
  }

  $form['mosaik'] = array(
    '#title' => !empty($mosaik['title']) ? $mosaik['title'] : t('Add Mosaik'),
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="mosaik-settings-wrapper">',
    '#suffix' => '</div>',
  );

  $form['mosaik']['settings'] = array(
    '#title' => t('Mosaik'),
    '#type' => 'vertical_tabs',
  );

  $form['mosaik']['settings']['mosaik'] = array(
    '#title' => t('Mosaik'),
    '#type' => 'fieldset',
  );

  $form['mosaik']['settings']['mosaik']['name'] = array(
    '#title' => t('Name'),
    '#type' => 'machine_name',
    '#default_value' => !empty($mosaik['name']) ? $mosaik['name'] : '',
    '#maxlength' => 64,
    '#machine_name' => array(
      'exists' => 'mosaik_name_exists',
    ),
    '#disabled' => !$clone && !empty($mosaik['name']),
  );

  $form['mosaik']['settings']['mosaik']['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($mosaik['title']) ? $mosaik['title'] : '',
  );

  $form['mosaik']['settings']['mosaik']['menu'] = array(
    '#title' => t('Behavior'),
    '#type' => 'select',
    '#options' => array(
      MENU_CALLBACK => t('Page'),
      MENU_NORMAL_ITEM => t('Page with menu entry'),
      MOSAIK_AS_BLOCK => t('Block'),
    ),
    '#default_value' => isset($mosaik['menu']) ? $mosaik['menu'] : 0,
    '#ajax' => array(
      'callback' => 'mosaic_settings_callback',
      'wrapper' => 'mosaik-settings-wrapper',
    ),
  );

  $menu = isset($form_state['values']['menu']) ? $form_state['values']['menu'] : $mosaik['menu'];
  $form['mosaik']['settings']['mosaik']['path'] = array(
    '#title' => t('Path'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#element_validate' => array('_mosaik_validate_path'),
    '#default_value' => !empty($mosaik['path']) ? $mosaik['path'] : '',
    '#access' => ($menu != MOSAIK_AS_BLOCK),
  );

  $form['mosaik']['settings']['mosaik']['sidebars'] = array(
    '#title' => t('Exclude sidebars'),
    '#type' => 'checkbox',
    '#default_value' => isset($mosaik['sidebars']) ? $mosaik['sidebars'] : 0,
    '#access' => ($menu != MOSAIK_AS_BLOCK),
  );

  $form['mosaik']['settings']['access'] = array(
    '#title' => t('Access'),
    '#type' => 'fieldset',
  );

  $form['mosaik']['settings']['access']['access_type'] = array(
    '#title' => t('Restrict access by'),
    '#type' => 'radios',
    '#options' => array(
      'permission' => t('Permission'),
      'roles' => t('Roles'),
    ),
    '#default_value' => !empty($mosaik['access_type']) ? $mosaik['access_type'] : 'permission',
  );

  $form['mosaik']['settings']['access']['access_by_roles'] = array(
    '#title' => t('Roles'),
    '#type' => 'checkboxes',
    '#options' => user_roles(),
    '#multiple' => TRUE,
    '#states' => array(
      'visible' => array('input[name="access_type"]' => array('value' => 'roles')),
    ),
  );

  if (!empty($mosaik) && $mosaik['access_type'] == 'roles') {
    $form['mosaik']['settings']['access']['access_by_roles']['#default_value'] = !empty($mosaik['access']) ? $mosaik['access'] : array();
  }

  foreach (module_implements('permission') as $module) {
    $perms[$module] = array();
    $function = $module . '_permission';
    $permissions = $function();
    asort($permissions);

    foreach ($permissions as $perm => $info) {
      $perms[$module][$perm] = strip_tags($info['title']);
    }
  }

  ksort($perms);

  $form['mosaik']['settings']['access']['access_by_permission'] = array(
    '#title' => t('Permissions'),
    '#type' => 'select',
    '#options' => $perms,
    '#states' => array(
      'visible' => array('input[name="access_type"]' => array('value' => 'permission')),
    ),
  );

  if (!empty($mosaik) && $mosaik['access_type'] == 'permission') {
    $form['mosaik']['settings']['access']['access_by_permission']['#default_value'] = !empty($mosaik['access']) ? $mosaik['access'] : array();
  }

  $form['mosaik']['settings']['appearence'] = array(
    '#title' => t('Appearence'),
    '#type' => 'fieldset',
  );

  $layouts = array();

  foreach (mosaik_layouts() as $theme => $layout) {
    $layouts[$theme] = $layout['template'];
  }

  $form['mosaik']['settings']['appearence']['layout'] = array(
    '#title' => t('Layout'),
    '#type' => 'select',
    '#options' => $layouts,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'mosaik_preview_callback',
    ),
    '#default_value' => !empty($mosaik['layout']) ? $mosaik['layout'] : NULL,
  );

  $form['mosaik']['settings']['appearence']['preview'] = array(
    '#type' => 'markup',
    '#value' => '',
    '#prefix' => '<div id="preview-div">',
    '#suffix' => '</div>',
  );

  // Add the buttons.
  $form['mosaik']['actions'] = array('#type' => 'actions');
  $form['mosaik']['actions']['submit'] = array(
    '#type' => 'submit',
    '#access' => !form_get_errors(),
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('mosaik_page_submit'),
  );

  if (!empty($mosaik['name'])) {
    $form['mosaik']['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('mosaik_page_delete_submit'),
    );
  }

  return $form;
}

/**
 * Ajax callback.
 *
 * @se mosaic_page()
 */
function mosaic_settings_callback($form) {
  return $form['mosaik'];
}

/**
 * 'html' callback.
 *
 * @see ajax_command_html()
 */
function mosaik_preview_callback($form, $form_state) {
  $layouts = mosaik_layouts();
  $layout = $layouts[$form_state['values']['layout']];
  $regions = array_keys($layout['variables']);

  foreach ($regions as $region) {
    $params[$region] = '<div class="mosaik-region-preview">' . $region . '</div>';
  }

  // Load default stylesheet in order to highlight mosaik regions.
  drupal_add_css(drupal_get_path('module', 'mosaik') . '/css/mosaik.css');
  mosaik_load_assets($layout);

  $preview = theme($form_state['values']['layout'], $params);
  $commands = array(ajax_command_html("#preview-div", $preview));

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Returns whether a mosaik name already exists.
 */
function mosaik_name_exists($value) {
  $exists = db_select('mosaik', 'm')
    ->fields('m', array('name'))
    ->condition('m.name', $value)
    ->execute()
    ->fetchField();
  return !empty($exists) ? TRUE : FALSE;
}

/**
 * Check if the provided path is already in use.
 */
function _mosaik_validate_path($element, &$form_state) {
  if ((!empty($form_state['mosaik']) && $form_state['mosaik']['path'] != $element['#value']) && drupal_valid_path($element['#value'], TRUE)) {
    form_error($element, t('The provided path is already in use.'));
  }
}

/**
 * Submit callback
 *
 * @see mosaik_page()
 */
function mosaik_page_submit($form, &$form_state) {
  $access = 'access_by_' . $form_state['values']['access_type'];
  $form_state['values']['access'] = $form_state['values'][$access];

  $action = !empty($form_state['mosaik']) ? t('updated') : t('saved');

  if ($form_state['values']['menu'] == MOSAIK_AS_BLOCK) {
    $form_state['values']['path'] = '';
    $form_state['values']['sidebars'] = 0;
  }

  $mosaik = mosaik_save($form_state['values']);
  drupal_set_message(t('@title @action', array(
    '@title' => $mosaik['title'],
    '@action' => $action,
  )));

  if (preg_match("|^" . MOSAIK_ADMIN_MENU . "|", current_path())) {
    $redirect = ($action == t('saved')) ? MOSAIK_ADMIN_MENU . '/' . $form_state['values']['name'] . '/pieces' : MOSAIK_ADMIN_MENU;
  }
  else {
    $redirect = $mosaik['path'];
  }

  // Rebuild menu on mosaik menu change.
  if (empty($form_state['mosaik']) || ($form_state['mosaik']['menu'] != $mosaik['menu'])) {
    drupal_set_message('Menu has been rebuilt');
    menu_rebuild();
  }

  $form_state['redirect'] = $redirect;
}

/**
 * Submit callback
 *
 * @see mosaik_page()
 */
function mosaik_page_delete_submit($form, &$form_state) {
  $form_state['redirect'] = MOSAIK_ADMIN_MENU . '/' . $form_state['values']['name'] . '/delete';
}

/**
 * Form constructor for the mosaik deletion confirmation form.
 */
function mosaik_delete_confirm($form, &$form_state, $mosaik) {
  $form['#mosaik'] = $mosaik;

  $form['name'] = array('#type' => 'value', '#value' => $mosaik['name']);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $mosaik['title'])),
    MOSAIK_ADMIN_MENU . '/' . $mosaik['name'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Executes mosaik deletion.
 *
 * @see mosaik_delete_confirm()
 */
function mosaik_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $mosaik = mosaik_load($form_state['values']['name']);
    mosaik_delete($mosaik);
    watchdog('mosaik', 'Mosaik: deleted %title.', array('%title' => $mosaik['title']));
    drupal_set_message(t('Mosaik %title has been deleted.', array('%title' => $mosaik['title'])));
    cache_clear_all('*', 'cache_menu', TRUE);
  }

  $form_state['redirect'] = MOSAIK_ADMIN_MENU;
}
