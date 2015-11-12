<?php
// $Id$

/**
 * @file
 * Contains theme override functions and preprocess functions for the jbase5 theme.
 */

/**
 * Changes the default meta content-type tag to the shorter HTML5 version
 */
function jbase5_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}


/**
 * Changes the search form to use the HTML5 "search" input attribute
 */
function jbase5_preprocess_search_block_form(&$vars) {
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
}


/**
 * Uses RDFa attributes if the RDF module is enabled
 * Lifted from Adaptivetheme for D7, full credit to Jeff Burnz
 * ref: http://drupal.org/node/887600
 * Custom classes provided by Fusion Core and IE specific stylesheets
 */
function jbase5_preprocess_html(&$vars) {
  // give <body> tag a unique id depending on PAGE PATH
  $path_alias = strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', drupal_get_path_alias($_GET['q'])));
  if ($path_alias == 'node') {
    $vars['body_id'] = 'page-front';
  }
  else {
    $vars['body_id'] = 'page-'. $path_alias;
  }

  // Add to the array of body classes
  // layout classes
  $vars['classes_array'][] = 'layout-'. (!empty($vars['page']['sidebar_first']) ? 'first-main' : 'main') . (!empty($vars['page']['sidebar_second']) ? '-second' : '');
  // preface classes
  if (!empty($vars['page']['preface_first']) || !empty($vars['page']['preface_second']) || !empty($vars['page']['preface_third'])) {
    $preface_regions = 'preface';
    $preface_regions .= (!empty($vars['page']['preface_first'])) ? '-first' : '';
    $preface_regions .= (!empty($vars['page']['preface_second'])) ? '-second' : '';
    $preface_regions .= (!empty($vars['page']['preface_third'])) ? '-third' : '';
    $vars['classes_array'][] = $preface_regions;
  }
  // postscripts classes
  if (!empty($vars['page']['postscript_first']) || !empty($vars['page']['postscript_second']) || !empty($vars['page']['postscript_third'])) {
    $postscript_regions = 'postscript';
    $postscript_regions .= (!empty($vars['page']['postscript_first'])) ? '-first' : '';
    $postscript_regions .= (!empty($vars['page']['postscript_second'])) ? '-second' : '';
    $postscript_regions .= (!empty($vars['page']['postscript_third'])) ? '-third' : '';
    $vars['classes_array'][] = $postscript_regions;
  }
  // footers classes
  if (!empty($vars['page']['footer_first']) || !empty($vars['page']['footer_second']) || !empty($vars['page']['footer_third'])) {
    $footer_regions = 'footers';
    $footer_regions .= (!empty($vars['page']['footer_first'])) ? '-first' : '';
    $footer_regions .= (!empty($vars['page']['footer_second'])) ? '-second' : '';
    $footer_regions .= (!empty($vars['page']['footer_third'])) ? '-third' : '';
    $vars['classes_array'][] = $footer_regions;
  }
  // Panels classes
  $vars['classes_array'][] = (module_exists('panels') && (panels_get_current_page_display())) ? 'panels' : '';
  if (module_exists('panels') && (panels_get_current_page_display())) {
    $panels_display = panels_get_current_page_display();
    $vars['classes_array'][] = 'panels-'. strtolower(preg_replace('/[^a-zA-Z0-9-]+/', '-', $panels_display->layout));
  }
  
  $vars['classes_array'] = array_filter($vars['classes_array']);

  // Add ie6, ie7, ie8 & ie9 stylesheets
  drupal_add_css(path_to_theme() . '/css/ie6-fixes.css',array('group' => CSS_THEME,'browsers' => array('IE' => 'IE 6','!IE' => FALSE,),'every_page' => TRUE,));
  drupal_add_css(path_to_theme() . '/css/ie7-fixes.css',array('group' => CSS_THEME,'browsers' => array('IE' => 'IE 7','!IE' => FALSE,),'every_page' => TRUE,));
  drupal_add_css(path_to_theme() . '/css/ie8-fixes.css',array('group' => CSS_THEME,'browsers' => array('IE' => 'IE 8','!IE' => FALSE,),'every_page' => TRUE,));
  drupal_add_css(path_to_theme() . '/css/ie9-fixes.css',array('group' => CSS_THEME,'browsers' => array('IE' => 'IE 9','!IE' => FALSE,),'every_page' => TRUE,));

  // Doctypes and RDF
  if (module_exists('rdf')) {
    $vars['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $vars['rdf']->version = 'version="HTML+RDFa 1.1"';
    $vars['rdf']->namespaces = $vars['rdf_namespaces'];
    $vars['rdf']->profile = ' profile="' . $vars['grddl_profile'] . '"';
  } else {
    $vars['doctype'] = '<!DOCTYPE html>' . "\n";
    $vars['rdf']->version = '';
    $vars['rdf']->namespaces = '';
    $vars['rdf']->profile = '';
  }
}


/**
 * Add preface, postscript, & footers classes with number of active sub-regions
 */
function jbase5_preprocess_page(&$vars) {

  // Page tpl suggestions
  if (isset($vars['node'])) {
    // If the node type is "blog_madness" the template suggestion will be "page--blog-madness.tpl.php".
    $vars['theme_hook_suggestions'][] = 'page__'. $vars['node']->type;
  }

  // Region list
  $region_list = array(
    'prefaces' => array('preface_first', 'preface_second', 'preface_third'), 
    'postscripts' => array('postscript_first', 'postscript_second', 'postscript_third'),
    'footers' => array('footer_first', 'footer_second', 'footer_third')
  );
  foreach ($region_list as $sub_region_key => $sub_region_list) {
    $active_regions = array();
    foreach ($sub_region_list as $region_item) {
      if (!empty($vars['page'][$region_item])) {
        $active_regions[] = $vars['page'][$region_item];
      }
    }
    $vars[$sub_region_key] = $sub_region_key .'-'. strval(count($active_regions));
  }
  
  // Render menu tree from main-menu
  $menu_tree = menu_tree('main-menu');
  $vars['main_menu_tree'] = render($menu_tree);
}


/**
 * Override or insert variables into the node templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function jbase5_preprocess_node(&$vars) {
  $vars['classes_array'][] = $vars['zebra'];
  if ($vars['view_mode'] == 'full') {
    $vars['classes_array'][] = 'node-full';
  }
  
  // Add node-type-page template suggestion
  if ($vars['page']) {
    $vars['theme_hook_suggestions'][] = 'node__'. $vars['node']->type .'-page';
    $vars['theme_hook_suggestions'][] = 'node__'. $vars['node']->type .'-'. $vars['node']->nid .'-page';
  }
  else {
    $vars['theme_hook_suggestions'][] = 'node__'. $vars['node']->type .'-teaser';
    $vars['theme_hook_suggestions'][] = 'node__'. $vars['node']->nid;
  }
}


/**
 * Preprocess variables for region.tpl.php
 *
 * Prepare the values passed to the theme_region function to be passed into a
 * pluggable template engine. Uses the region name to generate a template file
 * suggestions. If none are found, the default region.tpl.php is used.
 *
 * @see region.tpl.php
 */
function jbase5_preprocess_region(&$vars) {
  // Sidebar region template suggestion.
  if (strpos($vars['region'], 'sidebar_') === 0) {
    $vars['theme_hook_suggestions'][] = 'region__sidebar';
    $vars['theme_hook_suggestions'][] = 'region__' . $vars['region'];
  }
}


/**
 * Create unique class for block's order in region
 */
function jbase5_preprocess_block(&$vars) {
  $block = $vars['block'];
  // First/last block position
  $vars['position'] = ($vars['block_id'] == 1) ? 'block-first' : '';
  if ($vars['block_id'] == count(block_list($block->region))) {
    $vars['position'] = ($vars['position']) ? 'block-first block-last' : 'block-last';
  }
}


/**
 * Override or insert variables into the comment templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 */
function jbase5_preprocess_comment(&$vars) {
  // Add odd/even classes to comments classes_array 
  static $comment_odd = TRUE;
  $vars['classes_array'][] = $comment_odd ? 'odd' : 'even';
  $comment_odd = !$comment_odd;
}


/**
 * Output breadcrumb as an unorderd list with unique and first/last classes
 */
function jbase5_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    $crumbs = '<ul class="breadcrumbs clearfix">';
    $array_size = count($breadcrumb);
    $i = 0;
    while ( $i < $array_size) {
      $crumbs .= '<li class="breadcrumb-' . $i;
      if ($i == 0) {
        $crumbs .= ' first';
      }
      if ($i+1 == $array_size) {
        $crumbs .= ' last';
      }
      $crumbs .=  '">' . $breadcrumb[$i] . '</li>';
      $i++;
    }
    $crumbs .= '</ul>';
    return $crumbs;
  }
}


/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function jbase5_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<ul class="tabs primary clearfix">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs secondary clearfix">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}


/**
 * Override or insert variables into theme_menu_local_task().
 */
function jbase5_preprocess_menu_local_task(&$variables) {
  $link =& $variables['element']['#link'];

  // If the link does not contain HTML already, check_plain() it now.
  // After we set 'html'=TRUE the link will not be sanitized by l().
  if (empty($link['localized_options']['html'])) {
    $link['title'] = check_plain($link['title']);
  }
  $link['localized_options']['html'] = TRUE;
  $link['title'] = '<span class="tab">' . $link['title'] . '</span>';
}


/**
 * Add spans to form buttons for styling.
 */
function jbase5_button($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'submit';
  element_set_attributes($element, array('id', 'name', 'value'));

  $element['#attributes']['class'][] = 'form-' . $element['#button_type'];
  if (!empty($element['#attributes']['disabled'])) {
    $element['#attributes']['class'][] = 'form-button-disabled';
  }

  return '<span class="button-wrapper"><span class="button"><span>' . '<input' . drupal_attributes($element['#attributes']) . ' />' . '</span></span></span>';
}


/**
 * Changes labels from <span> to <label>
 * Also adds field-item count as class for field-item
 */
function jbase5_field($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<label>' . $variables['label'] . ':&nbsp;</label>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  $count = 0;
  foreach ($variables['items'] as $delta => $item)  {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even') . ' field-item-' . ++$count;
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}