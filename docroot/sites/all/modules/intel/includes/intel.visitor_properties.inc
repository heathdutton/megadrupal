<?php
/**
 * @file
 * Support for syncing visitor data accross various data sources
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */

function _intel_intel_visitor_property_info() {
  $prop = array();
  // identity properties
  $prop['data.vtk'] = array(
    'title' => t('Visitor token'),
    'category' => 'identity',
    'variables' => array(
      'vtk' => NULL,
      'vtkid' => NULL,
      'vtkc' => NULL,
    ),    
  );  
  $prop['data.age'] = array(
    'title' => t('Age'),
    'category' => 'identity',
    'variables' => array(
      'age' => NULL,
      'rangeLow' => NULL,
      'rangeHigh' => NULL,
    ),
  );
  $prop['data.bio'] = array(
    'title' => t('Bio'),
    'category' => 'identity',
    'variables' => array(
      'summary' => NULL,
      'full' => NULL,
    ),
  );
  $prop['data.gender'] = array(
    'title' => t('Gender'),
    'category' => 'identity',
    'variables' => array(
      'gender' => NULL,
    ),
  );
  $prop['data.image'] = array(
    'title' => t('Image'),
    'category' => 'identity',
    'variables' => array(
      'url' => NULL,
      'alt' => NULL,
      'title' => NULL,
    ),
  );
  $prop['data.name'] = array(
    'title' => t('Name'),
    'category' => 'identity',
    'variables' => array(
      'full' => NULL,
      'first' => NULL,
      'last' => NULL,
    ),
  );
  // contact points
  $prop['data.email'] = array(
    'title' => t('Email'),
    'category' => 'contact',
    'variables' => array(
      'email' => NULL,
    ),
  );
  $prop['data.phone'] = array(
    'title' => t('Phone'),
    'category' => 'contact',
    'variables' => array(
      'phone' => NULL,
    ),
  );
  $prop['data.website'] = array(
    'title' => t('Website'),
    'category' => 'contact',
    'variables' => array(
      'url' => NULL,
    ),
  );    
  
  // social properties
  $prop['data.aboutme'] = array(
    'title' => t('About.me'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );

  $prop['data.facebook'] = array(
    'title' => t('Facebook'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
      'username' => NULL,
    ),
  );
  $prop['data.flickr'] = array(
    'title' => t('Flickr'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
      'username' => NULL,
    ),
  );
  $prop['data.friendfeed'] = array(
    'title' => t('Friendfeed'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
      'username' => NULL,
    ),
  );
  $prop['data.foursquare'] = array(
    'title' => t('Foursquare'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
    ),
  );
  $prop['data.googleplus'] = array(
    'title' => t('Google+'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
    ),
  );
  $prop['data.googleprofile'] = array(
    'title' => t('Google Profile'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
    ),
  );
  $prop['data.gravatar'] = array(
    'title' => t('Gravatar'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'username' => NULL,
    ),
  );

  $prop['data.klout'] = array(
    'title' => t('Klout'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'id' => NULL,
      'username' => NULL,
      'score' => NULL,
      'topics' => array(),
    ),
  );
  $prop['data.lanyrd'] = array(
    'title' => t('Lanyrd'),
    'variables' => array(
      'url' => NULL,
    ),
  );
  $prop['data.linkedin'] = array(
    'title' => t('LinkedIn'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );
  $prop['data.myspace'] = array(
    'title' => t('Myspace'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );
  $prop['data.picasa'] = array(
    'title' => t('Picasa'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );

  $prop['data.tumblr'] = array(
    'title' => t('tumblr'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'username' => NULL,
    ),
  );
  $prop['data.twitter'] = array(
    'title' => t('Twitter'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'username' => NULL,
      'followers' => NULL,
      'following' => NULL,
    ),
    'key' => 'username',
    'validate' => 'intel_visitor_property_twiter_validate'
  );
  $prop['data.vimeo'] = array(
    'title' => t('Vimeo'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );
  $prop['data.yahoo'] = array(
    'title' => t('Yahoo!'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
      'username' => NULL,
    ),
  );
  $prop['data.youtube'] = array(
    'title' => t('YouTube'),
    'category' => 'social',
    'variables' => array(
      'url' => NULL,
    ),
  );
  $prop = intel_add_icons_to_visitor_properties($prop);
  return $prop;
}

function intel_visitor_property_select_options() {
  $prop_info = intel_get_visitor_property_info_all();
  $options = array();
  $options[''] = ' - ' . t('None') . '';
  foreach ($prop_info AS $ns => $pi) {
    foreach ($pi['variables'] AS $key => $default) {
      $options["$ns:$key"] = $pi['title'] . " ($key)";
    }
  }
  return $options;
}

function intel_add_icons_to_visitor_properties($props) {
  $available = array(
    'data.facebook' => 'facebook',
    'data.flickr' => 'flickr',
    'data.foursquare' => 'foursquare',
    'data.friendfeed' => 'friendfeed',
    'data.googleplus' => 'googleplus',
    'data.googleprofile' => 'googleprofile',
    'data.gravatar' => 'gravatar',  
    'data.klout' => 'klout',  
    'data.lanyrd' => 'lanyrd',
    'data.linkedin' => 'linkedin',
    'data.myspace' => 'myspace',
    'data.picasa' => 'picasa',
    'data.tumblr' => 'tumblr',
    'data.twitter' => 'twitter',    
    'data.vimeo' => 'vimeo',
    'data.yahoo' => 'yahoo',
    'data.youtube' => 'youtube',
  );
  //$available = array(
  //  'data.facebook' => 'facebook',
  //);
  $intel_icon_urls = variable_get('intel_icon_urls', array());
  $icon_base = intel_get_icon_params();
  $save_icon_urls = 0;
  foreach ($props AS $id => $value) {
    // check if prop does not have icon
    if (!isset($available[$id])) {
      continue;
    }

    $icon_name = $available[$id];
    if (empty($intel_icon_urls[$icon_name])) {
      $icon = clone $icon_base;
      $icon->name = $icon_name;
      if ($icon = intel_create_icon($icon)) {
        $intel_icon_urls[$icon_name] = $icon->url;
        $save_icon_urls = 1;
      }
    }
    if (!empty($intel_icon_urls[$icon_name])) {
      $props[$id]['icon'] = $intel_icon_urls[$icon_name];
    }
  }
  if ($save_icon_urls) {
    variable_set('intel_icon_urls', $intel_icon_urls);
  }
  return $props;
}

function intel_create_icon($icon) {
  if (!intel_api_access()) {
    return '';
  }
  $url = '';
  $apiUrl = variable_get('intel_l10iapi_url', '');
  $api_params = variable_get('intel_l10iapi_custom_params', array());
  $apiClientProps = array(
    'apiUrl' => ($apiUrl ? $apiUrl : INTEL_L10IAPI_URL) . '/',
    'apiConnector' => variable_get('intel_l10iapi_connector', ''),
  );

  if (!intel_include_library_file('class.apiclient.php')) {
    return FALSE;
  }
  $apiclient = new \LevelTen\Intel\ApiClient($apiClientProps);
  $ret = array();
  $params = (array)$icon;
  $params['tid'] = variable_get('intel_ga_account', '');
  $data = array(
    'apikey' => variable_get('intel_apikey', '')
  );
  try {
    $ret = (array) $apiclient->getJSON('icon/load', $params, $data);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'warning');
  }

  if (empty($ret['icon'])) {
    return FALSE;      
  }
  // if api returns an object, convert to array
  if (is_object($ret['icon'])) {
    $ret['icon'] = (array) $ret['icon'];
  }
  if (!isset($ret['icon']['url'])) {
    return FALSE;
  }

  $file = $ret['icon']['url'];
  
  $ext = 'png';
  
  $uri = "public://intel/icons/{$icon->set}/{$icon->style}/{$icon->size}";

  $ret = file_prepare_directory($uri, FILE_CREATE_DIRECTORY);
  if (!$ret) {
    return FALSE;
  }
  $dest = $uri . "/{$icon->name}.$ext";

  $data = file_get_contents($file);
  if (!$data) {
    return FALSE;
  }
  $ret = file_save_data($data, $dest, FILE_EXISTS_REPLACE);
  $icon->url = $dest;
  return $icon;
}

function intel_get_icon_params() {
  $params = (object)array(
    'set' => 'fullcontact',
    'style' => 'default',
    'size' => 32,
  );
  return $params;
}

function intel_visitor_property_twiter_validate(&$prop, $visitor) {
  $key = (!empty($prop['username'])) ? $prop['username'] : '';
  if (empty($prop['url'])) {
    $prop['url'] = 'http://twitter.com/' . $prop['username'];
  }
}