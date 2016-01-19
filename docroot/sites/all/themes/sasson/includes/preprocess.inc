<?php

/**
 * @file
 * Prepare variables for template files.
 */

/**
 * Implements template_preprocess_html().
 */
function sasson_preprocess_html(&$vars) {

  $vars['doctype'] = _sasson_doctype();
  $vars['rdf'] = _sasson_rdf($vars);
  $vars['html_attributes'] = 'lang="' . $vars['language']->language . '" dir="' . $vars['language']->dir . '" ' . $vars['rdf']->version . $vars['rdf']->namespaces;

  // IE coditional comments on the <html> tag
  // http://paulirish.com/2008/conditional-stylesheets-vs-css-hacks-answer-neither/
  if (theme_get_setting('sasson_ie_comments')) {
    $vars['html'] = '<!--[if lte IE 7 ]><html ' . $vars['html_attributes'] . ' class="no-js ie7 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 8 ]><html ' . $vars['html_attributes'] . ' class="no-js ie8 lte-ie9 lte-ie8"><![endif]-->
                     <!--[if IE 9 ]><html ' . $vars['html_attributes'] . ' class="no-js ie9 lte-ie9"><![endif]-->
                     <!--[if gt IE 9]><!--> <html ' . $vars['html_attributes'] . ' class="no-js"> <!--<![endif]-->';
  } else {
    $vars['html'] = '<html ' . $vars['html_attributes'] . ' class="no-js">';
  }

  // CSS resets
  // normalize remains the default
  $reset = theme_get_setting('sasson_cssreset') ? theme_get_setting('sasson_cssreset') : 'normalize';
  if (theme_get_setting('sasson_cssreset') != 'none') {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/reset/' . $reset . '.css', array('weight' => -1, 'every_page' => TRUE));
  }
  if (theme_get_setting('sasson_formalize')) {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/reset/formalize/css/formalize.css' , array('weight' => -1, 'every_page' => TRUE));
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/stylesheets/reset/formalize/js/jquery.formalize.js' , array('scope' => 'footer'));
  }

  // Layout stylesheets
  if (!theme_get_setting('sasson_responsive')) {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/layout/fixed.css', array('theme' => 'sasson'));
  }
  elseif (theme_get_setting('sasson_responsive_approach') == 'mobile_first') {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/layout/mobile-first.css', array('theme' => 'sasson'));
  }
  else {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/layout/desktop-first.css', array('theme' => 'sasson'));
  }

  // File-Watcher - auto-update the browser when a file is updated
  if (theme_get_setting('sasson_watcher') && user_access('administer themes')) {
    global $base_url;
    $list = array_map('trim',explode("\n", theme_get_setting('sasson_watch_file')));
    $watcher = "(function () {\n";
    foreach ($list as $file){
      if (substr($file, 0, 1) !== ';') {
        $watcher .= "  Drupal.sasson.watch('" . $base_url . "/" . $file . "');\n";
      }
    }
    $watcher .= "}());";
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/fileWatcher.js' , array('scope' => 'footer'));
    drupal_add_js($watcher, array('type' => 'inline', 'scope' => 'footer'));
  }

  // Custom fonts from Google web-fonts
  $font = str_replace(' ', '+', theme_get_setting('sasson_font'));
  if (theme_get_setting('sasson_font')) {
    drupal_add_css('//fonts.googleapis.com/css?family=' . $font , array('type' => 'external', 'group' => CSS_THEME));
    $font_face = theme_get_setting('sasson_font') ? strtok(theme_get_setting('sasson_font'), ':') : 'false';
    $font_fallback = theme_get_setting('sasson_font_fallback') ? ', ' . theme_get_setting('sasson_font_fallback') : '';
    $font_selectors = theme_get_setting('sasson_font_selectors') ? theme_get_setting('sasson_font_selectors') : '.sasson-font-face';
    $css = "{$font_selectors} { font-family: {$font_face} {$font_fallback} }";
    drupal_add_css($css, array(
      'group' => CSS_THEME,
      'type' => 'inline',
      'media' => 'screen',
      'preprocess' => FALSE,
    ));
  }

  // Enable HTML5 elements in IE
  $vars['html5shiv'] = theme_get_setting('sasson_html5shiv') ? '<!--[if lt IE 9]><script src="'. base_path() . drupal_get_path('theme', 'sasson') .'/scripts/html5shiv.js"></script><![endif]-->' : '';

  // Force latest IE rendering engine
  if (theme_get_setting('sasson_force_ie')) {
    $meta_force_ie = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'X-UA-Compatible',
        'content' =>  'IE=edge,chrome=1',
      )
    );
    drupal_add_html_head($meta_force_ie, 'meta_force_ie');
  }

  // Change the default meta content-type tag to the shorter HTML5 version
  $charset = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'charset' => 'utf-8',
    )
  );
  drupal_add_html_head($charset, 'system_meta_content_type');

  // Prompt IE users to install Chrome Frame
  if (theme_get_setting('sasson_prompt_cf') != 'Disabled') {
    $vars['prompt_cf'] = "<!--[if lte " . theme_get_setting('sasson_prompt_cf') . " ]>
      <div style='background: #eee; text-align: center; padding: 3px;'>" . t("Your browser is <b>out of date</b> and may <b>not display all features</b> of this and other websites. !url", array('!url' => l(t('Learn how to update your browser'), "http://outdatedbrowser.com/", array('attributes' => array('target'=>'_blank'))))) . "</div><![endif]-->";
  } else {
    $vars['prompt_cf'] = '';
  }

  //  Mobile viewport optimized: h5bp.com/viewport
  if (theme_get_setting('sasson_responsive')) {
    $mobile_viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' =>  'width=device-width,initial-scale=1',
      )
    );
    drupal_add_html_head($mobile_viewport, 'mobile_viewport');
  }

  // Load responsive menus if enabled in theme-settings
  if (theme_get_setting('sasson_responsive')) {
    $mobiledropdown_width = str_replace('px', '', theme_get_setting('sasson_responsive_menus_width'));
    if ($mobiledropdown_width > 0) {
      $inline_code = 'jQuery("' . theme_get_setting('sasson_responsive_menus_selectors') . '").mobileSelect({
          deviceWidth: ' . $mobiledropdown_width . ',
          autoHide: ' . theme_get_setting('sasson_responsive_menus_autohide') . ',
        });';
      drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/jquery.mobileselect.js');
      drupal_add_js($inline_code,
        array('type' => 'inline', 'scope' => 'footer')
      );
    }
  }

  // Since menu is rendered in preprocess_page we need to detect it here to add body classes
  $has_main_menu = theme_get_setting('toggle_main_menu');
  $has_secondary_menu = theme_get_setting('toggle_secondary_menu');

  /* Add extra classes to body for more flexible theming */

  if ($has_main_menu or $has_secondary_menu) {
    $vars['classes_array'][] = 'with-navigation';
  }

  if ($has_secondary_menu) {
    $vars['classes_array'][] = 'with-subnav';
  }

  if (!empty($vars['page']['featured'])) {
    $vars['classes_array'][] = 'featured';
  }

  if ($vars['is_admin']) {
    $vars['classes_array'][] = 'admin';
  }

  if (theme_get_setting('sasson_show_grid')) {
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/extra/switch.css', array('theme' => 'sasson'));
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/extra/grid-background.css', array('theme' => 'sasson'));
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/showGrid.js' , array('scope' => 'footer'));
    $vars['classes_array'][] = 'grid-background';
    $vars['classes_array'][] = 'grid-hidden';
  }

  if (theme_get_setting('sasson_overlay')) {
    $vars['classes_array'][] = 'with-overlay';
    $overlay_file = theme_get_setting('sasson_overlay_file');
    if ($overlay_file && file_load($overlay_file)) {
      $overlay_file = file_create_url(file_load($overlay_file)->uri);
    }
    else {
      $overlay_file = drupal_get_path('theme', 'sasson') . '/images/overlay.png';
    }
    drupal_add_css(drupal_get_path('theme', 'sasson') . '/stylesheets/extra/switch.css', array('theme' => 'sasson'));
    drupal_add_library('system', 'ui');
    drupal_add_library('system', 'ui.widget');
    drupal_add_library('system', 'ui.mouse');
    drupal_add_library('system', 'ui.draggable');
    drupal_add_js(drupal_get_path('theme', 'sasson') . '/scripts/imageOverlay.js' , array('scope' => 'footer'));
    drupal_add_js(array('sasson' => array(
      'overlay_url' => $overlay_file,
      'overlay_opacity' => theme_get_setting('sasson_overlay_opacity'),
    )), 'setting');
  }

  $vars['classes_array'][] = 'dir-' . $vars['language']->dir;

  if (!$vars['is_front']) {
    // Add unique classes for each page and website section
    $path = drupal_get_path_alias($_GET['q']);
    $temp = explode('/', $path, 2);
    $section = array_shift($temp);
    $page_name = array_shift($temp);

    if (isset($page_name)) {
      $vars['classes_array'][] = drupal_html_id('page-' . $page_name);
    }

    $vars['classes_array'][] = drupal_html_id('section-' . $section);

    if (arg(0) == 'node') {
      if (arg(1) == 'add') {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-add'; // Add 'section-node-add'
      } elseif (is_numeric(arg(1)) && (arg(2) == 'edit' || arg(2) == 'delete')) {
        if ($section == 'node') {
          array_pop($vars['classes_array']); // Remove 'section-node'
        }
        $vars['classes_array'][] = 'section-node-' . arg(2); // Add 'section-node-edit' or 'section-node-delete'
      }
    }
  }

  // Includes all custom style sheets for the current theme.
  sasson_css_include();

}


/**
 * Implements template_preprocess_page().
 */
function sasson_preprocess_page(&$vars) {

  if (isset($vars['node_title'])) {
    $vars['title'] = $vars['node_title'];
  }

  // Site navigation links.
  $vars['main_menu_links'] = '';
  if (!empty($vars['main_menu'])) {
    $main_menu_name = variable_get('menu_main_links_source', 'main-menu');
    $menu_tree = menu_build_tree($main_menu_name);
    $menu_tree_output = menu_tree_output($menu_tree);
    $menu_rendered = drupal_render($menu_tree_output);
    $vars['main_menu_links'] = $menu_rendered;
  }
  $vars['secondary_menu_links'] = '';
  if (!empty($vars['secondary_menu'])) {
    $secondary_menu_name = variable_get('menu_secondary_links_source', 'user-menu');
    $menu_tree = menu_build_tree($secondary_menu_name);
    $menu_tree_output = menu_tree_output($menu_tree);
    $menu_tree_output['#theme_wrappers'][] = 'menu_tree__secondary_menu';
    $menu_rendered = drupal_render($menu_tree_output);
    $vars['secondary_menu_links'] = $menu_rendered;
  }

  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($vars['title_suffix']['add_or_remove_shortcut']) && $vars['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $vars['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $vars['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $vars['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }

  if(!theme_get_setting('sasson_feed_icons')) {
    $vars['feed_icons'] = '';
  }
}


/**
 * Implements template_preprocess_node().
 *
 * Adds extra classes to node container for advanced theming
 */
function sasson_preprocess_node(&$vars) {
  // Striping class
  $vars['classes_array'][] = 'node-' . $vars['zebra'];

  // Node is published
  $vars['classes_array'][] = ($vars['status']) ? 'published' : 'unpublished';

  // Node has comments?
  $vars['classes_array'][] = ($vars['comment']) ? 'with-comments' : 'no-comments';

  if ($vars['sticky']) {
    $vars['classes_array'][] = 'sticky'; // Node is sticky
  }

  if ($vars['promote']) {
    $vars['classes_array'][] = 'promote'; // Node is promoted to front page
  }

  if ($vars['teaser']) {
    $vars['classes_array'][] = 'node-teaser'; // Node is displayed as teaser.
  }

  if ($vars['uid'] && $vars['uid'] === $GLOBALS['user']->uid) {
    $classes[] = 'node-mine'; // Node is authored by current user.
  }

  $vars['submitted'] = t('Submitted by !username on ', array('!username' => $vars['name']));
  $vars['submitted_date'] = t('!datetime', array('!datetime' => $vars['date']));
  $vars['submitted_pubdate'] = format_date($vars['created'], 'custom', 'Y-m-d\TH:i:s');

  if ($vars['view_mode'] == 'full' && node_is_page($vars['node'])) {
    $vars['classes_array'][] = 'node-full';
  }
}


/**
 * Implements template_preprocess_block().
 */
function sasson_preprocess_block(&$vars, $hook) {
  // Add a striping class.
  $vars['classes_array'][] = 'block-' . $vars['zebra'];

  $vars['title_attributes_array']['class'][] = 'block-title';

  // Use nav element for menu blocks and provide a suggestion for all of them
  $nav_blocks = array('navigation', 'main-menu', 'management', 'user-menu');
  $nav_modules = array('superfish', 'nice_menus', 'menu_block');
  if (in_array($vars['block']->delta, $nav_blocks) || in_array($vars['block']->module, $nav_modules)) {
    $vars['tag'] = 'nav';
    array_unshift($vars['theme_hook_suggestions'], 'block__menu');
  }
  elseif (!empty($vars['block']->subject)) {
    $vars['tag'] = 'section';
  }
  else {
    $vars['tag'] = 'div';
  }

  // In the header region visually hide block titles.
  if ($vars['block']->region == 'header') {
    $vars['title_attributes_array']['class'][] = 'element-invisible';
  }
}


/**
 * Implements template_proprocess_search_block_form().
 *
 * Changes the search form to use the HTML5 "search" input attribute
 */
function sasson_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}


/**
 * Implements template_proprocess_preprocess_menu_local_task().
 *
 * Override or insert variables into theme_menu_local_task().
 */
function sasson_preprocess_menu_local_task(&$vars) {
  $link = & $vars['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }

  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}


/**
 * Generate doctype for templates
 */
function _sasson_doctype() {
  return (module_exists('rdf')) ? '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN"' . "\n" . '"http://www.w3.org/MarkUp/DTD/xhtml-rdfa-1.dtd">' : '<!DOCTYPE html>' . "\n";
}


/**
 * Generate RDF object for templates
 *
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 *
 * @param array $vars
 */
function _sasson_rdf($vars) {
  $rdf = new stdClass();

  if (module_exists('rdf')) {
    $rdf->version = 'version="HTML+RDFa 1.1"';
    $rdf->namespaces = $vars['rdf_namespaces'];
    $rdf->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $rdf->version = '';
    $rdf->namespaces = '';
    $rdf->profile = '';
  }

  return $rdf;
}
