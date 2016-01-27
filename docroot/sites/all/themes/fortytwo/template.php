<?php

/**
 * @file
 * Template file for the fortytwo theme.
 */

require_once dirname(__FILE__) . '/includes/fortytwo.inc';

/**
 * Override or insert variables into the html templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 * @param string $hook
 *   The name of the template being rendered ("html" in this case.)
 */
function fortytwo_preprocess_html(&$variables, $hook) {
  fortytwo_load_debuggers();
  global $theme_key;
  $variables['staticpath'] = fortytwo_get_staticpath(TRUE, $theme_key);
  $variables['classes_array'][] = theme_get_setting('ft_layout_style');

  $variables['grid'] = theme_get_setting('ft_show_grid');
  ($variables['grid']) ? $variables['classes_array'][] = 'show-grid' : $variables['classes_array'][] = '';

  $variables['responsive_identifier'] = theme_get_setting('ft_show_responsive_identifier');
  ($variables['responsive_identifier']) ? $variables['classes_array'][] = 'show-responsive-identifier' : $variables['classes_array'][] = '';

  $variables['classes_array'][] = theme_get_setting('ft_layout_responsive');

  // Change the mime type of the favicon to amek it work in all browsers.
  $favicon = theme_get_setting('favicon');
  $type = 'image/x-icon';
  drupal_add_html_head_link(
    array(
      'rel'  => 'shortcut icon',
      'href' => drupal_strip_dangerous_protocols($favicon),
      'type' => $type,
    )
  );

  // Add the profile to the head as link element.
  drupal_add_html_head_link(
    array(
      'rel'  => 'profile',
      'href' => $variables['grddl_profile'],
    )
  );
  unset($variables['grddl_profile']);
}

/**
 * Implements hook_css_alter().
 */
function fortytwo_css_alter(&$css) {
  global $theme_info;
  // Disable css if base theme is fortytwo
  if (isset($theme_info->base_themes) && isset($theme_info->base_themes['fortytwo'])) {
    $keys = array_keys($css);
    foreach ($keys as $key) {
      if (strstr($key, 'fortytwo/static/css/fortytwo.css')) {
        unset($css[$key]);
      }
    }
  }
}

/**
 * Implements hook_page_alter().
 */
function fortytwo_page_alter(&$page) {
  fortytwo_get_theme();
}

/**
 * Implements hook_js_alter().
 *
 * Overrides collapse.js with our own collapse.js functionality. The implementation
 * from Drupal has a small overflow bug when a fieldset is collapsed and re-opened.
 * Added fix on line #20.
 */
function fortytwo_js_alter(&$javascript) {
  if (isset($javascript['misc/collapse.js'])) {
    $javascript['misc/collapse.js']['data'] = drupal_get_path('theme', 'fortytwo') . '/static/js/lib/patches/collapse.js';
  }
}

/**
 * Override or insert variables into the node templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 * @param string $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function fortytwo_preprocess_node(&$variables, $hook) {
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }

  $variables['classes_array'][] = $variables['type'];
  $classes = array($variables['type']);
  if (in_array('contextual-links-region', $variables['classes_array'])) {
    $classes[] = 'contextual-links-region';
  }
  $variables['classes_array'] = $classes;
}


/**
 * Override or insert variables into the field templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 * @param string $hook
 *   The name of the template being rendered ("node" in this case.)
 */
function fortytwo_preprocess_field(&$variables, $hook) {
  $element = $variables['element'];

  // There's some overhead in calling check_plain() so only call it if the label
  // variable is being displayed. Otherwise, set it to NULL to avoid PHP
  // warnings if a theme implementation accesses the variable even when it's
  // supposed to be hidden. If a theme implementation needs to print a hidden
  // label, it needs to supply a preprocess function that sets it to the
  // sanitized element title or whatever else is wanted in its place.
  $variables['label_hidden'] = ($element['#label_display'] == 'hidden');
  if (empty($variables['label'])) {
    $variables['label'] = $variables['label_hidden'] ? NULL : check_plain($element['#title']);
  }

  // We want other preprocess functions and the theme implementation to have
  // fast access to the field item render arrays. The item render array keys
  // (deltas) should always be a subset of the keys in #items, and looping on
  // those keys is faster than calling element_children() or looping on all keys
  // within $element, since that requires traversal of all element properties.
  $variables['items'] = array();
  foreach ($element['#items'] as $delta => $item) {
    if (!empty($element[$delta])) {
      $variables['items'][$delta] = $element[$delta];
    }
  }

  // Add default CSS classes. Since there can be many fields rendered on a page,
  // save some overhead by calling strtr() directly instead of
  // drupal_html_class().
  $variables['field_name_css'] = strtr($element['#field_name'], '_', '-');
  $variables['field_name_css'] = preg_replace('/field-/', '', $variables['field_name_css'], 1);
  $variables['classes_array'] = array(
    $variables['field_name_css'],
  );
}

/**
 * Implements theme_field().
 *
 * Clean up the HTML output for fields.
 */
function fortytwo_field($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="label">' . $variables['label'] . '</div>';
  }

  // Render the items.
  foreach ($variables['items'] as $item) {
    $output .= drupal_render($item);
  }

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}


/**
 * Return a themed breadcrumb trail.
 *
 * @param array $variables
 *   - title: An optional string to be used as a navigational heading to give
 *     context for breadcrumb links to screen-reader users.
 *   - title_attributes_array: Array of HTML attributes for the title. It is
 *     flattened into a string within the theme function.
 *   - breadcrumb: An array containing the breadcrumb links.
 *
 * @return string
 *   A string containing the breadcrumb output.
 */
function fortytwo_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  $output = '';

  // Determine if we are to display the breadcrumb.
  $show_breadcrumb = theme_get_setting('ft_breadcrumb');
  if ($show_breadcrumb == 'yes') {
    // Retrieve the breadcrumb settings.
    $show_breadcrumb_home = theme_get_setting('ft_breadcrumb_home');
    $breadcrumb_separator = theme_get_setting('ft_breadcrumb_separator');
    $add_trailing = theme_get_setting('ft_breadcrumb_trailing');
    $add_title = theme_get_setting('ft_breadcrumb_title');

    if (!$show_breadcrumb_home) {
      array_shift($breadcrumb);
    }

    // Return the breadcrumb with separators.
    if (!empty($breadcrumb)) {
      // Provide a navigational heading to give context for breadcrumb links to
      // screen-reader users.
      if (empty($variables['title'])) {
        $variables['title'] = t('You are here');
      }
      // Unless overridden by a preprocess function, make the heading invisible.
      if (!isset($variables['title_attributes_array']['class'])) {
        $variables['title_attributes_array']['class'][] = 'element-invisible';
      }

      if ($add_title) {
        $item = menu_get_item();
        if (!empty($item['tab_parent'])) {
          // If we are on a non-default tab, use the tab's title.
          $breadcrumb[] = check_plain($item['title']);
        }
        else {
          $breadcrumb[] = drupal_get_title();
        }
      }

      end($breadcrumb);
      $lastkey = key($breadcrumb);

      foreach ($breadcrumb as $key => $item) {
        $breadcrumb[$key] = array(
          'data' => $item,
          'data-icon' => $breadcrumb_separator,
        );
      }

      if (empty($add_trailing) && $lastkey == $key) {
        unset($breadcrumb[$key]['data-icon']);
      }

      // Build the breadcrumb trail.
      $output = '<nav class="breadcrumb" role="navigation">';
      $output .= '<h2' . drupal_attributes($variables['title_attributes_array']) . '>' . $variables['title'] . '</h2>';
      $output .= theme('item_list', array(
        'items' => $breadcrumb,
        'type' => 'ol',
      ));
      $output .= '</nav>';
    }
  }

  return $output;
}

/**
 * Implements hook_preprocess_block().
 *
 * Clean up the div classes for blocks
 */
function fortytwo_preprocess_block(&$variables) {
  $classes = array();
  if (in_array('block', $variables['classes_array'])) {
    $classes[] = 'block';
  }
  if (in_array('contextual-links-region', $variables['classes_array'])) {
    $classes[] = 'contextual-links-region';
  }
  $variables['classes_array'] = $classes;

  // Replace first occurrence of block- to clean up the ID's
  if (substr($variables['block_html_id'], 0, 6) === 'block-') {
    $variables['block_html_id'] = substr($variables['block_html_id'], 6);
  }
}

/**
 * Implements hook_preprocess_views_view().
 *
 * Clean up the div classes for views
 */
function fortytwo_preprocess_views_view(&$variables) {
 $dom_id_class = 'view-dom-id-' . $variables['view']->dom_id;
 $user_defined_class = $variables['css_class'];
 $variables['classes_array'] = array(
   'list-' . $variables['view']->name,
   'view',
   $user_defined_class,
   $dom_id_class
 );
}


/**
 * Implements hook_preprocess_entity().
 *
 * Clean up the div classes for field_collections
 */
function fortytwo_preprocess_entity(&$variables) {
  if ($variables['entity_type'] == 'field_collection_item') {
    $variables['classes_array'] = array(
      str_replace('field-collection-item-field-', '', $variables['classes_array'][2]),
    );
  }
  if ($variables['entity_type'] == 'registration') {
    $variables['classes_array'] = array(
      $variables['elements']['#entity']->type,
    );
  }
}

/**
 * Implements theme_tableselect().
 */
function fortytwo_tableselect($variables) {
  $sticky = TRUE;
  $element = $variables['element'];
  $rows = array();
  $header = $element['#header'];
  if (!empty($element['#options'])) {
    // Generate a table row for each selectable item in #options.
    foreach (element_children($element) as $key) {
      $row = array();

      $row['data'] = array();
      if (isset($element['#options'][$key]['#attributes'])) {
        $row += $element['#options'][$key]['#attributes'];
      }
      // Render the checkbox / radio element.
      $row['data'][] = drupal_render($element[$key]);

      // As theme_table only maps header and row columns by order, create the
      // correct order by iterating over the header fields.
      foreach ($element['#header'] as $fieldname => $title) {
        $row['data'][] = $element['#options'][$key][$fieldname];
      }
      $rows[] = $row;
    }
    // Add an empty header or a "Select all" checkbox to provide room for the
    // checkboxes/radios in the first table column.
    if ($element['#js_select']) {
      // Add a "Select all" checkbox.
      drupal_add_js('misc/tableselect.js');
      array_unshift($header, array('class' => array('select-all')));
    }
    else {
      // Add an empty header when radio buttons are displayed or a "Select all"
      // checkbox is not desired.
      array_unshift($header, '');
    }
  }

  if (isset($element['#sticky'])) {
    $sticky = $element['#sticky'];
  }

  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => $element['#empty'],
    'attributes' => $element['#attributes'],
    'sticky' => $sticky,
  ));
}
