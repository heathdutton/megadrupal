<?php
/**
 * @file
 * Admin configuration management
 */

/** 
 * Displays the form for the standard settings tab.
 * 
 * @return array 
 * A structured array for use with Forms API. 
*/ 
function intel_admin_settings($form, $form_state) {
  global $base_url;
  $apiUrl = variable_get('intel_l10iapi_url', '');
  $api_params = variable_get('intel_l10iapi_custom_params', array());
  $apiClientProps = array(
    'apiUrl' => ($apiUrl ? $apiUrl : INTEL_L10IAPI_URL) . '/',
    'apiConnector' => variable_get('intel_l10iapi_connector', ''),
    'apiParams' => $api_params,
  );
  $api_connection = 0;
  $account_level = '';

  $message = '';
  if (count($form_state['input']) == 0) { // hack to assure check and messages are only done once when form is submitted
    $account_level = intel_verify_apikey($message);
    if ($account_level) {
      drupal_set_message(t('LevelTen Intelligence API connected.'));
    }
    else {
      $msg = t('LevelTen Intelligence API not connected.');
      $msg .= ' ' . $message;
      $msg .= ' ' . l(t('View setup instructions tutorial.'), 'http://getlevelten.com/blog/tom-mccracken/intelligence-tutorial-install', array('attributes' =>  array('target' => 'getlevelten')));
      drupal_set_message($msg, 'error');
    }
  }
  
  $form['l10iapi'] = array(
    '#type' => 'fieldset',
    '#title' => t('LevelTen Intel API'),
    '#collapsible' => TRUE,
  );
  $desc = t('Enter the Google Analytics property tracking id you want to integrate with LevelTen Intel.');
  $desc .= ' ' . t('This should be a seperate property than the primary tracking id set in the Google Analytics module.');
  $form['l10iapi']['intel_ga_account'] = array(
    '#type' => 'textfield',
    '#title' => t('Tracking id'),
    '#default_value' => variable_get('intel_ga_account', ''),
    '#description' => $desc,
    '#size' => 18,
  );
  $desc = t('Input your LevelTen Intelligence API key. You can get one at !link',
    array(
      '!link' => l(t('api.getlevelten.com'), 'http://api.getlevelten.com/site', array('attributes' => array('target' => '_blank'))),
    )
  );
  $form['l10iapi']['intel_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#default_value' => variable_get('intel_apikey', ''),
    '#description' => $desc,
    '#size' => 32,    
  ); 
  if ($account_level) {
    $form['l10iapi']['intel_api_level'] = array(
      '#type' => 'item',
      '#title' => t('Account level'),
      '#markup' => strtoupper($account_level),
    );     
  }

  $form['l10iapi']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['l10iapi']['advanced']['intel_l10iapi_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom API URL'),
    '#field_prefix' => 'http(s)://',
    '#default_value' => variable_get('intel_l10iapi_url', ''),
    '#description' => t('The URL for the API without the protocall. Leave blank to use the default of api.getlevelten.com/api/intel.'),
    '#field_suffix' => '/',
  ); 
  $form['l10iapi']['advanced']['intel_l10iapi_connector'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom API connector'),
    '#default_value' => variable_get('intel_l10iapi_connector', ''),
    '#description' => t('For local API configuration, enter the server path to the API.'),
  );
  $form['l10iapi']['advanced']['intel_l10iapi_custom_params'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom API custom params'),
    '#default_value' => http_build_query($api_params),
    '#description' => t('Enter custom params as query formated string. (e.g. key1=value1&key2=value2)'),
  );
  $form['l10iapi']['advanced']['intel_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#default_value' => variable_get('intel_debug_mode', 0),
    '#description' => t('Switches to using non minified js, turns on js console logging.'),
  );
  $form['l10iapi']['advanced']['intel_debug_ga_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use ga_debug.js'),
    '#default_value' => variable_get('intel_debug_ga_debug', 0),
    '#description' => t('Check to replaces Google Analytics ga.js embed with ga_debug.js.'),
  );
  $form['l10iapi']['advanced']['intel_extended_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Extended mode'),
    '#default_value' => variable_get('intel_extended_mode', 0),
    '#description' => t('Turns on extended experimental features that are still in development.'),
  );
  $form['l10iapi']['advanced']['intel_sync_visitordata_fullcontact'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sync Full Contact data'),
    '#default_value' => variable_get('intel_sync_visitordata_fullcontact', INTEL_SYNC_VISITORDATA_FULLCONTACT_DEFAULT),
    '#description' => t('Check to enable syncing data via FullContact API. Only available for pro accounts.'),
  );
  $form['l10iapi']['advanced']['intel_l10iapi_js_ver'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom l10i.js version'),
    '#default_value' => variable_get('intel_l10iapi_js_ver', ''),
    '#description' => t('Leave blank to use the default.'),
    '#size' => 12,
  ); 
  $form['l10iapi']['advanced']['intel_js_monitor_script'] = array(
    '#type' => 'textarea',
    '#title' => t('JavaScript monitoring script'),
    '#default_value' => variable_get('intel_js_monitor_script', ''),
    '#description' => t('Use to place any JavaScript monitoring code snippet at the top of the header section.'), // . t('Do not include &lt;script&gt; tags.'),
    '#rows' => 4,
  );
  $form['l10iapi']['advanced']['intel_custom_embed_script'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom embed JavaScript'),
    '#default_value' => variable_get('intel_custom_embed_script', ''),
    '#description' => t('Use to place any JavaScript monitoring code snippet at the top of the header section.'), // . t('Do not include &lt;script&gt; tags.'),
    '#rows' => 4,
  );
  
  $form['tracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tracking settings'),
    '#collapsible' => TRUE,
  ); 
  $desc = t('If you are tracking multiple subdomains, set the main domain name for this property.');
  $form['tracking']['intel_domain_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain name'),
    '#default_value' => variable_get('intel_domain_name', ''),
    '#description' => $desc,
  ); 
   
  $form['extensions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extensions'),
    '#collapsible' => TRUE,
  ); 
  $intel_scripts = intel_intel_scripts();
  $options = array();
  $defaults = array();
  foreach ($intel_scripts AS $k => $v) {
    if (!empty($v['selectable'])) {
      $options[$k] = $v['title'] . ' - ' . $v['description'];
      $defaults[$k] = !empty($v['enabled']) ? $k : '';
    }
  }
  $enabled = variable_get('intel_intel_scripts_enabled', array());
  $enabled += $defaults;
  $form['extensions']['intel_intel_scripts_enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Intel scripts'),
    '#options' => $options,
    '#default_value' => $enabled,
    '#description' => t('Select any intel integration scripts you want to include on your site.'),
  );
  
  $form['reports'] = array(
    '#type' => 'fieldset',
    '#title' => t('Report settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  ); 
  $form['reports']['intel_visitor_default_image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default visitor picture'),
    '#default_value' => variable_get('intel_visitor_default_image_path', ''),
    '#description' => t('URL of picture to display for visitors with no custom picture. Leave blank for none.'),
  );
  return system_settings_form($form);
}

function intel_admin_settings_validate($form, &$form_state) {
  
  // convert string to array
  parse_str($form_state['values']['intel_l10iapi_custom_params'], $params);
  $form_state['values']['intel_l10iapi_custom_params'] = $params;

  /*
  if (trim(strpos($form_state['values']['intel_l10iapi_custom_params']), '<script') === 0) {
    $msg = t('It appears you have wrapped the JavaScript monitoring script with &lt;script&gt; tags.');
    $msg .= t('Please remove the &lt;script&gt; tags.');
    form_set_error('intel_js_monitor_script', $msg); 
    
  }
  */
}

function intel_admin_scoring($form, &$form_state) {
  require_once './' . drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
  
  $form['targets'] = array(
    '#type' => 'fieldset',
    '#title' => t('Targets'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ); 
  $form['targets']['site_kpi_month'] = array(
    '#type' => 'fieldset',
    '#title' => t('Monthly KPIs'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['targets']['site_day'] = array(
    '#type' => 'fieldset',
    '#title' => t('Daily site objectives'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['targets']['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page objectives'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['targets']['visit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visit objectives'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $metas = intel_init_targets();
  $targets = variable_get('intel_targets', array());  
  foreach ($metas AS $i => $m) {
    $value = !empty($targets [$i]) ? $targets [$i] : $m['value'];
    $form['targets'][$m['group']]['target_' . $i] = array(
      '#type' => 'textfield',
      '#title' => $m['title'],
      '#default_value' => $value,
      '#description' => $m['description'],
      '#size' => 8,
      '#field_prefix' => (!empty($m['prefix']) ? $m['prefix'] : ''),
      '#field_suffix' => (!empty($m['suffix']) ? $m['suffix'] : ''),
    ); 
  }
  
  $scorings = intel_get_scorings();
  $scorings = variable_get('intel_scorings', array());
  $form['scores'] = array(
    '#type' => 'fieldset',
    '#title' => t('Scoring'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  ); 
  $form['scores']['base'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visits & pageviews'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['scores']['events'] = array(
    '#type' => 'fieldset',
    '#title' => t('Events'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['scores']['goals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Goals'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $base_scorings = intel_get_base_scorings();
  foreach ($base_scorings AS $i => $m) {
    $value = !empty($scorings[$i]) ? $scorings[$i] : $m['value'];
    $form['scores']['base']['score_' . $i] = array(
      '#type' => 'textfield',
      '#title' => $m['title'],
      '#default_value' => $value,
      '#description' => $m['description'],
      '#size' => 8,
    );    
  } 

  $events = intel_get_intel_events();
  foreach ($events AS $i => $m) {
    if (empty($m['valued_event'])) {
      continue;
    }
    $value = !empty($scorings[$i]) ? $scorings[$i] : $m['value'];
    $form['scores']['events']['score_' . $i] = array(
      '#type' => 'textfield',
      '#title' => $m['title'],
      '#default_value' => $value,
      '#description' => $m['description'],
      '#size' => 8,
    ); 
  }
  $goals = variable_get('intel_event_goals', array());
  $goals = array_merge($goals, variable_get('intel_submission_goals', intel_get_submission_goals_default()));
  $form_state['goals'] = $goals;
  foreach ($goals AS $i => $m) {
    $value = !empty($scorings['goal_' . $i]) ? $scorings['goal_' . $i] : (isset($m['value']) ? $m['value'] : 0);
    $form['scores']['goals']['score_goal_' . $i] = array(
      '#type' => 'textfield',
      '#title' => $m['title'],
      '#default_value' => $value,
      '#description' => $m['description'],
      '#size' => 8,
    ); 
  }
  
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $event_goals = variable_get('intel_event_goals', array());
  $value = '';
  foreach ($event_goals AS $g) {
    $value .= $g['ga_id'] . '|' . $g['title'] . '|' . $g['description'] . "\n";
  }
  $form['advanced']['event_goals'] = array(
    '#type' => 'textarea',
    '#title' => t('Goals (event driven)'),
    '#default_value' => $value,
    '#description' => t('Enter any goal you would like to trigger using an event. Enter one goal per line as name,ga_goal_id (e.g. <em>Contact form,1</em>). Note in order for goals to track, they must also be setup properly in Google Analytics.'),
  );  
  $submission_goals = variable_get('intel_submission_goals', intel_get_submission_goals_default());
  $value = '';
  foreach ($submission_goals AS $g) {
    $value .= $g['ga_id'] . '|' . $g['title'] . '|' . $g['description'] . "\n";
  }
  $form['advanced']['submission_goals'] = array(
    '#type' => 'textarea',
    '#title' => t('Submission goals'),
    '#default_value' => $value,
    '#description' => t('Enter any goals that can be triggered by a form submission. Enter one goal per line as name,ga_goal_id (e.g. <em>Contact form,1</em>). Note in order for goals to track, they must also be setup properly in Google Analytics.'),
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Update settings'),
  );
 
  
  return $form; 
}



function intel_admin_scoring_submit(&$form, &$form_state) {
  $values = $form_state['values'];  
  $scores = array();
  $targets = array();  
  foreach ($values AS $k => $value) {
    if (substr($k, 0, 6) == 'score_') {
      $key = substr($k, 6);
      $scores[$key] = $value;
    }
    elseif (substr($k, 0, 7) == 'target_') {
      $key = substr($k, 7);
      $targets[$key] = $value;
    }
  }
  
  $defaults = intel_get_submission_goals_default();
  $goals = array();
  $a = explode("\n", $values['submission_goals']);
  foreach ($a AS $b) {
    $c = explode("|", $b);
    if ((count($c) == 2) || (count($c) == 3)) {
      $key = strtolower(drupal_clean_css_identifier($c[1]));
      $goals[$key] = array(
        'ga_id' => (int)trim($c[0]),
        'title' => trim($c[1]),
      );
      if (!empty($c[2])) {
        $goals[$key]['description'] = trim($c[2]);
      }
      if (isset($defaults[$key])) {
        if (isset($defaults[$key]['value'])) {
          $goals[$key]['value'] = $defaults[$key]['value'];
        }
      }
    }
  }
  variable_set('intel_submission_goals', $goals);
  
  $defaults = array();
  $goals = array();
  $a = explode("\n", $values['event_goals']);
  foreach ($a AS $b) {
    $c = explode("|", $b);
    if ((count($c) == 2) || (count($c) == 3)) {
      $key = strtolower(drupal_clean_css_identifier($c[1]));
      $goals[$key] = array(
        'ga_id' => (int)trim($c[0]),
        'title' => trim($c[1]),
      );
      if (!empty($c[2])) {
        $goals[$key]['description'] = trim($c[2]);
      }
      if (isset($defaults[$key])) {
        if (isset($defaults[$key]['value'])) {
          $goals[$key]['value'] = $defaults[$key]['value'];
        }
      }
    }
  }
  variable_set('intel_event_goals', $goals);  
  
  variable_set('intel_scorings', $scores);
  
  variable_set('intel_targets', $targets);  
  
}

function intel_admin_intel_event_list_page() {
  require_once './' . drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
 
  $events = intel_get_intel_events(TRUE);

  $header = array(
    t('Category'),
    t('Title'),
    t('Description'),
    //t('Type'),
    t('Module'), 
    t('Ops'),  
  ); 
  $rows = array();
  
  $options = array();  
  $custom_default_value = '';
  $link_options = array(
    'query' => drupal_get_destination(),
  );
  $link_options = array();
  foreach ($events AS $key => $event) {
    $ops = array();
    if (!empty($event['custom'])) {
      $ops[] = l(t('edit'), 'admin/config/services/intel/intel_event/' . $key . '/edit', $link_options);
      $ops[] = l(t('delete'), 'admin/config/services/intel/intel_event/' . $key . '/delete', $link_options);
    }
    else {
      $ops[] = t('NA');
    }
    $rows[] = array(
      $event['category'],
      $event['title'],
      $event['description'],
      //$attr['type'],
      $event['module'],
      implode(' ', $ops),
    );
  }
  
  $vars = array(
    'header' => $header,
    'rows' => $rows,
  );
  
  $output = theme('table', $vars);
  
  return $output;
}

function intel_admin_intel_event_add_page() {
  //drupal_set_title(t('Add visitor attribute'));
  $form = drupal_get_form('intel_admin_intel_event_form', array());
  return render($form);
}

function intel_admin_intel_event_edit_page($event) {
  drupal_set_title(t('Edit @title event', array('@title' => $event['title'])));
  $form = drupal_get_form('intel_admin_intel_event_form', $event);
  return render($form); 
}



function intel_admin_intel_event_form($form, &$form_state, $event = array()) {
  if (!is_array($event)) {
    $event = array();
  }
  $form_state['event'] = $event;
  $add = empty($event['key']);
  $custom = (!$event || !empty($event['custom'])) ? 1 : 0;

  $form['key'] = array(
    '#type' => ($custom && $add) ? 'textfield' : 'item',
    '#title' => t('Key'),
    '#default_value' => !empty($event['key']) ? $event['key'] : '',
    '#description' => t('A unique identifier for the event.'),
    '#size' => 32,
    '#required' => 1,
  ); 
  $form['key']['#markup'] = $form['key']['#default_value'];
  
  $form['title'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Title'),
    '#default_value' => !empty($event['title']) ? $event['title'] : '',
    '#description' => t('Human friendly title to displayed to site administrators.'),
    '#size' => 32,
    '#required' => 1,
  );
  $form['title']['#markup'] = $form['title']['#default_value'];
  
  $form['description'] = array(
    '#type' => ($custom) ? 'textarea' : 'item',
    '#title' => t('Description'),
    '#default_value' => !empty($event['description']) ? $event['description'] : '',
    '#description' => t('Description of the attribute.'),
    '#rows' => 2,
  );
  $form['description']['#markup'] = $form['description']['#default_value'];
  
  $form['ga_event_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics event parameters'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('The parameters sent to Google Analtyics when the event is triggered.'),
  );
  $form['ga_event_values']['category'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Event category'),
    '#default_value' => !empty($event['category']) ? $event['category'] : '',
    '#description' => t('Input Google Analytics event category value. If left blank the event title will be used.'),
    '#size' => 64,
  );
  $form['ga_event_values']['category']['#markup'] = $form['ga_event_values']['category']['#default_value'];
  
  $form['ga_event_values']['action'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Event action'),
    '#default_value' => !empty($event['action']) ? $event['action'] : '',
    '#description' => t('Input Google Analytics event action value. Tokens can be used. If left blank the current page\'s page title will be used'),
    '#size' => 64,
  );
  $form['ga_event_values']['action']['#markup'] = $form['ga_event_values']['action']['#default_value'];
  
  $form['ga_event_values']['label'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Event label'),
    '#default_value' => !empty($event['label']) ? $event['label'] : '',
    '#description' => t('Input Google Analytics event label value. Tokens can be used. If left blank the current pages url path will be used.'),
    '#size' => 64,
  );
  $form['ga_event_values']['action']['#markup'] = $form['ga_event_values']['action']['#default_value'];
  
  $form['ga_event_values']['valued_event'] = array(
    '#type' => ($custom) ? 'checkbox' : 'item',
    '#title' => t('Is valued event'),
    '#default_value' => !empty($event['valued_event']) ? $event['valued_event'] : 0,
    '#description' => t('If checked, values for this event will be counted in scoring.'),
  );
  $form['ga_event_values']['valued_event']['#markup'] = ($form['ga_event_values']['valued_event']['#default_value']) ? 'Yes' : 'No';
  
  $form['ga_event_values']['value'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Event value'),
    '#default_value' => !empty($event['value']) ? $event['value'] : 0,
    '#description' => t('Input Google Analytics event label value. This value can be overridden per node using the intel events value field.'),
    '#size' => 12,
  );
  $form['ga_event_values']['value']['#markup'] = $form['ga_event_values']['value']['#default_value'];

  $form['ga_event_values']['noninteraction'] = array(
    '#type' => ($custom) ? 'checkbox' : 'item',
    '#title' => t('Non interaction'),
    '#default_value' => !empty($event['noninteraction']) ? $event['noninteraction'] : 0,
    '#description' => t('If checked, the event will counted similar to a pageview for bounce tracking.'),
  );
  $form['ga_event_values']['noninteraction']['#markup'] = ($form['ga_event_values']['noninteraction']['#default_value']) ? 'Yes' : 'No';

  $form['trigger'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event trigger'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Used to set the conditions for triggering the event.'),
  );
  $form['trigger']['event'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('jQuery event'),
    '#default_value' => !empty($event['event']) ? $event['event'] : '',
    '#description' => t('Input a jQuery event to trigger this event. If left blank the even will trigger on showpage.'),
    '#size' => 64,
  );
  $form['trigger']['selector'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('jQuery selector'),
    '#default_value' => !empty($event['selector']) ? $event['selector'] : '',
    '#description' => t('Input a jQuery selector to bind the event to.'),
    '#size' => 64,
  );
  $form['ga_event_values']['value']['#markup'] = $form['ga_event_values']['value']['#default_value'];
  
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($event['type']) && empty($event['callback']),
    '#description' => t('Hooks to enable JavaScript to customize this event. Be careful using these settings as improper use may cause JavaScript errors.'),
  );
  $form['advanced']['type'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Event type'),
    '#default_value' => !empty($event['type']) ? $event['type'] : '',
    '#description' => t('The type enables custom event configuration before event processing executes. If type is set, a method in the form of trackIntelEvent[type] will be called.'),
    '#size' => 64,
  );
  $form['advanced']['callback'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Callback'),
    '#default_value' => !empty($event['callback']) ? $event['callback'] : '',
    '#description' => t('Declares a function to execute after the intel event data has been processed but before it is sent to Google Analytics. Must be set to the name of an avaialble JavaScript function.'),
    '#size' => 64,
  );
  $form['ga_event_values']['value']['#markup'] = $form['ga_event_values']['value']['#default_value'];
  
  $form['selectable'] = array(
    '#type' => ($custom) ? 'checkbox' : 'item',
    '#title' => t('Selectable'),
    '#default_value' => !empty($event['selectable']) ? $event['selectable'] : 1,
    '#description' => t('Check to make this event selectable in entity fields.'),
  );
  $form['selectable']['#markup'] = ($form['selectable']['#default_value']) ? 'Yes' : 'No';
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => $add ? t('Add event') : t('Save event'),
  );

  return $form;
}

function intel_admin_intel_event_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  $event = $form_state['event'];
  $key = !empty($values['key']) ? $values['key'] : $event['key'];
  
  $events = intel_get_intel_events();    
  
  // check unique key if new event
  if (!isset($event['key'])) {
    if (isset($event[$values['key']])) {
      $msg = t('Duplicate event key %key. Please select a unique key.',
      array(
        '%key' => $values['key'],
      ));
      form_set_error('key', $msg);   
    }
  }  
}

function intel_admin_intel_event_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $add = empty($form_state['event']['key']) ? 1 : 0;

  $event = array();
  if ($add) { 
    $key = $values['key'];   
  }
  else {
    $key = $form_state['event']['key'];
  }  
  $indexes = array(
    'title',
    'description',
    'category',
    'action',
    'label',
    'value',
    'event',
    'selector',
    'callback',
    'selectable',
    'noninteraction',
    'valued_event',
  );
  foreach ($indexes AS $index) {
    if (isset($values[$index])) {
      $event[$index] = trim($values[$index]);
    } 
  }
  
  $events = variable_get('intel_intel_events_custom', array()); 
  $events[$key] = $event;
  variable_set('intel_intel_events_custom', $events); 
  
  if ($add) {
    $msg = t('Intel event %title has been added.', array(
      '%title' => $event['title'],
    ));
  }
  else {
    $msg = t('Intel event %title has been updated.', array(
      '%title' => $event['title'],
    ));    
  }
  drupal_set_message($msg);
  drupal_goto('admin/config/services/intel/intel_event');
}

function intel_admin_intel_event_delete_page($event) {
  drupal_set_title(t('Are you sure you want to delete @title?', array('@title' => $event['title'])));
  $form = drupal_get_form('intel_admin_intel_event_delete_form', $event);
  return render($form); 
}

function intel_admin_intel_event_delete_form($form, &$form_state, $event) {
  $form_state['event'] = $event;
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'intel_admin_intel_event_delete_form_submit';
  $confirm_question = t('Are you sure you want to delete @title?', array('@title' => $event['title']));
  return confirm_form($form,
    $confirm_question,
    'admin/content', 
    t('This action cannot be undone.'),
    t('Delete'), 
    t('Cancel'));
}

function intel_admin_intel_event_delete_form_submit($form, &$form_state) {
  $key = $form_state['event']['key'];  
  
  $events = variable_get('intel_intel_events_custom', array()); 
  unset($events[$key]);
  variable_set('intel_intel_events_custom', $events); 
  
  $msg = t('Intel event %title has been deleted.', array(
    '%title' => $event['title'],
  ));    
  drupal_set_message($msg);
  drupal_goto('admin/config/services/intel/intel_event');
}


function intel_admin_visitor_attribute_list_page() {
  return intel_admin_attribute_list_page('visitor');
}

function intel_admin_page_attribute_list_page() {
  return intel_admin_attribute_list_page('page');
}

function intel_admin_attribute_list_page($scope = 'visitor') {
  require_once './' . drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
 
  if ($scope == 'page') {
    $attributes = intel_get_page_attributes();    
  }
  else {
    $attributes = intel_get_visitor_attributes();
  }

  $header = array(
    t('Key'),
    t('Title'),
    t('Description'),
    t('Type'),
    t('Module'), 
    t('Ops'),  
  ); 
  $rows = array();
  
  $options = array();  
  $custom_default_value = '';
  $link_options = array(
    'query' => drupal_get_destination(),
  );
  $link_options = array();
  foreach ($attributes AS $key => $attr) {
    $ops = array();
    $ops[] = l(t('edit'), 'admin/config/services/intel/' . $scope . '_attribute/' . $key . '/edit', $link_options);
    $rows[] = array(
      $key,
      $attr['title'],
      $attr['description'],
      $attr['type'],
      $attr['module'],
      implode(' ', $ops),
    );
  }
  
  $vars = array(
    'header' => $header,
    'rows' => $rows,
  );
  
  $output = theme('table', $vars);
  
  return $output;
}

function intel_admin_visitor_attribute_add_page() {
  drupal_set_title(t('Add visitor attribute'));
  $form = drupal_get_form('intel_admin_attribute_form', array(), 'visitor');
  return render($form);
}

function intel_admin_visitor_attribute_edit_page($attribute) {
  drupal_set_title(t('Edit @title visitor attribute', array('@title' => $attribute['title'])));
  $form = drupal_get_form('intel_admin_attribute_form', $attribute, 'visitor');
  return render($form); 
}

function intel_admin_page_attribute_add_page() {
  drupal_set_title(t('Add vpage attribute'));
  $form = drupal_get_form('intel_admin_attribute_form', array(), 'page');
  return drupal_render($form);
}

function intel_admin_page_attribute_edit_page($attribute) {
  drupal_set_title(t('Edit @title page attribute', array('@title' => $attribute['title'])));
  $form = drupal_get_form('intel_admin_attribute_form', $attribute, 'page');
  return render($form); 
}

function intel_admin_attribute_form($form, &$form_state, $attribute = array(), $scope = 'visitor') {
  if (!is_array($attribute)) {
    $attribute = array();
  }
  $form_state['attribute'] = $attribute;
  $form_state['scope'] = $scope;
  $add = (!$attribute) ? 1 : 0;
  $custom = (!$attribute || !empty($attribute['custom'])) ? 1 : 0;

  $form['key'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Key'),
    '#default_value' => !empty($attribute['key']) ? $attribute['key'] : '',
    '#description' => t('A short unique identifier for the attribute. Keep this a short as possible.'),
    '#size' => 12,
    '#required' => 1,
  ); 
  $form['key']['#markup'] = $form['key']['#default_value'];
  
  $form['title'] = array(
    '#type' => ($custom) ? 'textfield' : 'item',
    '#title' => t('Title'),
    '#default_value' => !empty($attribute['title']) ? $attribute['title'] : '',
    '#description' => t('Human friendly attribute title to displayed to site administrators.'),
    '#size' => 32,
    '#required' => 1,
  );
  $form['title']['#markup'] = $form['title']['#default_value'];
  
  $form['description'] = array(
    '#type' => ($custom) ? 'textarea' : 'item',
    '#title' => t('Description'),
    '#default_value' => !empty($attribute['description']) ? $attribute['description'] : '',
    '#description' => t('Description of the attribute.'),
    '#rows' => 2,
  );
  $form['description']['#markup'] = $form['description']['#default_value'];
  
  $types = array(
    'flag' => t('Flag'),
    'scalar' => t('Scalar'),
    'list' => t('List'),
    'vector' => t('Vector'),
  );
  $form['type'] = array(
    '#type' => ($custom) ? 'select' : 'item',
    '#title' => t('Type'),
    '#options' => $types,
    '#default_value' => !empty($attribute['type']) ? $attribute['type'] : '',
    '#description' => t('Format of the values attribute is storing.'),
    '#required' => 1,
  );
  $form['type']['#markup'] = $form['type']['#default_value'];
  
  $desc = !empty($attribute['options_description']) ? $attribute['options_description'] : '';
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => check_plain($desc),
  );
  
  if (empty($attribute['type']) || isset($attribute['static_options']) || isset($attribute['custom_options'])) {
    $custom_str = '';
    $header = array(
      t('Key'),
      t('Title'),
      t('Description'),
      t('Mode'),
    ); 
    $rows = array();
    if (!empty($attribute['static_options']) && is_array($attribute['static_options'])) {
      foreach ($attribute['static_options'] AS $key => $value) {
        $rows[] = array(
          $key,
          (is_array($value) && !empty($value['title'])) ? $value['title'] : $value,
          (is_array($value) && !empty($value['description'])) ? $value['description']: '',
          t('static'),
        );
      }
    }
    if (!empty($attribute['custom_options']) && is_array($attribute['custom_options'])) {
      foreach ($attribute['custom_options'] AS $key => $value) {
        $rows[] = array(
          $key,
          $value['title'],
          $value['description'],
          t('custom'),
        );
        $custom_str .= "$key|{$value['title']}";
        if (!empty($value['description'])) {
          $custom_st .= "|" . $value['description'];
        }
        $custom_st .= "\n";
      }
    }
    $form['options']['options'] = array(
      '#type' => 'item',
      '#title' => t('Existing options'),
      '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
    ); 
    
    if (empty($attribute['type']) || (isset($attribute['custom_options']) && is_array($attribute['custom_options']))) {
      $form['options']['custom_options'] = array(
        '#type' => 'textarea',
        '#title' => t('Custom options'),
        '#default_value' => $custom_str,
        '#description' => t('Enter one option per line as key|label|description (e.g. <em>p|Profile|User profiles</em>).'),
      ); 
    } 
  }
  
  $form['selectable'] = array(
    '#type' => ($custom) ? 'checkbox' : 'item',
    '#title' => t('Selectable'),
    '#default_value' => !empty($attribute['selectable']) ? $attribute['selectable'] : 1,
    '#description' => t('Check to make this attribute selectable in entity fields.'),
  );
  $form['selectable']['#markup'] = ($form['selectable']['#default_value']) ? 'Yes' : 'No';
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save attribute'),
  );

  return $form;
}

function intel_admin_attribute_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  $attribute = $form_state['attribute'];
  $key = !empty($values['key']) ? $values['key'] : $attribute['key'];
  
  $scope = $form_state['scope'];
  if ($scope == 'page') {
    $attributes = intel_get_page_attributes();    
  }
  else {
    $attributes = intel_get_visitor_attributes();
  }
  
  // check unique key if new attribute
  if (!isset($attribute['key'])) {
    if (isset($attributes[$values['key']])) {
      $msg = t('Duplicate attribute key %key. Please select a unique key.',
      array(
        '%key' => $values['key'],
      ));
      form_set_error('key', $msg);   
    }
  }
  
  // process custom options
  $a = explode("\n", $values['custom_options']);
  $static_options = $attribute['static_options'];
  $custom_options = array();
  foreach ($a AS $b) {
    if (!trim($b)) {
      continue;
    }
    $c = explode("|", $b);
    if ((count($c) != 2) && (count($c) != 3)) {
      $msg = t('Each custom option must contain 2 or 3 elements per line seperated by a | (pipe).');
      form_set_error('intel_page_intents_custom', $msg);      
    }
    elseif (isset($static_options[$c[0]])) {
      $msg = t('Custom option key same as static option key %key. Please select a unique key.',
      array(
        '%key' => $c[0],
      ));
      form_set_error('custom_options', $msg);         
    }   
    elseif (isset($keys[$c[0]])) {
      $msg = t('Duplicate custom option key %key. Please select a unique key.',
      array(
        '%key' => $c[0],
      ));
      form_set_error('custom_options', $msg);         
    }
    $custom_options[$c[0]] = array(
      'title' => $c[1],
      'description' => !empty($c[2]) ? $c[2] : '',
    );
  }
  $form_state['custom_options'] = $custom_options;
}

function intel_admin_attribute_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $add = empty($form_state['attribute']['key']) ? 1 : 0;
  $scope = $form_state['scope'];

  $attribute = array();
  if ($add) { 
    $key = $values['key'];   
  }
  else {
    $key = $form_state['attribute']['key'];
  }
  
  if (isset($values['title'])) {
    $attribute['title'] = $values['title'];
  }
  if (isset($values['description'])) {
    $attribute['description'] = $values['description'];
  }
  if (isset($values['type'])) {
    $attribute['type'] = $values['type'];
  }
  if (isset($values['selectable'])) {
    $attribute['selectable'] = $values['selectable'];
  }
  if (isset($values['custom_options'])) {
    $attribute['custom_options'] = $form_state['custom_options'];
  }  
  
  
  if ($scope == 'page') {
    $attributes = variable_get('intel_page_attributes_custom', array());
    $attributes[$key] = $attribute;
    variable_set('intel_page_attributes_custom', $attributes); 
  }
  else {
    $attributes = variable_get('intel_visitor_attributes_custom', array());
    $attributes[$key] = $attribute;
    variable_set('intel_visitor_attributes_custom', $attributes); 
  }
  
  if ($add) {
    $msg = t('@attr_type attribute %title has been added.', array(
      '@attr_type' => ucfirst($scope),
      '%title' => $attribute['title'],
    ));
  }
  else {
    $msg = t('@attr_type attribute %title has been updated.', array(
      '@attr_type' => ucfirst($scope),
      '%title' => $attribute['title'],
    ));    
  }
  drupal_set_message($msg);
}

function intel_admin_external_tracking($form, &$form_state) { 
  global $base_url, $base_path;

  $default_settings = intel_external_tracking_defaults();
  $settings = $saved_settings = variable_get('intel_external_tracking_settings', $default_settings);
  $intel_scripts = intel_intel_scripts();
  
  if (empty($settings['cms_hostpath'])) {
    $a = explode('//', $base_root . $base_path); 
    $settings['cms_hostpath'] = $a[1];
  }
  $pushes = array();
  if ($settings['is_landingpage']) {
    $pushes[] = array(
      'method' => '_addIntelEvent',
      'event' => 'pageshow',
      'category' => t('Landing page view'),
      'action' => '!page_title',
      'label' => '!location',
    );
    if (empty($settings['pa']['content_type'])) {
      $settings['pa']['content_type'] = 'landingpage';
    }
  }
  $settings['is_external'] = 1;
  $pushes[] = array(
    'method' => '_setIntelVar',
    'scope' => 'page',
    'namespace' => 'analytics',
    'keys' => 's',
    'value' => (!empty($settings['pa']['section'])) ? $settings['pa']['section'] : t('external'),
  );
  $pushes[] = array(
    'method' => '_setIntelVar',
    'scope' => 'page',
    'namespace' => 'analytics',
    'keys' => 'ct',
    'value' => (!empty($settings['pa']['content_type'])) ? $settings['pa']['content_type'] : t('externalpage'),
  );
  
  $form_state['l10i_settings'] = $settings; 
  
  $settings['pushes'] = $pushes;
  unset($settings['pa']);
  unset($settings['intel_scripts']);
  unset($settings['include_jquery']);

  $encoded_settings = drupal_json_encode($settings);

  // A dummy query-string is added to filenames, to gain control over
  // browser-caching. The string changes on every update or full cache
  // flush, forcing browsers to load a new copy of the files, as the
  // URL changed.   
  $query_string = '?' . variable_get('css_js_query_string', '0');
  $jquery_include = $settings['include_jquery'] ? '<script type="text/javascript" src="https://ajax.googleapis.com/ajax/libs/jquery/1.5.2/jquery.min.js"></script>' . "\n" : '';

  $header = '';
  $header .= "<!-- begin LevelTen Intel script -->\n";
  $header .= $jquery_include;
  $header .= intel_get_js_embed('combined', 'external', 'simple', ""); 
  if (!empty($saved_settings['intel_scripts'])) {
    foreach ($saved_settings['intel_scripts'] AS $k => $v) {
      if ($v) {
        $header .= '<script src="' . $base_url . $base_path . $intel_scripts[$k]['path'] . '"></script>' . "\n";
      }
    }
  }
  $header .= "<!-- end LevelTen Intel script -->";
  
  $header = str_replace("_l10iq.push(['_setOptions', {}]);", "_l10iq.push(['_setOptions', $encoded_settings]);", $header);
  
  $footer = '';

  $form['output']['header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header external tracking code'),
    '#default_value' => $header,
    '#description' => t('Copy and paste this code into any third party sites you want to track.'),
    '#rows' => 12,
  );
  
  /*
  $form['output']['footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer external tracking code'),
    '#default_value' => $footer,
    '#description' => t('Copy and paste this code into any third party sites you want to track.'),
    '#rows' => 12,
  );
  */
  
  $form_state['l10i_settings'] = $settings;
  
  $form['include_jquery'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include jQuery'),
    '#default_value' => $saved_settings['include_jquery'],
    '#description' => t('If checked, jQuery 1.5.2 will be attached on the page. Uncheck if the page already includes a compatiable version of jQuery.'),
  );
  
  $form['stop_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Stop pattern'),
    '#default_value' => $saved_settings['stop_pattern'],
    '#description' => t('Include any URL patterns you do not want tracked. Seperate multiple paths with a comman. The script will do a head pattern match against all strings.'),
  );
  
  $form['is_landingpage'] = array(
    '#type' => 'checkbox',
    '#title' => t('Is landing page'),
    '#default_value' => $saved_settings['is_landingpage'],
    '#description' => t('Check this box if you want the page to be tracked as a landing page.'),
  );
  
  $form['section'] = array(
    '#type' => 'textfield',
    '#title' => t('Section'),
    '#default_value' => $saved_settings['pa']['section'],
    '#description' => t('Used to set section ga custom var. Leave blank for section to be set to "external".'),
  );

  $form['content_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Content type'),
    '#default_value' => $saved_settings['pa']['content_type'],
    '#description' => t('Used to set content type ga custom var. If left blank, will be set to "landingpage" if the <em>is landingpage</em> box is checked, otherwise defaults to "external_page".'),
  ); 
  
  $options = array();
  foreach ($intel_scripts AS $k => $v) {
    $options[$k] = $v['title'];
  }
  $form['intel_scripts'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Intel scripts'),
    '#options' => $options,
    '#default_value' => !empty($saved_settings['intel_scripts']) ? $saved_settings['intel_scripts'] : array(),
    '#description' => t('Select any intel integration scripts you want to include on your external page.'),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Update settings'),
  );
  
  return $form;
}

function intel_admin_external_tracking_submit(&$form, &$form_state) {
  $settings = $form_state['l10i_settings'];

  $values = $form_state['values'];
  $settings['include_jquery'] = ($values['include_jquery']) ? 1 : 0;
  $settings['is_landingpage'] = ($values['is_landingpage']) ? 1 : 0;
  $settings['stop_pattern'] = trim($values['stop_pattern']);
  //$settings['domain_name'] = trim($values['domain_name']);
  $settings['intel_scripts'] = $values['intel_scripts'];
  $settings['pa'] = array();
  $settings['pa']['section'] = trim($values['section']);
  $settings['pa']['content_type'] = trim($values['content_type']);
  
  unset($settings['section']);  // temp to remove moved index
  unset($settings['content_type']); // temp to remove moved index
  
  variable_set('intel_external_tracking_settings', $settings);
}

function intel_external_tracking_defaults() {
 $settings = array(
    'include_jquery' => 1,
    'stop_pattern' => '', 
    'cms_hostpath' => '', 
    'track_forms' => 1,
    'is_landingpage' => 0,
    'pushes' => array(),  
  );
  return $settings; 
}

function intel_admin_modules_page() {
  $output = 'comming soon';
  return $output;
}