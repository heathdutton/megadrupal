<?php

$path_to_trotoar = drupal_get_path('theme', 'trotoar');
  
drupal_add_css(
      '.section {clear : both;} ',
      array( 'group' => CSS_THEME, 'type' => 'inline',)
);
if(theme_get_setting('hover') == 1){
drupal_add_css(
      ' .hover:hover { background : #a5cdff; } ',
      array( 'group' => CSS_THEME, 'type' => 'inline',)
);
}

drupal_add_css(
		$path_to_trotoar . '/styles/normalize.css',
	array('group' => CSS_THEME, 'weight' => 1,'preprocess' => FALSE)
);

drupal_add_css(
		$path_to_trotoar . '/styles/tabs.css',
	array('group' => CSS_THEME, 'weight' => 1,'preprocess' => FALSE)
);


if(theme_get_setting('layout_type') == 'grid'):
drupal_add_css(
		$path_to_trotoar . '/styles/grid/960.css',
	array('group' => CSS_THEME, 'weight' => 1,'preprocess' => FALSE)
);
else:
drupal_add_css(
		$path_to_trotoar . '/styles/grid/fluid.css',
	array('group' => CSS_THEME, 'weight' => 1,'preprocess' => FALSE)
);
endif;

if(theme_get_setting('container_grid_show_png') == 1){
drupal_add_css(
		$path_to_trotoar . '/styles/grid/images/background-grid.css',
	array('group' => CSS_THEME, 'weight' => 1,'preprocess' => FALSE)
);
}
