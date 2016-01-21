<?php

/**
 * Cart overrides settings form
 */
function commerce_responsive_ui_admin_settings($form, &$form_state) {
  // Get all eligible view options
  $cart_view_options = _commerce_responsive_ui_get_views("commerce_order","commerce_cart_form");

  if (empty($cart_view_options)) {
    $form['commerce_cart_view_override_page_view_none'] = array('#markup' => t('No eligible views found.'));
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

  
  $checkout_view_options = _commerce_responsive_ui_get_views("commerce_order","commerce_cart_summary");
  $form['commerce_responsive_ui_checkout'] = array(
    '#type' =>'fieldset',
    '#title' => t('Checkout Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['commerce_responsive_ui_checkout']['help'] = array(
    '#markup' => '<div class="messages status">'.t('The checkout cart views must be edited in the checkout settings.').'</div>'
  );
  $form['commerce_responsive_ui_checkout']['commerce_cart_contents_pane_view'] = array(
    '#type' => 'select',
    '#title' => t('Override the Checkout Cart Summary Page'),
    '#options' => $checkout_view_options,
    '#default_value' => variable_get('commerce_cart_contents_pane_view', 'commerce_cart_summary|default'),
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

function _commerce_responsive_ui_get_views($base_table="commerce_order",$match = false,$exclude=array('commerce_cart_block', 'commerce_orders', 'commerce_user_orders')) {
  
  // Build an options array of Views available
  $options = array();
  
  dpm($base_table);
  dpm($match);
  
  foreach (views_get_all_views() as $view_id => $view_value) {
    //dpm($view_value);
    
      dpm("checking if " . $view_id . " has ".$match. " in it");
    if ($match == false || stristr($view_id,$match)) {

      dpm($view_id . " matches!");

      // Only include line item Views, including a View that may be excluded but
      // has already been set to be the selected View some other way. The list of
      // excluded Views was added in as of Commerce 1.5, so we want to preserve
      // existing selections much like we do for Price fields with currency select
      // lists whose currency may have been disabled since the price was entered.
      if ($view_value->base_table == $base_table) {
        foreach ($view_value->display as $display_id => $display_value) {
          $key = $view_id . '|' . $display_id;

          if (!in_array($view_id, $exclude) || $key == $default) {
            $options[$view_id][$view_id . '|' . $display_id] = $display_value->display_title;
          }
        }
      }
    }
  }
  
  return $options;
}
