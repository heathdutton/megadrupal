<?php

/**
 * @file
 * Provides preprocess logic and other utilities to Bootstrap and child themes.
 *
 * @todo
 *
 *    -- Build settings page
 *    -- Duplicate form-related items for Webform (*sigh*)
 */


// Includes:
require_once('includes/form.inc.php');


// Constants:
define('BH_BOOTSTRAP_THEME_SETTINGS_COLUMN_VARIABLE_PATTERN', 'bh_bootstrap_%s_size');
define('BH_BOOTSTRAP_THEME_SETTINGS_ROW_VARIABLE_PATTERN', 'bh_bootstrap_%s_size');
define('BH_BOOTSTRAP_PAGE_TEMPLATE_VARIABLE_PATTERN', '%s_size');
define('BH_BOOTSTRAP_GRID_COLUMNS', 12);


/**
 * Replaces any character not a digit, letter or dash with a dash. Used to make
 * (potentially) user-entered strings safe for output.
 *
 * @param string $original
 *  -- the incoming string
 * @return string
 *  -- the sanitized string
 */
function _bh_bootstrap_css_safe($original) {
  return strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $original));
} // _bh_bootstrap_css_safe()


/**
 * Returns an array of regions available in the theme whose keys begin with the
 * string in $region_type--usually 'sidebar_' or 'row_' (or an empty array).
 *
 * @param array $region_types
 *    The types of regions--currently the theme settings and template.php only
 *    anticipate values of 'sidebar_' or 'row_'.
 * @param string $theme_override
 *    The theme whose row or sidebar regions are to be found (if not provided,
 *    the default theme will be used).
 * @return array $theme_regions
 *    The array of theme regions, regardless of how many items it contains.
 */
function _bh_bootstrap_get_multiple_regions($region_types = array('sidebar_'), $theme_override = NULL) {
  // Find out the current theme--or at least the theme the caller is interested
  // in:
  $current_theme = $theme_override ? $theme_override : variable_get('theme_default', $theme_override);
  // Retrieve information about the available regions in this theme:
  $regions = system_region_list($current_theme);
  // Go through the regions and find all those regions whose key begins with
  // 'sidebar_' etc; make a new array containing only those:
  $theme_regions = array();
  // Loop through the region types:
  foreach ($region_types as $region_type) {
    foreach ($regions as $key => $name) {
      if (strpos($key, $region_type) === 0) {
        $theme_regions[$region_type][$key] = $name;
      }
    }
  }
  // Return the result--whatever it is:
  return $theme_regions;
} // _bh_bootstrap_get_multiple_regions()


/**
 * A DRY function that just applies the appropriate markup and class attribute
 * to form element descriptions.
 *
 * @param string $description The form item description.
 */
function _bh_bootstrap_help_block($description) {
  return $description != '' ? sprintf('<p class="help-block">%s</p>', $description) : '';
} // _bh_bootstrap_help_block()


/**
 * Implements theme_breadcrumb().
 *
 * @todo
 *    -- Think about WTF we can do with breadcrumbs if the menu structure is
 *       using Boostrap's crazy non-menu menu items (i.e. dropdown toggle links).
 */
function bh_bootstrap_breadcrumb($variables) {
  // Convenience variable:
  $breadcrumb = $variables['breadcrumb'];
  // If we have any breadcrumbs:
  if (!empty($breadcrumb)) {
    // Convert 'em to a string:
    $breadcrumbs = implode(' <span class="divider">/</span> ', $breadcrumb);
    // Build a heading--here at least, we're following the D7 convention of
    // accompanying menus with invisible headings to aid in text-only navigation:
    $heading = t('You are here');
    // Pattern for output:
    $output_pattern = '<h2 class="element-invisible">%s</h2><ul class="breadcrumb">%s</ul>';
    // Return the markup:
    return sprintf($output_pattern, $heading, $breadcrumbs);
  }
} // bh_bootstrap_breadcrumb()

/**
 * Implements hook_form_FORMID_alter().
 *
 * @todo
 *    -- attempt to get the two selects side-by-side; difficult because we have
 *       no way to target the select elements' html wrappers from here...
 */
function bh_bootstrap_form_dblog_filter_form_alter(&$form, &$form_state, $form_id) {
  //$form['filters']['actions']['#attributes']['class'][] = 'row-fluid';
  //$form['filters']['status']['#attributes']['class'][] = 'span5';
} // bh_bootstrap_form_dblog_filter_form_alter


/**
 * Implements template_html_head_alter().
 *
 * @see bh_bootstrap_preprocess_html()
 */
function bh_bootstrap_html_head_alter(&$variables) {
  // We need to use html head alter because Drupal can't resist putting its own
  // favicon in. ^@@$^!@#$ druplicon...
  global $theme;
  if ($theme == 'bh_bootstrap') {
    if (theme_get_setting('default_favicon')) {
      foreach ($variables as $key => $value) {
        if (strpos($key, 'misc/favicon.ico') !== FALSE) {
          $variables[$key]['#attributes']['href'] = url(drupal_get_path('theme', 'bh_bootstrap') . '/images/icons/favicon.ico');
        }
      }
    }
  }
} // bh_bootstrap_html_head_alter()


/**
 * Implements theme_menu_link().
 *
 * @see bh_bootstrap_menu_tree__main_menu()
 * @see bh_bootstrap_preprocess_page()
 * @see http://twitter.github.com/bootstrap/components.html#navs
 */
function bh_bootstrap_menu_link__main_menu($variables) {
  // Store some values that'll be used more than once:
  $current_path = current_path();

  $element = $variables['element'];
  $sub_menu = '';

  if ($current_path == $element['#href'] || (drupal_is_front_page() && $element['#href'] == '<front>')) {
    $element['#attributes']['class'][] = 'active';
  }

  // Build the link:
  //
  // This will be tricky. The problem is that Drupal's always going to add the
  // base url. Ordinarily, this is a good thing, but for the <bst-dropdown> href
  // attribute, we only want this: href="#". Instead, we're liable to get
  // this: href="/#" which is quite different (and usually wrong).
  //
  // Maybe we should just try to use (and work around) the Special Menu Items
  // module.
  //
  // There's also the whole issue of WTF to do in the case of breadcrumbs if we
  // have these useless links. Why bootstrap, why?
  //
  // Figure out the ordinary link:
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);

  // But in special cases, modify the link output--this should only be possible
  // if the bh_bootstrap_menupath module is present and enabled (for this reason,
  // we don't actually use module_exists to check:
  switch ($element['#href']) {
    case '<bst-divider>':
      $element['#attributes']['class'][] = 'divider';
      $output = '';
      break;
    case '<bst-dropdown>':
      // Classes for the list item:
      $element['#attributes']['class'][] = 'dropdown';
      // Add the dropdown's visual indicator:
      $element['#title'] .= ' <b class="caret"></b>';
      $element['#localized_options']['html'] = TRUE;
      // Attributes for the link itself:
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      // Render the element--but do it differently:
      $element['#below']['#theme_wrappers'] = array('menu_tree__bh_bootstrap_dropdown');
      $sub_menu = drupal_render($element['#below']);
      $output = preg_replace('/href="[^"]+"/', 'href="#"', l($element['#title'], $element['#href'], $element['#localized_options']));
      break;
    case '<bst-nav-header>':
      $element['#attributes']['class'][] = 'nav-header';
      $output = $element['#title'];
      break;
  }

  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
} // bh_bootstrap_menu_link()


/**
 * Implements theme_menu_local_tasks().
 *
 * @todo
 *  -- Try to come up with something a little more clever than using pills as
 *     subnav for tabs. Why, oh why, can't Drupal just do this as a proper nested
 *     list using theme_item_list()? [In case it's not clear--the problem here
 *     is that, at any given time, when any given tab is active, we only have
 *     access to the secondary items FOR THE CURRENT TAB; this prevents us from
 *     just building an appropriately structured array and running it through
 *     theme('item_list', …) etc.]
 */
function bh_bootstrap_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="nav nav-tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="nav nav-pills secondary">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
} // bh_bootstrap_menu_local_tasks()


/**
 * Implements theme_menu_tree().
 *
 * @see bh_bootstrap_menu_link()
 */
function bh_bootstrap_menu_tree($variables) {
  return '<ul>' . $variables['tree'] . '</ul>';
} // bh_bootstrap_menu_tree()


/**
 * Implements theme_menu_tree().
 *
 * This implementation is specific to submenus of the main menu.
 *
 * @see bh_bootstrap_menu_tree__main_menu()
 * @see bh_bootstrap_menu_link__main_menu()
 */
function bh_bootstrap_menu_tree__bh_bootstrap_dropdown($variables) {
  return '<ul class="dropdown-menu">' . $variables['tree'] . '</ul>';
} // bh_bootstrap_menu_tree__dropdown_sub_menu()


/**
 * Implements theme_menu_tree().
 *
 * An implementation specific to the main menu.
 *
 * @see bh_bootstrap_menu_link__main_menu()
 */
function bh_bootstrap_menu_tree__main_menu($variables) {
  return '<ul class="nav">' . $variables['tree'] . '</ul>';
} // bh_bootstrap_menu_tree__main_menu()


/**
 * Implements theme_pager().
 *
 * @todo
 *  -- get rid of stupid default div.item-list in pager; this may mean fixing
 *     theme_item_list(), another case where we have to import the ENTIRE
 *     function just to remove a default class/add an additional class. Sigh.
 */
function bh_bootstrap_pager($variables) {
  $tags = $variables['tags'];
  $element = $variables['element'];
  $parameters = $variables['parameters'];
  $quantity = $variables['quantity'];
  global $pager_page_array, $pager_total;

  // Calculate various markers within this pager piece:
  // Middle is used to "center" pages around the current page.
  $pager_middle = ceil($quantity / 2);
  // current is the page we are currently paged to
  $pager_current = $pager_page_array[$element] + 1;
  // first is the first page listed by this pager piece (re quantity)
  $pager_first = $pager_current - $pager_middle + 1;
  // last is the last page listed by this pager piece (re quantity)
  $pager_last = $pager_current + $quantity - $pager_middle;
  // max is the maximum page number
  $pager_max = $pager_total[$element];
  // End of marker calculations.

  // Prepare for generation loop.
  $i = $pager_first;
  if ($pager_last > $pager_max) {
    // Adjust "center" if at end of query.
    $i = $i + ($pager_max - $pager_last);
    $pager_last = $pager_max;
  }
  if ($i <= 0) {
    // Adjust "center" if at start of query.
    $pager_last = $pager_last + (1 - $i);
    $i = 1;
  }
  // End of generation loop preparation.

  $li_first = theme('pager_first', array('text' => (isset($tags[0]) ? $tags[0] : t('« first')), 'element' => $element, 'parameters' => $parameters));
  $li_previous = theme('pager_previous', array('text' => (isset($tags[1]) ? $tags[1] : t('‹ previous')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_next = theme('pager_next', array('text' => (isset($tags[3]) ? $tags[3] : t('next ›')), 'element' => $element, 'interval' => 1, 'parameters' => $parameters));
  $li_last = theme('pager_last', array('text' => (isset($tags[4]) ? $tags[4] : t('last »')), 'element' => $element, 'parameters' => $parameters));

  if ($pager_total[$element] > 1) {
    if ($li_first) {
      $items[] = array(
        'class' => array('pager-first'),
        'data' => $li_first,
      );
    }
    if ($li_previous) {
      $items[] = array(
        'class' => array('pager-previous'),
        'data' => $li_previous,
      );
    }

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      if ($i > 1) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<a href="#">…</a>',
        );
      }
      // Now generate the actual pager piece.
      for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $items[] = array(
            //'class' => array('pager-item'),
            'data' => theme('pager_previous', array('text' => $i, 'element' => $element, 'interval' => ($pager_current - $i), 'parameters' => $parameters)),
          );
        }
        if ($i == $pager_current) {
          $items[] = array(
            'class' => array('pager-current', 'disabled'),
            'data' => '<a href="#">' . $i . '</a>',
          );
        }
        if ($i > $pager_current) {
          $items[] = array(
            //'class' => array('pager-item'),
            'data' => theme('pager_next', array('text' => $i, 'element' => $element, 'interval' => ($i - $pager_current), 'parameters' => $parameters)),
          );
        }
      }
      if ($i < $pager_max) {
        $items[] = array(
          'class' => array('pager-ellipsis', 'disabled'),
          'data' => '<a href="#">…</a>',
        );
      }
    }
    // End generation.
    if ($li_next) {
      $items[] = array(
        'class' => array('pager-next'),
        'data' => $li_next,
      );
    }
    if ($li_last) {
      $items[] = array(
        'class' => array('pager-last'),
        'data' => $li_last,
      );
    }
    return '<h2 class="element-invisible">' . t('Pages') . '</h2>' . '<div class="' . implode(' ', array_map('_bh_bootstrap_css_safe', theme_get_setting('bh_bootstrap_pager__classes'))) . '">' . theme('item_list', array(
      'items' => $items,
      //'attributes' => array('class' => array('pagination')),
    )) . '</div>';
  }
} // bh_bootstrap_pager()


/**
 * Implements template_preprocess_block().
 */
function bh_bootstrap_preprocess_block(&$variables) {
  // What region are we in?
  $current_region = $variables['block']->region;
  // Decide what to do:
  //
  // Try to deal with dropdowns appearing in aggravating places--as far as I'm
  // aware, this only happens with the menu block module, so make the whole
  // operation contingent on that module being present. See issue 1719836
  // (http://drupal.org/node/1719836) for details about why this is needed.
  //
  // I'd rather not invoke preg engine, but given that this circumstance usually
  // arises no more than once per page, and that blocks are cached and that the
  // pages are cached with the blocks in them, it probably doesn't get run that
  // often...
  if (module_exists('menu_block')) {
    // First, find the regions, if any, where dropdowns are allowed (and make sure
    // we're dealing with an array:
    $dropdown_regions = theme_get_setting('bh_bootstrap_dropdown_regions');
    $dropdown_regions = $dropdown_regions !== NULL ? $dropdown_regions : array();
    // Then, if this is NOT one of the dropdown regions AND if this is a menu
    // block:
    if (!in_array($current_region, $dropdown_regions) && $variables['block']->module === 'menu_block') {
      // "Find" patterns:
      $patterns = array(
        // Data toggle links:
        '/<a[^>]*data-toggle="dropdown"[^>]*>[\w\s]+<b class="caret"><\/b><\/a>/',
        // Class attributes containing 'dropdown-toggle' or
        // 'dropdown-menu':
        '/class="([^"]*)(dropdown[\w-]*)([^"]*)"/',
      );
      // "Replace" strings:
      $replacements = array(
        '',
        'class="$1$3"',
      );
      // Unless the theme is explicitly allowing the special menu items provided
      // by the bh_bootstrapmenupath module, we want to remove those items from
      // this menu:
      if (module_exists('bh_bootstrapmenupath') && theme_get_setting('bh_bootstrap_dropdown_regions_menupath_items') !== '1') {
        // Any empty divider or nav-header (we can safely assume there will be no
        // html tags inside either of these:
        $patterns[] = '/<li class="[^"]*(divider|nav-header)[^"]*">[^>]*<\/li>/';
        $replacements[] = '';
      }
      // Cleaned up version of menu:
      $variables['content'] = preg_replace($patterns, $replacements, $variables['content']);
    }
  }
  // General block-region noodling:
  if (strstr($current_region, 'row_') !== FALSE) {
    // Retrieve and make available variables for use in column classes--if there
    // are any sidebars:
    $row_region_details = _bh_bootstrap_get_multiple_regions(array('row_'));
    $row_regions = $row_region_details['row_'];
    // Count the results:
    $row_count = count($row_regions);
    // ROWS
    //
    // We're not going to do anything at all if the row count is zero:
    if ($row_count > 0) {
      $column_size_value = _bh_bootstrap_css_safe(theme_get_setting(sprintf(BH_BOOTSTRAP_THEME_SETTINGS_ROW_VARIABLE_PATTERN, $current_region)));
      $variables['classes_array'][] = 'span' . $column_size_value;
    }
  }
}
// bh_bootstrap_preprocess_block()


/**
 * Implements hook_preprocess_html().
 *
 * @see bh_bootstrap_html_head_alter()
 */
function bh_bootstrap_preprocess_html(&$variables) {
  // Cache path to theme for duration of this function:
  $path_to_bootstrap = drupal_get_path('theme', 'bh_bootstrap');
  // Add 'viewport' meta tag:
  drupal_add_html_head(
    array(
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => 'width=device-width, initial-scale=1',
      ),
    ),
    'bh_bootstrap:viewport_meta'
  );
  // Icon noodling--for favicon, see bh_bootstrap_html_head_alter():
  $path_to_icons = $path_to_bootstrap . '/images/icons/';
  global $theme;
  if ($theme == 'bh_bootstrap') {
    drupal_add_html_head_link(array('rel' => 'apple-touch-icon-precomposed', 'href' => $path_to_icons . 'apple-touch-icon-144-precomposed.png', 'sizes' => '144x144'));
    drupal_add_html_head_link(array('rel' => 'apple-touch-icon-precomposed', 'href' => $path_to_icons . 'apple-touch-icon-114-precomposed.png', 'sizes' => '114x114'));
    drupal_add_html_head_link(array('rel' => 'apple-touch-icon-precomposed', 'href' => $path_to_icons . 'apple-touch-icon-72-precomposed.png', 'sizes' => '72x72'));
    drupal_add_html_head_link(array('rel' => 'apple-touch-icon-precomposed', 'href' => $path_to_icons . 'apple-touch-icon-57-precomposed.png'));
  }
  // Classes related to admin_menu:
  if (user_access('Access administration menu')) {
    if (module_exists('admin_menu_toolbar')) {
      $variables['classes_array'][] = 'bootstrap-admin-menu-toolbar';
    }
    elseif (module_exists('admin_menu')) {
      $variables['classes_array'][] = 'bootstrap-admin-menu';
    }
  }
} // bh_bootstrap_preprocess_html()


/**
 * Implements template_preprocess_page().
 *
 * @todo
 *  -- We should probably make theme settings to show main_menu as nav bar, sub
 *     nav, or about any other kind of bootstrap menu; if we do this, we should
 *     move the logic out of preprocess page into a purpose-built function.
 */
function bh_bootstrap_preprocess_page(&$variables, $hook) {
  // We want to allow the theme to use copies of the bootstrap framework that're
  // installed in sites/all/libraries. Start with a few vars for figuring out
  // the library location:
  $current_directory = getcwd();
  $base_path = base_path();
  $path_to_bh_bootstrap = drupal_get_path('theme', 'bh_bootstrap');
  // The fallback path to the bootstrap library is here, in the theme:
  $bootstrap_theme_path = $current_directory . $base_path . drupal_get_path('theme', 'bh_bootstrap') . '/bootstrap';
  // But if the library module exists, we might find it elsewhere (and we'd
  // prefer to use that one):
  if (module_exists('libraries') && $path = libraries_get_path('bootstrap')) {
    // If the libraries module exists, here's one possiblity:
    $bootstrap_library_path = $current_directory . $base_path . $path;
  }
  // Decide which--if either--to use:
  if (isset($bootstrap_library_path) && is_dir($bootstrap_library_path)) {
    $bootstrap_path = $path;
  }
  elseif (is_dir($bootstrap_theme_path)) {
    $bootstrap_path = $path_to_bh_bootstrap;
  }
  // If it's just not available, display a message to the user:
  else {
    drupal_set_message(t('The Bootstrap framework could not be found. In order to use the BH Bootstrap theme, you must download the framework from !bootstrap-url, and extract it to <em>sites/all/libraries/bootstrap</em> (if the !libraries-module is installed) or to %bootstrap-theme-path.', array('!bootstrap-url' => l(t('the Bootstrap project page'), 'http://twitter.github.com/bootstrap/'), '!libraries-module' => l(t('7.x-1.x version of the Libraries module'), 'http://drupal.org/project/libraries'), '%bootstrap-theme-path' => $path_to_bh_bootstrap . '/bootstrap')), 'error');
  }

  // SIDEBARS
  //
  // Retrieve and make available variables for use in column classes--if there
  // are any sidebars:
  $sidebar_region_details = _bh_bootstrap_get_multiple_regions(array('sidebar_'));
  $sidebar_regions = $sidebar_region_details['sidebar_'];
  // Count the results:
  $sidebar_count = count($sidebar_regions);
  // Start from zero:
  $sidebar_total_width = 0;
  // If there are any sidebars, loop through all the columns:
  if ($sidebar_count > 0) {
    foreach ($sidebar_regions as $key => $name) {
      // If this sidebar actually has content:
      if (count($variables['page'][$key]) > 0) {
        // Find out how big it's supposed to be:
        $column_width_setting = (int) theme_get_setting(sprintf(BH_BOOTSTRAP_THEME_SETTINGS_COLUMN_VARIABLE_PATTERN, $key));
        // Make it available to the page template:
        $variables[sprintf(BH_BOOTSTRAP_PAGE_TEMPLATE_VARIABLE_PATTERN, $key)] = $column_width_setting;
        // Add the width of this sidebar to the total sidebar width:
        $sidebar_total_width += $column_width_setting;
      }
    }
  }
  // Set the content width variable to the total number of columns minus the
  // number of columns occupied by sidebars:
  $variables[sprintf(BH_BOOTSTRAP_PAGE_TEMPLATE_VARIABLE_PATTERN, 'content')] = BH_BOOTSTRAP_GRID_COLUMNS - $sidebar_total_width;

  // CSS
  drupal_add_css($bootstrap_path . '/css/bootstrap.css', array('media' => 'all'));
  drupal_add_css($bootstrap_path . '/css/bootstrap-responsive.css', array('media' => 'screen'));
  // JS
  drupal_add_js($bootstrap_path . '/js/bootstrap.min.js', array('scope' => 'footer'));

  // Main menu
  //
  // In case this theme is configured to use 'advanced' main_menu, AND the theme
  // is set to show a main_menu at all, we build a menu that's at least capable
  // of displaying multiple levels:
  //
  // This may or may not be the way to go--what we probably need to do, if we're
  // going to redefine page.tpl.php's $main_menu variable is pass on a suitably
  // structured renderable array so that it can be themed in the stock way:
  // theme('links__system_main_menu'…)
  if (theme_get_setting('bh_bootstrap_main_menu__navbar_advanced') == 1 && count($variables['main_menu']) > 0) {
    $pid = variable_get('menu_main_links_source', 'main-menu');
    $tree = menu_tree($pid);
    $variables['main_menu_advanced'] = drupal_render($tree);
  }
} // bh_bootstrap_preprocess_page()


/**
 * Implements template_preprocess_region().
 */
function bh_bootstrap_preprocess_region(&$variables) {
  if (strstr($variables['region'], 'row_') !== FALSE) {
    $variables['classes_array'][] = 'row';
  }
} // bh_bootstrap_preprocess_region()


/**
 * Implements theme_status_messages().
 *
 * @todo:
 *    -- clean up this mess of concatenation...hard to believe the core contains
 *       this kind of thing.
 *    -- IF we port the main Bootstrap logic to a module, consider providing a
 *       template for status messages.
 *    -- IMPORTANT: alerts sometimes cannot be dismissed by logged-in users; console
 *       doesn't show any errors, but that this affects only authenticated users
 *       suggests that jquery_ui and bootstrap js may be in conflict.
 */
function bh_bootstrap_status_messages(&$variables) {
  $display = $variables['display'];
  $output = '';

  $message_info = array(
    'status' => array(
      'heading' => 'Status message',
      'class' => 'success',
    ),
    'error' => array(
      'heading' => 'Error message',
      'class' => 'error',
    ),
    'warning' => array(
      'heading' => 'Warning message',
      'class' => '',
    ),
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $message_class = $type != 'warning' ? $message_info[$type]['class'] : 'warning';
    $output .= "<div class=\"alert alert-block alert-$message_class fade in\">\n<a class=\"close\" data-dismiss=\"alert\" href=\"#\">&times;</a>";
    if (!empty($message_info[$type]['heading'])) {
      $output .= '<h2 class="element-invisible">' . $message_info[$type]['heading'] . "</h2>\n";
    }
    if (count($messages) > 1) {
      $output .= " <ul>\n";
      foreach ($messages as $message) {
        $output .= '  <li>' . $message . "</li>\n";
      }
      $output .= " </ul>\n";
    }
    else {
      $output .= $messages[0];
    }
    $output .= "</div>\n";
  }
  return $output;
} // bh_bootstrap_status_messages()


/**
 * Implements theme_table().
 *
 * Assuming that Drupal's table markup is satisfactory, this implementation
 * should ensure that sub themes rarely, if ever, need to override this function--
 * any permutation of Bootstrap classes can be applied to tables using the info
 * file.
 *
 * The one circumstance where it'd be useful to override this function is when
 * it's desireable for some sub-set of tables to be styled differently from the
 * rest.
 *
 * @todo
 *  -- consider adding datatables (may mean replacing drupal behaviors, hm…) when
 *     bh_bootstrap_table() is used.
 */
function bh_bootstrap_table($variables) {
  // We really only want to add bootstrap classes to the table, so retrieve the
  // relevant theme settings (these will always be an array), and add them to
  // the attributes array:
  $variables['attributes']['class'] = array_map('_bh_bootstrap_css_safe', theme_get_setting('bh_bootstrap_table__classes'));
  // Also, we want to make one or two of the admin tables smaller--assuming we
  // can pick it out:
  if (isset($variables['attributes']['id'])) {
    switch ($variables['attributes']['id']) {
      case 'admin-dblog':
        // Condense the dblog table:
        //
        // We'd like to do the same for the content screen, but we don't seem to
        // have anything to hook onto...
        $variables['attributes']['class'][] = 'table-condensed';
        break;
    }
  }
  // THEN, call the theming function directly:
  return theme_table($variables);
} // bh_bootstrap_table()
