<?php
/**
 * @file
 *   Contains all checkout pane wrapper callbacks.
 */

/**
 * Checkout pane settings callback
 */
function _commerce_message_pane_settings_form($checkout_pane) {
  $form = array();

  // set default filter format
  $text_format_default = 'plain_text';
  if (filter_format_load('filtered_html')) {
    $text_format_default = 'filtered_html';
  }

  // get token info
  $token_var_info = array(
    'site' => array(
      'type' => 'site',
      'label' => t('Site information'),
      'description' => t('Site-wide settings and other global information.'),
    ),
    'commerce_order' => array(
      'label' => t('Order'),
      'type' => 'commerce_order',
    ),
    'commerce_order:owner' => array(
      'label' => t('Order Owner'),
      'type' => 'user',
    ),
  );
  $token_help = RulesTokenEvaluator::help($token_var_info);

  // create module based message name
  $setting_name = $checkout_pane['pane_id'];
  $message = '';

  // Find our default message and text format values.
  if (!is_null(variable_get("{$setting_name}_message", NULL))) {
    $data = variable_get("{$setting_name}_message", '');
    $message = $data['value'];
    $text_format = $data['format'];
  }
  else {
    $text_format = $text_format_default;
  }

  // build form for this type
  $container_key = $setting_name . '_container';
  $form[$container_key] = array(
    '#title' => t('Message'),
    '#type' => 'fieldset',
    '#attributes' => array('class' => array()),
    '#access' => filter_access(filter_format_load($text_format)),
  );

  $form[$container_key]["{$setting_name}_title"] = array(
    '#type' => 'textfield',
    '#title' => t('Sub Title'),
    '#description' => t('An alternate title to use instead of the checkout pane title.  If this pane is rendered as a fieldset, the pane title will show also.'),
    '#default_value' => variable_get("{$setting_name}_title", ''),
  );

  $form[$container_key]["{$setting_name}_required_check_enabled"] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a required checkbox'),
    '#description' => t('The required checkbox will show at the bottom of the pane by default.'),
    '#default_value' => variable_get("{$setting_name}_required_check_enabled", FALSE),
  );
  $form[$container_key]["{$setting_name}_required_check_label"] = array(
    '#type' => 'textfield',
    '#title' => t('Required checkbox label'),
    '#default_value' => variable_get("{$setting_name}_required_check_label", 'I agree'),
    '#prefix' => '<div class="commerce-message-pane-settings-required-check-label">',
    '#suffix' => '</div>',
    '#states' => array('invisible' => array(':input[name="' . $setting_name . '_required_check_enabled"]' => array('checked' => FALSE))),
  );

  $form[$container_key]["{$setting_name}_message"] = array(
    '#type' => 'text_format',
    '#default_value' => $message,
    '#format' => $text_format,
  );

  $form[$container_key]["{$setting_name}_help"] = $token_help;


  $form['#attributes'] = array('class' => array('commerce-message-pane-settings'));
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'commerce_message_pane') . '/theme/commerce_message_pane.settings.css',
  );
  return $form;
}

/**
 * Checkout pane form callback
 */
function _commerce_message_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // set default filter format
  $text_format_default = 'plain_text';
  if (filter_format_load('filtered_html')) {
    $text_format_default = 'filtered_html';
  }

  // load order owner
  $owner = user_load($order->uid);

  // build messages
  $setting_name = $checkout_pane['pane_id'];
  $message = '';
  if (!is_null(variable_get("{$setting_name}_message", NULL))) {
    $data = variable_get("{$setting_name}_message", array('value' => '', 'format' => $text_format_default));
    $message = $data['value'];
    $text_format = $data['format'];
  }

  if (empty($message)) {
    return $pane_form;
  }

  // Perform token replacement.
  $message = token_replace($message, array('commerce-order' => $order));

  // Apply the proper text format.
  $message = check_markup($message, $text_format);

  if (!empty($message)) {
    $title = variable_get($setting_name . '_title', '');
    if (!empty($title)) {
      $pane_form[$setting_name . '_title'] = array(
        '#markup' => '<h3 class="title ' . drupal_html_class($setting_name . '_title') . '">' . check_plain($title) . '</h3>',
      );
    }

    $pane_form[$setting_name . '_message'] = array(
      '#markup' => '<div class="' . drupal_html_class($setting_name . '_message') . '">' . $message . '</div>',
    );
  }

  // Required checkbox
  if (variable_get($setting_name . '_required_check_enabled', FALSE)) {
    $required_check_label = variable_get($setting_name . '_required_check_label', 'I agree');
    if (empty($required_check_label)) {
      $required_check_label = '';
    }

    $pane_form[$setting_name . '_required_check_response'] = array(
      '#type' => 'checkbox',
      '#title' => t('@commerce_message_pane_required_check_label', array('@commerce_message_pane_required_check_label' => $required_check_label)),
      '#required' => TRUE,
      '#default_value' => !empty($order->data[$setting_name . '_required_check_response']),
    );

    // Add a generic class.
    $pane_form['#attributes'] = array(
      'class' => array('commerce-message-pane-required'),
    );
  }

  if (!empty($pane_form)) {
    if (!empty($pane_form['#attributes']['class'])) {
      $pane_form['#attributes']['class'][] = 'commerce-message-pane';
    }
    else {
      $pane_form['#attributes'] = array(
        'class' => array('commerce-message-pane'),
      );
    }
  }

  return $pane_form;
}

/**
 * Checkout form submission callback.
 */
function _commerce_message_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (!empty($form_state['values'][$checkout_pane['pane_id']])) {
    $pane_values = $form_state['values'][$checkout_pane['pane_id']];
    $setting_name = $checkout_pane['pane_id'];

    // store required check response in the order data
    if (isset($pane_values[$setting_name . '_required_check_response'])) {
      $order->data[$setting_name . '_required_check_response'] = $pane_values[$setting_name . '_required_check_response'];
    }
  }
}
