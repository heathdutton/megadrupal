<?php
// $Id: sentry_client_xmlrpc.inc,v 1.1.6.5 2010/09/15 09:09:18 nevergone Exp $

/**
 * @file
 * XML-RPC callbacks, these will be loaded only when someone hits the xmlrpc.php
 */

/*
 * Callback to check the core status, key validation.
 */
function xmlc_sentry_client_core_status($key) {
  if (sentry_client_validate($key)) {
    return 1;
  }
  else { // invalid key
    return -5;
  }
}

/**
 * Callback to return the list of installed modules.
 */
function xmlc_sentry_client_update_status($key) {
  if (sentry_client_validate($key)) {
    if ($available = sentry_client_installed_modules()) {
      return $available;
    }
    else {
      return array('error' => t('No available modules.'));
    }
  }
}

function xmlc_sentry_client_hash($key) {
  if (sentry_client_validate($key)) {
    $result = db_query('SELECT filename FROM {system} WHERE status = :status_constant', array(':status_constant' => 1));
    foreach ($result as $file) {
      $hashes[$file->filename] = md5_file($file->filename);
    }
    return $hashes;
  }
}

function xmlc_sentry_client_requirements_status($key) {
  if (sentry_client_validate($key)) {
    include_once DRUPAL_ROOT . '/includes/install.inc';
    drupal_load_updates();

    // Check run-time requirements and status information.
    $requirements = module_invoke_all('requirements', 'runtime');
    usort($requirements, '_system_sort_requirements');
    return $requirements;

    /* TODO move it to server

    $severities = array(
      REQUIREMENT_INFO => 'info',
      REQUIREMENT_OK => 'ok',
      REQUIREMENT_WARNING => 'warning',
      REQUIREMENT_ERROR => 'error',
    );

    // Add default severity levels
    foreach ($requirements as &$item) {
      $item['severity'] = isset($requirement['severity']) ?
        (int)$requirement['severity'] : 0;
    }
    return $requirements;
     */
  }
}

function xmlc_sentry_client_offline_status($key) {
  if (sentry_client_validate($key)) {
    return variable_get('maintenance_mode', 0);
  }
}

function xmlc_sentry_client_seo_variables($key) {
  if (sentry_client_validate($key)) {
    $seo_variables = array('clean_url' => 1, 'googleanalytics_account' => '');
    return sentry_client_variables($seo_variables);
  }
}

function xmlc_sentry_client_performance_variables($key) {
  if (sentry_client_validate($key)) {
    $variables = array('block_cache' => 0, 'cache' => 0, 'preprocess_css' => 0, 'preprocess_js' => 0, 'page_compression' => 0);
    return sentry_client_variables($variables);
  }
}

/**
 * XML-RPC callback.
 *
 * Return an array of requested variable values.
 */
function sentry_client_variables($variables) {
  $remote_variables = array();
  foreach ($variables as $name => $value) {
    $remote_variables[$name] = variable_get($name, $value);
  }
  return $remote_variables;
}

/**
 * Validate the request using the client-side key.
 *
 * If either key, or client key is empty we abort, safety first!
 *
 * @param
 *   $key Remote key string. It can be NULL in case server has not set anything.
 * @return
 *   0 if keys dont match or are empty, 1 if keys match.
 */
function sentry_client_validate($key = '') {
  if ($key == '') {
    return 0;
  }
  $site_key = variable_get('sentry_client_key', '');
  if ($site_key == '') {
    return 0;
  }
  return (!strcmp($key, $site_key)); // Happiness and joy all around.
}

function sentry_client_installed_modules() {
  sentry_client_info_list($projects, system_rebuild_module_data(), 'module');
  sentry_client_info_list($projects, system_rebuild_theme_data(), 'theme');
  return $projects;
}

/**
 * Rewrite of _update_info_list so we don't depend on update status.
 * Populate an array of project data.
 */
function sentry_client_info_list(&$projects, $list, $project_type) {
  foreach ($list as $file) {
    if (empty($file->status)) {
      // Skip disabled modules or themes.
      continue;
    }

    // Skip if the .info file is broken.
    if (empty($file->info)) {
      continue;
    }

    // If the .info doesn't define the 'project', try to figure it out.
    if (!isset($file->info['project'])) {
      $file->info['project'] = sentry_client_get_project_name($file);
    }

    // If we still don't know the 'project', give up.
    if (empty($file->info['project'])) {
      continue;
    }

    $project_name = $file->info['project']; // machine friendly name
    $project_title = ('drupal' == $project_name) ? 'Drupal' : $file->info['name'];
    if (!isset($projects[$project_name])) {
      // Only process this if we haven't done this project, since a single
      // project can have multiple modules or themes.
      $projects[$project_name] = array(
        // Only important for custom stuff, real title come from the XML
        'title' => $project_title, // The user-friendly name
        'name' => $project_name,
        'version' => $file->info['version'],
        'description' => $file->info['description'],
        'project_type' => $project_name == 'drupal' ? 'core' : $project_type,
        'project_status_url' => $file->info['project status url'],
        'datestamp' => $file->info['datestamp'],
      );
    }
    else {
      $projects[$project_name]['includes'][$file->name] = $file->info['name'];
      $projects[$project_name]['info']['_info_file_ctime'] = max($projects[$project_name]['info']['_info_file_ctime'], $file->info['_info_file_ctime']);
    }
  }
}

/**
 * Given a $file object (as returned by system_get_files_database()), figure
 * out what project it belongs to.
 *
 * @see system_get_files_database()
 */
function sentry_client_get_project_name($file) {
  $project_name = '';
  if (isset($file->info['project'])) {
    $project_name = $file->info['project'];
  }
  elseif (isset($file->info['package']) && (strpos($file->info['package'], 'Core -') !== FALSE)) {
    $project_name = 'drupal';
  }
  elseif (in_array($file->name, array('bluemarine', 'chameleon', 'garland', 'marvin', 'minnelli', 'pushbutton'))) {
    // Unfortunately, there's no way to tell if a theme is part of core,
    // so we must hard-code a list here.
    $project_name = 'drupal';
  }
  return $project_name;
}
