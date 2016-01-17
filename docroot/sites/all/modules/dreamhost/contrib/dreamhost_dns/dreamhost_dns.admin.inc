<?php

/**
 * @file
 * Administration interface to manage the domains registered with Dreamhost.
 */

/**
 * Menu callback; Displays the administration settings for Disqus.
 */
function dreamhost_dns_admin_settings() {
  $dnses = dreamhost('dns-list_records');
  if (empty($dnses)) {
    return t('There are currently no DNSes available.');
  }
  $columns = array(
    'account_id' => '',
    'zone' => '',
    'comment' => '',
    'editable' => '',
    'record' => '',
    'type' => '',
    'value' => '',
    'operations' => '',
  );
  $header = array(
    'account_id' => t('Account ID'),
    'zone' => t('Zone'),
    'comment' => t('Comment'),
    'editable' => t('Editable'),
    'record' => t('Record'),
    'type' => t('Type'),
    'operations' => t('Operations'),
  );
  $rows = array();
  foreach ($dnses as $dns) {
    $row = array_merge($columns, $dns);
    unset($row['value']);
    $row['operations'] = l(t('Remove'), t('admin/config/services/dreamhost/dns/remove/@record/@type/@value', array(
      '@record' => $dns['record'],
      '@type' => $dns['type'],
      '@value' => $dns['value'],
    )));
    $rows[]['data'] = $row;
  }

  $output = theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));
  return $output;
}

/**
 * Menu callback; Display a confirmation message to remove a form.
 */
function dreamhost_dns_remove_form($form, $form_state, $record, $type, $value) {
  // Provide a title and description on the form.
  $title = t('Remove DNS record');
  $description = t('Are you sure you would like to remove %record? This action cannot be undone.', array(
    '%record' => $record,
  ));

  // Inject the values as hidden values.
  $form['record'] = array(
    '#type' => 'hidden',
    '#value' => $record,
  );
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => $type,
  );
  $form['value'] = array(
    '#type' => 'hidden',
    '#value' => $value,
  );

  // Construct the confirmation form.
  return confirm_form($form, $title, 'admin/config/services/dreamhost/dns', $description, t('Remove'), t('Cancel'));
}

/**
 * Form Callback; Remove a DNS record.
 */
function dreamhost_dns_remove_form_submit($form, $form_state) {
  // Retrieve which values to process.
  $record = $form_state['values']['record'];
  $type = $form_state['values']['type'];
  $value = $form_state['values']['value'];
  $dreamhost = dreamhost('dns-remove_record', array(
    'record' => $record,
    'type' => $type,
    'value' => $value,
  ));
  if (!isset($dreamhost)) {
    drupal_set_message(t('Failed to remove the DNS record.'), 'warning');
  }
  else {
    drupal_set_message(t('The DNS record has been removed.'));
  }
  drupal_goto('admin/config/services/dreamhost/dns');
}


/**
 * Menu callback; Add a DNS form.
 */
function dreamhost_dns_add_form($form, $form_state) {
  // Provide a title and description on the form.
  $title = t('Add DNS record');
  $description = t('Provide the values for the desired DNS below.');

  // Inject the values as hidden values.
  $form['record'] = array(
    '#type' => 'textfield',
    '#title' => t('Record'),
  );
  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
  );
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Value'),
  );
  $form['comment'] = array(
    '#type' => 'textfield',
    '#title' => t('Comment'),
  );

  // Construct the confirmation form.
  return confirm_form($form, $title, 'admin/config/services/dreamhost/dns', $description, t('Add'), t('Cancel'));
}

/**
 * Form Callback; Add a DNS record.
 */
function dreamhost_dns_add_form_submit($form, $form_state) {
  // Retrieve which values to process.
  $record = $form_state['values']['record'];
  $type = $form_state['values']['type'];
  $value = $form_state['values']['value'];
  $comment = $form_state['values']['comment'];
  $dreamhost = dreamhost('dns-add_record', array(
    'record' => $record,
    'type' => $type,
    'value' => $value,
    'comment' => $comment,
  ));
  if (!isset($dreamhost)) {
    drupal_set_message(t('Failed to add the DNS record.'), 'warning');
  }
  else {
    drupal_set_message(t('The DNS record has been added.'));
  }
  drupal_goto('admin/config/services/dreamhost/dns');
}
