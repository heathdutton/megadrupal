<?php

/**
 * @file
 * Provides Rules module support for extra discounts.
 */

/**
 * Implements hook_rules_condition_info_alter().
 */
function commerce_discount_extra_rules_condition_info() {
  $inline_conditions = inline_conditions_get_info();

  // Line item: user has role.
  $conditions['commerce_discount_extra_line_item_user_has_role'] = array(
    'label' => t('User has role'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'wrapped' => TRUE,
      ),
      'roles' => array(
        'type' => 'list<text>',
        'label' => t('Roles'),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  // Order: user has role.
  $conditions['commerce_discount_extra_order_user_has_role'] = array(
    'label' => t('User has role'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'wrapped' => TRUE,
      ),
      'roles' => array(
        'type' => 'list<text>',
        'label' => t('Roles'),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  // Line item: line item price comparison.
  $conditions['commerce_discount_extra_line_item_price_comp'] = array(
    'label' => t('Line item price comparison'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE,
      ),
      'price' => array(
        'type' => 'commerce_price',
        'label' => t('Price'),
        'description' => t('User-defined price for comparison against product'),
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'default value' => '>=',
        'options list' => '_commerce_discount_extra_operator_options',
      ),
      'method' => array(
        'type' => 'text',
        'label' => t('Method'),
        'default value' => 'base',
        'options list' => '_commerce_discount_extra_comp_method_options',
      ),
      'negate' => array(
        'type' => 'boolean',
        'label' => t('Negate expression'),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  // Line item: total items in order.
  $conditions['commerce_discount_extra_line_item_items_in_order'] = array(
    'label' => t('Total items in order'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'wrapped' => TRUE,
      ),
      'number' => array(
        'type' => 'integer',
        'label' => t('Number of items in order'),
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'default value' => '>=',
        'options list' => '_commerce_discount_extra_operator_options',
      ),
      'negate' => array(
        'type' => 'boolean',
        'label' => t('Negate expression'),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  // Line item: order has quantity product.
  $conditions['commerce_discount_extra_line_item_has_specific_quantity_products'] = array(
    'label' => t('Order has specific products'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE,
      ),
      'products' => array(
        'type' => 'text',
        'label' => t('Product SKU(s)'),
        'description' => t('Products SKU to look for on the order. Enter a comma-separated list of product SKU(s).'),
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The operator used with the product quantity value below.'),
        'default value' => '>=',
        'options list' => '_inline_conditions_order_operator_options',
      ),
      'quantity' => array(
        'type' => 'integer',
        'label' => t('Quantity'),
        'description' => t('Quantity value to be compared against each selected product(s).'),
      ),
    ),
  );

  // Order: total items in order.
  $conditions['commerce_discount_extra_order_items_in_order'] = array(
    'label' => t('Total items in order'),
    'parameter' => array(
      'entity' => array(
        'type' => 'entity',
        'label' => t('Entity'),
        'wrapped' => TRUE,
      ),
      'number' => array(
        'type' => 'integer',
        'label' => t('Number of items in order'),
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'default value' => '>=',
        'options list' => '_commerce_discount_extra_operator_options',
      ),
      'negate' => array(
        'type' => 'boolean',
        'label' => t('Negate expression'),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  $conditions['commerce_discount_extra_line_item_compare_order_amount'] = array(
    'label' => t('Order amount comparison'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'description' => t('The line item.'),
        'wrapped' => TRUE,
      ),
      'operator' => array(
        'type' => 'text',
        'label' => t('Operator'),
        'description' => t('The operator used with the order amount value below.'),
        'default value' => '>=',
        'options list' => '_inline_conditions_order_operator_options',
      ),
      'total' => array(
        'type' => 'commerce_price',
        'label' => t('Order amount'),
        'default value' => '',
        'description' => t("The value to compare against the passed line item's order amount."),
      ),
    ),
    'group' => t('Commerce discounts extra'),
  );

  $conditions['commerce_discount_extra_line_item_has_product_type'] = array(
    'label' => t('Product types'),
    'parameter' => array(
      'commerce_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
        'wrapped' => TRUE,
      ),
      'product_types' => array(
        'type' => 'list<text>',
        'label' => t('Product types'),
        'options list' => 'commerce_product_type_options_list',
        'multiple' => TRUE,
      ),
    ),
  );

  foreach ($conditions as $name => $condition) {
    $conditions[$name]['callbacks'] = array(
      'execute' => $inline_conditions[$name]['callbacks']['build'],
    );
  }

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_discount_extra_rules_action_info() {
  $actions['commerce_discount_extra_per_quantity_discount'] = array(
    'label' => t('Discount a certain number of products in an order'),
    'group' => t('Commerce discounts extra'),
    'parameter' => array(
      'commerce_order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
        'wrapped' => TRUE,
      ),
      'commerce_discount' => array(
        'label' => t('Commerce Discount'),
        'type' => 'token',
        'options list' => 'commerce_discount_entity_list',
      ),
    ),
  );

  return $actions;
}

/**
 * Rules action callback: apply a per quantity discount to an order.
 */
function commerce_discount_extra_per_quantity_discount(EntityDrupalWrapper $order_wrapper, $discount_name) {
  $discount_wrapper = entity_metadata_wrapper('commerce_discount', $discount_name);
  $order = $order_wrapper->value();

  if (!$discount_wrapper->commerce_discount_offer->commerce_discount_products->value()) {
    return;
  }

  $offer_wrapper = $discount_wrapper->commerce_discount_offer;
  $offer_type = $offer_wrapper->type->value();

  // Trigger product variables.
  $trigger_qty = $offer_wrapper->commerce_trigger_qty->value();
  $trigger_product_ids = $offer_wrapper->commerce_trigger_products->raw();

  // Discount product variables.
  $offer_product_ids = $offer_wrapper->commerce_discount_products->raw();
  $offer_qty_per_trigger = $offer_wrapper->commerce_offer_qty->value();

  // Determine the proper direction to use for sorting items. Offer direction
  // means the order in which eligible offer items are picked for discount.
  // Trigger direction means the order in which eligible offer-trigger items
  // ("x" in Buy x get y) are processed and thus marked as ineligible for
  // offers.
  switch ($offer_wrapper->commerce_pricing_strategy->value()) {
    case NULL:
    case 'high_first':
      $offer_direction = 'descending';
      $trigger_direction = 'ascending';
      break;

    case 'low_first':
      $offer_direction = 'ascending';
      $trigger_direction = 'descending';
  }

  // Find the maximum number of times a discount can fire based on the
  // user-defined offer limit settings.
  $limit = $offer_wrapper->commerce_offer_limit->value() * $offer_qty_per_trigger;

  // Create a manifest of products at each quantity position.
  foreach ($order_wrapper->commerce_line_items as $line_item_wrapper) {
    $type = $line_item_wrapper->type->value();
    // Do not count free product line items.
    if ($line_item_wrapper->value() && in_array($type, commerce_product_line_item_types()) && $type != 'product_discount') {
      $product_id = $line_item_wrapper->commerce_product->product_id->value();
      if (in_array($product_id, $trigger_product_ids) || in_array($product_id, $offer_product_ids)) {
        $pos = 0;
        while ($pos < $line_item_wrapper->quantity->value()) {
          // Only count trigger and offer products. Some will happen to be both.
          if (in_array($product_id, $trigger_product_ids) || in_array($product_id, $offer_product_ids)) {
            $manifest[] = $line_item_wrapper;
            $pos++;
          }
        }
      }
    }
  }

  // Counter for discount amount.
  $amount = 0;
  // Counter for number of times the discount has fired.
  $n = 0;

  // Each loop takes care of one phase of "buy x get y". The loop repeats
  // until all of the offer/trigger products have been counted.
  while (!empty($manifest)) {
    // Counter for trigger products per phase.
    $t = 0;
    // Counter for offer products per phase.
    $o = 0;

    // First try to find a trigger product that is not an offer product.
    foreach ($manifest as $i => $line_item_wrapper) {
      $product_id = $line_item_wrapper->commerce_product->product_id->value();
      if ($t < $trigger_qty && in_array($product_id, $trigger_product_ids) && !in_array($product_id, $offer_product_ids)) {
        $t++;
        unset($manifest[$i]);
      }
    }

    // If there are still not enough trigger products, look for any trigger
    // product, going in the order specified by the offer config.
    usort($manifest, 'commerce_discount_extra_sort_by_amount_' . $trigger_direction);
    foreach ($manifest as $i => $line_item_wrapper) {
      $product_id = $line_item_wrapper->commerce_product->product_id->value();
      if ($t < $trigger_qty && in_array($product_id, $trigger_product_ids)) {
        $t++;
        unset($manifest[$i]);
      }
    }

    // If there are enough trigger products, look for some products to which we
    // can apply the offer.
    if ($t == $trigger_qty) {
      // Sort eligible offer products so that discounts are applied in the
      // correct order.
      usort($manifest, 'commerce_discount_extra_sort_by_amount_' . $offer_direction);
      foreach ($manifest as $i => $line_item_wrapper) {
        $product_id = $line_item_wrapper->commerce_product->product_id->value();
        if ($o < $offer_qty_per_trigger && in_array($product_id, $offer_product_ids)) {
          $o++;
          unset($manifest[$i]);

          // Apply offer.
          if ($n < $limit || !$limit) {
            switch ($offer_type) {
              case 'per_quantity_fixed':
                $amount += $offer_wrapper->commerce_fixed_amount->amount->value();

                break;

              case 'per_quantity_percentage':
                $raw_price = $line_item_wrapper->commerce_unit_price->amount->value();
                $pct = $offer_wrapper->commerce_percentage->value();
                $amount += $raw_price * $pct;

                break;
            }
            $n++;
          }
        }
      }
    }
    // At this point if we have found no trigger products, break out of the
    // loop.
    else {
      break;
    }
  }

  if ($amount > 0) {
    $delta = $order_wrapper->commerce_discounts->count();
    $order->commerce_discounts[LANGUAGE_NONE][$delta]['target_id'] = $discount_wrapper->discount_id->value();

    // Add a discount line item.
    $discount_price = array(
      'amount' => -$amount,
      'currency_code' => $order_wrapper->commerce_order_total->currency_code->value(),
    );

    module_load_include('inc', 'commerce_discount', 'commerce_discount.rules');

    // Add the line item is it's not attached on order yet.
    if (!commerce_discount_set_existing_line_item_price($order_wrapper, $discount_name, $discount_price)) {
      commerce_discount_add_line_item($order_wrapper, $discount_name, $discount_price);
    }
  }
}

/**
 * Usort callback: sort line items by amount ascending.
 */
function commerce_discount_extra_sort_by_amount_ascending($a, $b) {
  $a_amount = $a->commerce_unit_price->amount->value();
  $b_amount = $b->commerce_unit_price->amount->value();
  if ($a_amount == $b_amount) {
    return 0;
  }
  return ($a_amount < $b_amount) ? -1 : 1;
}

/**
 * Usort callback: sort line items by amount descending.
 */
function commerce_discount_extra_sort_by_amount_descending($a, $b) {
  $a_amount = $a->commerce_unit_price->amount->value();
  $b_amount = $b->commerce_unit_price->amount->value();
  if ($a_amount == $b_amount) {
    return 0;
  }
  return ($a_amount > $b_amount) ? -1 : 1;
}
