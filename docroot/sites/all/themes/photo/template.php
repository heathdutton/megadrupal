<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function photo_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

function photo_preprocess_html(&$variables) {

  $variables['photo_bg'] = theme_get_setting('photo_bg');
  if (!$variables['photo_bg']) {
  global $base_url;
  $variables['photo_bg'] = $base_url.'/sites/all/themes/photo/css/images/1024px-2002-07_Sylt_-_Groyne.jpg';
  }
  
}  

/**
 * Override or insert variables into the page template.
 */
function photo_process_page(&$variables) {
  
  $variables['photo_1'] = theme_get_setting('photo_1');
  if (!$variables['photo_1']) {
	$variables['photo_1'] = 'http://upload.wikimedia.org/wikipedia/commons/9/95/2002-07_Sylt_-_Groyne.jpg';
  }
  $variables['attribute_photo_1'] = theme_get_setting('attribute_photo_1');
  if (!$variables['attribute_photo_1']) {
	$variables['attribute_photo_1'] = 'Magnus Manske';
  }
  
    $variables['photo_2'] = theme_get_setting('photo_2');
  if (!$variables['photo_2']) {
	$variables['photo_2'] = 'http://upload.wikimedia.org/wikipedia/commons/2/20/2003-05_Sylt_-_Westerland_Promenade.jpg';
  }
  $variables['attribute_photo_2'] = theme_get_setting('attribute_photo_2');
  if (!$variables['attribute_photo_2']) {
	$variables['attribute_photo_2'] = 'Magnus Manske';
  }
  
    $variables['photo_3'] = theme_get_setting('photo_3');
  if (!$variables['photo_3']) {
	$variables['photo_3'] = 'http://upload.wikimedia.org/wikipedia/commons/5/52/IMG_1444_Kinkaku-ji.JPG';
  }
  $variables['attribute_photo_3'] = theme_get_setting('attribute_photo_3');
  if (!$variables['attribute_photo_3']) {
	$variables['attribute_photo_3'] = 'Elly Waterman';
  }


}

function photo_page_alter($page) {

}
