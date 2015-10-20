<?php

/**
 * @file
 * Preprocess and process variables.
 *
 * Groundwork Front-end Framework for Drupal
 * Learn more ~ http://groundwork.noceda.me
 *
 * WARNING: DO NOT MODIFY THIS FILE. Create a subtheme for your customizations.
 *
 */

/**
 * Preprocess variables for html.tpl.php
 */
function groundwork_preprocess_html(&$variables, $hook) {
  // Attributes for html element.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );

  // Send X-UA-Compatible HTTP header to force IE to use the most recent
  // rendering engine or use Chrome's frame rendering engine if available.
  // This also prevents the IE compatibility mode button to appear when using
  // conditional classes on the html tag.
  if (is_null(drupal_get_http_header('X-UA-Compatible'))) {
    drupal_add_http_header('X-UA-Compatible', 'IE=edge,chrome=1');
  }

  global $theme_key;
  // Create groundwork_path for drush
  $groundwork_path = drupal_get_path('theme', 'groundwork');
  // Use the LESS Preprocessor or regular ol' CSS.
  $path = drupal_get_path('theme', $theme_key);
  if (theme_get_setting('noceda_basic_functionality')) {
    drupal_add_css($path . '/css/groundwork.css', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }
  else {
    drupal_add_css($path . '/less/main.css.less', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }
  // Discover an app-style.css file
  if (file_exists($path . '/css/app-style.css')) {
    drupal_add_css($path . '/css/app-style.css', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }

  // Return early, so the maintenance page does not call any of the code below.
  if ($hook != 'html') {
    return;
  }

  // Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($variables['rdf_namespaces']) {
    $prefixes = array();
    foreach (explode("\n  ", ltrim($variables['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $prefixes[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }
    $variables['rdf_namespaces'] = ' prefix="' . implode(' ', $prefixes) . '"';
  }

  // Classes for body element. Allows advanced theming based on context
  // (home page, node of certain type, etc.)
  if (!$variables['is_front']) {
    // Add unique class for each page.
    $path = drupal_get_path_alias($_GET['q']);
    // Add unique class for each website section.
    list($section, ) = explode('/', $path, 2);
    $arg = explode('/', $_GET['q']);
    if ($arg[0] == 'node' && isset($arg[1])) {
      if ($arg[1] == 'add') {
        $section = 'node-add';
      }
      elseif (isset($arg[2]) && is_numeric($arg[1]) && ($arg[2] == 'edit' || $arg[2] == 'delete')) {
        $section = 'node-' . $arg[2];
      }
    }
    $variables['classes_array'][] = drupal_html_class('section-' . $section);
  }
  if (theme_get_setting('noceda_hide_sitename_branding')) {
    $variables['classes_array'][] = 'hide-sitename-branding'; // Optionally hide the site name visually.
  }
  // Store the menu item since it has some useful information.
  $variables['menu_item'] = menu_get_item();
  if ($variables['menu_item']) {
    switch ($variables['menu_item']['page_callback']) {
      case 'views_page':
        // Is this a Views page?
        $variables['classes_array'][] = 'page-views';
        break;
      case 'page_manager_blog':
      case 'page_manager_blog_user':
      case 'page_manager_contact_site':
      case 'page_manager_contact_user':
      case 'page_manager_node_add':
      case 'page_manager_node_edit':
      case 'page_manager_node_view_page':
      case 'page_manager_page_execute':
      case 'page_manager_poll':
      case 'page_manager_search_page':
      case 'page_manager_term_view_page':
      case 'page_manager_user_edit_page':
      case 'page_manager_user_view_page':
        // Is this a Panels page?
        $variables['classes_array'][] = 'page-panels';
        break;
    }
  }
}

/**
 * Process variables for html.tpl.php
 */
function groundwork_process_html(&$variables, $hook) {
  // Hook into color.module. Planned Groundwork support in the future?
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
  // Flatten out html_attributes.
  $variables['html_attributes'] = drupal_attributes($variables['html_attributes_array']);
}

/**
 * Override or insert variables in the html_tag theme function.
 */
function groundwork_process_html_tag(&$variables) {
  $tag = &$variables['element'];

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
 * Preprocess variables for page.tpl.php
 */
function groundwork_preprocess_page(&$variables) {
  // Add the logged-in user role as class.
  // Set correct title to user pages.
  if (arg(0) == 'user' && arg(1) == 'login') {
    drupal_set_title(t('User login'));
  }

  if (arg(0) == 'user' && arg(1) == 'password') {
    drupal_set_title(t('Request new password'));
  }

  if (arg(0) == 'user' && arg(1) == 'register') {
    drupal_set_title(t('Create new account'));
  }

  // Build a variable for the main menu
  if (isset($variables['main_menu'])) {
    $variables['main_navigation'] = theme('links', array(
      'links' => $variables['main_menu'],
      'attributes' => array(
        'id' => 'main-menu-list',
        'class' => array('menu', 'main-menu-links', 'inline'),
      ),
      'heading' => array(
        'text' => t('Main navigation'),
        'level' => 'h2',
        'class' => array('visually-hidden'),
      )
    ));
  }

  // Build a variable for the secondary menu
  if (isset($variables['secondary_menu'])) {
    $variables['secondary_navigation'] = theme('links', array(
      'links' => $variables['secondary_menu'],
      'attributes' => array(
        'id' => 'secondary-menu-list',
        'class' => array('menu', 'secondary-menu-links', 'inline'),
      ),
      'heading' => array(
        'text' => t('Secondary navigation'),
        'level' => 'h2',
        'class' => array('visually-hidden'),
      )
    ));
  }

  // Build a variable for the attribution
  if (theme_get_setting('noceda_hide_attribution')) {
    $variables['theme_attribution'] = '<div class="visually-hidden">Theme powered by <a href="http://groundwork.noceda.me">Groundwork Front-end Framework for Drupal</a></div>';
  }
  else {
    $variables['theme_attribution'] = '<div id="theme-attribution-wrapper" class="wrapper"><div id="theme-attribution"><p>' . t('Theme powered by') . ' <a href="http://groundwork.noceda.me">Groundwork Front-end Framework for Drupal</a></p></div></div>';
  }
}

/**
 * Process or insert variables into the page template.
 */
function groundwork_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
}

/**
 * Preprocess variables for maintenance-page.tpl.php
 */
function groundwork_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  global $theme_key;
  $path = drupal_get_path('theme', $theme_key);
  if (theme_get_setting('noceda_basic_functionality')) {
    drupal_add_css($path . '/css/groundwork.css', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }
  else {
    drupal_add_css($path . '/less/main.css.less', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }
  // Discover a local.css file
  if (file_exists($path . '/css/app-style.css')) {
    drupal_add_css($path . '/css/app-style.css', array('group' => CSS_THEME, 'preprocess' => TRUE));
  }
  drupal_add_css($path . '/css/maintenance-page.css', array('group' => CSS_THEME, 'preprocess' => TRUE));
  $variables['theme_attribution'] = '<div class="visually-hidden">Theme powered by <a href="http://groundwork.noceda.me">Groundwork Front-end Framework for Drupal</a></div>';
}

function groundwork_preprocess_block(&$variables, $hook) {
  // Add class with the region embedding the current block
  $variables['classes_array'][] = drupal_html_class('block-region-' . $variables['block']->region);
  $variables['classes_array'][] = 'block-' . $variables['block_zebra'];
  if ($variables['block_id'] == 1) {
    $variables['classes_array'][] = 'block-first';
  }
  // The last_in_region property is set in noceda_page_alter().
  if (isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'block-last';
  }
  if ($variables['block_id'] == 1 && isset($variables['block']->last_in_region)) {
    $variables['classes_array'][] = 'block-solo';
  }
  // Optionally display block id for easy styling.
  if (theme_get_setting('noceda_dev_block_id')) {
    $variables['semantic_helper'] = '<small class="block-html-id">#' . $variables['block_html_id'] . '</small>';
  }
  else {
    $variables['semantic_helper'] = '';
  }
  $variables['title_attributes_array']['class'][] = 'block-title';
  if ($variables['block']->region == 'navigation' || $variables['block']->region == 'topbar') {
    $variables['title_attributes_array']['class'][] = 'visually-hidden';
  }
  $variables['content_attributes_array']['class'][] = 'block-content';

  // Add Aria Roles via attributes.
  switch ($variables['block']->module) {
    case 'system':
      switch ($variables['block']->delta) {
        case 'main':
          // Note: the "main" role goes in the page.tpl, not here.
          break;
        case 'help':
        case 'powered-by':
          $variables['attributes_array']['role'] = 'complementary';
          break;
        default:
          // Any other "system" block is a menu block.
          $variables['attributes_array']['role'] = 'navigation';
          break;
      }
      break;
    case 'menu':
    case 'menu_block':
    case 'blog':
    case 'book':
    case 'comment':
    case 'forum':
    case 'shortcut':
    case 'statistics':
      $variables['attributes_array']['role'] = 'navigation';
      break;
    case 'search':
      $variables['attributes_array']['role'] = 'search';
      break;
    case 'help':
    case 'aggregator':
    case 'locale':
    case 'poll':
    case 'profile':
      $variables['attributes_array']['role'] = 'complementary';
      break;
    case 'node':
      switch ($variables['block']->delta) {
        case 'syndicate':
          $variables['attributes_array']['role'] = 'complementary';
          break;
        case 'recent':
          $variables['attributes_array']['role'] = 'navigation';
          break;
      }
      break;
    case 'user':
      switch ($variables['block']->delta) {
        case 'new':
        case 'online':
          $variables['attributes_array']['role'] = 'complementary';
          break;
      }
      break;
  }
}

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
function groundwork_process_block(&$variables, $hook) {
  // Drupal 7 should use a $title variable instead of $block->subject.
  $variables['title'] = $variables['block']->subject;
}

/**
 * Preprocess variables for node.tpl.php
 * Override or insert variables into the node template.
 *
 */
function groundwork_preprocess_node(&$variables) {
  // Add $unpublished variable.
  $variables['unpublished'] = (!$variables['status']) ? TRUE : FALSE;
  $variables['classes_array'][] = $variables['zebra'];
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
  $variables['attributes_array']['role'][] = 'article';
  $variables['title_attributes_array']['class'][] = 'node-title';
  $variables['content_attributes_array']['class'][] = 'node-content';

  // Add nofollow to Book module print/export links
  if (isset($variables['content']['links']['book']['#links']['book_printer'])) {
    $variables['content']['links']['book']['#links']['book_printer']['attributes'] = array('rel' => array('nofollow'));
  }

  // add a $node_aside variable.
  if ($blocks = block_get_blocks_by_region('node_aside')) {
    $variables['node_aside'] = $blocks;
    $variables['node_aside'] = $blocks;
    $variables['node_aside']['#theme_wrappers'] = array('region');
    $variables['node_aside']['#region'] = 'node_aside';
    // add a class to the node as well, for easy styling.
    $variables['classes_array'][] = 'with-node-aside';
  }
  else {
    $variables['node_aside'] = '';
  }
}

/**
 * Preprocess variables for comment.tpl.php
 */
function groundwork_preprocess_comment(&$variables) {
  // Position and zebra striping
  if ($variables['id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  if ($variables['id'] == $variables['node']->comment_count) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['zebra'];
  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $variables['node']->type, 1) == 0) {
    $variables['title'] = '';
  }
  $variables['title_attributes_array']['class'][] = 'comment-title';
  $variables['content_attributes_array']['class'][] = 'comment-content';
}

/**
 * Changes the search form to use the HTML5 "search" input attribute.
 */
function groundwork_preprocess_search_block_form(&$variables) {
  $variables['search_form'] = str_replace('type="text"', 'type="search"', $variables['search_form']);
}

function groundwork_preprocess_search_result(&$variables) {
  $variables['theme_hook_suggestions'][] = 'search_result__' . $variables['module'];
}

function groundwork_preprocess_book_navigation(&$variables) {
  if (theme_get_setting('noceda_hide_book_navigation')) {
    $variables['noceda_hide_book_navigation_hide'] = ' visually-hidden';
  }
  else {
    $variables['noceda_hide_book_navigation_hide'] = '';
  }
}
