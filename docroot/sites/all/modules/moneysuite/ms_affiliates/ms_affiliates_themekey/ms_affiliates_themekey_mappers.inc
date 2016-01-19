<?php

/**
 * @file
 * Mapping functions for the Affiliates Suite Themekey module.
 */

/*
 * 
 * name: ms_affiliates_themekey_map_aid
 * 
 * ThemeKey mapping function to set a
 * ThemeKey property's value (destination)
 * with the aid of another ThemeKey property (source).
 *
 * src: system:dummy
 * dst: ms_affiliates:affiliate_id
 *
 * @param $data
 *   string containing current value of ThemeKey property user:aid
 *
 * @return
 *   user affiliate id from Moneysuite Affilates Suite for current user
 */
function ms_affiliates_themekey_map_aid($uid){
	// Cannot use ms_affiliates_get_affiliate() so soon in bootstrap
	// have to rely on session
	if (isset($_SESSION['affiliate']))
		return $_SESSION['affiliate'];
	return 0;
}

