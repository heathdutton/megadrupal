<?php

/**
 * @file
 * Theme functions for the Ajax Facets module.
 */

/**
 * Theme function to render the input field for ranges.
 */
function theme_ajax_facets_ranges_input($variables) {
	$output = !empty($variables['title']) ? '<label>' . $variables['title'] . '</label>' : '';
	$output .= '<input type="text" ' . drupal_attributes($variables['attributes']) . '" value="' . $variables['value'] . '" />';

	return "<p>$output</p>";
}
