<?php
// $Id;

/**
 * Pages
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_settings($value = '') {
  $result = db_query('SELECT dpmid, root, timeout FROM {dynamic_persistent_menu}', array());

  $data = array();

  foreach ($result as $record) {
    $root = explode(':', $record->root);

    if ($root[1] != 0) {
      $link = menu_link_load($root[1]);
      $title = $link['title'];
    }
    else {
      $title = t('&lt;root&gt;');
    }
    $record->actions = l(t('Edit'), 'admin/config/user-interface/dynamic_persistent_menu/' . $record->dpmid . '/edit') . ' ' . l(t('Delete'), 'admin/config/user-interface/dynamic_persistent_menu/' . $record->dpmid . '/delete');
    $record->root = $root[0] . ':' . $title;
    $data[] = (array) $record;
  }

  return theme('dynamic_persistent_menu_settings_list', array('menus' => $data));
}

/**
 * Callbacks
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_settings_form($form, $form_state, $dpm = NULL) {
  $form = dynamic_persistent_menu_setting_base_form($dpm);

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_settings_form_validate($form, &$form_state) {
  if (!ctype_digit($form_state['values']['timeout'])) {
    form_set_error('timeout ', t('Timeout must be an integer number'));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_settings_form_submit($form, &$form_state) {
  $menu = new stdClass();
  $menu->dpmid = $form_state['values']['dpmid'];
  $menu->root = $form_state['values']['root'];
  $menu->timeout = $form_state['values']['timeout'];
  if ($menu->dpmid) {
    drupal_write_record('dynamic_persistent_menu', $menu, array('dpmid'));
  }
  else {
    drupal_write_record('dynamic_persistent_menu', $menu);
  }

  $form_state['redirect'] = 'admin/config/user-interface/dynamic_persistent_menu';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_delete_form($form, $form_state, $dpm) {
  $form['dpmid'] = array(
    '#type' => 'value',
    '#value' => $dpm->dpmid,
  );

  return confirm_form($form,
    t('Are you sure you want to delete the dynamic persistent menu #%dpmid?', array('%dpmid' => $dpm->dpmid)),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/user-interface/dynamic_persistent_menu',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function dynamic_persistent_menu_delete_form_submit($form, &$form_state) {
  dynamic_persistent_menu_delete($form_state['values']['dpmid']);
  $form_state['redirect'] = 'admin/config/user-interface/dynamic_persistent_menu';
}

/**
 * Theme functions
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function theme_dynamic_persistent_menu_settings_list($variables) {
  $menus = $variables['menus'];
  $output = '';
  if ($menus) {
    $headers = array(t('Id'), t('root'), t('timeout'), t('action'));
    $output .= theme('table', array('header' => $headers, 'rows' => $menus));
  }
  else {
    $output = t('No menu has been defined yet. <a href="!url">Create a new one</a>.', array('!url' => url('admin/config/user-interface/dynamic_persistent_menu/add')));
  }
  return $output;
}

