<?php
// $Id: simple_contest.admin.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * @file
 * simple_contest settings form.
 */

/**
 * simple_contest settings form.
 */
function simple_contest_admin_settings(){
  //TODO: any global settings we have?
  $form = array();
  return $form;
}

function simple_contest_admin_settings_submit($form, $form_state) {

  //$values = $form_state['values'];

  drupal_set_message(t('Your changes have been saved.'));

}

/**
 * simple_contest admin_content_type_settings_page.
 */
function simple_contest_admin_content_type_settings_page($node_type){
  
  $type = $node_type;
 
  if (is_object($node_type)){
    $type = $node_type->type;
  }
 
  $form = drupal_get_form('simple_contest_content_type_settings_form', $type);
  
  return $form;
  
}

/**
 * simple_contest settings form.
 */
function simple_contest_content_type_settings_form($form, $form_state, $type) {
  
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type,
  );
  
  
  $settings = simple_contest_get_settings($type, TRUE);
  
  $form['#contest_settings'] = $settings;
  
  $is_sc = isset($form_state['values']['is_sc']) 
    ?  $form_state['values']['is_sc'] 
    :  simple_contest_is_simple_contest($type);
    

  $form['is_sc'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mark this content type as Simple contest.'),
    '#default_value' => $is_sc,
    '#description' => t('Select this checkbox to configure additional contest settings.'),
    '#options' => array(0 => 0, 1 => 1),
    '#ajax' => array(
      'callback' => 'simple_contest_content_type_settings_is_sc',
      'wrapper' => 'simple_contest_vertical_tabs_wrapper',
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );
  
  $form['sc'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div id="simple_contest_vertical_tabs_wrapper">',
  	'#suffix' => '</div>',
  );
  
  $form['sc']['additional_settings'] = array(
    '#type' => 'vertical_tabs',
    '#title' => t('Contest settings'),
  );
  
  $form['sc']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 10,
  );
  
  if (!$is_sc) {    
    return $form;
  }


  $form['sc']['simple_contest'] = array(
    '#type' => 'fieldset',
    '#title' => t('Simple contest'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
    '#group' => 'additional_settings',
  );

  $options = array('user' => 'User', 'node' => 'Node');

  $winner_type = isset($form_state['values']['winner_type']) 
    ?  $form_state['values']['winner_type'] 
    :  $settings['winner_type'];
  
  $form['sc']['simple_contest']['winner_type'] = array(
    '#type' => 'select',
    '#title' => t('Winner type'),
    '#default_value' => $winner_type,
    '#options' => $options,
    '#required' => TRUE,
    '#description' => t('Specify the winner type for this contest.
    	If node is selected, the winner selction view specified below must have the nid field in result. Otherwise it needs uid field.
    '),  
  );

  $form['sc']['simple_contest']['winner_selection_type'] = array(
    '#type' => 'select',
    '#title' => t('Winner selection type'),
    '#default_value' => $settings['winner_selection_type'],
    '#options' => array('auto' => t('Auto'), 'manual' => t('Manual'),'instant' => t('Instant'),),
    '#required' => TRUE,
    '#description' => t('Winners selection type. 
<br>Auto: specified view will be used to select the winner(s) automatically. It needs CRON. 
<br>Manual: Admin can select the winner(s).
<br>Instant: Winners can be selected instantly by calling the contest API.'),
  );

  $selection_type = isset($form_state['values']['selection_type']) 
    ?  $form_state['values']['selection_type'] 
    :  $settings['winner_selection_type'];
  
  
  
  
  $options_views = array('' => t('Select a View'));

  $views = views_get_all_views();

  foreach ($views as $name => $view) {
    $options_views[$name] = $name;
  }

  $view = isset($form_state['values']['view'])?$form_state['values']['view']:$settings['view'];
  
  $form['sc']['simple_contest']['view'] = array(
    '#type' => 'select',
    '#title' => t('Select view for winner selection'),
    '#required' => FALSE,
    '#default_value' => $view,
    '#options' => $options_views,
    '#description' => t('This view will be used to finalize the winners. leave empty if selection type is instant.'),  
    '#ajax' => array(
      'callback' => 'simple_contest_content_type_settings_view_fields',
      'wrapper' => 'simple_contest_view_field_wrapper',
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );
  
  $fields = array();
  if($view){ 
    $fields = _simple_contest_get_view_fields($view);
  }else{
    $fields = array('nid' => t('Node ID'));
  }
  $form['sc']['simple_contest']['view_field'] = array(
    '#type' => 'select',
    '#title' => t('View field that has Node ID or User ID value'),
    '#default_value' => !empty($fields) && isset($fields[$settings['view_field']])?$settings['view_field']:array(),
    '#options' => $fields,
    '#required' => TRUE,
    '#description' => t('Select this depends on the Contest type. If it is a node contest select Node ID field, User id field otherwise.'),
    '#prefix' => '<div id="simple_contest_view_field_wrapper">',
  	'#suffix' => '</div>',
  );

  $form['sc']['simple_contest']['results_display'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display a result view in contest node page?'),
    '#options' => array(0 => 0, 1 => 1),
    '#default_value' => $settings['results_display'],
    '#required' => FALSE,
  );
  $form['sc']['simple_contest']['results_display_view'] = array(
    '#type' => 'select',
    '#title' => t('Results display view'),
    '#default_value' => $settings['results_display_view'],
    '#options' => $options_views,
    '#required' => FALSE,
    '#description' => t('This view will be used to display the winners.'),
  );

  $form['sc']['simple_contest']['winners'] = array(
    '#type' => 'fieldset',
    '#title' => t('Winner Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => 0,
    '#group' => 'additional_settings',
  );
  
  $form['sc']['simple_contest']['winners']['max_winners'] = array(
    '#type' => 'select',
    '#title' => t('Maximum winners'),
    '#default_value' => $settings['max_winners'],
    '#options' => array(0 => t('unlimited'), 1=>1, 2=>2, 3=>3, 4=>4, 5 => 5, 10 =>10, 15=>15, 20=>20, 25=>25, 50=>50, 100=>100, 200=>200, 500=>500, 1000=>1000),
    '#description' => t('Maximum number of winners per contest.'),
    '#ajax' => array(
      'callback' => 'simple_contest_content_type_settings_max_winners',
      'wrapper' => 'simple_contest_vertical_tabs_wrapper',
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );
  
  
  $form['sc']['simple_contest']['winners']['winner_limit_interval'] = array(
    '#type' => 'select',
    '#title' => t('Winner limit interval'),
    '#default_value' => $settings['winner_limit_interval'],
    '#options' => array('hourly' => t('Hourly'), 'daily' => t('Daily'), 'weekly' => t('Weekly'), 'monthly' => t('Monthly'),),
    '#description' => t('TODO: Limit the winners. Useful only if winner selection mode is set to Instant.'),
  );
  
  $form['sc']['simple_contest']['winners']['winner_limit_count'] = array(
    '#type' => 'select',
    '#title' => t('Winner limit per interval'),
    '#default_value' => $settings['winner_limit_count'],
    '#options' => drupal_map_assoc(range(1, 50)),
    '#description' => t('TODO: Limit the winners per interval. Useful only if winner selection mode is set to Instant.'),
  );
  
  $form['sc']['simple_contest']['winners']['winner_select_hard'] = array(
    '#type' => 'select',
    '#title' => t('Winner selection hard range'),
    '#default_value' => $settings['winner_select_hard'],
    '#options' => drupal_map_assoc(range(1, 25)),
    '#description' => t('Hard mode for the draw. Useful only if winner selection mode is set to Instant.
<br>1  : easy, select always.
<br>25 : very hard.'),
  );
  
  
  $date_fields = _simple_contest_date_fields($type);
  
  $options =  array();
  foreach($date_fields as $field){
    $options[$field['field_name']] = $field['label'];
  }
  
  $date_field = isset($form_state['values']['date_field']) 
    ?  $form_state['values']['date_field'] 
    :  empty($settings['date_field'])?NULL:$settings['date_field'];
  
  $form['sc']['simple_contest']['date_field'] = array(
    '#type' => 'select',
    '#title' => t('Date field to be used for contest'),
    '#options' => $options,
    '#default_value' => $date_field,
    '#description' => t('Select a date field of this content type to use for contest Start and End dates. '),
    '#required' => TRUE,
  );

  $form['sc']['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('E-mail Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 1,
    '#group' => 'additional_settings',
  );
  $form['sc']['email']['email_to_winner'] = array(
  	'#type' => 'checkbox',
  	'#title' => t('E-mail to winner'),
  	'#default_value' => $settings['email_to_winner'],
  	'#options' => array(0 => 0, 1 => 1),
  	'#required' => FALSE,
  );
  $form['sc']['email']['email_to_winner_subject'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Winner e-mail subject'),
  	'#default_value' => $settings['email_to_winner_subject'],
  );
  $form['sc']['email']['email_to_winner_body'] = array(
  	'#type' => 'textarea',
  	'#title' => t('Winner e-mail body'),
  	'#default_value' => $settings['email_to_winner_body'],
  	'#rows' => 10,
  );

  $form['sc']['email']['email_to_participants'] = array(
    '#type' => 'checkbox',
    '#title' => t('E-mail to participants'),
    '#default_value' => $settings['email_to_participants'],
    '#options' => array(0 => 0, 1 => 1),
    '#required' => FALSE,
  );
  $form['sc']['email']['email_to_participants_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Participants e-mail subject'),
    '#default_value' => $settings['email_to_participants_subject'],
  );

  $form['sc']['email']['email_to_participants_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Participants e-mail body'),
    '#default_value' => $settings['email_to_participants_body'],
    '#rows' => 10,
  );

  $view = isset($form_state['values']['email_to_participants_view'])
  ?  $form_state['values']['email_to_participants_view']
  :  $settings['email_to_participants_view'];

  $form['sc']['email']['email_to_participants_view'] = array(
    '#type' => 'select',
    '#title' => t('Select a view that returns all participants details'),
    '#default_value' => $view,
    '#options' => $options_views,
    '#required' => FALSE,
    '#description' => t('This view will be used to get all participants details. This view must has nid (if node contest), Uid, User name, User E-mail.'),
  	'#ajax' => array(
      'callback' => 'simple_contest_content_type_settings_email_view_fields',
      'wrapper' => 'simple_contest_email_view_field_wrapper',
      'effect' => 'fade',
      'method' => 'replace',
    ),
  );

  $fields = array();

  if($view){
    $fields = _simple_contest_get_view_fields($view);
  }
  
  $form['sc']['email']['email_to_participants_view_field_uid'] = array(
		'#type' => 'select',
		'#title' => t('View field that has User ID value'),
		'#default_value' => (isset($settings['email_to_participants_view_field_uid']) && isset($fields[$settings['email_to_participants_view_field_uid']]))?$settings['email_to_participants_view_field_uid']:'',
		'#options' => $fields,
		'#required' => FALSE,
		'#description' => t('Select the User ID field from the view selected above.'),
    '#prefix' => '<div id="simple_contest_email_view_field_wrapper">',
  );
  if ($winner_type == 'node'){
    $form['sc']['email']['email_to_participants_view_field_nid'] = array(
  		'#type' => 'select',
  		'#title' => t('View field that has Node ID value'),
  		'#default_value' => isset($fields[$settings['email_to_participants_view_field_nid']])?$settings['email_to_participants_view_field_nid']:'',
  		'#options' => $fields,
  		'#required' => FALSE,
  		'#description' => t('Select the Node ID field from the view selected above. optional if contest type is User.'),
    );
  } 

  $form['sc']['email']['email_to_admin'] = array(
		'#type' => 'checkbox',
		'#title' => t('E-mail to admin'),
		'#default_value' => $settings['email_to_admin'],
		'#options' => array(0 => 0, 1 => 1),
		'#required' => FALSE,
    '#prefix' => '</div>'
  );
  $form['sc']['email']['email_to_admin_subject'] = array(
		'#type' => 'textfield',
		'#title' => t('admin e-mail subject'),
		'#default_value' => $settings['email_to_admin_subject'],
  );

  $form['sc']['email']['email_to_admin_body'] = array(
		'#type' => 'textarea',
		'#title' => t('Admin e-mail body'),
		'#default_value' => $settings['email_to_admin_body'],
		'#rows' => 10,
  );

  $form['sc']['email']['token_help'] = array(
    '#title' => t('Replacement patterns'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['sc']['email']['token_help']['help'] = array(
  	'#theme' => 'token_tree',
    '#token_types' => array(
      'contest-node', // => 'contest-node',
      'winner-user', // => 'winner-user',
      'winner-node', // => 'winner-node',
      'participant-user', // => 'participant-user',
    ),
  );
  
  if ($winner_type == 'node'){
    $form['sc']['email']['token_help']['help']['#token_types']['winner_node'] = 'node';
  }
  


  return $form;
}

function simple_contest_content_type_settings_form_validate($form, &$form_state) {
  //
  $values = $form_state['values'];
  
  if ( !$values['is_sc'] ) {
    return;
  }
  if( $values['email_to_participants'] && empty($values['email_to_participants_view'])) {
    form_set_error('email_to_participants_view', t('The view should be specified if E-mail to participants is enabled!'));
  }
  if( $values['results_display'] && empty($values['results_display_view'])) {
    form_set_error('results_display_view', t('The results display view should be specified if display a result view in contest node page is checked!'));
  }

}

function simple_contest_content_type_settings_form_submit($form, &$form_state) {

  $type = $form_state['values']['type'];
  $values = $form_state['values'];
  
  if ( $values['is_sc'] ) {
    
    $settings = simple_contest_get_settings($type, TRUE);  
    $settings['type'] = $type;

    foreach ($settings as $setting => $default) {
      if (isset($values[$setting])) {
        $settings[$setting] = $values[$setting];
      }
    }
    variable_set('simple_contest_' . $type, $settings);
    
  } else {
    variable_del('simple_contest_' . $type);
  }
  
  drupal_set_message('Your changes have been saved.');

}

function simple_contest_content_type_settings_view_fields($form, $form_state){
  return $form['sc']['simple_contest']['view_field'];
}

function simple_contest_content_type_settings_is_sc($form, $form_state){
  return $form['sc'];
}

function simple_contest_content_type_settings_max_winners($form, $form_state){
  return $form['sc'];
}

function simple_contest_content_type_settings_email_view_fields($form, $form_state) {
  $return = array();
  
  $return['email_to_participants_view_field_uid'] = $form['sc']['email']['email_to_participants_view_field_uid'];
  
  $winner_type = isset($form_state['values']['winner_type']) 
    ?  $form_state['values']['winner_type'] 
    :  $settings['winner_type'];
    
  if ($winner_type == 'node') {
    $return['email_to_participants_view_field_nid'] = $form['sc']['email']['email_to_participants_view_field_nid'];
  }
  
  return $return;
  
}

/**
 * Helper function _simple_contest_date_fields
 *
 * @param $content_type
 * @return
 */
function _simple_contest_date_fields($bundle) {
  $fields = array();
  
  $type = $bundle;
  if (is_object($type)){
    $type = $type->type;
  }
  if (is_array($type)){
    $type = $type['type'];
  }
  
  $all_fields = field_info_instances('node', $type);

  foreach ($all_fields as $field) {
    if ($field['widget']['module'] == 'date') {
      $fields[$field['field_name']] = $field;
    }
  }
  
  return $fields;
}


