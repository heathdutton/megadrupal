<?php
/**
 * @file
 * Checkout pane for profile2.
 */

/**
 * Implements hook_commerce_checkout_pane_info().
 *
 * This is where we define the name of the pane, related information, and
 * the base name of all the form builder functions used to present the
 * pane.
 */
function commerce_profile2_commerce_checkout_pane_info() {
  $panes['commerce_profile2_checkout_pane'] = array(
    'title' => t('Information'),
    'base' => 'commerce_profile2_checkout_pane',

    // The checkout page where this should be displayed by default.
    'page' => 'profile2_checkout_page',
    'weight' => -5,
  );

  return $panes;
}

/**
 * Account pane: settings form callback.
 *
 * This form provides configuration information for the pane.
 */
function commerce_profile2_checkout_pane_settings_form($checkout_pane) {
  $form['commerce_profile2_checkout_pane_message'] = array(
    '#type' => 'item',
    '#title' => t('EPC Med Checkout Pane.'),
    '#markup' => t('This pane has no admin settings'),
  );

  return $form;
}

/**
 * Commerce profile2: form callback.
 *
 * This is a standard FAPI form which will be presented in the pane.
 * The form gathers and stores information from $order->data, an array which
 * can be populated with free-form keys and values.
 */
function commerce_profile2_checkout_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  // Iterate through all the line items.
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    // Load the product from this line item.
    $product = commerce_product_load($line_item_wrapper->commerce_product->product_id->value());
    // Find which field types are attached to the product.
    $fields_info = field_info_instances('commerce_product', $product->type);
    $items = array();
    foreach ($fields_info as $field_name => $value) {
      $field_info = field_info_field($field_name);
      // Sets $items array with the product's commerce_profile2 field values.
      if (($field_info['type']) == ('commerce_profile2')) {
        $items = field_get_items('commerce_product', $product, $field_name);
      }
    }
    // Display the appropriate profile2 form if the product has a
    // 'commerce_profile2' field.
    if (isset($items['0']['profile2_type'])) {
      // Get the profile2 type field for this product.
      $profile2_type = check_plain($items['0']['profile2_type']);
      // Handy variables to reuse.
      $sku = $product->sku;
      $title = $product->title;
      // $quantity tells us how many times to repeat the form display for
      // each product line item.
      $quantity = $line_item_wrapper->quantity->value();
      for ($i = 1; $i <= $quantity; $i++) {
        // Build the form.
        $form['profile2_' . $sku]['profile2_' . $i] = array(
          '#type' => 'fieldset',
          '#tree' => TRUE,
          '#title' => t($title . ' #@profile', array('@profile' => $i)),
        );

        // Add our hidden values for later processing.
        $form['profile2_' . $sku]['profile2_' . $i]['hidden'] = array(
          '#type' => 'hidden',
          '#value' => array(
            'title' => t($title . ' #@profile', array('@profile' => $i)),
            'profile2_type' => $profile2_type,
            'user_role' => $items['0']['user_role'],
            'created_email' => $items['0']['created_email'],
          ),
        );

        $form['profile2_' . $sku]['profile2_' . $i]['profile2_email'] = array(
          '#type' => 'textfield',
          '#title' => t('E-mail address'),
          '#required' => TRUE,
          '#default_value' => !empty($order->data['checkout_pane']['profile2_' . $sku]['profile2_' . $i]['profile2_email']) ? $order->data['checkout_pane']['profile2_' . $sku]['profile2_' . $i]['profile2_email'] : '',
        );

        $form['profile2_' . $sku]['profile2_' . $i]['profile2_form'] = array(
          '#type' => 'fieldset',
          '#title' => t($profile2_type),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
        );

        // Create dummy profile to load the form.
        $profile = profile_create(array(
          'type' => $profile2_type,
          'uid' => $i + 99999999,
        ));
        // Add order data to pre-populate form when checkout review
        // back button is clicked.
        if (isset($order->data['checkout_pane']['profile2_' . $sku]['profile2_' . $i])) {
          foreach ($order->data['checkout_pane']['profile2_' . $sku]['profile2_' . $i] as $key => $data) {
            $profile->$key = $order->data['checkout_pane']['profile2_' . $sku]['profile2_' . $i][$key];
          }
        }
        // Attach the profile2 form.
        field_attach_form('profile2', $profile, $form['profile2_' . $sku]['profile2_' . $i]['profile2_form'], $form_state);

        $fields = &$form['profile2_' . $sku]['profile2_' . $i]['profile2_form'];
        foreach ($fields as $key => $data) {
          if (is_array($data) && drupal_substr($key, 0, 1) != '#') {
            // We set the parents on just fields so we can find them in
            // form_state later; fields in the form array don't start with a #.
            $fields[$key]['#parents'] = array(
              'commerce_profile2_checkout_pane',
              'profile2_' . $sku,
              'profile2_' . $i,
              $key,
            );
            // This allows the fields to work with Conditional Fields module.
            $fields[$key][LANGUAGE_NONE]['#field_parents'] = $fields[$key]['#parents'];
            // Remove the last value from the array (name of the field itself).
            array_pop($fields[$key][LANGUAGE_NONE]['#field_parents']);
          }
        }
      }
    }
  }

  return $form;
}

/**
 * Validation callback. Validates the e-mail.
 */
function commerce_profile2_checkout_pane_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  if (isset($form_state['values']['commerce_profile2_checkout_pane'])) {
    $pane_values = $form_state['values']['commerce_profile2_checkout_pane'];
    $mails = array();
    foreach ($pane_values as $profile2_sku => $profile2_index) {
      foreach ($profile2_index as $key => $value) {
        $mail = $value['profile2_email'];

        // Check if the e-mail is valid.
        if (!valid_email_address($mail)) {
          form_set_error('commerce_profile2_checkout_pane][' . $profile2_sku . '][' . $key . '][profile2_email', 'Invalid e-mail address.');
          return FALSE;
        }

        // Check if a profile already exists using that e-mail.
        $account = user_load_by_mail($mail);
        if ($account) {
          $profile = profile2_load_by_user($account, $value['hidden']['profile2_type']);
          if ($profile) {
            form_set_error('commerce_profile2_checkout_pane][' . $profile2_sku . '][' . $key . '][profile2_email', 'This e-mail has already been used.');
            return FALSE;
          }
        }

        // Check if the same e-mail address is entered more than once.
        if (in_array($mail, $mails)) {
          form_set_error('commerce_profile2_checkout_pane][' . $profile2_sku . '][' . $key . '][profile2_email', 'Each e-mail address must be unique.');
          return FALSE;
        }
        // Add this validated email to our mails array for the next iteration.
        $mails[] = $mail;
      }
    }
  }

  return TRUE;
}

/**
 * Checkout form submission callback.
 */
function commerce_profile2_checkout_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if (!empty($form_state['values'][$checkout_pane['pane_id']])) {
    $order->data['checkout_pane'] = $form_state['values'][$checkout_pane['pane_id']];
  }
}

/**
 * Presents the profile2 information collected in the Review checkout pane.
 */
function commerce_profile2_checkout_pane_review($form, $form_state, $checkout_pane, $order) {
  $order_data = (array) $order->data['checkout_pane'];
  foreach ($order_data as $profile2_sku => $profile2_index) {
    foreach ($profile2_index as $key => $value) {
      $content[$profile2_sku][$key] = array(
        '#type' => 'item',
        '#title' => t($order_data[$profile2_sku][$key]['hidden']['title']),
        '#markup' => 'Email: ' . check_plain($order_data[$profile2_sku][$key]['profile2_email']),
      );
    }
  }

  return drupal_render($content);
}
