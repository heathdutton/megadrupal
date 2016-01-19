<?php
/**
 * @file
 * Subs pages.
 */

/**
 * Page callback.
 * Displays page to select subscription type when adding new subscription.
 *
 * @see subs_menu()
 */
function subs_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);
  $count = count($content);

  // Bypass the subscription/add listing if only one subscription type is available.
  if ($count == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }
  // Display warning message if no subscription types are available.
  elseif ($count === 0) {
    return t("You haven't created any !types yet!", array('!types' => l(t('Subscription Types'), 'admin/structure/subscriptions')));
  }

  return theme('node_add_list', array('content' => $content));
}

/**
 * Page callback.
 * Displays add new subscription page for the selected type.
 *
 * @see subs_menu()
 */
function subs_add($type) {
  $subscription_type = subs_get_types($type);
  $subscription = entity_create('subs', array('type' => $type));

  // Check if user can create a subscription of this type.
  if (!subs_access('create', $subscription)) {
    drupal_set_message(t("You can't create any more subscriptions."), 'error', FALSE);
    drupal_access_denied();
    exit;
  }

  drupal_set_title(t('Create @name Subscription', array('@name' => $subscription_type->getTranslation('label'))));
  $output = drupal_get_form('subs_form', $subscription);
  return $output;
}

/**
 * Form callback.
 * Generates the subscription form.
 *
 * @see subs_menu()
 */
function subs_form($form, &$form_state, $subscription) {
  $form_state['subscription'] = $subscription;
  $subscription_type = subs_get_types($subscription->type);
  $aid = entity_id('subs', $subscription);
  $administer_subs = user_access('administer subs');

  // Only show Start Date if user has permission.
  if ($administer_subs) {
    $form['subs_start'] = array(
      '#type' => 'fieldset',
      '#title' => t('Start date'),
      '#collapsible' => FALSE,
      '#weight' => 2,
    );

    $form['subs_start']['auto'] = array(
      '#title' => t('Start automatically when subscription becomes active'),
      '#description' => t('Caution: If the subscription is already active and you check this, the start date will be recalculated from today.'),
      '#type' => 'checkbox',
      '#default_value' => $subscription->date_start == 0,
    );

    if ($subscription->date_start != 0) {
      $date_start = date('Y-m-d', $subscription->date_start);
    }
    else {
      $date_start = date('Y-m-d');
    }

    $date_start = explode('-', $date_start);
    $date_start = array(
      'year' => (int) $date_start[0],
      'month' => (int) $date_start[1],
      'day' => (int) $date_start[2],
    );

    $form['subs_start']['date_start'] = array(
      '#type' => 'date',
      '#title' => t('Start on a specific date'),
      '#default_value' => $date_start,
      '#states' => array(
        'visible' => array(
          'input[name="auto"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  // Only show End Date if editing, subscription started and user has permission.
  if (!empty($aid) && $administer_subs && $subscription->date_start) {
    // Only provide value if already set.
    if ($subscription->date_end > 0) {
      $date_end = date('Y-m-d', $subscription->date_end);
      $date_end = explode('-', $date_end);
      $date_end = array(
        'year' => (int) $date_end[0],
        'month' => (int) $date_end[1],
        'day' => (int) $date_end[2],
      );
    }
    else {
      $date_end = NULL;
    }

    $form['subs_end'] = array(
      '#type' => 'fieldset',
      '#title' => t('End date'),
      '#collapsible' => FALSE,
      '#weight' => 3,
    );

    $form['subs_end']['display'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set end date'),
      '#default_value' => isset($date_end),
    );

    $form['subs_end']['date_end'] = array(
      '#type' => 'date',
      '#title' => t('End Date'),
      '#default_value' => $date_end,
      '#states' => array(
        'visible' => array(
          'input[name="display"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  // Only show user and status fields if user has permission.
  if ($administer_subs) {
    if (isset($subscription->uid)) {
      $user = user_load($subscription->uid);
    }

    $form['uid'] = array(
      '#type' => 'textfield',
      '#title' => t('User'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => isset($user) ? $user->name : '',
      '#required' => TRUE,
      '#weight' => 0,
    );

    $form['status'] = array(
      '#type' => 'select',
      '#title' => t('Status'),
      '#options' => subs_status_options_list(),
      '#default_value' => isset($subscription->status) ? $subscription->status : $subscription_type->default_status,
      '#weight' => 1,
    );
  }

  field_attach_form('subs', $subscription, $form, $form_state);

  $submit = array();
  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions'] = array(
    '#weight' => 100,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save subscription'),
    '#submit' => $submit + array('subs_form_submit'),
  );

  // Display delete button if we're editing the subscription and have
  // permission.
  if (!empty($aid) && subs_access('delete', $subscription)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#limit_validation_errors' => array(),
      '#submit' => array('subs_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Validatation callback.
 *
 * @see subs_form()
 */
function subs_form_validate(&$form, &$form_state) {
  $subscription = $form_state['subscription'];

  // If we have a "uid" field submitted, the subscription is being created on
  // behalf of a user.
  if (isset($form_state['values']['uid'])) {
    // Check whether we have a valid user.
    if ($user = user_load_by_name($form_state['values']['uid'])) {
      // If we're creating a new subscription, check whether the current user
      // has permissions.
      if (isset($subscription->is_new)) {
        // Load current user.
        $account = user_uid_optional_load();

        if (subs_access('create', $subscription, $account)) {
          form_set_value($form['uid'], $user->uid, $form_state);
        }
        else {
          form_set_error('uid', t("You can't create subscriptions of this type!"));
        }
      }
    }
    // Bummer.
    else {
      form_set_error('uid', t('Invalid user.'));
    }
  }
  // If not, a user is creating a subscription for himself.
  else {
    $user = user_uid_optional_load();
    form_set_value($form['uid'], $user->uid, $form_state);
  }

  if (isset($form_state['values']['status'])) {
    // Make sure we have a valid status.
    $options = subs_status_options_list();
    if (!array_key_exists($form_state['values']['status'], $options)) {
      form_set_error('status', t('Invalid subscription status.'));
    }
  }
  // If we have no status field, the user doesn't have administer subs
  // permission so just assume the default status for this type.
  else {
    $subscription_type = subs_get_types($subscription->type);
    form_set_value($form['status'], $subscription_type->default_status, $form_state);
  }
}

/**
 * Submit callback.
 * Saves a subscription.
 *
 * @see subs_form()
 */
function subs_form_submit(&$form, &$form_state) {
  $subscription = $form_state['subscription'];
  // For convenience.
  $date_start = & $form_state['values']['date_start'];
  $date_end = & $form_state['values']['date_end'];

  // Automatic start date.
  if ($form_state['values']['auto'] == 1) {
    if ($form_state['values']['status'] == SUBS_STATUS_ACTIVE) {
      if ($subscription->status != SUBS_STATUS_ACTIVE) {
        $date_start = REQUEST_TIME;
        // Find subscription length.
        $subscription_type = subs_get_types($subscription->type);
        if ($subscription_type->length > 0) {
          $date_end = $date_start + $subscription_type->length;
        }
        else {
          $date_end = 0;
        }
      }
    }
    else {
      $date_start = 0;
    }
  }
  else {
    // Convert start date to timestamp.
    $date_start = strtotime("{$date_start['year']}-{$date_start['month']}-{$date_start['day']}");

    if (isset($form_state['values']['date_end'])) {
      if ($form_state['values']['display']) {
        // Convert end date to timestamp.
        $date_end = strtotime("{$date_end['year']}-{$date_end['month']}-{$date_end['day']}");
      }
      else {
        $date_end = 0;
      }
    }
  }

  unset($form_state['values']['auto']);

  // Build entity.
  entity_form_submit_build_entity('subs', $subscription, $form, $form_state);

  // Save entity.
  subs_save($subscription);

  // Redirect.
  $subscription_uri = entity_uri('subs', $subscription);
  $form_state['redirect'] = $subscription_uri['path'];

  // Success message.
  drupal_set_message(t('Subscription %title saved.', array('%title' => entity_label('subs', $subscription))));
}

/**
 * Submit callback.
 * Redirects to subscription delete confirmation form.
 *
 * @see subs_form()
 */
function subs_form_submit_delete($form, &$form_state) {
  $subscription = $form_state['subscription'];
  $subscription_uri = entity_uri('subs', $subscription);
  $redirect_path = $subscription_uri['path'] . '/delete';
  $form_state['redirect'] = $redirect_path;
}

/**
 * Form callback.
 * Displays subscription delete confirmation form.
 *
 * @see subs_menu()
 */
function subs_delete_form($form, &$form_state, $subscription) {
  $form_state['subscription'] = $subscription;

  // Always provide entity id in the same form key as in the entity edit form.
  $form['aid'] = array('#type' => 'value', '#value' => entity_id('subs', $subscription));
  $subscription_uri = entity_uri('subs', $subscription);
  return confirm_form($form, t('Are you sure you want to delete subscription %title?', array('%title' => entity_label('subs', $subscription))), $subscription_uri['path'], t('This action cannot be undone.'), t('Delete'), t('Cancel'));
}

/**
 * Submit handler.
 * Deletes an subscription.
 *
 * @see subs_delete_form()
 */
function subs_delete_form_submit($form, &$form_state) {
  $subscription = $form_state['subscription'];

  // Delete subscription.
  subs_delete($subscription);

  // Redirect to front page.
  $form_state['redirect'] = '<front>';

  // Success message.
  drupal_set_message(t('Subscription %title deleted.', array('%title' => entity_label('subs', $subscription))));
}