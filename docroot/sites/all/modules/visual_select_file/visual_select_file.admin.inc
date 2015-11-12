<?php

/**
 * Page callback for admin/config/media/visual-select-file.
 */
function visual_select_file_settings_form($form, &$form_state) {
  $form['visual_select_file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Core'),
  );
  $form['visual_select_file']['visual_select_file_public_is_public'] = array(
    '#type' => 'checkbox',
    '#title' => t('Public files are public'),
    '#default_value' => variable_get('visual_select_file_public_is_public', 0),
    '#description' => t("Drupal and FileField Sources add some weird access checks to public files. This checkbox will undo that: public is public, period.<br>Without this checkbox, you can't reference some files, like files uploaded by VSF Upload."),
  );

  return system_settings_form($form);
}
