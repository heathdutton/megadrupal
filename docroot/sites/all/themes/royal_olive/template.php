<?php
/**
 * @file
 * Contains theme override functions and preprocess functions
 * for the royal_olive theme.
 */

/**
 * Implements hook_html_head_alter().
 */
function royal_olive_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements hook_breadcrumb().
 */
function royal_olive_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide title .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Comment below line to hide current page to breadcrumb.
    $breadcrumb[] = drupal_get_title();
    $output .= '<nav class="breadcrumb">' . implode(' » ', $breadcrumb) . '</nav>';
    return $output;
  }
}


/**
 * Implements hook_preprocess_page().
 */
function royal_olive_preprocess_page(&$vars) {
  // Add superfish plugin if available.
  if ($sf_path = libraries_get_path('superfish')) {
    $superfish_path = $sf_path . '/superfish.js';
    drupal_add_js($superfish_path);
    drupal_add_js(drupal_get_path('theme', 'royal_olive') . '/js/initialize.js');
  }

  // Add Font Awesome library if available (must be version 3.2.1).
  if ($faw_path = libraries_get_path('font-awesome')) {
    $fawesome_path = $faw_path . '/css/font-awesome.css';
    drupal_add_css($fawesome_path);
  }

  // Search form condition variable.
  $dis_search = theme_get_setting('show_search', 'royal_olive');
  $vars['display_search'] = $dis_search;

  // Preprocess page.tpl.php to inject the search form block.
  $block = module_invoke('search', 'block_view', 'search');
  $rendered_block = render($block);
  $vars['olive_search_block'] = $rendered_block;

  // Social icons condition variable.
  $dis_social_icons = theme_get_setting('show_social_icons', 'royal_olive');
  $vars['display_social_icons'] = $dis_social_icons;

  // Social icons links variables.
  $twitt_link = check_plain(theme_get_setting('twitter_link', 'royal_olive'));
  $vars['twitter_link'] = $twitt_link;
  $faceb_link = check_plain(theme_get_setting('facebook_link', 'royal_olive'));
  $vars['facebook_link'] = $faceb_link;
  $googlep_link = check_plain(theme_get_setting('googleplus_link', 'royal_olive'));
  $vars['googleplus_link'] = $googlep_link;
  $linked_link = check_plain(theme_get_setting('linkedin_link', 'royal_olive'));
  $vars['linkedin_link'] = $linked_link;
  $pint_link = check_plain(theme_get_setting('pinterest_link', 'royal_olive'));
  $vars['pinterest_link'] = $pint_link;

  // Slideshow condition variable.
  $dis_slideshow = theme_get_setting('show_slideshow', 'royal_olive');
  $vars['display_slideshow'] = $dis_slideshow;

  // Slide description variables.
  $slide_1 = check_markup(theme_get_setting('slide_1_desc', 'royal_olive'), 'full_html');
  $vars['slide_1_desc'] = $slide_1;
  $slide_2 = check_markup(theme_get_setting('slide_2_desc', 'royal_olive'), 'full_html');
  $vars['slide_2_desc'] = $slide_2;
  $slide_3 = check_markup(theme_get_setting('slide_3_desc', 'royal_olive'), 'full_html');
  $vars['slide_3_desc'] = $slide_3;

  // Breadcrumb condition variable.
  $dis_breadcrumb = theme_get_setting('show_breadcrumb', 'royal_olive');
  $vars['display_breadcrumb'] = $dis_breadcrumb;

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
      ),
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
      ),
    ));
  }
  else {
    $vars['secondary_menu'] = FALSE;
  }
}

/**
 * Implements hook_preprocess_html().
 */
function royal_olive_preprocess_html(&$variables) {
  drupal_add_css(
    'http://fonts.googleapis.com/css?family=Ubuntu:300',
    array('type' => 'external')
    );

  // Add javascript files for front-page jquery slideshow.
  if (drupal_is_front_page() && ($jcycle_path = libraries_get_path('jquery.cycle'))) {
    $jquery_cycle_path = $jcycle_path . '/jquery.cycle.all.js';
    drupal_add_js($jquery_cycle_path);
    drupal_add_js(drupal_get_path('theme', 'royal_olive') . '/js/slider/slide.js');
  }
}

/**
 * Implements hook_menu_local_tasks().
 */
function royal_olive_menu_local_tasks(&$variables) {
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
 * Implements hook_preprocess_node().
 */
function royal_olive_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
  $variables['date'] = t('!datetime', array('!datetime' => date('D, j M - Y', $variables['created'])));
}
