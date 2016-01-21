<?php
/**
 * @file
 * Helper function for template.php.
 */

define('BOOTSTRAP_VERSION_MAJOR', 3);
define('BOOTSTRAP_VERSION_MINOR', 3);
define('BOOTSTRAP_VERSION_PATCH', 2);
define('BOOTSTRAP_VERSION', BOOTSTRAP_VERSION_MAJOR . '.' . BOOTSTRAP_VERSION_MINOR . '.' . BOOTSTRAP_VERSION_PATCH);

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
function _bootstrap_region_render(array $region_content, $region_id, $element = "section", $id = "", $classes = "", $attributes = array()) {

  $elementattributes = '';
  foreach ($attributes as $key => $value) {
    $elementattributes .= $key;
    $elementattributes .= ' = "';
    $elementattributes .= $value;
    $elementattributes .= '" ';
  }
  $output = NULL;
  $theme_content = theme_get_setting($region_id);
  // Display the block content if it have block content.
  if (($region_content)) {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '"   ' . $elementattributes . '>' . PHP_EOL;
    $output .= render($region_content) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }
  // Display the content from theme setting pages if any content available.
  elseif (trim($theme_content) && trim($theme_content) != '<disable>') {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '" ' . $elementattributes . '>' . PHP_EOL;
    $output .= _bootstrap_helper_token_replace($theme_content) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }
  // Hide the region of the block is disabled using <disabled>.
  elseif (trim($theme_content) == "<disable>") {
    return FALSE;
  }
  // Display the content from txt file if no content available.
  else {
    $output .= '<' . $element . ' id = "' . $id . '" class = "' . $classes . '" ' . $elementattributes . ' >' . PHP_EOL;
    $output .= _bootstrap_helper_get_content_text($region_id) . PHP_EOL;
    $output .= '</' . $element . '>';
    return $output;
  }
}

/**
 * Get content from txt file and replace the token and return it.
 */
function _bootstrap_helper_get_content_text($variable) {
  $path = drupal_get_path('theme', 'color_glass') . '/assets/html/' . $variable . '.txt';
  if (file_exists($path)) {
    return _bootstrap_helper_token_replace(file_get_contents($path));
  }
  return FALSE;
}

/**
 * Theme token replace.
 */
function _bootstrap_helper_token_replace($string, $token = '<<path-to-theme>>') {
  $replace = base_path() . drupal_get_path('theme', 'color_glass');
  return str_replace($token, $replace, $string);
}

/**
 * Function to get the theme setting or default html from contents folder.
 */
function _bootstrap_helper_theme_get_setting($variable, $theme = 'color_glass') {
  $theme_content = trim(theme_get_setting($variable, $theme));
  $output = $theme_content ? $theme_content : _bootstrap_helper_get_content_text($variable);
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
function _bootstrap_get_theme_info($theme_key = NULL, $key = NULL, $base_themes = TRUE) {
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
            $value = _bootstrap_get_theme_info($base_theme, $key);
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
function _bootstrap_colorize_button(array &$element) {
  if (_bootstrap_is_button($element)) {
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
      $class = _bootstrap_setting('button_colorize') ? _bootstrap_colorize_text($element['#value']) : FALSE;
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
function _bootstrap_colorize_text($string, $default = '') {
  static $texts;
  if (!isset($texts)) {
    $texts = array(
      // Text that match these specific strings are checked first.
      'matches' => array(
        // Primary class.
        t('Download feature')   => 'primary',

        // Success class.
        t('Add effect')         => 'success',
        t('Add and configure')  => 'success',
        t('Log in')             => 'success',

        // Info class.
        t('Save and add')       => 'info',
        t('Add another item')   => 'info',
        t('Update style')       => 'info',
      ),

      // Text that contain these words anywhere in the string are checked last.
      'contains' => array(
        // Primary class.
        t('Confirm')            => 'primary',
        t('Filter')             => 'primary',
        t('Submit')             => 'primary',
        t('Search')             => 'primary',

        // Success class.
        t('Add')                => 'success',
        t('Create')             => 'success',
        t('Save')               => 'success',
        t('Write')              => 'success',

        // Warning class.
        t('Export')             => 'warning',
        t('Import')             => 'warning',
        t('Restore')            => 'warning',
        t('Rebuild')            => 'warning',

        // Info class.
        t('Apply')              => 'info',
        t('Update')             => 'info',

        // Danger class.
        t('Delete')             => 'danger',
        t('Remove')             => 'danger',
      ),
    );

    // Allow sub-themes to alter this array of patterns.
    drupal_alter('bootstrap_colorize_text', $texts);
  }

  // Iterate over the array.
  foreach ($texts as $pattern => $strings) {
    foreach ($strings as $value => $class) {
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

  // Return the default if nothing was matched.
  return $default;
}


/**
 * Helper function for adding icons to button elements.
 *
 * @param array $element
 *   The form element, passed by reference.
 */
function _bootstrap_iconize_button(array &$element) {
  if (_bootstrap_setting('button_iconize') && _bootstrap_is_button($element) && ($icon = _bootstrap_iconize_text($element['#value']))) {
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
function _bootstrap_iconize_text($string, $default = '') {
  static $texts;
  if (!isset($texts)) {
    $texts = array(
      // Text that match these specific strings are checked first.
      'matches' => array(),

      // Text that contain these words anywhere in the string are checked last.
      'contains' => array(
        t('Manage')     => 'cog',
        t('Configure')  => 'cog',
        t('Download')   => 'download',
        t('Export')     => 'export',
        t('Filter')     => 'filter',
        t('Import')     => 'import',
        t('Save')       => 'ok',
        t('Update')     => 'ok',
        t('Edit')       => 'pencil',
        t('Add')        => 'plus',
        t('Write')      => 'plus',
        t('Cancel')     => 'remove',
        t('Delete')     => 'trash',
        t('Remove')     => 'trash',
        t('Upload')     => 'upload',
      ),
    );
  }

  // Iterate over the array.
  foreach ($texts as $pattern => $strings) {
    foreach ($strings as $value => $icon) {
      switch ($pattern) {
        case 'matches':
          if ($string === $value) {
            return _bootstrap_icon($icon, $default);
          }
          break;

        case 'contains':
          if (strpos(drupal_strtolower($string), drupal_strtolower($value)) !== FALSE) {
            return _bootstrap_icon($icon, $default);
          }
          break;
      }
    }
  }

  // Return a default icon if nothing was matched.
  return _bootstrap_icon($default);
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
function _bootstrap_is_simple_string($string, $length = 250, $allowed_tags = NULL, &$html = FALSE) {
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
 * Helper function for including theme files.
 *
 * @param string $theme
 *   Name of the theme to use for base path.
 * @param string $path
 *   Path relative to $theme.
 */
function _bootstrap_include($theme, $path) {
  static $themes = array();
  if (!isset($themes[$theme])) {
    $themes[$theme] = drupal_get_path('theme', $theme);
  }
  if ($themes[$theme] && ($file = DRUPAL_ROOT . '/' . $themes[$theme] . '/' . $path) && file_exists($file)) {
    include_once $file;
  }
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
 *
 * @todo Refactor in 7.x-4.x and get rid of the deprecated settings.
 */
function _bootstrap_setting($name, $theme = NULL, $prefix = 'bootstrap') {
  $prefix = !empty($prefix) ? $prefix . '_' : '';
  $setting = theme_get_setting($prefix . $name, $theme);
  switch ($prefix . $name) {

    case 'bootstrap_forms_smart_descriptions':
      $deprecated = theme_get_setting('bootstrap_tooltip_descriptions', $theme);
      if (isset($deprecated)) {
        $setting = (int) !empty($deprecated);
      }
      break;

    case 'bootstrap_forms_smart_descriptions_limit':
      $deprecated = theme_get_setting('bootstrap_tooltip_descriptions_length', $theme);
      if (isset($deprecated)) {
        $setting = (int) !empty($deprecated);
      }
      break;

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
function &_bootstrap_get_attributes(array &$element, $property = 'attributes') {
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
function &_bootstrap_get_classes(array &$element, $property = 'attributes') {
  $attributes = &_bootstrap_get_attributes($element, $property);

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
function _bootstrap_add_class($class, array &$element, $property = 'attributes') {
  // Retrieve the element's classes.
  $classes = &_bootstrap_get_classes($element, $property);

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
function _bootstrap_remove_class($class, array &$element, $property = 'attributes') {
  // Retrieve the element's classes.
  $classes = &_bootstrap_get_classes($element, $property);

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
function _bootstrap_get_base_themes($theme_key = NULL, $include_theme_key = FALSE) {
  static $themes;
  if (!isset($theme_key)) {
    $theme_key = $GLOBALS['theme_key'];
  }
  if (!isset($themes[$theme_key])) {
    $themes[$theme_key] = array_unique(array_filter((array) _bootstrap_get_theme_info($theme_key, 'base theme')));
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
function _bootstrap_file_scan_directory($dir, $mask, array $options = array()) {
  // Retrieve cached data.
  $cid = 'theme_registry:bootstrap:files';
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
function _bootstrap_find_theme_includes($registry, $extension, $path) {
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
function _bootstrap_element_info_array_merge(array $info, array $cached) {
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
function _bootstrap_element_smart_description(array &$element, array &$target = NULL, $input_only = TRUE, $length = NULL) {
  // Determine if tooltips are enabled.
  static $enabled;
  if (!isset($enabled)) {
    $enabled = _bootstrap_setting('tooltip_enabled') && _bootstrap_setting('forms_smart_descriptions');
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
    $length = (int) _bootstrap_setting('forms_smart_descriptions_limit');
    // Disable length checking by setting it to FALSE if empty.
    if (empty($length)) {
      $length = FALSE;
    }
  }

  // Retrieve the allowed tags for smart descriptions. This is primarily used
  // for display purposes only (i.e. non-UI/UX related elements that wouldn't
  // require a user to "click", like a link).
  $allowed_tags = array_filter(array_unique(array_map('trim', explode(',', _bootstrap_setting('forms_smart_descriptions_allowed_tags') . ''))));

  // Disable length checking by setting it to FALSE if empty.
  if (empty($allowed_tags)) {
    $allowed_tags = FALSE;
  }

  $html = FALSE;
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  if (!$input_only || !empty($target['#input']) || !empty($element['#smart_description']) || !empty($target['#smart_description'])) {
    if (!empty($element['#description']) && empty($target['#attributes']['title']) && empty($target['#attributes']['data-toggle'])) {
      if (_bootstrap_is_simple_string($element['#description'], $length, $allowed_tags, $html)) {
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
        $attributes = &_bootstrap_get_attributes($target, $property);

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
 * Helper function for returning an array of Bootstrap Glyphicons.
 *
 * @param string $version
 *   The specific version of glyphicons to return. If not set, the latest
 *   BOOTSTRAP_VERSION will be used.
 *
 * @return array
 *   An associative array of icons keyed by their classes.
 */
function _bootstrap_glyphicons($version = NULL) {
  static $versions;
  if (!isset($versions)) {
    $versions = array();
    $versions['3.0.0'] = array(
      // Class => Name.
      'glyphicon-adjust' => 'adjust',
      'glyphicon-align-center' => 'align-center',
      'glyphicon-align-justify' => 'align-justify',
      'glyphicon-align-left' => 'align-left',
      'glyphicon-align-right' => 'align-right',
      'glyphicon-arrow-down' => 'arrow-down',
      'glyphicon-arrow-left' => 'arrow-left',
      'glyphicon-arrow-right' => 'arrow-right',
      'glyphicon-arrow-up' => 'arrow-up',
      'glyphicon-asterisk' => 'asterisk',
      'glyphicon-backward' => 'backward',
      'glyphicon-ban-circle' => 'ban-circle',
      'glyphicon-barcode' => 'barcode',
      'glyphicon-bell' => 'bell',
      'glyphicon-bold' => 'bold',
      'glyphicon-book' => 'book',
      'glyphicon-bookmark' => 'bookmark',
      'glyphicon-briefcase' => 'briefcase',
      'glyphicon-bullhorn' => 'bullhorn',
      'glyphicon-calendar' => 'calendar',
      'glyphicon-camera' => 'camera',
      'glyphicon-certificate' => 'certificate',
      'glyphicon-check' => 'check',
      'glyphicon-chevron-down' => 'chevron-down',
      'glyphicon-chevron-left' => 'chevron-left',
      'glyphicon-chevron-right' => 'chevron-right',
      'glyphicon-chevron-up' => 'chevron-up',
      'glyphicon-circle-arrow-down' => 'circle-arrow-down',
      'glyphicon-circle-arrow-left' => 'circle-arrow-left',
      'glyphicon-circle-arrow-right' => 'circle-arrow-right',
      'glyphicon-circle-arrow-up' => 'circle-arrow-up',
      'glyphicon-cloud' => 'cloud',
      'glyphicon-cloud-download' => 'cloud-download',
      'glyphicon-cloud-upload' => 'cloud-upload',
      'glyphicon-cog' => 'cog',
      'glyphicon-collapse-down' => 'collapse-down',
      'glyphicon-collapse-up' => 'collapse-up',
      'glyphicon-comment' => 'comment',
      'glyphicon-compressed' => 'compressed',
      'glyphicon-copyright-mark' => 'copyright-mark',
      'glyphicon-credit-card' => 'credit-card',
      'glyphicon-cutlery' => 'cutlery',
      'glyphicon-dashboard' => 'dashboard',
      'glyphicon-download' => 'download',
      'glyphicon-download-alt' => 'download-alt',
      'glyphicon-earphone' => 'earphone',
      'glyphicon-edit' => 'edit',
      'glyphicon-eject' => 'eject',
      'glyphicon-envelope' => 'envelope',
      'glyphicon-euro' => 'euro',
      'glyphicon-exclamation-sign' => 'exclamation-sign',
      'glyphicon-expand' => 'expand',
      'glyphicon-export' => 'export',
      'glyphicon-eye-close' => 'eye-close',
      'glyphicon-eye-open' => 'eye-open',
      'glyphicon-facetime-video' => 'facetime-video',
      'glyphicon-fast-backward' => 'fast-backward',
      'glyphicon-fast-forward' => 'fast-forward',
      'glyphicon-file' => 'file',
      'glyphicon-film' => 'film',
      'glyphicon-filter' => 'filter',
      'glyphicon-fire' => 'fire',
      'glyphicon-flag' => 'flag',
      'glyphicon-flash' => 'flash',
      'glyphicon-floppy-disk' => 'floppy-disk',
      'glyphicon-floppy-open' => 'floppy-open',
      'glyphicon-floppy-remove' => 'floppy-remove',
      'glyphicon-floppy-save' => 'floppy-save',
      'glyphicon-floppy-saved' => 'floppy-saved',
      'glyphicon-folder-close' => 'folder-close',
      'glyphicon-folder-open' => 'folder-open',
      'glyphicon-font' => 'font',
      'glyphicon-forward' => 'forward',
      'glyphicon-fullscreen' => 'fullscreen',
      'glyphicon-gbp' => 'gbp',
      'glyphicon-gift' => 'gift',
      'glyphicon-glass' => 'glass',
      'glyphicon-globe' => 'globe',
      'glyphicon-hand-down' => 'hand-down',
      'glyphicon-hand-left' => 'hand-left',
      'glyphicon-hand-right' => 'hand-right',
      'glyphicon-hand-up' => 'hand-up',
      'glyphicon-hd-video' => 'hd-video',
      'glyphicon-hdd' => 'hdd',
      'glyphicon-header' => 'header',
      'glyphicon-headphones' => 'headphones',
      'glyphicon-heart' => 'heart',
      'glyphicon-heart-empty' => 'heart-empty',
      'glyphicon-home' => 'home',
      'glyphicon-import' => 'import',
      'glyphicon-inbox' => 'inbox',
      'glyphicon-indent-left' => 'indent-left',
      'glyphicon-indent-right' => 'indent-right',
      'glyphicon-info-sign' => 'info-sign',
      'glyphicon-italic' => 'italic',
      'glyphicon-leaf' => 'leaf',
      'glyphicon-link' => 'link',
      'glyphicon-list' => 'list',
      'glyphicon-list-alt' => 'list-alt',
      'glyphicon-lock' => 'lock',
      'glyphicon-log-in' => 'log-in',
      'glyphicon-log-out' => 'log-out',
      'glyphicon-magnet' => 'magnet',
      'glyphicon-map-marker' => 'map-marker',
      'glyphicon-minus' => 'minus',
      'glyphicon-minus-sign' => 'minus-sign',
      'glyphicon-move' => 'move',
      'glyphicon-music' => 'music',
      'glyphicon-new-window' => 'new-window',
      'glyphicon-off' => 'off',
      'glyphicon-ok' => 'ok',
      'glyphicon-ok-circle' => 'ok-circle',
      'glyphicon-ok-sign' => 'ok-sign',
      'glyphicon-open' => 'open',
      'glyphicon-paperclip' => 'paperclip',
      'glyphicon-pause' => 'pause',
      'glyphicon-pencil' => 'pencil',
      'glyphicon-phone' => 'phone',
      'glyphicon-phone-alt' => 'phone-alt',
      'glyphicon-picture' => 'picture',
      'glyphicon-plane' => 'plane',
      'glyphicon-play' => 'play',
      'glyphicon-play-circle' => 'play-circle',
      'glyphicon-plus' => 'plus',
      'glyphicon-plus-sign' => 'plus-sign',
      'glyphicon-print' => 'print',
      'glyphicon-pushpin' => 'pushpin',
      'glyphicon-qrcode' => 'qrcode',
      'glyphicon-question-sign' => 'question-sign',
      'glyphicon-random' => 'random',
      'glyphicon-record' => 'record',
      'glyphicon-refresh' => 'refresh',
      'glyphicon-registration-mark' => 'registration-mark',
      'glyphicon-remove' => 'remove',
      'glyphicon-remove-circle' => 'remove-circle',
      'glyphicon-remove-sign' => 'remove-sign',
      'glyphicon-repeat' => 'repeat',
      'glyphicon-resize-full' => 'resize-full',
      'glyphicon-resize-horizontal' => 'resize-horizontal',
      'glyphicon-resize-small' => 'resize-small',
      'glyphicon-resize-vertical' => 'resize-vertical',
      'glyphicon-retweet' => 'retweet',
      'glyphicon-road' => 'road',
      'glyphicon-save' => 'save',
      'glyphicon-saved' => 'saved',
      'glyphicon-screenshot' => 'screenshot',
      'glyphicon-sd-video' => 'sd-video',
      'glyphicon-search' => 'search',
      'glyphicon-send' => 'send',
      'glyphicon-share' => 'share',
      'glyphicon-share-alt' => 'share-alt',
      'glyphicon-shopping-cart' => 'shopping-cart',
      'glyphicon-signal' => 'signal',
      'glyphicon-sort' => 'sort',
      'glyphicon-sort-by-alphabet' => 'sort-by-alphabet',
      'glyphicon-sort-by-alphabet-alt' => 'sort-by-alphabet-alt',
      'glyphicon-sort-by-attributes' => 'sort-by-attributes',
      'glyphicon-sort-by-attributes-alt' => 'sort-by-attributes-alt',
      'glyphicon-sort-by-order' => 'sort-by-order',
      'glyphicon-sort-by-order-alt' => 'sort-by-order-alt',
      'glyphicon-sound-5-1' => 'sound-5-1',
      'glyphicon-sound-6-1' => 'sound-6-1',
      'glyphicon-sound-7-1' => 'sound-7-1',
      'glyphicon-sound-dolby' => 'sound-dolby',
      'glyphicon-sound-stereo' => 'sound-stereo',
      'glyphicon-star' => 'star',
      'glyphicon-star-empty' => 'star-empty',
      'glyphicon-stats' => 'stats',
      'glyphicon-step-backward' => 'step-backward',
      'glyphicon-step-forward' => 'step-forward',
      'glyphicon-stop' => 'stop',
      'glyphicon-subtitles' => 'subtitles',
      'glyphicon-tag' => 'tag',
      'glyphicon-tags' => 'tags',
      'glyphicon-tasks' => 'tasks',
      'glyphicon-text-height' => 'text-height',
      'glyphicon-text-width' => 'text-width',
      'glyphicon-th' => 'th',
      'glyphicon-th-large' => 'th-large',
      'glyphicon-th-list' => 'th-list',
      'glyphicon-thumbs-down' => 'thumbs-down',
      'glyphicon-thumbs-up' => 'thumbs-up',
      'glyphicon-time' => 'time',
      'glyphicon-tint' => 'tint',
      'glyphicon-tower' => 'tower',
      'glyphicon-transfer' => 'transfer',
      'glyphicon-trash' => 'trash',
      'glyphicon-tree-conifer' => 'tree-conifer',
      'glyphicon-tree-deciduous' => 'tree-deciduous',
      'glyphicon-unchecked' => 'unchecked',
      'glyphicon-upload' => 'upload',
      'glyphicon-usd' => 'usd',
      'glyphicon-user' => 'user',
      'glyphicon-volume-down' => 'volume-down',
      'glyphicon-volume-off' => 'volume-off',
      'glyphicon-volume-up' => 'volume-up',
      'glyphicon-warning-sign' => 'warning-sign',
      'glyphicon-wrench' => 'wrench',
      'glyphicon-zoom-in' => 'zoom-in',
      'glyphicon-zoom-out' => 'zoom-out',
    );
    $versions['3.0.1'] = $versions['3.0.0'];
    $versions['3.0.2'] = $versions['3.0.1'];
    $versions['3.0.3'] = $versions['3.0.2'];
    $versions['3.1.0'] = $versions['3.0.3'];
    $versions['3.1.1'] = $versions['3.1.0'];
    $versions['3.2.0'] = $versions['3.1.1'];
    $versions['3.3.0'] = array_merge($versions['3.2.0'], array(
      'glyphicon-eur' => 'eur',
    ));
    $versions['3.3.1'] = $versions['3.3.0'];
    $versions['3.3.2'] = array_merge($versions['3.3.1'], array(
      'glyphicon-alert' => 'alert',
      'glyphicon-apple' => 'apple',
      'glyphicon-baby-formula' => 'baby-formula',
      'glyphicon-bed' => 'bed',
      'glyphicon-bishop' => 'bishop',
      'glyphicon-bitcoin' => 'bitcoin',
      'glyphicon-blackboard' => 'blackboard',
      'glyphicon-cd' => 'cd',
      'glyphicon-console' => 'console',
      'glyphicon-copy' => 'copy',
      'glyphicon-duplicate' => 'duplicate',
      'glyphicon-education' => 'education',
      'glyphicon-equalizer' => 'equalizer',
      'glyphicon-erase' => 'erase',
      'glyphicon-grain' => 'grain',
      'glyphicon-hourglass' => 'hourglass',
      'glyphicon-ice-lolly' => 'ice-lolly',
      'glyphicon-ice-lolly-tasted' => 'ice-lolly-tasted',
      'glyphicon-king' => 'king',
      'glyphicon-knight' => 'knight',
      'glyphicon-lamp' => 'lamp',
      'glyphicon-level-up' => 'level-up',
      'glyphicon-menu-down' => 'menu-down',
      'glyphicon-menu-hamburger' => 'menu-hamburger',
      'glyphicon-menu-left' => 'menu-left',
      'glyphicon-menu-right' => 'menu-right',
      'glyphicon-menu-up' => 'menu-up',
      'glyphicon-modal-window' => 'modal-window',
      'glyphicon-object-align-bottom' => 'object-align-bottom',
      'glyphicon-object-align-horizontal' => 'object-align-horizontal',
      'glyphicon-object-align-left' => 'object-align-left',
      'glyphicon-object-align-right' => 'object-align-right',
      'glyphicon-object-align-top' => 'object-align-top',
      'glyphicon-object-align-vertical' => 'object-align-vertical',
      'glyphicon-oil' => 'oil',
      'glyphicon-open-file' => 'open-file',
      'glyphicon-option-horizontal' => 'option-horizontal',
      'glyphicon-option-vertical' => 'option-vertical',
      'glyphicon-paste' => 'paste',
      'glyphicon-pawn' => 'pawn',
      'glyphicon-piggy-bank' => 'piggy-bank',
      'glyphicon-queen' => 'queen',
      'glyphicon-ruble' => 'ruble',
      'glyphicon-save-file' => 'save-file',
      'glyphicon-scale' => 'scale',
      'glyphicon-scissors' => 'scissors',
      'glyphicon-subscript' => 'subscript',
      'glyphicon-sunglasses' => 'sunglasses',
      'glyphicon-superscript' => 'superscript',
      'glyphicon-tent' => 'tent',
      'glyphicon-text-background' => 'text-background',
      'glyphicon-text-color' => 'text-color',
      'glyphicon-text-size' => 'text-size',
      'glyphicon-triangle-bottom' => 'triangle-bottom',
      'glyphicon-triangle-left' => 'triangle-left',
      'glyphicon-triangle-right' => 'triangle-right',
      'glyphicon-triangle-top' => 'triangle-top',
      'glyphicon-yen' => 'yen',
    ));
  }

  // Return a specific versions icon set.
  if (isset($version) && isset($versions[$version])) {
    return $versions[$version];
  }

  // Return the latest version.
  return $versions[BOOTSTRAP_VERSION];
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
function _bootstrap_icon($name, $default = NULL) {
  $output = NULL;
  // Ensure the icon specified is a valid Bootstrap Glyphicon.
  // @todo Supply a specific version to _bootstrap_glyphicons() when Icon API
  // supports versioning.
  if (_bootstrap_setting('button_iconize') && in_array($name, _bootstrap_glyphicons())) {
    // Attempt to use the Icon API module, if enabled and it generates output.
    if (module_exists('icon')) {
      $output = theme('icon', array('bundle' => 'bootstrap', 'icon' => 'glyphicon-' . $name));
    }
    if (empty($output)) {
      // Mimic the Icon API markup.
      $attributes = array(
        'class' => array('icon', 'glyphicon', 'glyphicon-' . $name),
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
function _bootstrap_is_button(array $element) {
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
function _bootstrap_process_theme_registry(array &$registry, $themes) {
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
        _bootstrap_registry_sort_phase_functions($registry[$hook][$phase_key], $hook, $phase, $themes);

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
function _bootstrap_registry_sort_phase_functions(array &$functions, $hook, $phase, array $themes) {
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
function _bootstrap_process_theme_registry_suggestions(array &$registry, $themes) {
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
function _bootstrap_book_children(array $book_link) {
  // Rebuild entire menu tree for the book.
  $tree = menu_build_tree($book_link['menu_name']);
  $tree = menu_tree_output($tree);

  // Fix the theme hook suggestions.
  _bootstrap_book_fix_theme_hooks($book_link['nid'], $tree);

  // Return the rendered output.
  return drupal_render($tree);
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
function _bootstrap_book_fix_theme_hooks($bid, array &$element, $level = 0) {
  $hook = $level === 0 ? $bid : 'sub_menu__' . $bid;
  $element['#theme_wrappers'] = array('menu_tree__book_toc__' . $hook);
  foreach (element_children($element) as $child) {
    $element[$child]['#theme'] = 'menu_link__book_toc__' . $hook;
    // Iterate through all child menu items as well.
    if (!empty($element[$child]['#below'])) {
      _bootstrap_book_fix_theme_hooks($bid, $element[$child]['#below'], ($level + 1));
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
function _bootstrap_table_add_classes(array &$classes, array &$variables) {
  $context = $variables['context'];

  // Generic table class for all tables.
  $classes[] = 'table';

  // Bordered table.
  if (!empty($context['bordered']) || (!isset($context['bordered']) && _bootstrap_setting('table_bordered'))) {
    $classes[] = 'table-bordered';
  }

  // Condensed table.
  if (!empty($context['condensed']) || (!isset($context['condensed']) && _bootstrap_setting('table_condensed'))) {
    $classes[] = 'table-condensed';
  }

  // Hover rows.
  if (!empty($context['hover']) || (!isset($context['hover']) && _bootstrap_setting('table_hover'))) {
    $classes[] = 'table-hover';
  }

  // Striped rows.
  if (!empty($context['striped']) || (!isset($context['striped']) && _bootstrap_setting('table_striped'))) {
    $classes[] = 'table-striped';
  }

  // Responsive table.
  $variables['responsive'] = isset($context['responsive']) ? $context['responsive'] : _bootstrap_setting('table_responsive');
}
