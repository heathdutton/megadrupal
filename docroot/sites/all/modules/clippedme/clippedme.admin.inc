<?php

/**
 * @file
 * Admin interface for Clipped.me (clippedme) module.
 */

/**
 * Implements hook_form().
 *
 * Configuration form for Clipped.me module.
 *
 * @ingroup forms
 */
function clippedme_configure() {
  $form = array();
  $form['clippedme_selector'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Target CSS class'),
    '#size'          => 10,
    '#field_prefix'  => '.',
    '#default_value' => variable_get('clippedme_selector', 'clippedme'),
    '#required'      => TRUE,
    '#description'   => t("Previews will be attached to links with this class. Leading '.' is included automatically."),
  );
  $options = array(
    'tip-clippedme'    => t('White-and-gray (default)'),
    'tip-green'        => t('Green'),
    'tip-twitter'      => t('Black'),
    'tip-yellowsimple' => t('Yellow'),
    'tip-custom'       => t('None (I will theme it myself)'),
  );
  $form['clippedme_theme'] = array(
    '#type'          => 'select',
    '#title'         => t('Theme'),
    '#options'       => $options,
    '#default_value' => variable_get('clippedme_theme', 'tip-clippedme'),
    '#required'      => TRUE,
  );
  return system_settings_form($form);
}
