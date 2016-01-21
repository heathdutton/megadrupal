<?php
/**
 * @file
 * Provides administration functions for the auto_retina module.
 *
 * @ingroup auto_retina
 * @{
 */

/**
 * Form builder. Configure settings for auto_retina.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function auto_retina_admin_settings($form, &$form_state) {

  $form['auto_retina_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Retina filename suffix'),
    '#description' => t('This suffix appears at the end of a filename, before the dot+extension to indicate it is the retina version of an image.'),
    '#default_value' => variable_get('auto_retina_suffix', AUTO_RETINA_SUFFIX),
    '#required' => TRUE,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['auto_retina_regex'] = array(
    '#type' => 'textfield',
    '#title' => t('Retina filename regex'),
    '#description' => t('Enter a regex expression to use for determining if an url is retina.  The token <code>[suffix]</code> may be used to dynamically populate the setting from above. You may omit start/end delimiters.'),
    '#default_value' => variable_get('auto_retina_regex', AUTO_RETINA_REGEX),
    '#required' => TRUE,
  );

  $form['advanced']['auto_retina_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include the javascript settings <code>Drupal.settings.autoRetina</code> on every page?'),
    '#default_value' => variable_get('auto_retina_js', AUTO_RETINA_JS),
  );

  return system_settings_form($form);
}
