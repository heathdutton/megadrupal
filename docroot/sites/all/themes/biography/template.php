<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function biography_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

function biography_preprocess_html(&$variables) {


  
}  

/**
 * Override or insert variables into the page template.
 */
function biography_process_page(&$variables) {
  
  $variables['bio_name'] = theme_get_setting('bio_name');
  if (!$variables['bio_name']) {
	$variables['bio_name'] = 'Name of Biography person.';
  }
  
  $variables['copyright'] = theme_get_setting('copyright');
  if (!$variables['copyright']) {
	$variables['copyright'] = 'edit copyright text from theme setting page.';
  }
  
  $variables['biography_attribute']   = theme_get_setting('biography_attribute', 'biography') ? FALSE : TRUE;
  if (!$variables['biography_attribute']) {
	$variables['biography_attribute'] == '1';
  }

}

function biography_page_alter($page) {

}
