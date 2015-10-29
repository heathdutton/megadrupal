<?php

function businesstime_preprocess_html(&$vars) {
	
	// Add body class for sidebar layout
  $vars['classes_array'][] = theme_get_setting('sidebar_layout'); 	

}

function businesstime_preprocess_page(&$vars) {
	
  if (isset($vars['page']['content']['system_main']['nodes'])) {
    if ($nids = element_children($vars['page']['content']['system_main']['nodes'])) {
      $first_nid = reset($nids);
      $last_nid = end($nids);
      $first_node = $vars['page']['content']['system_main']['nodes'][$first_nid]['#node'];
      $first_node->classes_array = array('first');
      $last_node = $vars['page']['content']['system_main']['nodes'][$last_nid]['#node'];
      $last_node->classes_array = array('last');
    }
  }	
	
	$grid_info = get_grid_info();
	
	// Create page variables
	
	$vars['grid_size'] = 'container_' . $grid_info['grid_size'];
	$vars['grid_full_width'] = 'grid_' . $grid_info['grid_size'];
	$vars['sidebar_first_grid_width'] = 'grid_' . $grid_info['sidebar_first_width'];
    $vars['sidebar_second_grid_width'] = 'grid_' . $grid_info['sidebar_second_width'];
	
	for ($region_count = 1; $region_count <= 4; $region_count++) {
	  $vars['preface_' . $region_count . '_grid_width'] = 'grid_' . $grid_info['preface_' . $region_count . '_grid_width'];
		$vars['postscript_' . $region_count . '_grid_width'] = 'grid_' . $grid_info['postscript_' . $region_count . '_grid_width'];
	}	

	if (empty($vars['page']['sidebar_first']) && empty($vars['page']['sidebar_second'])) {
		$vars['main_content_grid_width'] = 'grid_' . $grid_info['grid_size'];
	} else if (!empty($vars['page']['sidebar_first']) && !empty($vars['page']['sidebar_second'])) {
		$vars['main_content_grid_width'] = 'grid_' . ($grid_info['grid_size'] - ($grid_info['sidebar_first_width'] + $grid_info['sidebar_second_width']));
 	} else if (empty($vars['page']['sidebar_first']) && !empty($vars['page']['sidebar_second'])) {
		$vars['main_content_grid_width'] = 'grid_' . ($grid_info['grid_size'] - $grid_info['sidebar_second_width']);
	} else if (!empty($vars['page']['sidebar_first']) && empty($vars['page']['sidebar_second'])) {
		$vars['main_content_grid_width'] = 'grid_' . ($grid_info['grid_size'] - $grid_info['sidebar_first_width']);
	}
	
}

function get_grid_info() {

	$grid_info = array();

	$grid_info['grid_size'] = theme_get_setting('grid_size');
	$grid_info['sidebar_first_width'] = theme_get_setting('sidebar_first_width');
	$grid_info['sidebar_second_width'] = theme_get_setting('sidebar_second_width');
	
	for ($region_count = 1; $region_count <= 4; $region_count++) {
	  $grid_info['preface_' . $region_count . '_grid_width'] = theme_get_setting('preface_' . $region_count . '_grid_width');
		$grid_info['postscript_' . $region_count . '_grid_width'] = theme_get_setting('postscript_' . $region_count . '_grid_width');
	}
	
	return $grid_info;

}

function businesstime_preprocess_node(&$vars) {
  $node = $vars['node'];
  if (!empty($node->classes_array)) {
    $vars['classes_array'] = array_merge($vars['classes_array'], $node->classes_array);
  }
}

/* Breadcrumbs */

function businesstime_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    // Adding the title of the current page to the breadcrumb.
    $breadcrumb[] = drupal_get_title();
   
    // Provide a navigational heading to give context for breadcrumb links to
    // screen-reader users. Make the heading invisible with .element-invisible.
    $output = '<span class="you-are-here">' . t('You are here: ') . '</span>';

    $output .= '<div class="breadcrumb"><div class="breadcrumb-inner">' . implode(' / ', $breadcrumb) . '</div></div>';
    return $output;
  }
}


