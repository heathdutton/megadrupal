<?php

/**
 * @file
 * Provides the Congratulations on anniversary order's administrative interface.
 */

/**
 * Implements hook_form().
 *
 * Creates form for a module's admin page.
 *
 * @see congratulations_on_anniversary_order_form_validate()
 * @see congratulations_on_anniversary_order_submit()
 */
function congratulations_on_anniversary_order_form($form, &$form_state) {
  // Get the number of the last already-completed order.
  $last_order_num = congratulations_on_anniversary_order_get_last_order_number();

  // Display information about currently-set anniversary order number.
  $anniv_order_number_absolute_val = variable_get('congratulations_on_anniversary_order_anniv_order_number_absolute_val');
  if (empty($anniv_order_number_absolute_val)) {
    $form['current_anniv_order_info'] = array(
      '#markup' => '<p>Anniversary order number was not set yet.</p>',
    );
  }
  else {
    $form['current_anniv_order_info'] = array(
      '#markup' => '<p>Currently-set anniversary order number is ' . $anniv_order_number_absolute_val . '</p>',
    );
  }

  // Field to enter anniversary order's number.
  $form['anniv_order_number'] = array(
    '#type' => 'textfield',
    '#title' => t('The anniversary order number'),
    // Show number of the last completed order in your store.
    '#description' => t('Last order number is @last_order_num', array('@last_order_num' => $last_order_num)),
    // Limit allowed field values to positive integers.
    '#element_validate' => array('element_validate_integer_positive'),
    '#default_value' => variable_get('congratulations_on_anniversary_order_anniv_order_number'),
    '#required' => TRUE,
  );
  // Create checkbox to switch to counting of anniversary order number from the
  // last order number, rather than from 0.
  $form['order_number_is_relative'] = array(
    '#type' => 'checkbox',
    '#title' => t("Relative anniversary order number. If you'll check it and set anniversary order number to 3, while there were 103 orders in your shop, absolute anniversary order number will be 106"),
    '#default_value' => variable_get('congratulations_on_anniversary_order_order_number_is_relative'),
  );
  // Create checkbox to switch on or off e-mail notifications to user about the
  // fact that he'd made an anniversary order.
  $form['send_mail_to_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send mail to user about his anniversary order'),
    '#description' => t('Send mail to user abot his anniversary order'),
    '#default_value' => variable_get('congratulations_on_anniversary_order_send_mail_to_user'),
  );
  // Set e-mail title (sent to user who've created an anniversary order).
  $form['title_message_user'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail subject'),
    '#default_value' => variable_get('congratulations_on_anniversary_order_title_message_user', '[site:name]. Your order is an anniversary order.'),
    '#description' => t('Title of congratulational e-mail to your customer'),
    // '#states' sets a field as dependable on the other field.
    // If admin chosed to send e-mail notifications, title_message_user field is
    // being marked as mandatory by star sign.
    '#states' => array(
      'required' => array(
        ':input[name="send_mail_to_user"]' => array('checked' => TRUE),
      ),
    ),
  );
  // Set e-mail body (sent to user who've created an anniversary order).
  $form['body_message_user'] = array(
    '#type' => 'textarea',
    '#title' => t('E-mail body'),
    '#default_value' => variable_get('congratulations_on_anniversary_order_body_messsage_user', 'Congratulations on anniversary order!'),
    '#description' => t('Tell your customer about his next steps to get reward'),
    // '#states' sets a field as dependable on the other field.
    // If admin chosed to send e-mail notifications, body_message_user field is
    // being marked as mandatory by star sign.
    '#states' => array(
      'required' => array(
        ':input[name="send_mail_to_user"]' => array('checked' => TRUE),
      ),
    ),
  );
  if (module_exists('token')) {
    // List of all tokens you can use for your message title and body.
    $form['token_tree'] = array(
      '#type' => 'fieldset',
      '#title' => t('Available tokens list'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#theme' => 'token_tree',
      '#token_types' => array('commerce-order'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 *
 * Form validation handler for congratulations_on_anniversary_order_form().
 *
 * @see congratulations_on_anniversary_order_form()
 * @see congratulations_on_anniversary_order_submit()
 */
function congratulations_on_anniversary_order_form_validate($form, &$form_state) {
  // If anniversary order number ISN'T calculated from the last order number.
  if ($form_state['values']['order_number_is_relative'] == 0) {
    // Try to load order with admin-provided anniversary number.
    $order_exists = db_query("SELECT order_id FROM {commerce_order} WHERE order_id = :orderNum", array(':orderNum' => $form_state['values']['anniv_order_number']))->fetchField();
    // If order with provided number is already exists, it can't be
    // anniversary order.
    if ($order_exists) {
      form_set_error($order_exists, t('Order already exists'));
    }
  }
  // If admin chosed to notify customer about his anniversary order by e-mail.
  if ($form_state['values']['send_mail_to_user'] == 1) {
    // But haven't set e-mail title and/or body, that's an error.
    if (empty($form_state['values']['title_message_user'])) {
      form_set_error('title_message_user', t('Please specify title of the e-mail.'));
    }
    elseif (empty($form_state['values']['body_message_user'])) {
      form_set_error('body_message_user', t('Please specify body of the e-mail.'));
    }
  }
}

/**
 * Implements hook_form_submit().
 *
 * Form submission handler for congratulations_on_anniversary_order_form().
 * Saves the anniversary order number and other module's settings.
 *
 * @see congratulations_on_anniversary_order_form()
 * @see congratulations_on_anniversary_order_form_validate()
 */
function congratulations_on_anniversary_order_form_submit($form, &$form_state) {
  variable_set('congratulations_on_anniversary_order_anniv_order_number', $form_state['values']['anniv_order_number']);
  variable_set('congratulations_on_anniversary_order_send_mail_to_user', $form_state['values']['send_mail_to_user']);
  variable_set('congratulations_on_anniversary_order_order_number_is_relative', $form_state['values']['order_number_is_relative']);
  variable_set('congratulations_on_anniversary_order_title_message_user', $form_state['values']['title_message_user']);
  variable_set('congratulations_on_anniversary_order_body_messsage_user', $form_state['values']['body_message_user']);

  // Setting to determine if user-entered anniversary order number is relative
  // or absolute.
  $order_number_is_relative = $form_state['values']['order_number_is_relative'];

  // Save absolute number of an anniversary order.
  if ($order_number_is_relative == 1) {
    // If we should count anniversary order number from the last existing order.
    $last_order_num = congratulations_on_anniversary_order_get_last_order_number();
    $anniv_order_number_absolute_val = $form_state['values']['anniv_order_number'] + $last_order_num;
    variable_set('congratulations_on_anniversary_order_anniv_order_number_absolute_val', $anniv_order_number_absolute_val);
  }
  else {
    // If user have entered absolute anniversary order number himself.
    $anniv_order_number_absolute_val = $form_state['values']['anniv_order_number'];
    variable_set('congratulations_on_anniversary_order_anniv_order_number_absolute_val', $anniv_order_number_absolute_val);
  }

  drupal_set_message(t('The configuration options have been saved. Anniversary order number is @anniv_order_number_result', array('@anniv_order_number_result' => $anniv_order_number_absolute_val)));
}

/**
 * Function to get the number of the last order made at the store.
 *
 * @return int
 *   The last order made at the store.
 */
function congratulations_on_anniversary_order_get_last_order_number() {
  $query = db_select('commerce_order', 'co');
  $query->addExpression('MAX(order_id)');
  $last_order_num = $query->execute()->fetchField();
  if (!($last_order_num)) {
    $last_order_num = 0;
  }
  return $last_order_num;
}
