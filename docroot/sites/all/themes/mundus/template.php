<?php

/**
 * @file
 * Provides preprocess logic and other utilities for
 * Mundus and Foundation integration.
 */

/**
 * Implements themename_preprocess_page().
 */
function mundus_preprocess_page(&$variables) {

  // RENDER SEARCH FROM INSIDE PAGE TEMPLATE:
  $mundus_search_init = drupal_get_form('search_form');
  $mundus_search = drupal_render($mundus_search_init);
  $variables['mundus_search'] = $mundus_search;
}

/**
 * Preprocess variables for html.tpl.php.
 */
function mundus_preprocess_html(&$vars) {

  // Using libraries to get foundation framework:
  if (module_exists('libraries') && $path = libraries_get_path('foundation') & $path_icons = libraries_get_path('foundation_icons_all')) {
    // If the libraries module exists:
    $foundation_path = $path;
    $foundation_icon_path = $path_icons;

    // Adding Foundation css and Foundation icons:
    drupal_add_css($foundation_path . '/css/foundation.css', array('media' => 'all'));
    drupal_add_css($foundation_icon_path . '/foundation_icons_social/stylesheets/social_foundicons.css', array('media' => 'all'));
    drupal_add_css($foundation_icon_path . '/foundation_icons_general_enclosed/stylesheets/general_enclosed_foundicons.css', array('media' => 'all'));
    drupal_add_css($foundation_path . '/css/normalize.css', array('media' => 'all'));

    // Adding Foundation JS library at bottom of page:
    drupal_add_js($foundation_path . '/js/vendor/custom.modernizr.js', array('scope' => 'header'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.topbar.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.cookie.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.forms.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.orbit.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.placeholder.js', array('scope' => 'footer'));
    drupal_add_js($foundation_path . '/js/foundation/foundation.reveal.js', array('scope' => 'footer'));

    // Invoke Foundation:
    drupal_add_js('jQuery(document).foundation();', array(
      'type' => 'inline',
      'scope' => 'footer',
    ));
  }

  // If it's just not available, display a message to the user:
  else {
    drupal_set_message(t('<h1>The Foundation framework and icons could not be found.
      <p></p>In order to use the Mundus theme, you must download the framework:
      <a href="http://foundation.zurb.com/download.php" target="_blank">Click here</a>
      and extract it to <em>sites/all/libraries/foundation</em><p></p>
      Additional you need to download Foundation icons:
      <a href="http://zurb.com/playground/uploads/upload/upload/146/foundation_icons_all.zip" target="_blank">Click here</a>
      and extract it to <em>sites/all/libraries/foundation_icons_all</em>
      </h1>'), 'error');
  }

  // Sanitize user provided text before printing.
  if (theme_get_setting('google_font_body')) {
    $font_body = check_plain(theme_get_setting('google_font_body'));
  }
  else {
    $font_body = 'Ubuntu';
  }

  if (theme_get_setting('google_font_name')) {
    $font_name = check_plain(theme_get_setting('google_font_name'));
  }
  else {
    $font_name = 'Merriweather+Sans';
  }

  // Implements  adding Google fonts:
  drupal_add_css('http://fonts.googleapis.com/css?family=' . $font_name . '|' . $font_body . '', array(
    'type' => 'external',
  ));

  drupal_add_css('h1,h2,h3,h4,h5,h6 {font-family: ' . $font_name . '; }body {font-family: ' . $font_body . ';}', array(
    'group' => CSS_THEME,
    'type' => 'inline',
  ));

  // ADD FIXES FOR IE8 foundation & mundus grid:
  drupal_add_css(drupal_get_path('theme', 'mundus') . '/css/foundation_mundus_ie8.css', array(
    'group' => CSS_THEME,
    'browsers' => array(
      'IE' => 'lt IE 9',
      '!IE' => FALSE,
    ),
    'preprocess' => TRUE,
  ));

  // ADD FIXES FOR IE7 mundus style:
  drupal_add_css(drupal_get_path('theme', 'mundus') . '/css/foundation_mundus_ie7.css', array(
    'group' => CSS_THEME,
    'browsers' => array(
      'IE' => 'lt IE 8',
      '!IE' => FALSE,
    ),
    'preprocess' => TRUE,
  ));

  // Load color style from theme settings:
  drupal_add_css(drupal_get_path('theme', 'mundus') . '/css/style/' . theme_get_setting('mundus_style') . '.css', array(
    'group' => CSS_THEME,
    'type' => 'file',
  ));
}

/**
 * Add mobile viewport, force IE to chrome frame.
 */
function mundus_html_head_alter(&$head_elements) {
  // HTML5 charset declaration.
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );

  // Optimize mobile viewport.
  $head_elements['mobile_viewport'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'name' => 'viewport',
      'content' => 'width=device-width',
    ),
  );

  // Force IE to use Chrome Frame if installed.
  $head_elements['chrome_frame'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'content' => 'ie=edge, chrome=1',
      'http-equiv' => 'x-ua-compatible',
    ),
  );

  // Remove image toolbar in IE.
  $head_elements['ie_image_toolbar'] = array(
    '#type' => 'html_tag',
    '#tag' => 'meta',
    '#attributes' => array(
      'http-equiv' => 'ImageToolbar',
      'content' => 'false',
    ),
  );
}

/**
 * Remove default css files from Drupal.
 */
function mundus_css_alter(&$css) {

  $remove = array(
    'misc/vertical-tabs.css',
    'misc/vertical-tabs-rtl.css',
    'misc/ui/jquery.ui.core.css',
    'misc/ui/jquery.ui.theme.css',
    'modules/aggregator/aggregator.css',
    'modules/poll/poll.css',
    'modules/comment/comment.css',
    'modules/comment/comment-rtl.css',
    'modules/field/theme/field.css',
    'modules/field/theme/field-rtl.css',
    'modules/file/file.css',
    'modules/filter/filter.css',
    'modules/node/node.css',
    'modules/search/search.css',
    'modules/search/search-rtl.css',
    'modules/system/system.admin.css',
    'modules/system/system.admin-rtl.css',
    'modules/system/system.maintenance.css',
    'modules/system/system.menus.css',
    'modules/system/system.menus-rtl.css',
    'modules/system/system.messages.css',
    'modules/system/system.messages-rtl.css',
    'modules/system/system.theme.css',
    'modules/system/system.theme-rtl.css',
    'modules/user/user.css',
    'modules/user/user-rtl.css',
    'sites/all/modules/ctools/css/ctools.css',
  );
  foreach ($remove as $value) {
    unset($css[$value]);
  }
}

/**
 * Style main menu / show submenu as dropdown.
 */
function mundus_links__system_main_menu($vars) {
  // We need to fetch the links ourselves because we need the entire tree.
  $links = menu_tree_output(menu_tree_all_data(variable_get('menu_main_links_source', 'main-menu')));
  $output = _mundus_links($links);

  return '<ul class="left">' . $output . '</ul>';
}
 /**
  * Helper function to output a Drupal menu as a Foundation Top Bar.
  *
  * @param array $links
  *   An array of menu links.
  *
  * @return string
  *   A rendered list of links, with no <ul> or <ol> wrapper.
  *
  * @see mundus_links__system_main_menu()
  * @see mundus_links__system_secondary_menu()
  */
function _mundus_links($links) {
  $output = '';

  foreach (element_children($links) as $key) {
    $output .= _mundus_render_link($links[$key]);
  }

  return $output;
}

/**
 * Helper function to recursively render sub-menus.
 *
 * @param array $link
 *   An array of menu links.
 *
 * @return string
 *   A rendered list of links, with no <ul> or <ol> wrapper.
 *
 * @see _mundus_links()
 */
function _mundus_render_link($link) {
  $output = '';

  // This is a duplicate link that won't get the dropdown class and will only
  // show up in small-screen.
  $small_link = $link;

  if (!empty($link['#below'])) {
    $link['#attributes']['class'][] = 'has-dropdown';
  }

  // Render top level and make sure we have an actual link.
  if (!empty($link['#href'])) {
    $output .= '<li' . drupal_attributes($link['#attributes']) . '>' . l($link['#title'], $link['#href']);

    // Add repeated link under the dropdown for small-screen.
    $small_link['#attributes']['class'][] = 'show-for-small';
    $sub_menu = '<li' . drupal_attributes($small_link['#attributes']) . '>' . l($link['#title'], $link['#href']);

    // Build sub nav recursively.
    foreach ($link['#below'] as $sub_link) {
      if (!empty($sub_link['#href'])) {
        $sub_menu .= _mundus_render_link($sub_link);
      }
    }

    $output .= !empty($link['#below']) ? '<ul class="dropdown">' . $sub_menu . '</ul>' : '';
    $output .= '</li>';
  }
  return $output;
}

/**
 * Remove filter tips from comment form.
 */
function mundus_form_comment_form_alter(&$form, &$form_state) {

  $form['comment_body']['#after_build'][] = 'mundus_customize_comment_form';
  $form['author']['homepage']['#access'] = FALSE;
}

/**
 * Remove filter tips from comment form.
 */
function mundus_customize_comment_form(&$form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}

/**
 * Adding separate template for teaser / adding block region to node template.
 */
function mundus_preprocess_node(&$variables) {

  // If the node is a teaser:
  if ($variables['teaser']) {
    // Allow us to use a different template:
    $variables['theme_hook_suggestions'][] = 'node__' . $variables['node']->type . '_teaser';
  }
  // Adding block region to node.tpl.php:
  if ($blocks = block_get_blocks_by_region('node_block')) {
    $variables['node_block'] = $blocks;
  }
}

/**
 * Override for node teaser - display single image.
 */
function mundus_process_field(&$vars) {
  $element = $vars['element'];
  // Field type image.
  if ($element['#field_type'] == 'image') {
    // Reduce number of images in teaser view mode to single image:
    if ($element['#view_mode'] == 'teaser') {
      $item = reset($vars['items']);
      $vars['items'] = array($item);
    }
  }
}

/**
 * Label theming.
 */
function mundus_field($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div ' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  foreach ($variables['items'] as $item) {
    $output .= drupal_render($item);
  }

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Implements hook_form_alter().
 *
 * Change style output of buttons.
 *
 * @see hook_form_alter()
 *
 * @ingroup themeable
 */
function mundus_form_alter(&$form, &$form_state) {

  if (!empty($form['actions'])) {
    $classes = array(
      'secondary',
      'small button',
      'radius',
    );

    // Button style submit:
    if (!empty($form['actions']['submit'])) {
      if (isset($form['actions']['submit']['#attributes']['class'])) {
        $form['actions']['submit']['#attributes']['class'] = array_merge($form['actions']['submit']['#attributes']['class'], $classes);
      }
      else {
        $form['actions']['submit']['#attributes']['class'] = $classes;
      }
    }
    // Button style preview:
    if (!empty($form['actions']['preview'])) {
      if (isset($form['actions']['preview']['#attributes']['class'])) {
        $form['actions']['preview']['#attributes']['class'] = array_merge($form['actions']['preview']['#attributes']['class'], $classes);
      }
      else {
        $form['actions']['preview']['#attributes']['class'] = $classes;
      }
    }
    // Button style delete:
    if (!empty($form['actions']['delete'])) {
      if (isset($form['actions']['delete']['#attributes']['class'])) {
        $form['actions']['delete']['#attributes']['class'] = array_merge($form['actions']['delete']['#attributes']['class'], $classes);
      }
      else {
        $form['actions']['delete']['#attributes']['class'] = $classes;
      }
    }
  }
}

/**
 * Breadcrumb.
 */
function mundus_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $breadcrumbs = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $breadcrumbs .= '<ul class="breadcrumbs">';

    foreach ($breadcrumb as $value) {
      $breadcrumbs .= '<li>' . $value . '</li>';
    }

    $title = strip_tags(drupal_get_title());
    $breadcrumbs .= '<li class="current"><a href="#">' . $title . '</a></li>';
    $breadcrumbs .= '</ul>';

    return $breadcrumbs;
  }
}

/**
 * FUNCTION FOR USING SLIDESHOW ON VIEWS.
 */
function mundus_preprocess_views_view_list(&$vars) {
  $handler = $vars['view']->style_plugin;

  $class = explode(' ', $handler->options['class']);
  $class = array_map('views_clean_css_identifier', $class);

  $vars['class'] = implode(' ', $class);

  if ($vars['class'] == 'data-orbit') {
    $vars['list_type_prefix'] = '<' . $handler->options['type'] . ' ' . $vars['class'] . '>';
  }
}
