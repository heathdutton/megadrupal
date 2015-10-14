<?php

/**
 * @file
 * Admin page callbacks for the imageproperty check module.
 */

/**
 * Creates form to fill in the maximum memory size limit of each preset in KB.
 *
 * Menu callback for hook_menu()
 *   Returns a form with list of presets which stores max memory size of each.
 */
function imageproperty_check_admin_form() {
  $form = array();
  $form['html'] = array(
    '#type' => 'markup',
    '#markup' => t('Enter the maximum size (in <strong>Kilobytes</strong>) an image style could have. Default is 100KB.'),
  );
  $list_image_style = image_styles();
  foreach ($list_image_style as $value) {
    $form['imageproperty_check_type' . $value['name']] = array(
      '#type' => 'textfield',
      '#title' => $value['label'],
      '#default_value' => variable_get('imageproperty_check_type' . $value['name'], 100),
      '#size' => 50,
    );
  }
  return system_settings_form($form);
}

/**
 * Form to configure pager.
 */
function imageproperty_check_pager_config_form() {
  $form = array();
  $form['imageproperty_check_pager'] = array(
    '#type' => 'textfield',
    '#title' => 'Pager configuration',
    '#description' => t("Number of images to be displayed on a page"),
    '#default_value' => variable_get('imageproperty_check_pager', 10),
  );
  $form['imageproperty_check_cron'] = array(
    '#type' => 'textfield',
    '#title' => 'Cron hour configuration',
    '#description' => t("Number of hours after which mail should be sent"),
    '#default_value' => variable_get('imageproperty_check_cron', 10),
  );
  return system_settings_form($form);
}
