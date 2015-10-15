<?php

/**
 * @file
 * Administrative settings for the Emphasis module.
 */

/**
 * Page callback for Emphasis administrative form page.
 */
function emphasis_admin_settings_form() {

  $form = array();

  $form['emphasis_selector'] = array(
    '#title' => t('jQuery paragraph selector'),
    '#type' => 'textfield',
    '#description' => t('You must use a jQuery compatible selector that selects paragraph tags in some way'),
    '#default_value' => variable_get('emphasis_selector', '.node p'),
  );

  $form['emphasis_key_code'] = array(
    '#title' => t('Trigger key code'),
    '#type' => 'textfield',
    '#description' => t('The numeric key code of the keyboard key that will trigger Emphasis.Default key is the shift key, with key code of 16'),
    '#default_value' => variable_get('emphasis_key_code', '16'),
    '#element_validate' => array('element_validate_number'),
  );

  $form['emphasis_key_code_count'] = array(
    '#title' => t('Trigger key code count'),
    '#type' => 'textfield',
    '#description' => t('The number of times that the trigger key must be pressed to trigger Emphasis.'),
    '#default_value' => variable_get('emphasis_key_code_count', '2'),
    '#element_validate' => array('element_validate_number'),
  );

  $form['emphasis_scroll'] = array(
    '#title' => t('Scroll to emphasized text'),
    '#type' => 'checkbox',
    '#description' => t('Scroll to the first highlighted paragraph on page load'),
    '#default_value' => variable_get('emphasis_scroll', TRUE),
  );

  if (module_exists('sharethis')) {
    $form['emphasis_update_sharethis'] = array(
      '#title' => t('Update ShareThis links with Emphasis hash'),
      '#type' => 'checkbox',
      '#description' => t('When enabled, ShareThis links will be updated via JS when text is selected via Emphasis.'),
      '#default_value' => variable_get('emphasis_update_sharethis', TRUE),
    );
  }
  else {
    $form['emphasis_sharethis_message'] = array(
      '#prefix' => "<div>",
      '#markup' => t('Install the ShareThis module to ingrate with social media services'),
      '#suffix' => '</div>',
    );
  }

  $form['emphasis_update_og_metatags'] = array(
    '#title' => t('Update og:url Open Graph metatag with Emphasis Hash'),
    '#type' => 'checkbox',
    '#description' => t('When enabled, Open Graph metatags will be updated via JS with the Emphasis hash.'),
    '#default_value' => variable_get('emphasis_update_og_metatags', TRUE),
  );

  $form['emphasis_update_anchors'] = array(
    '#title' => t('Update on-page links with Emphasis Hash'),
    '#type' => 'checkbox',
    '#description' => t('When enabled, all links pointing to the current page will be updated via JS with the Emphasis hash.'),
    '#default_value' => variable_get('emphasis_update_anchors', TRUE),
  );

  return system_settings_form($form);
}
