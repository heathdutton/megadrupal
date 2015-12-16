<?php
/**
 * @file
 * Theme override and preprocess functions for Doune.
 */

/**
 * Implements hook_theme().
 */
function doune_theme() {
  $items = array();

  // Split out pager list into separate theme function.
  $items['pager_list'] = array('arguments' => array(
    'tags' => array(),
    'limit' => 10,
    'element' => 0,
    'parameters' => array(),
    'quantity' => 9,
  ));

  return $items;
}

/**
 * Implements hook_css_alter(). Borrowed from Tao.
 *
 * Omitted:
 * - color.css
 * - contextual.css
 * - dashboard.css
 * - field_ui.css
 * - image.css
 * - locale.css
 * - shortcut.css
 * - simpletest.css
 * - toolbar.css
 */
function doune_css_alter(&$css) {
  $exclude = array(
    'misc/vertical-tabs.css' => FALSE,
    'modules/aggregator/aggregator.css' => FALSE,
    'modules/block/block.css' => FALSE,
    'modules/book/book.css' => FALSE,
    'modules/comment/comment.css' => FALSE,
    'modules/dblog/dblog.css' => FALSE,
    'modules/file/file.css' => FALSE,
    'modules/filter/filter.css' => FALSE,
    'modules/forum/forum.css' => FALSE,
    'modules/help/help.css' => FALSE,
    'modules/menu/menu.css' => FALSE,
    'modules/node/node.css' => FALSE,
    'modules/openid/openid.css' => FALSE,
    'modules/poll/poll.css' => FALSE,
    'modules/profile/profile.css' => FALSE,
    'modules/search/search.css' => FALSE,
    'modules/statistics/statistics.css' => FALSE,
    'modules/syslog/syslog.css' => FALSE,
    'modules/system/admin.css' => FALSE,
    'modules/system/maintenance.css' => FALSE,
    'modules/system/system.css' => FALSE,
    'modules/system/system.admin.css' => FALSE,
    'modules/system/system.base.css' => FALSE,
    'modules/system/system.maintenance.css' => FALSE,
    'modules/system/system.menus.css' => FALSE,
    'modules/system/system.messages.css' => FALSE,
    'modules/system/system.theme.css' => FALSE,
    'modules/taxonomy/taxonomy.css' => FALSE,
    'modules/tracker/tracker.css' => FALSE,
    'modules/update/update.css' => FALSE,
    'modules/user/user.css' => FALSE,
  );
  $css = array_diff_key($css, $exclude);
}

/**
 * Implements hook_html_head_alter().
 * Overwrite the default meta character type tag with HTML5 version.
 */
function doune_html_head_alter(&$head_elements) {
  // Borrowed from Boron
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * Preprocess variables for html.tpl.php.
 */
function doune_preprocess_html(&$vars) {
  // Borrowed from AdaptiveTheme
  $vars['rdf'] = new stdClass();
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = ' version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }

  // Add a class for the current layout
  $vars['classes_array'][] = drupal_html_class('layout-' . doune_active_layout());
}

/**
 * Preprocesses variables for page template.
 */
function doune_preprocess_page(&$vars) {
  $vars['main_menu_rendered'] = theme('links__system_main_menu', array(
    'links' => $vars['main_menu'],
    'attributes' => array(
      'id' => 'main-menu',
      'class' => array('links', 'inline', 'clearfix'),
    ),
    'heading' => array(
      'text' => t('Main menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  ));

  $vars['secondary_menu_rendered'] = theme('links__system_secondary_menu', array(
    'links' => $vars['secondary_menu'],
    'attributes' => array(
      'id' => 'secondary-menu',
      'class' => array('links', 'inline', 'clearfix'),
    ),
    'heading' => array(
      'text' => t('Secondary menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  ));

  // Allow modules and subthemes to easily modify the page title element's classes
  $vars['title_classes_array'] = array();
  $vars['title_classes_array'][] = 'title';

  $vars['classes_array'][] = 'container';
  $vars['title_tabs_content_wrapper_classes'] = implode(' ', _doune_region_classes('title_tabs_content'));
}

/**
 * Process page variables as late as possible.
 */
function doune_process_page(&$vars) {
  // Flatten title classes.
  $vars['title_classes'] = implode(' ', $vars['title_classes_array']);
}

/**
 * Preprocesses variables for maintenance page template.
 */
function doune_preprocess_maintenance_page(&$vars) {
  // Add basic classes to achieve minimal layout.
  $vars['classes_array'][] = 'layout-default';
  $vars['page_classes_array'][] = 'container';
  $vars['title_tabs_content_wrapper_classes'] = 'sixteen columns';

  // Create empty variables to mirror normal page template.
  $vars['main_menu_rendered'] = '';
  $vars['secondary_menu_rendered'] = '';
}

/**
 * Process maintenance page variables as late as possible.
 */
function doune_process_maintenance_page(&$vars) {
  // Flatten page classes.
  $vars['page_classes'] = implode(' ', $vars['page_classes_array']);
}

/**
 * Determines classes for a region wrapper based on current layout.
 */
function _doune_region_classes($region) {
  $layouts = _doune_layouts();
  $layout = doune_active_layout();
  return isset($layouts[$layout][$region]) ? $layouts[$layout][$region] : array();
}

/**
 * Retrieves the name of the currently active layout.
 */
function doune_active_layout() {
  static $layout;
  // determine the layout
  if (!isset($layout)) {
    $layout = 'default';
    if (module_exists('context_layouts') && ($layout_info = context_layouts_get_active_layout())) {
      $layout = $layout_info['layout'];
    }
  }
  return $layout;
}

/**
 * Returns all Doune layouts registered by modules and themes.
 */
function _doune_layouts() {
  static $layouts;
  if (!isset($layouts)) {
    // We supply default layouts
    $layouts = array(
      'default' => array(
        'title_tabs_content' => array('eight', 'columns', 'push-by-four'),
        'sidebar_first' => array('four', 'columns', 'pull-by-eight'),
        'sidebar_second' => array('four', 'columns'),
      ),
      'thin_and_thick' => array(
        'title_tabs_content' => array('eight', 'columns', 'push-by-three'),
        'sidebar_first' => array('three', 'columns', 'pull-by-eight'),
        'sidebar_second' => array('five', 'columns'),
      ),
    );

    // Give other extensions a chance to add and modify layouts
    drupal_alter('doune_layouts', $layouts);
  }
  return $layouts;
}

/**
 * Preprocesses variables for regions.
 */
function doune_preprocess_region(&$vars) {
  $vars['classes_array'] = array_merge($vars['classes_array'], _doune_region_classes($vars['region']));
}

/**
 * Changes the search form to use the "search" input element of HTML5.
 */
function doune_preprocess_search_block_form(&$vars) {
  // Borrowed from Boron
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
  $vars['search_form'] = str_replace('value=""', 'placeholder="Search"', $vars['search_form']);
}

/**
 * Override of theme_pager(). Adapted from Tao.
 */
function doune_pager($vars) {
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $quantity = $vars['quantity'];
  $vars['unthemed'] = TRUE;
  $pager_links = theme('pager_list', $vars);

  $links = array();
  $links['pager-first'] = theme('pager_first', array(
    'text' => (isset($tags[0]) ? $tags[0] : t('First')),
    'element' => $element,
    'parameters' => $parameters
  ));
  $links['pager-previous'] = theme('pager_previous', array(
    'text' => (isset($tags[1]) ? $tags[1] : t('Prev')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters
  ));
  $links = array_merge($links, $pager_links);
  $links['pager-next'] = theme('pager_next', array(
    'text' => (isset($tags[3]) ? $tags[3] : t('Next')),
    'element' => $element,
    'interval' => 1,
    'parameters' => $parameters
  ));
  $links['pager-last'] = theme('pager_last', array(
    'text' => (isset($tags[4]) ? $tags[4] : t('Last')),
    'element' => $element,
    'parameters' => $parameters
  ));
  $links = array_filter($links);
  $pager_list = theme('links', array(
    'links' => $links,
    'attributes' => array('class' => 'links pager pager-links')
  ));
  if ($pager_list) {
    return "<div class='pager clearfix'>$pager_list</div>";
  }
}

/**
 * Split out page list generation into its own function.
 */
function doune_pager_list($vars) {
  $tags = $vars['tags'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $quantity = $vars['quantity'];

  global $pager_page_array, $pager_total;
  if ($pager_total[$element] > 1) {
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

    $links = array();

    // When there is more than one page, create the pager list.
    if ($i != $pager_max) {
      // Now generate the actual pager piece.
      for ($i; $i <= $pager_last && $i <= $pager_max; $i++) {
        if ($i < $pager_current) {
          $links["pager-item-$i pager-item"] = theme('pager_previous', array(
            'text' => $i,
            'element' => $element,
            'interval' => ($pager_current - $i),
            'parameters' => $parameters
          ));
        }
        if ($i == $pager_current) {
          $links["pager-item-$i pager-current"] = array('title' => $i);
        }
        if ($i > $pager_current) {
          $links["pager-item-$i pager-item"] = theme('pager_next', array(
            'text' => $i,
            'element' => $element,
            'interval' => ($i - $pager_current),
            'parameters' => $parameters
          ));
        }
      }
      return isset($vars['unthemed']) ? $links :
        theme('links', array(
          'links' => $links,
          'attributes' => array('class' => 'links pager pager-list')
        ));
    }
  }
  return isset($vars['unthemed']) ? array() : '';
}

/**
 * Return an array suitable for theme_links() rather than marked up HTML link.
 */
function doune_pager_link($vars) {
  $text = $vars['text'];
  $page_new = $vars['page_new'];
  $element = $vars['element'];
  $parameters = $vars['parameters'];
  $attributes = $vars['attributes'];

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  if ($new_page = implode(',', pager_load_array($page_new[$element], $element, explode(',', $page)))) {
    $parameters['page'] = $new_page;
  }

  $query = array();
  if (count($parameters)) {
    $query = drupal_get_query_parameters($parameters, array());
  }
  if ($query_pager = pager_get_query_parameters()) {
    $query = array_merge($query, $query_pager);
  }

  // Set each pager link title
  if (!isset($attributes['title'])) {
    static $titles = NULL;
    if (!isset($titles)) {
      $titles = array(
        t('« first') => t('Go to first page'),
        t('‹ previous') => t('Go to previous page'),
        t('next ›') => t('Go to next page'),
        t('last »') => t('Go to last page'),
      );
    }
    if (isset($titles[$text])) {
      $attributes['title'] = $titles[$text];
    }
    else if (is_numeric($text)) {
      $attributes['title'] = t('Go to page @number', array('@number' => $text));
    }
  }

  return array(
    'title' => $text,
    'href' => $_GET['q'],
    'attributes' => $attributes,
    'query' => count($query) ? $query : NULL,
  );
}
