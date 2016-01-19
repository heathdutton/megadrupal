<?php

/**
 * @file
 * Provides Commerce Checkout pane functionality for giftcards.
 */

/*
 * Checkout pane callback: checkout form
 */
function commerce_gc_product_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $pane_form = array();
  
  // Find line items that contain giftcard products  
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if ($line_item_wrapper->value() && in_array($line_item_wrapper->type->value(), commerce_gc_product_giftcard_line_item_types())) {
      // Generate a container.
      if (!isset($pane_form)) {
        $pane_form = array();
      }
      
      $line_item = $line_item_wrapper->value();
      $line_item_id = $line_item->line_item_id;
      $label = $line_item_wrapper->commerce_product->title->value();
      
      // Build an entity form for this line item.
      $pane_form[$line_item_id]['line_item_fields'] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($label),
        '#parents' => array($line_item_id, 'commerce_gc_product')
      );
      $line_item_form =& $pane_form[$line_item_id]['line_item_fields'];
      field_attach_form('commerce_line_item', $line_item, $line_item_form, $form_state);
      
      // Remove any fields that haven't been marked for inclusion on the 
      // add-to-cart form.
      foreach (element_children($line_item_form) as $field_name) {
        $info = field_info_instance('commerce_line_item', $field_name, $line_item->type);
        $line_item_form[$field_name]['#commerce_cart_settings'] = commerce_cart_field_instance_access_settings($info);

        if (empty($line_item_form[$field_name]['#commerce_cart_settings']['field_access'])) {
          $line_item_form[$field_name]['#access'] = FALSE;
        }
      }
    }
  }
  
  return $pane_form;
}

/*
 * Checkout pane callback: checkout form validate
 */
function commerce_gc_product_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);  
  
  // Validate the line item form(s).
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if (in_array($line_item_wrapper->type->value(), commerce_gc_product_giftcard_line_item_types())) {      
      $line_item = $line_item_wrapper->value();
      
      $line_item_id = $line_item->line_item_id;      
      field_attach_form_validate('commerce_line_item', $line_item, $form['commerce_gc_product'][$line_item_id]['line_item_fields'], $form_state);
    }
  }
  
  return form_get_errors() ? FALSE : TRUE;
}

/*
 * Checkout pane callback: checkout form submit
 */
function commerce_gc_product_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);  
  
  // Validate the line item form(s).
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    if (in_array($line_item_wrapper->type->value(), commerce_gc_product_giftcard_line_item_types())) {      
      $line_item = $line_item_wrapper->value();
      $line_item_id = $line_item->line_item_id;
      
      // Submit and save
      field_attach_submit('commerce_line_item', $line_item, $form['commerce_gc_product'][$line_item_id]['line_item_fields'], $form_state);
      commerce_line_item_save($line_item);
    }
  }
}

/*
 * Checkout pane callback: review
 */
function commerce_gc_product_pane_review($form, $form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $output = array();
  
  // Validate the line item form(s).
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    if ($line_item && commerce_gc_product_line_item_is_giftcard($line_item_wrapper)) {
      
      $output[$line_item->line_item_id]['title'] = array(
        '#prefix' => '<div ' . drupal_attributes(array('class' => 'commerce-gc-line-item-review-title')) . '>',
        '#markup' => check_plain($line_item_wrapper->commerce_product->title->value()),
        '#suffix' => '</div>'
      );
      
      // Special markup for free product line items
      if ($line_item->type == 'product_discount') {
        $output[$line_item->line_item_id]['title']['#markup'] .= ' ' . t('(Free product)');
      }      
      
      foreach (field_info_instances('commerce_line_item', $line_item->type) as $instance) {
        if (!empty($instance['commerce_cart_settings']['field_access'])) {
          $field_name = $instance['field_name'];
          $field_render = field_view_field('commerce_line_item', $line_item, $field_name, 'default');
          // Special handling for the email field.
          if ($field_name == 'commerce_gc_mail' && !$field_render) {
            $render = array(
              '#markup' => '<strong>' . t('Purchased for myself') . '</strong>' . 
                '<div ' . drupal_attributes(array('class' => 'commerce-gc-line-item-review-title')) . '>' . check_plain($order->mail) . '</div>'
            );
          }
          else {
            $render = $field_render;
          }
          $output[$line_item->line_item_id][$field_name] = $render;
        }
      }
    }
  }
  
  return drupal_render($output);
}
