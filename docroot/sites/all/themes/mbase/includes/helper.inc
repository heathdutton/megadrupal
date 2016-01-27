<?php
/**
 * @file
 * Helper function for template.php.
 */

/**
 * Helper function to draw region based on 3 conditions.
 *
 * It displays block if block is enabled.
 * It displays content from the theme settings page if content
 * is added in theme settings page and block disabled
 * hides the region if both region dont have any block and theme
 * settings page.
 *
 * @param array $region_content
 *   Block content array.
 * @param string $region_id
 *   Machine name of the block regions.
 * @param string $element
 *   Element to wrap the block, Usually div or section.
 * @param string $id
 *   ID if the element.
 * @param string $classes
 *   Classes for the element.
 * @param array $attributes
 *   Other attributes for the element.
 *
 * @return string|array|false
 *   A string or array depending on the type of value and if a base theme also
 *   contains the same $key, FALSE if no $key is found.
 */
function _mbase_region_render(array $region_content, $region_id, $theme, $element = "div", $id = "", $classes = "", $attributes = array()) {

  $elementattributes = '';
  foreach ($attributes as $key => $value) {
    $elementattributes .= $key;
    $elementattributes .= ' = "';
    $elementattributes .= $value;
    $elementattributes .= '" ';
  }
  $output = NULL;
  // Display the block content if it have block content.
  if (($region_content)) {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '"   ' . $elementattributes . '>' . PHP_EOL;
    $output .= render($region_content) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }

  $region_code = theme_get_setting($region_id . '_code', $theme);
  $region_status = theme_get_setting($region_id . '_status', $theme);
  // Display the content from theme setting pages if any content available.
  if (trim($region_code) && $region_status) {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '" ' . $elementattributes . '>' . PHP_EOL;
    $output .= _mbase_helper_token_replace($region_code, $theme) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }
  // Hide the region of the block is disabled using <disabled>.
  if (!$region_status) {
    return FALSE;
  }
  // Display the content from txt file if no content available.
  if ($region_status && !trim($region_code)) {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '" ' . $elementattributes . ' >' . PHP_EOL;
    $output .= _mbase_helper_get_content_text($region_id, $theme) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }
}

/**
 * Same as _mbase_region_render() but trying to implement different way.
 */
function _mbase_smart_region(&$var, $regions, $theme) {
  foreach ($regions as $region) {
    $drupal_block = $var['page'][$region];
    // Display the block if it exists.
    if ($drupal_block) {
      continue;
    }

    // Display theme settings snippet if (visible, block disabled,
    // region code not empty).
    $mbase_block_status = theme_get_setting($region . '_status', $theme);
    $mbase_block_content = theme_get_setting($region . '_code', $theme);
    if (empty($drupal_block) && $mbase_block_status && trim($mbase_block_content)) {
      $var['page'][$region] = _mbase_block_helper($region, $mbase_block_content);
      continue;
    }
    // Display from txt file, if (visible, block disabled, region code empty,
    // txt file not empty).
    $mbase_block_file_content = _mbase_helper_get_content_text($region, $theme);
    if (empty($drupal_block) && $mbase_block_status && !trim($mbase_block_content) && $mbase_block_file_content) {
      $var['page'][$region] = _mbase_block_helper($region, $mbase_block_file_content);
      continue;
    }
  }
}

/**
 * Helper function to fake the block content from theme settings page.
 */
function _mbase_block_helper($region_name, $content, $subject = NULL) {
  $block[$region_name]['#markup'] = $content;
  $blockobj = new stdClass();
  $blockobj->title = $subject;
  $blockobj->subject = $subject;
  $blockobj->module = 'block';
  $blockobj->delta = 1;
  $blockobj->status = 1;
  $blockobj->region = $region_name;
  $block[$region_name]['#block'] = $blockobj;
  $block[$region_name]['#theme_wrappers'] = array('block');
  $block['#theme_wrappers'] = array('region');
  $block['#region'] = $region_name;
  return $block;
}

/**
 * Get content from txt file and replace the token and return it.
 */
function _mbase_helper_get_content_text($variable, $theme) {
  $path = drupal_get_path('theme', $theme) . '/assets/html/' . $variable . '.txt';
  if (file_exists($path)) {
    return _mbase_helper_token_replace(file_get_contents($path), $theme);
  }
  return FALSE;
}

/**
 * Theme token replace.
 */
function _mbase_helper_token_replace($string, $theme, $token = '<<path-to-theme>>') {
  $replace = base_path() . drupal_get_path('theme', $theme);
  return str_replace($token, $replace, $string);
}

/**
 * Function to get the theme setting or default html from contents folder.
 */
function _mbase_helper_theme_get_setting($variable, $theme = 'mbase') {
  $theme_content = trim(theme_get_setting($variable . '_code', $theme));
  $output = $theme_content ? $theme_content : _mbase_helper_get_content_text($variable, $theme);
  return $output;
}

/**
 * Return information from the .info file of a theme (and possible base themes).
 *
 * @param string $theme_key
 *   The machine name of the theme.
 * @param string $key
 *   The key name of the item to return from the .info file. This value can
 *   include "][" to automatically attempt to traverse any arrays.
 * @param bool $base_themes
 *   Recursively search base themes, defaults to TRUE.
 *
 * @return string|array|false
 *   A string or array depending on the type of value and if a base theme also
 *   contains the same $key, FALSE if no $key is found.
 */
function _mbase_get_theme_info($theme_key = NULL, $key = NULL, $base_themes = TRUE) {
  // If no $theme_key is given, use the current theme if we can determine it.
  if (!isset($theme_key)) {
    $theme_key = !empty($GLOBALS['theme_key']) ? $GLOBALS['theme_key'] : FALSE;
  }
  if ($theme_key) {
    $themes = list_themes();
    if (!empty($themes[$theme_key])) {
      $theme = $themes[$theme_key];
      // If a key name was specified, return just that array.
      if ($key) {
        $value = FALSE;
        // Recursively add base theme values.
        if ($base_themes && isset($theme->base_themes)) {
          foreach (array_keys($theme->base_themes) as $base_theme) {
            $value = _mbase_get_theme_info($base_theme, $key);
          }
        }
        if (!empty($themes[$theme_key])) {
          $info = $themes[$theme_key]->info;
          // Allow array traversal.
          $keys = explode('][', $key);
          foreach ($keys as $parent) {
            if (isset($info[$parent])) {
              $info = $info[$parent];
            }
            else {
              $info = FALSE;
            }
          }
          if (is_array($value)) {
            if (!empty($info)) {
              if (!is_array($info)) {
                $info = array($info);
              }
              $value = drupal_array_merge_deep($value, $info);
            }
          }
          else {
            if (!empty($info)) {
              if (empty($value)) {
                $value = $info;
              }
              else {
                if (!is_array($value)) {
                  $value = array($value);
                }
                if (!is_array($info)) {
                  $info = array($info);
                }
                $value = drupal_array_merge_deep($value, $info);
              }
            }
          }
        }
        return $value;
      }
      // If no info $key was specified, just return the entire info array.
      return $theme->info;
    }
  }
  return FALSE;
}

/**
 * Pre-render fieldset element.
 */
function _bootstrap_pre_render_fieldset($element) {
  // Fieldsets may be rendered outside of a Form API context.
  if (!isset($element['#parents']) || !isset($element['#groups'])) {
    return $element;
  }
  // Inject group member elements belonging to this group.
  $parents = implode('][', $element['#parents']);
  $children = element_children($element['#groups'][$parents]);
  if (!empty($children)) {
    if (empty($element['#default_tab'])) {
      $children_keys = array_values($children);
      $element['#default_tab'] = $element['#groups'][$parents][array_shift($children_keys)]['#id'];
    }
    foreach ($children as $key) {
      // Break references and indicate that the element should be rendered as
      // group member.
      $child = (array) $element['#groups'][$parents][$key];
      $child['#attributes']['id'] = $child['#id'];
      $child['#group_fieldset'] = TRUE;
      // Inject the element as new child element.
      $element[] = $child;

      $sort = TRUE;
    }
    // Re-sort the element's children if we injected group member elements.
    if (isset($sort)) {
      $element['#sorted'] = FALSE;
    }
  }

  if (isset($element['#group'])) {
    $group = $element['#group'];
    // If this element belongs to a group, but the group-holding element does
    // not exist, we need to render it (at its original location).
    if (!isset($element['#groups'][$group]['#group_exists'])) {
      // Intentionally empty to clarify the flow; we simply return $element.
    }
    // If we injected this element into the group, then we want to render it.
    elseif (!empty($element['#group_fieldset'])) {
      // Intentionally empty to clarify the flow; we simply return $element.
    }
    // Otherwise, this element belongs to a group and the group exists, so we do
    // not render it.
    elseif (element_children($element['#groups'][$group])) {
      $element['#printed'] = TRUE;
    }
  }

  return $element;
}

/**
 * Process all elements.
 */
function _bootstrap_process_element(&$element, &$form_state) {
  if (!empty($element['#attributes']['class']) && is_array($element['#attributes']['class'])) {
    if (in_array('container-inline', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-inline';
    }
    if (in_array('form-wrapper', $element['#attributes']['class'])) {
      $element['#attributes']['class'][] = 'form-group';
    }
  }
  return $element;
}

/**
 * Process input elements.
 */
function _bootstrap_process_input(&$element, &$form_state) {
  // Only add the "form-control" class for specific element input types.
  $types = array(
    // Core.
    'password',
    'password_confirm',
    'select',
    'textarea',
    'textfield',
    // Elements module.
    'emailfield',
    'numberfield',
    'rangefield',
    'searchfield',
    'telfield',
    'urlfield',
  );
  if (!empty($element['#type']) && (in_array($element['#type'], $types) || ($element['#type'] === 'file' && empty($element['#managed_file'])))) {
    $element['#attributes']['class'][] = 'form-control';
  }
  return $element;
}

/**
 * Process fieldset element.
 */
function _bootstrap_process_fieldset(&$element, &$form_state) {
  $parents = implode('][', $element['#parents']);

  // Each fieldset forms a new group. The #type 'vertical_tabs' basically only
  // injects a new fieldset.
  $form_state['groups'][$parents]['#group_exists'] = TRUE;
  $element['#groups'] = &$form_state['groups'];

  // Process vertical tabs group member fieldsets.
  if (isset($element['#group'])) {
    // Add this fieldset to the defined group (by reference).
    $element['#theme_wrappers'] = array('bootstrap_panel');
    $group = $element['#group'];
    $form_state['groups'][$group][] = &$element;
  }

  // Contains form element summary functionalities.
  $element['#attached']['library'][] = array('system', 'drupal.form');

  // The .form-wrapper class is required for #states to treat fieldsets like
  // containers.
  if (!isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = array();
  }

  return $element;
}

/**
 * Helper function for adding colors to button elements.
 *
 * @param array $element
 *   The form element, passed by reference.
 */
function _mbase_colorize_button(array &$element) {
  if (_mbase_is_button($element)) {
    // Do not add the class if one is already present in the array.
    $button_classes = array(
      'btn-default',
      'btn-primary',
      'btn-success',
      'btn-info',
      'btn-warning',
      'btn-danger',
      'btn-link',
    );
    $class_intersection = array_intersect($button_classes, $element['#attributes']['class']);
    if (empty($class_intersection)) {
      // Get the matched class.
      $class = _mbase_setting('button_colorize') ? _mbase_colorize_text($element['#value']) : FALSE;
      // If no particular class matched, use the default style.
      if (!$class) {
        $class = 'default';
      }
      $element['#attributes']['class'][] = 'btn-' . $class;
    }
  }
}

/**
 * Helper function for associating Bootstrap classes based on a string's value.
 *
 * @param string $string
 *   The string to match classes against.
 * @param string $default
 *   The default class to return if no match is found.
 *
 * @return string
 *   The Bootstrap class matched against the value of $haystack or $default if
 *   no match could be made.
 */
function _mbase_colorize_text($string, $default = '') {
  static $texts;
  if (!isset($texts)) {
    $filedata = _mbase_load_config_file_data();
    $texts = $filedata['textcolor'];
  }
  // Iterate over the array.
  foreach ($texts as $pattern => $strings) {
    foreach ($strings as $class => $values) {
      foreach ($values as $value) {
        switch ($pattern) {
          case 'matches':
            if ($string === $value) {
              return $class;
            }
            break;

          case 'contains':
            if (strpos(drupal_strtolower($string), drupal_strtolower($value)) !== FALSE) {
              return $class;
            }
            break;
        }
      }
    }
  }
  // Return the default if nothing was matched.
  return $default;
}


/**
 * Helper function for adding icons to button elements.
 *
 * @param array $element
 *   The form element, passed by reference.
 */
function _mbase_iconize_button(array &$element) {
  if (_mbase_setting('button_iconize') && _mbase_is_button($element) && ($icon = _mbase_iconize_text($element['#value']))) {
    $element['#icon'] = $icon;
  }
}

/**
 * Helper function for associating Bootstrap icons with text.
 *
 * @param string $string
 *   The string to match classes against.
 * @param string $default
 *   The default icon to return if no match is found.
 *
 * @return string
 *   The Bootstrap icon matched against the value of $haystack or $default if
 *   no match could be made.
 */
function _mbase_iconize_text($string, $default = '') {
  static $texts;
  if (!isset($texts)) {
    $filedata = _mbase_load_config_file_data();
    $texts = $filedata['texticons'];
  }
  // Iterate over the array.
  foreach ($texts as $pattern => $strings) {
    if (!empty($strings)) {
      foreach ($strings as $icon => $values) {
        foreach ($values as $value) {
          switch ($pattern) {
            case 'matches':
              if ($string === $value) {
                return _mbase_icon($icon);
              }
              break;

            case 'contains':
              if (strpos(drupal_strtolower($string), drupal_strtolower($value)) !== FALSE) {
                return _mbase_icon($icon);
              }
              break;
          }
        }
      }
    }
  }

  // Return a default icon if nothing was matched.
  return _mbase_icon($default);
}

/**
 * Helper function for invoking a specific suggestion's preprocess functions.
 *
 * @param array $variables
 *   The theme implementation variables array.
 */
function _bootstrap_preprocess_theme_suggestion(array &$variables) {
  $registry = theme_get_registry();
  if (!empty($variables['theme_hook_suggestion']) && !empty($registry[$variables['theme_hook_suggestion']]['preprocess functions'])) {
    // Save the suggestion as the hook to pass to the function.
    $hook = $variables['theme_hook_suggestion'];

    // Iterate over the preprocess functions.
    foreach ($registry[$hook]['preprocess functions'] as $function) {
      if (function_exists($function)) {
        // Invoke theme hook suggestion preprocess function.
        $function($variables, $hook);

        // Unset the theme_hook_suggestion so the suggestion's preprocess
        // functions can provide theme_hook_suggestions if needed.
        if (!empty($variables['theme_hook_suggestions'])) {
          unset($variables['theme_hook_suggestion']);
        }
      }
    }
  }
}

/**
 * Helper function for invoking a specific suggestion's process functions.
 *
 * @param array $variables
 *   The theme implementation variables array.
 */
function _bootstrap_process_theme_suggestion(array &$variables) {
  $registry = theme_get_registry();
  if (!empty($variables['theme_hook_suggestion']) && !empty($registry[$variables['theme_hook_suggestion']]['process functions'])) {
    // Save the suggestion as the hook to pass to the function.
    $hook = $variables['theme_hook_suggestion'];

    // Iterate over the process functions.
    foreach ($registry[$hook]['process functions'] as $function) {
      if (function_exists($function)) {
        // Invoke theme hook suggestion process function.
        $function($variables, $hook);

        // Unset the theme_hook_suggestion so the suggestion's preprocess
        // functions can provide theme_hook_suggestions if needed.
        if (!empty($variables['theme_hook_suggestions'])) {
          unset($variables['theme_hook_suggestion']);
        }
      }
    }
  }
}

/**
 * Helper function to determine if a string of text is considered "simple".
 *
 * @param string $string
 *   The string of text to check "simple" criteria on.
 * @param int|FALSE $length
 *   The length of characters used to determine whether or not $string is
 *   considered "simple". Set explicitly to FALSE to disable this criteria.
 * @param array|FALSE $allowed_tags
 *   An array of allowed tag elements. Set explicitly to FALSE to disable this
 *   criteria.
 * @param bool $html
 *   A variable, passed by reference, that indicates whether or not the
 *   string contains HTML.
 *
 * @return bool
 *   Returns TRUE if the $string is considered "simple", FALSE otherwise.
 */
function _mbase_is_simple_string($string, $length = 250, $allowed_tags = NULL, &$html = FALSE) {
  // Use the advanced drupal_static() pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['strings'] = &drupal_static(__FUNCTION__);
  }
  $strings = &$drupal_static_fast['strings'];
  if (!isset($strings[$string])) {
    $plain_string = strip_tags($string);
    $simple = TRUE;
    if ($allowed_tags !== FALSE) {
      $filtered_string = filter_xss($string, $allowed_tags);
      $html = $filtered_string !== $plain_string;
      $simple = $simple && $string === $filtered_string;
    }
    if ($length !== FALSE) {
      $simple = $simple && strlen($plain_string) <= intval($length);
    }
    $strings[$string] = $simple;
  }
  return $strings[$string];
}

/**
 * Retrieves a setting for the current theme or for a given theme.
 *
 * This is a wrapper for theme_get_setting(), ensuring to use deprecated
 * setting values instead.
 *
 * @param string $name
 *   The name of the setting to be retrieved.
 * @param string $theme
 *   The name of a given theme; defaults to the currently active theme.
 * @param string $prefix
 *   The prefix used on the $name of the setting, this will be appended with
 *   "_" automatically if set.
 *
 * @return mixed
 *   The value of the requested setting, NULL if the setting does not exist.
 *
 * @see theme_get_setting()
 */
function _mbase_setting($name, $theme = NULL, $prefix = 'mbase', $load_parent = FALSE) {
  $prefix = !empty($prefix) ? $prefix . '_' : '';
  $setting = theme_get_setting($prefix . $name, $theme);
  switch ($prefix . $name) {

    case 'mbase_forms_smart_descriptions':
      $deprecated = theme_get_setting('mbase_tooltip_descriptions', $theme);
      if (isset($deprecated)) {
        $setting = (int) !empty($deprecated);
      }
      break;

    case 'mbase_forms_smart_descriptions_limit':
      $deprecated = theme_get_setting('mbase_tooltip_descriptions_length', $theme);
      if (isset($deprecated)) {
        $setting = (int) !empty($deprecated);
      }
      break;

  }
  // Load few settings from mbase, if it is not avaliable in child theme.
  if (!is_array($setting) && $load_parent) {
    if (!trim($setting)) {
      $setting = theme_get_setting($prefix . $name, 'mbase');
    }
  }
  return $setting;
}

/**
 * Helper function for retrieving an element attributes array.
 *
 * @param array $element
 *   The individual renderable array element. It is possible to also pass the
 *   $variables parameter in [pre]process functions and it will logically
 *   determine the correct path to that particular theme hook's attribute array.
 *   Passed by reference.
 * @param string $property
 *   Determines which attributes array to retrieve. By default, this is the
 *   normal attributes, but can be "wrapper_attributes" or
 *   "input_group_attributes".
 *
 * @return array
 *   The attributes array. Passed by reference.
 */
function &_mbase_get_attributes(array &$element, $property = 'attributes') {
  // Attempt to retrieve a renderable element attributes first.
  if (
    isset($element['#type']) ||
    isset($element['#theme']) ||
    isset($element['#pre_render']) ||
    isset($element['#markup']) ||
    isset($element['#theme_wrappers']) ||
    isset($element["#$property"])
  ) {
    if (!isset($element["#$property"])) {
      $element["#$property"] = array();
    }
    return $element["#$property"];
  }
  // Treat $element as if it were a [pre]process function $variables parameter
  // and look for a renderable "element".
  elseif (isset($element['element'])) {
    if (!isset($element['element']["#$property"])) {
      $element['element']["#$property"] = array();
    }
    return $element['element']["#$property"];
  }

  // If all else fails, create (if needed) a default "attributes" array. This
  // will, at the very least, either work or cause an error that can be tracked.
  if (!isset($element[$property])) {
    $element[$property] = array();
  }

  return $element[$property];
}

/**
 * Helper function for retrieving an element classes array.
 *
 * @param array $element
 *   The individual renderable array element. It is possible to also pass the
 *   $variables parameter in [pre]process functions and it will logically
 *   determine the correct path to that particular theme hook's classes array.
 *   Passed by reference.
 * @param string $property
 *   Determines which attributes array to retrieve. By default, this is the
 *   normal attributes, but can be "wrapper_attributes" or
 *   "input_group_attributes".
 *
 * @return array
 *   The classes array. Passed by reference.
 */
function &_mbase_get_classes(array &$element, $property = 'attributes') {
  $attributes = &_mbase_get_attributes($element, $property);

  if (!isset($attributes['class'])) {
    $attributes['class'] = array();
  }
  // Contrib modules have a very bad habit of frequently adding classes as
  // strings, convert them to a proper array.
  // @see https://www.drupal.org/node/2269653
  elseif (!is_array($attributes['class'])) {
    $attributes['class'] = explode(' ', $attributes['class']);
  }

  // Ensure classes are not duplicated.
  $attributes['class'] = array_unique($attributes['class']);
  return $attributes['class'];
}

/**
 * Helper function for adding a class to an element.
 *
 * @param string|array $class
 *   An individual class or an array of classes to add.
 * @param array $element
 *   The individual renderable array element. It is possible to also pass the
 *   $variables parameter in [pre]process functions and it will logically
 *   determine the correct path to that particular theme hook's classes array.
 *   Passed by reference.
 * @param string $property
 *   Determines which attributes array to retrieve. By default, this is the
 *   normal attributes, but can be "wrapper_attributes" or
 *   "input_group_attributes".
 */
function _mbase_add_class($class, array &$element, $property = 'attributes') {
  // Retrieve the element's classes.
  $classes = &_mbase_get_classes($element, $property);

  // Convert the class to an array.
  if (!is_array($class)) {
    $class = array($class);
  }

  // Iterate over all classes to add.
  foreach ($class as $_class) {
    // Ensure the class to add does not yet already exist.
    if (!in_array($_class, $classes)) {
      $classes[] = $_class;
    }
  }
}

/**
 * Helper function for removing a class from an element.
 *
 * @param string|array $class
 *   An individual class or an array of classes to remove.
 * @param array $element
 *   The individual renderable array element. It is possible to also pass the
 *   $variables parameter in [pre]process functions and it will logically
 *   determine the correct path to that particular theme hook's classes array.
 *   Passed by reference.
 * @param string $property
 *   Determines which attributes array to retrieve. By default, this is the
 *   normal attributes, but can be "wrapper_attributes" or
 *   "input_group_attributes".
 */
function _mbase_remove_class($class, array &$element, $property = 'attributes') {
  // Retrieve the element's classes.
  $classes = &_mbase_get_classes($element, $property);

  // Convert the class to an array.
  if (!is_array($class)) {
    $class = array($class);
  }

  // Iterate over all classes to add.
  foreach ($class as $_class) {
    $key = array_search($_class, $classes);
    if ($key !== FALSE) {
      unset($classes[$key]);
    }
  }
}

/**
 * Helper function for returning whether a theme is Bootstrap based.
 *
 * @param string $theme_key
 *   The machine name of the theme to check, if not set the active theme name
 *   will be used.
 * @param bool $include_theme_key
 *   Whether to append the returned list with $theme_key.
 *
 * @return array
 *   An indexed array of base themes.
 */
function _mbase_get_base_themes($theme_key = NULL, $include_theme_key = FALSE) {
  static $themes;
  if (!isset($theme_key)) {
    $theme_key = $GLOBALS['theme_key'];
  }
  if (!isset($themes[$theme_key])) {
    $themes[$theme_key] = array_unique(array_filter((array) _mbase_get_theme_info($theme_key, 'base theme')));
  }
  if ($include_theme_key) {
    $themes[$theme_key][] = $theme_key;
  }
  return $themes[$theme_key];
}

/**
 * Helper function that wraps file_scan_directory().
 *
 * Finds all files that match a given mask in a given directory and then caches
 * the results. A general site cache clear will force new scans to be initiated
 * for already cached directories.
 *
 * @param string $dir
 *   The base directory or URI to scan, without trailing slash.
 * @param string $mask
 *   The preg_match() regular expression of the files to find.
 * @param array $options
 *   Additional options to pass to file_scan_directory().
 *
 * @return array
 *   An associative array (keyed on the chosen key) of objects with 'uri',
 *   'filename', and 'name' members corresponding to the matching files.
 *
 * @see file_scan_directory()
 */
function _mbase_file_scan_directory($dir, $mask, array $options = array()) {
  // Retrieve cached data.
  $cid = 'theme_registry:mbase:files';
  $files = array();
  if ($cache = cache_get($cid)) {
    $files = $cache->data;
  }
  // Generate a unique hash for all parameters passed as a change in any of
  // them would return different results.
  $hash = drupal_hash_base64(serialize(func_get_args()));
  if (!isset($files[$hash])) {
    $files[$hash] = file_scan_directory($dir, $mask, $options);
    cache_set($cid, $files);
  }
  return $files[$hash];
}


/**
 * Stub implementation for hook_theme().
 *
 * This base-theme's custom theme hook implementations. Never define "path"
 * or "template" as these are detected and automatically added.
 *
 * @see bootstrap_theme_registry_alter()
 * @see bootstrap_hook_theme_complete()
 * @see bootstrap_theme()
 * @see hook_theme()
 */
function _bootstrap_theme(&$existing, $type, $theme, $path) {
  // Bootstrap Carousels.
  $hooks['bootstrap_carousel'] = array(
    'variables' => array(
      'attributes' => array(),
      'items' => array(),
      'start_index' => 0,
      'controls' => TRUE,
      'indicators' => TRUE,
      'interval' => 5000,
      'pause' => 'hover',
      'wrap' => TRUE,
    ),
  );
  // Bootstrap Dropdowns.
  $hooks['bootstrap_dropdown'] = array(
    'render element' => 'element',
  );

  // Bootstrap Modals.
  $hooks['bootstrap_modal'] = array(
    'variables' => array(
      'heading' => '',
      'body' => '',
      'footer' => '',
      'attributes' => array(),
      'html_heading' => FALSE,
    ),
  );

  // Bootstrap Panels.
  $hooks['bootstrap_panel'] = array(
    'render element' => 'element',
  );

  // Bootstrap search form wrapper.
  // @todo Remove this as it's not really needed and should use suggestions.
  $hooks['bootstrap_search_form_wrapper'] = array(
    'render element' => 'element',
  );
  return $hooks;
}


/**
 * Discovers and sets the path to each `THEME-HOOK.$extension` file.
 */
function mbase_find_theme_includes($registry, $extension, $path) {
  $regex = '/' . str_replace('.', '\.', $extension) . '$/';
  $files = drupal_system_listing($regex, $path, 'name', 0);

  $hook_includes = array();
  foreach ($files as $name => $file) {
    // Chop off the remaining extension.
    if (($pos = strpos($name, '.')) !== FALSE) {
      $name = substr($name, 0, $pos);
    }
    // Transform "-" in filenames to "_" to match theme hook naming scheme.
    $hook = strtr($name, '-', '_');
    // File to be included by core's theme function when the hook is invoked.
    // This only applies to base hooks. When hook derivatives are called
    // (those with a double "__"), it checks for the base hook, calls its
    // variable processors and ignores anything specific to the derivative.
    // Due to the way it works, It becomes redundant to give it a path that is
    // not a base hook.
    // @see https://drupal.org/node/939462
    if (isset($registry[$hook]) && !isset($registry[$hook]['base hook'])) {
      // Include the file so core can discover any containing functions.
      include_once DRUPAL_ROOT . '/' . $file->uri;
      $hook_includes[$hook]['includes'][] = $file->uri;
    }
  }

  return $hook_includes;
}


/**
 * Merges the cached element information into the runtime array.
 *
 * @param array $info
 *   The element info array to merge data into.
 * @param array $cached
 *   The cached element info data array to merge from.
 *
 * @return array
 *   The altered element info array.
 */
function _mbase_element_info_array_merge(array $info, array $cached) {
  foreach ($cached as $type => $element) {
    $replacement_data = isset($element['#bootstrap_replace']) ? $element['#bootstrap_replace'] : array();
    unset($element['#bootstrap_replace']);
    foreach ($element as $property => $data) {
      if (is_array($data)) {
        if (!isset($info[$type][$property])) {
          $info[$type][$property] = array();
        }
        // Append the values if not already in the array.
        foreach ($data as $key => $value) {
          if (!in_array($value, $info[$type][$property])) {
            $info[$type][$property][] = $value;
          }
        }
      }
      // Create the property, if not already set.
      elseif (!isset($info[$type][$property])) {
        $info[$type][$property] = $data;
      }
    }
    // Replace data, if necessary.
    foreach ($replacement_data as $property => $data) {
      if (is_array($data)) {
        foreach ($data as $needle => $replacement) {
          if (!empty($info[$type][$property]) && ($key = array_search($needle, $info[$type][$property])) !== FALSE) {
            $info[$type][$property][$key] = $replacement;
          }
        }
      }
      // Replace the property with the new data.
      else {
        $info[$type][$property] = $data;
      }
    }
  }

  // Return the altered element info array.
  return $info;
}

/**
 * Helper function for smartly converting an element description into a tooltip.
 *
 * @param array $element
 *   An element render array, passed by reference.
 * @param array $target
 *   The target element render array the tooltip is to be attached to, passed
 *   by reference. If not set, it will default to the $element passed.
 * @param bool $input_only
 *   Toggle determining whether or not to only convert input elements.
 * @param int $length
 *   The length of characters to determine if description is "simple".
 */
function _mbase_element_smart_description(array &$element, array &$target = NULL, $input_only = TRUE, $length = NULL) {
  // Determine if tooltips are enabled.
  static $enabled;
  if (!isset($enabled)) {
    $enabled = _mbase_setting('tooltip_enabled') && _mbase_setting('forms_smart_descriptions');
  }

  // Immediately return if "simple" tooltip descriptions are not enabled.
  if (!$enabled) {
    return FALSE;
  }

  // Allow a different element to attach the tooltip.
  if (!isset($target)) {
    $target = &$element;
  }

  // Retrieve the length limit for smart descriptions.
  if (!isset($length)) {
    $length = (int) _mbase_setting('forms_smart_descriptions_limit');
    // Disable length checking by setting it to FALSE if empty.
    if (empty($length)) {
      $length = FALSE;
    }
  }

  // Retrieve the allowed tags for smart descriptions. This is primarily used
  // for display purposes only (i.e. non-UI/UX related elements that wouldn't
  // require a user to "click", like a link).
  $allowed_tags = array_filter(array_unique(array_map('trim', explode(',', _mbase_setting('forms_smart_descriptions_allowed_tags') . ''))));

  // Disable length checking by setting it to FALSE if empty.
  if (empty($allowed_tags)) {
    $allowed_tags = FALSE;
  }

  $html = FALSE;
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  if (!$input_only || !empty($target['#input']) || !empty($element['#smart_description']) || !empty($target['#smart_description'])) {
    if (!empty($element['#description']) && empty($target['#attributes']['title']) && empty($target['#attributes']['data-toggle'])) {
      if (_mbase_is_simple_string($element['#description'], $length, $allowed_tags, $html)) {
        // Default property (on the element itself).
        $property = 'attributes';

        // Add the tooltip to the #label_attributes property for 'checkbox'
        // and 'radio' elements.
        if ($type === 'checkbox' || $type === 'radio') {
          $property = 'label_attributes';
        }
        // Add the tooltip to the #wrapper_attributes property for 'checkboxes'
        // and 'radios' elements.
        elseif ($type === 'checkboxes' || $type === 'radios') {
          $property = 'wrapper_attributes';
        }
        // Add the tooltip to the #input_group_attributes property for elements
        // that have valid input groups set.
        elseif ((!empty($element['#field_prefix']) || !empty($element['#field_suffix'])) && (!empty($element['#input_group']) || !empty($element['#input_group_button']))) {
          $property = 'input_group_attributes';
        }

        // Retrieve the proper attributes array.
        $attributes = &_mbase_get_attributes($target, $property);

        // Set the tooltip attributes.
        $attributes['title'] = $allowed_tags !== FALSE ? filter_xss($element['#description'], $allowed_tags) : $element['#description'];
        $attributes['data-toggle'] = 'tooltip';
        if ($html || $allowed_tags === FALSE) {
          $attributes['data-html'] = 'true';
        }

        // Remove the element description so it isn't (re-)rendered later.
        unset($element['#description']);
      }
    }
  }
}

/**
 * Helper function for returning a Bootstrap Glyphicon.
 *
 * @param string $name
 *   The icon name, minus the "glyphicon-" prefix.
 * @param string $default
 *   (Optional) The default value to return.
 *
 * @return string
 *   The HTML markup containing the icon defined by $name, $default value if
 *   icon does not exist or returns empty output for whatever reason.
 */
function _mbase_icon($name, $default = NULL, $icon_provider = 'glyphicon') {
  $output = NULL;
  // @todo, child themes should check if iconizing is enabled before continue.
  if (_mbase_setting('fontawesome')) {
    $icon_provider = 'fa';
  }
  // Get data from config file.
  $filedata = _mbase_load_config_file_data();
  $file_icons = $filedata['icons'];
  if (_mbase_setting('button_iconize')) {
    // Attempt to use the Icon API module, if enabled and it generates output.
    if (module_exists('icon')) {
      $output = theme('icon', array('bundle' => 'bootstrap', 'icon' => $icon_provider . '-' . $name));
    }
    if (empty($output)) {
      // Mimic the Icon API markup.
      $temp_icon = NULL;

      if (isset($file_icons[$name][$icon_provider])) {
        $temp_icon = $file_icons[$name][$icon_provider];
      }
      else {
        if ($default && trim($default)) {
          $temp_icon = $file_icons[$default][$icon_provider];
        }
        else {
          return $default;
        }
      }
      $attributes = array(
        'class' => array('icon', $icon_provider, $temp_icon),
        'aria-hidden' => 'true',
      );
      $output = '<span' . drupal_attributes($attributes) . '></span>';
    }
  }
  return empty($output) && isset($default) ? $default : $output;
}

/**
 * Helper function for determining whether an element is a button.
 *
 * @param array $element
 *   A renderable element.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function _mbase_is_button(array $element) {
  return
    !empty($element['#type']) &&
    !empty($element['#value']) && (
      $element['#type'] === 'button' ||
      $element['#type'] === 'submit' ||
      $element['#type'] === 'image_button'
    );
}

/**
 * Processes registered hooks in the theme registry against list of themes.
 *
 * Discovers and fills missing elements in the theme registry. This is similar
 * to _theme_process_registry(), however severely modified for Bootstrap based
 * themes.
 *
 * All additions or modifications must live in `./templates`, relative to the
 * base theme or sub-theme's base folder. These files can be organized in any
 * order using sub-folders as it searches recursively.
 *
 * Adds or modifies the following theme hook keys:
 *  - `includes`: When a variables file `*.vars.php` is found.
 *  - `includes`: When a function file `*.func.php` is found.
 *  - `function`: When a specific theme hook function override is found.
 *  - `template`: When a template file `*.tpl.php` is found in. Note, if both
 *    a function and a template are defined, a template implementation will
 *    always be used and the `function` will be unset.
 *  - `path`: When a template file `*.tpl.php` is found.
 *  - `preprocess functions`: When a specific theme hook suggestion function
 *    `hook_preprocess_HOOK__SUGGESTION` is found.
 *  - `process functions` When a specific theme hook suggestion function
 *    `hook_process_HOOK__SUGGESTION` is found.
 *
 * @param array $registry
 *   The theme registry array, passed by reference.
 * @param string|array $themes
 *   The name of the theme or list of theme names to process.
 *
 * @see bootstrap_theme_registry_alter()
 * @see _theme_process_registry()
 * @see _theme_build_registry()
 */
function _mbase_process_theme_registry(array &$registry, $themes) {
  // Convert to an array if needed.
  if (is_string($themes)) {
    $themes = array();
  }

  // Processor functions work in two distinct phases with the process
  // functions always being executed after the preprocess functions.
  $variable_process_phases = array(
    'preprocess functions' => 'preprocess',
    'process functions'    => 'process',
  );

  // Iterate over each theme passed.
  // Iterate over the [pre]process phases.
  foreach ($variable_process_phases as $phase_key => $phase) {
    foreach ($themes as $theme) {
      // Get the theme's base path.
      $path = drupal_get_path('theme', $theme);

      // Find theme function overrides.
      foreach (drupal_system_listing('/\.(func|vars)\.php$/', $path, 'name', 0) as $name => $file) {
        // Strip off the extension.
        if (($pos = strpos($name, '.')) !== FALSE) {
          $name = substr($name, 0, $pos);
        }

        // Transform "-" in file names to "_" to match theme hook naming scheme.
        $hook = strtr($name, '-', '_');

        // File to be included by core's theme function when a theme hook is
        // invoked.
        if (isset($registry[$hook])) {
          if (!isset($registry[$hook][$phase_key])) {
            $registry[$hook][$phase_key] = array();
          }
          if (!isset($registry[$hook]['includes'])) {
            $registry[$hook]['includes'] = array();
          }

          // Include the file now so functions can be discovered below.
          include_once DRUPAL_ROOT . '/' . $file->uri;
          if (!in_array($file->uri, $registry[$hook]['includes'])) {
            $registry[$hook]['includes'][] = $file->uri;
          }
        }
      }

      // Process core's normal functionality.
      _theme_process_registry($registry, $theme, $GLOBALS['theme_key'] === $theme ? 'theme' : 'base_theme', $theme, $path);

      // Find necessary templates in the theme.
      $registry = drupal_array_merge_deep($registry, drupal_find_theme_templates($registry, '.tpl.php', $path));

      // Iterate over each registered hook.
      foreach ($registry as $hook => $info) {
        // Remove function callbacks if a template was found.
        if (isset($info['function']) && isset($info['template'])) {
          unset($registry[$hook]['function']);
        }

        // Correct template theme paths.
        if (!isset($info['theme path'])) {
          $registry[$hook]['theme path'] = $path;
        }

        // Correct the type that is implementing this override.
        $registry[$hook]['type'] = $GLOBALS['theme_path'] === $registry[$hook]['theme path'] ? 'theme' : 'base_theme';

        // Sort the phase functions.
        // @see https://www.drupal.org/node/2098551
        _mbase_registry_sort_phase_functions($registry[$hook][$phase_key], $hook, $phase, $themes);

        // Setup a default "context" variable. This allows #context to be passed
        // to every template and theme function.
        // @see https://drupal.org/node/2035055
        if (isset($info['variables']) && !isset($info['variables']['context'])) {
          $registry[$hook]['variables']['context'] = array();
        }

        // Setup a default "icon" variable. This allows #icon to be passed
        // to every template and theme function.
        // @see https://drupal.org/node/2219965
        if (isset($info['variables']) && !isset($info['variables']['icon'])) {
          $registry[$hook]['variables']['icon'] = NULL;
        }
        if (isset($info['variables']) && !isset($info['variables']['icon_position'])) {
          $registry[$hook]['variables']['icon_position'] = 'before';
        }
      }
    }
  }
}

/**
 * Ensures the phase functions are invoked in the correct order.
 *
 * @param array $functions
 *   The phase functions to iterate over.
 * @param string $hook
 *   The current hook being processed.
 * @param string $phase
 *   The current phase being processed.
 * @param array $themes
 *   An indexed array of current themes.
 *
 * @see https://www.drupal.org/node/2098551
 */
function _mbase_registry_sort_phase_functions(array &$functions, $hook, $phase, array $themes) {
  // Immediately return if there is nothing to sort.
  if (count($functions) < 2) {
    return;
  }

  // Create an associative array of theme functions to ensure sort order.
  $theme_functions = array_fill_keys($themes, array());

  // Iterate over all the themes.
  foreach ($themes as $theme) {
    // Only add the function to the array of theme functions if it currently
    // exists in the $functions array.
    $function = $theme . '_' . $phase . '_' . $hook;
    $key = array_search($function, $functions);
    if ($key !== FALSE) {
      // Save the theme function to be added later, but sorted.
      $theme_functions[$theme][] = $function;

      // Remove it from the current $functions array.
      unset($functions[$key]);
    }
  }

  // Iterate over all the captured theme functions and place them back into
  // the phase functions array.
  foreach ($theme_functions as $array) {
    $functions = array_merge($functions, $array);
  }
}

/**
 * Processes registered hooks in the theme registry against list of themes.
 *
 * This is used to add the necessary phased functions to theme hook suggestions.
 * Because it uses get_defined_functions(), it must be invoked after all
 * includes have been detected and loaded. This is similar to
 * drupal_find_theme_functions(), however severely modified for Bootstrap based
 * themes.
 *
 * @param array $registry
 *   The theme registry array, passed by reference.
 * @param string|array $themes
 *   The name of the theme or list of theme names to process.
 *
 * @see https://drupal.org/node/939462
 * @see drupal_find_theme_functions()
 */
function _mbase_process_theme_registry_suggestions(array &$registry, $themes) {
  // Convert to an array if needed.
  if (is_string($themes)) {
    $themes = array();
  }

  // Merge in normal core detections first.
  $registry = drupal_array_merge_deep($registry, drupal_find_theme_functions($registry, $themes));

  // Processor functions work in two distinct phases with the process
  // functions always being executed after the preprocess functions.
  $variable_process_phases = array(
    'preprocess functions' => 'preprocess',
    'process functions'    => 'process',
  );

  $functions = get_defined_functions();

  // Iterate over each theme passed.
  foreach ($themes as $theme) {
    // Iterate over each registered hook.
    foreach ($registry as $hook => $info) {
      // The pattern to match.
      $pattern = isset($info['pattern']) ? $info['pattern'] : ($hook . '__');

      // Only process hooks that have not explicitly "turned off" patterns.
      if (empty($pattern)) {
        continue;
      }

      // Iterate over the [pre]process phases.
      foreach ($variable_process_phases as $phase_key => $phase) {
        // Find functions matching the specific theme, phase and pattern.
        if ($matches = preg_grep('/^' . $theme . '_' . $phase . '_' . $pattern . '/', $functions['user'])) {
          foreach ($matches as $match) {
            // Determine the current theme implementation.
            $hook = substr($match, strlen($theme . '_' . $phase) + 1);
            $base_hook = $hook;

            // If there's no current theme implementation, keep checking for
            // more generic base hooks. If there's still no implementation,
            // one must be created using the last found implementation
            // information.
            if (!isset($registry[$base_hook]) || isset($registry[$base_hook]['base hook'])) {
              // Iteratively strip everything after the last '__' delimiter,
              // until an implementation is found.
              while ($pos = strrpos($base_hook, '__')) {
                $base_hook = substr($base_hook, 0, $pos);
                if (isset($registry[$base_hook])) {
                  break;
                }
              }

              // No base hook was found, this allows the implementation to be
              // ignored in the next steps.
              if (!isset($registry[$base_hook])) {
                $base_hook = FALSE;
              }
            }

            // Process specific base hook implementations if necessary.
            if ($base_hook) {
              // The matched theme implementation does not exist in the
              // registry, one must be created if base hook information was
              // found, otherwise it will be ignored.
              if (!isset($registry[$hook])) {
                $hook_type = isset($registry[$base_hook]['function']) ? 'function' : 'template';
                $arg_name = isset($registry[$base_hook]['variables']) ? 'variables' : 'render element';
                $registry[$hook] = array(
                  $hook_type => $registry[$base_hook][$hook_type],
                  $arg_name => $registry[$base_hook][$arg_name],
                  'base hook' => $base_hook,
                  'type' => $registry[$base_hook]['type'],
                  'preprocess functions' => array(),
                  'process functions' => array(),
                );
                if (isset($registry[$base_hook]['path'])) {
                  $registry[$hook]['path'] = $registry[$base_hook]['path'];
                }
                if (isset($registry[$base_hook]['theme path'])) {
                  $registry[$hook]['theme path'] = $registry[$base_hook]['theme path'];
                }
              }
            }

            // If the hook exists, merge in the functions. Otherwise ignore it
            // since there was no base hook found and a new implementation
            // could not be created.
            if (isset($registry[$hook])) {
              $registry[$hook] = drupal_array_merge_deep($registry[$hook], array(
                $phase_key => array($match),
              ));

              // Due to how theme() functions, if a base hook implements
              // preprocess or process functions, then the base hook info is
              // used to invoke the necessary phase functions instead of the
              // suggestion hook info. To get around this, a helper function
              // must be appended to the base hook info so it can call the
              // theme suggestion implementation's phase function.
              $function = '_bootstrap_' . $phase . '_theme_suggestion';
              if (!in_array($function, $registry[$base_hook][$phase_key])) {
                $registry[$base_hook][$phase_key][] = $function;
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Formats the menu links for the child pages of the current page.
 *
 * @param array $book_link
 *   A fully loaded menu link that is part of the book hierarchy.
 *
 * @return string
 *   HTML for the links to the child pages of the current page.
 */
function _mbase_book_children(array $book_link) {
  if (isset($book_link['menu_name'])) {
    // Rebuild entire menu tree for the book.
    $tree = menu_build_tree($book_link['menu_name']);
    $tree = menu_tree_output($tree);
    // Fix the theme hook suggestions.
    _mbase_book_fix_theme_hooks($book_link['nid'], $tree);

    // Return the rendered output.
    return drupal_render($tree);
  }
}

/**
 * Helper function to fix theme hooks in book TOC menus.
 *
 * @param int $bid
 *   The book identification number.
 * @param array $element
 *   The element to iterate over, passed by reference.
 * @param int $level
 *   Used internally to determine the current level of the menu.
 */
function _mbase_book_fix_theme_hooks($bid, array &$element, $level = 0) {
  $hook = $level === 0 ? $bid : 'sub_menu__' . $bid;
  $element['#theme_wrappers'] = array('menu_tree__book_toc__' . $hook);
  foreach (element_children($element) as $child) {
    $element[$child]['#theme'] = 'menu_link__book_toc__' . $hook;
    // Iterate through all child menu items as well.
    if (!empty($element[$child]['#below'])) {
      _mbase_book_fix_theme_hooks($bid, $element[$child]['#below'], ($level + 1));
    }
  }
}

/**
 * Helper function for adding the necessary classes to a table.
 *
 * @param array $classes
 *   The array of classes, passed by reference.
 * @param array $variables
 *   The variables of the theme hook, passed by reference.
 */
function _mbase_table_add_classes(array &$classes, array &$variables) {
  $context = $variables['context'];

  // Generic table class for all tables.
  $classes[] = 'table';

  // Bordered table.
  if (!empty($context['bordered']) || (!isset($context['bordered']) && _mbase_setting('table_bordered'))) {
    $classes[] = 'table-bordered';
  }

  // Condensed table.
  if (!empty($context['condensed']) || (!isset($context['condensed']) && _mbase_setting('table_condensed'))) {
    $classes[] = 'table-condensed';
  }

  // Hover rows.
  if (!empty($context['hover']) || (!isset($context['hover']) && _mbase_setting('table_hover'))) {
    $classes[] = 'table-hover';
  }

  // Striped rows.
  if (!empty($context['striped']) || (!isset($context['striped']) && _mbase_setting('table_striped'))) {
    $classes[] = 'table-striped';
  }

  // Responsive table.
  $variables['responsive'] = isset($context['responsive']) ? $context['responsive'] : _mbase_setting('table_responsive');
}
/**
 * Helper function for adding the necessary classes to a link.
 */
function _mbase_links(&$variables) {
  $links = $variables['links'];
  $attributes = $variables['attributes'];
  $heading = $variables['heading'];
  global $language_url;
  $output = '';

  if (count($links) > 0) {
    // Treat the heading first if it is present to prepend it to the
    // list of links.
    if (!empty($heading)) {
      if (is_string($heading)) {
        // Prepare the array that will be used when the passed heading
        // is a string.
        $heading = array(
          'text' => $heading,
          // Set the default level of the heading.
          'level' => 'h2',
        );
      }
      $output .= '<' . $heading['level'];
      if (!empty($heading['class'])) {
        $output .= drupal_attributes(array('class' => $heading['class']));
      }
      $output .= '>' . check_plain($heading['text']) . '</' . $heading['level'] . '>';
    }
    $output .= '<ul' . drupal_attributes($attributes) . '>';
    $num_links = count($links);
    $i = 1;
    foreach ($links as $key => $link) {
      $class = array($key);
      // Add first, last and active classes to the list of links.
      if ($i == 1) {
        $class[] = 'first';
      }
      if ($i == $num_links) {
        $class[] = 'last';
      }
      if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page())) && (empty($link['language']) || $link['language']->language == $language_url->language)) {
        $class[] = 'active';
      }
      $output .= '<li' . drupal_attributes(array('class' => $class)) . '>';
      if (isset($link['href'])) {
        // Pass in $link as $options, they share the same keys.
        $link_icon = _mbase_iconize_text($link['title']);
        if ($link_icon) {
          $link['html'] = TRUE;
          $link_icon = $link_icon . ' ';
        }
        $link_class = _mbase_colorize_text($link['title'], 'primary');
        $link['attributes']['class'][] = 'btn';
        $link['attributes']['class'][] = 'btn-xs';
        if ($link_class) {
          $link['attributes']['class'][] = 'btn-' . $link_class;;
        }
        $link_text = $link_icon . $link['title'];
        $output .= l($link_text, $link['href'], $link);
      }
      elseif (!empty($link['title'])) {
        $link['attributes']['class'][] = 'label';
        $link_class = _mbase_colorize_text($link['title'], 'primary');
        if ($link_class) {
          $link['attributes']['class'][] = 'label-' . $link_class;;
        }
        // Some links are actually not links, but we wrap these in <span> for
        // adding title and class attributes.
        if (empty($link['html'])) {
          $link['title'] = check_plain($link['title']);
        }
        $span_attributes = '';
        if (isset($link['attributes'])) {
          $span_attributes = drupal_attributes($link['attributes']);
        }
        $output .= '<span' . $span_attributes . '>' . $link['title'] . '</span>';
      }
      ++$i;
      $output .= "</li>\n";
    }
    $output .= '</ul>';
  }
  return $output;
}

/**
 * Helper function to load data from info.txt file if not in cache.
 */
function _mbase_load_config_file_data($theme_key = FALSE) {
  // Use the advanced drupal_static pattern, since this is called very often.
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast = &drupal_static(__FUNCTION__);
  }
  // Get our static data.
  $data = &$drupal_static_fast;
  $data_types = array('icons', 'textcolor', 'texticons', 'animate');
  if (!$theme_key) {
    $theme_key = $GLOBALS['theme_key'];
  }
  $base_themes = array_reverse(_mbase_get_base_themes($theme_key, TRUE));
  foreach ($data_types as $type) {
    if (!isset($data[$type])) {
      // Retrieve cached data.
      $cid = 'theme_registry:' . $theme_key . ':config_' . $type;
      if (($cache = cache_get($cid)) && !empty($cache->data)) {
        $data[$type] = $cache->data;
      }
      else {
        foreach ($base_themes as $base_theme) {
          // Load the data from info.txt file.
          $data_file = drupal_get_path('theme', $base_theme) . '/config/' . $type . '.info.txt';
          if (file_exists($data_file)) {
            $temp_store = drupal_parse_info_file($data_file);
            $data[$type] = $temp_store[$type];
            // Store our retrived data in the cache.
            cache_set($cid, $data[$type]);
            break;
          }
        }
      }
    }
  }
  return $data;
}

/**
 * Helper function for sub-themes, to make theme-settings.php lean.
 */
function _mbase_subtheme_themesettings($regions, $theme_key, $region_type, $region_type_text) {
  $form = array();
  if (!empty($regions)) {
    $form[$region_type] = array(
      '#type' => 'fieldset',
      '#title' => t('%regiontype Content settings', array('%regiontype' => $region_type_text)),
      '#group' => 'bootstrap',
      '#description' => t('This portion helps you to setup some default contents without using drupal block system for %regiontype, but still you can use blocks to override this', array('%regiontype' => $region_type_text)),
    );
    foreach ($regions as $region_id => $region) {
      $form[$region_type][$region_id] = array(
        '#title' => t('%regiontype <strong>%regionname</strong> region.', array('%regiontype' => $region_type_text, '%regionname' => $region['regionname'])),
        '#type' => 'fieldset',
        '#description' => 'Settings for <a href = "http://cmsbots.com/node/56#design=' . $region['designid'] . '" target = "_blank"> this design</a>. Support and issues <a href = "http://help.cmsbots.com/issues/component#platform=d7&id=' . $region['designid'] . '" target = "_blank"> here</a>',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form[$region_type][$region_id][$region_id . '_status'] = array(
        '#title' => t('Enable Content for %regiontype "%regionname" region', array('%regiontype' => $region_type_text, '%regionname' => $region['regionname'])),
        '#default_value' => _mbase_setting($region_id . '_status', $theme_key, ''),
        '#type' => 'checkbox',
        '#required' => FALSE,
      );
      $form[$region_type][$region_id][$region_id . '_code'] = array(
        '#title' => t('Content for %regiontype "%regionname" region', array('%regiontype' => $region_type_text, '%regionname' => $region['regionname'])),
        '#description' => t('Content/Code for %regiontype <strong>%regionname</strong> region.', array('%regiontype' => $region_type_text, '%regionname' => $region['regionname'])),
        '#default_value' => _mbase_helper_theme_get_setting($region_id, $theme_key),
        '#type' => 'textarea',
        '#required' => FALSE,
      );
    }
  }
  return $form;
}
