<?php
/**
 * @file
 * Circle theme logic and magic.
 * See function comments to understand how they work.
 */

/**
 * Supplementary function for loading additional files.
 */
function _circle_load_include($type, $theme, $name = NULL) {
  if (empty($name)) {
    $name = $theme;
  }
  $file = './' . drupal_get_path('theme', $theme) . "/$name.$type";
  if (is_file($file)) {
    require_once $file;
  }
  else {
    return FALSE;
  }
}

// Define path to Circle theme.
$path_circle = drupal_get_path('theme', 'circle');

// Include additional theme files.
_circle_load_include('inc', 'circle', 'includes/icons.template');
_circle_load_include('inc', 'circle', 'includes/form.template');
_circle_load_include('inc', 'circle', 'includes/menu.template');
_circle_load_include('inc', 'circle', 'includes/css.template');
_circle_load_include('inc', 'circle', 'includes/theme-functions.template');

/**
 * Implements theme_preprocess().
 */
function circle_preprocess(&$vars, $hook) {

  if ($hook == "html") {
    global $theme;
    global $base_url;
    $path = drupal_get_path('theme', $theme);
    $path_circle = drupal_get_path('theme', 'circle');

    // For third-generation iPad with high-resolution Retina display:
    $appletouchicon = '<link rel="apple-touch-icon" sizes="144x144" href="' . $base_url . '/' . $path . '/apple-touch-icon-144x144.png">';
    // For iPhone with high-resolution Retina display:
    $appletouchicon .= '<link rel="apple-touch-icon" sizes="114x114" href="' . $base_url . '/' . $path . '/apple-touch-icon-114x114.png">' . "\n";
    // For first- and second-generation iPad:
    $appletouchicon .= '<link rel="apple-touch-icon" sizes="72x72" href="' . $base_url . '/' .  $path . '/apple-touch-icon-72x72.png">' . "\n";
    // For non-Retina iPhone, iPod Touch, and Android 2.1+ devices:
    $appletouchicon .= '<link rel="apple-touch-icon" href="' . $base_url . '/' .  $path . '/apple-touch-icon.png">' . "\n";
    $appletouchicon .= '<link rel="apple-touch-startup-image" href="' . $base_url . '/' . $path . '/apple-startup.png">' . "\n";

    // Add icons to html variabes. We'll print them in our html.tpl.php
    $vars['appletouchicon'] = $appletouchicon;

    // Add modernizer from CDN.
    if (theme_get_setting('circle_modernizr')) {
      drupal_add_js('http://cdnjs.cloudflare.com/ajax/libs/modernizr/2.0.6/modernizr.min.js', 'external');
    }

    // Add html5shiv from CDN.
    if (theme_get_setting('circle_js_htmlshiv')) {
      drupal_add_js('http://html5shiv.googlecode.com/svn/trunk/html5.js', 'external');
    }

    // Add Placeholder.js
    if (theme_get_setting('circle_js_placeholder')) {
      drupal_add_js($path . '/js/placeholder.js');
    }
    // Add LESS javascript from CDN.
    if (theme_get_setting('circles_enable_less')) {
      drupal_add_js('http://cdnjs.cloudflare.com/ajax/libs/less.js/1.4.1/less.min.js', 'external');
    }
    // Add Normalize css.
    if (theme_get_setting('circle_css_normalize')) {
      drupal_add_css($path_circle . '/css/normalize.css', array(
        'group' => CSS_THEME,
        'every_page' => TRUE,
        'weight' => -20,
      ));
    }

    // Add IE stylesheets.
    if (theme_get_setting('circle_css_ie')) {
      drupal_add_css($path . '/css/ie.css',
        array(
          'group' => CSS_THEME,
          'weight' => 115,
          'browsers' => array('IE' => 'IE', '!IE' => FALSE),
          'preprocess' => FALSE,
        )
      );
      drupal_add_css($path . '/css/ie9.css',
        array(
          'group' => CSS_THEME,
          'weight' => 120,
          'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE),
          'preprocess' => FALSE,
        )
      );
      drupal_add_css($path . '/css/ie8.css',
        array(
          'group' => CSS_THEME,
          'weight' => 125,
          'browsers' => array('IE' => 'lte IE 8', '!IE' => FALSE),
          'preprocess' => FALSE,
        )
      );
    }

    // Add Bootstrap styles.
    if (theme_get_setting('circle_css_bootstrap')) {
      $version = theme_get_setting('circle_css_bootstrap_version');
      $css_link = '//netdna.bootstrapcdn.com/bootstrap/' . $version . '/css/bootstrap.min.css';
      drupal_add_css($css_link, 'external');
    }
    // Add Bootstrap javaScript.
    if (theme_get_setting('circle_js_bootstrap')) {
      $version = theme_get_setting('circle_js_bootstrap_version');
      $js_link = '//netdna.bootstrapcdn.com/bootstrap/' . $version . '/js/bootstrap.min.js';
      drupal_add_js($js_link, 'external');
    }
    // Add Foundation styles.
    if (theme_get_setting('circle_css_foundation')) {
      $version = theme_get_setting('circle_css_foundation_version');
      $css_link = '//cdn.jsdelivr.net/foundation/' . $version . '/css/foundation.min.css';
      drupal_add_css($css_link, 'external');
    }
    // Add Foundation javaScript.
    if (theme_get_setting('circle_js_foundation')) {
      $version = theme_get_setting('circle_js_foundation_version');
      $js_link = '//cdnjs.cloudflare.com/ajax/libs/foundation/' . $version . '/js/foundation.min.js';
      drupal_add_js($js_link, 'external');
    }

    // Include files locally if Libraries exist.
    if (module_exists('libraries')) {
      // Add modernizer locally.
      if (theme_get_setting('circle_modernizr_local')) {
        if ($modernizer = libraries_get_path('modernizr')) {
          drupal_add_js($modernizer . '/modernizr.min.js');
        }
      }
      // Add Bootstrap CSS locally.
      if (theme_get_setting('circle_css_bootstrap_local')) {
        if ($bootstrap = libraries_get_path('bootstrap')) {
          drupal_add_css($bootstrap . '/css/bootstrap.min.css', array(
            'group' => CSS_THEME,
            'every_page' => TRUE,
            'weight' => -20,
          ));
        }
      }
      // Add Bootstrap JS locally.
      if (theme_get_setting('circle_js_bootstrap_local')) {
        if ($bootstrap = libraries_get_path('bootstrap')) {
          drupal_add_js($bootstrap . '/js/bootstrap.min.js');
        }
      }
      // Add Foundation CSS locally.
      if (theme_get_setting('circle_css_foundation_local')) {
        if ($foundation = libraries_get_path('foundation')) {
          drupal_add_css($foundation . '/css/foundation.min.css', array(
            'group' => CSS_THEME,
            'every_page' => TRUE,
            'weight' => -18,
          ));
        }
      }
      // Add Foundation JS locally.
      if (theme_get_setting('circle_js_foundation_local')) {
        if ($foundation = libraries_get_path('foundation')) {
          drupal_add_js($foundation . '/js/foundation.min.js');
        }
      }
      if (theme_get_setting('circle_js_htmlshiv_local')) {
        if ($htmlshiv = libraries_get_path('htmlshiv')) {
          drupal_add_js($htmlshiv . '/js/html5.js');
        }
      }
    }

    // Add circle layout styles.
    if (theme_get_setting('circle_css_layout')) {
      drupal_add_css($path_circle . '/css/circle-layout.css', array(
        'group' => CSS_THEME,
        'every_page' => TRUE,
        'weight' => -17,
      ));
    }
    // Add circle base styles.
    if (theme_get_setting('circle_css_circlestyles')) {
      drupal_add_css($path_circle . '/css/circle.css', array(
        'group' => CSS_THEME,
        'every_page' => TRUE,
        'weight' => -16,
      ));
    }

    // Less support in theme info file.
    if (theme_get_setting('circles_enable_less')) {
      // Make a list of base themes and the current theme.
      $themes = $GLOBALS['base_theme_info'];
      $themes[] = $GLOBALS['theme_info'];
      $less_styles = array();
      foreach (array_keys($themes) as $key) {
        $theme_path = dirname($themes[$key]->filename) . '/';
        if (isset($themes[$key]->info['stylesheets-less'])) {
          foreach ($themes[$key]->info['stylesheets-less'] as $stylesheet) {
            foreach ($stylesheet as $style) {
              // Add each conditional stylesheet.
              $less_styles[] = $theme_path . $style;
            }
          }
        }
      }
      $less_links = array_unique($less_styles);
      $less_files = '';
      foreach ($less_links as $less_style) {
        if (!empty($less_style)) {
          $less_files .= '<link rel="stylesheet/less" type="text/css" href="' . $less_style . '">';
        }
      }
      // Add less stylesheets to html variables.
      $vars['less_styles'] = $less_files;
    }
  }
  elseif ($hook == "node") {
    // Find if node is published or not.
    $unpublished = TRUE;
    if ($vars['status'] == 1) {
      $unpublished = FALSE;
    }
    $vars['unpublished'] = $unpublished;
  }
  elseif ($hook == "page") {
    // If page is a panel, add an additional variable.
    if (module_exists('panels')) {
      $panel_page = panels_get_current_page_display();
      $vars['is_panel_page'] = ($panel_page) ? TRUE : FALSE;
    } else {
      $vars['is_panel_page'] = FALSE;
    }
    // TODO: Page title prefix and suffix from node.
  }
}

/**
 * Override or insert variables into the html template.
 */
function circle_preprocess_html(&$variables, $hook) {
  // Attributes for html element.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );
}

/**
 * Implements theme_html_head_alter().
 */
function circle_html_head_alter(&$elements) {
  // Optimize the mobile viewport.
  $elements['circle_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width, initial-scale=1.0',
    ),
  );
}

/**
 * Override or insert variables into the html templates.
 */
function circle_process_html(&$variables, $hook) {
  // Flatten out html_attributes.
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
}

/**
 * Removing old XHTML attributes.
 */
function circle_process_html_tag(&$vars) {
  $tag = &$vars['element'];
  if ($tag['#tag'] == 'style' || $tag['#tag'] == 'script') {
    // Remove redundant type attribute and CDATA comments.
    unset($tag['#attributes']['type'], $tag['#value_prefix'], $tag['#value_suffix']);

    // Remove media="all" but leave others unaffected.
    if (isset($tag['#attributes']['media']) && $tag['#attributes']['media'] === 'all') {
      unset($tag['#attributes']['media']);
    }
  }
}

/**
 * Implements hook_theme_registry_alter().
 */
function circle_theme_registry_alter(&$theme_registry) {
  if (isset($theme_registry['menu_tree'])) {
    if ($theme_registry['menu_tree']['preprocess functions'][0] == 'template_preprocess_menu_tree') {
      $theme_registry['menu_tree']['preprocess functions'][0] = '_circle_preprocess_menu_tree';
    }
  }
}

/**
 * Using cdn jquery instead of local for faster page load.
 */
function circle_js_alter(&$js) {
  if (theme_get_setting('circle_js_jquerycdn')) {
    // Get jQuery version from theme settings.
    $version = check_plain(theme_get_setting('circle_js_jquerycdn_version'));
    if (isset($js['misc/jquery.js'])) {
      // Rewrite drupal core jquery to CDN.
      $js['misc/jquery.js']['data'] = 'http://ajax.googleapis.com/ajax/libs/jquery/' . $version . '/jquery.min.js';
      $js['misc/jquery.js']['type'] = 'external';
      $js['misc/jquery.js']['weight'] = -100;
    }
  }

  if (theme_get_setting('circle_footer_js')) {
    // Force all scripts to the footer, unless they are explicit about being in
    // the header.
    foreach ($js as &$jscript) {
      $jscript['scope'] = (isset($jscript['force header']) && $jscript['force header']) ? 'header' : 'footer';
    }
  }

  // Compress all js to 1 file.
  if (theme_get_setting('circle_js_onefile')) {
    uasort($js, 'drupal_sort_css_js');
    $i = 0;
    foreach ($js as $name => $script) {
      $js[$name]['weight'] = $i++;
      $js[$name]['group'] = JS_DEFAULT;
      $js[$name]['every_page'] = FALSE;
    }
  }
}

/**
 * Add image style class name to rendered image.
 */
function circle_preprocess_image_style(&$variables) {
  $styleclass = 'style-' . $variables['style_name'];
  $variables['attributes']['class'][] = $styleclass;
}

/**
 * Alters the default Panels render callback so it removes the panel separator.
 */
function circle_panels_default_style_render_region($variables) {
  return implode('', $variables['panes']);
}

/**
 * Theme callback for stack layout.
 */
function circle_panels_frame_stack($vars) {
  $output = '';
  $frame_counter = 0;
  foreach ($vars['frames'] as $name => $content) {
    // Add frame position number to class.
    $output .= '<div class="frame clearfix frame-' . $name . ' frame-item-' . $frame_counter . '">' . $content . '</div>';
    $frame_counter++;
  }
  return '<div' . $vars['attributes'] . '>' . $output . '</div>';
}

/**
 * Moving breadcrumb separator to theme settings.
 */
function circle_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    if (theme_get_setting('breadcrumb_append_current')) {
      $breadcrumb[] = drupal_get_title();
    }
    if (theme_get_setting('breadcrumb_hide_single')) {
      if (count($breadcrumb) == 1) {
        return '';
      }
    }
    $separator = '<span class="sep">' . check_plain(theme_get_setting('breadcrumb_separator')) . '</span>';
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $output .= '<div class="breadcrumb clearfix">';
    $array_size = count($breadcrumb);
    $i = 0;
    while ($i < $array_size) {
      $output .= '<span class="crumb-item crumb-item-' . $i;
      if ($i == 0) {
        $output .= ' first';
      }
      if ($i + 1 == $array_size) {
        $output .= ' last';
      }
      $output .= '">' . $breadcrumb[$i] . '</span>';
      if ($i + 1 != $array_size) {
        $output .= $separator;
      }
      $i++;
    }
    $output .= '</div>';
    return $output;
  }
}

/**
 * Returns HTML for a marker for new or updated content.
 */
function circle_mark($variables) {
  $type = $variables['type'];

  if ($type == MARK_NEW) {
    return ' <mark class="new">' . t('new') . '</mark>';
  }
  elseif ($type == MARK_UPDATED) {
    return ' <mark class="updated">' . t('updated') . '</mark>';
  }
}

/**
 * Implements template_preprocess_panels_pane().
 */
function circle_preprocess_panels_pane(&$vars) {
  if (isset($vars['title']) && !empty($vars['title'])) {
    // If pane has title add hook suggestion with the title.
    $pane_title = strtolower('panels_pane__' . str_replace(' ', '_', $vars['title']));
    $vars['theme_hook_suggestions'][] = check_plain($pane_title);
  }
  if (isset($vars['classes_array'][3])) {
    $classes = '';
    for ($i = 3; $i < count($vars['classes_array']); $i++) {
      if (strpos($vars['classes_array'][$i], 'pane-') !== 0) {
        $classes .= check_plain($vars['classes_array'][$i]);
      }
    }
    // If pane has additional classes, add them as hook suggestion.
    if ($classes != '') {
      $pane_classes = explode(' ', $classes);
      $single_class = check_plain($pane_classes[0]);
      $vars['theme_hook_suggestions'][] = 'panels_pane__class__' . $single_class;
      $vars['theme_hook_suggestions'][] = 'panels_pane__class__' . str_replace(' ', '_', $classes);
    }
  }
  if (isset($vars['title_class'])) {
    $vars['title_attributes_array']['class'][] = $vars['title_class'];
  }
}

/**
 * Implements hook_preprocess_date_display_single().
 */
function circle_preprocess_date_display_single(&$variables) {
  // Add format type to date field.
  $variables['attributes']['dateformat'][] = $variables['dates']['format'];
}

/**
 * Override theme_field().
 */
function circle_field($variables) {
  $output = '';
  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }
  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    // Allow adding classes to theme_field items.
    preg_match('/class\=\"(.*)\"/i', $variables['item_attributes'][$delta], $match);
    if (isset($match[1])) {
      $classes .= ' ' . $match[1];
    }
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';
  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';
  return $output;
}
