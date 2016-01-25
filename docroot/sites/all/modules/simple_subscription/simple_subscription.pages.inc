<?php
/**
 * @file simple_subscription.pages.inc
 */

function simple_subscription_main_admin_form() {
  $form = array();

  $form['ssubscription_user_account_attach'] = array(
    '#type'         => 'checkbox',
    '#title'        => t('Attach subscriptions to user accounts.'),
    '#description'  => t('If a logged in user subscribes to the newsletter(s), then he can manage subscriptions from his user account, and no verification mail is sent on subscription. Only one subscription mail is allowed by Drupal user.<br />If unchecked no correspondance will be kept between subscriptions and user accounts.'),
    '#default_value' => variable_get('ssubscription_user_account_attach', TRUE),
  );

  $form['ssubscription_user_account_attach_options'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Drupal users subscription options'),
    '#states' => array(
      'visible' => array(
        ':input[name="ssubscription_user_account_attach"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['ssubscription_user_account_attach_options']['ssubscription_user_account_add'] = array(
    '#type'   => 'checkbox',
    '#title'  => t('Add subscription form to user edit form'),
    '#default_value' => variable_get('ssubscription_user_account_add', TRUE),
  );

  $form['ssubscription_user_account_attach_options']['ssubscription_delete_user_action'] = array(
    '#type'    => 'radios',
    '#title'   => t('On user delete'),
    '#options' => array(
      SSUBSCRIPTION_DELETE_USER_NOTHING  => t('Do nothing.'),
      SSUBSCRIPTION_DELETE_USER_UNSUBSCRIBE => t('Remove subscription (but keep record).'),
      SSUBSCRIPTION_DELETE_USER_DELETE   => T('Delete subscription.'),
    ),
    '#default_value' => variable_get('ssubscription_delete_user_action', SSUBSCRIPTION_DELETE_USER_NOTHING),
    '#description' => t('Choose action to perform when a subscribed user is deleted from Drupal'),
  );

  $form['ssubscription_user_account_attach_options']['ssubscription_block_user_options'] = array(
    '#type'    => 'radios',
    '#title'   => t('Subscription block state when the current user is already subscribed'),
    '#options' => array(
      SSUBSCRIPTION_BLOCK_VISIBILITY_HIDE    => t('Hide block for subscribed users.'),
      SSUBSCRIPTION_BLOCK_VISIBILITY_PREFILL => t('Prefill block with current subscription options.'),
    ),
    '#default_value' => variable_get('ssubscription_delete_user_action', SSUBSCRIPTION_DELETE_USER_NOTHING),
    '#description' => t('Choose action to perform when a subscribed user is deleted from Drupal'),
  );

  return system_settings_form($form);
}



/**
 * Admin form
 */
function simple_subscription_admin_form() {
  $form = array();

  // Build the sortable table header.
  $header = array(
    'mail' => array('data' => 'E-mail', 'field' => 'mail', 'sort' => 'asc'),
    'uid' => array('data' => 'Account', 'field' => 'uid'),
    'status' => array('data' => 'Status', 'field' => 'status'),
    'created' => array('data' => 'Created', 'field' => 'created'),
  );

  // Build the database query.
  $query = db_select('simple_subscription_subscriptions', 's')->fields('s');
  $query->leftJoin('users', 'u', 's.uid = u.uid');
  $query = $query->fields('u', array('name'))
                 ->extend('PagerDefault')
                 ->extend('TableSort')
                 ->orderByHeader($header)
                 ->limit(20);
  $results = $query->execute();

  // Prepare the list of subscribers.
  $options = array();
  foreach ($results as $result) {
    $options[$result->sid] = array(
      'mail' => $result->mail,
      'uid' => $result->uid ? l($result->name, 'user/' . $result->uid) : '',
      'status' => $result->status ? 'Active' : 'Inactive',
      'created' => format_date($result->created, 'short'),
    );
  }

  // Prepare form elements.
  $form['subscriptions'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );
  $form['pager'] = array('#markup' => theme('pager'));

  $form['submit'][] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
  );

  return $form;
}

function simple_subscription_admin_form_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['subscriptions']) || !count(array_filter($form_state['values']['subscriptions']))) {
    form_set_error('', t('No items selected.'));
  }
}

function simple_subscription_admin_form_submit($form, &$form_state) {
  simple_subscription_delete_multiple(array_filter($form_state['values']['subscriptions']));
  $count = count($form_state['values']['subscriptions']);
  drupal_set_message(format_plural($count, '1 subscription deleted.', '@count subscriptions deleted.'));
}

/**
 * Export form
 */
function simple_subscription_export_form() {
  $form = array();

  $form['#prefix'] = t('<p>This form allows you to export subscriptions in a CSV (Comma-Separated Value) file. However, you can define which separator you want to use.</p>');

  $form['delimiter'] = array(
    '#type' => 'select',
    '#title' => t('Delimiter'),
    '#options' => array(
      1 => ',',
      2 => ';',
      3 => 'tab',
    ),
    '#default_value' => ',',
  );

  // TODO: implement more fields
  $form['headers'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Headers'),
    '#options' => array(
      'mail' => t('E-mail address'),
      'created' => t('Date of subscription'),
    ),
    '#default_value' => array('mail'),
    '#required' => TRUE,
    '#description' => t('Define which columns you want to appear in the CSV file.'),
  );

  $form['skip'] = array(
    '#type' => 'checkbox',
    '#title' => t('Skip first row'),
    '#description' => t('If enabled, no headers will be added to the file.'),
  );

  $form['quote'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force quote wrapping'),
    '#description' => t('If enabled, values will always be wrapped between quotes.'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );

  return $form;
}

function simple_subscription_export_form_validate($form, &$form_state) {
  $subscriptions = simple_subscription_load_multiple(FALSE);
  if (empty($subscriptions)) {
    form_set_error('', 'There are no subscriptions to export.');
  }
}

function simple_subscription_export_form_submit($form, &$form_state) {
  $output = '';

  // Build the query
  $fields = array_filter($form_state['values']['headers']);
  $query = db_select('simple_subscription_subscriptions', 's')->fields('s', $fields);
  $results = $query->execute();

  $delimiters = array(
    1 => ',',
    2 => ';',
    3 => 'tab',
  );
  $del = $delimiters[$form_state['values']['delimiter']];

  // Append table headers
  if ($form_state['values']['skip'] == FALSE) {
    foreach ($fields as $field) {
      if ($form_state['values']['quote'] && strstr($form['headers']['#options'][$field], '"') == FALSE) {
        $output .= $form['headers']['#options'][$field] . $del;
      }
      else {
        $output .= '"' . addcslashes($form['headers']['#options'][$field], '"') . '"' . $del;
      }
    }
    $output .= "\n";
  }

  // Format the values as demanded
  foreach ($results as $result) {
    foreach ($fields as $field) {
      $value = _simple_subscribption_format_value($field, $result->$field);
      if ($form_state['values']['quote'] && strstr($value, '"') == FALSE) {
        $output .= $value . $del;
      }
      else {
        $output .= '"' . addcslashes($value, '"') . '"' . $del;
      }
    }
    $output .= "\n";
  }

  // Add HTTP headers
  drupal_add_http_header('Content-type', 'text/csv');
  drupal_add_http_header('Cache-Control', 'no-store, no-cache');
  drupal_add_http_header('Content-Disposition', 'attachment; filename="' . t('subscriptions') . '.csv');

  print $output;
  exit;
}

/**
 * Helper function to format values to be added to the CSV file
 *
 * @param  $field_name
 * @param  $value
 */
function _simple_subscribption_format_value($field_name, $value) {
  switch ($field_name) {
    case 'mail':
      return $value;
    case 'created':
      return format_date($value, 'custom', 'd/m/Y H:i:s');
  }
}

/**
 * Page callback for unsubscribing
 *
 * @param  $subscription
 */
function simple_subscription_unsubscribe_page($subscription) {
  if ($subscription && !$subscription->status) {
    drupal_set_message(t("You have already unsubscribed from our newsletter."));
  }
  else {
    simple_subscription_unsubscribe($subscription->sid);
    drupal_set_message(t("You were successfully removed from the newsletter recipients."));
  }
  drupal_goto('');
}
