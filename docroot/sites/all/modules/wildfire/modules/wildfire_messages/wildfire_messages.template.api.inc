<?php
/**
 * @file
 *  wildfire_messages.template.api.inc
 *
 * API for template management
 *
 * @author Craig Jones <craig@tiger-fish.com>
 *
 * Code derived from Wildfire 1:
 * @author Chris Cohen <chris.cohen@tiger-fish.com>
 */

/**
 * Gets a specific template.
 *
 * @param $name string
 *    The name of the template to be returned.
 * @return array
 *    FALSE if the template does not exist. Otherwise, an associative array with
 *    the following keys:
 *    - name:         The name of the template.
 *    - title:        The human-readable title of the template.
 *    - description:  A textual description of the template.
 */
function wildfire_template_get($name) {
  $templates = wildfire_templates_get();

  return !empty($templates[$name]) ? $templates[$name] : FALSE;
}

/**
 * Get all the possible paths where templates might reside.
 *
 * The order in which items are presented in this array controls the priority of
 * template folders, so the first item in the array is the most important path.
 *
 * @param string $default_theme
 *  The name of the default theme to use when checking for mailtemplates.
 *
 * @return array
 *    An array with each of the paths expressed as internal Drupal paths.
 */
function wildfire_templates_get_paths($default_theme) {
  return array(
    'sites/all/mailtemplates',
    conf_path() . '/mailtemplates',
    drupal_get_path('theme', $default_theme) . '/mailtemplates',
    drupal_get_path('module', 'wildfire') . '/mailtemplates'
  );
}

/**
 * Gets the current settings for a given template
 *
 * @param string $template
 *  Machine name of the template
 * @param int $delta
 *  Delta for the set of settings to obtain
 *
 * @return array
 *  Associative array of settings. keyed by setting name, or FALSE on failure
 */
function wildfire_templates_get_settings($template, $delta) {

  $query = db_select('wildfire_templates_settings', 'wts')
    ->fields('wts', array(
      'data'
  ));
  $query->condition('template', $template, '=');
  $query->condition('delta', $delta, '=');
  $result = $query->execute()->fetchField(0);

  if (!empty($result)) {
    $result = unserialize($result);
    if (!is_array($result)) {
      return FALSE;
    }
    return $result;
  }
  else {
    return FALSE;
  }

}


/**
 * Gets all regions (repeaters, snippets, etc) that are currently in existence.
 *
 * @param string $template
 *    The name of the template for which to get regions. If omitted, will return
 *    regions for ALL templates.
 * @return array
 *    An array where the keys are the region names, and the values are arrays as
 *    follows:
 *    - name: The human-readable name of the region
 *    - type: The type of the region such as repeater or snippet.
 */
function wildfire_template_get_regions($template = NULL) {
  return module_invoke_all('wildfire_template_get_regions', $template);
}

/**
 * Get the array of templates from the static cache, or refresh it if not in
 * cache.
 *
 * @param $reset bool
 *    If TRUE, ignores the statically cached value and forces a reset.
 * @return array
 *    An array of mail templates.
 */
function wildfire_templates_get($reset = FALSE) {

  $wildfire_templates =& drupal_static(__FUNCTION__);

  if ($reset || !isset($wildfire_templates)) {
    $wildfire_templates = array();

    $result = db_select('wildfire_templates', 'ct')
      ->fields('ct')
      ->execute();

    while ($dbrow = $result->fetchAssoc()) {
      // Make sure the description doesn't contain any funny business.
      if (isset($dbrow['description'])) {
        $dbrow['description'] = check_plain($dbrow['description']);
      }
      $wildfire_templates[$dbrow['name']] = $dbrow;
    }

    // Grab any template regions for all the templates.
    foreach ($wildfire_templates as $key => &$template) {
      // Get the region structure from this specific template
      $template['regions'] = wildfire_template_get_regions($template['name']);
    }

    // Check to see if PNG, JPG or GIF screenshot exists for each theme, in that
    // specific order.
    foreach ($wildfire_templates as &$template) {
      $base = $template['path'] . '/' . $template['name'];

      // Try to find valid image files and make sure they are 128x128.
      foreach (array('.png', '.jpg', '.gif') as $fileext) {
        if (file_exists($base . $fileext) && ($size = @getimagesize($base . $fileext)) !== FALSE) {
          list($width, $height) = $size;

          // If width and height are 128, add this to the template as a valid
          // screenshot.
          if ($width == 128 && $height == 128) {
            $template['screenshot'] = $base . $fileext;
            break;
          }
        }
      }

      // Set the 'no screenshot' image if no screenshot found.
      if (!isset($template['screenshot'])) {
        $template['screenshot'] = drupal_get_path('module', 'wildfire') . '/images/no-screenshot.png';
      }
    }
  }

  return $wildfire_templates;
}

/**
 * Gets the templates available to the current user.
 *
 * Checks for permissions to see whether the user has permission to see the
 * 'invisible' templates or not.
 *
 * @return array
 *    An array of mail templates.
 */
function wildfire_template_get_available() {
  if (user_access('administer wildfire templates')) {
    return wildfire_templates_get();
  }
  else {
    $templates = wildfire_templates_get();

    // Prune out the ones with status == 0.
    foreach ($templates as $name => $template) {
      if ($template['status'] == 0) {
        unset($templates[$name]);
      }
    }

    return $templates;
  }
}

/**
 * Rescans for template files and rebuilds the template registry.
 *
 * @returns
 *    The array of templates as would be returned by wildfire_templates_get().
 */
function wildfire_templates_refresh() {

  $default_theme = variable_get('theme_default', 'bartik');
  $paths = wildfire_templates_get_paths($default_theme);

  $valid_paths = FALSE;

  foreach ($paths as $path) {
    // There must be a mailtemplates folder, otherwise we can't have any
    // templates!
    if (file_exists($path)) {
      if ($valid_paths === FALSE) {
        // No valid paths have been found yet so create an empty array and
        // populate the first item
        $valid_paths = array();
        $valid_paths[] = $path;
      }
      else {
        // array has already been created so just add another item
        $valid_paths[] = $path;
      }
    }
  }

  // Show the user an error if no valid template paths were found.
  if ($valid_paths === FALSE) {
    $pathstext = implode(', ', $paths);
    drupal_set_message(
      t('The mailtemplates folder was not found in any valid location. There must be a mailtemplates folder at one of these locations. The locations searched were: %path', array('%path' => $pathstext)),
      'error'
    );
  }
  else {
    $scanned = array();

    // Now loop through the different paths in the $valid_path array
    foreach ($valid_paths as $valid_path) {
      /**
       * Scan the files and folders in the mailtemplates folder and recurse.
       *
       * The result is collated in $scanned, which is passed in by ref so that
       * the results keep getting appended to the end.
       */

      wildfire_templates_scan($scanned, $valid_path);

      if (count($scanned) == 0) {
        drupal_set_message(
          t('A mailtemplates folder was found at %path, but no templates were found inside it.', array('%path' => $valid_path)),
          'warning'
        );
        return $scanned;
      }

    }

    // Get the existing templates by querying the database.
    $templates = wildfire_templates_get();

    // Merge together, giving priority to the freshly scanned templates, so that
    // paths are updated and titles are altered if they've been changed.
    foreach ($scanned as $name => &$scan) {
      // Copy the status from the template's record in the database, if present,
      // because status is not stored in the template info files.
      $scan['status'] = isset($templates[$name]['status']) && $templates[$name]['status']
                      ? $templates[$name]['status']
                      : 0;
    }

    // Flush the {wildfire_templates} table, as we are about to regenerate it.
    db_query("TRUNCATE TABLE {wildfire_templates}");

    // Write the new set of templates out...
    foreach ($scanned as $name => $template) {
      // Set the template name to be the same as the array key of this template,
      // if not already set.
      $template['name'] = $name;
      drupal_write_record('wildfire_templates', $template);
    }

    // Get rid of everything from the {wildfire_templates_regions} table
    // since this will be regenerated when we update the regions.
    db_query("TRUNCATE TABLE {wildfire_templates_regions}");

    // Update the repeaters for this template.
    wildfire_template_repeaters_update($scanned);

    // Update the snippets for this template.
    wildfire_template_snippets_update($scanned);

    // Update the settings for this template.
    wildfire_templates_settings_update($scanned);

  }

  return wildfire_templates_get(TRUE);
}

/**
 * Scans the given path for templates, adding them to an array.
 *
 * This is a recursive function so it will call itself for each of the results.
 *
 * @param $scanned
 *    The array containing templates the function has found.
 * @param $path
 *    The path that should be scanned.
 */
function wildfire_templates_scan(&$scanned, $path) {
  if (is_dir($path)) {
    $contents = scandir($path);

    foreach ($contents as $item) {
      $tname = array();
      $details = array();

      // Does the filename end in .itpl and contain no dashes? If so, it's a
      // template file.
      if (preg_match('/^(.*)\.tinfo$/i', $item, $tname)) {
        // Parse the .tinfo file into an array.
        $details = drupal_parse_info_file($path . '/' . $item);

        // Add the template's path to the details array.
        $details['path'] = $path;

        // Grab the info file name and use it to assign the details array to the
        // right place in the template array.
        $scanned[$tname[1]] = $details;
      }
      elseif (!($item == '.' || $item == '..')) {
        // Recurse here.
        wildfire_templates_scan($scanned, $path . '/' . $item);
      }
    }
  }
}

/**
 * Saves the settings for a given template
 *
 * @param string $template
 *  Machine name of the template
 * @param int $delta
 *  Delta for the set of settings to obtain
 * @param array $data
 *  Associative array of settings to save
 *
 * @return int
 *  ID of the save record, or FALSE on failure
 */
function wildfire_templates_set_settings($template, $delta, $data) {

  $id = db_merge('wildfire_templates_settings')
    ->key(array(
      'template' => $template,
      'delta' => $delta,
    ))
    ->fields(array(
      'data' => serialize($data)
    ))
    ->execute();

  return $id;

}


/**
 * Disable the specified mail template.
 *
 * @param $tname string
 *    The machine name of the template that should be disabled.
 */
function wildfire_template_disable($tname) {
  $record = array();
  $record['status'] = 0;
  $record['name'] = $tname;

  drupal_write_record('wildfire_templates', $record, 'name');
}

/**
 * Enable the specified mail template.
 *
 * @param $tname string
 *    The machine name of the template that should be enabled.
 */
function wildfire_template_enable($tname) {
  $record = array();
  $record['status'] = 1;
  $record['name'] = $tname;

  drupal_write_record('wildfire_templates', $record, 'name');
}

/**
 * Update the list of available repeaters.
 *
 * This is usually called on a template refresh.
 *
 * @param array $templates
 *    The keyed array of templates.
 */
function wildfire_template_repeaters_update($templates) {
  // Start by disabling all repeaters and we'll re-enable them if they're still
  // available. Note we don't delete any repeaters here.
  db_update('wildfire_repeaters')
    ->fields(array(
      'status' => 0,
    ))
    ->execute();

  // Go through each template and if it has repeaters, add these to the list.
  foreach ($templates as $name => $template) {
    if (isset($template['repeaters'])) {
      foreach ($template['repeaters'] as $key => $repeater) {
        $id = db_merge('wildfire_repeaters')
          ->key(array(
            'name' => $key
          ))
          ->fields(array(
            'title' => $repeater,
            'status' => 1,
          ))
          ->execute();

        $id = db_insert('wildfire_templates_regions')
          ->fields(array(
            'template' => $name,
            'type' => 'repeater',
            'name' => $key,
          ))
          ->execute();
      }
    }
  }
}

/**
 * Update the list of available settings.
 *
 * This is usually called on a template refresh.
 *
 * @param array $templates
 *    The keyed array of templates.
 */
function wildfire_templates_settings_update($templates) {

  // Go through each template and if it has settings, add these to the list.
  foreach ($templates as $name => $template) {
    if (isset($template['settings'])) {

      // Read in any existing settings for this template.
      $query = db_select('wildfire_templates_settings', 'wts')
        ->fields('wts');
      $query->condition('template', $name, '=');
      $result = $query->execute();

      if ($result->rowCount() > 0) {
        // Merge the current fields with the currently stored fields,
        // removing any values that are no longer defined.
        while ($row = $result->fetchAssoc()) {

          // Merge the current with the new
          $existing = unserialize($row['data']);

          if (empty($existing)) {
            // Existing data is broken; store the defaults and continue
            wildfire_templates_set_settings($name, 0, $template['settings']);
            continue;
          }

          $new = $template['settings'];
          $data = wildfire_array_overwrite_values($new, $existing);

          $record = array(
            'template' => $row['template'],
            'delta' => $row['delta'],
            'data' => $data
          );

          drupal_write_record('wildfire_templates_settings', $record, array('template', 'delta'));

        }
      }
      else {
        // Add a new row containing default values
        wildfire_templates_set_settings($name, 0, $template['settings']);
      }

    }
  }
}


/**
 * Update the list of available snippets.
 *
 * This is usually called on a template refresh.
 *
 * @param array $templates
 *    The keyed array of templates.
 */
function wildfire_template_snippets_update($templates) {
  // Start by disabling all snippets and we'll re-enable them if they're still
  // available. Note we don't delete any snippets here.
  db_update('wildfire_snippets')
    ->fields(array(
      'status' => 0,
    ))
    ->execute();

  // Go through each template and if it has repeaters, add these to the list.
  foreach ($templates as $name => $template) {
    if (isset($template['snippets'])) {
      foreach ($template['snippets'] as $key => $snippet) {
        $id = db_merge('wildfire_snippets')
          ->key(array(
            'name' => $key
          ))
          ->fields(array(
            'title' => $snippet,
            'status' => 1,
          ))
          ->execute();

        $id = db_insert('wildfire_templates_regions')
          ->fields(array(
            'template' => $name,
            'type' => 'snippet',
            'name' => $key,
          ))
          ->execute();
      }
    }
  }
}
