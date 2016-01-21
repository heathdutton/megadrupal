<?php
/**
 * @file keybase.admin.inc
 * Settings form for the keybase module.
 */

/**
 * Settings form callback.
 */
function keybase_settings_form() {
  $form['keybase_well_known'] = array(
    '#type' => 'textarea',
    '#title' => t('Keybase Well Known Content'),
    '#description' => t('Paste the output of <em>keybase prove web @url</em>. The content will be available for keybase verification at !url</a>.', array('@url' => url(NULL, array('absolute' => TRUE)), '!url' => url('keybase.txt', array('absolute' => TRUE)))),
    '#default_value' => variable_get('keybase_well_known', ''),
    '#rows' => 30,
  );
  return system_settings_form($form);
}
