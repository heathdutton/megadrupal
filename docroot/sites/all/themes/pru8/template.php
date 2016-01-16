<?php
function pru8_page_class($sidebar_first, $sidebar_second) {
	if ($sidebar_first && $sidebar_second) {
		$id = 'layout-type-2';	
	}
	else if ($sidebar_first || $sidebar_second) {
		$id = 'layout-type-1';
	}

	if(isset($id)) {
		print ' id="'. $id .'"';
	}
}

function pru8_preprocess_html(&$vars) {
  // Add conditional CSS for IE6.
drupal_add_css(path_to_theme() . '/style.ie6.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 6', '!IE' => FALSE), 'preprocess' => FALSE));
}

function pru8_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'pru8') . '/maintenance.css');
  drupal_add_js(drupal_get_path('theme', 'pru8') . '/scripts/jquery.cycle.all.js');
}

if (drupal_is_front_page()) {
  drupal_add_js(drupal_get_path('theme', 'pru8') . '/scripts/jquery.cycle.all.js');
}

function pru8_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    array_shift($breadcrumb);
	$breadcrumb[] = drupal_get_title();
    return '<div class="breadcrumb">' . implode(' » ', $breadcrumb) . '</div>';
  }
}

