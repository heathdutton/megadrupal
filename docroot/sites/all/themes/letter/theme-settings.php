<?php
/**
 * @file
 * Contains letter_form_system_theme_settings_alter
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 */
function letter_form_system_theme_settings_alter(&$form, &$form_state) {

  /*	Support for external webfont providers	*/
  $form['webfont'] = array(
    '#type'			 =>	'fieldset',
    '#title'		 =>	t('Webfont Settings'),
  );
  $form['webfont']['letter_webfont_css'] = array(
    '#type'          => 'textfield',
    '#title'         => t('CSS url'),
    '#default_value' => theme_get_setting('letter_webfont_css'),
    '#description'   => t('Use this field to easily link to an external css, eg. from <a href="http://google.com/webfonts" target="_blank">Google Webfonts</a>.'),
  );
  $form['webfont']['letter_webfont_js'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Javascript url'),
    '#default_value' => theme_get_setting('letter_webfont_js'),
    '#description'   => t('Use this field to easily link to an external javascript, eg. from <a href="http://webfonts.fonts.com" target="_blank">Fonts.com</a>.'),
  );

  /*	Remove some of the base theme's settings.	*/
  unset($form['themedev']['zen_layout']);

  /* We are editing the $form in place, so we don't need to return anything. */
}
