<?php

/**
 * @file
 * Dominion theme admin file.
 */

function dominion_theme_admin_settings() {
  $form = array();

  $form['rebuild'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rebuild CSS files'),
    '#description' => t('With this button you can rebuild the CSS files for all subsites. You may need to run this after updating modules or themes.')
  );
  $form['rebuild']['rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild CSS files'),
  );

  $options = array();
  $res = db_select('system', 's')
  ->fields('s', array('name'))
  ->condition('type', 'theme')
  ->condition('status', 1)
  ->orderBy('name', 'ASC')
  ->execute()
  ->fetchAll();

  foreach ($res as $theme) {
    $options[$theme->name] = $theme->name;
  }

  $form[DOMINION_THEME_VAR_THEMES] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available themes'),
    '#options' => $options,
    '#default_value' => variable_get(DOMINION_THEME_VAR_THEMES, array()),
    '#description' => t('Themes to choose from when creating a subsite.'),
  );

  $form[DOMINION_THEME_VAR_DEFAULT_THEME] = array(
    '#type' => 'select',
    '#title' => t('Default theme'),
    '#options' => $options,
    '#default_value' => variable_get(DOMINION_THEME_VAR_DEFAULT_THEME, ''),
    '#description' => t('Default theme for new subsites.'),
  );

  $options = array('' => t('Disable'));
  if (module_exists('imagecache')) {
    $options = array();
    $res = db_select('imagecache_preset', 'icp')
    ->fields('icp', array('presetname'))
    ->ordeBy('presetname', 'ASC')
    ->execute()
    ->fetchAll();

    foreach ($res as $row) {
      $options[$row->presetname] = $row->presetname;
    }
  }
  $form[DOMINION_THEME_VAR_IMAGECACHE] = array(
    '#type' => 'select',
    '#title' => t('Imagecache preset for thumbnails'),
    '#options' => $options,
    '#default_value' => variable_get(DOMINION_THEME_VAR_IMAGECACHE, ''),
    '#description' => t('Used for thumbnails of theme files.'),
  );

  $form['#submit'][] = 'dominion_theme_admin_settings_submit';

  return system_settings_form($form);
}

function dominion_theme_admin_settings_validate($form, &$form_state) {
  $enabled_themes = array();
  foreach ($form_state['values'][DOMINION_THEME_VAR_THEMES] as $theme) {
    if ($theme) {
      $enabled_themes[] = $theme;
    }
  }
  if (!in_array($form_state['values'][DOMINION_THEME_VAR_DEFAULT_THEME], $enabled_themes)) {
    form_set_error(DOMINION_THEME_VAR_DEFAULT_THEME, t('The default theme must be available.'));
  }
}

function dominion_theme_admin_settings_submit($form, &$form_state) {

  // If 'rebuild css' buttons is pressed.
  if (strstr($form_state['clicked_button']['#id'], 'edit-rebuild')) {

    // select domain_id from domain where dominion_domain = 1
    $query = db_select('domain', 'd');
    $query->fields('d', array('domain_id'));
    $query->condition('d.dominion_domain', 1);
    $result = $query->execute();
    $res = $result->fetchAll();

    // Loop for every dominon domain.
    foreach ($res as $rec) {
      if ($subsite = dominion_get_info($rec->domain_id)) {
        // Generate the CSS for a domain.

        $theme_data = _dominion_theme_apply_configuration($subsite->domain_id, $subsite->theme, $subsite->theme_config);
        if ($theme_data) {
          extract($theme_data);
        }

        else {
          $stylesheets = array();
          $files = array();
        }
        $stylesheets_serialized = serialize($stylesheets);
        $files_serialized = serialize($files);

        db_update('dominion_theme')
        ->fields(array(
            'stylesheets' => $stylesheets_serialized,
            'files' => $files_serialized,
        ))
        ->condition('domain_id', $subsite->domain_id)
        ->execute();
        cache_clear_all("info:" . $subsite->domain_id, 'cache_dominion');
      }
    }
  }
}
