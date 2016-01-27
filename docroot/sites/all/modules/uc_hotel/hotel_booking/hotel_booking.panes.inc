<?php
/*
 * @file hotel_booking.panes.inc
 * Provides cart, checkout and order panes for Ubercart
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands (larowlan) leerowlands at rowlands-bcs dot com
 * @author Will Vincent (tcindie) tcindie at gmail dot com
 * @todo - this whole file needs some love
 *
 */


/**
 * Callbacks for order pane
 * @param $op string op called with hook_order_pane etc
 * @param $order object uc_order object
 */
function hotel_booking_booking_pane($op, $order) {
  $hotel_booking_items_exist = FALSE;

  switch ($op) {
    case 'view':
    case 'customer':
      if (is_array($order->products)) {
        foreach ($order->products as $product) {
          if ($product->data['module'] == 'hotel_booking') {
            //this is a hotel booking product
            $hotel_booking_items_exist = TRUE;
          }
        }
      }
      if ($hotel_booking_items_exist) {
        return (drupal_get_form('hotel_booking_details_pane_order', $order->products, isset($order->as_template) ? $order->as_template: FALSE));
      }
      break;
  }
}

/**
 * Provides booking rewards pane,
 * param $op string op called with hook_order_pane etc
 * @param $order object uc_order object
*/
function hotel_booking_pane_rewards($op, $order) {
  switch ($op) {
    case 'view':
    case 'customer':
      if (!empty($order->reward_membership)) {
        $reward_number = $order->reward_membership;
        return t('Reward Membership Number: @reward', array('@reward' => $reward_number));
      }
      break;
  }
}



/**
 * Functions to build fieldsets & tables for order pane
 * @param $form_state array std drupal FAPI form state var
 * @param $products array array of products as per $order->products
 * @param $as_template bool whether pane is being displayed in a template (ie printed invoice, emailed)
 *
 * @todo NOTE THAT travel_supplier module references this form
 */
function hotel_booking_details_pane_order($form, $form_state, $products, $as_template) {
  foreach ($products as $product) {
    if ($product->data['module'] == 'hotel_booking') {
      $nights_count[$product->order_product_id] = count($product->data['nights']);
    }
  }

  foreach ($products as $product) {
    if ($product->data['module'] == 'hotel_booking') {
      $title = check_plain($product->title)
                .' ('. format_plural(count($product->data['nights']), '1 Night, ', '@count Nights, ')
                . format_plural($product->data['adults'], '1 Adult', '@count Adults')
                . format_plural($product->data['children'], ', 1 Child', ', @count Children')
                .')';
      $node = node_load($product->nid);

      $contents['booking_details'][$product->order_product_id] = array(
        '#type' => 'fieldset',
        '#title' => $title,
        '#description' => theme('hotel_booking_pane_help', array('product' => $product, 'node' => $node)),
        '#collapsible' => TRUE,
        // Default to collapsed if there are multiple room booking items in the cart,
        // or the booking has more than three nights and more than two add_ons, or if
        // a single booking item has more than 5 nights.
        '#collapsed' => (count($nights_count) > 1 ||
                         (count($product->data['nights']) > 3 && count($product->data['booking_upgrades']) > 2) ||
                         (count($product->data['nights']) > 5)),
      );
      list($details, $additional) = hotel_booking_order_details_table($product, $products, FALSE, $as_template);
      $contents['booking_details'][$product->order_product_id]['nid'] = array(
        '#markup' => $details,
      );
      if ($additional) {
        $contents['booking_details'][$product->order_product_id]['nid']['additional'] = array(
          '#markup' => hotel_booking_order_details_table($product, $products, TRUE, $as_template),
        );
      }
    }
  }
  return array('contents' => $contents);
}

/**
 * Utility, provides a table in the booking panes
 * @param $item object the product item
 * @param $products array all products in the cart
 * @param $additional bool whether the table is for additional non date based items
 * @param $as_template bool whether the table is being displayed in an invoice template (printed/email)
 *                          and as such, needs to include style tags
 * @return array(details table, additional table)
*/
function hotel_booking_order_details_table(&$item, $products, $additional = FALSE, $as_template = FALSE) {
  $add_ons = $table_attributes = array();
  if ($as_template) {
    //this is for email template where we have to use inline styles
    $table_attributes = array('width' => '100%', 'style' => 'font-family: verdana, arial, helvetica; font-size: small;');
  }
  if (!isset($item->data['booking_upgrades_order'])) {
    $item->data['booking_upgrades_order'] = array();
  }
  if ($item->data['booking_upgrades_order'] && count($item->data['booking_upgrades_order']) > 0) {
    foreach ($item->data['booking_upgrades_order'] as $nid => $child_id) {
      foreach ($products as $product) {
        if ($product->nid == $nid && $product->data['child_id'] == $child_id) {
          $add_ons[$product->nid]['order'] = $product;
          $add_ons[$product->nid]['data'] = $product->data;
          $add_ons[$product->nid]['daily'] = ($add_ons[$product->nid]['data']['days']) ? TRUE : FALSE;
          if (!$additional_items) {
            $additional_items = (!$add_ons[$product->nid]['daily']) ? TRUE : FALSE;
          }
          if (!$daily_items) {
            $daily_items = ($add_ons[$product->nid]['daily']) ? TRUE : FALSE;
          }
        }
      }
    }
  }
  if (count($item->data['booking_upgrades']) == 0) {
  	unset($item->data['booking_upgrades']);
  }
  if (!$additional) {
    $addon_title = '';
    $qtys = '';;
    if ($add_ons && $daily_items) {
      $addon_title = t('Date Based Add-On Items');
      $qtys = t('Qty.');
    }
    $header = array(
      t('Dates'),
      array('data' => $addon_title, 'width' => '45%'),
      array('data' => '&nbsp', 'colspan' => 2),
      $qtys,
      t('Total')
    );
    $check_in = t('Check-In');

    $night_count = 1;

    foreach ($item->data['nights'] as $night) {
      $night_date = new DateObject($night, date_default_timezone(), DATE_FORMAT_DATETIME); //make a date object
      $night_formatted = format_date($night_date->format('U'), 'custom', 'l F j, Y'); //format object
      $price = $item->data['prices'][$night_count - 1];

      //alter our price (eg vat/gst/multi currency)
      $price = uc_currency_format($price);

      //create our row
      $rows[] = array(
        $night_formatted,
        '&nbsp;',
        t('Night !night',
          array('!night' => $night_count)
        ),
        $check_in,
        '&nbsp;',
        array('data' => $price, 'class' => 'price')
      );

      _hotel_booking_add_date_based_add_ons($rows, $add_ons, $night);
      $night_count++;
      $check_in = '';
    }
    $night_date->modify('+1 day');
    $check_out_formatted = format_date($night_date->format('U'), 'custom', 'l F j, Y');
    //add our check out row
    $rows[] = array(
      $check_out_formatted,
      '&nbsp;',
      '&nbsp;',
      t('Check-Out'),
      '&nbsp;',
      '&nbsp;'
    );

    _hotel_booking_add_date_based_add_ons($rows, $add_ons, $night_date->format('Y-m-d'));

    $table = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $table_attributes));
    return array($table, FALSE);
  }
  else {
    $header = array(
      array('data' => t('Additional Add-On & Upgrade Items'), 'colspan' => 2, 'width' => '100%'),
      t('Total')
    );
    $nids = array_keys($add_ons);
    $nodes = node_load_multiple($nids);
    foreach ($add_ons as $nid => $info) {
      if (!$add_ons[$nid]['daily']) {

        $price = $add_ons[$nid]['order']->qty * $add_ons[$nid]['order']->price;

        $price = uc_currency_format($price);

        $rows[] = array(
          $add_ons[$nid]['order']->qty .'x',
          array('data' => l($add_ons[$nid]['order']->title, 'booking_upgrades/'. $add_ons[$nid]['data']['parent_id']), 'width' => '100%'),
          array('data' => $price, 'class' => 'price'));
      }
    }

    return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $table_attributes));
  }
}

/**
 * Util to add date based add_ons to the table
 * @param $rows array the existing table rows
 * @param $add_ons array the add_ons data
 * @param $night string date of the night in ISO format
*/
function _hotel_booking_add_date_based_add_ons(&$rows, $add_ons, $night) {
  $nids = array_keys($add_ons);
  $nodes = node_load_multiple($nids);
  foreach ($add_ons as $nid => $info) {
    if ($info['daily'] && $info['data']['days'][$night]) {
      if ($info['order']) {
        //we have an order object with the historic price/title
        $price = $info['data']['days'][$night] * $info['order']->price;
        $title = $info['order']->title;
      }
      else {
        //we use the latest details
        $price = $info['data']['days'][$night] * $info['node']->sell_price;
        $title = $info['node']->title;
      }

      $price = uc_currency_format($price);
      $rows[] = array(
        '&nbsp;',
        l($title, 'booking_upgrades/'. $info['data']['parent_id']),
        '&nbsp;',
        '&nbsp;',
        array('data' => $info['data']['days'][$night], 'class' => 'qty'),
        array('data' => $price, 'class' => 'price')
      );
    }
  }
}

/**
 * Functions to build fieldsets & tables for cart pane
 */

function hotel_booking_details_pane_cart() {
  $items = uc_cart_get_contents();
  foreach ($items as $item) {
    if ($item->module == 'hotel_booking') {
      $nights_count[$item->cart_item_id] = count($item->data['nights']);
    }
  }

  $contents['booking_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Booking Details'),
    '#description' => (count($nights_count) > 1) ? t('Click on the Room Types below to view your booking details.') : '',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );

  foreach ($items as $item) {
    if ($item->module == 'hotel_booking') {
      $title = check_plain($item->title)
        .' ('. format_plural(count($item->data['nights']), '1 Night, ', '@count Nights, ')
        . format_plural($item->data['adults'], '1 Adult', '@count Adults')
        . format_plural($item->data['children'], ', 1 Child', ', @count Children')
        .')';
      $title .= ')';
      $node = node_load($item->nid);

      $contents['booking_details'][$item->cart_item_id] = array(
        '#type' => 'fieldset',
        '#title' => $title,
        '#description' => theme('hotel_booking_pane_help', array('product' => $item, 'node' => $node)),
        '#collapsible' => TRUE,
        // Default to collapsed if there are multiple room booking items in the cart,
        // or the booking has more than three nights and more than two add_ons, or if
        // the single booking item has more than 5 nights.
        '#collapsed' => (count($nights_count) > 1 ||
                         ($nights_count[$item->cart_item_id] > 3 &&
                          isset($item->data['booking_upgrades']) &&
                          count($item->data['booking_upgrades']) > 2) ||
                         ($nights_count[$item->cart_item_id] > 5)),
      );
      list($details, $additional) = hotel_booking_cart_details_table($item);
      $contents['booking_details'][$item->cart_item_id]['nid'] = array(
        '#markup' => $details,
      );
      if ($additional) {
        $contents['booking_details'][$item->cart_item_id]['nid']['additional'] = array(
          '#markup' => hotel_booking_cart_details_table($item, TRUE),
        );
      }
    }
  }
  return array('contents' => $contents);
}



/**
 * Callbacks for checkout pane
 * @param $op string op to perform
 * @param $order object uc order object
 * @param $values array form values after submit
 */
function hotel_booking_rewards($op, &$order, $values) {
  switch ($op) {
    case 'view':
      $description = t('If you have a reward membership card, please enter the number here.');
      $contents['reward_membership'] = array(
        '#type'          => 'textfield',
        '#title'         => variable_get('hotel_booking_reward_prompt', ''),
        '#default_value' => ($order->reward_membership) ? $order->reward_membership : '',
      );
      return array('description' => $description, 'contents' => $contents);

    case 'process':
      $order->reward_membership = $values['reward_membership'];
      return TRUE;

    case 'review':
      $review = ($order->reward_membership) ? $order->reward_membership : NULL;
      return $review;
  }
}

/**
 * Callback for the checkout pane details booking details
 * @param $op string op to perform
 * @param $order object uc order object
 * @param $arg2 varies
*/
function hotel_booking_details_pane_checkout($op, &$order, $arg2) {
  switch ($op) {
    case 'view':
      $contents = array();

      $items = uc_cart_get_contents();
      foreach ($items as $item) {
        if ($item->module == 'hotel_booking') {
          $nights_count[$item->cart_item_id] = count($item->data['nights']);
        }
      }

      foreach ($items as $item) {
        if ($item->module == 'hotel_booking') {
          $title = check_plain($item->title)
            .' ('. format_plural(count($item->data['nights']), '1 Night, ', '@count Nights, ')
            . format_plural($item->data['adults'], '1 Adult', '@count Adults')
            . format_plural($item->data['children'], ', 1 Child', ', @count Children')
            .')';
          $title .= ')';
          $node = node_load($item->nid);

          $contents['booking_details'][$item->cart_item_id] = array(
            '#type' => 'fieldset',
            '#title' => $title,
            '#description' => theme('hotel_booking_pane_help', array('product' => $item, 'node' => $node)),
            '#collapsible' => TRUE,
            // Default to collapsed if there are multiple room booking items in the cart,
            // or the booking has more than three nights and more than two add_ons, or if
            // a single booking item has more than 5 nights.
            '#collapsed' => (count($nights_count) > 1 ||
                         ($nights_count[$item->cart_item_id] > 3 &&
                          isset($item->data['booking_upgrades']) &&
                          count($item->data['booking_upgrades']) > 2) ||
                         ($nights_count[$item->cart_item_id] > 5)),
          );
          list($details, $additional) = hotel_booking_cart_details_table($item);
          $contents['booking_details'][$item->cart_item_id]['nid'] = array(
            '#markup' => $details,
          );
          if ($additional) {
            $contents['booking_details'][$item->cart_item_id]['nid']['additional'] = array(
              '#markup' => hotel_booking_cart_details_table($item, TRUE),
            );
          }
        }
      }
      return array('contents' => $contents);
  }
}

/**
 * Provides the details table for cart/checkout pane
 * @param $item object the cart/checkout item
 * @param $additional boolean TRUE if just showing additional items (non date based)
 * @return array($details, $additional items)
*/
function hotel_booking_cart_details_table(&$item, $additional = FALSE) {
  $add_ons = array();
  if (!isset($item->data['booking_upgrades'])) {
    $item->data['booking_upgrades'] = array();
  }
  $nids = array_keys($item->data['booking_upgrades']);
  $nodes = node_load_multiple($nids);
  if (count($item->data['booking_upgrades']) > 0) {
    foreach ($item->data['booking_upgrades'] as $nid => $cart_item_id) {
      $add_ons[$nid]['cart'] = db_query('SELECT * FROM {uc_cart_products} WHERE cart_item_id = :id', array(':id' => $cart_item_id))->fetchAll();
      $add_ons[$nid]['data'] = unserialize($add_ons[$nid]['cart']['data']);
      $add_ons[$nid]['daily'] = ($add_ons[$nid]['data']['days']) ? TRUE : FALSE;
      if (!$additional_items) {
        $additional_items = (!$add_ons[$nid]['daily']) ? TRUE : FALSE;
      }
      if (!$daily_items) {
        $daily_items = ($add_ons[$nid]['daily']) ? TRUE : FALSE;
      }

      // For the first time running through this function ($additional = FALSE)
      // If the item is not a daily addon item, we just won't pay it any mind.
      if (!$additional && $add_ons[$nid]['daily']) {
        $add_ons[$nid]['node'] = $nodes[$nid];
      }

      // If we're coming around a second time for non-daily items don't do
      // a node load on those daily items.
      elseif ($additional && !$add_ons[$nid]['daily']) {
        $add_ons[$nid]['node'] = $nodes[$nid];
      }
    }
  }
  if (!$additional) {
    $addon_title = '';
    $qtys = '';
    if ($add_ons && $daily_items) {
      $addon_title = t('Date Based Add-On Items');
      $qtys = t('Qty.');
    }
    $header = array(
      array('data' => t('Dates'), 'colspan' => 3),
      t('Total')
    );

    $check_in = t('Check-In');

    $night_count = 1;

    foreach ($item->data['nights'] as $night) {
      $night_date = new DateObject($night, date_default_timezone(), DATE_FORMAT_DATETIME); //make a date object
      $night_formatted = format_date($night_date->format('U'), 'custom', 'l F j, Y'); //format object

      $price = $item->data['prices'][$night_count - 1];

      $price = uc_currency_format($price);

      //create our row
      $rows[] = array(
        $night_formatted,
        t('Night !night',
          array('!night' => $night_count)
        ),
        $check_in,
        array('data' => $price, 'class' => 'price')
      );

      $night_count++;
      $check_in = '';
    }
    $night_date->modify('+1 day');
    $check_out_formatted = format_date($night_date->format('U'), 'custom', 'l F j, Y');
    //add our check out row
    $rows[] = array(
      $check_out_formatted,
      '&nbsp;',
      t('Check-Out'),
      '&nbsp;'
    );

    $table = theme('table', array('header' => $header, 'rows' => $rows));
    return array($table, FALSE);
  }
  else {
    $header = array(
      array('data' => t('Additional Add-On & Upgrade Items'), 'colspan' => 2, 'width' => '100%'),
      t('Total')
    );
    $nids = array_keys($add_ons);
    $nodes = node_load_multiple($nids);
    foreach ($add_ons as $nid => $info) {
      if (!$add_ons[$nid]['daily']) {

        $price = $info['cart']->qty * $info['node']->sell_price;

        //now format it
        $price = uc_currency_format($price);

        $rows[] = array(
          $info['cart']->qty .'x',
          array('data' => l($info['node']->title, 'booking_upgrades/'. $info['data']['parent_id']), 'width' => '100%'),
          array('data' => $price, 'class' => 'price'));
      }
    }

    return theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => $table_attributes));
  }
}
