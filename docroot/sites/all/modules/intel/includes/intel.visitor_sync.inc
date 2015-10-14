<?php
/**
 * @file
 * Support for syncing visitor data accross various data sources
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

function intel_sync_visitordata($visitor, $request = array()) {  
if (!empty($_GET['debug'])) {
  dpm('visitor0');//
  dpm($visitor);//
}
  $l10api_synced = FALSE;
  $statuses = array();

  // don't sync data if no pro api
  $api_level = variable_get('intel_api_level', '');
  if ($api_level != 'pro') {
    $statuses['na'] = 1;
    return $statuses;
  }

  // sync google analytics data
  $statuses['intel_ga'] = intel_ga_sync_visitordata($visitor);

  // if email is set, run l10iapi sync
  if (isset($visitor->email)) {
    $statuses['intel_api'] = intel_l10iapi_sync_visitordata($visitor);
    $l10api_synced = TRUE;
  }

  foreach (module_implements('intel_sync_visitordata') AS $module) {
    $function = $module . '_intel_sync_visitordata';
    $statuses[$module] = $function($visitor, $request);
  }

  if (!$l10api_synced && isset($visitor->email)) {
    $statuses['intel_api'] = intel_l10iapi_sync_visitordata($visitor);
  }

  return $statuses;
}

function intel_work_sync_visitor_request_queue($runtime = 15) {
  intel_queue_sync_visitor_requests();

  $end = time() + $runtime;
  $queue = DrupalQueue::get('intel_sync_visitor_requests');
  while (time() < $end && ($item = $queue->claimItem())) {
    if (!empty($_GET['debug'])) {
      dpm('syncing from queue:');//
      dpm($item);//
    }
    intel_sync_visitor_request_worker($item->data);
    $queue->deleteItem($item);
  }
}

function intel_queue_sync_visitor_requests() {
  $items = variable_get('intel_sync_visitor_requests', array());
  if (count($items)) {
    $queue = DrupalQueue::get('intel_sync_visitor_requests');
    foreach ($items AS $i => $item) {
      if ($item->run_after < REQUEST_TIME) {
        $queue->createItem($item);
        unset($items[$i]);
      }
    }
    variable_set('intel_sync_visitor_requests', $items);  
  }   
}



/**
 * Implements intel_sync_visitordata
 * @param unknown_type $visitor
 */
function intel_ga_sync_visitordata(&$visitor) {
  require_once drupal_get_path('module', 'intel') . "/includes/intel.ga.inc";
  $vtkids = array();

  foreach ($visitor->identifiers['vtk'] AS $vtk) {
    $vtkids[] = substr($vtk, 0, 20);
  }
  $gadata = intel_fetch_analytics_visitor_meta_data($vtkids);
  if (!empty($_GET['debug'])) {
    dpm('gadata'); dpm($gadata);//
  }
  if (empty($gadata)) {
    return FALSE;
  }
  if (isset($gadata['location'])) {
    $addthis_geo = $visitor->getVar('api_visitor', 'addthis', 'geo');
    if ($addthis_geo) {
      $gadata['location']['country_code'] = isset($addthis_geo['country']) ? $addthis_geo['country'] : '';
      $gadata['location']['region_code'] = isset($addthis_geo['region']) ? $addthis_geo['region'] : '';
      $gadata['location']['zip'] = isset($addthis_geo['zip']) ? $addthis_geo['zip'] : '';
    }
  }
  
  $visitor->setVar('data', 'location', $gadata['location']);
  $visitor->setVar('data', 'environment', $gadata['environment']);
  $visitor->setVar('data', 'analytics_visits', $gadata['visits']);
  $visitor->setVar('ext', 'ga', $gadata);
  $visitor->merge();

  return TRUE;
}

function intel_calculate_distance($lat1, $lon1, $lat2, $lon2, $miles = TRUE) {
  $pi80 = M_PI / 180;
  $lat1 *= $pi80;
  $lon1 *= $pi80;
  $lat2 *= $pi80;
  $lon2 *= $pi80;
 
  $r = 6372.797; // mean radius of Earth in km
  $dlat = $lat2 - $lat1;
  $dlon = $lon2 - $lon1;
  $a = sin($dlat / 2) * sin($dlat / 2) + cos($lat1) * cos($lat2) * sin($dlon / 2) * sin($dlon / 2);
  $c = 2 * atan2(sqrt($a), sqrt(1 - $a));
  $km = $r * $c;
 
  return ($miles ? ($km * 0.621371192) : $km);
}

function intel_sync_fullcontact_page($vtk) {
  $visitor = intel_visitor_load($vtk, 1);
  $status = intel_l10iapi_sync_visitordata($visitor);
  
  $output = t('status:') . ' ' . $status;
  
  return $output;
}

function intel_l10iapi_sync_visitordata(&$visitor) {
  $status = TRUE;
  if (
    intel_api_access()
    && variable_get('intel_sync_visitordata_fullcontact', INTEL_SYNC_VISITORDATA_FULLCONTACT_DEFAULT)
    && $visitor->getVar('data', 'settings', 'sync_visitordata.fullcontact', INTEL_SYNC_VISITORDATA_FULLCONTACT_DEFAULT)
  ) {
    $person = $visitor->apiPersonLoad();
    $fc_data = $visitor->getVar('api_person_fullcontact');

    if (!empty($_GET['debug'])) {
      dpm('FullContact person:'); dpm($fc_data);//
    }
  
    if (!empty($fc_data)) {
      intel_sync_fullcontact_visitordata($visitor, $fc_data);
      $visitor->setVar('ext', 'fullcontact', $fc_data);
      $visitor->merge();
      // if fullcontact data is queued for search, keep sync request
      if ($fc_data['status'] == 202) {
        $status = FALSE;
      }
    }
  }
  return $status;
}

function intel_sync_fullcontact_visitordata(&$visitor, $data) {
  if (empty($data['status']) || ($data['status'] != 200)) {
    return TRUE;
  }
  // only use data if likelihood of match is above threshold
  if ($data['likelihood'] < .80) {
    return TRUE;
  }
  $prop_options = array(
    'source' => 'fullcontact',
  );
  $bio = '';
  $klout = '';
  if ($data['photos'] && is_array($data['photos'])) {
    foreach ($data['photos'] AS $i => $v) {
      if ($v['isPrimary']) {
        $visitor->setProp('image', $v, $prop_options);
      }
    }
  }
  if ($data['socialProfiles'] && is_array($data['socialProfiles'])) {
    foreach ($data['socialProfiles'] AS $i => $v) {
      if ($v['type'] == 'aboutme') {
        $visitor->setProp('aboutme', $v, $prop_options);
      }
      else if ($v['type'] == 'facebook') {
        $visitor->setProp('facebook', $v, $prop_options);
      }
      else if ($v['type'] == 'flickr') {
        $visitor->setProp('flickr', $v, $prop_options);
      }
      else if ($v['type'] == 'foursquare') {
        $visitor->setProp('foursquare', $v, $prop_options);
      }
      else if ($v['type'] == 'friendfeed') {
        $visitor->setProp('friendfeed', $v, $prop_options);
      }
      else if ($v['type'] == 'googleplus') {
        $visitor->setProp('googleplus', $v, $prop_options);
      }
      else if ($v['type'] == 'googleprofile') {
        $visitor->setProp('googleprofile', $v, $prop_options);
      }
      else if ($v['type'] == 'gravatar') {
        $visitor->setProp('gravatar', $v, $prop_options);
      }
      else if ($v['type'] == 'klout') {        
        //intel_visitor_property_set($visitor, 'data.klout', $v, $prop_options);
        $klout = $v;
      }
      else if ($v['type'] == 'lanyrd') {
        $visitor->setProp('lanyrd', $v, $prop_options);
      }
      else if ($v['type'] == 'linkedin') {
        $visitor->setProp('linkedin', $v, $prop_options);
      }
      else if ($v['type'] == 'myspace') {
        $visitor->setProp('myspace', $v, $prop_options);
      }
      else if ($v['type'] == 'picasa') {
        $visitor->setProp('picasa', $v, $prop_options);
      }
      else if ($v['type'] == 'tumblr') {
        $visitor->setProp('tumblr', $v, $prop_options);
      }
      else if ($v['type'] == 'twitter') {        
        $visitor->setProp('twitter', $v, $prop_options);
        if ($v['bio']) {
          $bio = $v['bio'];
        }
      }
      else if ($v['type'] == 'vimeo') {
        $visitor->setProp('vimeo', $v, $prop_options);
      }
      else if ($v['type'] == 'yahoo') {
        $visitor->setProp('yahoo', $v, $prop_options);
      }
      else if ($v['type'] == 'youtube') {
        $visitor->setProp('youtube', $v, $prop_options);
      }
    }
  }
  if (is_array($klout) && !empty($klout['id'])) {
    $klout['topics'] = array();
    if (isset($data['digitalFootprint']) && isset($data['digitalFootprint']['scores']) && is_array($data['digitalFootprint']['scores'])) {
      foreach ($data['digitalFootprint']['scores'] AS $i => $v) {
        if ($v['provider'] == 'klout') {
          $klout['score'] = $v['value'];
        }
      }
    }
    if (isset($data['digitalFootprint']) && isset($data['digitalFootprint']['topics']) && is_array($data['digitalFootprint']['topics'])) {
      foreach ($data['digitalFootprint']['topics'] AS $i => $v) {
        if ($v['provider'] == 'klout') {
          $klout['topics'][] = $v['value'];
        }
      }
    }
    $visitor->setProp('klout', $klout, $prop_options);
  }
  if ($bio) {
    $v = array(
      'summary' => $bio,
    );
    $visitor->setProp('bio', $v, $prop_options);
  }
  
}