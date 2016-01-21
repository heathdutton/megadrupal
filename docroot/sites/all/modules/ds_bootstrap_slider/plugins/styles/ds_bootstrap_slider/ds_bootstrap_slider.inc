<?php 

// Plugin definition
$plugin = array(
		'title' => t('Bootstrap carousel'),
		'description' => t('Presents the panes in a bootstrap carousel'),
		'render region' => 'ds_bootstrap_slider_style_render_region',
		'settings form' => 'ds_bootstrap_slider_style_settings_form',
);

function theme_ds_bootstrap_slider_style_render_region($vars) {
	$id = drupal_html_id($vars['region_id'] .'-ds-carousel') ;
	$vars['settings']['id'] = $id ;
	
	return theme('ds_bootstrap_slider',array('items' => $vars['panes'],'settings' => $vars['settings'],'pane' ));
}

function ds_bootstrap_slider_style_settings_form($settings) {
	$form['interval'] = array(
		'#type'           => 'textfield',                           // Use a select box widget
		'#title'          => t('Interval'),                   // Widget label
		'#description'    => t('The amount of time to delay between automatically cycling an item. If false, carousel will not automatically cycle.'), // Helper text
		'#default_value'  => (!$settings['interval']) ? 5000 : $settings['interval'],              // Get the value if it's already been set
	);
	$form['pause'] = array(
		'#type'           => 'textfield',                        // Use a textbox
		'#title'          => t('Pause'),                      // Widget label
		'#description'    => t('Pauses the cycling of the carousel on mouseenter and resumes the cycling of the carousel on mouseleave.'),  // helper text
		'#default_value'  => (!$settings['pause']) ? "hover" : $settings['pause'],               // Get the value if it's already been set
	);
	$form['wrap'] = array(
		'#type'           => 'checkbox',                        // Use a textbox
		'#title'          => t('Wrap'),                      // Widget label
		'#description'    => t('Whether the carousel should cycle continuously or have hard stops.'),  // helper text
		'#return_value'   => 1,
		'#default_value'  => (!$settings['wrap']) ? 1 : $settings['wrap'],               // Get the value if it's already been set
	);
	$form['start'] = array(
		'#type'           => 'textfield',                        // Use a textbox
		'#title'          => t('Start item'),                      // Widget label
		'#description'    => t('Whether the carousel should cycle continuously or have hard stops.'),  // helper text
		'#return_value'   => 1,
		'#default_value'  => (!$settings['start']) ? 1 : $settings['start'],               // Get the value if it's already been set
	);
	return $form;
}