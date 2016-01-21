<?php

/**
 * @file views_fixed_grid.views.inc
 * 
 * Include file to implement view's hooks.
 */

// Documentation can be found at: http://groups.drupal.org/node/10129

/**
* Implements of hook_views_plugins().
*/
function views_fixed_grid_views_plugins() {
	return array(
		'style' => array( //declare the views_fixed_grid style plugin
			'views_fixed_grid' => array(
				'title' => t('Fixed Grid'),
				'theme' => 'views_view_fixed_grid',
				'help' => t('Provides a plugin style creating a grid of Fixed size. Then the rows can be placed randomly in the grid.'),
				'handler' => 'views_plugin_style_fixed_grid',
				'uses row plugin' => TRUE,
				'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
				'type' => 'normal',
				'parent' => 'grid',
			),
		),
	);
}
