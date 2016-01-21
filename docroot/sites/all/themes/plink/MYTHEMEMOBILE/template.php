<?php

// Preprocess Hooks
// -------------------------------------------------------------------------------------

// function MYTHEMEMOBILE_preprocess(&$vars, $hook) { }
// 
function MYTHEMEMOBILE_preprocess_html(&$vars) { 
  
  drupal_add_css('http://code.jquery.com/mobile/1.0a3/jquery.mobile-1.0a3.min.css',array('type'=>'external'));
  drupal_add_js('http://code.jquery.com/jquery-1.5.min.js',array('type'=>'external'));
	drupal_add_js('http://code.jquery.com/mobile/1.0a3/jquery.mobile-1.0a3.min.js', array('type'=>'external'));

}
// 
// function MYTHEMEMOBILE_preprocess_page(&$vars) { }
// 
// function MYTHEMEMOBILE_preprocess_region(&$vars) {}
// 
// function MYTHEMEMOBILE_preprocess_maintenance_page(&$vars) { }
// 
// function MYTHEMEMOBILE_preprocess_node(&$vars) { }
// 
// function MYTHEMEMOBILE_preprocess_block(&$vars) { }
// 
// function MYTHEMEMOBILE_preprocess_comment(&$vars) {}


// Process hooks
// -------------------------------------------------------------------------------------


// function MYTHEMEMOBILE_process(&$vars, $hook) { }
// 
// function MYTHEMEMOBILE_process_html(&$vars) { }
// 
// function MYTHEMEMOBILE_process_page(&$vars) { }
// 
// function MYTHEMEMOBILE_process_region(&$vars) {}
// 
// function MYTHEMEMOBILE_process_node(&$vars) { }
// 
// function MYTHEMEMOBILE_process_block(&$vars) { }
// 
// function MYTHEMEMOBILE_process_comment(&$vars) { }





