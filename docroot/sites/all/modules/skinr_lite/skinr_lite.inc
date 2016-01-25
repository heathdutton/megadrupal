<?php
// $Id$

/**
 * @file
 *
 */


/**
 * Gets all skin info definitions that are enabled for a particular theme.
 * 
 * @param string $theme_key
 * The machine name of the theme being queried.
 * 
 * @return array 
 * An array of skin info definitions that apply to the current theme, keyed by
 * machine name of the skin.
 */
function skinr_lite_get_skin_info($theme_key = NULL, $reset = FALSE) {
  if (is_null($theme_key)) {
    global $theme_key;
  }
  
  if (!$reset) {
    if ($skins = cache_get("skinr_lite:theme_{$theme_key}_skins")) {
      return $skins->data;
    }
  }
  
  $skins = array();

  
  // We get the themes that are defined in the theme's info file
  $info = drupal_parse_info_file(drupal_get_path('theme', $theme_key) . "/$theme_key.info");
  if (!empty($info['skinr']) && is_array($info['skinr']) && $info['skinr']['api'] == 2) {
    $path = drupal_get_path('theme', $theme_key);
    $file = "$path/$theme_key.skinr.inc";
    @include_once $file;
    if (function_exists($function = $theme_key . '_skinr_skin_info')) {
      module_load_include('inc', 'skinr_lite');
      $skins = $function();
      $source = array(
        'api' => 2,
        'type' => 'theme',
        'name' => $theme_key,
        'directory' => !empty($info['skinr']['directory']) ? $info['skinr']['directory'] : 'skins',
        'path' => $path,
        'include file' => $path . "/$theme_key.skinr.inc",
        'base themes' => !empty($info['base theme']) ? array($info['base theme']) : array(),
      );
      _skinr_lite_skin_info_process($skins, $source);
    }
  }

  // We use skinr module, if it is available, to get other skins.
  // @todo: why don't we do this first, or just instead?
  if ($path = drupal_get_path('module', 'skinr')) {
    include_once $path . '/skinr.module';
    cache_clear_all('skinr_skin_info', 'cache');
    cache_clear_all('skinr_implements_api', 'cache');
    $skins = array_merge($skins, skinr_get_skin_info());
  }

  if (!empty($skins) && is_array($skins)) {
    // Here we check each skin to ensure that it is enabled for the theme,
    // that it has a supported form item type,
    // and that it is used in a supported theme hook.
    // @todo: add support for block theme hook
    foreach ($skins as $name => $skin_info) {
      if (!$skin_info['status'][$theme_key] ||
        !in_array($skin_info['type'], array('checkboxes', 'select', 'radios')) ||
        ($skin_info['theme hooks'] != array('*') && !in_array('html', $skin_info['theme hooks']))
      ) {
        unset($skins[$name]);
      }
    }
  }
  
  // Set skinr_lite_THEME_skins cache.
  variable_set("skinr_lite_theme_{$theme_key}_skins", $skins);
  if (!empty($skins)) {
    cache_set("skinr_lite:theme_{$theme_key}_skins", $skins);
    return $skins;
  }
  return FALSE;
}

/**
 * Parse a skin_infos array as returned from a skins plugin.
 *
 * This function inserts any missing defaults and updates the stylesheet and
 * script paths to be relative to Drupal's root.
 *
 * @param $skin_infos
 *   An array of skins as returned from skin plugins.
 * @param $source
 *   An associative array containing information about the source of the skin.
 *   See skinr_implements() for details.
 *
 * @todo Merge into skinr_get_skin_info() and remove this function.
 */
function _skinr_lite_skin_info_process(&$skin_infos, $source) {
  foreach ($skin_infos as $skin_name => $skin_info) {
    // Populate default properties.
    $skin_infos[$skin_name] += array(
      'name' => '',
      'title' => '',
      'type' => 'checkboxes',
      'description' => '',
      'group' => 'general',
      'theme hooks' => array('*'),
      'attached' => array(),
      'options' => array(),
      'weight' => NULL,
      'default status' => 0,
      'status' => array(),
    );

    // Merge in name.
    $skin_infos[$skin_name]['name'] = $skin_name;

    // Merge in source information.
    $skin_infos[$skin_name]['source'] = $source;

    // Merge in default status for all themes.
    if (empty($skin_info['status'][$source{'name'}])) {
      $skin_infos[$skin_name]['status'][$source{'name'}] = $skin_infos[$skin_name]['default status'];
      if (!empty($skin_infos[$skin_name]['status'][$source{'base themes'}{0}])) {
        $skin_infos[$skin_name]['status'][$source{'name'}] |= $skin_infos[$skin_name]['status'][$source{'base themes'}{0}];
      }
    }

    // Add path to stylesheets.
    if (isset($skin_infos[$skin_name]['attached']['css'])) {
      _skinr_lite_add_path_to_files($skin_infos[$skin_name]['attached']['css'], $source['path']);
    }
    // Add path to scripts.
    if (isset($skin_infos[$skin_name]['attached']['js'])) {
      _skinr_lite_add_path_to_files($skin_infos[$skin_name]['attached']['js'], $source['path']);
    }

    foreach ($skin_infos[$skin_name]['options'] as $option_name => $option) {
      // Add path to stylesheets.
      if (isset($option['attached']['css'])) {
        _skinr_lite_add_path_to_files($skin_infos[$skin_name]['options'][$option_name]['attached']['css'], $source['path']);
      }
      // Add path to scripts.
      if (isset($option['attached']['js'])) {
        _skinr_lite_add_path_to_files($skin_infos[$skin_name]['options'][$option_name]['attached']['js'], $source['path']);
      }

      // Validate class by running it through drupal_html_class().
      if (!is_array($skin_infos[$skin_name]['options'][$option_name]['class'])) {
        $skin_infos[$skin_name]['options'][$option_name]['class'] = array($skin_infos[$skin_name]['options'][$option_name]['class']);
      }
      foreach ($skin_infos[$skin_name]['options'][$option_name]['class'] as $key => $class) {
        $skin_infos[$skin_name]['options'][$option_name]['class'][$key] = drupal_html_class($class);
      }
    }
  }
}

/**
 * Helper function which creates form items from skins.
 * 
 * @param array $skins
 * An array of fully processed skinr skins, with local file paths, etc.
 * 
 * @return array
 * An array of form elements
 */
function _skinr_lite_skin_form_element($theme_key, $skins = array(), $ajax = FALSE) {
  
  $formelement = array('#title' => t('Skin options'), '#tree' => TRUE, '#skinr_lite_theme_key' => $theme_key, '#skinr_lite_skins' => $skins);
  $skin_settings = skinr_lite_get_theme_settings($theme_key);
  
  // @todo: add skinr group support
  // Render each available skin as a theme setting.
  foreach ($skins as $name => $skin_info) {
    if ($field = _skinr_lite_single_element($skin_info, $skin_settings)) {
      if ($ajax == TRUE) {
        $field['#ajax'] = array(
          'callback' => 'skinr_lite_save_form_setting',
          'progress' => array('type' => 'throbber', 'message' => t('...')),
        );
      }
      $formelement[$name] = $field;
      $settings[$name] = $skin_info;
      foreach ($skin_info['options'] as $option_name => $option) {
        foreach ($option['class'] as $class) {
          switch($field['#type']) {
            case 'select':
          $classes[$class]["#skinr-lite-id-$name select:$option_name"] = 'select';
              break;
            case 'radios':
              $classes[$class]["#skinr-lite-id-$name input[value='$option_name']"] = 'radio';
              break;
            case 'checkboxes':
              $classes[$class]["#skinr-lite-id-$name input[name='skins[{$name}][$option_name]']"] = 'checkbox';
              break;
          }
        }
      }
    }
  }
  
  // @todo: add a table of classes indicating which options trigger each class
  drupal_add_js(array('skinrLite' => array(
    'settings' => $settings,
    'classes' => $classes,
  )), 'setting');
  return $formelement;
  
}

/**
 * Helper function to return a single form element based on a $skin_info 
 * definition as provided in hook_skinr_skin_info.
 * 
 * @param array $skin_info
 * Skin definition as provided in hook_skinr_skin_info.
 * 
 * @param array $skin_settings
 * Current active settings for all skins, keyed by their machine names, as 
 * returned by skinr_lite_get_theme_settings.
 * 
 * @return string
 */
function _skinr_lite_single_element($skin_info, $skin_settings = array()) {
  $field = FALSE;
  $name = $skin_info['name'];
  if (!empty($skin_settings[$name])) {
    $value = $skin_settings[$name];
  }
  switch ($skin_info['type']) {
    case 'checkboxes':
      $field = array(
        '#type' => 'checkboxes',
        '#multiple' => TRUE,
        '#title' => t($skin_info['title']),
        '#options' => skinr_lite_info_options_to_form_options($skin_info['options']),
        '#default_value' => isset($value) ? $value : array(),
        '#description' => t($skin_info['description']),
        '#weight' => isset($skin_info['weight']) ? $skin_info['weight'] : NULL,
        '#attributes' => array('class' => array('skinr-lite options checkboxes'), 'skinrlite' => $skin_info['name']),
      );
      break;
    case 'radios':
      $field = array(
        '#type' => 'radios',
        '#title' => t($skin_info['title']),
        '#options' => array_merge(array('' => '&lt;none&gt;'), skinr_lite_info_options_to_form_options($skin_info['options'])),
        '#default_value' => isset($value) ? $value : '',
        '#description' => t($skin_info['description']),
        '#weight' => isset($skin_info['weight']) ? $skin_info['weight'] : NULL,
        '#attributes' => array('class' => array('skinr-lite options radios'), 'skinrlite' => $skin_info['name']),
      );
      break;
    case 'select':
      $field = array(
        '#type' => 'select',
        '#title' => t($skin_info['title']),
        '#options' => array_merge(array('' => '<none>'), skinr_lite_info_options_to_form_options($skin_info['options'])),
        '#default_value' => isset($value) ? $value : '',
        '#description' => t($skin_info['description']),
        '#weight' => isset($skin_info['weight']) ? $skin_info['weight'] : NULL,
        '#attributes' => array('class' => array('skinr-lite select'), 'skinrlite' => $skin_info['name']),
      );
      break;
    default:
      // If Skinr module is used in the code base, it may return some skin types
      // that Skinr lite cannot handle.
      break;
  }
  if (!empty($field)) {
    
    // Attach necessary files as defined in the $skin info.
    skinr_lite_attach_files($field, $skin_info);
    
  }
  return $field;
}

/**
 * Helper function to convert an array of options, as specified in the .info
 * file, into an array usable by Form API.
 *
 * @param $options
 *   An array containing at least the 'class' and 'label' keys.
 *
 * @return
 *   A Form API compatible array of options.
 */
function skinr_lite_info_options_to_form_options($options) {
  $form_options = array();
  foreach ($options as $option_name => $option) {
    $form_options[$option_name] = t($option['title']);
  }
  return $form_options;
}


/**
 * Helper function to prepend a path to an array of stylesheet or script filenames.
 *
 * If the url is absolute (e.g. the url start with 'http://' or 'https://')
 * or relative to the site's root (e.g. the url starts with '/') the path does
 * not get prepended.
 *
 * @param $files
 *   A an array of filenames that need the path prepended.
 * @param $path
 *   The path to prepend.
 */
function _skinr_lite_add_path_to_files(&$files, $path) {
  $newfiles = array();
  foreach ($files as $data => $options) {
    if (!is_array($options)) {
      // $options is not an array, it's a filename and passed as first
      // (and only) argument.
      if (_skinr_lite_is_local_file($path . '/' . $options)) {
        $options = $path . '/' . $options;
      }
      $newfiles[] = $options;
    }
    elseif (is_numeric($data)) {
      // If $options is an array, but $data is not a filename, find $data in the
      // $options array.
      if (_skinr_lite_is_local_file($path . '/' . $options['data'])) {
        $options['data'] = $path . '/' . $options['data'];
      }
      $newfiles[] = $options;
    }
    else {
      if (_skinr_lite_is_local_file($path . '/' . $data)) {
        $data = $path . '/' . $data;
      }
      $newfiles[$data] = $options;
    }
  }
  $files = $newfiles;
}

/**
 * Helper function to determine whether or not a given file is local or not.
 */
function _skinr_lite_is_local_file($file) {
  if (strpos($file, 'http://') === 0 || strpos($file, 'https://') === 0 || strpos($file, '/') === 0) {
    return FALSE;
  }
  if (!file_exists($file)) {
    return FALSE;
  }
  return TRUE;
}

function skinr_lite_process_color_stylesheets($theme_key, $files, $palette) {
  $theme_path = drupal_get_path('theme', $theme_key);
  $current = color_get_palette($theme_key);
  $nonpalette = array();
  $styles = '';
  
  foreach ($files as $filename) {
    $stylesheet = drupal_load_stylesheet("$theme_path/$filename", TRUE);
    
    // Get rid of all comments
    $stylesheet = preg_replace("|\/\*.+?\*\/|s", "", $stylesheet);
    
    // Separate out rules
    $rules = explode('}', $stylesheet);

    // Separate each rule
    while (list($i, $rule) = each($rules)) {
      
      $add_selectors = $add_propvals = array();
      
      // Separate each rule into selector and list of property:values
      $onerule = explode('{', $rule);
      
      // Empty space check
      if ($selectors = strtolower(trim($onerule[0]))) {
        
        // Separate out the list of selectors
        foreach(explode(',', $selectors) as $selector) {

          // Empty space check
          $selector = trim($selector);
          if (!empty($selector)) {
            
            // Add 'html ' to each selector
            $add_selectors[] = 'html ' . $selector;
          }
        }
            
        // Separate out the list of property:values
        $propvals = explode(';', $onerule[1]);

        // Get each property:value pair
        while (list($i, $propval) = each($propvals)) {

          // Ensure that the property:value pair makes sense
          if (!empty($propval) && stripos($propval, ':')) {

            // Separate out each property and value
            list ($property, $value) = explode(':', $propval);

            // Get all the colors from the value
            $colors = _skinr_lite_get_colors($value);

            // Check the colors for a match to the palette
            foreach ($colors as $color) {

              // If the color is not in the palette, add it to non-palette colors, with its base key
              if (!in_array($color, $palette)) {
                $key = _skinr_lite_get_color_base($selectors . '{' . $property);
                $nonpalette[$color] = $key;
              }
              
            }
            
            $add_propvals[] = "$property: $value;";
            
          }
        }
      }
      
      // Add to $styles here
      if (!empty($add_selectors) && !empty($add_propvals)) {
        $styles .= ' ' . implode(', ', $add_selectors) . ' {' . implode(' ', $add_propvals) . '}';
      }
      
    }
  }
  
  drupal_add_js(array('skinrLiteColors' => array(
    'styles' => $styles,
    'palette' => $palette,
    'nonpalette' => $nonpalette,
    'current' => $current,
  )), 'setting');
  
}

function _skinr_lite_get_colors($value) {
  $colors = array();
  preg_match('|(#[0-9A-Fa-f]{6})|', $value, $colors);
  return $colors;
}

function _skinr_lite_get_color_base($selector) {
  // 'a' declarations. Use link.
  if (preg_match('@[^a-z0-9_-](a)[^a-z0-9_-][^/{]*{[^{]+$@i', $selector)) {
    $base = 'link';
  }
  // 'color:' styles. Use text.
  elseif (preg_match('/(?<!-)color[^{:]*:[^{#]*$/i', $selector)) {
    $base = 'text';
  }
  // Reset back to base.
  else {
    $base = 'base';
  }
  return $base;
}
