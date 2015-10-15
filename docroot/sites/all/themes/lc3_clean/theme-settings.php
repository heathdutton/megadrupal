<?php
// vim: set ts=2 sw=2 sts=2 et:

/**
 * @file
 * Custom theme settings.
 */

/**
 * Add custom theme settings.
 *
 * @param array &$form
 *   The form.
 * @param array &$form_state
 *   The form state.
 *
 * @hook   form_FORM_ID_alter
 * @return void
 */
function lc3_clean_form_system_theme_settings_alter(array &$form, array &$form_state) {

  $form['social_links'] = array(

    '#type'              => 'fieldset',
    '#title'             => t('Social links'),
    '#description'       => t('Enable or disable links to your accounts in social network services'),
    '#weight'            => -10,

    'theme_social_link_facebook' => array(
      '#type'          => 'textfield',
      '#title'         => t('Facebook business page'),
      '#default_value' => theme_get_setting('theme_social_link_facebook'),
      '#description'   => t('Name of your Facebook business page'),
    ),

    'theme_social_link_twitter' => array(
      '#type'          => 'textfield',
      '#title'         => t('Twitter account'),
      '#default_value' => theme_get_setting('theme_social_link_twitter'),
      '#description'   => t('Name of your Twitter account'),
    ),

  );
}
