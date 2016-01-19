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
      'product_type' => '',
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

  $form['extra']['product_type'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => commerce_product_type_options_list(),
    '#title' => t('Product type'),
    '#default_value' => $component['extra']['product_type'],
    '#description' => t('Use either this OR the product skus field below. By selecting a product type here, all products of this type will be available for selection on the webform.'),
    '#weight' => 1,
  );

  $form['extra']['items'] = array(
    '#type' => 'textfield',
    '#title' => t('Product skus'),
    '#description' => t('Use this instead of setting a product type above. List the product skus you would like to offer as options on this webform. When the webform is saved, the user has that product added to thier basket.'),
    '#default_value' => implode(', ', $skus),
    '#autocomplete_path' => 'commerce_webform/autocomplete',
    '#size' => 128,
    '#maxlength' => 2048,
    '#element_validate' => array('_webform_edit_validate_productfield'),
    '#weight' => 2,
  );

  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default sku'),
    '#default_value' => $component['value'],
    '#description' => t('Enter the product sku which should be selected by default or leave blank for no default. A token can be used here, e.g. [current-page:query:sku] to retrieve it from the URL query parameter sku.'),
    '#size' => 60,
    '#maxlength' => 1024,
    '#weight' => 3,
  );

  $form['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#default_value' => $component['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
    '#weight' => 4,
  );

  $form['extra']['choose_quantity'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow the user to set the quantity.'),
    '#default_value' => empty($component['extra']['choose_quantity']) ? 0 : $component['extra']['choose_quantity'],
    '#description' => t('Check this option if the user should be allowed to set the number of products selected. The default is 1 for single selections and 0 if it is possbile to select multiple products.'),
    '#weight' => 5,
  );

  $form['extra']['choose_quantity_min'] = array(
    '#type' => 'textfield',
    '#size' => '6',
    '#title' => t('Minimum quantity:'),
    '#states' => array(
      'invisible' => array(
        ':input[name="extra[choose_quantity]"]' => array('checked' => FALSE)
      )
    ),
    '#default_value' => empty($component['extra']['choose_quantity_min']) ? 0 : $component['extra']['choose_quantity_min'],
    '#description' => t('Minimum quantity the user can select.'),
    '#weight' => 6,
  );

  $form['extra']['choose_quantity_max'] = array(
    '#type' => 'textfield',
    '#size' => '6',
    '#title' => t('Maximum quantity:'),
    '#states' => array(
      'invisible' => array(
        ':input[name="extra[choose_quantity]"]' => array('checked' => FALSE)
      )
    ),
    '#default_value' => empty($component['extra']['choose_quantity_max']) ? 0 : $component['extra']['choose_quantity_max'],
    '#description' => t('Maximum quantity the user can select. Leave at 0 if you would like to render a textfield instead of a select option.'),
    '#weight' => 7,
  );

  $form['display']['aslist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Listbox'),
    '#default_value' => $component['extra']['aslist'],
    '#description' => t('Check this option if you want the select component to be of list box type instead of radio buttons or checkboxes. This does nothing if both multiple and choose quantity are selected above.'),
    '#weight' => 8,
    '#parents' => array('extra', 'aslist'),
  );

  $form['extra']['hide_price'] = array(
    '#type' => 'select',
    '#title' => t('Hide prices'),
    '#options' => array(
      'show' => t('Show all prices (default)'),
      'hide' => t('Hide all prices'),
      'zero' => t('Hide zero prices'),
    ),
    '#default_value' => empty($component['extra']['hide_price']) ? 'show' : $component['extra']['hide_price'],
    '#description' => t('Check this option if product prices should be hidden in webform for none, all or only zero priced products. This has no effect on other product displays like shopping cart and checkout.'),
    '#weight' => 8,
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
  $product_ids = _webform_productfield_product_ids($component);

  $element = array(
    '#type' => 'webform_productfield',
    '#choose_quantity' => !empty($component['extra']['choose_quantity']),
    '#multiple' => !empty($component['extra']['multiple']),
    '#productids' => $product_ids,
    '#required' => !empty($component['required']),
    '#default_value' => isset($value) ? $value : _webform_filter_values($component['value'], NULL, NULL, NULL, FALSE),
    '#weight' => $component['weight'],
    '#aslist' => !empty($component['extra']['aslist']),
    '#name' => $filter ? _webform_filter_xss($component['name']) : $component['name'],
    '#description' => $filter ? _webform_filter_descriptions($component['extra']['description'], $node) : $component['extra']['description'],
    '#title_display' => $component['extra']['title_display'],
    '#theme_wrappers' => array('webform_element'),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'commerce_webform') . '/commerce_webform.js'),
    ),
  );

  if (empty($component['extra']['choose_quantity'])) {
    // If we are not allowing the use to select the quantity then we don't use a
    // fieldset so we want to set a title here.
    $element['#title'] = $filter ? _webform_filter_xss($component['name']) : $component['name'];
  }

  return $element;
}

/**
 * Form API #process function to expand a productfield.
 */
function _commerce_webform_productfield_expand($element) {
  $element += element_info('webform_productfield');

  $product_ids = empty($element['#productids']) ? array() : $element['#productids'];
  $choose_quantity = !empty($element['#choose_quantity']);
  $required = !empty($element['#required']);
  $value = $element['#default_value'];
  $aslist = !empty($element['#aslist']);
  $multiple = !empty($element['#multiple']);
  $name = $element['#name'];
  $description = $element['#description'];
  $title_display = !empty($element['#title_display']) ? $element['#title_display'] : 'before';

  // Each product should have a hidden element which describes it which allows
  // js to identify the products for possible dynamic total field.
  // @TODO Replace with RDFa.
  $hidden_elements_html = '';

  $options = array();

  if (!$multiple && !$required) {
    $options[''] = ' - ' . t('None') . ' - ';
  }

  $products = commerce_product_load_multiple($product_ids);

  foreach ($products as $product) {
    // Check if the product is active.
    if ($product->status == 1) {
      $price = commerce_product_calculate_sell_price($product);
      $options[$product->product_id] = theme('commerce_webform_product_display', array(
        'product' => $product,
        'price' => $price,
        'element' => $element,
      ));
      $hidden_elements_html .= "<input type='hidden' name='commerce_webform_product[{$product->product_id}]' value='{$price['amount']}' />\n";
    }
  }

  $default_product_ids = $multiple ? array() : '';
  $default_quantities = $multiple ? array() : 1;
  $disabled = FALSE;

  if (is_array($value)) {
    // Load a previously saved value.
    foreach ($value as $id => $encoded_details) {
      $details = json_decode($encoded_details);

      if (empty($details)) {
        continue;
      }

      if (!empty($details->order_id)) {
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
  elseif (!empty($value)) {
    $defaults = explode(',', $value);
    foreach ($defaults as $default) {
      $product = commerce_product_load_by_sku(trim($default));

      if (!empty($product) && array_key_exists($product->product_id, $options)) {
        if ($multiple) {
          $default_product_ids[] = $product->product_id;
          $default_quantities[$product->product_id] = 1;
        }
        else {
          $default_product_ids = $product->product_id;
        }
      }
    }
  }

  if ($disabled) {
    // The product has been paid for so should not be able to change it.
    $element[] = array(
      '#type' => 'value',
      '#value' => $value,
    );

    $markup = array('#markup' => '<p>' . t('It is no longer possible to edit the products in this submission.') . '</p>');
    $markup['#markup'] .= theme_webform_display_productfield(array('element' => $element));

    $element[] = $markup;
    $element['#title_display'] = 'before';
  }
  elseif (!$multiple || !$choose_quantity) {
    // Single quantity options.
    $new_element = array(
      '#type' => $aslist ? 'select' : ($multiple ? 'checkboxes' : 'radios'),
      '#multiple' => $multiple,
      '#size' => $aslist && $multiple ? 4 : 0,
      '#title' => $name,
      '#title_display' => 'none',
      '#required' => $required,
      '#description' => $description,
      '#translatable' => array('title', 'description', 'options'),
      '#options' => $options,
      '#suffix' => $hidden_elements_html,
      '#pre_render' => array(),
      '#validated' => 'TRUE',
      '#element_validate' => array('_webform_productfield_selection_validate'),
    );

    if (!empty($default_product_ids)) {
      $new_element['#default_value'] = $default_product_ids;
    }

    $element[] = $new_element;

    if (!empty($choose_quantity) && !empty($element['#webform_component']['extra']['choose_quantity_max'])) {
      $quantity_options = array();
      foreach (range($element['#webform_component']['extra']['choose_quantity_min'], $element['#webform_component']['extra']['choose_quantity_max']) as $quantity_option) {
        $quantity_options[$quantity_option] = $quantity_option;
      }
      $element[] = array(
        '#type' => 'select',
        '#options' => $quantity_options,
        '#title' => t('@product quantity', array('@product' => $name)),
        '#title_display' => 'after',
        '#default_value' => is_array($default_quantities) ? $element['#webform_component']['extra']['choose_quantity_min'] : $default_quantities,
        '#element_validate' => array('_webform_productfield_quantity_validate'),
        '#weight' => 1,
        '#attributes' => array('class' => array('productfield-quantity')),
      );
    }
    else {
      $editable = empty($multiple) && !empty($choose_quantity);
      $element[] = array(
        '#type' => $editable ? 'textfield' : 'value',
        '#title' => t('@product quantity', array('@product' => $name)),
        '#default_value' => $choose_quantity ? (is_array($default_quantities) ? 1 : $default_quantities) : 1,
        '#element_validate' => array('_webform_productfield_quantity_validate'),
        '#required' => $required,
        '#weight' => 1,
        '#attributes' => array('class' => array('productfield-quantity')),
      );
    }
  }
  else {
    // Product field is multiple and user can choose quantity.
    $i = 1;
    $element['multiple_product_quantities'] = array(
      '#type' => 'fieldset',
      '#title' => $name,
      '#title_display' => $title_display,
      '#element_validate' => array('_webform_productfield_required_multiple_quantities_validate'),
      '#required' => $required,
      '#weight' => 0,
      // Hide title as fieldsets don't support #title_display.
      '#pre_render' => array('webform_element_title_display'),
    );

    foreach ($options as $product_id => $product_name) {
      if (empty($element['#webform_component']['extra']['choose_quantity_max'])) {
        $element['multiple_product_quantities'][$product_id] = array(
          '#type' => 'textfield',
          '#title' => $product_name,
          '#title_display' => 'after',
          '#size' => 6,
          '#default_value' => isset($default_quantities[$product_id]) ? $default_quantities[$product_id] : 0,
          '#element_validate' => array('_webform_productfield_quantity_validate'),
          '#weight' => $i++,
        );
      }
      else {
        $quantity_options = array();
        foreach (range($element['#webform_component']['extra']['choose_quantity_min'], $element['#webform_component']['extra']['choose_quantity_max']) as $quantity_option) {
          $quantity_options[$quantity_option] = $quantity_option;
        }
        $element['multiple_product_quantities'][$product_id] = array(
          '#type' => 'select',
          '#options' => $quantity_options,
          '#title' => $product_name,
          '#title_display' => 'after',
          '#default_value' => isset($default_quantities[$product_id]) ? $default_quantities[$product_id] : 0,
          '#element_validate' => array('_webform_productfield_quantity_validate'),
          '#weight' => $i++,
          '#attributes' => array('class' => array('productfield-quantity')),
        );
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
  $keep_previous_submission = FALSE;

  if (is_array($value)) {
    // First check if we are dealing with a modified submission or if
    // this is a resubmission, in which case we leave products unmodified.
    foreach ($value as $product) {
      if (!is_string($product) || is_numeric($product)) {
        $keep_previous_submission = FALSE;
        break;
      }
      else {
        $keep_previous_submission = TRUE;
      }
    }
  }

  if ($keep_previous_submission) {
    // $value is an array of previously saved products.
    $return = $value;
  }
  elseif ($multiple && $choose_quantity) {
    foreach ($value['multiple_product_quantities'] as $product_id => $quantity) {
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
        if (!empty($product_id)) {
          $details = array(
            'product_id' => $product_id,
            'quantity' => 1,
            'order_id' => FALSE,
            'line_item_id' => FALSE,
            'paid' => FALSE,
          );

          $return[] = json_encode($details);
        }
        else {
          $return[] = 0;
        }
      }
    }
    elseif (!empty($value[0]) && is_numeric($value[0])) {
      // $value[0] is a product id.
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
  $options = _webform_productfield_generate_options_list_for_component($element['#webform_component']);

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

  $element['#format'] = empty($element['#format']) ? 'html' : $element['#format'];

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
      $paid_display_option = !empty($submission->paid) ? t('Paid') : t('Unpaid');
      if (isset($rows[$submission->product_id . '_' . $paid_display_option])) {
        $rows[$submission->product_id . '_' . $paid_display_option][1]++;
      }
      else {
        $rows[$submission->product_id . '_' . $paid_display_option] = array(
          $display_option . ' (' . _webform_filter_xss($paid_display_option) . ')',
          1
        );
      }
    }
  }

  return array('table_rows' => $rows);
}

/**
 * Implements _webform_table_component().
 */
function _webform_table_productfield($component, $value) {
  // Convert submitted 'safe' values to un-edited, original form.
  $products = _productfield_products($component);

  $value = (array) $value;
  $items = array();
  // Set the value as a single string.
  foreach ($value as $option_value) {
    $option_value = json_decode($option_value);
    if (!empty($option_value->product_id)) {
      if (isset($products[$option_value->product_id])) {
        $item = $option_value->quantity . ' x ' . _webform_filter_xss($products[$option_value->product_id]->title);
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
  $options = _productfield_products($component);

  $headers = array(
    0 => array(),
    1 => array($component['name']),
    2 => array(),
  );


  foreach ($options as $product_id => $product) {
    $headers[2][] = $product->sku . ': PAID';
    $headers[2][] = $product->sku . ': UNPAID';
  }

  return $headers;
}

/**
 * Implements _webform_csv_data_component().
 */
function _webform_csv_data_productfield($component, $export_options, $values) {
  $values = is_array($values) ? $values : array();

  $return = array();

  $products = _productfield_products($component);

  foreach ($values as $id => $value) {
    $values[$id] = json_decode($value);
  }

  foreach ($products as $product_id => $product) {
    $index = FALSE;
    foreach ($values as $value) {
      if ($value->product_id == $product_id) {
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

    $value = $form_state['values'];
    foreach ($element['#parents'] as $parent) {
      $value = $value[$parent];
    }

    foreach ($value as $product_id => $quantity) {
      if ($quantity > 0) {
        // At least one element has a quantity set.
        return;
      }
    }
    $name = implode('][', $element['#parents']);
    form_set_error($name, t('You must choose at least one product from this selection by setting its quantity to something greater than 1'));
  }
}

/**
 * Theme of the product selection on the webform.
 */
function theme_commerce_webform_product_display($variables) {
  $product = $variables['product'];
  $price = $variables['price'];

  $hide = empty($variables['element']['#webform_component']['extra']['hide_price'])
    ? 'show'
    : $variables['element']['#webform_component']['extra']['hide_price'];

  switch ($hide) {
    case 'hide':
      $display = filter_xss($product->title);
      break;

    case 'zero':
      $display = filter_xss($product->title);

      if (!empty($price['amount'])) {
        $display .= ' [' . commerce_currency_format($price['amount'], $price['currency_code'], $product) . ']';
      }
      break;

    default:
      $display = filter_xss($product->title) . ' [' . commerce_currency_format($price['amount'], $price['currency_code'], $product) . ']';
      break;
  }

  return $display;
}

/**
 * Get a list of all product ids of a given type(s).
 *
 * @param string|array $product_type
 *   A product_type string or an array of product type strings
 *
 * @return array
 *   An array of product ids.
 */
function _webform_productfield_product_ids_from_type($product_type) {
  $products_ids = array();

  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_product')
    ->entityCondition('bundle', $product_type)
    ->execute();

  if (!empty($result['commerce_product'])) {
    $product_ids = array_keys($result['commerce_product']);
  }

  return $product_ids;
}

/**
 * Get a list of all the product_ids available to choose from on a productfield.
 *
 * @param array $component
 *   A webform productfield component
 *
 * @return array
 *   An array of product ids. Note that if a product type is
 *   specified only, this will return an empty array.
 */
function _webform_productfield_product_ids($component) {
  $items = isset($component['extra']['items']) ? $component['extra']['items'] : array();
  $product_type = isset($component['extra']['product_type']) ? $component['extra']['product_type'] : '';

  $product_ids = array();

  // If a product type is set, load all the products for
  // that type.
  if (!empty($product_type)) {
    $product_ids = _webform_productfield_product_ids_from_type($product_type);
  }
  else {
    foreach ($items as $item) {
      $product_ids[] = $item['product_id'];
    }
  }

  return $product_ids;
}

/**
 * @param array $component
 *   A webform productfield component
 *
 * @return array
 *   Keys are product ids, values are the names of the products.
 */
function _webform_productfield_generate_options_list_for_component($component) {
  $options = array();
  $product_ids = _webform_productfield_product_ids($component);
  $products = commerce_product_load_multiple($product_ids);

  foreach ($products as $id => $product) {
    $options[$product->product_id] = $product->title;
  }

  return $options;
}

/**
 * Define conditional operators for productfields.
 */
function _commerce_webform_conditional_operator_info() {
  $operators = array();

  $operators['productfield']['product_is'] = array(
    'label' => t('is a product'),
    'comparison callback' => 'webform_conditional_operator_product_is',
    'js comparison callback' => 'conditionalOperatorProductEqual',
    'form callback' => 'commerce_webform_conditional_product_select',
  );

  $operators['productfield']['product_is_not'] = array(
    'label' => t('is not a product'),
    'comparison callback' => 'webform_conditional_operator_product_is_not',
    'js comparison callback' => 'conditionalOperatorProductNotEqual',
    'form callback' => 'commerce_webform_conditional_product_select',
  );

  $operators['productfield']['product_is_of_type'] = array(
    'label' => t('is a product of type'),
    'comparison callback' => 'webform_conditional_operator_product_is_of_type',
    'js comparison callback' => 'conditionalOperatorProductOfType',
    'form callback' => 'commerce_webform_conditional_product_type_select',
    'comparison prepare js' => 'webform_conditional_prepare_productfield_type_js',
  );

  $operators['productfield']['product_is_not_of_type'] = array(
    'label' => t('is not a product of type'),
    'comparison callback' => 'webform_conditional_operator_product_is_not_of_type',
    'js comparison callback' => 'conditionalOperatorProductNotOfType',
    'form callback' => 'commerce_webform_conditional_product_type_select',
    'comparison prepare js' => 'webform_conditional_prepare_productfield_type_js',
  );

  $operators['productfield']['quantity_equals'] = array(
    'label' => t('total quantity is equal to'),
    'comparison callback' => 'webform_conditional_operator_product_quantity_equals',
    'js comparison callback' => 'conditionalOperatorProductQuantityEquals',
  );

  $operators['productfield']['quantity_less_than'] = array(
    'label' => t('total quantity is less than'),
    'comparison callback' => 'webform_conditional_operator_product_quantity_less_than',
    'js comparison callback' => 'conditionalOperatorProductQuantityLessThan',
  );

  $operators['productfield']['quantity_greater_than'] = array(
    'label' => t('total quantity is greater than'),
    'comparison callback' => 'webform_conditional_operator_product_quantity_greater_than',
    'js comparison callback' => 'conditionalOperatorProductQuantityGreaterThan',
  );

  return $operators;
}

/**
 * Prepare a conditional value for adding as a JavaScript setting.
 *
 * @param string $rule_value
 *   The rule value will be a product type (bundle).
 *
 * @return array
 *   An array of product ids which can be set on the productfield
 *   for this rule to trigger.
 */
function webform_conditional_prepare_productfield_type_js($rule_value) {
  return _webform_productfield_product_ids_from_type($rule_value);
}

/**
 * Webform conditional form callback.
 */
function commerce_webform_conditional_product_select($node) {
  $forms = array();
  webform_component_include('productfield');
  foreach ($node->webform['components'] as $cid => $component) {
    if (webform_component_property($component['type'], 'conditional_type') == 'productfield') {
      $options = _webform_productfield_generate_options_list_for_component($component);

      $element = array(
        '#type' => 'select',
        '#multiple' => FALSE,
        '#size' => NULL,
        '#attributes' => array(),
        '#id' => NULL,
        '#name' => NULL,
        '#options' => $options,
        '#parents' => array(),
      );

      $forms[$cid] = drupal_render($element);
    }
  }
  return $forms;
}

/**
 * Webform conditional form callback.
 */
function commerce_webform_conditional_product_type_select($node) {
  $forms = array();
  webform_component_include('productfield');
  foreach ($node->webform['components'] as $cid => $component) {
    if (webform_component_property($component['type'], 'conditional_type') == 'productfield') {
      $options = commerce_product_type_options_list();

      $element = array(
        '#type' => 'select',
        '#multiple' => FALSE,
        '#size' => NULL,
        '#attributes' => array(),
        '#id' => NULL,
        '#name' => NULL,
        '#options' => $options,
        '#parents' => array(),
      );

      $forms[$cid] = drupal_render($element);
    }
  }
  return $forms;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_is($input_values, $rule_value) {
  $selected_products = _commerce_webform_get_selected_products_from_input_values($input_values);

  foreach ($selected_products as $product_id => $quantity) {
    if ($quantity > 0 && $product_id == $rule_value) {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_is_not($input_values, $rule_value) {
  return !webform_conditional_operator_product_is($input_values, $rule_value);
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_quantity_equals($input_values, $rule_value) {
  return _commerce_webform_get_total_quantity_from_input_values($input_values) == $rule_value;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_quantity_less_than($input_values, $rule_value) {
  return _commerce_webform_get_total_quantity_from_input_values($input_values) < $rule_value;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_quantity_greater_than($input_values, $rule_value) {
  return _commerce_webform_get_total_quantity_from_input_values($input_values) > $rule_value;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_is_of_type($input_values, $rule_value) {
  $selected_products = _commerce_webform_get_selected_products_from_input_values($input_values);

  foreach ($selected_products as $product_id => $quantity) {
    if ($quantity > 0) {
      $product = commerce_product_load($product_id);
      if ($product->type == $rule_value) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 * Webform conditionals comparison callback for product fields.
 */
function webform_conditional_operator_product_is_not_of_type($input_values, $rule_value) {
  return !webform_conditional_operator_product_is_of_type($input_values, $rule_value);
}

/**
 * Helper function for conditional callbacks.
 *
 * The input value format to a conditional callback changes depending
 * on the configuration of the component. This function normalises
 * the input to an array of selected product ids and quantities.
 *
 * @param array $input_values
 *   This is either:
 *    1. an array with one element which is an array of product ids
 *    2. an array where each value is a json blob object with the following properties:
 *       product_id - int id
 *       quantity   - int
 *       order_id   - FALSE
 *       This occurs after web form submission when the data has been turned into
 *       a JSON blob.
 *    3. an array of 2 elements where the first element is the product id and the second is the quantity
 *    4. an array with a single element with key 'multiple_product_quantities'
 *       whose values are an array with keys as the product ids and values are
 *       the quantities
 *
 *   This table describes the web form component options and which of the
 *   values structures is used :-
 *     single | no quantity | 1 product | radio - 3
 *     single | no quantity | 2 product | radio - 3
 *     single | quantity | 1 product | radio - 3
 *     single | quantity | 2 products | radio - 3
 *     multiple | no quantity | 1 product | radio - 1
 *     multiple | no quantity | 2 products | radio - 1
 *     multiple | quantity    | 1 product | radio - 5
 *     multiple | quantity    | 2 product | radio - 5
 *     single | no quantity | 1 product | list - 3
 *     single | no quantity | 2 products | list - 3
 *     single | quantity | 1 product | list - 3
 *     single | quantity | 2 products | list - 3
 *     multiple | no quantity | 1 product | list - 1
 *     multiple | no quantity | 2 products | list - 1
 *     multiple | quantity    | 1 product | list - 5
 *     multiple | quantity    | 2 product | list - 5
 *
 * @return array
 *   Keys are product ids, values are quantities.
 */
function _commerce_webform_get_selected_products_from_input_values(array $input_values = array()) {
  if (empty($input_values)) {
    return array();
  }

  if (!empty($input_values['multiple_product_quantities'])) {
    // Situation 4 - product ids and quantities are stored in 'multiple_product_quantities'.
    return $input_values['multiple_product_quantities'];
  }

  $selected_products = array();

  if (is_array($input_values[0])) {
    // Situation 1 - singular list of product ids stored in array element 0.
    foreach ($input_values[0] as $product_id) {
      if ($product_id > 0) {
        $selected_products[$product_id] = 1;
      }
    }
  }
  elseif (!empty($input_values[0]) && !is_numeric($input_values[0]) && !is_null(json_decode($input_values[0]))) {
    // Situation 2 - each element of input values is a json encoded string.
    foreach ($input_values as $json_string) {
      $json_object = json_decode($json_string);
      $selected_products[$json_object->product_id] = $json_object->quantity;
    }
  }
  else {
    // Situation 3 - element 0 is the product id, element 1 is the quantity.
    $selected_products[$input_values[0]] = $input_values[1];
  }

  return $selected_products;
}

/**
 * Get the total quantity selected from input values.
 */
function _commerce_webform_get_total_quantity_from_input_values($input_values) {
  $selected_products = _commerce_webform_get_selected_products_from_input_values($input_values);

  $total = 0;
  foreach ($selected_products as $product_id => $quantity) {
    $total += $quantity;
  }

  return $total;
}

/**
 * Get a list of commerce products avaiable for selection.
 *
 * @param array $component
 *   Webform productfield component
 *
 * @return array
 *   An array of commerce_product objects available for
 *   selection with this component.
 */
function _productfield_products($component) {
  $items = isset($component['extra']['items']) ? $component['extra']['items'] : array();
  $product_type = $component['extra']['product_type'];

  $product_ids = array();

  if (empty($product_type)) {
    // Build an array of product IDs from this field's values.
    foreach ($items as $item) {
      $product_ids[] = $item['product_id'];
    }
  }
  else {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'commerce_product')
      ->entityCondition('bundle', $product_type)
      ->execute();

    if (!empty($result['commerce_product'])) {
      $product_ids = array_keys($result['commerce_product']);
    }
  }

  return commerce_product_load_multiple($product_ids);
}
