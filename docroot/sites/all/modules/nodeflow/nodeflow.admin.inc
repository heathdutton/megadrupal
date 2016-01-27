<?php
/**
 * @file
 * Administrative page callbacks for the nodeflow module.
 */

/**
 * General configuration form for controlling the nodeflow behaviour.
 */
function nodeflow_admin() {
  $form = array();

  $form['nodeflow_update_created_time'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update the created time of the node to match the published time.'),
    '#default_value' => variable_get('nodeflow_update_created_time', '0'),
    '#return_value' => 1,
    '#description' => t("If checked, this will update the created time of the node to the time the node was published."),
  );

  return system_settings_form($form);
}
