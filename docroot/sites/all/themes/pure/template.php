<?php

/**
 * @file
 * Functions to extend and initialize the Pure theme system.
 */

/**
 * Implements hook_theme().
 */
function pure_theme($existing, $type, $theme, $path) {
  return array(
    // Register the newly added theme_form_content() hook so we can utilise
    // theme hook suggestions.
    // @see pure_form_alter().
    'form_content' => array(
      'render element' => 'form',
      'path' => drupal_get_path('theme', 'pure') . '/templates',
      'template' => 'form-content',
    ),
  );
}

/**
 * Add an entry to the active themes list.
 *
 * @param $themes_active
 *   An array of active theme objects.
 * @param $theme
 *   An object containing the information for the theme to be added.
 */
function _pure_themes_active_populate(&$themes_active, $theme) {
  $themes_active[$theme->name] = $theme;
  $themes_active[$theme->name]->path = drupal_get_path('theme', $theme->name);
}

/**
 * Collect all information for the active theme.
 *
 * @return
 *   An array of active theme objects.
 */
function _pure_theme_collector() {
  // Store the theme data in a static variable.
  $themes_active = &drupal_static(__FUNCTION__, array());

  // If our themes are not collected, collect them.
  if (empty($themes_active)) {
    $themes = list_themes();
    global $theme_info;

    // If there is a base theme, collect the names of all themes that may have
    // data files to load.
    if (isset($theme_info->base_theme)) {
      global $base_theme_info;
      foreach ($base_theme_info as $base) {
        _pure_themes_active_populate($themes_active, $base);
      }
    }

    // Add the active theme to the list of themes that may have data files.
    _pure_themes_active_populate($themes_active, $theme_info);
  }

  return $themes_active;
}

/**
 * Collect paths for all the include files.
 *
 * @param $directory_name
 *   Name of the directory declared in a theme's .info file.
 * @param $root_path
 *   The path from the Drupal root that $directory_name resides.
 *
 * @return
 *   An array of files to be included.
 */
function _pure_include_files_collector($directory_name, $root_path) {
  // This function relies on a proper path to look into, so if they are not
  // available return nothing.
  if (empty($directory_name) || empty($root_path) || !is_dir($root_path . '/' .  $directory_name)) {
    return '';
  }

  // Open the directory and recursively scan for all *.inc files.
  $directory = opendir($root_path . '/' .  $directory_name);
  $entry_array = array();
  while($entry_name = readdir($directory)) {
    if ($entry_name != '.' && $entry_name != '..' && !is_dir($root_path . '/' .  $directory_name . '/' . $entry_name)) {
      $ext = substr($entry_name, strrpos($entry_name, '.') + 1);
      if ($ext == 'inc') {
        $entry_array[$directory_name][] = $root_path . '/' .  $directory_name . '/' . $entry_name;
      }
    }
    elseif ($entry_name != '.' && $entry_name != '..' && is_dir($root_path . '/' .  $directory_name . '/' . $entry_name)) {
      $entry_array = array_merge($entry_array, _pure_include_files_collector($entry_name, $root_path . '/' .  $directory_name, FALSE));
    }
  }
  closedir($directory);

  return $entry_array;
}

/**
 * Load the include files.
 *
 * @param $includes
 *   An array of files to include.
 */
function _pure_load_files($includes = array()) {
  if (is_array($includes)) {
    foreach ($includes as $path) {
      if (is_array($path)) {
        _pure_load_files($path);
      }
      elseif (is_file($path)) {
        require $path;
      }
    }
  }
}

/**
 * Allow sub-themes to declare new theme hooks.
 *
 * Sub-themes will be able to declare new subthemes with an implementation of
 * THEME_suggestions(). A theme hook specific version can also be used in the
 * form of THEME_suggestions_HOOK().
 *
 * @param $variables
 *  An array of elements from a preprocess function.
 * @param $hook
 *   The current theme hook being processed.
 */
function _pure_suggestion_collector(&$variables, $hook) {
  $themes = _pure_theme_collector();
  $suggestions = &$variables['theme_hook_suggestions'];
  $functions = array();
  $new_suggestions = array();

  // Look for core suggestion functions in relation to themes and the primary
  // hook.
  foreach ($themes as $theme => $data) {
    // This loop will prevent duplicated code for constructing the base
    // suggestion hook for a theme (ex. pure_suggestions()) and one for the
    // specific theme hook being processed (ex. pure_suggestions_node()).
    foreach (array('' => '', $hook => '_') as $hook_name => $delimiter) {
      $function = $theme . '_suggestions' . $delimiter . $hook_name;
      if (function_exists($function)) {
        $functions[] = $function;
      }
    }
  }

  // Cycle through existing theme hook suggestions for suggestion functions.
  foreach ($suggestions as $suggestion) {
    foreach ($themes as $theme => $data) {
      $function = $theme . '_suggestions_' . $suggestion;
      if (function_exists($function)) {
        $functions[] = $function;
      }
    }
  }

  // Iterate through found functions and run them on $variables.
  foreach ($functions as $function) {
    $new_suggestions = array_merge($new_suggestions, $function($variables, $hook, $new_suggestions));
  }

  // Add the new theme hook suggestions if they are not a duplicate.
  foreach ($new_suggestions as $suggestion) {
    if ($suggestion != $hook && !in_array($suggestion, $suggestions)) {
      $suggestions[] = $suggestion;
    }
  }
}

/**
 * Call processors for all theme hook suggestions.
 *
 * Drupal will not call a precessor or preprocessor function for a theme hook
 * unless it is registered in hook_theme(). This circumvents the need to do
 * this by calling it if it would not otherwise be called.
 *
 * @param $type
 *   The type of processing function. Valid options are "process" and
 *   "preprocess".
 * @param $variables
 *   The array of data fed to the original processor.
 * @param $hook
 *   The original hook being processed.
 */
function _pure_hook_engine($type, &$variables, $hook) {
  // Get all the active themes.
  $themes = _pure_theme_collector();

  // Sometimes there is a disconnect between the primary theme_hook_suggestion
  // and what the hook really is. In order to maintain the separation between
  // modules and related overrides, check for this and run a preprocess
  // function if it exists. This will avoid the need to declare this theme hook
  // in a hook_theme implementation.
  if (!empty($variables['theme_hook_suggestion']) && $hook != $variables['theme_hook_suggestion']) {
    foreach ($themes as $theme => $data) {
      $function = $theme . '_' . $type  . '_' . $variables['theme_hook_suggestion'];
      if (function_exists($function)) {
        $function($variables, $variables['theme_hook_suggestion']);
      }
    }
  }

  // Enable the calling of theme suggestion preprocess without having to declare
  // it in hook_theme().
  if (!empty($variables['theme_hook_suggestions'])) {
    foreach ($variables['theme_hook_suggestions'] as $suggestion) {
      foreach ($themes as $theme => $data) {
        $function = $theme . '_' . $type  . '_' . $suggestion;
        if (function_exists($function)) {
          $function($variables, $suggestion);
        }
      }
    }
  }
}

/**
 * Calls cache_get() specifically for Pure based caches.
 *
 * @param $type
 *   The cache being accessed.
 *
 * @return
 *   An array of cached data.
 */
function _pure_cache_get($type) {
  if ($cache = cache_get('theme_registry:pure:' . $type . ':' . $GLOBALS['theme'])) {
    return $cache;
  }
}

/**
 * Calls cache_set() specifically for Pure based caches.
 *
 * @param $type
 *   The cache being written.
 * @param $data
 *   The data to be cached.
 */
function _pure_cache_set($type, $data) {
  cache_set('theme_registry:pure:' . $type . ':' . $GLOBALS['theme'], $data);
}

/**
 * Initialize the pure system.
 */
function _pure_init() {
  // Get the files to be included.
  if ($cache = _pure_cache_get('includes')) {
    $paths = $cache->data;
  }
  else {
    $themes = _pure_theme_collector();
    $paths = array();
    foreach ($themes as $name => $theme) {
      if (isset($theme->info['include directories'])) {
        foreach ($theme->info['include directories'] as $directory) {
          $paths[$name][$directory] = _pure_include_files_collector($directory, $theme->path);
        }
      }
    }

    _pure_cache_set('includes', $paths);
  }

  // Include the files.
  foreach ($paths as $theme_paths) {
    _pure_load_files($theme_paths);
  }
}

/**
 * Process additional stylesheet types.
 *
 * Check for stylesheets to be placed at the top of the stack or conditional
 * Internet Explorer styles in the .info file. Add them to the $styles
 * variable.
 */
function _pure_process_stylesheets() {
  // Prepare the needed variables.
  global $theme_info;
  $themes_active = _pure_theme_collector();
  $framework_styles = array();
  $conditional_styles = array();

  // If there is more than one active theme, check all base themes for
  // stylesheets.
  if (count($themes_active) > 1) {
    global $base_theme_info;
    foreach ($base_theme_info as $name => $info) {
      if (isset($info->info['framework stylesheets'])) {
        $framework_styles[$info->name] = $info->info['framework stylesheets'];
      }
      if (isset($info->info['conditional stylesheets'])) {
        $conditional_styles[$info->name] = $info->info['conditional stylesheets'];
      }
    }
  }

  // Check the current theme for stylesheets.
  if (isset($theme_info->info['framework stylesheets'])) {
    $framework_styles[$theme_info->name] = $theme_info->info['framework stylesheets'];
  }
  if (isset($theme_info->info['conditional stylesheets'])) {
    $conditional_styles[$theme_info->name] = $theme_info->info['conditional stylesheets'];
  }

  // If there is at least one entry in the $framework_styles array, process it.
  if (count($framework_styles) >= 1) {
    // Add all the framework stylesheets to a group so they are loaded first.
    foreach ($framework_styles as $theme => $medias) {
      foreach ($medias as $media => $stylesheets) {
        foreach ($stylesheets as $path) {
          $path = drupal_get_path('theme', $theme) . '/' . $path;
          drupal_add_css($path, array(
            'group' => CSS_SYSTEM,
            'media' => $media,
            'weight' => -1000,
            'every_page' => TRUE,
          ));
        }
      }
    }
  }

  // If there is at least one entry in the $conditional_styles array, process it.
  if (count($conditional_styles) >= 1) {
    // Add all the conditional stylesheets with drupal_add_css().
    foreach ($conditional_styles as $theme => $conditions) {
      foreach ($conditions as $condition => $medias) {
        foreach ($medias as $media => $stylesheets) {
          foreach ($stylesheets as $path) {
            $path = drupal_get_path('theme', $theme) . '/' . $path;
            if ($condition == '!ie') {
              $browsers = array('!IE' => TRUE, 'IE' => FALSE);
            }
            else {
              $browsers = array('!IE' => FALSE, 'IE' => $condition);
            }
            drupal_add_css($path, array(
              'media' => $media,
              'every_page' => TRUE,
              'browsers' => $browsers,
              'group' => CSS_THEME,
              'weight' => 1000,
            ));
          }
        }
      }
    }
  }
}

/**
 * Start the engines.
 */
_pure_init();

