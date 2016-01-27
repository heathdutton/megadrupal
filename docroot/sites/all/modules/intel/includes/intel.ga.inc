<?php
/**
 * @file
 * Functions to support extended Google Analytics data.
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

/**
 * types = flag, list, scalar, vector 
 */
function intel_get_intel_events_default() {
  $event = array();

  $event['landing_page_view'] = array(
    'title' => t('Landing page view'), 
    'description' => t('Landing page pageview'),   
    //'event' => 'pageshow',
  );
  $event['landing_page_conversion'] = array(
    'title' => t('Landing page conversion'), 
    'description' => t('Form submission from a landing page'),   
    'valued_event' => 1,
    'value' => 0,
  );
  $event['form_submission'] = array(
    'title' => t('Form submission'), 
    'description' => t('Genearl form submission'),   
    'valued_event' => 1,
    'value' => 25,
  );
  $event['addthis_social_share'] = array(
    'title' => t('AddThis social share'), 
    'category' => t('Social share'), 
    'description' => t('Click on social sharing widget'),  
    'valued_event' => 1,
    'value' => 5, 
    'js_setting' => 1,
  );
  $event['addthis_social_share_clickback'] = array(
    'title' => t('AddThis social share clickback'), 
    'category' => t('Social share clickback'), 
    'description' => t('Visitor referred from a social share'),  
    'valued_event' => 1,
    'value' => 0, 
    'js_setting' => 1,
  );
  $event['disqus_comment'] = array(
    'title' => t('Disqus comment'), 
    'category' => t('Comment'), 
    'description' => t('Click on social sharing widget'),   
    'valued_event' => 1,
    'value' => 10,
    'js_setting' => 1,
  );
  return $event;
}

function intel_get_event_goals() {
  $event = array();
  $goals = variable_get('intel_event_goals', array());
  foreach ($goals AS $key => $goal) {
    $ekey = 'goal_' . $key;
    $event[$ekey] = $goal;
    $event[$ekey]['title'] = $event[$ekey]['category'] = t('Goal') . ': ' . $goal['title'];
    $event[$ekey]['category'] .= '+';
    $event[$ekey]['selectable'] = 1;
  }
  
  $goals = variable_get('intel_submission_goals', intel_get_submission_goals_default());

  foreach ($goals AS $key => $goal) {
    $ekey = 'submission_goal_' . $key;
    $event[$ekey] = $goal;
    $event[$ekey]['title'] = $event[$ekey]['category'] = t('Form submission') . ': ' . $goal['title'];
    $event[$ekey]['category'] .= '+';
    $event[$ekey]['event'] = 'form_submission';
  }
  return $event;
}

function intel_get_intel_events($include_goals = FALSE) {
  $events = &drupal_static(__FUNCTION__);
  if (count($events)) {
    return $events;
  }
  $events = intel_get_intel_events_default();
  if (1 || $include_goals) {
    $events = array_merge($events, intel_get_event_goals());
  }
  
  foreach ($events AS $k => $v) {
    if (isset($v['static_options'])) {
      $events[$k]['options'] = $v['static_options'];
    }
    if (empty($v['category'])) {
      $events[$k]['category'] = $v['title'];
    }
    $events[$k]['module']  = 'intel';
  } 

  $custom = variable_get('intel_intel_events_custom', array());
  foreach ($custom AS $k => $v) {
    // check if custom attribute
    if (isset($events[$k])) {
      foreach ($v AS $a => $b) {
        $events[$k][$a] = $b; 
      }
    }
    else {
      $custom[$k]['module']  = 'intel';
      $custom[$k]['custom'] = 1;
      $events[$k] = $custom[$k];
      if (empty($v['category'])) {
        $events[$k]['category'] = $v['title'];
      }
    }
    if (!empty($custom[$k]['custom_options'])) {
//sdm($custom[$k]['custom_options']);
      if (!isset($events[$k]['options'])) {
        $events[$k]['options'] = $custom[$k]['custom_options'];
      }
      else {
        $events[$k]['options'] += $custom[$k]['custom_options'];
      }
    }
  } 
  
  foreach (module_implements('intel_intel_events') AS $module) {
    $function = $module . '_intel_intel_events';
    $a = $function();
    if (empty($a) || !is_array($a)) {
      continue;
    }
    foreach ($a AS $k => $v) {
      $v['module'] = $module;
      $events[$k] = $v;
    }
  }

  drupal_alter('intel_intel_events', $events);
  
  uasort($events, '_intel_sort_by_category');
 
  return $events;  
}

function intel_intel_event_load($key) {
  $events = intel_get_intel_events();
  $event = $events[$key];
  $event['key'] = $key;
  return $event; 
}

/**
 * types = flag, list, scalar, vector 
 */
function intel_get_visitor_attributes_default() {
  $attributes = array();
  $user_roles = user_roles();
  // remove anonymous user
  unset($user_roles[1]);

  $attributes['r'] = array(
    'title' => t('User roles'), 
    'description' => t('Set to Drupal user roles for authenticated visitors.'),   
    'type' => 'list',
    'coded_options' => $user_roles,
  );
  $attributes['k'] = array(
    'title' => t('Known'), 
    'description' => t('If a visitor has submitted an email address.'),      
    'type' => 'flag',
  );
  $attributes['s'] = array(
    'title' => t('Score'),  
    'description' => t('Sum of valued events and goals triggered by visitor.'),    
    'type' => 'scalar',
    'selectable' => 1,
  );
  $attributes['l'] = array(
    'title' => t('Lead score'), 
    'description' => t('Used to value visitors customer potential.'),    
    'type' => 'scalar',
    'selectable' => 1,
  );
  $attributes['g'] = array(
    'title' => t('Groups'),  
    'description' => t('Used to categorize visitors by a defining characteristics.'),    
    'type' => 'list',
    'custom_options' => array(),
    'selectable' => 1,
  );
  $attributes['i'] = array(
    'title' => t('Interests'),  
    'description' => t('Used to value a visitors interest in various items.'),    
    'type' => 'vector',
    'custom_options' => array(),
    'selectable' => 1,
  );
  return $attributes;
}

function intel_get_page_attributes_default() {
  $attributes = array();

  $attributes['a'] = array(
    'title' => t('Author'), 
    'description' => t('The uid of a node author.'),   
    'type' => 'scalar',
    'options_description' => t('Auto generated from entity uid.'),  
  );
  $attributes['ct'] = array(
    'title' => t('Content type'),  
    'description' => t('Node type or entity bundle type.'), 
    'type' => 'scalar',
    'options_description' => t('Auto generated from entity type/bundle.'),    
    'type' => 'value',
  );
  $attributes['et'] = array(
    'title' => t('Entity type'), 
    'description' => t('The entity type of the page. (node, user...)'),      
    'type' => 'list',
  );
  $attributes['i'] = array(
    'title' => t('Page intent'),  
    'description' => t('The role a page plays on the site.'),    
    'type' => 'list',
    'static_options' => intel_get_page_intents_default('config'),
    'custom_options' => array(),
    'selectable' => 1,
  );
  $attributes['pt'] = array(
    'title' => t('Publish time'), 
    'description' => t('The time (unix timestamp) a node was created.'),
    'options_description' => t('Auto generated from entity created date.'), 
    'type' => 'scalar',
  );
  $attributes['s'] = array(
    'title' => t('Section'),  
    'description' => t('Node type or entity bundle type.'),    
    'type' => 'scalar',
    'custom_options' => array(),
    'selectable' => 1,
  );
  $attributes['t'] = array(
    'title' => t('Taxomony term'), 
    'description' => t('List of taxomonies term ids.'),   
    'type' => 'list',
    'options_description' => t('Auto generated from terms in selected taxonomy fields.'),  
  );
  return $attributes;
}

function intel_get_page_attributes() {
  return intel_get_attributes('page');
}

function intel_get_visitor_attributes() {
  return intel_get_attributes('visitor');
}

function intel_get_attributes($mode = 'visitor') {
  $attributes = &drupal_static(__FUNCTION__);
  if (isset($attributes[$mode])) {
    return $attributes[$mode];
  }
  $attrs = ($mode == 'page') ? intel_get_page_attributes_default() : intel_get_visitor_attributes_default();

  foreach ($attrs AS $k => $v) {
    if (isset($v['static_options'])) {
      $attrs[$k]['options'] = $v['static_options'];
    }
    $attrs[$k]['module']  = 'intel';
  } 

  $custom = variable_get('intel_' . $mode . '_attributes_custom', array());
  foreach ($custom AS $k => $v) {
    // check if custom attribute
    if (isset($attrs[$k])) {
      foreach ($v AS $a => $b) {
        $attrs[$k][$a] = $b; 
      }
    }
    else {
      $custom[$k]['module']  = 'intel';
      $custom[$k]['custom'] = 1;
      $attrs[$k] = $custom[$k];
    }
    if (!empty($custom[$k]['custom_options'])) {
//sdm($custom[$k]['custom_options']);
      if (!isset($attrs[$k]['options'])) {
        $attrs[$k]['options'] = $custom[$k]['custom_options'];
      }
      else {
        $attrs[$k]['options'] += $custom[$k]['custom_options'];
      }
    }
  } 
  
  foreach (module_implements('intel_' . $mode . '_attributes') AS $module) {
    $function = $module . '_intel_' . $mode . '_attributes';
    $a = $function();
    if (empty($a) || !is_array($a)) {
      continue;
    }
    foreach ($a AS $k => $v) {
      $v['module'] = $module;
      $attributes[$k] = $v;
    }
  }

  drupal_alter('intel_' . $mode . '_attributes', $attrs);
  
  uasort($attrs, '_intel_sort_by_title');
  
  $attributes[$mode] = $attrs;  
  return $attributes[$mode];  
}

function intel_page_attribute_load($key) {
  $attributes = intel_get_page_attributes();
  $attribute = $attributes[$key];
  $attribute['key'] = $key;
  return $attribute; 
}

function intel_get_page_intents_default($filter = 'report') {
  $intents = array();
  if ($filter == 'report') {
    $intents[''] = array(
      'title' => t('(not set)'),
      'description' => t('Intent is not set by page.'),
      'category' => 'system',
    );
  }
  if ($filter == 'entity_edit') {
    $intents['_default'] = array(
      'title' => t('- Default -'),
      'description' => t('Use the default for the entity type.'),
      'category' => 'system',
    );
  }
  $intents['a'] = array(
    'title' => t('Admin'),
    'description' => t('Pages used to administer the site.'),
  );
  $intents['t'] = array(
    'title' => t('Attraction'),
    'description' => t('Pages designed to attract people and generate buzz such as blog posts and podcasts.'),
  );
  $intents['i'] = array(
    'title' => t('Information'),
    'description' => t('Page that are informational such as product and services.'),
  );
  $intents['l'] = array(
    'title' => t('Landing page'),
    'description' => t('Page that is designed to entice visitors to submit a form or do one specific action.'),
  );
  $intents['u'] = array(
    'title' => t('Utility'),
    'description' => t('Support pages that should be excluded from content reports. Useful for thankyou pages.'),
  );
  return $intents;
}

function intel_get_page_intents($filter = 'report') {
  $intents = &drupal_static(__FUNCTION__);
  if (!empty($intents)) {
    return $intents;
  }
  $page_attributes = intel_get_page_attributes();
  
  $page_intents = $page_attributes['i']['options'];
  
  uasort($page_intents, '_intel_sort_by_title');
  
  return $page_intents;
}



function intel_visitor_attribute_load($key) {
  $attributes = intel_get_visitor_attributes();
  $attribute = $attributes[$key];
  $attribute['key'] = $key;
  return $attribute; 
}

function _intel_sort_by_title($a, $b) {
  return ($a['title'] > $b['title']) ? 1 : -1; 
}

function _intel_sort_by_category($a, $b) {
  return ($a['category'] > $b['category']) ? 1 : -1; 
}

function _intel_sort_by_rtime($a, $b) {
  return ($a['time'] < $b['time']) ? 1 : -1; 
}

function intel_init_targets() {
  $target = array();
  $target['entrances_per_month'] = array(
    'title' => t('Total visitors / month'),    
    'description' => t('Your target for the number of total visitors you want to get to the site.'),
    'value' => 3000,
    'group' => 'site_kpi_month',
  );
  $target['leads_per_month'] = array(
    'title' => t('New contacts / month'),    
    'description' => t('Your target for the number of contacts in a month.'),
    'value' => 30,
    'group' => 'site_kpi_month',
  );
  $target['posts_per_month'] = array(
    'title' => t('Posts / month'),    
    'description' => t('Your target attraction, e.i. blog, posts in a month.'),
    'value' => 8,
    'group' => 'site_kpi_month',
  );
  $target['entrances_per_day'] = array(
    'title' => t('Total visitors / day'),    
    'description' => t('Your target for the number of total visitors you want to get to the site.'),
    'value' => 100.00,
    'group' => 'site_day',
  );
  $target['entrances_per_day_warning'] = array(
    'title' => t('Total visitors / day (warning)'),    
    'description' => t('Your target for the number of total visitors you want to get to the site.'),
    'value' => 25.00,
    'group' => 'site_day',
  );
  
  $target['value_per_day'] = array(
    'title' => t('Total value / day'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 100.00,
    'group' => 'site_day',
  );
  $target['value_per_day_warning'] = array(
    'title' => t('Total value / day (warning)'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 25.00,
    'group' => 'site_day',
  );
  
  $target['conversions_per_day'] = array(
    'title' => t('Total conversions / day'),    
    'description' => t('Your target for the number of total visitors you want to get to the site.'),
    'value' => 4.00,
    'group' => 'site_day',
  );
  $target['conversions_per_day_warning'] = array(
    'title' => t('Total conversions / day (warning)'),    
    'description' => t('Your target for the number of total visitors you want to get to the site.'),
    'value' => 1.00,
    'group' => 'site_day',
  );
  
  $target['value_per_page_per_day'] = array(
    'title' => t('Value per page / day'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 1.00,
    'group' => 'page',
  ); 
  $target['value_per_page_per_day_warning'] = array(
    'title' => t('Value per page / day (warning)'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 0.10,
    'group' => 'page',
  );
  
  $target['value_per_page_per_entrance'] = array(
    'title' => t('Value per page / entry'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 0.50,
    'group' => 'page',
  ); 
  $target['value_per_page_per_entrance_warning'] = array(
    'title' => t('Value per page / entry (warning)'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 0.20,
    'group' => 'page',
  );

  $target['value_per_visit'] = array(
    'title' => t('Value / visit'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 1.00,
    'group' => 'visit',
  ); 
  $target['value_per_visit_warning'] = array(
    'title' => t('Value per page / day (warning)'),    
    'description' => t('Your target for the total value of all traffic to the site.'),
    'value' => 0.10,
    'group' => 'visit',
  ); 
  

  return $target;
}

function intel_get_targets() {
  $stargets = &drupal_static(__FUNCTION__);
  if (isset($targets)) {
    return $targets;
  }
  $targets = array();
  $defaults = intel_init_targets();
  foreach ($defaults AS $k => $v) {
    $targets[$k] = $v['value'];
  }
  $ds = variable_get('intel_targets', array());
  foreach ($ds AS $k => $v) {
    if (trim($v)) {
      $targets[$k] = $v;
    }
  }
  if (empty($targets['entrances_per_page_per_day'])) {
    $targets['entrances_per_page_per_day'] = $targets['value_per_page_per_day'] / $targets['value_per_page_per_entrance'];
  }
  if (empty($targets['entrances_per_page_per_day_warning'])) {
    $targets['entrances_per_page_per_day_warning'] = $targets['value_per_page_per_day_warning'] / $targets['value_per_page_per_entrance_warning'];
  }
  return $targets;
}

function intel_get_base_scorings() {
  $scoring = array();
  $scoring['entrance'] = array(
    'title' => t('Entrances'),    
    'description' => t('A visit to the site.'),
    'value' => 0.05,
  );
  $scoring['stick'] = array(
    'title' => t('Sticks'),
    'description' => t('The visitor went to at least on other page or triggered a interaction event. Entrances - bounces'),
    'value' => 0.10,
  );
  $scoring['additional_pages'] = array(
    'title' => t('Additional pages'),
    'description' => t('Each additional page beyond the initial entrance page.'),
    'value' => 0.02,
  );
  return $scoring;
}

function intel_get_submission_goals_default() {
  $defs = array();
  $defs['tofu-conversion'] = array(
    'title' => t('ToFu conversion'),
    'description' => t('Top of the funnel conversion'),
    'value' => 15,
    'ga_id' => 1,
  );
  $defs['mofu-conversion'] = array(
    'title' => t('MoFu conversion'),
    'description' => t('Middle of the funnel conversion'),
    'value' => 50,
    'ga_id' => 2,
  );
  $defs['bofu-conversion'] = array(
    'title' => t('BoFu conversion'),
    'description' => t('Bottom of the funnel conversion'),
    'value' => 100,
    'ga_id' => 3,
  );
  $defs['subscribe-form'] = array(
    'title' => t('Subscribe form'),
    'description' => t('Subscribe to email updates form submission'),
    'value' => 25,
    'ga_id' => 4,
  ); 
  $defs['contact-form'] = array(
    'title' => t('Contact form'),
    'description' => t('Main contact form submission'),
    'value' => 25,
    'ga_id' => 5,
  );
  
  return $defs;
}

function intel_get_scorings($filter = '') {
  $scorings = &drupal_static(__FUNCTION__);
  if (isset($scorings[$filter])) {
    return $scorings[$filter];
  }
  if (!isset($scorings)) {
    $scorings = array();
  }
  if (!isset($scorings[$filter])) {
    $scorings[$filter] = array();
  }
  $custom = variable_get('intel_scorings', array());
  $base = intel_get_base_scorings();
  foreach ($base AS $i => $m) {
    if (($filter == 'js_setting') && (empty($m['js_setting']))) {
      continue;
    }
    $scorings[$filter][$i] = isset($custom[$i]) ? $custom[$i] : $m['value'];
  }
  
  $events = intel_get_intel_events();
  foreach ($events AS $i => $m) {
    if (($filter == 'js_setting') && (empty($m['js_setting']))) {
      continue;
    }
    if (!empty($m['valued_event'])) {
      $scorings[$filter][$i] = isset($custom[$i]) ? $custom[$i] : $m['value'];
    }
  }
  
  $goals = variable_get('intel_event_goals', array());
  $goals = array_merge($goals, variable_get('intel_submission_goals', intel_get_submission_goals_default()));
  foreach ($goals AS $i => $m) {
    if (($filter == 'js_setting') && (empty($m['js_setting']))) {
      continue;
    }
    $i = 'goal_' . $i;
    $scorings[$filter][$i] = isset($custom[$i]) ? $custom[$i] : (isset($m['value']) ? $m['value'] : 0);
  }

  return $scorings[$filter];
}

function intel_get_scoring($event, $mode = 'machinename') {
  $scoring = intel_get_scorings();
  
  return $scoring;
}

function intel_score_visit_aggregation($data, $divideby = 1, &$score_components = array(), $mode = '') {
  $scoring = intel_get_scorings();
  $scores = array(
    '_all' => 0,
    'events' => 0,
    'goals' => 0,
    'traffic' => 0, 
  );
  
  $entrance = array(
    'entrances' => 0,
  );
  if ($mode == 'social') {
    if (!empty($data['socialNetwork']['_all']['entrance'])) {
      $entrance = $data['socialNetwork']['_all']['entrance'];
    }
    elseif (!empty($data['socialNetwork']['entrance'])) { // for all rows
      $entrance = $data['socialNetwork']['entrance'];
    }
  }
  elseif ($mode == 'seo') {
    if (!empty($data['organicSearch']['_all']['entrance'])) {
      $entrance = $data['organicSearch']['_all']['entrance'];
    }
    elseif (!empty($data['organicSearch']['entrance'])) { // for all rows
      $entrance = $data['organicSearch']['entrance'];
    }
  }
  else {
    if (!empty($data['entrance'])) {
      $entrance = $data['entrance'];
    }
  }

  // entrance scoring
  if ($entrance['entrances']) {
    // the value of any goal achieved on any page during a visit that entererd through this page
    $scores['goals'] += $entrance['goalValueAll'];
    // the value of any goal achieved on any page during a visit that entererd through this page
    if (isset($entrance['events']['_all'])) {
      $scores['events'] += ($entrance['events']['_all']['value']); 
    }
    $scores['traffic'] += $entrance['entrances'] * $scoring['entrance'];
    if (!empty($entrance['sticks'])) {
      $scores['traffic'] += $entrance['sticks'] * $scoring['stick'];
    }
    if ($entrance['pageviews'] > ($entrance['entrances'] - $entrance['sticks'])) {
      $scores['traffic'] += ($entrance['pageviews'] - $entrance['entrances'] - $entrance['sticks']) * $scoring['additional_pages'];
    }
  } 
  
  foreach ($scores AS $i => $score) {
    if (substr($i, 0, 1) == '_') {
      continue;
    }      
    $scores[$i] = $score / $divideby;
    $scores['_all'] += $scores[$i];
  }
  
  $score_components = $scores;
  return $scores['_all'];
}

function intel_score_page_aggregation($data, $divideby = 1, &$score_components = array(), $mode = '') {
  $scoring = intel_get_scorings();
  $scores = array(
    '_all' => array(
      '_all' => 0,
      'events' => 0,
      'goals' => 0,
      'traffic' => 0, 
    ),
    'entrance' => array(
      '_all' => 0,
      'events' => 0,
      'goals' => 0,
      'traffic' => 0, 
    ),
    'assist' =>  array(
      '_all' => 0,
      'events' => 0,
      'goals' => 0, 
      'traffic' => 0,
    ),
  );
  $ac = .1; // assist coef
  $page = $data['pageview'];
  $entrance = array(
    'entrances' => 0,
  );
  if ($mode == 'social') {
    if (!empty($data['socialNetwork']['_all']['entrance'])) {
      $entrance = $data['socialNetwork']['_all']['entrance'];
    }
  }
  elseif ($mode == 'seo') {
    if (!empty($data['organicSearch']['_all']['entrance'])) {
      $entrance = $data['organicSearch']['_all']['entrance'];
    }
  }
  else {
    if (!empty($data['entrance'])) {
      $entrance = $data['entrance'];
    }
  }
  // entrance scoring
  if ($entrance['entrances']) {
    // the value of any goal achieved on any page during a entrance that entererd through this page    
    $scores['entrance']['goals'] += $entrance['goalValueAll'];
    $scores['_all']['goals'] += (1 - $ac) * $entrance['goalValueAll'];
    
    // the value of any goal achieved on any page during a entrance that entererd through this page
    if (isset($entrance['events']['_all'])) {      
      $scores['entrance']['events'] += $entrance['events']['_all']['value'];      
      $scores['_all']['events'] += $ac * ($entrance['events']['_all']['value'] - $page['events']['_all']['value']); 
    }
    $scores['entrance']['traffic'] += $entrance['entrances'] * $scoring['entrance'];
    $scores['_all']['traffic'] += $entrance['entrances'] * $scoring['entrance'];
    if (!empty($entrance['sticks'])) {
      $scores['entrance']['traffic'] += $entrance['sticks'] * $scoring['stick'];
      $scores['_all']['traffic'] += $entrance['sticks'] * $scoring['stick'];
    }
    if ($entrance['pageviews']['traffic'] > 2) {
      $scores['entrance']['traffic'] += ($entrance['pageviews'] - $entrance['entrances'] - $entrance['sticks']) * $scoring['additional_pages'];
      $scores['_all']['traffic'] += (1 - $ac) * ($entrance['pageviews'] - $entrance['entrances'] - $entrance['sticks']) * $scoring['additional_pages'];
    }
  } 

  // page hit scoring
  // the value of any goal achieved on any page during a entrance that entererd through this page
  if (!empty($entrance['goalValueAll'])) {
    $scores['assist']['goals'] += ($page['goalValueAll'] - $entrance['goalValueAll']);
    $scores['_all']['goals'] += $ac * ($page['goalValueAll'] - $entrance['goalValueAll']);
  }
  else {
    $scores['assist']['goals'] += $page['goalValueAll'];
    $scores['_all']['goals'] += $ac * $page['goalValueAll'];
  }
  // the value of any goal achieved on any page during a entrance that entererd through this page
  if (isset($page['events']['_all'])) {
    $scores['assist']['events'] += $page['events']['_all']['value']; 
    $scores['_all']['events'] += (1 - $ac) * $page['events']['_all']['value']; 
  }
  $scores['assist']['traffic'] += ($page['pageviews'] - $entrance['entrances']) * $scoring['additional_pages'];
  $scores['_all']['traffic'] += $ac * ($page['pageviews'] - $entrance['entrances']) * $scoring['additional_pages'];
  
  // do divide bys and total up alls
  foreach ($scores AS $i => $a) {
    foreach ($a AS $j => $b) {
      if (substr($j, 0, 1) == '_') {
        continue;
      }      
      $scores[$i][$j] = $scores[$i][$j] / $divideby;
      $scores[$i]['_all'] += $scores[$i][$j];
    }
  }

  $score_components = $scores;
  return $scores['_all']['_all']; 
}

function intel_ga_feed_request_callback($request, $args) {
  $cache_options = $args['cache_options'];
  $request['sort_metric'] = $request['sort'];
  unset($request['sort']);
  $feed = google_analytics_api_report_data($request, $cache_options);
  return $feed;
}

function intel_report_add_js_callback($script, $type = 'file') {
  if ($type == 'inline') {
    drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
  }
  else {
    $a = explode('//', $script);
    if ((count($a) == 2) && (substr($a[0], 0, 4) == 'http')) {
      drupal_add_js($script, array('type' => 'external', 'scope' => 'header'));
    }
    else {
      drupal_add_js(libraries_get_path('intel') . '/' . $script);
    }
  }
}

/**
 * TODO move to library
 * @param $vtk
 */
function intel_fetch_analytics_visitor_meta_data($vtkids) {
  intel_include_library_file('ga/class.ga_model.php');
  
  $visitor = array(
    'location' => array(),
    'environment' => array(),
    'lasthit' => 0,
  );
  
  list($start_date, $end_date, $number_of_days) = _intel_get_report_dates("-1 year", "Today");
  $cache = array(
    'refresh' => 1,
  );
  
  $segment = 'dynamic::ga:customVarValue5==' . implode(',dynamic::ga:customVarValue5==', $vtkids);  
  
  $request = array(
    'dimensions' => array('ga:browser', 'ga:browserVersion', 'ga:operatingSystem', 'ga:operatingSystemVersion', 'ga:language', 'ga:screenResolution', 'ga:customVarValue4'),
    'metrics' => array('ga:entrances'),
    'sort_metric' => '-ga:customVarValue4',
    'start_date' => $start_date,
    'end_date' => $end_date,
    'segment' => $segment,
    'max_results' => 1,
  );  
  
  $data = google_analytics_api_report_data($request, $cache);
  if (!empty($_GET['debug'])) {
    dpm('ga_request'); dpm($request);//
    dpm('ga_data'); dpm($data);//
  }
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      if ($ts > $visitor['lasthit']) {
        $visitor['lasthit'] = $ts;
      }
      $visitor['environment'] = array();
      $visitor['environment']['browser'] = $row['browser'];
      $visitor['environment']['browserVersion'] = $row['browserVersion'];
      $visitor['environment']['operatingSystem'] = $row['operatingSystem'];
      $visitor['environment']['operatingSystemVersion'] = $row['operatingSystemVersion'];
      $visitor['environment']['language'] = $row['language'];
      $visitor['environment']['screenResolution'] = $row['screenResolution'];
    }
  }
  
  $request['dimensions'] = array('ga:isMobile', 'ga:mobileDeviceBranding', 'ga:mobileDeviceModel', 'ga:mobileDeviceInfo', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 

  $data = google_analytics_api_report_data($request, $cache);

  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      $visitor['environment']['isMobile'] = $row['isMobile'];
      $visitor['environment']['mobileDeviceBranding'] = $row['mobileDeviceBranding'];
      $visitor['environment']['mobileDeviceModel'] = $row['mobileDeviceModel'];
      $visitor['environment']['mobileDeviceInfo'] = $row['mobileDeviceInfo'];
    }
  }  
  
  $request['dimensions'] = array('ga:country', 'ga:region', 'ga:city', 'ga:metro', 'ga:latitude', 'ga:longitude', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 

  $data = google_analytics_api_report_data($request, $cache);

  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $ts = (int)$row['customVarValue4'];
      if ($ts > $visitor['lasthit']) {
        $visitor['lasthit'] = $ts;
      }
      $visitor['location']['country'] = $row['country'];
      $visitor['location']['region'] = $row['region'];
      $visitor['location']['city'] = $row['city'];
      $visitor['location']['metro'] = $row['metro'];
      $visitor['location']['latitude'] = $row['latitude'];
      $visitor['location']['longitude'] = $row['longitude'];
    }
  }
  
  $visitor['visits'] = array(
    '_all' => array(
      'entrance' => array(
        'entrances' => 0,
        'pageviews' => 0,
        'timeOnSite' => 0,
      ),
    ),
  );
  $request['max_results'] = 10;

    
  $request['dimensions'] = array('ga:visitCount', 'ga:medium', 'ga:source', 'ga:referralPath', 'ga:keyword', 'ga:socialNetwork', 'ga:customVarValue4');
  $request['metrics'] = array('ga:entrances'); 
  $request['sort_metric'] = '-ga:customVarValue4';  // note: cant sort by visitCount as it is treated as a string not an int

  $data = google_analytics_api_report_data($request, $cache);

  $visitCountMin = 0;
  $visitCountMax = 0;
  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $visitCountMin = $i = (int)$row['visitCount'];
      if (!$visitCountMax) {
        $visitCountMax = $i;
      }
      $visitor['visits'][$i] = array(
        'trafficsource' => array(),
      );
      $visitor['visits'][$i]['visitCount'] = $row['visitCount'];
      $visitor['visits'][$i]['time'] = (int)$row['customVarValue4'];
      $visitor['visits'][$i]['trafficsource']['medium'] = $row['medium'];
      $visitor['visits'][$i]['trafficsource']['source'] = $row['source'];
      $visitor['visits'][$i]['trafficsource']['referralPath'] = $row['referralPath'];
      $visitor['visits'][$i]['trafficsource']['keyword'] = $row['keyword'];
      $visitor['visits'][$i]['trafficsource']['socialNetwork'] = $row['socialNetwork'];
      //$visitor['visits'][$ts]['trafficsource']['campaign'] = $row['campaign'];
    }
  }
  $request['dimensions'] = array('ga:visitCount', 'ga:campaign');
  $request['metrics'] = array('ga:pageviews', 'ga:bounces', 'ga:timeOnSite', 'ga:goalValueAll');
  $request['sort_metric'] = ''; 
  $visitCountFilter = ($visitCountMin > 1) ? LevelTen\Intel\GAModel::formatGtRegexFilter('ga:visitCount', ($visitCountMin-1)) : '';
  $request['filters'] = $visitCountFilter;

  $data = google_analytics_api_report_data($request, $cache);

  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $i = (int)$row['visitCount'];
      $visitor['visits'][$i]['entrance'] = array();
      $visitor['visits'][$i]['entrance']['entrances'] = 1;
      $visitor['visits'][$i]['entrance']['pageviews'] = $row['pageviews'];
      $visitor['visits'][$i]['entrance']['sticks'] = (1 - $row['bounces']);
      $visitor['visits'][$i]['entrance']['timeOnSite'] = $row['timeOnSite'];
      $visitor['visits'][$i]['entrance']['goalValueAll'] = $row['goalValueAll'];
      $visitor['visits'][$i]['trafficsource']['campaign'] = $row['campaign'];
      
      $visitor['visits']['_all']['entrance']['entrances'] ++;
      $visitor['visits']['_all']['entrance']['pageviews'] += $row['pageviews'];
      $visitor['visits']['_all']['entrance']['timeOnSite'] += $row['timeOnSite'];
    }
  }
  $visitor['visits']['_all']['visitCount'] = $visitCountMax;
  
  // get valued events
  $request['dimensions'] = array('ga:visitCount');
  $request['metrics'] = array('ga:totalEvents', 'ga:uniqueEvents', 'ga:eventValue'); 
  $request['filters'] = 'ga:eventCategory=~^*!$' . (($visitCountFilter) ? ';' . $visitCountFilter : '');

  $data = google_analytics_api_report_data($request, $cache);

  if (!empty($data->results) && is_array($data->results)) {
    foreach ($data->results AS $row) {
      $i = (int)$row['visitCount'];
      $visitor['visits'][$i]['entrance']['events'] = array(
        '_all' => array(
          'value' => $row['eventValue'],
          'totalValuedEvents' => $row['totalEvents'],
          'uniqueValuedEvents' => $row['uniqueEvents'],
        ),
      );
    }
  }

  $score_components = '';
  $score = 0;
  foreach ($visitor['visits'] AS $index => $visit) {
    if (substr($index, 0, 1) == '_') {
      continue;
    }
    $visitor['visits'][$index]['score'] = intel_score_visit_aggregation($visitor['visits'][$index], 1, $score_components);
    $score += $visitor['visits'][$index]['score']; 
    //$visitor['visits'][$index]['score_components'] = $score_components;  
  }
  $visitor['visits']['_all']['score'] = $score;
  
  if ($visitor['lasthit']) {
    return $visitor;
  }
  else {
    return FALSE;
  }
}

function _intel_get_report_dates_from_ops($ops = 'l30d', &$cache_options = array(), $return_hash = FALSE) {
  $start = '';
  $end = '';
  if (!empty($_GET['timeops'])) {
    $ops = $_GET['timeops'];
  }
  
  if ($ops == 'today') {
    $start = 'midnight';
    $end = 'now';
    $cache_options = array('refresh' => 1);
  }
  elseif (($ops == 'l24') || ($ops == 'l24fn')) {
    $start = '-24 hours';
    $end = 'now';
    $cache_options = array('refresh' => 1);
  }
  elseif ($ops == 'yesterday') {
    $start = '-1 day midnight';
    $end = "midnight - 1 second";
  }
  elseif ($ops == 'l7dfn') {
    $start = "-7 days";
    $end = "now";
    $cache_options = array('refresh' => 1);
  }
  elseif ($ops == 'l7d') {
    $start = "-7 days midnight";
    $end = "midnight - 1 second";
    $cache_options = array('refresh' => 1);
  }
  elseif ($ops == 'l30dfn') {
    $start = "-30 days";
    $end = "now";
    $cache_options = array('refresh' => 1);
  }
  elseif ($ops == 'thismonth') {
    $start = "midnight first day of this month";
    $end = "midnight first day of next month - 1 second";
  }
  elseif ($ops == 'lastmonth') {
    $start = "midnight first day of last month";
    $end = "midnight first day of this month - 1 second";
  }
  elseif ($ops == 'monthtodate') {
    $start = "midnight first day of this month";
    $end = "now";
  }
  else  {  // "l30d" last 30 days from yesterday
    $start = "-30 days midnight";
    $end = "midnight - 1 second";
  }
  return _intel_get_report_dates($start, $end);
}

function _intel_get_report_dates($start_default = "-31 days", $end_default = "-1 day", $return_hash = FALSE) {
  if (!empty($_GET['dates'])) {
    $a = explode(":", $_GET['dates']);
    $_GET['start_date'] = $a[0];
    $_GET['end_date'] = $a[1];
  }
  $start_date = (!empty($_GET['start_date'])) ? strtotime($_GET['start_date']) : strtotime($start_default);
  $end_date = (!empty($_GET['end_date'])) ? strtotime($_GET['end_date']) : strtotime($end_default);
  $number_of_days = round(($end_date - $start_date)/60/60/24);  
  if (!$return_hash) {
    return array(
      $start_date,
      $end_date,
      $number_of_days,
    );
  }
  else {
    return array(
      'start_date' => $start_date,
      'end_date' => $end_date,
      'number_of_days' => $number_of_days,
    );
  }
}

function intel_report_filters_form($form, &$form_state, $filters = array(), $context = 'site') {
  $dynamic_filters = array();
  $static_filter = '';
  $form['filters'] = array(
    '#type' => 'fieldset', 
    '#title' => t('Filters'), 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,  
  );
  $events = array(
    '' => t('-'),
  );
  $metas = intel_get_intel_events();
  foreach ($metas AS $meta) {
    if (isset($meta['category'])) {
      $events[$meta['category']] = $meta['title'];
    }
  }
  
  if ($context != 'page') {
    $modes = array(
      '' => '-',
      'landingPagePath' => t('Entrance'),
      'pagePath' => t('View'),
    );
    $mode_default = 'entrance';
    $path_default = '';
    if (!empty($filters['page'])) {
      $a = explode(':', $filters['page']);
      $mode_default = $a[0];
      $path_default = $a[1];
      $dynamic_filters['page'] = "{$a[0]}={$a[1]}";
    }  
    $form['filters']['page_mode'] = array(
      '#type' => 'select',
      '#title' => t('Page mode'),
      '#id' => 'page-mode',
      '#options' => $modes,
      '#default_value' => $mode_default,
      '#prefix' => '<div class="filter-fieldset filter-fieldset-page">',
    );
    $form['filters']['page_path'] = array(
      '#type' => 'textfield',
      '#title' => t('Page path'),
      '#id' => 'page-path',
      '#size' => 80,
      '#default_value' => $path_default,
      '#suffix' => '</div>',
    );
  }
  
  $form['filters']['event'] = array(
    '#type' => 'select',
    '#title' => t('Events'),
    '#id' => 'event-filter',
    '#options' => $events,
    '#default_value' => !empty($filters['event']) ? $filters['event'] : '',
    '#prefix' => '<div class="filter-fieldset filter-fieldset-event">',
    '#suffix' => '</div>',
  );
  
  if ($context != 'trafficsource') {
    $trafficsource_types = array(
      '' => t('-'),
      'source' => t('Source'),
      'medium' => t('Medium'),
      'referralPath' => t('Referral path'),
      'socialNetwork' => t('Social network'),
      'keyword' => t('Keyword'),    
    );
    $trafficsource_type_default = '';
    $trafficsource_value_default = '';
    if (!empty($filters['trafficsource'])) {
      $a = explode(':', $filters['trafficsource']);
      $trafficsource_type_default = $a[0];
      $trafficsource_value_default = $a[1];
      $dynamic_filters['trafficsource'] = "{$a[0]}={$a[1]}";
    }
    $form['filters']['trafficsource_type'] = array(
      '#type' => 'select',
      '#title' => t('Traffic source type'),
      '#id' => 'trafficsource-type',
      '#options' => $trafficsource_types,
      '#default_value' => $trafficsource_type_default,
      '#prefix' => '<div class="filter-fieldset filter-fieldset-referrer">',
    );
    $form['filters']['trafficsource_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Traffic source value'),
      '#id' => 'trafficsource-value',
      '#size' => 80,
      '#default_value' => $trafficsource_value_default,
      '#suffix' => '</div>',
    );
  }
  
  $loc_types = array(
    '' => t('-'),
    'subContinent' => t('Sub Continent'),
    'country' => t('Country'),
    'region' => t('Region / state'),
    'city' => t('City'),
    'metro' => t('Metro'),
  );
  $loc_type_default = '';
  $loc_value_default = '';
  if (!empty($filters['location'])) {
    $a = explode(':', $filters['location']);
    $loc_type_default = $a[0];
    $loc_value_default = $a[1];
    $dynamic_filters['location'] = "{$a[0]}={$a[1]}";
  }
  $form['filters']['location_type'] = array(
    '#type' => 'select',
    '#title' => t('Location type'),
    '#id' => 'location-type',
    '#options' => $loc_types,
    '#default_value' => $loc_type_default,
    '#prefix' => '<div class="filter-fieldset filter-fieldset-location">',
  );
  $form['filters']['loc_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Location value'),
    '#id' => 'location-value',
    '#size' => 80,
    '#default_value' => $loc_value_default,
    '#suffix' => '</div>',
  );
  
  if ($context != 'visitor') {
    $visitor_options = array(
      '' => t('-'),
      'vtkid' => t('By visitor token'),
      'email' => t('By email address'),
    );
    $visitor_type_default = '';
    $visitor_value_default = '';
    if (!empty($filters['visitor'])) {
      $a = explode(':', $filters['visitor']);
      $visitor_type_default = $a[0];
      $visitor_value_default = $a[1];
      $dynamic_filters['visitor'] = "{$a[0]}={$a[1]}";
    }
    $form['filters']['visitor_type'] = array(
      '#type' => 'select',
      '#title' => t('Visitor'),
      '#id' => 'visitor-type',
      '#options' => $visitor_options,
      '#default_value' => $visitor_type_default,
      '#prefix' => '<div class="filter-fieldset filter-fieldset-visitor">',
    );
    $form['filters']['visitor_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Visitor value'),
      '#id' => 'visitor-value',
      '#size' => 80,
      '#default_value' => $visitor_value_default,
      '#suffix' => '</div>',
    ); 
    
    $va_options = array(
      '' => t('-'),
    );
    $va_options = array_merge($va_options, intel_get_field_visitor_attribute_allowed_values());
    $va_type_default = '';
    $va_value_default = '';
    if (!empty($filters['visitor-attr'])) {
      $a = explode(':', $filters['visitor-attr']);
      $va_type_default = $a[0];
      $va_value_default = $a[1];
      $dynamic_filters['visitor-attr'] = "{$a[0]}={$a[1]}";
    }
    $form['filters']['visitor_attr_type'] = array(
      '#type' => 'select',
      '#title' => t('Visitor attribute'),
      '#id' => 'visitor-attr-type',
      '#options' => $va_options,
      '#default_value' => $va_type_default,
      '#prefix' => '<div class="filter-fieldset filter-fieldset-visitor-attr">',
    );
    $form['filters']['visitor_attr_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Visitor attribute value'),
      '#id' => 'visitor-attr-value',
      '#size' => 80,
      '#default_value' => $va_value_default,
      '#suffix' => '</div>',
    ); 
  } 

  $form['filters']['apply'] = array(
    '#type' => 'markup',
    '#markup' => '<input type="button" id="apply-report-filter" value="Apply" class="form-submit">',
  );
  
  // do not display form if not in extended mode
  if (!variable_get('intel_extended_mode', 0)) {
    $form = array();
  }
  
  if (($context == 'page') && ($filters['page'])) {
    $a = explode(':', $filters['page']);
    $form['filters_display'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>' . t('Page') . '</strong>' . ': ' . $a[1],
    );    
  }
  if (($context == 'trafficsource') && ($filters['trafficsource'])) {
    $a = explode(':', $filters['trafficsource']);
    $form['filters_display'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>' . t('Traffic source') . '</strong>' . ': ' . $a[1] . " ({$a[0]})",
    );    
  }
  if (($context == 'visitor') && ($filters['visitor'])) {
    $a = explode(':', $filters['visitor']);
    $form['filters_display'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>' . t('Visitor') . '</strong>' . ': ' . $a[1],
    );    
  }
  if (count($dynamic_filters)) {
    $form['filters_display'] = array(
      '#type' => 'markup',
      '#markup' => '<strong>' . t('Filters') . '</strong>' . ': ' . implode(', ', $filters_display),
    );
  }
  return $form;
}

function intel_add_report_headers() {
  drupal_add_js('https://www.google.com/jsapi', array('type' => 'external', 'scope' => 'header'));
  drupal_add_js("google.load('visualization', '1', {packages: ['corechart']});google.load('visualization', '1', {packages: ['table']});", array('type' => 'inline', 'scope' => 'header'));
  drupal_add_js(libraries_get_path('LevelTen') . '/Intel/charts/charts.js');
  drupal_add_css(drupal_get_path('module', 'intel') . '/css/intel.report.css');
  drupal_add_js(drupal_get_path('module', 'intel') . '/js/intel.report.js'); 
}

function intel_get_report_ajax_container() {
  $output = '<div id="intel-report-container" data-q="' . $_GET['q'] . '" data-refresh="' . ((!empty($_GET['refresh'])) ? 1 : 0) . '"><div class="loader"><img src="/' . drupal_get_path('module', 'intel') . '/images/ajax_loader_report.gif' . '" alt="loading LevelTen Intelligence"><br>Gathering Intelligence...</div></div>';
  return $output;
}