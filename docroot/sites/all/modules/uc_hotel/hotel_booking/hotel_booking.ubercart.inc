<?php
/*
 * @file hotel_booking.ubercart.inc
 * Ubercart hooks for hotel_booking module
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */
/**
 * Implements hook_update_cart_item().
 */
function hotel_booking_uc_update_cart_item($nid, $data = array(), $qty, $cid = NULL) {
    if (!$nid) return NULL;

  $cid = !(is_null($cid) || empty($cid)) ? $cid : uc_cart_get_id();
  if ($qty === 0) {
    // If the Addon module is enabled, remove all the upgrade & addon items from
    // the cart when their parent item is deleted.
    if (module_exists('hotel_addons_upgrades')) {
      hotel_booking_inc('util');
      hotel_booking_remove_upgrades($data['booking_upgrades']);
    }
    uc_cart_remove_item($nid, $cid, $data);
  }
  else {
    db_update("uc_cart_products")
    ->fields(array(
      'qty' => $qty,
      'changed' => REQUEST_TIME
    ))
    ->condition('nid', $nid)
    ->condition('cart_id', $cid)
    ->condition('data', serialize($data))
    ->execute();
  }

  // Rebuild the items hash
  uc_cart_get_contents(NULL, 'rebuild');
}

/**
 * Implements hook_order().
 */
function hotel_booking_uc_order($op, &$order) {
  switch ($op) {
    case 'load':
      hotel_booking_inc('util');
      if (hotel_booking_item_in_order($order) &&
          variable_get('hotel_booking_reward_prompt', '')) {
        $data = db_query('SELECT *
                           FROM {hotel_booking_reward_memberships}
                           WHERE order_id = :order_id', array(':order_id' => $order->order_id),
                           array('fetch' => PDO::FETCH_OBJ))->fetch();
        if ($data) {
          $order->reward_membership = $data->member_number;
        }
        foreach ($order->products as $key => $item) {
          if ($item->data['module'] == 'hotel_booking') {
            $item->price = $item->data['total'];
            $item->weight = NULL;
            $item->cost = 0;
            $item->shippable = FALSE;
            $item->sell_price = $item->price;
            $order->products[$key] = $item;
          }
        }
      }
      break;

    case 'save':
      if (hotel_booking_item_in_order($order) &&
          variable_get('hotel_booking_reward_prompt', '')) {
        if (!empty($order->reward_membership)) {
          $record = array(
            'order_id' => $order->order_id,
            'member_number' => $order->reward_membership
          );
          drupal_write_record('hotel_booking_reward_memberships', $record, array('order_id'));
        }
      }
      break;

    case 'delete':
      db_delete('hotel_booking_reward_memberships')->condition('order_id', $order->order_id)->execute();
      break;
  }
}


/**
 * Implements hook_cart_display().
 */
function hotel_booking_uc_cart_display($item) {
  $node = node_load($item->nid);
  $element = array();
  $element['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $element['module'] = array('#type' => 'value', '#value' => 'hotel_booking');
  $element['remove'] = array('#type' => 'checkbox');

  $title = check_plain($item->title);
  $title .= ' ('. count($item->data['nights']) . t(' Nights, ');
  $title .= format_plural($item->data['adults'], '1 Adult', '@count Adults');
  $title .= format_plural($item->data['children'], ', 1 Child', ', @count Children');
  $title .= ')';
  $element['title'] = array(
    '#markup' => node_access('view', $node) ? l($title, 'node/'. $node->nid) : check_plain($item->title),
  );

  $element['#total'] = $item->price;
  $element['data'] = array('#type' => 'value', '#value' => serialize($item->data));
  $element['qty'] = array(
    '#type'          => 'markup',
    '#markup'         => 1,
    '#default_value'  => 1 //this is to stop the ubercart errors
  );

  if (variable_get('hotel_booking_teaser_in_cart', FALSE)) {
    $description = theme('hotel_booking_room_cart_description', array('teaser' => $node->teaser, 'data' => NULL)); //$item->data);
  }
  else {
    $description = theme('hotel_booking_room_cart_description', array('data' => NULL)); //$item->data);
  }
  if ($description) {
    $element['description'] = array('#markup' => $description);
    $addtl = hotel_booking_uc_product_description($item);
    if ($addtl) {
      $element['description']['#markup'] .= render($addtl);
    }
  }

  return $element;
}

/**
 * Implements hook_uc_product_description
*/
function hotel_booking_uc_product_description($product) {
  if ($product->data['module'] == 'hotel_booking' &&
      ($type = variable_get('hotel_booking_payment_type', 0)) &&
      ($rate = variable_get('hotel_booking_payment_rate')) &&
    $product->data['total'] != $product->price) {
    return array(
      '#theme' => 'item_list',
      '#items' => array(
        t('!type Deposit: !deposit',
        array(
          '!type' => ($type == 2 ? number_format($rate, 2) .'%' : uc_currency_format($rate)), 
          '!deposit' => uc_currency_format($product->price),
        )),
        t('Full Price: !price',
        array(
          '!price' => uc_currency_format($product->data['total']),
        )),
        t('Balance Due on Check-in: !balance',
        array(
          '!balance' => uc_currency_format($product->data['total'] - $product->price)
        ))
      ),
      '#type' => 'ul',
      '#title' => t('Payment details')
    );
  }
}

/**
 * Implements hook_cart_item().
 */
function hotel_booking_uc_cart_item($op, $item) {
  switch ($op) {
    case 'load':
      if ($item->module == 'hotel_booking') {
        $data = $item->data;
        $item->price = $data['total'];
        if (($type = variable_get('hotel_booking_payment_type', 0)) &&
            ($rate = variable_get('hotel_booking_payment_rate'))) {
          $item->full_price = $item->price;
          switch ($type) {
            case 1:
              $item->price = $rate;
              break;
            case 2:
              $item->price = $data['total'] * ($rate / 100);
          }
        }
        $item->weight = NULL;
        $item->cost = 0;
        $item->weight_units = 'kg';
        $item->shippable = FALSE;
        $item->sell_price = $item->price;
        $item->qty = 1;
      }
      break;

    case 'remove':
      if (!is_array($item->data)) {
        $data = unserialize($item->data);
      }
      else {
        $data = $item->data;
      }
      if ($data['module'] == 'hotel_booking') {
        $item->price = $data['total'];
        $item->qty = 1;

        // Add the nid so we've got it in the trigger.
        $data['nid'] = $item->nid; 
      }
      break;
  }
}

/**
 * Implements hook_cart_item_description().
 */
function hotel_booking_uc_cart_item_description($item) {
  $node = node_load($item->nid);
  return theme('hotel_booking_room_cart_description', array('teaser' => $node->teaser, 'data' => $item->data));
}

/**
 * ----------------------------------------
 * Cart, Checkout, and Order Pane functions
 * ----------------------------------------
 */


/**
 * Implements hook_cart_pane().
 */
function hotel_booking_uc_cart_pane() {
  hotel_booking_inc('util');
  if (hotel_booking_item_in_cart()) {
    $panes[] = array(
      'id' => 'booking_details',
      'title' => t('Booking Details'),
      'weight' => -15,
      'body' => (drupal_get_form('hotel_booking_details_pane_cart')),
      'collapsible' => TRUE,
      'collapsed'   => FALSE,
    );
    return $panes;
  }
}

/**
 * Implements hook_checkout_pane().
 */
function hotel_booking_uc_checkout_pane() {
  hotel_booking_inc('util');
  if (hotel_booking_item_in_cart()) {
    if (variable_get('hotel_booking_reward_prompt', '')) {
      $panes[] = array(
        'id'          => 'reward_membership',
        'callback'    => 'hotel_booking_rewards',
        'title'       => t('Reward Membership'),
        'weight'      => 6,
        'enabled'     => TRUE,
        'shippable' => FALSE,
        'process'     => TRUE,
        'collapsible' => FALSE,
      );
    }
    $panes[] = array(
      'id'            => 'booking_details',
      'callback'      => 'hotel_booking_details_pane_checkout',
      'title'         => t('Booking Details'),
      'weight'        => -8,
      'shippable' => FALSE,
      'enabled'       => TRUE,
      'collapsible'   => TRUE,
      'process'       => TRUE,
    );
    return $panes;
  }
}

/**
 * Implements hook_order_pane().
 */
function hotel_booking_uc_order_pane() {
  $panes[] = array(
    'id' => 'hotel_booking',
    'callback' => 'hotel_booking_booking_pane',
    'title' => t('Booking Details'),
    'desc' => t('Display details of room bookings.'),
    'class' => 'abs-left',
    'weight' => 5,
    'show' => array('view', 'customer'),
  );
  if (variable_get('hotel_booking_reward_prompt', '')) {
    $panes[] = array(
      'id' => 'reward_membership',
      'callback' => 'hotel_booking_pane_rewards',
      'title' => t('Reward Membership'),
      'weight' => 5,
      'show' => array('view', 'customer'),
    );
  }
  return $panes;
}