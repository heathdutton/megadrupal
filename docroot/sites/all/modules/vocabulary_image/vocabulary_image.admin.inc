<?php

/**
 * @file
 *   Administrative settings for image validation.
 */

/**
 * Helper function for image validation admin configuration.
 */
function vocab_image_settings_form($form,&$form_state){
	// Initializes form with common settings.
	$form['vocabulary'] = array(
		'#type' => 'fieldset', 
		'#title' => t('Vocabulary image configuration'), 
		'#weight' => 0, 
		'#collapsible' => TRUE, 
		'#collapsed' => FALSE, 
	);

	$form['vocabulary']['vocab_image_extension'] = array(
		'#type' => 'textfield',
		'#title' => t('Allowed file extensions'),
		'#description' => 'Separate extensions with a space or comma and do not include the leading dot.',
		'#default_value' => variable_get('vocab_image_extension'),
	);
	
	$form['vocabulary']['vocab_max_image_resolution'] = array(
		'#type' => 'item',
		'#title' => t('Maximum image resolution'),
	);
	$form['vocabulary']['vocab_max_image_resolution_width'] = array(
		'#type' => 'textfield',
		//'#title' => t('Maximum image resolution'),
		'#size' => 5,
		'#maxlength' => 4,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => 'x',
		'#default_value' => variable_get('vocab_max_image_resolution_width'),
	);
	$form['vocabulary']['vocab_max_image_resolution_height'] = array(
		'#type' => 'textfield',
		'#description' => t('<br/>The maximum allowed image size expressed as WIDTHxHEIGHT (e.g. 640x480). Leave blank for no restriction. If a larger image is uploaded, it will be resized to reflect the given width and height. Resizing images on upload will cause the loss of EXIF data in the image.'),
		'#size' => 5,
		'#maxlength' => 4,
		'#suffix' => '</div><br/>',
		'#field_suffix' => 'pixels',
		'#default_value' => variable_get('vocab_max_image_resolution_height'),
	);
	
	$form['vocabulary']['vocab_min_image_resolution'] = array(
		'#type' => 'item',
		'#title' => t('Minimum image resolution'),
	);
	$form['vocabulary']['vocab_min_image_resolution_width'] = array(
		'#type' => 'textfield',
		//'#title' => t('Maximum image resolution'),
		'#size' => 5,
		'#maxlength' => 4,
		'#prefix' => '<div class="container-inline">',
		'#suffix' => 'x',
		'#default_value' => variable_get('vocab_min_image_resolution_width'),
	);
	$form['vocabulary']['vocab_min_image_resolution_height'] = array(
		'#type' => 'textfield',
		'#description' => t('<br/>The minimum allowed image size expressed as WIDTHxHEIGHT (e.g. 640x480). Leave blank for no restriction. If a smaller image is uploaded, it will be rejected.'),
		'#size' => 5,
		'#maxlength' => 4,
		'#suffix' => '</div>',
		'#field_suffix' => 'pixels',
		'#default_value' => variable_get('vocab_min_image_resolution_height'),
	);
	
	$form['vocabulary']['vocab_image_max_upload_size'] = array(
		'#type' => 'textfield',
		'#title' => t('Maximum upload size '),
		'#description' => 'Enter a value like "512" (bytes), "80 KB" (kilobytes) or "50 MB" (megabytes) in order to restrict the allowed file size. If left empty the file sizes will be limited only by PHP\'s maximum post and file upload sizes (current limit 2 MB).',
		'#default_value' => variable_get('vocab_image_max_upload_size'),
	);

	$form['#validate'][] = 'vocab_image_settings_page_validate';
	$form['#submit'][] = 'vocab_image_settings_page_submit';
	return system_settings_form($form);
}
/**
 * Helper function for admin settings form validation
*/
function vocab_image_settings_page_validate($form,&$form_state){
	if(!empty($form_state['values']['vocab_max_image_resolution_width']) && !(is_numeric($form_state['values']['vocab_max_image_resolution_width']))){
		form_set_error('vocab_max_image_resolution_width','Enter valid width for "Maximum image resolution".');
	}
	if(!empty($form_state['values']['vocab_max_image_resolution_height']) && !(is_numeric($form_state['values']['vocab_max_image_resolution_height']))){
		form_set_error('vocab_max_image_resolution_height','Enter valid height for "Maximum image resolution".');
	}
	
	if(!empty($form_state['values']['vocab_min_image_resolution_width']) && !(is_numeric($form_state['values']['vocab_min_image_resolution_width']))){
		form_set_error('vocab_min_image_resolution_width','Enter valid width for "Minimum image resolution".');
	}
	if(!empty($form_state['values']['vocab_max_image_resolution_height']) && !(is_numeric($form_state['values']['vocab_min_image_resolution_height']))){
		form_set_error('vocab_min_image_resolution_height','Enter valid height for "Minimum image resolution".');
	}
}
/**
 * Helper function for admin settings form submission
*/
function vocab_image_settings_page_submit($form,&$form_state){
	if($form_state['values']['vocab_default_image']){
		// Load the file via file.fid.
		$file = file_load($form_state['values']['vocab_default_image']);
		// Change status to permanent.
		$file->status = FILE_STATUS_PERMANENT;
		// Save.
		file_save($file);
		file_usage_add($file, 'vocabulary_image', 'vocabulary_image', $file->fid);
	}
}
