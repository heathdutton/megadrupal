<?php
// $Id: seotools.admin.inc,v 1.1.2.3 2011/01/05 22:29:29 tomdude48 Exp $
/**
 * @file * Admin include file. 
*/

/** 
 * Displays the form for the standard settings tab.
 * 
 * @return array 
 * A structured array for use with Forms API. 
*/ 

function seo_admin_settings() {
	$form['msg'] = array(
	  '#markup' => t('Settings not implemented yet. Coming soon.'),
	);
	return $form;
}