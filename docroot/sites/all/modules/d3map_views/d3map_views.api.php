<?php

/**
 * @file
 * API documentation for D3 Map views.
 */

/**
 * Allows altering the path ID data while rendering a D3 Map views row before
 * the data is passed to the map.
 *
 * Changing the shape ID can be accomplished by replacing the text value
 * of $shape.
 * @param $result
 *   The current row data from the view.
 * @param $shape
 *   The ID of the shape path that will be passed to the map.
 */
function hook_d3map_views_alter_shape_data_alter($result, &$shape) {
  // If state path shape ID is 'VA' change it to something else
  if ($shape == 'VA') {
  	// Change it to the CA path ID
  	$shape = 'CA'; 
  }
}
