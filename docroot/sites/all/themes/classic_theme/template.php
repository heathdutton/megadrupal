<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function classic_theme_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

/**
 * HTML preprocessing
 */
function classic_theme_preprocess_html(&$vars) {
  // Add body classes for custom design options
  $colors = theme_get_setting('color_scheme', 'classic_theme');
  $classes = explode(" ", $colors);
  if (!theme_get_setting('backgroundimg', 'classic_theme')){
    $vars['classes_array'][] = 'nobkimg';
  }
  for($i=0; $i<count($classes); $i++){
    $vars['classes_array'][] = $classes[$i];
  }
}


/**
  * Theme override for search form.
  */

function classic_theme_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'search_block_form') {
    $form['search_block_form']['#title'] = '';
    $form['search_block_form']['#default_value'] = 'Search.';
    $form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = 'Search.';}";
    $form['search_block_form']['#attributes']['onfocus'] = "if (this.value == 'Search.') {this.value = '';}";
    $form['actions']['submit']['#value'] = t('');
  }    
}


/**
 * Insert themed breadcrumb page navigation at top of the node content.
 */
function classic_theme_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // comment below line to hide current page to breadcrumb
	$breadcrumb[] = drupal_get_title();
    $output .= '<nav class="breadcrumb">' . implode(' » ', $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Override or insert variables into the page template.
 */
function classic_theme_preprocess_page(&$vars) {
  if (isset($vars['main_menu'])) {
    $vars['main_menu'] = theme('links__system_main_menu', array(
      'links' => $vars['main_menu'],
      'attributes' => array(
        'class' => array('links', 'main-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Main menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['main_menu'] = FALSE;
  }
  if (isset($vars['secondary_menu'])) {
    $vars['secondary_menu'] = theme('links__system_secondary_menu', array(
      'links' => $vars['secondary_menu'],
      'attributes' => array(
        'class' => array('links', 'secondary-menu', 'clearfix'),
      ),
      'heading' => array(
        'text' => t('Secondary menu'),
        'level' => 'h2',
        'class' => array('element-invisible'),
      )
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}

/**
 * Duplicate of theme_menu_local_tasks() but adds clearfix to tabs.
 */
function classic_theme_menu_local_tasks(&$variables) {
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
 * Override or insert variables into the node template.
 */
function classic_theme_preprocess_node(&$variables) {
  $node = $variables['node'];
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
}

/**
 * Add css for color style.
 */
if (theme_get_setting('color_scheme', 'classic_theme') == 'dark') {
  drupal_add_css(drupal_get_path('theme', 'classic_theme') . '/css/color-schemes.css');
}

/**
 * Add css for background image.
 */
if (!theme_get_setting('backgroundimg', 'classic_theme')) {
  drupal_add_css(drupal_get_path('theme', 'classic_theme') . '/css/background.css');
}

/**
 * Add javascript files for front-page jquery slideshow.
 */
if (drupal_is_front_page()) {
  if (theme_get_setting('slideshow_display', 'classic_theme')){
    drupal_add_js(drupal_get_path('theme', 'classic_theme') . '/js/slides.min.jquery.js');
    drupal_add_js(drupal_get_path('theme', 'classic_theme') . '/js/slider.js');
  }
}

