<?php
/**
 * @file
 * Commerce Credits Transaction page callbacks.
 */


/**
 * Provide a CTools form to transfer credits that may be embedded as a panel
 * pane or as a node link somewhere.
 *
 * @param $credit_type
 *   The Commerce Credits credit type.
 * @param $entity_type
 *   The entity type to transfer to.
 * @param $entity_id
 *   The entity id to transfer to.
 * @param $js
 *   Whether in ajax or nojs context.
 * @param $from_type
 *   An optional parameter for the entity type to transfer from.
 * @param $from_id
 *   An optional parameter for the entity id to transfer from.
 * @return HTML
 *   Output.
 */
function commerce_credits_transaction_page($credit_type, $entity_type, $entity_id, $js, $from_type = FALSE, $from_id = FALSE) {
  // Load the entity to transfer to.
  $entity = entity_load($entity_type, array($entity_id));
  $entity = is_array($entity) ? array_shift($entity) : FALSE;

  if ($from_type && $from_id && entity_get_info($from_type)) {
    $from_entity = entity_load($from_type, array($from_id));
    $from_entity = is_array($from_entity) ? array_shift($from_entity) : NULL;
  }

  $form_state = array(
    'ajax' => $js,
    'title' => t('Credits Transaction'),
    'entity' => $entity,
    'entity_id' => $entity_id,
    'entity_type' => $entity_type,
    'from_entity' => isset($from_entity) ? $from_entity : NULL,
    'from_type' => $from_type,
    'from_id' => $from_id,
    'credit_type' => $credit_type,
  );

  if (!$js) {
    // Return the normal form.
    return drupal_build_form('commerce_credits_transaction_transfer_form', $form_state);
  }

  ctools_include('ajax');
  ctools_include('modal');

  $output = ctools_modal_form_wrapper('commerce_credits_transaction_transfer_form', $form_state);
  if (!empty($form_state['executed'])) {
    $commands = array();
    $commands[] = ajax_command_remove('#messages');
    $commands[] = ajax_command_invoke('#page', 'trigger', array(
      'commerceCreditsTransactionNotify',
      array(
        'message' => '<div id="messages"><div class="section clearfix">' . theme('status_messages') . '</div></div>',
      ),
    ));
    $commands[] = ctools_modal_command_dismiss();
  }
  else {
    $commands = $output;
  }

  print ajax_render($commands);
  exit;
}

/**
 * Commerce Credits Transaction Transfer Form
 *
 * @param $form
 *   Form array
 * @param &$form_state
 *   Form state array. If no entity, then this will print an error.
 * @return array
 *   Form API array.
 */
function commerce_credits_transaction_transfer_form($form, &$form_state) {
  global $user;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['cancel'] = array(
    '#theme' => 'link',
    '#path' => '',
    '#text' => t('Cancel'),
    '#options' => array(
      'html' => FALSE,
      'attributes' => array(),
    ),
    '#weight' => 42,
  );

  if (!isset($form_state['entity']) || !isset($form_state['credit_type'])) {
    drupal_set_message(t('An error occurred executing this form.'), 'error', FALSE);
    return $form;
  }

  if (!isset($form_state['from_entity'])) {
    drupal_set_title(t('Transfer credits to !label', array('!label' => entity_label($form_state['entity_type'], $form_state['entity']))));
    $credits = entity_get_controller('commerce_credits')->loadByEntity($user->uid, 'user', $form_state['credit_type']);
    $credits = is_array($credits) ? array_shift($credits) : FALSE;
  }
  else {
    $from_label = entity_label($form_state['from_type'], $form_state['from_entity']);
    $to_label = entity_label($form_state['entity_type'], $form_state['entity']);
    drupal_set_title(t('Transfer credits from !from to !to', array('!from' => $from_label, '!to' => $to_label)));
    $credits = entity_get_controller('commerce_credits')->loadByEntity($form_state['from_id'], $form_state['from_type'], $form_state['credit_type']);
    $credits = is_array($credits) ? array_shift($credits) : FALSE;
  }

  if (!$credits || $credits->credits == 0) {
    // From entity does not have ANY credits at all!
    $form['error'] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('messages', 'error')),
      'message' => array(
        '#markup' => t('There are not enough credits to fulfill this transaction. You may purchase more credits in the store.'),
      ),
    );
    return $form;
  }

  $form_state['from_credits'] = $credits;
  $max = format_plural($credits->credits, '1 credit', '@count credits');
  $form['credits'] = array(
    '#type' => 'textfield',
    '#title' => t('Credits'),
    '#description' => t('Please provide the number of credits to transfer up to !max', array('!max' => $max)),
    '#default_value' => 1,
    '#required' => TRUE,
  );

  // Finally add submit button.
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Pay With Credits'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Commerce Credits Transaction transfer form validate
 */
function commerce_credits_transaction_transfer_form_validate($form, &$form_state) {
  if (!filter_var($form_state['values']['credits'], FILTER_VALIDATE_INT) || $form_state['values']['credits'] < 0) {
    // min_range does not work in filter_var if max_range not provided.
    form_error($form['credits'], t('You must provide a valid, positive number of credits.'));
  }

  if ($form_state['values']['credits'] > $form_state['from_credits']->credits) {
    form_error($form['credits'], t('There are not enough credits to fulfill this transaction.'));
  }

  if ($form_state['from_credits']->entity_type == $form_state['entity_type'] && $form_state['from_credits']->entity_id == $form_state['entity_id']) {
    form_error($form, t('You cannot transfer credits to yourself.'));
  }
}

/**
 * Commerce Credits Transaction transfer form submit.
 */
function commerce_credits_transaction_transfer_form_submit($form, &$form_state) {
  $from_credits = $form_state['from_credits'];
  $credits = $form_state['values']['credits'];
  $controller = entity_get_controller('commerce_credits');

  $to_credits = $controller->loadByEntity($form_state['entity_id'], $form_state['entity_type'], $form_state['credit_type']);
  if (!$to_credits || !is_array($to_credits)) {
    // Create a credits entity if it does not exist yet.
    $to_credits = $controller->createByEntity($form_state['entity_id'], $form_state['entity_type'], $form_state['credit_type']);
  }
  else {
    $to_credits = array_shift($to_credits);
  }

  try {
    // Transfer credits with the pending status.
    $controller->transferCreditWithTransaction($from_credits, $to_credits, $credits, 'pending');

    $plural = format_plural($credits, '1 credit', '@count credits');
    $to_label = entity_label($form_state['entity_type'], $form_state['entity']);
    drupal_set_message(t('Successfully paid !credits to %label. This transaction is pending, and may be reversed at a later date.', array('!credits' => $plural, '%label' => $to_label)), 'status', FALSE);
  }
  catch (Exception $e) {
    watchdog_exception('commerce_credits', $e, 'An error occurred attempting to transfer credits: %message', array('%message' => $e->getMessage()), WATCHDOG_CRITICAL);
    drupal_set_message(t('An error occurred attempting to transfer credits.'), 'error', FALSE);
    $form_state['rebuild'] = TRUE;
  }
}
