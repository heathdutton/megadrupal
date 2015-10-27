<?php

/**
 * @file
 * Logic for warden output
 */

/**
 * Page callback.
 *
 * Request that the site generates its site information data and sends it
 * to the configured Warden application.
 *
 * @return string
 *   Return JSON formatted status message, either OK or ERROR
 */
function warden_status_page() {
  drupal_page_is_cacheable(FALSE);

  try {
    _warden_update_warden();
    drupal_json_output(array(
      'data' => 'OK',
    ));
  }
  catch (Exception $e) {
    watchdog_exception('warden', $e);
    drupal_add_http_header('Status', '500 Internal Server Error');
    drupal_json_output(array('error' => 'Internal fault'));
  }
}

/**
 * Generate all the site's data for Warden.
 *
 * @return array
 *   The site's data as an array.
 */
function _warden_generate_site_data() {
  global $base_url;

  $system_modules = system_list('module_enabled');
  $system_themes = list_themes(TRUE);

  $res = array(
    'core' => array(),
    'contrib' => array(),
    'custom' => array(),
    'url' => $base_url,
  );

  // Go over themes.
  if (variable_get('warden_match_contrib', TRUE)) {
    foreach ($system_themes as $theme) {
      if (isset($theme->info['package']) && $theme->info['package'] == 'Core') {
        continue;
      }

      if (isset($theme->info['version'])) {
        $res['contrib'][$theme->name] = array("version" => $theme->info['version']);
      }
    }
  }

  // Go over modules.
  foreach ($system_modules as $module => $module_info) {
    $filename = $module_info->filename;

    // Match for custom modules.
    if (variable_get('warden_match_custom', TRUE)) {
      $regex = variable_get('warden_preg_match_custom', '{^sites\/([A-z,\.,\-]+)\/modules\/custom\/*}');
      if (preg_match($regex, $filename)) {
        // if this is part of a project, only set the project.
        if (isset($module_info->info['project'])) {
          $res['custom'][$module_info->info['project']] = array("version" => $module_info->info['version']);
        }
        else {
          $res['custom'][$module] = array("version" => $module_info->info['version']);
        }
      }
    }
    else {
      $res['custom'] = "disabled";
    }

    // Match for contrib modules.
    if (variable_get('warden_match_contrib', TRUE)) {
      $regex = variable_get('warden_preg_match_contrib', '{^sites\/([A-z,\.,\-]+)\/modules\/contrib\/*}');

      if (preg_match($regex, $filename)) {
        // if this is part of a project, only set the project.
        if (isset($module_info->info['project'])) {
          $res['contrib'][$module_info->info['project']] = array("version" => $module_info->info['version']);
        }
        else {
          $res['contrib'][$module] = array("version" => $module_info->info['version']);
        }
      }
    }
    else {
      $res['contrib'] = "disabled";
    }

    // Core modules.
    if (variable_get('warden_match_core', TRUE)) {
      if (strtolower($module_info->info['package']) == "core") {
        $res['core'][$module] = array("version" => $module_info->info['version']);
      }
    }
    else {
      $res['core'] = "disabled";
    }
  }

  // Include Drupal core version.
  if (!isset($res['core']['drupal'])) {
    $res['core']['drupal'] = array("version" => VERSION);
  }

  $res['site_name'] = variable_get('site_name', $base_url);

  // Sign the request with our local key and the timestamp.
  $res['key'] = _warden_get_local_token();
  $res['time'] = time();

  return $res;
}

/**
 * Update Warden with latest site data.
 *
 * @throws Exception
 *   If any problems occur.
 */
function _warden_update_warden() {
  $data = _warden_generate_site_data();
  warden_get_api()->postSiteData($data);
}