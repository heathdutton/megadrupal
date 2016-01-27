<?php
/**
 * Implements hook_html_head_alter().
 * This will overwrite the default meta character type tag with HTML5 version.
 */
function beach_html_head_alter(&$head_elements) {
  $head_elements['system_meta_content_type']['#attributes'] = array(
    'charset' => 'utf-8'
  );
}

function beach_preprocess_html(&$variables) {

  $variables['beach_width'] = theme_get_setting('beach_width');
  if (!$variables['beach_width']) {
    $variables['beach_width'] = '960px';
  }
  
}  

/**
 * Override or insert variables into the page template.
 */
function beach_process_page(&$variables) {
  //Logic for footer blocks.  
  
}
