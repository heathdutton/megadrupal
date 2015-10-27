<?php

/**
 * Cart overrides settings form
 */
function commerce_responsive_ui_admin_settings($form, &$form_state) {
  // Get all eligible view options
  $cart_view_options = commerce_responsive_ui_cartoptions_list();

  if (empty($cart_view_options)) {
    $form['commerce_cart_view_override_page_view_none'] = array('#markup' => t('No eligible views found.') . '<br />' . $description);
    return $form;
  }

  $help_items = array();
  $help_items[] = t('View machine name that starts with "commerce_cart_form", ie "commerce_cart_form_custom"');
  $help_items[] = t('A base table of Commerce Order');
  $help_items[] = t('The first contextual filter (argument) must be a Commerce Order ID with an argument id that starts with "order_id"');
  $form['commerce_responsive_ui_cart'] = array(
    '#type' =>'fieldset',
    '#title' => t('Cart Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['commerce_responsive_ui_cart']['help'] = array(
    '#markup' => '<div class="messages status"><strong><em>'.t('A cart page view must have:').'</em></strong>'.theme('item_list', array('items' => $help_items)).'</div>'
  );
  $form['commerce_responsive_ui_cart']['commerce_responsive_ui_cart_override'] = array(
    '#type' => 'select',
    '#title' => t('Override the Cart Page'),
    '#options' => $cart_view_options,
    '#default_value' => variable_get('commerce_responsive_ui_cart_override', 'commerce_cart_form'),
  );

  $form['commerce_responsive_ui_checkout'] = array(
    '#type' =>'fieldset',
    '#title' => t('Checkout Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['commerce_responsive_ui_checkout']['help'] = array(
    '#markup' => '<div class="messages status">'.t('The checkout cart views must be edited in the checkout settings.').'</div>'
  );
  $form['commerce_responsive_ui_user_orders'] = array(
    '#type' =>'fieldset',
    '#title' => t('Order Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['commerce_responsive_ui_user_orders']['help'] = array(
    '#markup' => '<div class="messages status">'.t('You must enable the user order view override in the display settings for Order entities by choosing a different view (similar to the checkout settings change). Also, you can Enable the "User orders (responsive)" view.').'</div>'
  );
  /*
  @Todo need to add a way for people to override the checkout view and enable the order user view from this page


  $form['commerce_responsive_ui_cart']['commerce_responsive_ui_cart_override_checkoutpane'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set the Cart pane view to this option as well.'),
  );*/

  $form['#submit'][] = 'commerce_responsive_ui_admin_settings_submit';

  return system_settings_form($form);
}

function commerce_responsive_ui_admin_settings_submit($form, &$form_state) {
  /*
  See @todo above.
  foreach ($form_state['values'] as $key => $value) {
    if ($key == "commerce_responsive_ui_cart_override_checkoutpane") {
      if ($value == 1 && !empty($form_state['values']['commerce_responsive_ui_cart_override'])) {
        // @todo: Make the original form show all display modes of matching views.
        variable_set('commerce_cart_contents_pane_view', $form_state['values']['commerce_responsive_ui_cart_override'].'|default');
      }

      unset($form_state['values'][$key]);
    }
  }*/
}

