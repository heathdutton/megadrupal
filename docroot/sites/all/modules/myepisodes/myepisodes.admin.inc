<?php

/**
 * @file
 * My Episode global administration screens.
 */

/**
 * Global configuration form.
 */
function myepisodes_admin_services_settings($form, &$form_state) {
  $form[MYEPISODES_REFRESH_DELAY_VAR] = array(
    '#type' => 'select',
    '#title' => t("Refresh my list"),
    '#options' => array(
       43200 => t("Twice a day"),
       86400 => t("Once a day"),
      172800 => t("Every two days"),
      604800 => t("Once a week"),
    ),
    '#default_value' => variable_get(MYEPISODES_REFRESH_DELAY_VAR, MYEPISODES_REFRESH_DELAY_DEFAULT),
  );
  return system_settings_form($form);
}
