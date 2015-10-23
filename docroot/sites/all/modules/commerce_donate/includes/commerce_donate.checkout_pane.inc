<?php

/**
 * @file
 * Contains callback functions for the commerce_donate checkout pane.
 */

/**
 * Configure the donation checkout pane.
 */
function commerce_donate_checkout_pane_settings_form($checkout_pane) {
  $options = array();
  $products = commerce_product_load_multiple(array(), array('type' => 'donation', 'status' => 1));
  if (empty($products)) {
    drupal_set_message(t('You must configure at least one donation product in order to accept donations during checkout.'), 'error');
  }
  foreach ($products as $pid => $product) {
    $options[$pid] = $product->title;
  }

  $form['commerce_donate_checkout_pane_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkout pane title'),
    '#default_value' => variable_get('commerce_donate_checkout_pane_title', ''),
  );

  $form['commerce_donate_checkout_pane_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Checkout pane description'),
    '#default_value' => variable_get('commerce_donate_checkout_pane_description', ''),
  );

  $form['commerce_donate_checkout_pane_product_id'] = array(
    '#type' => 'select',
    '#title' => t('Donation product'),
    '#options' => $options,
    '#default_value' => variable_get('commerce_donate_checkout_pane_product_id', 0),
    '#description' => t('Donation product to use when adding line item to the cart.'),
  );
  $form['commerce_donate_checkout_pane_override_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override available options'),
    '#default_value' => variable_get('commerce_donate_checkout_pane_override_options', FALSE),
  );
  $form['commerce_donate_checkout_pane_donate_amounts'] = array(
    '#type' => 'textarea',
    '#title' => t('Available options'),
    '#description' => t('A list of values that are, by default, available for selection. Enter one value per line, in the format key|label. The key is the value that will be stored in the database, and the label is what will be displayed to the user.  For example, "5|5.00"'),
    '#default_value' => variable_get('commerce_donate_checkout_pane_donate_amounts', ''),
    '#states' => array(
      'visible' => array(
        ':input[name="commerce_donate_checkout_pane_override_options"]' => array('checked' => TRUE),
      ),
    ),
  );

  return $form;
}

/**
 * Create the donate checkout pane form.
 */
function commerce_donate_checkout_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $product_id = variable_get('commerce_donate_checkout_pane_product_id', 0);
  if (!empty($product_id) || !is_numeric($product_id)) {
    $pane_form = array();
    $product = commerce_product_load($product_id);
    $line_item = commerce_product_line_item_new($product, 1, 0, array(), 'commerce_donate');
    $form_state['donate_line_item'] = $line_item;

    // Add the line item's fields to a container on the form.
    $pane_form[$pane_id] = array(
      '#type' => 'container',
      '#parents' => array($pane_id),
      '#weight' => 10,
      '#tree' => TRUE,
    );

    // Check if donation has already been added.
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      if (!in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        continue;
      }
      $id = (int) $line_item_wrapper->commerce_product->product_id->value();
      if ($id == $product_id) {
        $pane_form[$pane_id]['already_added'] = array(
          '#markup' => t('A donation has already been added to your order. You can remove this donation by editing your <a href="@url">shopping cart</a>.', array('@url' => url('cart'))),
        );
        // Return without additional fields.
        return $pane_form;
      }
    }

    field_attach_form('commerce_line_item', $line_item, $pane_form[$pane_id], $form_state);
    foreach (element_children($pane_form[$pane_id]) as $field_name) {
      $info = field_info_instance('commerce_line_item', $field_name, $line_item->type);
      $pane_form[$pane_id][$field_name]['#commerce_cart_settings'] = commerce_cart_field_instance_access_settings($info);

      if (empty($pane_form[$pane_id][$field_name]['#commerce_cart_settings']['field_access'])) {
        $pane_form[$pane_id][$field_name]['#access'] = FALSE;
      }
      // Add in a "none" option and make it the default.
      elseif ($field_name == 'commerce_donate_amount') {
        $language = $pane_form[$pane_id][$field_name]['#language'];
        $form_state['language'] = $language;
        $pane_form[$pane_id][$field_name][$language]['#default_value'] = '';
        $pane_form[$pane_id][$field_name][$language]['#required'] = FALSE;
        // Allow for people changing the select to a text field.
        if (!empty($pane_form[$pane_id][$field_name][$language]['#options'])) {
          $default_options = $pane_form[$pane_id][$field_name][$language]['#options'];
          // Override default field available options.
          if (variable_get('commerce_donate_checkout_pane_override_options', FALSE)) {
            $overridden_options_string = variable_get('commerce_donate_checkout_pane_donate_amounts', '');
            if (!empty($overridden_options_string)) {
              $line_options = explode("\n", $overridden_options_string);
              $pos = strpos($overridden_options_string, '|');

              if ($pos !== FALSE) {
                // There are keys.
                foreach ($line_options as $line) {
                  $exploded = explode('|', $line);
                  $temp_options[$exploded[0]] = $exploded[1];
                }
                $default_options = $temp_options;
              }
            }
          }
          // Add in 'none' option.
          $options = array('' => t('None'));
          foreach ($default_options as $key => $label) {
            $options[$key] = $label;
          }
          $pane_form[$pane_id][$field_name][$language]['#options'] = $options;
        }

        $description = variable_get('commerce_donate_checkout_pane_description', '');
        if (!empty($description)) {
          $pane_form[$pane_id][$field_name][$language]['#prefix'] = filter_xss($description);
        }

      }
    }
    return $pane_form;
  }
}

/**
 * Validate the donation amount entered, if any.
 */
function commerce_donate_checkout_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {

  $pane_id = $checkout_pane['pane_id'];
  if (!empty($form[$pane_id]) && !empty($form_state['values']) && !empty($form_state['values'][$pane_id])) {
    $pane_form = $form[$pane_id];
    $pane_values = $form_state['values'][$pane_id];
    $language = $form_state['language'];
    $amount = (!empty($pane_values['commerce_donate_amount'][$language][0]['value'])) ? $pane_values['commerce_donate_amount'][$language][0]['value'] : 0;

    // Validate the amount entered if not 'none'.
    if (!empty($amount)) {
      field_attach_form_validate('commerce_line_item', $form_state['donate_line_item'], $form[$pane_id], $form_state);
      if (form_get_errors()) {
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
 * Add the additional donation as a line item to the cart.
 */
function commerce_donate_checkout_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  $pane_id = $checkout_pane['pane_id'];
  $pane_values = $form_state['values'];

  // No donation pane, or perhaps product, available.
  if (empty($form_state['donate_line_item']) || empty($pane_values[$pane_id])) {
    return;
  }

  $line_item = $form_state['donate_line_item'];
  $language = $form_state['language'];
  $amount = (!empty($pane_values[$pane_id]['commerce_donate_amount'][$language][0]['value'])) ? $pane_values[$pane_id]['commerce_donate_amount'][$language][0]['value'] : 0;

  if (!empty($amount)) {
    field_attach_submit('commerce_line_item', $line_item, $form[$pane_id], $form_state);
    $product_id = variable_get('commerce_donate_checkout_pane_product_id', 0);

    if (!empty($product_id) && !empty($amount) && !empty($order)) {
      $product = commerce_product_load($product_id);
      $line_item->line_item_label = $product->sku;
      // Save the line item and add it to the cart.
      commerce_line_item_save($line_item);
      commerce_cart_product_add($order->uid, $line_item, FALSE);
    }
  }
}
