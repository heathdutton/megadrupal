<?php

// See theme-settings.php

// Load CSS files depending upon Theme Settings
function squaregrid_preprocess_html(&$vars) {
  // first load css that always applies
  drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/normalize.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-10'));
  drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/default.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-9'));

  // Load appropriate squaregrid css depending upon break_point setting
  switch (theme_get_setting('break_point')) {
    case 'break320':
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/squaregrid360.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-8'));
      break;
    case 'break480':
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/squaregrid480.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-8'));
      break;
    case 'break640':
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/squaregrid640.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-8'));
      break;
    case 'break768':
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/squaregrid768.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-8'));
      break;
    case 'break800':
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/squaregrid800.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '-8'));
      break;
  break;
  }

  $vars['path_to_squaregrid'] = (drupal_get_path('theme', 'squaregrid'));
    // If "html5shiv" selected:
  if (theme_get_setting('html5shiv')) {
    // set showgrid variable for html.tpl.php 
    $vars['add_html5shiv'] = 1;
  }
  else {
    $vars['add_html5shiv'] = 0;
  }

    // If "mobile web app" selected:
  if (theme_get_setting('mobilewebapp')) {
    // set showgrid variable for html.tpl.php 
    $vars['meta_mobilewebapp'] = 1;
  }
  else {
    $vars['meta_mobilewebapp'] = 0;
  }

    // If "show grid" selected:
  if (theme_get_setting('showgrid')) {
    // set showgrid variable for html.tpl.php 
    $vars['grid'] = 'showgrid';
    // load the global showgrid stylesheet
    drupal_add_css(drupal_get_path('theme', 'squaregrid') . '/styles/showgrid.css', array('group' => CSS_THEME, 'preprocess' => TRUE, 'every_page' => TRUE, 'weight' => '6'));
  }
  else {
    $vars['grid'] = 'nogrid';
  }
}

function squaregrid_preprocess_page(&$variables, $hook) {
  $page = $variables['page'];

  // set maximum width
  $variables['max_width'] = theme_get_setting('max_width');

  // Set page region classes for main and supplementary content, based on settings
  // if both sidebars are present
  if ($page['sidebar_first'] && $page['sidebar_second']) {
    $variables['class_content'] = (theme_get_setting('c3_main_width')) . t(' ') . (theme_get_setting('c3_main_push'));
    $variables['class_sidebar_first'] = (theme_get_setting('c3_supp1_width')) . t(' ') . (theme_get_setting('c3_supp1_push'));
    $variables['class_sidebar_second'] = (theme_get_setting('c3_supp2_width'))  . t(' ') . (theme_get_setting('c3_supp2_push'));
  }
  // if only first sidebar is present
  if ($page['sidebar_first'] && !$page['sidebar_second']) {
    $variables['class_content'] = (theme_get_setting('c2_s1_main_width')) . t(' ') . (theme_get_setting('c2_s1_main_push'));
    $variables['class_sidebar_first'] = (theme_get_setting('c2_s1_supp1_width')) . t(' ') . (theme_get_setting('c2_s1_supp1_push'));
  }
  // if only second sidebar is present
  if ($page['sidebar_second'] && !$page['sidebar_first']) {
    $variables['class_content'] = (theme_get_setting('c2_s2_main_width')) . t(' ') . (theme_get_setting('c2_s2_main_push'));
    $variables['class_sidebar_second'] = (theme_get_setting('c2_s2_supp2_width')) . t(' ') . (theme_get_setting('c2_s2_supp2_push'));
  }
  // if no sidebar is present
  if (!$page['sidebar_first'] && !$page['sidebar_second']) {
    $variables['class_content'] = (theme_get_setting('c1_main_width')) . t(' ') . (theme_get_setting('c1_main_push'));
  }

  // For easy printing of basic page elements
  // Branding
  $variables['logo_img'] = '';
  if (!empty($variables['logo'])) {
    $variables['logo_img'] = theme('image__logo', array(
      'path'  => $variables['logo'],
      'alt'   => t('Site Logo'),
      'attributes' => array(
        'id'   => t('main-logo'),
        'class' => t('logo'),
      ),
    ));
  }
  $variables['linked_logo_img']  = '';
  if (!empty($variables['logo_img'])) {
    $variables['linked_logo_img'] = l($variables['logo_img'], '<front>', array(
      'html' => TRUE,
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
    ));
  }
  $variables['linked_site_name'] = '';
  if (!empty($variables['site_name'])) {
    $variables['linked_site_name'] = l($variables['site_name'], '<front>', array(
      'attributes' => array(
        'rel'   => 'home',
        'title' => t('Home'),
      ),
    ));
  }

  // Site navigation links.
  $variables['main_menu_links'] = theme('links__system_main_menu', array(
    'links' => $variables['main_menu'],
    'attributes' => array(
      'id'    => 'main-menu',
      'class' => array('main-menu', 'inline'),
      'role'  => 'menu',
    ),
    'heading' => array(
      'text'  => t('Main menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  ));
  $variables['secondary_menu_links'] = theme('links__system_secondary_menu', array(
    'links' => $variables['secondary_menu'],
    'attributes' => array(
      'id'    => 'secondary-menu',
      'class' => array('secondary-menu', 'inline', 'hello'),
      'role'  => 'menu',
   ),
    'heading' => array(
      'text'  => t('Secondary menu'),
      'level' => 'h2',
      'class' => array('element-invisible'),
    ),
  ));
}

// Add ARIA roles for main and supplementary content
function squaregrid_preprocess_region(&$variables, $hook) {

  $variables['region'] = drupal_html_id($variables['region']);

  // Sidebar regions get some extra classes and a common template suggestion.
  if (strpos($variables['region'], 'sidebar_') === 0) {
    $variables['classes_array'][] = 'supplementary-content';
    $variables['attributes_array']['role'][] = 'complementary';
  }
  if (strpos($variables['region'], 'content') === 0) {
    $variables['attributes_array']['role'][] = 'main';
  }
}

// Add helpful classes to node arrays
function squaregrid_preprocess_node(&$variables, $hook) {

  // Add an unpublished variable
  $variables['unpublished'] = (!$variables['status']) ? TRUE : FALSE;

  // Add a class for user's own node
  if ($variables['uid'] && $variables['uid'] == $GLOBALS['user']->uid) {
    $variables['classes_array'][] = 'node-by-viewer';
  }
  $variables['classes_array'][] = $variables['view_mode'];
  $variables['classes_array'][] = 'clearfix';
  $variables['attributes_array']['role'][] = 'article';

  $variables['title_attributes_array']['class'][] = 'node-title';
  $variables['title_attributes_array']['class'][] =  $variables['type'];
  $variables['title_attributes_array']['class'][] =  $variables['view_mode'];
  $variables['title_attributes_array']['rel'][] = 'bookmark';
  //add ARIA role to node article element
}

// Add helpful classes to comment arrays
function squaregrid_preprocess_comment(&$variables, $hook) {

  // If comment subjects are disabled, don't display them.
  if (variable_get('comment_subject_field_' . $variables['node']->type, 1) == 0) {
    $variables['title'] = '';
  }

  // Add zebra classes
  if ($variables['id'] == 1) {
    $variables['classes_array'][] = 'first';
  }
  if ($variables['id'] == $variables['node']->comment_count) {
    $variables['classes_array'][] = 'last';
  }
  $variables['classes_array'][] = $variables['zebra'];

  // Mark title with comment title class
  $variables['title_attributes_array']['class'][] = 'comment-title';
}

// block classes and ARIA roles
function squaregrid_preprocess_block(&$variables) {
  $variables['classes_array'][] = $variables['block_zebra'];

  $variables['title_attributes_array']['class'][] = 'block-title';

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
      $variables['attributes_array']['role'] = 'navigation';
      break;
    case 'menu_block':
      $variables['attributes_array']['role'] = 'navigation';
      break;
    case 'blog':
    case 'book':
      $variables['attributes_array']['role'] = 'navigation';
      break;
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
      $variables['attributes_array']['role'] = 'note';
      break;
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
        case 'login':
          $variables['attributes_array']['role'] = 'form';
          break;
        case 'new':
        case 'online':
          $variables['attributes_array']['role'] = 'complementary';
          break;
      }
      break;
  }
}