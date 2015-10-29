<?php

/**
 * @file
 * Theme setting callbacks for the center theme.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * @param $form
 *   The form.
 * @param $form_state
 *   The form state.
 */
function center_form_system_theme_settings_alter(&$form, $form_state) {
  $form['dev_mode'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Development Mode'),
    '#default_value' => theme_get_setting('dev_mode'),
    '#description'   => t("Enable Center's development tools"),
  );

  $form['nuke_fields'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Nuke Field Markup'),
    '#default_value' => theme_get_setting('nuke_fields'),
    '#description'   => t("Remove all markup from fields. WARNING: This will also remove labels."),
  );
}

