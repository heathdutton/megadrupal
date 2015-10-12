<?php

/**
 * @file
 *
 * Page callbacks for the add, update, delete for the subscription form.
 *
 * @author: ratanphp
 */

 /**
  * Subscription form.
  */
function user_recurring_subscription_add_form($form, &$form_state, $product = NULL) {

  $form['sid'] = array(
    '#type' => 'hidden',
    '#value' => isset($product->sid) ? $product->sid : '',
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => isset($product->title) ? $product->title : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#description' => t('Subscription plan description'),
    '#default_value' => isset($product->description) ? $product->description : '',
  );

  $form['fee'] = array(
    '#type' => 'textfield',
    '#title' => t('Setup fee'),
    '#default_value' => isset($product->fee) ? $product->fee : '',
    '#size' => 10,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#field_prefix' => USER_RECURRING_SUBSCRIPTION_USD,
    '#description' => t('Recurring profile setup fee. If initial charge duration is 0 days then it will be added in first recurring billing cycle. <br /> If initial charge or recurring profile creatation date is future date then only fee will be charge at the time of profile creation. its a one time charge.'),
  );

  $form['price'] = array(
    '#type' => 'textfield',
    '#title' => t('Recurring fee amount'),
    '#default_value' => isset($product->price) ? $product->price : '',
    '#size' => 10,
    '#maxlength' => 128,
    '#field_prefix' => USER_RECURRING_SUBSCRIPTION_USD,
    '#required' => TRUE,
    '#description' => t('Charge this amount each billing period.'),
  );

  $form['interval'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Interval Settings'),
    '#collapsible' => FALSE,
    '#description' => t('This section specifies when the recurring amount will be charged.'),
  );

  $form['interval']['initial'] = array(
    '#type' => 'fieldset',
    '#title' => t('Initial charge'),
    '#collapsible' => FALSE,
    '#description' => t('Specify the time to wait to start charging the recurring fee after creating the profile.'),
    '#attributes' => array('class' => array('interval-fieldset')),
  );

  $form['interval']['initial']['initial_charge_value'] = array(
    '#type' => 'select',
    '#title' => '',
    '#options' => drupal_map_assoc(range(0, 52)),
    '#default_value' => isset($product->initial_charge_value) ? $product->initial_charge_value : 1,
  );

  $form['interval']['initial']['initial_charge_unit'] = array(
    '#type' => 'select',
    '#options' => array(
      'days' => t('day(s)'),
      'weeks' => t('week(s)'),
      'months' => t('month(s)'),
      'years' => t('year(s)'),
    ),
    '#default_value' => isset($product->initial_charge_unit) ? $product->initial_charge_unit : 'months',
  );

  $form['interval']['regular'] = array(
    '#type' => 'fieldset',
    '#title' => t('Regular interval'),
    '#collapsible' => FALSE,
    '#description' => t('Specify the length of the billing period for this fee.'),
    '#attributes' => array('class' => array('interval-fieldset')),
  );

  $form['interval']['regular']['regular_interval_unit'] = array(
    '#type' => 'select',
    '#options' => array(
      'DAYS' => t('Daily'),
      'WEEK' => t('Weekly'),
      'BIWK' => t('Every Two Weeks'),
      'SMMO' => t('Twice Every Month'),
      'FRWK' => t('Every Four Weeks'),
      'MONT' => t('Monthly'),
      'QTER' => t('Quarterly'),
      'SMYR' => t('Twice Every Year'),
      'YEAR' => t('Yearly'),
    ),
    '#default_value' => isset($product->regular_interval_unit) ? $product->regular_interval_unit : 'months',
  );

  $roles = user_roles();
  $form['assigned_role'] = array(
    '#type' => 'select',
    '#title' => t('Role assign'),
    '#options' => array(NULL => '-Select-') + $roles,
    '#default_value' => isset($product->assigned_role) ? $product->assigned_role : NULL,
    '#description' => t('Role assigned to the user once successfully purchers the subscription.'),
    '#required' => TRUE,
  );

  $form['remove_role'] = array(
    '#type' => 'select',
    '#title' => t('Role removed'),
    '#options' => array(NULL => '-Select-') + $roles,
    '#default_value' => isset($product->remove_role) ? $product->remove_role : NULL,
    '#description' => t('Role remove from the user once recurring payment is failed.'),
    '#required' => TRUE,
  );

  $form['is_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Active.'),
    '#description' => t('If active then this subscription will avaiable for new users. If not then it will display only for those user who purches it earliear.'),
    '#attributes' => !empty($product->is_active) ? array('checked' => 'checked') : array(),
  );

  $form['is_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Default.'),
    '#description' => t('If checked then this subscription plan is default selected.'),
    '#attributes' => !empty($product->is_default) ? array('checked' => 'checked') : array(),
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Published.'),
    '#description' => t('If active then this subscription will avaiable for new users.'),
    '#attributes' => !empty($product->status) ? array('checked' => 'checked') : array(),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Subscription validate handler.
 */
function user_recurring_subscription_add_form_validate($form, &$form_state) {
  $fee = trim($form_state['values']['fee']);
  $price = trim($form_state['values']['price']);

  if (!filter_var($fee, FILTER_VALIDATE_FLOAT) || $fee <= 0) {
    form_set_error('fee', t('Invalid recurring profile fee'));
  }
  if (!filter_var($price, FILTER_VALIDATE_FLOAT) || $price <= 0) {
    form_set_error('price', t('Invalid recurring profile price'));
  }
}

/**
 * Subscription submit handler.
 */
function user_recurring_subscription_add_form_submit($form, &$form_state) {
  global $user;
  $values = (object) $form_state['values'];
  $values->initial_charge_unit = $values->initial_charge_value . ' ' . $values->initial_charge_unit;
  if (empty($values->sid)) {
    $values->uid = $user->uid;
    $values->created = REQUEST_TIME;
    $values->changed = REQUEST_TIME;
    drupal_write_record('subscription', $values);
    drupal_set_message(t('Subscription add successfully.'), 'status');

  }
  else {
    $values->changed = REQUEST_TIME;
    drupal_write_record('subscription', $values, array('sid'));
    drupal_set_message(t('Subscription updated successfully.'), 'status');
  }
}

/**
 * Subscription manage list.
 */
function user_recurring_subscription_manage() {

  $header = array(
    'title' => array('data' => t('Title'), 'field' => 's.title'),
    'initial_interval' => array('data' => t('Free Trial')),
    'regular_interval' => array('data' => t('Billing Cycle')),
    'fee' => array('data' => t('Setup Fee')),
    'price' => t('Recurring Amount'),
    'status' => array('data' => t('Status')),
    'changed' => array(
      'data' => t('Updated'),
      'field' => 's.changed',
      'sort' => 'desc'),
    'operations' => array('data' => t('Operations')),
  );

  $query = db_select('subscription', 's')->extend('PagerDefault')->extend('TableSort');
  $sids = $query
    ->fields('s', array('sid'))
    ->limit(50)
    ->orderByHeader($header)
    ->execute()
    ->fetchCol();

  $destination = drupal_get_destination();
  $options = array();
  foreach ($sids as $sid) {
    $product = user_recurring_subscription_load($sid);
    $options[$product->sid] = array(
      'title' => check_plain($product->title),
      'initial_interval' => check_plain($product->initial_charge_unit),
      'regular_interval' => check_plain($product->regular_interval_unit),
      'fee' => !empty($product->fee) ? '$' . $product->fee : '$0.00',
      'price' => !empty($product->price) ? '$' . $product->price : '$0.00',
      'status' => $product->status ? t('published') : t('not published'),
      'changed' => format_date($product->changed, 'short'),
    );

    // Build a list of all the accessible operations for the current node.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'admin/config/subscription/' . $product->sid . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'admin/config/subscription/' . $product->sid . '/delete',
      'query' => $destination,
    );
    $options[$product->sid]['operations'] = array();
    if (count($operations) > 1) {
      // Render an unordered list of operations links.
      $options[$product->sid]['operations'] = array(
        'data' => array(
          '#theme' => 'links__node_operations',
          '#links' => $operations,
          '#attributes' => array('class' => array('links', 'inline')),
        ),
      );
    }
  }
  $output  = theme('table', array('header' => $header, 'rows' => $options));
  $output .= theme('pager', array('tags' => NULL, 'element' => 0));
  return $output;
}

/**
 * Menu callback; delete a single subscription plan.
 *
 * @ingroup forms
 */
function user_recurring_subscription_delete_confirm($form, &$form_state, $subscription) {
  $form['sid'] = array('#type' => 'value', '#value' => $subscription->sid);
  $form['title'] = array('#type' => 'value', '#value' => $subscription->title);

  $message = t('Are you sure you want to delete the subscription %title?', array('%title' => $subscription->title));
  $caption = '';
  $label = 'Delete';
  $num = db_query("SELECT COUNT(*) FROM {recurring} WHERE sid = :sid AND status = :status", array(':sid' => $subscription->sid, ':status' => USER_RECURRING_SUBSCRIPTION_ACTIVE))->fetchField();
  if ($num) {
    $caption .= '<p>' . t('%title is used by @count users in their recurring profile. You can not delete this subscription. You can only unpublish this subscription', array('%title' => $subscription->title, '@count' => $num)) . '</p>';
    $label = 'Unpublish';
  }

  $caption .= '<p>' . t('This action cannot be undone.') . '</p>';

  return confirm_form($form, $message, 'admin/config/subscription/manage', $caption, $label);
}

/**
 * Process subscription plan delete confirm submissions.
 *
 * @see user_recurring_subscription_delete_confirm()
 */
function user_recurring_subscription_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Delete') {
    db_delete('subscription')
        ->condition('sid', $form_state['values']['sid'])
        ->execute();
  }
  else {
    db_update('subscription')
        ->fields(array('status' => 0))
        ->condition('sid', $form_state['values']['sid'])
        ->execute();
  }
  $t_args = array('%name' => $form_state['values']['title']);
  drupal_set_message(t('The subscription %name has been deleted.', $t_args));
  watchdog('user_recurring_subscription_delete', 'Deleted subscription %name.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/config/subscription/manage';
}
