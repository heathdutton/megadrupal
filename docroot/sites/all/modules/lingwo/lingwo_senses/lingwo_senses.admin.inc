<?php

/**
 * @file
 * For the admin pages.
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function lingwo_senses_admin_settings_form($form, &$form_state) {
  $settings = &LingwoSensesSettings::get();
  $form = array();

  $form[$settings->name('difference')] = array(
    '#type' => 'radios',
    '#title' => t('Sense differentiation'),
    '#description' => t('Controls how to tell senses apart'),
    '#options' => array(
      'definition' => t('Every sense gets a definition'),
      'always' => t('Every sense gets a few words describing how it is different than the other senses'),
      'morethanone' => t('Every sense gets a few words describing how it is different than the other senses, <strong>but only if there are two or more senses</strong>'),
    ),
    '#default_value' => $settings->difference,
  );

  $form[$settings->name('example')] = array(
    '#type' => 'radios',
    '#title' => t('Examples'),
    '#description' => t('The style of examples for each sense'),
    '#options' => array(
      'single' => t('Single line example.'),
      'multiple' => t('Multiple line example.  Can be used for multiple examples.'),
      'none' => t('No example'),
    ),
    '#default_value' => $settings->example,
  );

  // merge in the relationship values from our hook_lingwo_language_form().
  $form = array_merge($form, _lingwo_language_form('lingwo_senses_lingwo_language_form'));

  return system_settings_form($form);
}

