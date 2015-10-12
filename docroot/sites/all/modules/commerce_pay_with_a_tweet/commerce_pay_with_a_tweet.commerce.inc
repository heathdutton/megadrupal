<?php

/**
 * @file
 * Drupal commerce hooks on this file.
 */


/**
 * Implements hook_commerce_payment_method_info().
 */
function commerce_pay_with_a_tweet_commerce_payment_method_info() {
  $payment_methods = array();

  $payment_methods['commerce_pay_with_a_tweet'] = array(
    'title' => t('Tweet payment'),
    'description' => t('Commerce tweet payment.'),
    'active' => TRUE,
    'offsite' => TRUE,
    'offsite_autoredirect' => TRUE,
  );

  return $payment_methods;
}

/**
 * Payment method callback: submit form.
 */
function commerce_pay_with_a_tweet_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  // Get default tweet id.
  // See admin/commerce/config/commerce_pay_with_a_tweet_config.
  $commerce_pay_with_a_tweet_default_tweet = variable_get('commerce_pay_with_a_tweet_default_tweet', FALSE);
  $form = array();

  // Merge in values from the order.
  if (!empty($order->data['commerce_pay_with_a_tweet'])) {
    $pane_values += $order->data['commerce_pay_with_a_tweet'];
  }

  // Default tweet content.
  $tweet_content = t('Please contact an administrator: error to get default tweet message');

  // Check if default tweet is selected.
  if (!empty($commerce_pay_with_a_tweet_default_tweet)) {
    // Get pid (product < product display < default tweet).
    $pid = commerce_pay_with_a_tweet_get_tweet_id($order);
    if (!empty($pid)) {
      $commerce_pay_with_a_tweet = pay_with_a_tweet_load($pid);

      if (!empty($commerce_pay_with_a_tweet)) {
        // Save tweet message.
        // Try to replace tokens.
        // see commerce_product_tokens() and
        // "site" (https://drupal.org/node/390482#drupal7tokenslist).
        $tweet_content = commerce_pay_with_a_tweet_get_message_with_first_product_token_replace($order, $commerce_pay_with_a_tweet->message);
      }
      else {
        commerce_pay_with_a_tweet_administrator_set_message('Unable to load commerce_pay_with_a_tweet object @pid', array('@pid' => $commerce_pay_with_a_tweet->pid), WATCHDOG_ERROR);
      }
    }
    else {
      commerce_pay_with_a_tweet_administrator_set_message('Error in commerce_pay_with_a_tweet_get_tweet_id()', array(), WATCHDOG_ERROR);
    }
  }
  else {
    commerce_pay_with_a_tweet_administrator_set_message('Please check your config, no default tweet selected', array(), WATCHDOG_ERROR);
  }

  // Twitter image.
  $vars_img = array(
    'path' => drupal_get_path('module', 'commerce_pay_with_a_tweet') . '/img/twitter.png',
    'alt' => 'Twitter logo',
    'title' => 'Twitter logo',
    'width' => '41px',
    'height' => '39px',
    'attributes' => array('class' => array('twitter-logo')),
  );
  $img = theme('image', $vars_img);

  // See template theme/commerce-tweet-payment.tpl.php.
  $form['tweet_information'] = array(
    '#markup' => theme('commerce_pay_with_a_tweet_payment', array(
      'tweet' => $tweet_content,
      'img' => $img,
    )),
  );

  return $form;
}

/**
 * Payment method callback: submit form submission.
 */
function commerce_pay_with_a_tweet_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $order->data['commerce_pay_with_a_tweet'] = $pane_values;
  $order->data['commerce_pay_with_a_tweet']['payment_method'] = $payment_method;
  $order->data['commerce_pay_with_a_tweet']['charge'] = $charge;
  commerce_order_save($order);
}

/**
 * Creates an example payment transaction for the specified charge amount.
 *
 * @param object $payment_method
 *   The payment method instance object used to charge this payment.
 * @param object $order
 *   The order object the payment applies to.
 * @param array $charge
 *   An array indicating the amount and currency code to charge.
 */
function commerce_pay_with_a_tweet_success_transaction($payment_method, $order, $charge) {
  $transaction = commerce_payment_transaction_new('commerce_pay_with_a_tweet', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = 'Commerce payment with success status.';

  commerce_payment_transaction_save($transaction);
}

/**
 * Payment method callback; generation callback for the payment redirect form.
 *
 * Returns form elements that should be submitted to the redirected payment
 * service; because of the array merge that happens upon return, the service’s
 * URL that should receive the POST variables should be set in the #action
 * property of the returned form array.
 */
function commerce_pay_with_a_tweet_redirect_form($form, &$form_state, $order, $payment_method) {
  $commerce_pay_with_a_tweet_default_tweet = variable_get('commerce_pay_with_a_tweet_default_tweet', FALSE);
  if (empty($commerce_pay_with_a_tweet_default_tweet)) {
    commerce_pay_with_a_tweet_administrator_set_message('Please check your config, no default tweet selected', array(), WATCHDOG_ERROR);
  }
  else {
    // Skip form after a come back to checkout/[order_id].
    if ((empty($order->data['commerce_pay_with_a_tweet']['httpcode'])) || ($order->data['commerce_pay_with_a_tweet']['httpcode'] != 200)) {
      module_load_include('inc', 'commerce_pay_with_a_tweet', 'commerce_pay_with_a_tweet.page');

      // pid: Tweet id to load. Default value: $settings['default_tweet'].
      $pid = commerce_pay_with_a_tweet_get_tweet_id($order);
      if (!empty($pid)) {
        $commerce_pay_with_a_tweet = pay_with_a_tweet_load($pid);
        if (!empty($commerce_pay_with_a_tweet)) {
          // Connection to twitter to post a tweet.
          commerce_pay_with_a_tweet_twitter_connect($commerce_pay_with_a_tweet, COMMERCE_PAY_WITH_A_TWEET_PAYMENT);
        }
        else {
          commerce_pay_with_a_tweet_administrator_set_message('Unable to load commerce_pay_with_a_tweet object @pid', array('@pid' => $commerce_pay_with_a_tweet->pid), WATCHDOG_ERROR);
        }
      }
    }
    elseif (!empty($order->data['commerce_pay_with_a_tweet'])) {
      unset($order->data['commerce_pay_with_a_tweet']);
    }
  }
}
