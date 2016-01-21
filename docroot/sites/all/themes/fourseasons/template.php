<?php 


/*

	What do i need for template.php
	add breadcrumb
	override theme_form for xhtml compliance

*/
/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
*/
function fourseasons_preprocess_page(&$variables) {
	if (variable_get('theme_fourseasons_first_install', TRUE)) {
    	include_once('theme-settings.php');
	    fourseasons_install();
  	}
	Drupal_add_js(
      array(
        'fourseasons' => array(
          'base_color' => theme_get_setting('fourseasons-basecolor'),
          )
      ),
      array('type' => 'setting')
    );
	
	$banners = fourseasons_display_banners(fourseasons_get_banners());
	$variables['header_image'] = fourseasons_show_banner($banners);
	
}

/**
 * Override of theme_file().
 * Reduce the size of the upload fields.
 */
function fourseasons_file($variables) {

 $element = $variables['element'];
 $element['#attributes']['type'] = 'file';
 $element['#attributes']['size'] = '20';
 
 element_set_attributes($element, array('id', 'name', 'size'));
 _form_set_class($element, array('form-file'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}



/**
 * Override of theme_fieldset().
 * We use divs instead of fieldsets for better theming.
 */

function fourseasons_fieldset($variables) {
	$_path = drupal_strtolower(drupal_get_path_alias($_GET['q']));
	//if(!drupal_match_path($_path, "admin/appearance/settings/fourseasons")) {
  $element = $variables['element'];
  element_set_attributes($element, array('id'));
  _form_set_class($element, array('form-wrapper'));
  
  
 //
  $output = '<div' . drupal_attributes($element['#attributes']) . '>';
  if (!empty($element['#title'])) {
    $output .= '<div class="pseudo-fieldset-title">'. $element['#title'] .'</div>';
  }
   $output .= '<div class="pseudo-fieldset-content">';
  if (!empty($element['#description'])) {
    $output .= '<div class="fieldset-description">' . $element['#description'] . '</div>';
  }
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  $output .= '</div>';
  $output .= "</div>\n";
  return $output;
 //}
}
//*/
/*
 * Override of theme_form().
*/
function fourseasons_form($variables) {
  $element = $variables['element'];
$action = $element['#action'] ? 'action="'. check_url($element['#action']) .'" ' : '';
  if (isset($element['#action'])) {
    $element['#attributes']['action'] = drupal_strip_dangerous_protocols($element['#action']);
  }
  if ($element['#id'] == 'system-modules') {
    return "<a href='#' onclick='javascript:return false;' id='collapse-all-fieldsets'>".t('Collapse all fieldsets')."</a> | <a href='#' onclick='javascript:return false;' id='open-all-fieldsets'>".t('Open all fieldsets').'</a><form '. $action .' accept-charset="UTF-8" method="'. $element['#method'] .'" id="'. $element['#id'] .'"'. drupal_attributes($element['#attributes']) .">\n<div>". $element['#children'] ."\n</div></form>\n";    
  } else {
	return '<form '. $action .' accept-charset="UTF-8" method="'. $element['#method'] .'" id="'. $element['#id'] .'"'. drupal_attributes($element['#attributes']) .">\n<div>". $element['#children'] ."\n</div></form>\n";   
  }
}

function fourseasons_get_banners($all = TRUE) {
  $banners = variable_get('theme_fourseasons_header_settings', array());

  if (!$all) {
    $banners_value = array();
    foreach ($banners as $banner) {
      if ($banner['image_published']) {
        $banners_value[] = $banner;
		
      }
    }
    return $banners_value;
  } else {
    return $banners;
  }
}

function fourseasons_set_banners($value) {

  variable_set('theme_fourseasons_header_settings', $value);
}

/*
	Display 1 header image. Make it random if there are more then 1.
	TODO: make a cycle if there are more then 1 images.
*/
function fourseasons_show_banner($banners){

  $output = '';
  $_numbanners = count($banners);
  
  switch ($_numbanners) {
	case "0";
		return false;
		break;
	case "1":
		$i = 0;
		break;
	default:
		$i = rand(0, $_numbanners) -1;
  }
  $banner = $banners[$i];
  $variables = array(
    'path'  => $banner['image_path'],
    'alt' => $banner['image_description'],
    'title'   => $banner['image_title'],
  );
  // Draw image
  $image   = theme('image', $variables);
  // Remove link if is the same page
  $banner['image_url'] = ($banner['image_url'] == current_path()) ? FALSE  : $banner['image_url'];
  // Add link (if required)
  $output .= $banner['image_url'] ? l($image, $banner['image_url'], array('html' => TRUE)) : $image;
  
  return $output;
}

function fourseasons_display_banners() {
  $banners = fourseasons_get_banners(FALSE);
  $display_banners = array();

  // Current path alias
  $path = drupal_strtolower(drupal_get_path_alias($_GET['q']));

  // Check visibility for each banner
  foreach ($banners as $banner) {
    // Pages
    $pages = drupal_strtolower($banner['image_visibility']);

    // Check path for alias, and (if required) for path
    $page_match = drupal_match_path($path, $pages);
    if ($path != $_GET['q']) {
      $page_match = $page_match || drupal_match_path($_GET['q'], $pages);
    }

    // Add banner to visible banner
    if ($page_match) {
      $display_banners[] = $banner;
    }
  }
  return $display_banners;
}
//*/
/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *   An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.

function phptemplate_breadcrumb($breadcrumb) {
  if (!empty($breadcrumb)) {
    return '<div class="breadcrumb"><span>'. implode('</span> <span class="breadcrumb-separator">&rsaquo;</span> <span>', $breadcrumb) .'</span></div>';
  }
}
 */