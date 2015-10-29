<?php
// $Id$

/**
 * @file
 * Theme Functions
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */
define('OM_BASE_THEME_PATH', drupal_get_path('theme', 'om'));  

include_once OM_BASE_THEME_PATH . '/inc/om_regions.inc'; 
include_once OM_BASE_THEME_PATH . '/inc/om_grids.inc'; 
include_once OM_BASE_THEME_PATH . '/inc/om_utils.inc'; 
include_once OM_BASE_THEME_PATH . '/inc/om_offline.inc'; 
include_once OM_BASE_THEME_PATH . '/inc/deprecated.inc'; 


/**
 * Implementation of hook_theme().
 *
 */
function om_theme() {
  //dsm($type);
  return array(
    'region_wrapper' => array( /* @Legacy - soon will be deleted */
      'variables' => array('region' => NULL, 'region_classes' => NULL, 'region_inner' => 0),
    ),
    'identity' => array(
      'variables' => array('logo' => NULL, 'site_name' => NULL, 'site_slogan' => NULL, 'front_page' => NULL),
    ),
    'content_elements' => array(
      'variables' => array('tabs' => NULL, 'prefix' => NULL, 'title' => NULL, 'suffix' => NULL, 'action' => NULL),
    ),    
    'menu' => array(
      'variables' => array('menu_name' => NULL, 'menu' => NULL, 'menu_tree' => NULL),
    ),              
  );
}


/** 
 * Identity
 *
 * Grouped variables
 * - Logo
 * - Site Name
 * - Site Slogan
 *
 */
function om_identity($vars) {
  if (!empty($vars['logo']) || !empty($vars['site_name']) || !empty($vars['site_slogan'])) { 
    $out = '<div id="logo-title">';
    if (!empty($vars['logo'])) $out .= '<a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home" id="logo"><img src="' . $vars['logo'] . '" alt="' . t('Home') . '" /></a>';
    if (!empty($vars['site_name']) || !empty($vars['site_slogan'])) { 
      $out .= '<div id="name-and-slogan">';
      if (!empty($vars['site_name'])) $out .= '<h2 id="site-name"><a href="' . $vars['front_page'] . '" title="' . t('Home') . '" rel="home">' . $vars['site_name'] . '</a></h2>';
      if (!empty($vars['site_slogan'])) $out .= '<div id="site-slogan">' . $vars['site_slogan'] . '</div>';
      $out .= '</div> <!-- /#name-and-slogan -->';
    }    
    $out .= '</div> <!-- /#logo-title -->';
    return $out;
  }
}


/**
 * Content Elements
 *
 * Grouped variables
 * - Tabs
 * - Prefix, Title, Suffix
 * - Action
 *
 */
function om_content_elements($vars) {
  $out = '';
  if (!empty($vars['tabs']))   $out .= '<div id="page-tabs" class="tabs">' . render($vars['tabs']) . '</div>'; 
  if (!empty($vars['prefix'])) $out .= render($vars['prefix']); 
  if (!empty($vars['title']))  $out .= '<h1 id="page-title" class="title">' . preg_replace('/\[break\]/', '<br />', $vars['title']) . '</h1>'; 
  if (!empty($vars['suffix'])) $out .= render($vars['suffix']); 
  if (!empty($vars['action'])) $out .= '<ul class="action-links">' . render($vars['action']) . '</ul>'; 
  return $out;
}


/**
 * Returns HTML for a breadcrumb trail.
 *
 * @param $vars
 *   An associative array containing:
 *   - breadcrumb: An array containing the breadcrumb links.
 */
function om_breadcrumb($vars) {
  $breadcrumb = $vars['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';

    $output .= '<div id="page-breadcrumb" class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
    return $output;
  }
}


/**
 * Primary, Secondary Menus
 *
 * Adding markups
 *
 */
function om_menu($vars) {
  if (isset($vars['menu_tree'])) return '<div id="menubar-' . $vars['menu_name'] . '" class="menubar">' . render($vars['menu_tree']) . '</div>';
}


/**
 * Implementation of theme_menu_link()
 *
 * Overriding the menu item behavior
 *
 */
function om_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';
  
  $mlid = $element['#original_link']['mlid'];
  $element['#attributes']['class'][] = 'menu-' . $mlid;
  if ($element['#below']) $sub_menu = drupal_render($element['#below']);
  
  // OM Tools integration
  if (module_exists('om_tools')) {
    $om_tools_values = variable_get('om_tools', '');
    if (isset($om_tools_values['menu']) && ($om_tools_values['menu']['menu_classes_switch'] == 1)) {
      $class = isset($om_tools_values['menu']['menu_classes_' . $mlid]) ? $om_tools_values['menu']['menu_classes_' . $mlid]: ''; 
      $element['#localized_options']['attributes']['class'][] = $class;
    }
  }
    
  $output = l($element['#title'], trim($element['#href']), $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}


/**
 * Preprocess variables for region.tpl.php
 *
 * Prepare the values passed to the theme_region function to be passed into a
 * pluggable template engine. Uses the region name to generate a template file
 * suggestions. If none are found, the default region.tpl.php is used.
 *
 * @see drupal_region_class()
 * @see region.tpl.php
 */
/* 
function om_preprocess_region(&$vars) {
  // get region inner variable
  //dsm($vars);
}
*/


/**
 * Process variables for html.tpl.php
 *
 * Perform final addition and modification of variables before passing into
 * the template. To customize these variables, call drupal_render() on elements
 * in $vars['page'] during THEME_preprocess_page().
 *
 * @see template_preprocess_html()
 * @see html.tpl.php
 */
function om_preprocess_html(&$vars) {
  global $theme; 
  global $theme_path;

  // additional info settings
  $info = drupal_parse_info_file($theme_path . '/' . $theme . '.info');
            
  $vars['head_title'] = preg_replace('/\[break\]/', '', $vars['head_title']);

  // body classes
  om_body_classes($vars, $info, NULL);
    
  // if om tools doesn't exist
  om_body_node_classes($vars);  
  
}

/**
 * Process variables for html.tpl.php
 *
 * Perform final addition and modification of variables before passing into
 * the template. To customize these variables, call drupal_render() on elements
 * in $variables['page'] during THEME_preprocess_page().
 *
 * @see template_preprocess_html()
 * @see html.tpl.php
 */
function om_process_html(&$vars) {
  global $theme; 
  global $theme_path;

  if (arg(0) == 'admin') {
    $theme = variable_get('admin_theme', 'om');
    $theme_path = drupal_get_path('theme', $theme);
  }

  // additional info settings
  $info = drupal_parse_info_file($theme_path . '/' . $theme . '.info');
  
  // addtional meta
  om_meta_get($vars, $info);

  // adding grids layout, guides for admin
  om_grids_html_vars($vars, $info);

  // Render page_top and page_bottom into top level variables.
  date_default_timezone_set('UTC');
  $vars['page_bottom'] .= '<div id="legal"><a href="http://www.drupal.org/project/om">OM Base Theme</a> ' . date('Y') . ' | V7.x-2.x | <a href="http://www.danielhonrade.com">Daniel Honrade</a></div>';
}

    
/**
 * Implementation of template_preprocess_page()
 *
 */
function om_preprocess_page(&$vars) {
  global $theme; 
  global $theme_path;

  // additional info settings
  $info = drupal_parse_info_file($theme_path . '/' . $theme . '.info');

  // get all region content, styles, scripts, grids
  om_region_process_variables($vars);
  
  // grid style, guide js
  om_grids_page_vars($vars, $info);

  // activates om offline countdown
  om_offline($vars, $info);
        
  // OM Tools integration
  $om_tools_values = variable_get('om_tools', '');
  $vars['title'] = drupal_get_title();
  if (module_exists('om_tools')) {
    $node = menu_get_object();        
    if ($node) {
      $node_type = (is_object($node)) ? $node->type: '';
      if (isset($om_tools_values['node']) && ($om_tools_values['node']['node_type_titles_switch'] == 1) && !empty($node_type)) {
        if ($om_tools_values['node']['node_' . $node_type . '_titles'] == 1) $vars['title'] = drupal_set_title('');
      }
    }
  }
  
  // i18n module integration  
  if (!isset($vars['main_menu_tree']) && empty($vars['main_menu_tree'])) {
    if (module_exists('i18n') && function_exists('i18n_menu_translated_tree')) {
      $vars['main_menu_tree'] = i18n_menu_translated_tree('main-menu');
    }
    else {
      $vars['main_menu_tree'] = menu_tree('main-menu'); 
    }
  }  

  // Generate menu tree from source of primary links
  $vars['main_menu_vars']        = ($vars['main_menu']) ? array('menu_name' => 'main-menu', 'menu' => $vars['main_menu'], 'menu_tree' => $vars['main_menu_tree']) : array();
  $vars['secondary_menu_tree']   = menu_tree('user-menu');
  $vars['secondary_menu_vars']   = ($vars['secondary_menu']) ? array('menu_name' => 'secondary-menu', 'menu' => $vars['secondary_menu'], 'menu_tree' => $vars['secondary_menu_tree']) : array();
  $vars['identity_vars']         = array('logo' => $vars['logo'], 'site_name' => $vars['site_name'], 'site_slogan' => $vars['site_slogan'], 'front_page' => $vars['front_page']);
  $vars['content_elements_vars'] = array('tabs' => $vars['tabs'], 'prefix' => $vars['title_prefix'], 'title' => $vars['title'], 'suffix' => $vars['title_suffix'], 'action' => $vars['action_links']);  
  //dsm($vars);
}

 
/**
 * Implementation of template_preprocess_block()
 *
 */
function om_preprocess_block(&$vars) {

  $blocks = _block_load_blocks();
  $region = $vars['block']->region;
  $vars['classes_array'][] = drupal_html_class('block-' . $vars['block_zebra']);
  $vars['classes_array'][] = drupal_html_class('block-' . $vars['block_id']);
  if (isset($blocks[$region])) $vars['classes_array'][] = drupal_html_class('block-group-' . count($blocks[$region])); 
    
  if ($vars['block_id'] == 1) $vars['classes_array'][] = drupal_html_class('block-first');
  if (isset($blocks[$region]) && ($vars['block_id'] == count($blocks[$region]))) $vars['classes_array'][] = drupal_html_class('block-last');
} 


/**
 * Act on blocks prior to rendering.
 *
 *
function om_block_list_alter(&$blocks) {
  //dsm($blocks);
}
*/







