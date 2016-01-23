<?php

/**
 * @file
 * Commerce free shipping admininistration settings form.
 */

/**
 * Form constructor for Free shipping settings.
 */
function commerce_free_shipping_admin_form($form, &$form_state) {
  $form = system_settings_form($form);
  $form['#attributes'] = array('class' => array('commerce-free-shipping-admin-form'));
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'commerce_free_shipping') . '/includes/commerce-free-shipping-admin.js',
  );
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_free_shipping') . '/includes/commerce-free-shipping-admin.css',
  );
  $shipping_services = commerce_shipping_services();
  foreach ($shipping_services as $service) {
    $base = $service['base'];
    $form[$base] = array(
      '#title' => t('Service') . ' : ' . $base,
      '#type' => 'fieldset',
      '#attributes' => array('class' => array('base')),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      '#weight' => variable_get($base . '_free_shipping_enabled', FALSE) * -1,
    );
    if (variable_get($base . '_free_shipping_enabled', FALSE)) {
      $form[$base]['#attributes']['class'][] = 'active';
      $form[$base]['#title'] .= ' - <strong class="active">&#x2713;</strong><strong class="inactive element-invisible">&#x2717;</strong>';
    }
    else {
      $form[$base]['#title'] .= ' - <strong class="active element-invisible">&#x2713;</strong><strong class="inactive">&#x2717;</strong>';
    }
    $form[$base][$base . '_free_shipping_enabled'] = array(
      '#title' => variable_get($base . '_free_shipping_enabled', FALSE) ? t('Uncheck and save to disable free shipping.') : t('Check to enable free shipping.'),
      '#type' => 'checkbox',
      '#attributes' => array('class' => array('base-checkbox-enable')),
      '#default_value' => variable_get($base . '_free_shipping_enabled', FALSE),
    );

    $default_currency = variable_get($base . '_free_shipping_rate_limit_currency', 'default');
    if ($default_currency == 'default') {
      $default_currency = commerce_default_currency();
    }
    $currency = commerce_currency_load($default_currency);
    // Convert the price amount to a user friendly decimal value.
    $default_amount = commerce_currency_amount_to_decimal(variable_get($base . '_free_shipping_rate_limit', 6000), $currency['code']);

    // Run it through number_format() to ensure it has the proper number of
    // decimal places.
    $default_amount = number_format($default_amount, $currency['decimals'], '.', '');
    $form[$base]['price'] = array(
      '#type' => 'fieldset',
      '#title' => t('Price threshold'),
      '#collapsible' => TRUE,
    );
    $form[$base]['price'][$base . '_free_shipping_rate_limit'] = array(
      '#title' => t('Price threshold to obtain free shipping'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => $default_amount,
      '#required' => TRUE,
    );

    // Build an options array of allowed currency values including the option
    // for the widget to always use the store's default currency.
    $options = array(
      'default' => t('- Default store currency -'),
    );
    foreach (commerce_currencies(TRUE) as $currency_code => $currency) {
      $options[$currency_code] = t('@code - @name', array('@code' => $currency['code'], '@name' => $currency['name']));
    }
    $form[$base]['price'][$base . '_free_shipping_rate_limit_currency'] = array(
      '#type' => 'select',
      '#title' => t('Currency'),
      '#options' => $options,
      '#default_value' => $default_currency,
    );

    $form[$base]['price'][$base . '_free_shipping_excluded_line_item_types'] = array(
      '#type' => 'select',
      '#title' => t('Exclude line item(s) of type(s) from the order total calcul'),
      '#multiple' => TRUE,
      '#options' => commerce_line_item_type_options_list(),
      '#default_value' => variable_get($base . '_free_shipping_excluded_line_item_types', array('shipping' => 'shipping')),
    );

    $form[$base]['price'][$base . '_free_shipping_excluded_line_item_product_types'] = array(
      '#type' => 'select',
      '#title' => t('Exclude line item(s) that reference product of type(s) from the order total calcul'),
      '#multiple' => TRUE,
      '#options' => commerce_product_type_options_list(),
      '#default_value' => variable_get($base . '_free_shipping_excluded_line_item_product_types', array()),
    );

    // Price threshold conditons failed message:
    $form[$base]['message_no_free_shipping'] = array(
      '#type' => 'fieldset',
      '#title' => t('Message if price threshold condition failed'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if (variable_get($base . '_free_shipping_reject_message_display', TRUE)) {
      $form[$base]['message_no_free_shipping']['#title'] .= ' - <strong>&#x2713;</strong>';
    }
    else {
      $form[$base]['message_no_free_shipping']['#title'] .= ' - &#x2717;';
    }
    $form[$base]['message_no_free_shipping'][$base . '_free_shipping_reject_message_display'] = array(
      '#title' => t("Display a message when the user don't get free shipping price because the order amount is below the price threshold."),
      '#type' => 'checkbox',
      '#default_value' => variable_get($base . '_free_shipping_reject_message_display', TRUE),
    );
    $form[$base]['message_no_free_shipping'][$base . '_free_shipping_reject_message'] = array(
      '#title' => t("Message to display on the checkout shipping service panes when the customer doesn't obtain the free shipping because the order amount is below the price threshold."),
      '#description' => t('%rate will be replaced by the price left to obtain a free shipping'),
      '#type' => 'textfield',
      '#size' => 200,
      '#description' => t('Allowed markup: @markup', array('@markup' => "'b', 'strong', 'i', 'u', 'em', 'a'")),
      '#default_value' => variable_get($base . '_free_shipping_reject_message', '%rate left to get free pricing on ' . $service['display_title']),
    );

    // Additionnal rule conditions settings:
    $form[$base]['rules'] = array(
      '#title' => 'Additionnal conditions with rules',
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if (variable_get($base . '_free_shipping_additionnal_rules', TRUE)) {
      $form[$base]['rules']['#title'] .= ' - <strong class="active">&#x2713;</strong>';
    }
    else {
      $form[$base]['rules']['#title'] .= ' - <strong class="inactive">&#x2717;</strong>';
    }
    $form[$base]['rules'][$base . '_free_shipping_additionnal_rules'] = array(
      '#title' => t('Enable/Disable additionnal condition with rules.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get($base . '_free_shipping_additionnal_rules', FALSE),
      '#suffix' => '<small>' . t('These conditions are evaluated only if the free shipping price limit condition are evaluated to TRUE') . '</small><br>',
    );
    // Check if free shipping is enabled for this services.
    if (variable_get($base . '_free_shipping_additionnal_rules', FALSE)) {
      $form[$base]['rules']['markup'] = array(
        '#markup' => l(t('Click to see / edit the additionnal condition'), 'admin/commerce/config/free-shipping/' . $base, array('attributes' => array('class' => array('ctools-use-modal')))),
      );
      // Rules conditions failed message:
      $form[$base]['rules']['message_rules_no_free_shipping'] = array(
        '#type' => 'fieldset',
        '#title' => t('Message if free shipping rules conditions failed'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      if (variable_get($base . '_free_shipping_rules_reject_message_display', TRUE)) {
        $form[$base]['rules']['message_rules_no_free_shipping']['#title'] .= ' - <strong class="active">&#x2713;</strong>';
      }
      else {
        $form[$base]['rules']['message_rules_no_free_shipping']['#title'] .= ' - <strong class="inactive">&#x2717;</strong>';
      }
      $form[$base]['rules']['message_rules_no_free_shipping'][$base . '_free_shipping_rules_reject_message_display'] = array(
        '#title' => t("Display a message when the user don't get free shipping price because of the rules conditions."),
        '#type' => 'checkbox',
        '#default_value' => variable_get($base . '_free_shipping_rules_reject_message_display', TRUE),
      );
      $form[$base]['rules']['message_rules_no_free_shipping'][$base . '_free_shipping_rules_reject_message'] = array(
        '#title' => t("Message to display on the checkout shipping service panes when the customer doesn't obtain the free shipping because of the rules conditions."),
        '#type' => 'textfield',
        '#size' => 200,
        '#description' => t('Allowed markup: @markup', array('@markup' => "'b', 'strong', 'i', 'u', 'em', 'a'")),
        '#default_value' => variable_get($base . '_free_shipping_rules_reject_message', "You can get free shipping with " . $service['display_title'] . ", but you don't have enough {explain your conditions}."),
      );
    }

    // Messages settings, if user conditions succeed :
    $form[$base]['message_free_shipping'] = array(
      '#type' => 'fieldset',
      '#title' => t('Message if the customer got free shipping'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if (variable_get($base . '_free_shipping_gift_message_display', TRUE)) {
      $form[$base]['message_free_shipping']['#title'] .= ' - <strong class="active">&#x2713;</strong>';
    }
    else {
      $form[$base]['message_free_shipping']['#title'] .= ' - <strong class="inactive">&#x2717;</strong>';
    }
    $form[$base]['message_free_shipping'][$base . '_free_shipping_gift_message_display'] = array(
      '#title' => t('Display a message when the user get free shipping price.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get($base . '_free_shipping_gift_message_display', TRUE),
    );
    $form[$base]['message_free_shipping'][$base . '_free_shipping_gift_message'] = array(
      '#title' => t('Message to display on the checkout shipping service panes when the customer obtain the free shipping.'),
      '#description' => t('%rate will be replaced by the limit price to obtain free shipping'),
      '#type' => 'textfield',
      '#size' => 200,
      '#description' => t('Allowed markup: @markup', array('@markup' => "'b', 'strong', 'i', 'u', 'em', 'a'")),
      '#default_value' => variable_get($base . '_free_shipping_gift_message', 'You have got free pricing on ' . $service['display_title'] . ', because your order total has raise %rate !'),
    );
  }

  $form['#validate'][] = 'commerce_free_shipping_admin_form_validate';
  return $form;
}

/**
 * Validate callback: ensures the amount value is numeric and converts it from decimal to integer.
 */
function commerce_free_shipping_admin_form_validate($form, &$form_state) {
  $shipping_services = commerce_shipping_services();
  foreach ($shipping_services as $service) {
    $base = $service['base'];
    // Price threshold convert decimal to amount and update the form_state values.
    if ($form[$base]['price'][$base . '_free_shipping_rate_limit']['#value'] !== '') {
      if ($form[$base]['price'][$base . '_free_shipping_rate_limit_currency']['#value'] == 'default') {
        $currency = commerce_default_currency();
      }
      else {
        $currency = $form[$base]['price'][$base . '_free_shipping_rate_limit_currency']['#value'];
      }
      // Ensure the price is numeric.
      if (!is_numeric($form[$base]['price'][$base . '_free_shipping_rate_limit']['#value'])) {
        form_error($form[$base]['price'][$base . '_free_shipping_rate_limit'], t('%title: you must enter a numeric value for the price amount.', array('%title' => $form[$base]['price'][$base . '_free_shipping_rate_limit']['#title'])));
      }
      else {
        // Convert the decimal amount value entered to an integer based amount value.
        form_set_value($form[$base]['price'][$base . '_free_shipping_rate_limit'], commerce_currency_decimal_to_amount($form[$base]['price'][$base . '_free_shipping_rate_limit']['#value'], $currency), $form_state);
      }
    }

    if (!$form[$base]['rules'][$base . '_free_shipping_additionnal_rules']['#value']
        && variable_get($base . '_free_shipping_additionnal_rules', FALSE)) {
      // Delete rules components when the user disable the additionnal rules.
      $rule = rules_config_load('commerce_free_shipping_service_' . $base);
      if ($rule) {
        $rule->delete();
      }
    }
  }
}

/**
 * Form callback for the edition of the free shipping rules condition set.
 */
function commerce_free_shipping_rules_admin_form($form, &$form_state) {
  $base = arg(4);
  // See rules doc: https://drupal.org/node/905262 for tips.
  $rule = rules_config_load('commerce_free_shipping_service_' . $base);
  $form_state['rule_component'] = $rule;
  RulesPluginUI::$basePath = 'admin/commerce/config/free-shipping/' . $base;
  $options = array('button' => TRUE);
  $rule->form($form, $form_state, $options);
  $form['#prefix'] = '<p>' . t('These conditions are evaluated only if the free shipping price limit condition are evaluated to TRUE') . '</p>';
  $form['#validate'][] = 'commerce_free_shipping_rules_admin_form_validate';
  $form['#submit'][] = 'commerce_free_shipping_rules_admin_form_submit';

  return $form;
}

/**
 * Rules form validate handler.
 */
function commerce_free_shipping_rules_admin_form_validate($form, &$form_state) {
  $form_state['rule_component']->form_validate($form, $form_state);
}

/**
 * Rules form submit handler.
 */
function commerce_free_shipping_rules_admin_form_submit($form, &$form_state) {
  $form_state['rule_component']->form_submit($form, $form_state);
  drupal_set_message(t('Your changes have been saved.'), 'status', FALSE);
}
