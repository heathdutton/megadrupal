<?php

/**
 * @file
 * Theme setting callbacks for the frontmap theme.
 */

/**
 * Implements hook_form_system_theme_settings_alter().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function frontmap_form_system_theme_settings_alter(&$form, &$form_state, $form_id = NULL)  {
  // Work-around for a core bug affecting admin themes. See issue #943212.
  if (isset($form_id)) {
    return;
  }

  $maps = openlayers_map_options();

  $form['frontmap'] = array(
    '#type' => 'select',
    '#title' => t('Frontmap'),
    '#options' => $maps,
    '#default_value' => theme_get_setting('frontmap'),
    '#description' => t('Choose an OpenLayers map which will be used on the frontpage.'),
  );

  $form['frontmap_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display Content'),
    '#default_value' => theme_get_setting('frontmap_content'),
    '#description' => t('Display content on the frontpage.'),
  );
}
