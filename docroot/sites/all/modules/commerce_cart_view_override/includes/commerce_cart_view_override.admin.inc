<?php

/**
 * Cart overrides settings form
 */
function commerce_cart_view_override_admin_settings($form, &$form_state) {
  // Get all eligible view options
  $cart_view_options = commerce_cart_view_override_options_list();

  $help_items = array();
  $help_items[] = t('View machine name that starts "commerce_cart_form", ie "commerce_cart_form_custom"');
  $help_items[] = t('A base table of Commerce Order');
  $help_items[] = t('The first contextual filter (argument) must be a Commerce Order ID with an argument id that starts with "order_id"');

  $form['help'] = array(
    '#type' => 'item',
    '#title' => t('A cart page view must have:'),
    '#prefix' => t('For more information, refer to the <a href="http://drupal.org/project/commerce_cart_view_override">project page</a>'),
    '#markup' => theme('item_list', array('items' => $help_items)),
  );

  if (empty($cart_view_options)) {
    $form['commerce_cart_view_override_page_view_none'] = array('#markup' => t('No eligible views found.') . '<br />' . $description);
    return $form;
  }
  $form['commerce_cart_view_override_page_view'] = array(
    '#type' => 'select',
    '#title' => t('Cart page view'),
    '#options' => $cart_view_options,
    '#default_value' => variable_get('commerce_cart_view_override_page_view', 'commerce_cart_form'),
  );

  return system_settings_form($form);
}
