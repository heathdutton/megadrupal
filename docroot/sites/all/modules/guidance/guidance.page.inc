<?php


/**
 * Page callback when the user clicks on a guide
 *
 * Hook defined: hook_guide_load($url);
 *
 * @param int $nid
 * @param int $lid
 */
function guidance_link_redirect($nid, $lid) {
 	$link = guidance_get_guide_link($lid);
 	guidance_save_visit($link);
 	
 	cache_clear_all(NULL, 'cache_page');
	
  drupal_goto($link->url);
}

