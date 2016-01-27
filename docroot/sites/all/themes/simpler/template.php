<?php
// $Id: template.php,v 1.7 2010/11/14 01:41:18 danprobo Exp $
function simpler_page_class($sidebar_first, $sidebar_second) {
	if ($sidebar_first && $sidebar_second) {
		$class = 'sidebars-2';
		$id = 'sidebar-side-2';	
	}
	else if ($sidebar_first || $sidebar_second) {
		$class = 'sidebars-1';
		$id = 'sidebar-side-1';
	}

	if(isset($id)) {
		print ' id="'. $id .'"';
	}
	
	if(isset($class)) {
		print ' class="'. $class .'"';
	}

}

function simpler_preprocess_html(&$variables) {
  drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

