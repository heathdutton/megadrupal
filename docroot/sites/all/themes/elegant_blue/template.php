<?php
/**
 * @file
 * Contains theme override functions and preprocess functions for the theme.
 */

/**
 * Implements hook_html_head_alter().
 */
function elegant_blue_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Insert themed breadcrumb page navigation at top of the node content.
 */
function elegant_blue_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Comment below line to hide current page to breadcrumb.
    $breadcrumb[] = drupal_get_title();
    $output .= '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Add javascript files for front-page jquery slideshow.
 */
if (drupal_is_front_page()) {
  drupal_add_js(drupal_get_path('theme', 'elegant_blue') . '/js/slider.js');
}

/**
 * Add Google Fonts.
 */
function elegant_blue_preprocess_html(&$variables) {
  drupal_add_css('http://fonts.googleapis.com/css?family=Vollkorn', array('type' => 'external'));
  drupal_add_css('http://fonts.googleapis.com/css?family=Dancing+Script', array('type' => 'external'));
}

/**
 * Override or insert variables into the page template.
 */
function elegant_blue_preprocess_page(&$vars) {
  $vars['twitter'] = theme_get_setting('twitter', 'elegant_blue');
  $vars['facebook'] = theme_get_setting('facebook', 'elegant_blue');
  $vars['googleplus'] = theme_get_setting('googleplus', 'elegant_blue');
  $vars['linkedin'] = theme_get_setting('linkedin', 'elegant_blue');
  $vars['theme_path_social'] = base_path() . drupal_get_path('theme', 'elegant_blue');
  $vars['display'] = theme_get_setting('display', 'elegant_blue');
  $vars['footer_copyright'] = theme_get_setting('footer_copyright');
  $vars['footer_developed'] = theme_get_setting('footer_developed');
  $vars['footer_developedby_url'] = filter_xss_admin(theme_get_setting('footer_developedby_url', 'elegant_blue'));
  $vars['footer_developedby'] = filter_xss_admin(theme_get_setting('footer_developedby', 'elegant_blue'));
  $vars['searchblock'] = module_invoke('search', 'block_view', 'form');
  if (module_exists('i18n_menu')) {
    $vars['main_menu_tree'] = i18n_menu_translated_tree(variable_get('menu_main_links_source', 'main-menu'));
  }
  else {
    $vars['main_menu_tree'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
  }
  // Frontpage variables.
  $vars['slideshow_display'] = theme_get_setting('slideshow_display', 'elegant_blue');
  $vars['url1'] = theme_get_setting('slide1_url', 'elegant_blue');
  $vars['url2'] = theme_get_setting('slide2_url', 'elegant_blue');
  $vars['url3'] = theme_get_setting('slide3_url', 'elegant_blue');
  $vars['slide1'] = theme_get_setting('slide1_desc', 'elegant_blue');
  $vars['slide2'] = theme_get_setting('slide2_desc', 'elegant_blue');
  $vars['slide3'] = theme_get_setting('slide3_desc', 'elegant_blue');
  $vars['wtitle'] = filter_xss_admin(theme_get_setting('welcome_title', 'elegant_blue'));
  $vars['wtext'] = filter_xss_admin(theme_get_setting('welcome_text', 'elegant_blue'));
  $vars['col1'] = filter_xss_admin(theme_get_setting('colone', 'elegant_blue'));
  $vars['col1title'] = filter_xss_admin(theme_get_setting('colonetitle', 'elegant_blue'));
  $vars['col2'] = filter_xss_admin(theme_get_setting('coltwo', 'elegant_blue'));
  $vars['col2title'] = filter_xss_admin(theme_get_setting('coltwotitle', 'elegant_blue'));
  $vars['col3'] = filter_xss_admin(theme_get_setting('colthree', 'elegant_blue'));
  $vars['col3title'] = filter_xss_admin(theme_get_setting('colthreetitle', 'elegant_blue'));
  $vars['img1'] = base_path() . drupal_get_path('theme', 'elegant_blue') . '/images/slide-image-1.jpg';
  $vars['img2'] = base_path() . drupal_get_path('theme', 'elegant_blue') . '/images/slide-image-2.jpg';
  $vars['img3'] = base_path() . drupal_get_path('theme', 'elegant_blue') . '/images/slide-image-3.jpg';
  $image1var = array(
    'path' => $vars['img1'],
    'alt' => $vars['slide1'],
    'title' => $vars['slide1'],
    'attributes' => array('class' => 'slide-img'),
  );
  $vars['slideimage1'] = theme('image', $image1var);
  $image2var = array(
    'path' => $vars['img2'],
    'alt' => $vars['slide2'],
    'title' => $vars['slide2'],
    'attributes' => array('class' => 'slide-img'),
  );
  $vars['slideimage2'] = theme('image', $image2var);
  $image3var = array(
    'path' => $vars['img3'],
    'alt' => $vars['slide3'],
    'title' => $vars['slide3'],
    'attributes' => array('class' => 'slide-img'),
  );
  $vars['slideimage3'] = theme('image', $image3var);
}
