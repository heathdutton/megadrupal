<?php
/**
 * @file
 * Contains theme override and preprocess functions 
 * for the Squid Pro theme.
 */
 
/**
 * Add theme call to define the contact form template and path
 */
function squid_pro_theme() {
	return array(
   		'contact_site_form' => array(
						'render element' => 'form',
						'template' => 'contact-site-form',
						'path' => drupal_get_path('theme', 'squid_pro').'/templates',
							),
  			);
}

/**
 * Preproccess call to process the site contact form
 */
function squid_pro_preprocess_contact_site_form(&$variables)
{
	//this is the contents of the form
	$variables['contact'] = drupal_render_children($variables['form']);
}

/**
 * Implements hook_html_head_alter().
 */
function squid_pro_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8',
  );
}

/**
 * Implements hook_breadcrumb().
 */
function squid_pro_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide title .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // Comment below line to hide current page to breadcrumb.
    $breadcrumb[] = '<a href="#">'.drupal_get_title().'</a>';
	 if(arg(0)=='search')
    {
      unset($breadcrumb[2]);
      unset($breadcrumb[3]);
    }
    $output .= '<nav class="breadcrumb">' . implode('', $breadcrumb) . '</nav>';
    return $output;
  }
}

/**
 * Implements hook_preprocess_page().
 */
function squid_pro_preprocess_page(&$vars) {

  // Add superfish plugin if available.
  if ($sf_path = libraries_get_path('superfish')) {
    $supersubs_path = $sf_path . '/supersubs.js';
    $superfish_path = $sf_path . '/superfish.js';
    drupal_add_js($supersubs_path);
    drupal_add_js($superfish_path);
    drupal_add_js(drupal_get_path('theme', 'squid_pro') . '/js/initialize.js');
  }
  
  // Add Font Awesome library if available (must be version 3.2.1).
  if ($faw_path = libraries_get_path('font-awesome')) {
    $fawesome_path = $faw_path . '/css/font-awesome.css';
    drupal_add_css($fawesome_path);
  }
  
  // Add javascript files for front-page jquery slideshow.
  if (drupal_is_front_page() && ($jcycle_path = libraries_get_path('jquery.cycle'))) {
    $jquery_cycle_path = $jcycle_path . '/jquery.cycle.all.js';
    drupal_add_js($jquery_cycle_path);
    drupal_add_js(drupal_get_path('theme', 'squid_pro') . '/js/slider/slide.js');
  }
  
  // Preprocess page.tpl.php to inject the search form block.
  $block = module_invoke('search', 'block_view', 'search');
  $rendered_block = render($block);
  $vars['squidpro_search_block'] = $rendered_block;
  
  // Search condition variable.
  $vars['display_search'] = theme_get_setting('show_search', 'squid_pro');
  
  // Sitename condition variable.
  $vars['display_sitename'] = theme_get_setting('show_sitename', 'squid_pro');
  
  // Social icons condition variable.
  $vars['display_social_icons'] = theme_get_setting('show_social_icons', 'squid_pro');
  
  // Social icons links variables.
  $vars['twitter_link'] = check_plain(theme_get_setting('twitter_link', 'squid_pro'));
  $vars['facebook_link'] = check_plain(theme_get_setting('facebook_link', 'squid_pro'));
  $vars['googleplus_link'] = check_plain(theme_get_setting('googleplus_link', 'squid_pro'));
  $vars['linkedin_link'] = check_plain(theme_get_setting('linkedin_link', 'squid_pro'));
  $vars['youtube_link'] = check_plain(theme_get_setting('youtube_link', 'squid_pro'));
  
  // Slideshow condition variable.
  $vars['display_slideshow'] = theme_get_setting('show_slideshow', 'squid_pro');
  
  // Slide description variables.
  $vars['slide_1_desc'] = check_markup(theme_get_setting('slide_1_desc', 'squid_pro'), 'full_html');
  $vars['slide_2_desc'] = check_markup(theme_get_setting('slide_2_desc', 'squid_pro'), 'full_html');
  $vars['slide_3_desc'] = check_markup(theme_get_setting('slide_3_desc', 'squid_pro'), 'full_html');
  
  // Breadcrumb condition variable.
  $vars['display_breadcrumb'] = theme_get_setting('show_breadcrumb', 'squid_pro');
  
  // Check if contact module is enabled and the current page is contact page.
  if (module_exists('contact')) {
   if ( arg(0) == 'contact' ) {
    $vars['title'] = t('Contact Us');
	drupal_set_title($vars['title']);
  }
  }
  
  // Process main menu / secondary menu if enabled.
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
function squid_pro_preprocess_html(&$variables) {
  drupal_add_css(
    'http://fonts.googleapis.com/css?family=Exo+2',
    array('type' => 'external')
    );

  // Add html5shiv plugin if available.
  if ($html5shiv = libraries_get_path('html5shiv')) {
    $html5shiv_path = $html5shiv . '/html5shiv.min.js';

// Adds a small amount of javascript code to the top of every page, so that HTML5 output will work correctly in IE.
$html5shiv = array(
  '#tag' => 'script',
  '#attributes' => array( // Set up an array of attributes inside the tag
    'src' => $GLOBALS['base_url'] . "/" . $html5shiv_path, 
  ),
  '#prefix' => '<!--[if lt IE 9]>',
  '#suffix' => '</script><![endif]-->',
);

drupal_add_html_head($html5shiv, 'html5shiv');
}
}

/**
 * Implements hook_menu_local_tasks().
 */
function squid_pro_menu_local_tasks(&$variables) {
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
function squid_pro_preprocess_node(&$variables) {
  if ($variables['view_mode'] == 'full' && node_is_page($variables['node'])) {
    $variables['classes_array'][] = 'node-full';
  }
  $variables['date'] = t('!datetime', array('!datetime' => date('l, d-M-Y', $variables['created'])));
}
