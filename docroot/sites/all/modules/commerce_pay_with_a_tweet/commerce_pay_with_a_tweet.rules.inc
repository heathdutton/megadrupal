<?php
/**
 * @file
 * File with rules hooks.
 *
 * @ingroup commerce_pay_with_a_tweet
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_pay_with_a_tweet_rules_condition_info() {
  return array(
    'commerce_pay_with_a_tweet_config_checked' => array(
      'group' => 'commerce_pay_with_a_tweet',
      'label' => t('Config is checked'),
      'module' => 'commerce_pay_with_a_tweet',
    ),
  );
}

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_pay_with_a_tweet_default_rules_configuration() {
  $configs = array();
  $rule = '{ "commerce_payment_commerce_pay_with_a_tweet" : {
    "LABEL" : "Tweet payment",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "Commerce Payment" ],
    "REQUIRES" : [ "commerce_pay_with_a_tweet", "commerce_payment" ],
    "ON" : [ "commerce_payment_methods" ],
    "IF" : [ { "commerce_pay_with_a_tweet_config_checked" : [] } ],
    "DO" : [
      { "commerce_payment_enable_commerce_pay_with_a_tweet" : {
          "commerce_order" : [ "commerce-order" ],
          "payment_method" : "commerce_pay_with_a_tweet"
        }
      }
    ]
  }
}';
  $configs['commerce_payment_commerce_pay_with_a_tweet'] = rules_import($rule);
  return $configs;
}

/**
 * Rules condition: Config is checked.
 */
function commerce_pay_with_a_tweet_config_checked() {
  $result = FALSE;

  // Get default tweet.
  $commerce_pay_with_a_tweet_default_tweet = variable_get('commerce_pay_with_a_tweet_default_tweet', FALSE);

  // If default tweet is defined.
  if (!empty($commerce_pay_with_a_tweet_default_tweet)) {

    // If tweet object can be loaded.
    $tweet = pay_with_a_tweet_load($commerce_pay_with_a_tweet_default_tweet);
    if (!empty($tweet)) {
      $result = TRUE;
    }
  }
  else {
    commerce_pay_with_a_tweet_administrator_set_message('Please check your config, no default tweet selected', array(), WATCHDOG_ERROR);
  }

  return $result;
}
