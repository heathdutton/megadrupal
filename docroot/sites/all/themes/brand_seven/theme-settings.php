<?php

/**
 * @file
 * Theme setting callbacks for the brand seven theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * $form
 *   The form.
 * $form_state
 *   The form state.
 */
function brand_seven_form_system_theme_settings_alter(&$form, &$form_state) {
  $form['brand_seven'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brand Seven'),
  );
  $form['brand_seven']['brand_seven_page_background'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Background Colour'),
    '#description' => t('For the main content area and a few others things.<br />Best to use a light colour e.g.(#ECECEC)'),
    '#default_value' => theme_get_setting('brand_seven_page_background'),
  );
  $form['brand_seven']['brand_seven_branding_light'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Light Branding'),
    '#description' => t('Used for text in the top bar and a few others things.<br />Best to use a light colour e.g.(#FFFFFF)'),
    '#default_value' => theme_get_setting('brand_seven_branding_light'),
  );
  $form['brand_seven']['brand_seven_branding_dark'] = array(
    '#type' => 'textfield',
    '#size' => 10,
    '#title' => t('Dark Branding'),
    '#description' => t('Used for background of the top bar and a few others things.<br />Best to use a dark colour e.g.(#000000)'),
    '#default_value' => theme_get_setting('brand_seven_branding_dark'),
  );
  $form['brand_seven']['brand_seven_hide_status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Status'),
    '#description' => t('Hide the status dropdown on the find content page<br />Not selecting an option below overwrites this'),
    '#default_value' => theme_get_setting('brand_seven_hide_status'),
  );
  $form['brand_seven']['brand_seven_hide_update_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Update Options'),
    '#description' => t('Hide the update options dropdown on the find content page<br />Not selecting an option below overwrites this'),
    '#default_value' => theme_get_setting('brand_seven_hide_update_options'),
  );
  $form['brand_seven']['brand_seven_disable_publishing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Publishing'),
    '#description' => t('All content will be published when saved'),
    '#default_value' => theme_get_setting('brand_seven_disable_publishing'),
  );
  $form['brand_seven']['brand_seven_disable_promoted'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Promoted'),
    '#description' => t('Removes abaility to promote site to front page'),
    '#default_value' => theme_get_setting('brand_seven_disable_promoted'),
  );
  $form['brand_seven']['brand_seven_disable_sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Sticky'),
    '#description' => t('Removes abaility to create sticky content'),
    '#default_value' => theme_get_setting('brand_seven_disable_sticky'),
  );
}
