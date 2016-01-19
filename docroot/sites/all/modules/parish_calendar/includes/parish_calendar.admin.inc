<?php

/**
 * @file
 *
 * Parish calendar configuration form.
 */

function parish_calendar_configuration_form() {
  $form['description'] = array(
    '#markup' => '<p>' . t('You can manage settings for your parish calendar on this page.') . '</p>',
  );

  $form['empty'] = array(
    '#markup' => '<p>' . t('There are currently no configurable options available.') . '</p>',
  );

  return system_settings_form($form);
}
