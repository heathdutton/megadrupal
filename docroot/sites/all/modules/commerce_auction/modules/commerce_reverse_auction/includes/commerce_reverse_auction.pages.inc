<?php

/**
 * @file
 * Contains page callbacks of commerce_reverse_auction module.
 */

/**
 * Commerce reverse auction bidding page callback.
 */
function commerce_reverse_auction_place_bid_form($form, &$form_state, $node) {
  $entity = entity_create('commerce_auction_bid', array());
  $entity->type = 'reverse_auction_bid';
  field_attach_form('commerce_auction_bid', $entity, $form, $form_state);
  $form_state['#entity'] = $entity;
  $form_state['#node'] = $node;
  unset($form['reverse_auction_ref']);
  $form['save_bid'] = array(
    '#value' => t('Place my bid!'),
    '#attributes' => array('class' => array('display-block', 'margin-top-10')),
    '#type' => 'submit',
    '#ief_submit_all' => TRUE,
    '#weight' => 100,
  );
  if (isset($form['#metatags'])) {
    unset($form['#metatags']);
  }
  return $form;
}

/**
 * Validation function of place bid and edit bid forms.
 */
function commerce_reverse_auction_place_bid_form_validate($form, &$form_state) {
  field_attach_form_validate('commerce_auction_bid',
                            $form_state['#entity'],
                            $form, $form_state);

  // Validate timeout
  $timeout_field = field_get_items('node', $form_state['#node'], 'auction_timeout');
  $expiration_timestamp = $timeout_field[0]['value'];
  $validate_decreasing = variable_get('commerce_reverse_auction_decreasing_amount', TRUE);
  // If this auction is expired, do not allow the user to place bid.
  if ($expiration_timestamp <= REQUEST_TIME) {
    form_set_error('auction_bid',
                    t('Sorry, The auction has been finilized, no more bids are accepted.'));
  }

  // Validate min bid < bid amount < max bid.
  $bid_amount = -1;
  if (isset($form_state['values']['bid_product'][LANGUAGE_NONE]['form'])) {
    $bid_amount = $form_state['values']['bid_product'][LANGUAGE_NONE]['form']['commerce_price'][LANGUAGE_NONE][0]['amount'];
  }
  elseif (isset($form_state['values']['bid_product'][LANGUAGE_NONE]['entities'][0]['form'])) {
    $bid_amount = $form_state['values']['bid_product'][LANGUAGE_NONE]['entities'][0]['form']['commerce_price'][LANGUAGE_NONE][0]['amount'];
  }
  elseif (isset($form_state['values']['bid_product'][LANGUAGE_NONE]['entities'][0])) {
    $instance_id = $form_state['field']['bid_product'][LANGUAGE_NONE]['instance']['id'];
    $product = $form_state['inline_entity_form'][$instance_id]['entities'][0]['entity'];
    $price_field = field_get_items('commerce_product', $product, 'commerce_price');
    $bid_amount = $price_field[0]['amount'];
  }
  else {
    form_set_error('bid_product', t('Please define a product for your bid.'));
  }
  $min_amount = field_get_items('node', $form_state['#node'], 'auction_min_price');
  $max_amount = field_get_items('node', $form_state['#node'], 'auction_max_price');
  if ($bid_amount < $min_amount[0]['amount']) {
    $amount = commerce_currency_amount_to_decimal($min_amount[0]['amount'], commerce_default_currency());
    $currency = commerce_currency_load(commerce_default_currency());
    form_set_error('bid_product][und][form][product_details', t('Your bid amount should be higher than minimum bid amount (@min)', array('@min' => $currency['symbol'] . $amount)));
  }
  elseif ($bid_amount > $max_amount[0]['amount']) {
    $amount = commerce_currency_amount_to_decimal($max_amount[0]['amount'], commerce_default_currency());
    $currency = commerce_currency_load(commerce_default_currency());
    form_set_error('bid_product][und][form][product_details', t('Your bid amount should be less than maximum bid amount (@max)', array('@max' => $currency['symbol'] . $amount)));
  }

  if ($validate_decreasing) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'commerce_auction_bid')
          ->entityCondition('bundle', 'reverse_auction_bid')
          ->fieldCondition('reverse_auction_ref', 'target_id', $form_state['#node']->nid, '=')
          ->propertyOrderBy('changed', 'DESC')->range(0, 1);
    $result = $query->execute();
    if ($result) {
      $entity_id = array_shift($result['commerce_auction_bid']);
      $entity = entity_load('commerce_auction_bid', array($entity_id->id));
      $entity = $entity[$entity_id->id];
      $product_field = field_get_items('commerce_auction_bid', $entity, 'field_product');
      if ($product_field) {
        $product = commerce_product_load($product_field[0]['product_id']);
        $price = field_get_items('commerce_product', $product, 'commerce_price');
        if ($price) {
          if ($price[0]['amount'] <= $bid_amount) {
            $amount = commerce_currency_amount_to_decimal($max_amount[0]['amount'], commerce_default_currency());
            $currency = commerce_currency_load(commerce_default_currency());
            form_set_error('bid_product][und][form][product_details', t('Your bid amount should be less than previous bid (@pre).', array('@pre' => $currency['symbol'] . $amount)));
          }
        }
      }
    }
  }
}

/**
 * Submit function for reverse auction bid form.
 */
function commerce_reverse_auction_place_bid_form_submit($form, &$form_state) {
  $entity = $form_state['#entity'];
  global $user;
  // The bid product should be disabled, it will get enabled if the auction owner
  // accepts this bid.
  $instance_id = $form_state['field']['bid_product'][LANGUAGE_NONE]['instance']['id'];
  $form_state['inline_entity_form'][$instance_id]['entities'][0]['entity']->status = 0;
  $entity->title = t('Bid by @user', array('@user' => $user->name));
  $entity->uid = $user->uid;
  $entity->created = REQUEST_TIME;
  field_attach_submit('commerce_auction_bid', $entity, $form, $form_state);
  $entity->reverse_auction_ref[LANGUAGE_NONE][0]['target_id'] = $form_state['#node']->nid;
  entity_save('commerce_auction_bid', $entity);
  drupal_set_message(t('Your bid has been saved!'));
  drupal_goto('node/' . $form_state['#node']->nid);
}

/**
 * Form callback for editting bids.
 */
function commerce_reverse_auction_edit_bid_form($form, &$form_state, $node) {
  global $user;
  $query = new EntityFieldQuery();
  // Entity type is commerce_auction_bid
  $query->entityCondition('entity_type', 'commerce_auction_bid')
  // Bundle is the reverse_auction_bid
  ->entityCondition('bundle', 'reverse_auction_bid')
  // Owner is the same as the currently logged in user,
  ->propertyCondition('uid', $user->uid, '=')
  // reverse_auction_ref referencing this node,
  ->fieldCondition('reverse_auction_ref', 'target_id', $node->nid, '=');
  $result = $query->execute();
  $entity_id = array_shift($result['commerce_auction_bid']);
  $entity = entity_load('commerce_auction_bid', array($entity_id->id));
  $entity = $entity[$entity_id->id];

  $bid_product = field_get_items('commerce_auction_bid', $entity, 'bid_product');
  $form_state['#product_id'] = $bid_product[0]['product_id'];
  field_attach_form('commerce_auction_bid', $entity, $form, $form_state);
  $form_state['#entity'] = $entity;
  $form_state['#node'] = $node;
  unset($form['reverse_auction_ref']);
  $form['save_bid'] = array(
    '#value' => t('Update my bid!'),
    '#attributes' => array('class' => array('display-block', 'margin-top-10')),
    '#type' => 'submit',
    '#weight' => 100,
    '#ief_submit_all' => TRUE,
  );
  if (user_access('delete own bid from reverse auction')) {
    $form['delete_bid'] = array(
      '#value' => t('Delete my bid!'),
      '#attributes' => array('class' => array('display-block', 'margin-top-10')),
      '#type' => 'submit',
      '#weight' => 101,
      '#ief_submit_all' => TRUE,
    );
  }
  if (isset($form['#metatags'])) {
    unset($form['#metatags']);
  }
  $form['#validate'][] = 'commerce_reverse_auction_place_bid_form_validate';
  return $form;
}

/**
 * Submit callback for bid edit page.
 */
function commerce_reverse_auction_edit_bid_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == $form_state['values']['save_bid']) {
    $entity = $form_state['#entity'];
    $instance_id = $form_state['field']['bid_product'][LANGUAGE_NONE]['instance']['id'];
    $form_state['inline_entity_form'][$instance_id]['entities'][0]['entity']->status = 0;
    field_attach_submit('commerce_auction_bid', $entity, $form, $form_state);
    entity_save('commerce_auction_bid', $entity);
    drupal_set_message(t('Your bid has been saved!'));
  }
  elseif ($form_state['values']['op'] == $form_state['values']['delete_bid']) {
    commerce_product_delete($form_state['#product_id']);
    entity_delete('commerce_auction_bid', $form_state['#entity']->id);
    drupal_set_message(t('Your bid has been deleted!'));
  }
  drupal_goto('node/' . $form_state['#node']->nid);
}

/**
 * Form callback for accepting a bid.
 */
function commerce_reverse_auction_accept_bid_confirm($form, &$form_state, $node, $bid_id) {
  $form['#node'] = $node;
  $entity = entity_load('commerce_auction_bid', array($bid_id));
  $form['#bid'] = $entity[$bid_id];
  $entity = $form['#bid'];
  $account = user_load($entity->uid);
  return confirm_form($form,
          t('Are you sure you want to accept this !bid by !user?',
                  array('!bid' => l(t('bid'), 'node/' . $node->nid . '/bid/' . $bid_id), '!user' => theme('username', array('account' => $account)))),
          'node/' . $node->nid . '/bid/' . $bid_id,
          t('Doing this will close this reverse auction and add the product to your shopping cart. This action cannot be undone.'),
          t('Yes'),
          t('No')
  );
}

/**
 * Validate callback for accept bid form.
 */
function commerce_reverse_auction_accept_bid_confirm_validate($form, &$form_state) {
  $product_field = field_get_items('node', $form['#node'], 'field_product');
  if (isset($product_field[0]['product_id']) && $product_field[0]['product_id'] > 0) {
    drupal_set_message(t('You have already accepted one of the bids for this reverse auction.'), 'warning');
    drupal_goto('node/' . $form['#node']->nid);
  }
}

/**
 * Submit callback for accept bid form.
 */
function commerce_reverse_auction_accept_bid_confirm_submit($form, &$form_state) {
  // Reference the bid product from reverse auction display node.
  $node = $form['#node'];
  $bid = $form['#bid'];
  $product_field = field_get_items('commerce_auction_bid', $bid, 'bid_product');
  $node->field_product[LANGUAGE_NONE][] = $product_field[0];
  $node->auction_timeout[LANGUAGE_NONE][0]['value'] = REQUEST_TIME;
  node_save($node);

  // Add the product to the cart of the logged in user.
  $order = commerce_cart_order_load($node->uid);
  if (empty($order)) {
    $order = commerce_order_new($node->uid, 'checkout_checkout');
    commerce_order_save($order);
  }
  $product = commerce_product_load($product_field[0]['product_id']);
  $product->status = 1;
  commerce_product_save($product);
  $line_item = commerce_product_line_item_new($product, 1, $order->order_id, array('context' => array('display_path' => 'node/' . $node->nid)), 'commerce_reverse_auction_lineitem');
  commerce_line_item_save($line_item);
  commerce_cart_product_add($node->uid, $line_item);
  // Fire the Rules event.
  rules_invoke_event('commerce_reverse_auction_bid_selected', $bid, $node);

  // Display a success message.
  drupal_set_message(t('Congratulations! The product is added to your cart, Please consider checking out as soon as possible!'));
  drupal_goto('node/' . $node->nid);
}
