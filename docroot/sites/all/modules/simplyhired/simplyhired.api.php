<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Allows the job search text to be altered prior to being sent to SimplyHired's API.
 * @param string $query
 */
function hook_simplyhired_search_query_alter($query) {
	$query = str_replace('bogus text', 'meaningful text', $query);
}

/**
 * Allows the SimplyHired API result set to be altered prior to returning it.
 * @param array $results
 */
function hook_simplyhired_search_results_alter($results) {
	// Add a new key for the timestamp the results 
	// were retrieved to the $results array.
	$results['timestamp'] = time();
	// return $results;
}