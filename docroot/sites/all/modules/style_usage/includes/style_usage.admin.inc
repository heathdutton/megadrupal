<?php

/**
 * @file
 *   Style usage administration form
 *
 *  @developers:
 *    Gabor Szanto <hello@szantogabor.com>
 *    http://szantogabor.com
 *
 */

/**
 * Settings form builder.
 */
function style_usage_admin_form() {
  $form = array();
  $form['style_usage_portrait'] = array(
    '#type' => 'textfield',
    '#title' => t('Image for portrait picture'),
    '#default_value' => variable_get('style_usage_portrait'),
    '#description' => t('Use relative path from the private file system path.'),
  );
  $form['style_usage_landscape'] = array(
    '#type' => 'textfield',
    '#title' => t('Image for landscape picture'),
    '#default_value' => variable_get('style_usage_landscape'),
    '#description' => t('Use relative path from the private file system path.'),
  );
  $form['style_usage_collapsed_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show style fieldsets collapsed'),
    '#default_value' => variable_get('style_usage_collapsed_default', 0),
    '#description' => t('Displays the style fieldsets collapsed by default on the style usage details page.'),
  );
  return system_settings_form($form);
}
