<?php
/**
 * @file
 * Checkout pane callback functions for the cart module.
 */

/**
 * Checkout pane callback: returns the cart contents pane's settings form.
 */
function commerce_checkout_products_list_settings_form($checkout_pane) {
  $form = array();

  // Build an options array of Views available for the cart contents pane.
  $options = array();

  // Generate an option list from all user defined and module defined Views,
  // excluding the core Commerce cart block / form and order listing Views.
  $exclude = array('commerce_cart_block', 'commerce_cart_form', 'commerce_orders', 'commerce_user_orders');
  $default = variable_get('commerce_checkout_products_list_view', 'commerce_products|default');

  foreach (views_get_all_views() as $view_id => $view_value) {
    // Only include line item Views, including a View that may be excluded but
    // has already been set to be the selected View some other way. The list of
    // excluded Views was added in as of Commerce 1.5, so we want to preserve
    // existing selections much like we do for Price fields with currency select
    // lists whose currency may have been disabled since the price was entered.
    if ($view_value->base_table == 'commerce_product') {
      foreach ($view_value->display as $display_id => $display_value) {
        $key = $view_id . '|' . $display_id;

        if (!in_array($view_id, $exclude) || $key == $default) {
          $options[$view_id][$view_id . '|' . $display_id] = $display_value->display_title;
        }
      }
    }
  }

  $form['commerce_checkout_products_list_order_only'] = array(
    '#type' => 'checkbox',
    '#title' => t('Products in commerce order'),
    '#description' => t('Display the products in the commerce order for this pane.'),
    '#default_value' => variable_get('commerce_checkout_products_list_order_only', FALSE),
  );

  $form['commerce_checkout_products_list_view'] = array(
    '#type' => 'select',
    '#title' => t('Products list View'),
    '#description' => t('Specify the View to use to generate the products list for this pane.'),
    '#options' => $options,
    '#default_value' => $default,
    '#states' => array(
      'visible' => array(
        'input[name="commerce_checkout_products_list_order_only"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['commerce_checkout_products_list_price'] = array(
    '#type' => 'radios',
    '#title' => t('Use Rules when displaying item prices'),
    '#description' => t('If Commerce Discount is installed it can affect price display and lead to confusion over what is shown. Not triggering the Rules can avoid the price display from changing, and causing some confusion for visitors. In specific terms, the "commerce_product_calculate_sell_price" rule will not be triggered. Note: this does not prevent rules and discounts from applying to the items in the actual cart, it only affects the price shown on the form.'),
    '#options' => array(
      'rules' => t('Use Rules'),
      'no_rules' => t("Don't use Rules"),
    ),
    '#default_value' => variable_get('commerce_checkout_products_list_price', 'rules'),
  );

  return $form;
}

/**
 * Checkout pane callback: returns a customer profile edit form.
 */
function commerce_checkout_products_list_checkout_form($form, &$form_state, &$checkout_pane, $order) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_checkout_products_list') . '/commerce_checkout_products_list.checkout_pane.inc';
  
  global $user;

  // Load the current shopping cart order.
  $order = commerce_cart_order_load($user->uid);
  $quantities = array();
  $prices = array();
  if (!empty($order->commerce_line_items[LANGUAGE_NONE])) {
    foreach ($order->commerce_line_items[LANGUAGE_NONE] as $item) {
      // Load the line item.
      $line_item = commerce_line_item_load($item['line_item_id']);

      // Only work with products.
      if ($line_item->type != 'product') {
        continue;
      }

      // Get the product ID.
      $product_id = $line_item->commerce_product[LANGUAGE_NONE][0]['product_id'];

      // Work out how many of each item is in the cart.
      $quantities[$product_id] = $line_item->quantity;
      $totals[$product_id] = $line_item->commerce_total;
      if (!empty($totals[$product_id][LANGUAGE_NONE][0]['amount'])) {
        $totals[$product_id] = $totals[$product_id][LANGUAGE_NONE][0]['amount'];
      }
      else {
        $totals[$product_id] = 0;
      }
    }
  }

  $products = array();

  // Get a list of the products which are part of the current order.
  if (variable_get('commerce_checkout_products_list_order_only', FALSE)) {
    foreach (array_keys($totals) as $product_id) {
      $products[] = commerce_product_load($product_id);
    }
  }

  // Get a list of all products from a view.
  else {
    $view_default = variable_get('commerce_checkout_products_list_view', 'commerce_products|default');
    list($view_name, $view_display_id) = explode('|', $view_default);
    foreach (views_get_view_result($view_name, $view_display_id) as $item) {
      $products[] = commerce_product_load($item->product_id);
    }
  }

  // Build a table of the products.
  $items = array();
  foreach ($products as $product) {
    $quantity = 0;
    if (isset($quantities[$product->product_id])) {
      $quantity = $quantities[$product->product_id];
    }
    // Update the quantity from the form submission; only happens during a form
    // validation.
    if (isset($form_state['input']['product_' . $product->product_id])) {
      $new_quantity = intval($form_state['input']['product_' . $product->product_id]);
      if ($new_quantity >= 0) {
        $quantity = $new_quantity;
      }
    }
    $total = 0;
    if (isset($totals[$product->product_id])) {
      $total = $totals[$product->product_id];
    }
    $total = number_format($total / 100, 2);

    // Load the price.
    if (variable_get('commerce_checkout_products_list_price', 'rules') == 'rules') {
      $price = commerce_product_calculate_sell_price($product);
    }
    else {
      $price = _commerce_checkout_products_list_sell_price($product);
    }
    if (!empty($price['amount'])) {
      $price = $price['amount'];
    }
    else {
      $price = 0;
    }

    $items[] = array(
      'sku' => $product->sku,
      'name' => $product->title,
      'price' => number_format($price / 100, 2),
      'quantity' => array(
        'data' => array(
          '#type' => 'textfield',
          '#value' => intval($quantity),
          '#title' => t('Quantity'),
          '#title_display' => 'invisible',
          '#size' => 3,
          '#attributes' => array(
            'name' => 'product_' . $product->product_id,
          ),
          '#field_prefix' => '<a class="decrement" data-product_id="' . $product->product_id . '">-</a>',
          '#field_suffix' => '<a class="increment" data-product_id="' . $product->product_id . '">+</a>',
        ),
      ),
      'total' => $total,
    );
  }

  // Display the products list.
  $pane_form['items'] = array(
    '#theme' => 'table',
    '#rows' => $items,
    '#header' => array(
      t('SKU'),
      t('Product'),
      t('Price'),
      t('Quantity'),
      t('Total'),
    ),
    '#attributes' => array(
      'class' => array('views-table cols-5'),
    ),
  );
  $pane_form['#attached']['js'][] = drupal_get_path('module', 'commerce_checkout_products_list') . '/commerce_checkout_products_list.js';

  // Display the current order total. Copied form
  // commerce_price_field_formatter_view().

  // Build an array of component display titles and their prices.
  $components = array();
  $weight = 0;
  $item = $order->commerce_order_total[LANGUAGE_NONE][0];
  foreach ($order->commerce_order_total[LANGUAGE_NONE][0]['data']['components'] as $key => $component) {
    $component_type = commerce_price_component_type_load($component['name']);

    if (empty($components[$component['name']])) {
      $components[$component['name']] = array(
        'title' => check_plain($component_type['display_title']),
        'price' => commerce_price_component_total($item, $component['name']),
        'weight' => $component_type['weight'],
      );

      $weight = max($weight, $component_type['weight']);
    }
  }

  // If there is only a single component and its price equals the field's,
  // then remove it and just show the actual price amount.
  if (count($components) == 1 && in_array('base_price', array_keys($components))) {
    $components = array();
  }

  // Add the actual field value to the array.
  $components['commerce_price_formatted_amount'] = array(
    'title' => t('Order total'),
    'price' => $item,
    'weight' => $weight + 1,
  );

  // Allow other modules to alter the components.
  drupal_alter('commerce_price_formatted_components', $components, $item, $order);

  // Sort the components by weight.
  uasort($components, 'drupal_sort_weight');

  // Format the prices for display.
  foreach ($components as $key => &$component) {
    $component['formatted_price'] = commerce_currency_format(
      $component['price']['amount'],
      $component['price']['currency_code'],
      $order
    );
  }

  $pane_form['total'] = array(
    '#theme' => 'commerce_price_formatted_components',
    '#components' => $components,
    '#price' => $item,
  );

  // Cheat on the theming ;-)
  $pane_form['#attributes'] = array(
    'class' => array(
      'view',
    ),
  );

  $pane_form['#validate'][] = 'commerce_checkout_products_list_checkout_form_validate';
  $pane_form['#submit'][] = 'commerce_checkout_products_list_checkout_form_submit';
  return $pane_form;
}

function commerce_checkout_products_list_checkout_form_validate($form, &$form_state, $checkout_pane, $order) {
  global $user;
  $count = 0;
  $error = '';
  $products = array();

  // Loop through all of the form 'input' values; this approach must be taken
  // because the form isn't using the Form API :(
  foreach ($form_state['input'] as $key => $value) {
    if (strpos($key, 'product_') === 0) {
      // This is a product, so start off with a quantity of zero.
      $product_id = intval(str_replace('product_', '', $key));
      $products[$product_id] = 0;

      // Check to see if a valid quantity was passed in.
      if (!empty($value)) {
        // Yay, a valid quantity!
        if (is_numeric($value) && intval($value) > 0) {
          $products[$product_id] = intval($value);
          $count += intval($value);
        }
        else {
          $error = t('The quantity may only be a number.');
        }
      }
    }
  }

  // Load the current shopping cart order.
  $order = commerce_cart_order_load($user->uid);

  // Update the cart.
  _commerce_checkout_products_list_update_cart($products, $order);

  if (!empty($error)) {
    form_set_error('', $error);
    return FALSE;
  }
  elseif ($count == 0) {
    form_set_error('', t('No items were added to the cart!'));
    return FALSE;
  }
  else {
    return TRUE;
  }
}

/**
 * FormAPI submission callback for
 * commerce_cart_form_checkout_pane_checkout_form().
 */
function commerce_checkout_products_list_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  // Identify the products that are being ordered.
  $products = array();
  foreach ($form_state['input'] as $key => $value) {
    if (is_numeric($value) && strpos($key, 'product_') === 0) {
      $product_id = intval(str_replace('product_', '', $key));
      $products[$product_id] = intval($value);
    }
  }

  // Update the cart.
  _commerce_checkout_products_list_update_cart($products, $order);
}

/**
 * Update the current cart based upon a given list of products.
 *
 * @param array $products
 *   A list of product IDs and their respective quantities.
 * @param object $order
 *   The CommerceOrder object for a given user.
 */
function _commerce_checkout_products_list_update_cart($products, $order) {
  // Identify the products already in the cart.
  $line_item_products = array();
  if (!empty($order->commerce_line_items[LANGUAGE_NONE])) {
    foreach ($order->commerce_line_items[LANGUAGE_NONE] as $item) {
      $line_item = commerce_line_item_load($item['line_item_id']);
      $product_id = $line_item->commerce_product[LANGUAGE_NONE][0]['product_id'];
      $line_item_products[$product_id] = $line_item;
    }
  }

  // Make sure there's a line item for each product being ordered.
  foreach ($products as $product_id => $quantity) {
    $product = commerce_product_load($product_id);
    if (isset($line_item_products[$product_id])) {
      $line_item = $line_item_products[$product_id];

      // If the quantity was set to zero, then delete the item.
      if ($quantity == 0) {
        commerce_line_item_delete_references($line_item);
        commerce_line_item_delete($line_item->line_item_id);
      }

      // If the quantities are different, update the line item.
      elseif ($quantity != intval($line_item->quantity)) {
        $line_item->quantity = $quantity;
        commerce_line_item_save($line_item);
      }

      // If the quantities are the same, do nothing.
      else {
        // Do nothing.
      }
    }

    // The quantity is a positive number, i.e. they would like some.
    elseif ($quantity > 0) {
      // Generate a line item record.
      $line_item = commerce_product_line_item_new($product, $quantity, $order->order_id);

      // Save the line item to get its ID.
      commerce_line_item_save($line_item);

      // Add the line item to the order.
      $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
      $order_wrapper->commerce_line_items[] = $line_item;

      // Save the order again to update its line item reference field.
      commerce_order_save($order);
    }

    // The product quantity is zero.
    else {
      // Do nothing.
    }
  }
}

/**
 * Calculate the selling price of a product.
 *
 * This is a clone of commerce_product_calculate_sell_price() with one change:
 * - The rule is not triggered.
 */
function _commerce_checkout_products_list_sell_price($product, $precalc = FALSE) {
  // First create a pseudo product line item that we will pass to Rules.
  $line_item = commerce_product_line_item_new($product);

  // Allow modules to prepare this as necessary.
  drupal_alter('commerce_product_calculate_sell_price_line_item', $line_item);

  // Attempt to fetch a database stored price if specified.
  if ($precalc) {
    $module_key = commerce_product_pre_calculation_key();

    $result = db_select('commerce_calculated_price')
      ->fields('commerce_calculated_price', array('amount', 'currency_code', 'data'))
      ->condition('module', 'commerce_product_pricing')
      ->condition('module_key', $module_key)
      ->condition('entity_type', 'commerce_product')
      ->condition('entity_id', $product->product_id)
      ->condition('field_name', 'commerce_price')
      ->execute()
      ->fetchObject();

    // If a pre-calculated price was found...
    if (!empty($result)) {
      // Wrap the line item, swap in the price, and return it.
      $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

      $wrapper->commerce_unit_price->amount = $result->amount;
      $wrapper->commerce_unit_price->currency_code = $result->currency_code;

      // Unserialize the saved prices data array and initialize to an empty
      // array if the column was empty.
      $result->data = unserialize($result->data);
      $wrapper->commerce_unit_price->data = !empty($result->data) ? $result->data : array();

      return $wrapper->commerce_unit_price->value();
    }
  }

  // Pass the line item to Rules.
  // rules_invoke_event('commerce_product_calculate_sell_price', $line_item);

  return entity_metadata_wrapper('commerce_line_item', $line_item)->commerce_unit_price->value();
}
