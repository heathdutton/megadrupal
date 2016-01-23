<?php

/**
 * @file
 * Provide administration options.
 */

function textcleaner_admin_settings_form() {
	$fields = field_info_field_types();
	
	$field_options = array();
	
	foreach ($fields as $field_type => $field) {
		$field_options[$field_type] = $field['label'];
	}
	
	$saved_values = variable_get('textcleaner_fields', '');
	
	$form['textcleaner_fields'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Fields'),
		'#description' => t('Select which field types TextCleaner should run on. TextCleaner works best on Fields that are used as content fields generally formatted by a WYSIWYG.'),
		'#options' => $field_options,
		'#default_value' => drupal_map_assoc($saved_values),
	);
	
	return system_settings_form($form);
}