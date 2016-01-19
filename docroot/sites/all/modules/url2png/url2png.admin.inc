<?php



function url2png_admin_settings() {
	
  $form['url2png_api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'API Key ',
    '#description' => 'Get key from http://url2png.com/dashboard/',
    '#default_value' => variable_get('url2png_api_key', ''),
  );
  
  $form['url2png_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => 'SECRET Key',
    '#default_value' => variable_get('url2png_secret_key', ''),
  );
  
  $form['url2png_viewport'] = array(
    '#type' => 'textfield',
    '#title' => 'Viewport',
    '#description' => 'The viewport for the browser. Max is 5000x5000',
    '#default_value' => variable_get('url2png_viewport', '1280x1024'),
  );
  
  $form['url2png_fullpage'] = array(
    '#type' => 'checkbox',
    '#title' => 'Fullpage screenshot. ',
    '#description' => 'Capture the entire page, even what is outside of the viewport.',
    '#default_value' => variable_get('url2png_fullpage', 0),
  );
  
  
  $form['url2png_thumbnail_max_width'] = array(
    '#type' => 'textfield',
    '#title' => 'Thumbnail max width',
    '#description' => 'Maximum with of image returned. If not specified image return will be 1:1.',
    '#default_value' => variable_get('url2png_thumbnail_max_width', ''),
  );
  
  
  $form['url2png_number_cron_items'] = array(
    '#type' => 'textfield',
    '#title' => 'Number of node each time cron run',
    '#default_value' => variable_get('url2png_number_cron_items', 10),
  );
  
  
  $names = node_get_types('names');
  
  $form['url2png_content_type'] = array(
    '#type' => 'select',
    '#title' => 'Fields on Content type',
    '#options' => $names,
    '#multiple' => TRUE,
    '#description' => 'Choose content type field_url and field_image on.',
    '#default_value' => variable_get('url2png_content_type', array()),
  );

  $form['buttons']['cron'] = array(
    '#type' => 'submit',
    '#value' => 'Reset all sreenshot',
  );
  $form['#submit'][] = 'url2png_admin_settings_submit';
  return system_settings_form($form);
}

function url2png_admin_settings_submit($form, &$form_state) {
	$op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

	if($op == 'Reset all sreenshot') {
		variable_set('url2png_cron_start', 'start');
		variable_set('url2png_cron_nid_progress', 0);
	}
}