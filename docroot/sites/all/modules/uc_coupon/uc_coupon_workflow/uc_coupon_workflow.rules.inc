<?php
/**
 * @file
 * Rules integration for uc_coupon_workflow
 */

/**
 * Implements hook_rules_condition_info().
 */

/**
 * Implements hook_rules_event_info().
 * 
 */
function uc_coupon_workflow_rules_event_info() {
  // Event invoked when a valid coupon is applied.
  $events['uc_coupon_workflow_applied'] = array(
    'label' => t('A valid discount coupon is applied'),
    'group' => t('Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
  );
  
  // Event invoked when a valid coupon is applied.
  $events['uc_coupon_workflow_removed'] = array(
    'label' => t('A valid discount coupon is removed'),
    'group' => t('Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
  );
  
  // Event invoked when checking for automatic coupons.
  $events['uc_coupon_workflow_automatic'] = array(
    'label' => t('Check for automatic discounts.'),
    'group' => t('Coupon'),
    'variables' => array(
      'current_order' => array(
        'type' => 'uc_order',
        'label' => t('Current Order'),
      ),
    ),
  );
  
  return $events;
}

/**
 * Implements hook_rules_condition_info().
 */
function uc_coupon_workflow_rules_condition_info() {
  $conditions['uc_coupon_workflow_suspended'] = array(
    'label' => t('Check if an administrator has suspended automatic coupon workflow for this request.'),
    'group' => t('Coupon'),
  );
  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function uc_coupon_workflow_rules_action_info() {
  $actions = array();
  
  // Assign a coupon to a user.
  $actions['uc_coupon_workflow_assign'] = array(
    'label' =>  t('Authorize a user to use a coupon.'),
    'group' => t('Coupon'),
    'provides' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
    'parameter' => array(
      'account' => array(
        'type' => 'user',
        'label' => t('Account'),    
        'description' => t('Select the user account you wish to authorize.'),
      ), 
      'cid' => array(
        'type' => 'integer',
        'label' => t('Coupon'),
        'description' => t('Select the base coupon to be assigned. 
            The new user will be added to the list of authorized users for the selected coupon.'), 
        'options list' => 'uc_coupon_workflow_cid_options',
        'restriction' => 'input',
      ),
    ),
  );

  // Send an e-mail regarding a coupon.
  $actions['uc_coupon_workflow_email'] = array(
    'label' =>  t('Send an email regarding a coupon.'),
    'group' => t('Coupon'),
    'parameter' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
      'from' => array(
        'type' => 'text',
        'label' => t('Sender'),
        'restriction' => 'input',
      ),
      'addresses' => array(
        'type' => 'text',
        'label' => t('Recipients'),
        'description' => t('Enter the email addresses to receive the notifications, one per line.'),
        'restriction' => 'input',
      ),
      'subject' => array(
        'type' => 'text',
        'label' => t('Subject'),
        'description' => t('Enter the subject line for the notifications.'),
        'restriction' => 'input',
      ),
      'message' => array(
        'type' => 'text',
        'label' => t('Message'),
        'description' => t('Enter the message body for the notifications.'),
        'restriction' => 'input',
      ),
      'format' => array(
        'type' => 'text',
        'label' => t('Message format'),
        'options list' => 'uc_coupon_workflow_message_formats',
      ),
    ),
  );
  
  // Apply an automatic coupon.
  $actions['uc_coupon_workflow_apply'] = array(
    'label' =>  t('Apply a coupon to the current order.'),
    'group' => t('Coupon'),
    'provides' => array(
      'coupon' => array(
        'type' => 'uc_coupon',
        'label' => t('Coupon'),
      ),
    ),
    'parameter' => array(
      'cid' => array(
        'type' => 'integer',
        'label' => t('Coupon'),
        'description' => t('Select the coupon to apply.  The coupon will only be
            applied if and when all of its restrictions are satisfied.'),
        'options list' => 'uc_coupon_workflow_apply_options',
        'restriction' => 'input',
      ),
      'mode' => array(
        'type' => 'text',
        'label' => t('Mode'),
        'description' => t('Choose the way this coupon should be applied.'),
        'options list' => 'uc_coupon_workflow_apply_modes',
        'default value' => 'retain',
        'restriction' => 'input',
      ),
    ),
  );
  
  return $actions;
}

/**
 * Options list callback for list of coupons to assign.
 */
function uc_coupon_workflow_cid_options() {
  return db_query('SELECT cid, name FROM {uc_coupons} WHERE status = 1')->fetchAllKeyed();
}

/**
 * Options list callback for message formats.
 */
function uc_coupon_workflow_message_formats() {
  global $user;

  $options = array();
  $formats = filter_formats($user);
  foreach ($formats as $format) {
    $options[$format->format] = $format->name;
  }
  return $options;
}


/**
 * For certain events (e.g. new user creation), administrators are provided
 * with an interface which allows them to prevent execution of coupon actions
 * based on those events.
 */
function uc_coupon_workflow_suspended($settings) {
  $auth = drupal_static('uc_coupon_workflow_suspended');
  return !empty($auth);
}


/**
 * Action callback to assign a coupon to a user.
 */
function uc_coupon_workflow_assign($account, $cid) {
  $coupon = uc_coupon_load($cid);
  if ($coupon->cid) {
    // Grant access to the coupon.
    $coupon->data['users'][$account->uid] = $account->uid;
    uc_coupon_save($coupon);
    return array('coupon' => $coupon);
  }
  else {
    return array('coupon' => NULL);
  }
}

/**
 * Action callback to send an e-mail regarding a coupon.
 */
function uc_coupon_workflow_email($coupon, $from, $addresses, $subject, $message, $format) {
  // Build the e-mail parameters.
  $params = array(
    'from' => $from,
    'subject' => $subject,
    'message' => $message,
    'format' => $format,
    'replacements' => array(), // The replacements are already handled by Rules.
  );
  
  // Split up our recipient e-mail addresses.
  $recipients = array();
  foreach (explode("\n", $addresses) as $address) {
    $recipients[] = trim($address);
  }

  // Send the e-mails.
  foreach ($recipients as  $email) {
    $sent = drupal_mail('uc_order', 'action-mail', $email, uc_store_mail_recipient_language($email), $params, $from);
    if (!$sent['result']) {
      watchdog('uc_coupon', 'Attempt to e-mail @email concerning coupon @cid failed.', array('@email' => $email, '@cid' => $coupon->cid), WATCHDOG_ERROR);
    }
  }
}

/**
 * Action callback to apply a coupon.
 * This simply adds the coupon to the session; it is validated later.
 */
function uc_coupon_workflow_apply($cid, $mode) {
  $coupon = uc_coupon_load($cid);
  if ($coupon->cid) {
    uc_coupon_session_add($coupon->code, $mode);
    return array('coupon' => $coupon);
  }
  else {
    return array('coupon' => NULL);
  }
}

function uc_coupon_workflow_apply_help() {
  return '<p>' . 
          t('This action can be used in two ways: to apply a coupon normally in response to a discrete event
            (for example, when a particular user logs in), or to apply an automatic discount in response to the 
            "%event" event, which occurs every time the current cart contents are updated (for example, to 
            apply a discount if a particular combination of products is ordered).', 
            array('%event' => t('Check for automatic discounts'))) . 
          '</p><p>' .
          t('Automatic discounts are treated differently than normal coupons: they are listed in separate panes
             on the checkout and cart pages, and they may not be removed by the customer. Unlike normal coupons,
             they are automatically removed if the rule\'s conditions are not met.') . 
          '</p><p>' .
          t('Note that a coupon specified in this action will only apply to an order if its restrictions
             (as specified on the coupon add/edit page) are met. That is, both the conditions of this rule
             and the coupon\'s restrictions must be satisfied.  Note also that normal coupons always take
             precedence over automatic discounts, so if a customer enters a code for a coupon which has not
             been configured combine with a particular automatic discount, that automatic discount will 
             be pre-empted.') .
          '</p>';
}

/**
 * Generate list of coupon options for auto apply action.
 */
function uc_coupon_workflow_apply_options() {
  // Only non-bulk coupons may be applied automatically to avoid ambiguity regarding which code to apply.
  $rows = db_query('SELECT cid, name, code FROM {uc_coupons} WHERE status = :status AND bulk = :bulk', 
    array(':status' => 1, ':bulk' => 0, ));
  $ops = array();
  foreach ($rows as $row) {
    $ops[$row->cid] = t("@name (Code: @code)", array('@name' => $row->name, '@code' => $row->code)); 
  }
  return $ops;
}

function uc_coupon_workflow_apply_modes() {
  return array(
    'retain' => t('Apply this coupon normally. It will added to the session for the current customer, and when its
                   restrictions are satisfied, it will be applied exactly as if the code had been submitted manually.'),
    'auto' => t('Apply this coupon as an automatic discount.') . ' <strong>' . t('Only select this option if this rule 
                 reacts to the "%event" event.', array('%event' => t('Check for automatic discounts'))) . '</strong>. ' .
                 t('Then, this rule will be evaluated whenever the cart contents are updated, and the coupon applied
                 only if all conditions and restrictions are satisfied.'),
  );
}

/**
 * Implementatino of 'form_alter' callback for the apply action.
 * Change the select list to radio buttons.
 */
function uc_coupon_workflow_apply_form_alter(&$form, $form_state) {
  $form['parameter']['mode']['settings']['mode']['#type'] = 'radios';
}
