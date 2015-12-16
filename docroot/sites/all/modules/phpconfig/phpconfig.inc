<?php

/**
 * @file
 * The helper function for menu callbacks.
 */

/**
 * Menu handler for admin/config/system/phpconfig.
 */
function phpconfig_list() {
  $header = array('Item', 'Value', 'Status', 'Operations');
  $rows = array();

  $results = db_query("SELECT * FROM {phpconfig_items} ORDER BY item ASC");

  if ($results) {
    while ($conf = $results->fetchObject()) {
      $rows[] = array(
        $conf->item,
        $conf->value,
        ($conf->status == 1) ? 'Enabled' : 'Disabled',
        l(t('edit'), "admin/config/system/phpconfig/$conf->configid/edit"),
      );
    }
  }
  $output = l(t('Add new'), 'admin/config/system/phpconfig/add');
  $output .= (!empty($rows)) ? theme('table', array('header' => $header, 'rows' => $rows)) : '';
  return $output;
}

/**
 * Get particular PHP config from db.
 */
function phpconfig_get_config($configid = 0) {
  if (is_numeric($configid)) {
    $config = db_query("SELECT * FROM {phpconfig_items} WHERE configid = :configid", array(':configid' => $configid))->fetchObject();
    if (!empty($config)) {
      return $config;
    }
    else {
      return NULL;
    }
  }
  else {
    return NULL;
  }
}

/**
 * Get particular PHP config from db by config item.
 */
function phpconfig_get_config_by_item($item) {
  return db_query("SELECT * FROM {phpconfig_items} WHERE item = :item", array(':item' => $item))->fetchObject();
}

/**
 * Rendering PHP config add, edit form.
 */
function phpconfig_newform($form, &$form_state, $configid = NULL) {
  if (isset($form_state['storage']['confirm']) && $form_state['storage']['confirm'] == TRUE) {
    $form['#redirect'] = 'admin/config/system/phpconfig';
    $msg = t('Are you sure you want to add this configuration?');
    $desc = t('You can disable the configuration any time you want.');
    if (isset($form_state['storage']['delete'])) {
      $msg = t('Are you sure you want to delete this configuration?');
      $desc = '';
    }
    return confirm_form($form, $msg, 'admin/config/system/phpconfig', $desc, t('Yes'), t('Cancel'));
  }
  else {
    $edit_flag = FALSE;
    if (isset($configid)) {
      $config_object = phpconfig_get_config($configid);
      if (empty($config_object)) {
        drupal_set_message(t('The configuration id !id is not valid', array('!id' => $configid)));
        return;
      }
      else {
        $edit_flag = TRUE;
        $form_state['op_type'] = "update";
        $form_state['configid'] = $config_object->configid;
      }
    }
    $form['phpconfig'] = array(
      '#type' => 'fieldset',
      '#title' => t('PHP Config'),
    );
    $form['phpconfig']['item'] = array(
      '#type' => 'textfield',
      '#title' => t('Item'),
      '#description' => t('Type PHP configuration item. E.g. memory_limit'),
      '#default_value' => ($edit_flag) ? $config_object->item : '',
      '#required' => TRUE,
    );
    $form['phpconfig']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#description' => t('Type value of the above configuration item.'),
      '#default_value' => ($edit_flag) ? $config_object->value : '',
      '#required' => TRUE,
    );
    if ($edit_flag) {
      $form['phpconfig']['status'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enabled'),
        '#default_value' => $config_object->status,
      );
    }
    $form['phpconfig']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    if ($edit_flag) {
      $form['phpconfig']['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
      );
    }
    drupal_add_js(drupal_get_path('module', 'phpconfig') . '/phpconfig_test_config.js');
    global $user;
    drupal_add_js(array(
      'phpconfig_test' => array(
        'ajaxUrl' => url('admin/config/system/phpconfig/test', array('absolute' => TRUE)),
        'msg' => t('Your current configuration may result into a WSOD!'),
        'phpconfig_tok' => drupal_get_token('phpconfig_verify_' . $user->uid),
      ),
    ), 'setting');
    return $form;
  }
}

/**
 * Validation handler of PHP config add, edit form.
 */
function phpconfig_newform_validate($form, &$form_state) {
  if (!isset($form_state['values']['confirm'])) {
    if ($form_state['values']['op'] != t('Delete')) {
      $item = $form_state['values']['item'];
      $available_config = phpconfig_get_config_by_item($item);
      // Check are we adding new item at the time of updating.
      if (isset($form_state['configid']) && !empty($available_config) && $available_config->configid != $form_state['configid']) {
        form_set_error('item', t('The item !item is already in the DB.', array('!item' => $item)));
      }
      // Check if we have any existing item already.
      elseif (!isset($form_state['configid']) && !empty($available_config)) {
        form_set_error('item', t('The item !item is already in the DB.', array('!item' => $item)));
      }
      $configs = ini_get_all();
      if (!isset($configs[$item])) {
        form_set_error('item', t('!item is not a valid item or not available in your current PHP version.', array('!item' => $item)));
      }
      elseif ($configs[$item]['access'] == 2 || $configs[$item]['access'] == 6) {
        form_set_error('item', t('The item !item can only be set in php.ini, .htaccess or httpd.conf file.', array('!item' => $item)));
      }
      elseif ($configs[$item]['access'] == 4) {
        form_set_error('item', t('The item !item can only be set in php.ini or httpd.conf file.', array('!item' => $item)));
      }
    }
  }
}

/**
 * Submit handler of PHP config add, edit form.
 */
function phpconfig_newform_submit($form, &$form_state) {
  if (!isset($form_state['values']['confirm']) && (!isset($form_state['values']['status']) || $form_state['values']['status'] != 0)) {
    $form_state['storage']['confirm'] = TRUE;
    if ($form_state['values']['op'] == t('Delete')) {
      $form_state['storage']['delete'] = TRUE;
    }
    $form_state['storage']['values'] = $form_state;
    $form_state['rebuild'] = TRUE;
  }
  else {
    if (isset($form_state['values']['confirm'])) {
      $form_state = $form_state['storage']['values'];
      unset($form_state['storage']);
    }
    if ($form_state['values']['op'] == t('Delete')) {
      db_query("DELETE FROM {phpconfig_items} WHERE configid = :configid", array(':configid' => $form_state['configid']));
    }
    else {
      global $user;
      $uid = $user->uid;
      $item = $form_state['values']['item'];
      $value = $form_state['values']['value'];
      if (isset($form_state['op_type']) && $form_state['op_type'] == 'update') {
        $status = $form_state['values']['status'];
        $configid = $form_state['configid'];
        db_update('phpconfig_items')
          ->fields(array(
            'uid' => $uid,
            'item' => $item,
            'value' => $value,
            'status' => $status,
          ))
          ->condition('configid', $configid, '=')
          ->execute();
      }
      elseif ($form_state['values']['op'] == t('Save')) {
        db_insert('phpconfig_items')
          ->fields(array('uid', 'item', 'value', 'status'))
          ->values(array(
            'uid' => $uid,
            'item' => $item,
            'value' => $value,
            'status' => 1,
          ))
          ->execute();
      }
    }
    $form_state['redirect'] = 'admin/config/system/phpconfig';
  }
}

/**
 * Implements phpconfig_test().
 */
function phpconfig_test() {
  echo drupal_json_output('success');
}
