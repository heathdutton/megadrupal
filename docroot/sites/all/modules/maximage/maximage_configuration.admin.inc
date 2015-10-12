<?php
/**
 * @file
 * Administrative page callbacks for maximage module.
 */

/**
 * Configuration settting form.
 */
function maximage_configuration_setting_form($form, &$form_state) {

  $maximage_jspathmaximage = variable_get('jspathmaximage');
  $maximage_jspathcycle = variable_get('jspathcycle');
  $maximage_csspath = variable_get('csspath');

  $form['jspathmaximage'] = array(
    '#type' => 'textfield',
    '#title' => 'Path to Javascript maximage libraries',
    '#description' => t('Path to javascript maximage library. No / at the beginning of path!'),
    '#default_value' => $maximage_jspathmaximage,
  );
  $form['jspathcycle'] = array(
    '#type' => 'textfield',
    '#title' => 'Path to Javascript cycle libraries',
    '#description' => t('Path to javascript cycle library. No / at the beginning of path!'),
    '#default_value' => $maximage_jspathcycle,
  );
  $form['csspath'] = array(
    '#type' => 'textfield',
    '#title' => 'Path to css',
    '#description' => t('Path to css, change if you put css in other directories. No / at the beginning of path!'),
    '#default_value' => $maximage_csspath,
  );
  return system_settings_form($form, TRUE);
}

/**
 * Validation form.
 */
function maximage_configuration_setting_form_validate($form, &$form_state) {

  $jspathmaximage = $form_state['values']['jspathmaximage'];
  $jspathcycle = $form_state['values']['jspathcycle'];
  $csspath = $form_state['values']['csspath'];

  if ((isset($jspathmaximage[0]) && $jspathmaximage[0] == '/') || (isset($jspathcycle[0]) && $jspathcycle[0] == '/') || (isset($csspath[0]) &&  $csspath[0] == '/')) {
    form_set_error("Path", t("Path shouldn't start with a /"));
  }
  if (!is_dir($jspathmaximage)) {
    form_set_error("Path", t("The selected path to maximage.js doesn't exist."));
  }
  if (!is_dir($jspathcycle)) {
    form_set_error("Path", t("The selected path to cycle.js doesn't exist."));
  }
  if (!is_dir($csspath)) {
    form_set_error("Path", t("The selected css path doesn't exist."));
  }
}
