<?php

/**
 * @file
 * Contains commerce_auction page callback functions.
 */

/**
 * Bidding form callback function.
 */
function commerce_auction_place_bid_form($form, &$form_state, $node) {
  $form['warnings'] = array(
    '#weight' => -1,
    '#type' => 'markup',
    '#markup' => '<p>' . t('Please keep in mind that once you bid on an auction, you are supposed to buy the product if you win the auction.') . '<br />' . t('We will decrease your rate and access in this website, or if it happens repeatedly, completely suspend your account.') . '</p>',
  );

  $entity = entity_create('commerce_auction_bid', array());
  $entity->type = 'auction_bid';
  field_attach_form('commerce_auction_bid', $entity, $form, $form_state);

  $form_state['#entity'] = $entity;
  $form_state['#node'] = $node;
  $form['save_bid'] = array(
    '#value' => t('Place my bid!'),
    '#attributes' => array('class' => array('display-block', 'margin-top-10')),
    '#type' => 'submit',
    '#weight' => 100,
  );

  $currency = commerce_currency_load(commerce_default_currency());
  $form['bid_amount'][LANGUAGE_NONE][0]['amount']['#field_suffix'] = $currency['symbol'];
  if (isset($form['#metatags'])) {
    unset($form['#metatags']);
  }

  return $form;
}

/**
 * Validation function of bidding form.
 */
function commerce_auction_place_bid_form_validate($form, &$form_state) {
  field_attach_form_validate('commerce_auction_bid',
                              $form_state['#entity'],
                              $form, $form_state);

  $currency = commerce_currency_load(commerce_default_currency());
  $highest_bid = commerce_currency_amount_to_decimal(commerce_auction_get_highest_bid($form_state['#node']), $currency['code']);
  $bid_amount = commerce_currency_amount_to_decimal($form_state['values']['bid_amount'][LANGUAGE_NONE][0]['amount'], $currency['code']);
  if ($highest_bid > 0 && $bid_amount <= $highest_bid) {
    form_set_error('auction_bid',
                    t('Your bid amount should be more than last bid amount(@amount)',
                      array('@amount' => $highest_bid)
                      )
                  );
  }
  if ($highest_bid < 0) {
    $highest_bid = commerce_currency_amount_to_decimal($form_state['#node']->auction_starting_price[LANGUAGE_NONE][0]['amount'], $currency['code']);
  }

  $increase_amount = $bid_amount - $highest_bid;
  $bid_inc = variable_get('commerce_auction_bid_increment', 0.25);
  $min_bid_inc = variable_get('commerce_auction_min_bid_inc', 0.50);
  $max_bid_inc = variable_get('commerce_auction_max_bid_inc', 1000.00);
  $max_bid_inc_percent = variable_get('commerce_auction_max_bid_inc_percent', 100);

  $tmp = field_get_items('node', $form_state['#node'], 'field_bid_increment');
  $bid_inc = commerce_auction_get_increment_limit($tmp, $bid_inc);

  $tmp = field_get_items('node', $form_state['#node'], 'field_min_bid_inc');
  $min_bid_inc = commerce_auction_get_increment_limit($tmp, $min_bid_inc);

  $tmp = field_get_items('node', $form_state['#node'], 'field_max_bid_inc');
  $max_bid_inc = commerce_auction_get_increment_limit($tmp, $max_bid_inc);

  $tmp = field_get_items('node', $form_state['#node'], 'field_max_bid_inc_percent');
  $max_bid_inc_percent = commerce_auction_get_increment_limit($tmp, $max_bid_inc_percent);

  if ($max_bid_inc_percent) {
    $max_bid_inc_p_val = $highest_bid * ($max_bid_inc_percent / 100);
  }
  if ($bid_inc && !commerce_auction_dividable($increase_amount, $bid_inc)) {
    form_set_error('auction_bid',
                    t('Your bid amount should increase the current high bid (@highest) by a multiple of @inc to be accepted',
                      array(
                        '@inc' => $bid_inc,
                        '@highest' => $currency['symbol'] . $highest_bid,
                      )
                    )
                  );
  }
  if ($min_bid_inc && $increase_amount < $min_bid_inc) {
    form_set_error('auction_bid',
                    t('Your bid amount must be at least @min_inc higher than the current high bid (@highest) to be accepted.',
                      array(
                        '@min_inc' => $currency['symbol'] . $min_bid_inc,
                        '@highest' => $currency['symbol'] . $highest_bid,
                      )
                    )
                  );
  }
  if ($max_bid_inc && $increase_amount > $max_bid_inc) {
    form_set_error('auction_bid',
                    t('Your bid amount can not be more than @max_inc higher than the current high bid (@highest).',
                      array(
                        '@max_inc' => $currency['symbol'] . $max_bid_inc,
                        '@highest' => $currency['symbol'] . $highest_bid,
                      )
                    )
                  );
  }
  if ($max_bid_inc_percent && $increase_amount > $max_bid_inc_p_val) {
    form_set_error('auction_bid',
                    t('Your bid should not increase the current high bid (@highest) more than @percent% of current highest bid (@max_inc).',
                      array(
                        '@percent' => $max_bid_inc_percent,
                        '@max_inc' => $currency['symbol'] . $max_bid_inc_p_val,
                        '@highest' => $currency['symbol'] . $highest_bid,
                      )
                    )
                  );
  }

  $timeout_field = field_get_items('node', $form_state['#node'], 'auction_timeout');
  $expiration_timestamp = $timeout_field[0]['value'];
  // If This auction is hosted by this user or is expired,
  // do not allow her to access this page.
  if ($expiration_timestamp <= REQUEST_TIME) {
    form_set_error('auction_bid',
                    t('Sorry, The auction has been finilized, no more bids are accepted.'));
  }
  $product_ref_field = commerce_auction_find_field_name('node', $form_state['#node']->type, 'commerce_product_reference');
  $product_field = field_get_items('node', $form_state['#node'], $product_ref_field);
  $product = commerce_product_load($product_field[0]['product_id']);
  $field_name = commerce_auction_find_field_name('commerce_product', $product->type, 'commerce_price');
  $field = field_get_items('commerce_product', $product, $field_name);
  $base_price = $field[0]['amount'] / 100;

  if ($form_state['values']['bid_amount'][LANGUAGE_NONE][0]['amount'] <= $base_price) {
    form_set_error('auction_bid',
                    t('Your bid amount should be more than base price(@amount)',
                      array('@amount' => $base_price))
                  );
  }
}

/**
 * Submit function for bidding form.
 */
function commerce_auction_place_bid_form_submit($form, &$form_state) {
  $node = $form_state['#node'];
  $timeout_field = field_get_items('node', $node, 'auction_timeout');
  if (!isset($timeout_field) || empty($timeout_field) || !$timeout_field[0]['value']) {
    drupal_set_message(t('Auction timeout has no value, Please first set a value for the timeout field.'), 'error');
  }
  field_attach_submit('commerce_auction_bid', $form_state['#entity'], $form, $form_state);
  global $user;

  // Save entity (new commerce_auction_bid entity that contains the bid amount
  // entered in the form.)
  $entity = $form_state['#entity'];
  $entity->uid = $user->uid;
  $entity->created = REQUEST_TIME;
  $amount = commerce_currency_amount_to_decimal($entity->bid_amount[LANGUAGE_NONE][0]['amount'], commerce_default_currency());
  $entity->title = $user->name . ' (' . $amount . ')';
  $entity->save();

  // Update node, so that it references this bid too,
  $delta = (empty($node->auction_bid_refs[LANGUAGE_NONE])) ? 0 : max(array_keys($node->auction_bid_refs[LANGUAGE_NONE])) + 1;
  $node->auction_bid_refs[LANGUAGE_NONE][$delta]['target_id'] = $entity->id;

  // Update highest bid on display node.
  // This makes creating per-user bidding tables easier.
  $node->auction_highest_bid[LANGUAGE_NONE][0] = $entity->bid_amount[LANGUAGE_NONE][0];

  // If we are in the last 15 minutes of the auction timeout,
  // Extend timeout for 30minutes
  $expiration_timestamp = $timeout_field[0]['value'];
  $time_period = variable_get('commerce_auction_final_period', 15) * 60;
  $extension_time = variable_get('commerce_auction_extension_time', 30) * 60;
  $extend_timeout = variable_get('commerce_auction_extend', TRUE);
  if ($extend_timeout &&
      $expiration_timestamp - REQUEST_TIME < $time_period &&
      $expiration_timestamp - REQUEST_TIME > 0) {
    $timeout_field[0]['value'] = $extension_time + $expiration_timestamp;
  }
  $node->auction_timeout[LANGUAGE_NONE][0]['value'] = $timeout_field[0]['value'];

  // Now that everything is updated, save the node!
  node_save($node);

  drupal_set_message(t('Your bid is saved!'));
  drupal_goto('node/' . $node->nid);
}
