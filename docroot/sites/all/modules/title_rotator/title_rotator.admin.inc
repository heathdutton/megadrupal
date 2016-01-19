<?php

/**
 * @file
 * title_rotator.admin.inc admin configuration settings for Title Rotator.
 */

/**
 * Menu callback for admin configuration admin/config/system/title_rotator.
 */
function title_rotator_admin_configure() {

  $form['title_rotator_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site name and Slogan Rotator configuration'),
    '#description' => t("Enter <em>site name</em> and <em>site slogan</em> alternatives, to be rotated with every click. If both have the same number of alternatives, they'll remain in sync. Do not use special characters like ampersands."),
  );
  $form['title_rotator_config']['title_rotator_site_names'] = array(
    '#title'=> t('List of alternative site names'),
    '#type' => 'textfield',
    '#size' => 511,
    '#maxlength' => 511,
    '#default_value' => variable_get('title_rotator_site_names'),
    '#description' => t('Separate alternatives by a |. Include your current site name "%site_name", if you wish.', array(
      '%site_name' => variable_get('site_name', ''))),
  );
  $form['title_rotator_config']['title_rotator_site_slogans'] = array(
    '#title' => t('List of alternative site slogans'),
    '#type' => 'textfield',
    '#size' => 511,
    '#maxlength' => 511,
    '#default_value' => variable_get('title_rotator_site_slogans'),
    '#description' => t('Separate alternatives by a |. Include your current site slogan "%site_slogan", if you wish.', array(
      '%site_slogan' => variable_get('site_slogan', ''))),
  );
  return system_settings_form($form);
}
