<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function simplyhired_listing_theme() {
	return array(
		'simplyhired_listings' => array(
			'variables' => array(
				'title' => NULL,
				'jobs' => NULL,
				'message' => NULL,
			),
			'template' => 'simplyhired-listings',
		),
		'simplyhired_job' => array(
			'variables' => array(
				'title' => NULL,
				'source' => NULL,
				'post_date' => NULL,
				'company' => NULL,
				'location' => NULL,
				'description' => NULL,
			),
			'template' => 'simplyhired-job',
		),
		'simplyhired_location' => array(
			'variables' => array(
				'city' => NULL,
				'province' => NULL,
				'postal_code' => NULL,
				'county' => NULL,
				'country' => NULL,
				'source' => NULL,
			),
			'template' => 'simplyhired-location',
		),
	);
}

/*
 * @todo: get template suggestions to work so users can have
 * different location templates per country so as to display
 * the proper address format.
 
function simplyhired_listing_preprocess_simplyhired_location(&$variables) {
	$variables['theme_hook_suggestions'][] = 'simplyhired-location';
	$variables['theme_hook_suggestions'][] = 'simplyhired-location-' . $variables['source'];
}
*/