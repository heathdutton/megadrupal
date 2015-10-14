<?php

/**
 * @file
 * This defines the webform callbacks for the productfield
 * webform component.
 */

require_once drupal_get_path('module', 'webform') . '/components/select.inc';

/**
 * Implements _webform_defaults_component().
 */
function _webform_defaults_productfield() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'multiple' => NULL,
      'choose_quantity' => NULL,
      'aslist' => NULL,
      'optrand' => 0,
      'other_option' => NULL,
      'other_text' => t('Other...'),
      'title_display' => 0,
      'description' => '',
      'custom_keys' => FALSE,
      'options_source' => '',
      'private' => FALSE,
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_productfield($component) {
  $form = array();

  $items = isset($component['extra']['items']) && is_array($component['extra']['items']) ? $component['extra']['items'] : array();
  $product_ids = array();
  $skus = array();

  // Build an array of product IDs from this field's values.
  foreach ($items as $item) {
    $product_ids[] = $item['product_id'];
    $skus[] = $item['sku'];
  }

  $form['extra']['items'] = array(
    '#type' => 'textfield',
    '#title' => t('Product skus'),
    '#description' => t('List the product skus you would like to offer as options on this webform. The user must select one of these products.  When the webform is saved, the user has that product added to thier basket.'),
    '#default_value' => implode(', ', $skus),
    '#autocomplete_path' => 'commerce_webform/autocomplete',
    '#size' => 128,
    '#maxlength' => 2048,
    '#element_validate' => array('_webform_edit_validate_productfield'),
    '#weight' => 4,
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default sku'),
    '#default_value' => $component['value'],
    '#description' => t('Enter the product sku above which should be selected by default.'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#weight' => 3,
  );

  $form['extra']['choose_quantity'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow the user to set the quantity.'),
    '#default_value' => empty($component['extra']['choose_quantity']) ? 0 : $component['extra']['choose_quantity'],
    '#description' => t('Check this option if the user should be allowed to set the number of products selected. The default is 1 for single selections and 0 if it is possbile to select multiple products.'),
    '#weight' => 0,
  );

  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#weight' => 0,
  );

  $form['display']['aslist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Listbox'),
    '#default_value' => $component['extra']['aslist'],
    '#description' => t('Check this option if you want the select component to be of listbox type instead of radio buttons or checkboxes.'),
    '#weight' => 0,
    '#parents' => array('extra', 'aslist'),
  );

  return $form;
}

/**
 * Element validation callback. Ensure keys are not duplicated.
 */
function _webform_edit_validate_productfield($element, &$form_state) {
  // If a value was entered into the autocomplete...
  if (!empty($element['#value'])) {
    // Translate SKUs into product IDs.
    $typed_skus = drupal_explode_tags($element['#value']);

    $value = array();

    // Loop through all the entered SKUs...
    foreach ($typed_skus as $typed_sku) {
      // To see if the product actually exists...
      if ($product = commerce_product_load_by_sku(trim($typed_sku))) {
        // And store its product ID for later validation.
        $value[$product->product_id] = array(
          'product_id' => $product->product_id,
          'title' => $product->title,
          'sku' => $product->sku,
          'type' => $product->type,
        );
      }
    }
  }
  else {
    $value = array();
  }

  // Update the value of this element so the field can validate the product IDs.
  form_set_value($element, $value, $form_state);
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_productfield($component, $value = NULL, $filter = TRUE) {
  $node = isset($component['nid']) ? node_load($component['nid']) : NULL;
  $items = isset($component['extra']['items']) ? $component['extra']['items'] : array();
  $multiple = $component['extra']['multiple'];
  $choose_quantity = !empty($component['extra']['choose_quantity']);

  // Each product should have a hidden element which describes it which allows
  // js to identify the products for possible dynamic total field.
  // @TODO Replace with RDFa.
  $hidden_elements_html = '';

  $options = array();

  if ($multiple != '1') {
    $options[''] = ' - ' . (empty($component['mandatory']) ? t('None') : t('Select')) . ' - ';
  }

  // Build an array of product IDs from this field's values.
  $product_ids = array();
  foreach ($items as $item) {
    $product_ids[] = $item['product_id'];
  }

  $products = commerce_product_load_multiple($product_ids);

  foreach ($products as $product) {
    // Check if the product is active.
    if ($product->status == 1) {
      $price = commerce_product_calculate_sell_price($product);
      // @TODO - needs a theme function here.
      $options[$product->product_id] = filter_xss($product->title) . ' [' . commerce_currency_format($price['amount'], $price['currency_code'], $product) . ']';
      $hidden_elements_html .= "<input type='hidden' name='commerce_webform_product[{$product->product_id}]' value='{$price['amount']}' />\n";
    }
  }

  $default_product_ids = $multiple ? array() : 0;
  $default_quantities = $multiple ? array() : 1;
  $disabled = FALSE;

  if (!empty($value)) {
    // Load a previously saved value.
    foreach ($value as $id => $encoded_details) {
      $details = json_decode($encoded_details);
      if ($details->paid && !user_access('alter product field on paid webform')) {
        $disabled = TRUE;
      }
      if ($multiple) {
        $default_product_ids[] = $details->product_id;
        if ($choose_quantity) {
          $default_quantities[$details->product_id] = $details->quantity;
        }
        else {
          $default_quantities = $details->quantity;
        }
      }
      else {
        $default_product_ids = $details->product_id;
        $default_quantities = $details->quantity;
      }
    }
  }
  elseif (!empty($component['value'])) {
    $defaults = explode(',', $component['value']);
    foreach ($defaults as $default) {
      $product = commerce_product_load_by_sku(trim($default));

      if (!empty($product) && array_key_exists($product->product_id, $options)) {
        if ($component['extra']['multiple']) {
          $default_product_ids[] = $product->product_id;
        }
        else {
          $default_product_ids = $product->product_id;
        }
      }
    }
  }

  // @TODO: Make default quantity configurable.
  // @TODO: Add validation of the quantity value.
  $element = array(
    '#weight' => $component['weight'],
  );

  if (!$multiple || !$choose_quantity) {
    // Single quantity options.
    if ($disabled) {
      // The product has been paid for so should not be able to change it.
      $rows = array();
      $element[] = array(
        '#type' => 'value',
        '#value' => $default_product_ids,
      );

      $element[] = array(
        '#type' => 'value',
        '#value' => $default_quantities,
      );

      if (is_array($default_product_ids)) {
        foreach ($default_product_ids as $product_id) {
          $rows[] = array($product_id, $options[$product_id], $default_quantities);
        }
      }
      elseif ($default_product_ids > 0) {
        $rows[] = array($default_product_ids, $options[$default_product_ids], $default_quantities);
      }

      // @TODO - put this in a theme function.
      $element['#markup'] = '<strong>' . _webform_filter_xss($component['name']) . '</strong>';
      $element['#markup'] .= theme('table', array('header' => array('Product ID', 'Product name', 'Quantity'), 'rows' => $rows));
      $element['#markup'] .= '<p>' . t('This order has been completed and products choices cannot be edited online.') . '</p>';
    }
    else {
      $element[] = array(
        '#type' => $component['extra']['aslist'] ? 'select' : ($component['extra']['multiple'] ? 'checkboxes' : 'radios'),
        '#multiple' => $component['extra']['multiple'],
        '#size' => $component['extra']['aslist'] && $component['extra']['multiple'] ? 4 : 0,
        '#title' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#required' => $component['mandatory'],
        '#weight' => $component['weight'],
        '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
        '#translatable' => array('title', 'description', 'options'),
        '#options' => $options,
        '#default_value' => $default_product_ids,
        '#prefix' => '<div class="commerce_webform_products">',
        '#suffix' => $hidden_elements_html . '</div>',
        // Needed to disable double-wrapping of radios and checkboxes.
        '#pre_render' => array(),
        '#theme_wrappers' => array('webform_element'),
        '#element_validate' => array('_webform_productfield_selection_validate'),
      );

      $editable = empty($component['extra']['multiple']) && !empty($component['extra']['choose_quantity']);
      $element[] = array(
        '#type' => $editable ? 'textfield' : 'value',
        '#title' => t('@product quantity', array('@product' => ($filter ? _webform_filter_xss($component['name']) : $component['name']))),
        '#default_value' => $choose_quantity ? (is_array($default_quantities) ? 1 : $default_quantities) : 1,
        '#element_validate' => array('_webform_productfield_quantity_validate'),
        '#required' => $component['mandatory'],
        '#weight' => $component['weight'] + 0.5,
      );
    }
  }
  else {
    // Product field is multiple and user can choose quantity.
    $i = 1;

    if ($disabled) {
      $rows = array();
      foreach ($default_quantities as $product_id => $quantity) {
        $element[$product_id] = array(
          '#type' => 'value',
          '#value' => $quantity,
        );
        $rows[] = array($product_id, $options[$product_id], $quantity);
      }

      // @TODO - put this in a theme function.
      $element['#markup'] = '<strong>' . _webform_filter_xss($component['name']) . '</strong>';
      $element['#markup'] .= theme('table', array('header' => array('Product ID', 'Product name', 'Quantity'), 'rows' => $rows));
      $element['#markup'] .= '<p>' . t('This order has been completed and products choices cannot be edited online.') . '</p>';
    }
    else {
      $element = array(
        '#type' => 'fieldset',
        '#title' => _webform_filter_xss($component['name']),
        '#title_display' => $component['extra']['title_display'] ? $component['extra']['title_display'] : 'before',
        '#element_validate' => array('_webform_productfield_required_multiple_quantities_validate'),
        '#required' => $component['mandatory'],
        '#weight' => $component['weight'],
        // Hide title as fieldsets don't support #title_display.
        '#pre_render' => array('webform_element_title_display'),
      );

      foreach ($options as $product_id => $product_name) {
        $element[$product_id] = array(
          '#type' => 'textfield',
          '#title' => $product_name,
          '#title_display' => 'after',
          '#size' => 2,
          '#default_value' => isset($default_quantities[$product_id]) ? $default_quantities[$product_id] : 0,
          '#element_validate' => array('_webform_productfield_quantity_validate'),
          '#weight' => $component['weight'] + ('0.' . $i),
        );
        $i++;
      }
    }
  }

  return $element;
}

/**
 * Implements _webform_display_component().
 */
function _webform_display_productfield($component, $value, $format = 'html') {
  $options = array();
  foreach ($component['extra']['items'] as $product_id => $details) {
    $options[$product_id] = "{$details['sku']}: {$details['title']} ";
  }

  return array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#theme' => 'webform_display_productfield',
    '#theme_wrappers' => $format == 'html' ? array('webform_element') : array('webform_element_text'),
    '#format' => $format,
    '#options' => $options,
    '#value' => (array) $value,
    '#translatable' => array('title', 'options'),
  );
}

/**
 * Implements _webform_submit_component().
 * This executes when the webform is submitted by the user.
 * Convert FAPI 0/1 values into something saveable.
 */
function _webform_submit_productfield($component, $value) {
  $return = array();
  $multiple = $component['extra']['multiple'];
  $choose_quantity = !empty($component['extra']['choose_quantity']);

  if ($multiple && $choose_quantity) {
    foreach ($value as $product_id => $quantity) {
      if ($quantity > 0) {
        $details = array(
          'product_id' => $product_id,
          'quantity' => $quantity,
          'order_id' => FALSE,
          'line_item_id' => FALSE,
          'paid' => FALSE,
        );

        $return[] = json_encode($details);
      }
    }
  }
  else {
    if (is_array($value[0])) {

      foreach ($value[0] as $id => $product_id) {
        $details = array(
          'product_id' => $product_id,
          'quantity' => 1,
          'order_id' => FALSE,
          'line_item_id' => FALSE,
          'paid' => FALSE,
        );

        $return[] = json_encode($details);
      }
    }
    else {
      $details = array(
        'product_id' => $value[0],
        'quantity' => empty($value[0]) ? '0' : $value[1],
        'order_id' => FALSE,
        'line_item_id' => FALSE,
        'paid' => FALSE,
      );

      $return[] = json_encode($details);
    }
  }

  return $return;
}

/**
 * Format the text output for this component.
 */
function theme_webform_display_productfield($variables) {
  $element = $variables['element'];

  // Flatten the list of options so we can get values easily. These options
  // may be translated by hook_webform_display_component_alter().
  $options = $element['#options'];

  $items = array();

  foreach ($element['#value'] as $value) {
    $value = json_decode($value);
    // Administer provided values.
    if (!empty($value->product_id)) {
      if (isset($options[$value->product_id])) {
        $paid_display_option = empty($value->paid) ? t('Unpaid') : t('Paid');
        $paid_display_option = empty($value->order_id) ? $paid_display_option : l($paid_display_option, "admin/commerce/orders/{$value->order_id}/view");
        $name = _webform_filter_xss($options[$value->product_id]);
        $items[] = "{$value->quantity} x {$name} (<strong>{$paid_display_option}</strong>)";
      }
      else {
        $items[] = check_plain($value->product_id);
      }
    }
  }

  if ($element['#format'] == 'html') {
    $output = count($items) > 1 ? theme('item_list', array('items' => $items)) : (isset($items[0]) ? $items[0] : t('Not selected'));
  }
  else {
    if (count($items) > 1) {
      foreach ($items as $key => $item) {
        $items[$key] = ' - ' . $item;
      }
      $output = implode("\n", $items);
    }
    else {
      $output = isset($items[0]) ? $items[0] : t('Not selected');
    }
  }

  return $output;
}

/**
 * Implements _webform_analysis_component().
 */
function _webform_analysis_productfield($component, $sids = array(), $single = FALSE) {
  $query = db_select('webform_submitted_data', 'wsd', array('fetch' => PDO::FETCH_ASSOC))
    ->fields('wsd', array('data'))
    ->condition('nid', $component['nid'])
    ->condition('cid', $component['cid'])
    ->condition('data', '', '<>')
    ->groupBy('data');

  if (count($sids)) {
    $query->condition('sid', $sids, 'IN');
  }

  $results = $query->execute();

  $rows = array();

  foreach ($results as $result) {
    $submission = json_decode($result['data']);
    if (isset($component['extra']['items'][$submission->product_id])) {
      $display_option = _webform_filter_xss($component['extra']['items'][$submission->product_id]['title']);

      // Update the paid count.
      $paid_display_option = !empty($submission->paid) ? 'Paid' : 'Unpaid';
      if (isset($rows[$submission->product_id . '_' . $paid_display_option])) {
        $rows[$submission->product_id . '_' . $paid_display_option]++;
      }
      else {
        $rows[$submission->product_id . '_' . $paid_display_option] = array($display_option . ' (' . _webform_filter_xss(t($paid_display_option)) . ')', 1);
      }
    }
  }

  return $rows;
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_productfield($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = $component['extra']['items'];

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    $option_value = json_decode($option_value);
    if (!empty($option_value->product_id)) {
      if (isset($options[$option_value->product_id])) {
        $item = $option_value->quantity . ' x ' . _webform_filter_xss($options[$option_value->product_id]['title']);
      }
      else {
        $item = check_plain($option_value->product_id);
      }

      $paid_display_option = empty($option_value->paid) ? t('Unpaid') : t('Paid');
      $paid_display_option = empty($option_value->order_id) ? $paid_display_option : l($paid_display_option, "admin/commerce/orders/{$option_value->order_id}/view");
      $item .= " (<strong>{$paid_display_option}</strong>)";
      $items[] = $item;
    }
  }

  return implode('<br />', $items);
}

/**
 * Implements _webform_csv_headers_component().
 */
function _webform_csv_headers_productfield($component, $export_options) {
  $options = $component['extra']['items'];

  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  $headers[0][] = '';
  $headers[1][] = '';

  foreach ($options as $product_id => $details) {
    $headers[2][] = $details['sku'] . ': PAID';
    $headers[2][] = $details['sku'] . ': UNPAID';
  }

  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_productfield($component, $export_options, $values) {
  $options = $component['extra']['items'];

  foreach ($values as $id => $value) {
    $values[$id] = json_decode($value);
  }

  $return = array();

  foreach ($options as $key => $item) {
    $index = FALSE;
    foreach ($values as $value) {
      if ($value->product_id == $key) {
        $index = $value;
      }
    }

    if ($index !== FALSE) {
      if (!empty($index->paid)) {
        $return[] = $index->quantity;
        $return[] = '0';
      }
      else {
        $return[] = '0';
        $return[] = $index->quantity;
      }
    }
    else {
      $return[] = '0';
      $return[] = '0';
    }
  }

  return $return;
}

/**
 * Validate the user entered value in the quantity field.
 */
function _webform_productfield_quantity_validate($element, &$form_state, $form) {
  $value = $form_state['values'];
  foreach ($element['#parents'] as $parent) {
    $value = $value[$parent];
  }

  $name = implode('][', $element['#parents']);

  if (!isset($value) || empty($value)) {
    $value = 0;
  }

  if (!is_numeric($value)) {
    form_set_error($name, 'Quantity must be a number.');
  }
  elseif ($element['#required'] && $value < 1) {
    form_set_error($name, 'Quantity must be greater than 0.');
  }
  elseif ($element['#required'] && $value < 0) {
    form_set_error($name, 'Quantity must be a positive number or 0.');
  }
}

/**
 * Validate the user entered value in the quantity field.
 */
function _webform_productfield_selection_validate($element, &$form_state, $form) {
  if ($element['#required'] && $element['#multiple'] == '0') {
    $value = $form_state['values'];
    $name = '';

    foreach ($element['#parents'] as $index => $container) {
      $value = $value[$container];
      $name .= !empty($name) ? '][' : '';
      $name .= $element['#parents'][$index];
    }

    if (!isset($value[0]) || $value[0] < 1) {
      form_set_error($name, t('!name field is required', array('!name' => $element['#title'])));
    }
  }
}

/**
 * Validate a required multiple selection with quantity selection control.
 * At least one sub element must have a positivie quantity set.
 */
function _webform_productfield_required_multiple_quantities_validate($element, &$form_state, $form) {
  if ($element['#required']) {
    $form_key = $element['#webform_component']['form_key'];
    foreach ($form_state['values']['submitted'][$form_key] as $product_id => $quantity) {
      if ($quantity > 0) {
        // At least one element has a quantity set.
        return;
      }
    }
    $name = implode('][', $element['#parents']);
    form_set_error($name, t('You must choose at least one product from this selection by setting its quantity to something greater than 1'));
  }
}
