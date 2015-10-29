<?php // $Id$

function acloudyday_feed_icon(&$variables) {  
  $text = t('Subscribe to @feed-title', array('@feed-title' => $variables['title']));
  if ($image = theme('image', array('path' => path_to_theme() . '/images/rss.png', 'alt' => $text))) {
    return l($image, $variables['url'], array('html' => TRUE, 'attributes' => array('class' => array('feed-icon'), 'title' => $text)));
  }
}

function acloudyday_breadcrumb(&$variables) {	
  $breadcrumb = $variables['breadcrumb'];
  if (!empty($breadcrumb)) {
    $output = '<h2 class="element-invisible">' . t('You are here') . '</h2>';	
	  $output .= '<div class="breadcrumb">'. implode(" <img src=\"".base_path().path_to_theme()."/images/list-item.gif\" alt=\"\" /> ", $breadcrumb) .'</div>';
	  return $output;
	}
}

function acloudyday_process_page(&$variables)	{
	if (isset($variables['main_menu'])) {
    $pid = variable_get('menu_main_links_source', 'main-menu');
    $tree = menu_tree($pid);
    $variables['primary_nav'] = drupal_render($tree);
  } else {
    $variables['primary_nav'] = FALSE;
  }
}