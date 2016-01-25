<?php

/**
 * @file
 * commerce_notification.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_notification_default_rules_configuration() {
  $rules = array();

  // Abandoned cart notification.
  $rule = rules_rule_set(array(
    'abandoned_cart' => array('label' => 'Abandoned Cart', 'type' => 'commerce_order'),
  ));

  // Component.
  $rule->label = t('Notify the user of their abandoned cart');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  // Display a message if the notification has already been sent.
  $rule_action_message = rule();
  $rule_action_message->label = t('Display a message if the notification has already been sent.');
  $rule_action_message->condition(rules_condition('data_is', array(
    'data:select' => 'abandoned-cart:status',
    'value' => 'user_notified',
  )));
  $rule_action_message->action('drupal_message', array(
    'message' => 'A notification has already been sent to [abandoned-cart:mail].',
    'type' => 'warning',
  ));

  $rule->rule($rule_action_message);

  // Send e-mail to the user with the content of their abandoned cart.
  $rule_action_emails = rule();
  $rule_action_emails->label = t('Send e-mail to the user with the content of their abandoned cart');
  $rule_action_emails->condition(rules_condition('data_is_empty', array(
    'data:select' => 'abandoned-cart:mail',
  ))->negate());
  $rule_action_emails->condition(rules_condition('data_is', array(
    'data:select' => 'abandoned-cart:status',
    'value' => 'user_notified',
  ))->negate());
  $rule_action_emails->action('entity_create', array(
    'type' => 'message',
    'param_type' => 'abandoned_cart_message',
    'param_user:select' => 'abandoned-cart:owner',
    'entity_created:label' => 'Created entity',
    'entity_created:var' => 'message',
  ))
  ->action('data_set', array(
    'data:select' => 'message:field-abandoned-cart-order',
    'value:select' => 'abandoned-cart',
  ))
  ->action('entity_save', array(
    'data:select' => 'message',
    'immediate' => '1',
  ))
  ->action('message_notify_process', array(
    'message:select' => 'message',
    'save_on_fail' => '0',
    'save_on_success' => '0',
    'mail:select' => 'abandoned-cart:mail',
  ))
  ->action('commerce_order_update_status', array(
    'commerce_order:select' => 'abandoned-cart',
    'order_status' => 'user_notified',
  ))
  ->action('save_abandoned_cart_notification', array(
    'account_user:select' => 'abandoned-cart:owner',
    'order:select' => 'abandoned-cart',
  ))
  ->action('drupal_message', array(
    'message' => 'Successfully sent e-mail to [abandoned-cart:mail].',
    'type' => 'status',
  ));

  $rule->rule($rule_action_emails);

  // Display a message if no e-mail address is attached to the cart.
  $rule_message_empty = rule();
  $rule_message_empty->label = t('Display a message if no e-mail address is attached to the cart.');
  $rule_message_empty->condition(rules_condition('data_is_empty', array(
    'data:select' => 'abandoned-cart:mail',
  )));
  $rule_message_empty->action('drupal_message', array(
    'message' => 'No e-mail attached to the shopping cart n°[abandoned-cart:order-id].',
    'type' => 'status',
  ));

  $rule->rule($rule_message_empty);

  $rules['commerce_abandon_notify'] = $rule;

  // Reaction rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Send notification emails to users for their abandoned cart');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  $rule
    ->event('cron')
    ->action('entity_fetch', array(
      'type' => 'user',
      'id' => '1',
      'entity_fetched:label' => 'Admin User',
      'entity_fetched:var' => 'admin_user',
    ))
    ->action('views_bulk_operations_action_load_list', array(
      'view' => 'abandoned_cart_orders|default',
      'view_user:select' => 'admin-user',
      'entity_list:label' => t('Orders'),
      'entity_list:var' => 'orders',
    ));

  $loop = rules_loop(array(
    'list:select' => 'orders',
    'item:label' => t('Current order'),
    'item:var' => 'order',
  ));

  $loop->action('component_commerce_abandon_notify', array(
    'abandoned_cart:select' => 'order',
  ));
  $rule->action($loop);

  $rules['rules_notify_user_abandoned_cart'] = $rule;

  // Notification for customers who have not purchase x months or weeks.
  $rule = rules_rule_set(array(
    'month_weeks_purchases' => array('label' => 'X months-weeks purchases', 'type' => 'commerce_order'),
  ));

  // Component.
  $rule->label = t('Send emails to users that have not purchase for some time');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  // Display a message if the notification has already been sent.
  $rule_action_email_notify = rule();
  $rule_action_email_notify->label = t('Display a message if the notification has already been sent.');

  $rule_action_email_notify->condition(rules_condition('set_date_condition_for_abandoned_cart_notification', array(
    'account_user:select' => 'month-weeks-purchases:owner',
    'order:select' => 'month-weeks-purchases',
    'order_count_comparison' => 'greater_equal',
    'order_count_value' => '+1 day',
  ))->negate());
  $rule_action_email_notify->action('drupal_message', array(
    'message' => 'A notification has already been sent to [month-weeks-purchases:mail].',
    'type' => 'warning',
  ));

  $rule->rule($rule_action_email_notify);

  // Send e-mail to the user with the content of their abandoned cart.
  $rule_action_emails = rule();
  $rule_action_emails->label = t('Send emails to customers who have not purchase x months week');
  $rule_action_emails->condition(rules_condition('data_is_empty', array(
    'data:select' => 'month-weeks-purchases:mail',
  ))->negate());
  $rule_action_emails->condition(rules_condition('set_date_condition_for_abandoned_cart_notification', array(
    'account_user:select' => 'month-weeks-purchases:owner',
    'order:select' => 'month-weeks-purchases',
    'order_count_comparison' => 'greater_equal',
    'order_count_value' => '+1 day',
  )));
  $rule_action_emails->action('entity_create', array(
    'type' => 'message',
    'param_type' => 'customer_not_ordered_x_months_weeks',
    'param_user:select' => 'month-weeks-purchases:owner',
    'entity_created:label' => 'Created entity',
    'entity_created:var' => 'message',
  ))
    ->action('message_notify_process', array(
      'message:select' => 'message',
      'save_on_fail' => '0',
      'save_on_success' => '0',
      'mail:select' => 'month-weeks-purchases:owner:mail',
    ))
    ->action('commerce_order_update_status', array(
      'commerce_order:select' => 'month-weeks-purchases',
      'order_status' => 'user_notified',
    ))
    ->action('drupal_message', array(
      'message' => 'Successfully sent e-mail to [month-weeks-purchases:mail].',
      'type' => 'status',
    ))
    ->action('save_abandoned_cart_notification', array(
      'account_user:select' => 'month-weeks-purchases:owner',
      'order:select' => 'month-weeks-purchases',
    ));

  $rule->rule($rule_action_emails);

  // Display a message if no e-mail address is attached to the cart.
  $rule_action_message = rule();
  $rule_action_message->label = t('Display a message if no e-mail address is attached to the cart.');
  $rule_action_message->condition(rules_condition('data_is_empty', array(
    'data:select' => 'month-weeks-purchases:mail',
  )));
  $rule_action_message->action('drupal_message', array(
    'message' => 'No e-mail attached to the shopping cart n°[month-weeks-purchases:order-id].',
    'type' => 'status',
  ));

  $rule->rule($rule_action_message);

  $rules['customers_not_purchase_some_time'] = $rule;

  // Reaction rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Notify customers who didn\'t order x months or weeks');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  $rule
    ->event('cron')
    ->action('entity_fetch', array(
      'type' => 'user',
      'id' => '1',
      'entity_fetched:label' => 'Admin User',
      'entity_fetched:var' => 'admin_user',
    ))
    ->action('views_bulk_operations_action_load_list', array(
      'view' => 'customers_who_have_not_ordered_for_X_weeks_months|default',
      'view_user:select' => 'admin-user',
      'entity_list:label' => t('Orders'),
      'entity_list:var' => 'orders',
    ));

  $loop = rules_loop(array(
    'list:select' => 'orders',
    'item:label' => t('Current order'),
    'item:var' => 'order',
  ));

  $loop->action('component_customers_not_purchase_some_time', array(
    'month_weeks_purchases:select' => 'order',
  ));
  $rule->action($loop);

  $rules['customers_who_have_not_purchase_x_months_weeks'] = $rule;

  // Aniversary notification.
  $rule = rules_rule_set(
    array(
      'aniversary' => array('label' => 'Aniversary notification', 'type' => 'commerce_order'),
    )
  );

  // Component.
  $rule->label = t('Send emails to users that have not purchase for 1 year');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  // Display a message if the notification has already been sent.
  $rule_action_message = rule();
  $rule_action_message->label = t('Display a message if the notification has already been sent.');
  $rule_action_message->condition(rules_condition('set_date_condition_for_abandoned_cart_notification', array(
    'account_user:select' => 'aniversary:owner',
    'order:select' => 'aniversary',
    'order_count_comparison' => 'greater_equal',
    'order_count_value' => '+1 year',
  ))->negate());
  $rule_action_message->action('drupal_message', array(
    'message' => 'A notification has already been sent to [aniversary:mail].',
    'type' => 'warning',
  ));

  $rule->rule($rule_action_message);

  // Send e-mail to the user with the content of their abandoned cart.
  $rule_action_emails = rule();
  $rule_action_emails->label = t('Send emails to customers who have not purchase one year');
  $rule_action_emails->condition(rules_condition('data_is_empty', array(
    'data:select' => 'aniversary:mail',
  ))->negate());
  $rule_action_emails->condition(rules_condition('set_date_condition_for_abandoned_cart_notification', array(
    'account_user:select' => 'aniversary:owner',
    'order:select' => 'aniversary',
    'order_count_comparison' => 'greater_equal',
    'order_count_value' => '+1 year',
  )));
  $rule_action_emails->action('entity_create', array(
    'type' => 'message',
    'param_type' => 'anniversary_message',
    'param_user:select' => 'aniversary:owner',
    'entity_created:label' => 'Created entity',
    'entity_created:var' => 'message',
  ))
    ->action('message_notify_process', array(
      'message:select' => 'message',
      'save_on_fail' => '0',
      'save_on_success' => '0',
      'mail:select' => 'aniversary:owner:mail',
    ))
    ->action('commerce_order_update_status', array(
      'commerce_order:select' => 'aniversary',
      'order_status' => 'user_notified',
    ))
    ->action('drupal_message', array(
      'message' => 'Successfully sent e-mail to [aniversary:mail].',
      'type' => 'status',
    ))
    ->action('save_abandoned_cart_notification', array(
      'account_user:select' => 'aniversary:owner',
      'order:select' => 'aniversary',
    ));

  $rule->rule($rule_action_emails);

  // Display a message if no e-mail address is attached to the cart.
  $rule_action_message = rule();
  $rule_action_message->label = t('Display a message if no e-mail address is attached to the cart.');
  $rule_action_message->condition(rules_condition('data_is_empty', array(
    'data:select' => 'aniversary:mail',
  )));
  $rule_action_message->action('drupal_message', array(
    'message' => 'No e-mail attached to the shopping cart n°[aniversary:order-id].',
    'type' => 'status',
  ));

  $rule->rule($rule_action_message);

  $rules['send_aniversary_emails'] = $rule;

  // Reaction rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Send anyverssary emails to customers that have not purchace for one year');
  $rule->tags = array('Commerce Order');
  $rule->active = TRUE;

  $rule
    ->event('cron')
    ->action('entity_fetch', array(
      'type' => 'user',
      'id' => '1',
      'entity_fetched:label' => 'Admin User',
      'entity_fetched:var' => 'admin_user',
    ))
    ->action('views_bulk_operations_action_load_list', array(
      'view' => 'purchase_anniversary_promo_view|default',
      'view_user:select' => 'admin-user',
      'entity_list:label' => t('Orders'),
      'entity_list:var' => 'orders',
    ));

  $loop = rules_loop(array(
    'list:select' => 'orders',
    'item:label' => t('Current order'),
    'item:var' => 'order',
  ));

  $loop->action('component_send_aniversary_emails', array(
    'aniversary:select' => 'order',
  ));
  $rule->action($loop);

  $rules['send_aniversary_emails_to_customers'] = $rule;

  return $rules;
}
