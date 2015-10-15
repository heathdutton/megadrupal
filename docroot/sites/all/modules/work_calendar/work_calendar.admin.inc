<?php

/**
 * @file
 * Work calendar admin settings UI.
 */

/**
 * Form builder for the site work calendar settings.
 */
function work_calendar_sitewide_settings() {
  $options = db_select('work_calendar', 'wc')
    ->fields('wc', array('name', 'label'))
    ->execute()
    ->fetchAllKeyed(0, 1);

  if (empty($options)) {
    $form['message'] = array(
      '#markup' => t("There's any work calendar created yet. Create one at <a href='!url'>admin/structure/work-calendars</a>.", array('!url' => url('admin/structure/work-calendars'))),
    );
  }

  $form['work_calendar_default'] = array(
    '#type' => 'select',
    '#title' => t('Default work calendar'),
    '#empty_value' => '',
    '#default_value' => variable_get('work_calendar_default', ''),
    '#options' => $options,
    '#disabled' => empty($options),
  );

  return system_settings_form($form);
}
