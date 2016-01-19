<?php
/**
 * @file
 * Administrative page callbacks for the media desk module.
 */

/**
 * General configuration form for controlling the media desk behaviour.
 */
function media_desk_admin() {
  $form = array();

  $form['media_desk_clone_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clone files on reuse'),
    '#default_value' => variable_get('media_desk_clone_file', '0'),
    '#return_value' => 1,
    '#description' => t("If checked, re-using an image will automatically clone the file. This is to ensure that images can be reused with different captions and crop parameters per instance."),
  );

  return system_settings_form($form);
}
