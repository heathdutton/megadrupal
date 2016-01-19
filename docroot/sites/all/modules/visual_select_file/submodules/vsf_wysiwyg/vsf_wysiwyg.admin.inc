<?php

/**
 * Implements hook_form_FORM_ID_alter() for visual_select_file_settings_form().
 */
function vsf_wysiwyg_form_visual_select_file_settings_form_alter(&$form, &$form_state) {
  $form['vsf_wysiwyg'] = array(
    '#type' => 'fieldset',
    '#title' => t('VSF WYSIWYG'),
  );
  $form['vsf_wysiwyg']['visual_select_file_default_style'] = array(
    '#type' => 'select',
    '#title' => t('Default selected formatter in WYSIWYG popup'),
    '#options' => _vsf_wysiwyg_default_image_style_options(),
    '#default_value' => variable_get('visual_select_file_default_style', ''),
  );

  $form['vsf_wysiwyg']['vsf_wysiwyg_check_unchanged_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t("Always parse text-ish fields to find VSF WYSIWYG images, even when they haven't changed"),
    '#default_value' => variable_get('vsf_wysiwyg_check_unchanged_fields', VSF_DEFAULT_CHECK_UNCHANGED_FIELDS),
    '#description' => t("File Usage was implemented AFTER 7.x-1.5, so even though always parsing is inefficient, all images included before that, should be parsed and saved. I can't do that manually, so this option is safe 'for next time'."),
  );
}
