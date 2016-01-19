<?php
function admire_grunge_form_system_theme_settings_alter(&$form, &$form_state) {

	$form['admire_grunge_feed_url'] = array(
	'#type' => 'textfield',
	'#title' => t('URL to your rss feed.'),
	'#default_value' => theme_get_setting('admire_grunge_feed_url'),
 	'#description' => t('If you use external feed services like feedburner, please enter full URL with http://'),
	'#size' => 60,
	);

	return $form;
}