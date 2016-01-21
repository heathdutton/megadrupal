<?php

/**
 * @file
 * Admin forms creation page.
 */

/**
 * Form API to display add/edit screen on admin page.
 */
function mini_blocks_admin_add_form($form = NULL, &$form_state = NULL, $action = array(), $mb = '') {
  $form = array();
  $val_name = (isset($mb->mb_value)) ? $mb->mb_value : '';
  $mb_id    = (isset($mb->mb_id)) ? $mb->mb_id : '';
  $mb_key   = (isset($mb->id)) ? $mb->id : '';

  $form['add_block'] = array(
    '#type' => 'markup',
    '#markup' => t('Type in a unique key and a value to @action its value', array('@action' => $action)),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['mini_block_primary_key'] = array(
    '#type' => 'value',
    '#value' => $mb_key,
  );
  $form['mini_block_action'] = array(
    '#type' => 'value',
    '#value' => $action,
  );
  $form['mini_block_key'] = array(
    '#type' => 'machine_name',
    '#title' => t('Key'),
    '#description' => t('This must be machine-readable entry.'),
    '#default_value' => $mb_id,
    '#required' => TRUE,
    '#machine_name' => array(
      'exists' => 'mini_blocks_key_exists',
    ),
  );
  $form['mini_block_value'] = array(
    '#type' => 'textarea',
    '#title' => t('Value'),
    '#rows' => '10',
    '#cols' => '100',
    '#required' => TRUE,
    '#default_value' => $val_name,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Changes'),
    '#id' => t('edit-submit'),
  );

  return $form;
}

/**
 * Form api to submit add/edit form.
 */
function mini_blocks_admin_add_form_submit($form, &$form_state) {
  global $user;
  $uid = (isset($user->uid)) ? $user->uid : 0;
  $key = $form_state['values']['mini_block_key'];
  $val = $form_state['values']['mini_block_value'];
  $id = $form_state['values']['mini_block_primary_key'];
  $action = $form_state['values']['mini_block_action'];
  $primary_key = '';

  $record = array(
    'id' => $id,
    'mb_id' => $key,
    'mb_value' => $val,
    'uid' => $uid,
    'mb_date' => REQUEST_TIME,
  );

  if ($action == 'edit') {
    $primary_key = 'id';
    $record['id'] = $id;
    $return = drupal_write_record('mini_blocks', $record, $primary_key);
  }
  else {
    $return = drupal_write_record('mini_blocks', $record);
  }

  $form_state['redirect'] = 'admin/structure/mini-blocks';

  return $return;
}


/**
 * Show list of mini blocks admin page.
 */
function mini_blocks_overview() {
  $result = db_select('mini_blocks', 'mb')
    ->fields('mb')
    ->orderBy('mb_id', 'ASC')
    ->execute()
    ->fetchAll();

  $headers = array(
    'key' => t('Mini Block Key'),
    'value' => t('Mini Block Value'),
    'date' => t('Date'),
  );
  $rows = array();
  $i = 0;
  foreach($result as $mb) :
    $rows[$i] = array(
      l($mb->mb_id, 'admin/structure/mini-blocks/edit/' . $mb->mb_id),
      '<pre>' . check_plain(substr($mb->mb_value, 0, 200)) . '</pre>',
      format_date($mb->mb_date, 'custom', 'm/d/Y h:sa'),
    );
    $i++;
  endforeach;

  return theme('table', array(
    'header'  => $headers,
    'rows'    => $rows,
  ));
}
