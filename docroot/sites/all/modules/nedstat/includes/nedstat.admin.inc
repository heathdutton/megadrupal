<?php


/**
 * Returns a form for default nedstat
 * configuration settings
 *
 * @return array
 */
function nedstat_admin_settings_form() {
  $form['#attached']['js'] = array(
    'data' => drupal_get_path('module', 'nedstat') . '/js/nedstat.js',
  );

 $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['default']['nedstat_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base url'),
    '#description' => t('The Nedstat base url.<br/> (<em>for example: <strong>countrycode</strong>.sitestat.com</em>)'),
    '#default_value' => variable_get('nedstat_base_url', ''),
    '#required' => TRUE,
  );

  $form['default']['nedstat_country_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Country Code'),
    '#description' => t('The Nedstat Country Code.<br/> (<em>for example: <strong>uk</strong></em>)'),
    '#default_value' => variable_get('nedstat_country_code', ''),
    '#required' => TRUE,
  );

  $form['default']['nedstat_client_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Name'),
    '#description' => t('The Nedstat Client Name, given by nedstat.<br/> (<em>for example: <strong>comscore</strong></em>)'),
    '#default_value' => variable_get('nedstat_client_name', ''),
    '#required' => TRUE,
  );

  $form['default']['nedstat_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Name'),
    '#description' => t('The Nedstat Site Name.<br/> (<em>for example: <strong>comscore-uk</strong></em>)'),
    '#default_value' => variable_get('nedstat_site_name', ''),
    '#required' => TRUE,
  );

  $form['countername'] = array(
    '#type' => 'fieldset',
    '#title' => t('Countername settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['countername']['nedstat_general_countername_generate_type'] = array(
    '#type' => 'radios',
    '#title' => t('Countername generate type:'),
    '#options' => array(
      'path' => t('Based on path url'),
      'node_type_tokens' => t('Based on nodetype tokens'),
    ),
    '#default_value' => variable_get('nedstat_general_countername_generate_type', 'path'),
  );

  $form['countername']['node_type_tokens'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#prefix' => '<div id="nedstat-countername-node-type-settings">',
    '#suffix' => '</div>',
  );

  $form['countername']['node_type_tokens']['nedstat_node_types']['nedstat_general_node_type_settings'] = array(
    '#type' => 'textfield',
    '#title' => t('General countername settings'),
    '#description' => t('Assign a general countername for all content-types.  You can use tokens to make this more dynamic.'),
    '#default_value' => variable_get('nedstat_general_node_type_settings', ''),
    '#required' => FALSE,
  );
  
  $node_types = node_type_get_names();
  foreach ((array)$node_types as $key => $node_type) {
    $form['countername']['node_type_tokens']['nedstat_node_types']['nedstat_node_type_' . $key . '_settings'] = array(
      '#type' => 'textfield',
      '#title' => t('Countername settings !name', array('!name' => $node_type)),
      '#description' => t('Assign a countername to contenttype "<em>!content-type</em>".  You can use tokens.  If no countername is set fro this content-type, the general countername will be used.', array('!content-type' => $node_type)),
      '#default_value' => variable_get('nedstat_node_type_' . $key . '_settings', ''),
      '#required' => FALSE,
    );
  }
  
  $form['countername']['node_type_tokens']['tokens'] = array(
    '#theme' => 'token_tree',
    '#token_types' => array('node'),
  );
  
  $form['special_pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Special pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['special_pages']['nedstat_frontpage_countername'] = array(
    '#type' => 'textfield',
    '#title' => t('Frontpage'),
    '#description' => t('Assign a countername for the frontpage.'),
    '#default_value' => variable_get('nedstat_frontpage_countername', ''),
    '#required' => FALSE,
  );
  
  $form['special_pages']['nedstat_error_403_settings'] = array(
    '#type' => 'textfield',
    '#title' => t('403 error'),
    '#description' => t('Assign a countername for all 403 pages.'),
    '#default_value' => variable_get('nedstat_error_403_settings', ''),
    '#required' => FALSE,
  );
  
  $form['special_pages']['nedstat_error_404_settings'] = array(
    '#type' => 'textfield',
    '#title' => t('404 error'),
    '#description' => t('Assign a countername for all 404 pages.'),
    '#default_value' => variable_get('nedstat_error_404_settings', ''),
    '#required' => FALSE,
  );

  $form['prefix_suffix'] = array(
    '#type' => 'fieldset',
    '#title' => t('Prefix & suffix'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['prefix_suffix']['nedstat_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#description' => t('The prefix of each counter.<br/> (<em>for example: <strong>testcounter</strong></em>)'),
    '#default_value' => variable_get('nedstat_prefix', ''),
    '#required' => FALSE,
  );

  $form['prefix_suffix']['nedstat_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#description' => t('The suffix of each counter.<br/> (<em>for example: <strong>page</strong></em>)'),
    '#default_value' => variable_get('nedstat_suffix', ''),
    '#required' => FALSE,
  );


  $form['measurements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Measurement types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

 	$form['measurements']['normal']['nedstat_measurement_type_normal'] = array(
	  '#type' => 'checkbox',
	  '#title' => t('Normal'),
	  '#description' => t('Select this measurement type for basic tracking'), 
	  '#default_value' => variable_get('nedstat_measurement_type_normal', 1),
	  '#required' => FALSE,
	);
	
	$form['measurements']['technical'] = array(
    '#type' => 'fieldset',
    '#title' => '',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
	
	$form['measurements']['technical']['nedstat_measurement_type_technical'] = array(
    '#type' => 'checkbox',
    '#title' => t('Technical'),
    '#description' => t('Select this measurement type for technical tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_technical', 1),
    '#required' => FALSE,
  );
  
  $form['measurements']['technical']['nedstat_measurement_type_technical_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Technical paths'),
    '#description' => t('Add all pages that should have technical tracking. Specify pages by using their paths. Each line represent a exclude. An * is permitted.'), 
    '#default_value' => variable_get('nedstat_measurement_type_technical_paths', ''),
    '#required' => FALSE,
  );
  
  $form['measurements']['loading']['nedstat_measurement_type_loading'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loading time'),
    '#description' => t('Select this measurement type for loading time tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_loading', 0),
    '#required' => FALSE,
  );
  
  $form['measurements']['click_in']['nedstat_measurement_type_click_in'] = array(
    '#type' => 'checkbox',
    '#title' => t('Click in'),
    '#description' => t('Select this measurement type for click in tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_click_in', 0),
    '#required' => FALSE,
  );
  
  $form['measurements']['click_out']['nedstat_measurement_type_click_out'] = array(
    '#type' => 'checkbox',
    '#title' => t('Click out'),
    '#description' => t('Select this measurement type for click out tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_click_out', 0),
    '#required' => FALSE,
  );
  
  $form['measurements']['click_out']['nedstat_measurement_type_pdf'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pdf documents'),
    '#description' => t('Select this measurement type for Pdf documents tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_pdf', 0),
    '#required' => FALSE,
  );
  
  $form['measurements']['click_out']['nedstat_measurement_type_campaigns'] = array(
    '#type' => 'checkbox',
    '#title' => t('Campaigns'),
    '#description' => t('Select this measurement type for campaign tracking'), 
    '#default_value' => variable_get('nedstat_measurement_type_campaigns', 0),
    '#required' => FALSE,
  );
  

  return system_settings_form($form);
}