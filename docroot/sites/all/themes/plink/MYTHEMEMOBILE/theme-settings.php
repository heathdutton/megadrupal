<?php
// Settings for the MYTHEMEMOBILE theme

/**
* Implementation of THEMENAME_form_system_theme_settings_alter() function.
*/
function MYTHEMEMOBILE_form_system_theme_settings_alter(&$form, &$form_state) {
 
 $form['meta_info'] = array();
 $form['apple_touch'] = array();
 $form['breadcrumbs'] = array();
 $form['master'] = array();
 $form['page_layout_options'] = array();
 $form['content_layouts_mq1'] = array();
 $form['content_layouts_mq2'] = array();
 $form['content_layouts_mq3'] = array();
 
 $form['warning']['#markup'] = '<p>&nbsp;</p><h2>Sorry, but theme settings are disabled for this theme.</h2><p>&nbsp;</p>';

}

function MYTHEMEMOBILE_theme_get_setting($var, $theme = NULL) {
	
}