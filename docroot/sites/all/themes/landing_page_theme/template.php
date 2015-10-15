<?php
/**
 * @file
 * template.php
 */
require_once('landing_page_theme.inc');

/**
* Add modify variables before the html renders.
*/
function landing_page_theme_preprocess_html(&$variables) {
  $default_background = theme_get_setting('default_background');
  $theme_path = drupal_get_path('theme', 'landing_page_theme');
  $background_upload = theme_get_setting('background_upload');
  $variables['html_background_style'] = '';
  if ($default_background && $default_background == 1) {
    $variables['html_background_style'] =  'style="background-image:url(' . $theme_path . '/images/background.png)"';
  } 
  elseif ($background_upload) {
    $image = file_load($background_upload);
    $background_upload_path = file_create_url($image->uri);
    $variables['html_background_style'] = 'style="background-image:url(' . $background_upload_path . ')"';
  } 
}

/**
* Add modify variables before the page renders.
*/
function landing_page_theme_preprocess_page(&$vars) {
  if (isset($vars['node']->type)) {
    $vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
  }
  if ($fid = theme_get_setting('background_file')) {
    $variables['background_url'] = file_create_url(file_load($fid)->uri);
  }

  landing_page_theme_exist_libraries();
}

/**
* Implements template_menu_link function
*/
function landing_page_theme_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

//  l($element['#original_link']['link_title'], '', array('fragment' => 'namedanchor', 'external' => TRUE)); 

  // if it's main menu home link
  if (($element["#theme"] == "menu_link__main_menu") && ($element["#original_link"]["router_path"] == "") && !isset($element["#original_link"]["options"]["fragment"])) {
    $element['#attributes']['class'][]  = 'home';
    $logo_path = theme_get_setting('logo');
    $logo = array(
       'path' => $logo_path, 
       'alt' => t('Home'),
       'attributes' => array(),
    );
    $logo_img = theme_image($logo);
    $output = l(t('<i class="fa fa-drupal"></i>'), $element['#href'], array('html' => TRUE));
  }
  else{
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/*
* Override or insert variables into the page template.
*/
function landing_page_theme_process_page(&$variables) {

  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
}

/**
* Override or insert variables into the page template for HTML output.
*/
function landing_page_theme_process_html(&$vars) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($vars);
  }
}

function landing_page_theme_libraries_info() {
  $libraries = array();
  $libraries['appear'] = array(
    'name' => 'jQuery.appear',
    'vendor url' => 'https://github.com/morr/jquery.appear',
    'download url' => 'https://github.com/morr/jquery.appear',
    'version arguments' => array(
      'file' => 'jquery.appear.js',
      'pattern' => '/Version: (\d+\.+\d+\.+\d+)/',
      'lines' => 9,
    ),
    'files' => array(
      'js' => array('jquery.appear.js'),
    ),
  );
    $libraries['fontawesome'] = array(
        'name' => 'Font Awesome',
        'vendor url' => 'http://fontawesome.io',
        'download url' =>  'https://github.com/FortAwesome/Font-Awesome/archive/v4.2.0.zip',
        'version arguments' => array(
            'file' => 'css/font-awesome.css',
            'pattern' => '/((?:\d+\.?){2,3})/',
            'lines' => 10,
            'cols' => 14,
        ),
        'files' => array(
            'css' => array(
                'css/font-awesome.css',
            ),
        ),
    );
  return $libraries;
}
