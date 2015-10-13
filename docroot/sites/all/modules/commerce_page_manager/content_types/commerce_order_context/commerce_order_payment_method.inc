<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Selected payment method'),
  'icon' => 'icon_order.png',
  'description' => t('The payment method selected by the customer for this order.'),
  'required context' => new ctools_context_required(t('Commerce Order'), 'commerce_order'),
  'category' => t('Commerce Order'),
);

/**
 * Render the custom content type.
 */
function commerce_page_manager_commerce_order_payment_method_content_type_render($subtype, $conf, $panel_args, $context) {
  if (empty($context) || empty($context->data)) {
    return;
  }

  // Ensure an appropriate default title type.
  if (empty($conf['title_type'])) {
    $conf['title_type'] = 'display_title';
  }

  // Get a shortcut to the order.
  $commerce_order = clone $context->data;

  // Build the content type block.
  $block = new stdClass();
  $block->module = 'commerce_order_payment_method';
  $block->title = t('Payment method');

  // Load the payment method instance.
  if (!empty($commerce_order->data['payment_method'])) {
    $payment_method = commerce_payment_method_instance_load($commerce_order->data['payment_method']);

    if (!empty($payment_method)) {
      $block->content = $payment_method[$conf['title_type']];
    }
  }

  // If we could not determine the payment method, return nothing.
  if (empty($block->content)) {
    return;
  }

  $block->delta = $commerce_order->order_id;

  return $block;
}

/**
 * Returns an edit form for custom content type settings.
 */
function commerce_page_manager_commerce_order_payment_method_content_type_edit_form($form, &$form_state) {
  // Returning $form allows ctools to provide a panel title override in the settings form.
  $conf = $form_state['conf'] + array(
    'title_type' => 'display_title',
  );

  $form['title_type'] = array(
    '#type' => 'radios',
    '#title' => t('Render the following payment method title'),
    '#options' => array(
      'title' => t('Administrative title'),
      'short_title' => t('Short administrative title'),
      'display_title' => t('Display title (shown on the checkout form)'),
    ),
    '#default_value' => $conf['title_type'],
  );

  return $form;
}

/**
 * Submits the edit form for the custom content type.
 */
function commerce_page_manager_commerce_order_payment_method_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['title_type'] = $form_state['values']['title_type'];
}

/**
 * Returns the administrative title for a type.
 */
function commerce_page_manager_commerce_order_payment_method_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" order payment method', array('@s' => $context->identifier));
}
