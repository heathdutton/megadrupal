<?php
/**
 * @file
 * theme-settings.php provides the custom theme settings
 *
 * Provides the checkboxes for the CSS overrides functionality
 * as well as the serif/sans-serif style option.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 */
function elementary_form_system_theme_settings_alter(&$form, $form_state) {
  $form['css_overrides'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS Overrides'),
  );
  $form['css_overrides']['remove_all_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only use CSS from this theme'),
    '#description' => t('Removes all CSS files provided by core & contrib modules'),
    '#default_value' => theme_get_setting('remove_all_css'),
  );

  $form['font_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fonts'),
  );
  $form['font_styles']['font_serif'] = array(
    '#type' => 'radios',
    '#title' => t('Use a serif or sans-serif font?'),
    '#default_value' => theme_get_setting('font_serif'),
    '#options' => array('serif' => '<span class="serif">' . t('Serif') . '</span>', 'sans' => t('Sans-serif')),
  );
}
