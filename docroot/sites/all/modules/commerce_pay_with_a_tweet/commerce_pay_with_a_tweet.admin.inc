<?php

/**
 * @file
 * Admin page callbacks for commerce_pay_with_a_tweet module.
 */

/**
 * Menu callback to configure commerce_pay_with_a_tweet module settings.
 */
function commerce_pay_with_a_tweet_config_form() {
  $form = array();

  // Get tweets list.
  $commerce_pay_with_a_tweet_option_list = _commerce_pay_with_a_tweet_get_options_list();

  // Select list to choose a tweet by default.
  $form['tweet_options']['commerce_pay_with_a_tweet_default_tweet'] = array(
    '#type' => 'select',
    '#title' => t('Default tweet'),
    '#options' => $commerce_pay_with_a_tweet_option_list,
    '#default_value' => variable_get('commerce_pay_with_a_tweet_default_tweet', reset($commerce_pay_with_a_tweet_option_list)),
    '#description' => t('Define default tweet.'),
    '#required' => TRUE,
  );

  // Add form to validate tweet_options.
  $form['#validate'][] = 'commerce_pay_with_a_tweet_config_form_validate';

  return system_settings_form($form);
}

/**
 * Validate commerce_pay_with_a_tweet_config_form.
 *
 * @param array $form
 *   Array to build the form.
 * @param array $form_state
 *   Current state of the form.
 */
function commerce_pay_with_a_tweet_config_form_validate($form, $form_state) {
  // To validate this form, user have to chose a specific tweet.
  if (empty($form_state['values']['commerce_pay_with_a_tweet_default_tweet'])) {
    form_set_error('commerce_pay_with_a_tweet_default_tweet', t('Please create pay_with_a_tweet instance if this select list is empty.'));
  }
}
