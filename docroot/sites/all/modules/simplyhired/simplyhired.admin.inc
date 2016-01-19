<?php
/**
 * @file
 * Administration page callbacks for the jobamatic module.
 */

/**
 * Admin form callback.
 */
function simplyhired_admin_settings() {
  $form = array();

  $form['simplyhired_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Account Settings'),
    '#tree' => FALSE,
  );

  $pshid_msg = t('You can obtain this information by logging in to the
<a href="@portal_url" target="_blank">SimplyHired Partner portal</a>
then clicking on the XML API tab.',
  array(
    '@portal_url' => url('https://simply-partner.com/partner/', array('external' => TRUE)),
  ));

  $form['simplyhired_account']['simplyhired_pshid'] = array(
    '#type' => 'textfield',
    '#title' => t('Publisher ID'),
    '#default_value' => variable_get('simplyhired_pshid', ''),
    '#description' => $pshid_msg,
		'#required' => TRUE,
  );

  $form['simplyhired_account']['simplyhired_auth'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('simplyhired_auth', ''),
    '#description' => t('Your SimplyHired Partner API key is listed under the API tab on your portal page.'),
		'#required' => TRUE,
  );
	
	$form['simplyhired_global'] = array(
    '#title' => t('Global Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );
	
	$form['simplyhired_global']['simplyhired_source'] = array(
		'#type' => 'select',
		'#title' => 'API source',
		'#default_value' => variable_get('simplyhired_source', 'us'),
		'#options' => _simplyhired_get_endpoints(),
		'#description' => t('SimplyHired provides API\'s for several countries around the world. Please select the default API endpoint to use.')
	);
	
	$form['simplyhired_global']['simplyhired_logo'] = array(
		'#type' => 'radios',
		'#title' => t('Attribution image'),
		'#options' => drupal_map_assoc(array('default', 'light')),
		'#default_value' => variable_get('simplyhired_logo', 'default'),
		'#description' => t('The SimplyHired terms of service requires that an attribution be displayed on any page that displays SimplyHired data. Select the image you wish to use for this attribution.')
	);
	
	$image_path = drupal_get_path('module', 'simplyhired') . '/images/';
	
	$sample = <<<SAMPLE
<div style="font-size: 10px;"><strong>Sample Attribution Images</strong></div>
<div style="float:left;width:160px;text-align:center;margin-left:8px;padding:8px;font-size:10px;">
  <img src="/{$image_path}sh-jobsby-normal.png" height="20" width="150" alt="" /><br />
	<em>Default image</em>
</div>
<div style="float:left;width:160px;text-align:center;padding:8px;font-size:10px;background:#000000;color:#CCCCCC;">
  <img src="/{$image_path}sh-jobsby-light.png" height="20" width="150" alt="" /><br />
	<em>Light image</em>
</div>
<div class="clearfix"></div>
SAMPLE;
	
	$form['simplyhired_global']['logo_sample'] = array(
		'#markup' => $sample,
	);
  /*
  $form['simplyhired_defaults'] = array(
    '#title' => t('Default Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );
	
	$form['simplyhired_defaults']['info'] = array(
		'#markup' => '<p>' . t('These are the default settings that will be used when creating new <em>Job Search</em> nodes.') . '</p>',
	);
	
	$defaults = variable_get('simplyhired_defaults', '');

  if (!is_array($defaults)) {
    $defaults = _simplyhired_get_defaults();
  }

  $form['simplyhired_defaults']['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Default search criteria'),
    '#default_value' => $defaults['query'],
    '#maxlength' => 500,
		'#description' => t('')
  );

  $form['simplyhired_defaults']['location'] = array(
    '#type' => 'textfield',
    '#title' => t('Location'),
    '#description' => t('City/State to confine the search to. Can be either city/state or zip code. SimplyHired API currently does not support multiple locations.'),
    '#default_value' => $defaults['location'],
  );

  $options = array(
    '5' => '5 miles',
    '10' => '10 miles',
    '25' => '25 miles',
    '50' => '50 miles',
    '100' => '100 miles',
  );

  $form['simplyhired_defaults']['miles'] = array(
    '#type' => 'select',
    '#title' => t('Search radius in miles'),
    '#options' => $options,
    '#default_value' => $defaults['miles'],
  );

  $form['simplyhired_defaults']['sort_by'] = array(
    '#type' => 'select',
    '#title' => t('Sort By'),
    '#description' => '',
    '#default_value' => $defaults['sort_by'],
    '#options' => _simplyhired_get_sort_options(),
  );

  $form['simplyhired_defaults']['window_size'] = array(
    '#type' => 'select',
    '#title' => t('Jobs per page'),
    '#default_value' => $defaults['window_size'],
    '#options' => drupal_map_assoc(array(10, 20, 50, 75, 100)),
  );
	*/
	

  $form['#validate'][] = 'simplyhired_admin_settings_validate';

  $form['#submit'][] = 'simplyhired_admin_settings_submit';

  return system_settings_form($form);
}

/**
 * Settings form validation.
 */
function simplyhired_admin_settings_validate(&$form, &$form_state) {
  $pshid = trim($form_state['values']['simplyhired_pshid']);
  $pshid_exp = '/^[0-9]{3,}$/';
  if ($pshid != '' && preg_match($pshid_exp, $pshid) !== 1) {
    form_set_error('simplyhired_pshid', t('Publisher ID must contain only numbers and must be at least three characters in length.'));
  }
}

/**
 * Callback function that processes the admin settings for submissions.
 */
function simplyhired_admin_settings_submit($form, &$form_state) {
  variable_set('simplyhired_pshid', trim($form_state['values']['simplyhired_pshid']));
  variable_set('simplyhired_auth', $form_state['values']['simplyhired_auth']);
	variable_set('simplyhired_source', $form_state['values']['simplyhired_source']);
	variable_set('simplyhired_logo', $form_state['values']['simplyhired_logo']);
  /*
  $defaults = _simplyhired_get_defaults();
  foreach ($defaults as $key => $value) {
    $defaults[$key] = $form_state['values'][$key];
  }

  variable_set('simplyhired_defaults', $defaults);
  */
}
