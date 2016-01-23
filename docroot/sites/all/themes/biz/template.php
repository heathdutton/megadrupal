<?php

function biz_preprocess_page(&$vars) {
  $use_font = theme_get_setting('use_font');
  $use_default_logo = theme_get_setting('default_logo');
  if($use_font == 2 && file_exists(variable_get('file_public_path', conf_path() . '/files') ."/fontlogo.png") && $use_default_logo){
  unset($vars['logo']);
  $vars['logo'] = base_path().variable_get('file_public_path', conf_path() . '/files').'/fontlogo.png';
  } 
  drupal_add_js('misc/jquery.js', array(
   'type' => 'file', 
   'scope' => 'header',
   'every_page' => TRUE,
   'weight' => -10,  
   )
  );
  drupal_add_js(drupal_get_path('theme', 'biz').'/js/superfish.js',
    array('type' => 'file', 'scope' => 'header', 'weight' => 5)
  );
drupal_add_js('(function ($) {
$(\'ul.sf-menu\').superfish({
            delay:       500,                            // one second delay on mouseout
            animation:   {opacity:\'show\',height:\'show\'},  // fade-in and slide-down animation
            speed:       \'fast\',                          // faster animation speed
            autoArrows:  false,                           // disable generation of arrow mark-up
            dropShadows: false                            // disable drop shadows
        });
   }(jQuery));',
    array('type' => 'inline', 'scope' => 'footer')
  );
}

?>
