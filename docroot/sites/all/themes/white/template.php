<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function white_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

function white_preprocess_html(&$variables) {

  $variables['white_tone'] = theme_get_setting('white_tone');
  if (!$variables['white_tone']) {
    $variables['white_tone'] = 'whitestandard.css';
  }
  
}  

/**
 * Override or insert variables into the page template.
 */
function white_process_page(&$variables) {
  
  $variables['copyright'] = theme_get_setting('copyright');
  if (!$variables['copyright']) {
	$variables['copyright'] = 'edit copyright text from theme setting page.';
  }
  
  $variables['biography_attribute']   = theme_get_setting('biography_attribute', 'biography') ? FALSE : TRUE;
  if (!$variables['biography_attribute']) {
	$variables['biography_attribute'] = TRUE;
  }
  
   // Get the entire main menu tree
  $main_menu_tree = menu_tree_all_data('main-menu');

  // Add the rendered output to the $main_menu_expanded variable
  $variables['main_menu_expanded'] = menu_tree_output($main_menu_tree);
  
    $variables['photo_1'] = theme_get_setting('photo_1');
  if (!$variables['photo_1']) {
	$variables['photo_1'] = 'sites/all/themes/white/css/images/1.jpg';
  }
    $variables['photo_2'] = theme_get_setting('photo_2');
  if (!$variables['photo_2']) {
	$variables['photo_2'] = 'http://farm4.staticflickr.com/3700/9463128385_bce6dc8205_h.jpg';
  }
    $variables['photo_3'] = theme_get_setting('photo_3');
  if (!$variables['photo_3']) {
	$variables['photo_3'] = 'http://upload.wikimedia.org/wikipedia/commons/5/52/IMG_1444_Kinkaku-ji.JPG';
  }
    $variables['photo_4'] = theme_get_setting('photo_4');
  if (!$variables['photo_4']) {
	$variables['photo_4'] = 'http://farm8.staticflickr.com/7305/8722022301_fd62249e7f_o.jpg';
  }
    $variables['photo_5'] = theme_get_setting('photo_5');
  if (!$variables['photo_5']) {
	$variables['photo_5'] = 'http://upload.wikimedia.org/wikipedia/commons/2/2d/%D0%9D%D1%96%D0%B6%D0%BD%D0%B8%D0%B9_%D1%80%D0%B0%D0%BD%D0%BA%D0%BE%D0%B2%D0%B8%D0%B9_%D1%81%D0%B2%D1%96%D1%82%D0%BB%D0%BE.jpg';
  }


}

function white_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Use CSS to hide titile .element-invisible.
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';
    // comment below line to hide current page to breadcrumb
	$breadcrumb[] = drupal_get_title();
    $output .= '<div class="breadcrumb">' . implode('<span class="sep">»</span>', $breadcrumb) . '</div>';
    return $output;
  }
}

