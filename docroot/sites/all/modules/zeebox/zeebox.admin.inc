<?php
/**
 * @file
 * Administration settings functionality for the zeebox Everywhere module.
 */

/**
 * Page callback for admin/config/regional/zeebox.
 */
function zeebox_admin_settings() {
  $form['zeebox_region'] = array(
    '#type' => 'select',
    '#title' => t('zeebox Region'),
    '#description' => t("Please select the appropriate zeebox region for your site's audience.
      This will affect which zeebox region the TV Rooms and show data are loaded from."
    ),
    '#options' => _zeebox_allowable_regions(),
    '#default_value' => variable_get('zeebox_region', 'us'),
  );
  return system_settings_form($form);
}
