<?php
/**
 * @file
 * The admin form for configuring the tweetpack module.
 *
 */

/**
 * Implements hook_admin().
 */
function tweetpack_admin() {
  $form = array();

  $form['tweetpack_enable_intents'] = array(
    '#type' => 'radios',
    '#title' => t('Enable Twitter Web Intents Site Wide?'),
    '#description' => t(l("Twitter Web Intents", "https://dev.twitter.com/docs/intents",  array('attributes' => array('target' => '_blank'))) . ' allow you to link to user profiles, share tweets and all other kinds of cool stuff.'),
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('tweetpack_enable_intents', 1),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}