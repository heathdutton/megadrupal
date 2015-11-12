<?php

/**
 * @file
 * Helper functions for reports
 * 
 * @author Tom McCracken <tomm@getlevelten.com>
 */


function theme_intel_visitor_profile($vars) {
  drupal_add_css(drupal_get_path('module', 'intel') . "/css/intel.visitor_profile.css");
  $output = '';
  $output .= '<div id="intel-profile">';
  $sidebar_items = 0;
  // start sidebar
  if (isset($vars['sidebar']) && count($vars['sidebar'])) {
    $sidebar_items = count($vars['sidebar']);
    $output .= '<div id="intel-profile-sidebar" class="sidebar">';
    
    foreach ($vars['sidebar'] AS $id => $elem) {
      $elem['#id'] = $id;
      $output .= render($elem);
    }
    $output .= '</div>';
    // end sidebar
  }
  // start main
  if ($sidebar_items) {
    $output .= '<div id="intel-profile-main" class="main">';
  }
  
  $output .= '<div id="intel-profile-main-header" class="main main-header">';
  $output .= '<h1>' . $vars['h1'] . '</h1>';
  if (isset($vars['h2'])) {
    $output .= '<h2>' . $vars['h2'] . '</h2>';
  }
  if (isset($vars['social-icons'])) {
    $output .= $vars['social-icons'];
  }
  $output .= '<div id="intel-profile-main-header-content" class="main main-header main-header-content">';
  foreach ($vars['header'] AS $id => $elem) {
    $elem['#id'] = $id;
    $output .= render($elem);
  }
  $output .= '</div>';
  $output .= '</div>';
  $output .= '<div id="intel-profile-main-body" class="main main-body">';
  foreach ($vars['main'] AS $id => $elem) {
    $elem['#id'] = $id;
    $output .= render($elem);
    //$output .= theme($elem['#theme'], $elem['vars']);
  }
  $output .= '</div>';
  
  if ($sidebar_items) {
    $output .= '</div>';
  }  
  
  // end main
  $output .= '</div>';
  
  return $output;
}

function theme_intel_visitor_profile2($vars) {
  drupal_add_css(drupal_get_path('module', 'intel') . "/css/intel.visitor_profile.css");
  $output = '';
  $output .= '<div id="intel-profile">';
  $sidebar_items = 0;
  $output .= '<div id="profile-header">';
  
  $output .= render($vars['image']);
  
  $output .= '<div id="profile-header-main">';  
  $output .= '<h1>' . $vars['h1'] . '</h1>';
  if (isset($vars['h2'])) {
    $output .= '<h2>' . $vars['h2'] . '</h2>';
  }

  if (isset($vars['social-icons'])) {
    $output .= $vars['social-icons'];
  }
  
  if (isset($vars['bio'])) {
    $output .= '<div id="intel-profile-bio">'; 
    $output .= $vars['bio'];
    $output .= '</div>';
  }
  $output .= '</div>';  // end #profile-header-main  
  
  $output .= '</div>';  // end #profile-header 
  
  
  // key metrics
  if (!empty($vars['key-metrics']) && is_array($vars['key-metrics']) ) {
    $output .= '<div id="key-metrics">'; 
    foreach ($vars['key-metrics'] AS $metric) {
      $output .= theme_intel_key_metric($metric);
    }
    $output .= '</div>';  // end #key-metrics
  }
  
  $output .= '<div id="intel-profile-body">'; 

  // start sidebar
  if (isset($vars['sidebar']) && count($vars['sidebar'])) {
    $sidebar_items = count($vars['sidebar']);
    $output .= '<div id="intel-profile-sidebar" class="sidebar">';
    
    foreach ($vars['sidebar'] AS $id => $elem) {
      $elem['#id'] = $id;
      $output .= render($elem);
    }
    $output .= '</div>';  // end #intel-profile-sidebar
    // end sidebar
  }
  // start main
  $output .= '<div id="intel-profile-main">';

  foreach ($vars['main'] AS $id => $elem) {
    $elem['#id'] = $id;
    $output .= render($elem);
    //$output .= theme($elem['#theme'], $elem['vars']);
  }
  $output .= '</div>';  // end #intel-profile-main
  
  $output .= '</div>';  // end #intel-profile-body
  
  // end main
  $output .= '</div>'; // end #intel-profile
  
  return $output;
}

function theme_intel_profile_value($vars) {
  $idtag = (!empty($vars['id'])) ? 'id="' . $vars['id']. '" ' : '';
  $class = 'intel-profile-value';
  $output .= '<div ' . $idtag . 'class="' . $class . '">';
  $output .= '<label for="value-' . $vars['id'] . '">' . $vars['label'] . ':</label>';
  $output .= '<div class="value">' . $vars['value'] . '</div>';
  $output .= '</div>';
  return $output;
}

function theme_intel_key_metric($vars) {
  $idtag = (!empty($vars['id'])) ? 'id="' . $vars['id']. '" ' : '';
  $class = 'intel-profile-key-metric';
  $output .= '<div ' . $idtag . 'class="' . $class . '">';
  $output .= '<div ' . $idtag . 'class="' . $class . '-box">';
  $output .= '<div class="value">' . $vars['value'] . '</div>';
  $output .= '<div class="desc">' . $vars['title'] . '</div>';
  $output .= '</div>';
  $output .= '</div>';
  return $output;  
}

function theme_intel_profile_fieldset($vars) {
  $content = '';
  foreach ($vars['items'] AS $id => $elem) {
    $elem['#id'] = $id;
    $content .= render($elem);
  }
  $block_vars = array(
    'title' => $vars['title'],
    'content' => $content,
  );
  $output = theme_intel_profile_block($block_vars);
  return $output;
}

function theme_intel_profile_block($vars) {
  $output = '';
  $idtag = (!empty($vars['id'])) ? 'id="' . $vars['id'] . '" ' : '';
  $class = 'intel-profile-block';
  $output .= '<div ' . $idtag . 'class="' . $class . '">';
  if (isset($vars['title'])) {
    $output .= '<h3>' . $vars['title'] . '</h3>';
  }
  $output .= $vars['content'];
  $output .= '</div>';
  return $output;
}

function theme_intel_profile_image($vars) {
  $output = '';
  $idtag = (!empty($vars['id'])) ? 'id="' . $vars['id'] . '" ' : '';
  $class = 'intel-profile-image';
  $output .= '<div ' . $idtag . 'class="' . $class . '">';
  if (empty($vars['path'])) {
    $vars['path'] = url(variable_get('intel_visitor_default_image_path', ''));
  }
  $output .= theme('image', $vars);
  $output .= '</div>';
  return $output;
}

//function theme_intel_location_map($locations, $options = array(), $s = '') {
function theme_intel_location_map($vars) {
  static $map_index;
  if (!isset($map_index)) {
    $map_index = 0;
  }
  $locations = $vars['locations'];

  $div_id = 'map_div_' . $map_index;
  // check if single element was passed
  if (isset($locations['latitude'])) {
    $locations = array(
      $locations,
    );
  }
  $mode = 1;
  $output = '';
  if ($mode == 1) {
    $options = array('type' => 'external', 'weight' => -1);
    drupal_add_js('https://maps.googleapis.com/maps/api/js?v=3.exp&sensor=false', $options);
    $locstr = '';
    $center = array('lat' => 0, 'lon' => 0, );
    foreach ($locations AS $loc) {
      if (!isset($loc['lat']) && isset($loc['latitude'])) {
        $loc['lat'] = $loc['latitude'];
      }
      if (!isset($loc['lon']) && isset($loc['longitude'])) {
        $loc['lon'] = $loc['longitude'];
      }
      $locstr .= "[" . $loc['lat'] . ", " . $loc['lon'] . ", " . "'" . $loc['name'] . "'],\n";
      $center['lat'] = $loc['lat']; 
      $center['lon'] = $loc['lon']; 
    }
    $locstr = substr($locstr, 0, -1); // chop last comma
    //$output .= <<<EOT
    $script = <<<EOT
function initialize_map_$map_index() {
  var mapOptions = {
    zoom: 4,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    disableDefaultUI: true,
    mapTypeId: google.maps.MapTypeId.ROADMAP
  };

  var map = new google.maps.Map(document.getElementById('map-canvas-$map_index'),
      mapOptions);
      
  var circleOptions = {
    strokeColor: '#FF0000',
    strokeOpacity: 0.8,
    strokeWeight: 1,
    fillColor: '#FF0000',
    fillOpacity: 0.35,
    map: map,
    center: new google.maps.LatLng({$center['lat']}, {$center['lon']}),
    radius: 50000
  };
  locCircle = new google.maps.Circle(circleOptions);
}
google.maps.event.addDomListener(window, 'load', initialize_map_$map_index);
EOT;
    drupal_add_js($script, array('type' => 'inline', 'scope' => 'header'));
    $output = '<div class="google-map"><div id="map-canvas-' . $map_index . '" class="map-canvas"></div></div>' . "\n";
    $map_index++;
  } 
  return $output; 
}

function theme_intel_browser_environment($vars) {
  $output = '';
  $v = array(
    'label' => t('Browser'),
    'value' => $vars['environment']['browser'] . " v" . $vars['environment']['browserVersion'],
  );
  $output .= theme('intel_profile_value', $v);
  
  $v = array(
    'label' => t('OS'),
    'value' => $vars['environment']['operatingSystem'] . " " . $vars['environment']['operatingSystemVersion'],
  );
  $output .= theme('intel_profile_value', $v);
  
  if (!empty($vars['environment']['mobileDeviceInfo'])) {
    $v = array(
      'label' => t('Mobile device'),
      'value' => $vars['environment']['mobileDeviceInfo'],
    );
    $output .= theme('intel_profile_value', $v);
  }
  
  $v = array(
    'label' => t('Screen'),
    'value' => $vars['environment']['screenResolution'],
  );
  $output .= theme('intel_profile_value', $v);
  
  $v = array(
    'label' => t('Language'),
    'value' => $vars['environment']['language'],
  );
  $output .= theme('intel_profile_value', $v);
  /*
  $output .= t('Browser') . ': ' . $vars['environment']['browser'] . " v" . $vars['environment']['browserVersion'] . "<br />\n";
  $output .= t('Operating system') . ': ' . $vars['environment']['operatingSystem'] . " " . $vars['environment']['operatingSystemVersion'] . "<br />\n";
  if (!empty($vars['environment']['mobileDeviceInfo'])) {
    $output .= t('Mobile device') . ': ' . $vars['environment']['mobileDeviceInfo'] . "<br />\n";
  }
  $output .= t('Screen resolution') . ': ' . $vars['environment']['screenResolution'] . "<br />\n";
  $output .= t('Language') . ': ' . $vars['environment']['language'] . "<br />\n";
  */
  return $output;
}

function theme_intel_social_profile_icons($vars) {
  $output = '<div class="intel-social-profile-icons">';
  $properties = $vars['properties'];
  $property_info = intel_get_visitor_property_info_all();
  foreach ($property_info AS $prop_name => $info) {
    if (!isset($info['category']) || ($info['category'] != 'social') || empty($info['icon'])) {
      continue;
    }
    $a = explode('.', $prop_name);
    $scope = $a[0];
    $namespace = $a[1];
    if (empty($properties[$namespace]) || empty($properties[$namespace]['url'])) {
      continue;
    }
    $prop = $properties[$namespace];
    $alt = t('@title profile (source: @source)',
      array(
        '@title' => $info['title'],
        '@source' => !empty($prop['_source']) ? $prop['_source'] : t('(not set)'),
      )
    );
    $iv = array(
      'path' => $info['icon'],
      'alt' => $alt,
      'title' => $alt,
    );
    $img = theme('image', $iv);
    $options = array(
      'html' => true,
      'attributes' => array(
        'target' => $namespace,
      ),
    );
    $output .= '<div class="profile icon ' . $namespace . '">';
    $output .= l($img, $prop['url'], $options);
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}


// deprecated theming functions

function theme_intel_table_visitor_id($vars) {
  $visitor = $vars['visitor'];
  if (empty($var['link_options'])) {
    $var['link_options'] = array();
  }
  $options = array(
    'attributes' => array(
      'class' => array(),
    ),
  );
  $var['link_options'] += $options;
  if (!empty($visitor->name)) {
    $vars['link_options']['attributes']['class'][] = 'known-visitor';
    return theme_intel_table_visitor_name($vars);
  }
  else {
    $vars['link_options']['attributes']['class'][] = 'anonymous-visitor';
    return l($visitor->vtkid, 'visitor/' . $visitor->vtkid, $vars['link_options']);
  }
}

function theme_intel_table_visitor_name($vars) {
  $visitor = $vars['visitor'];
  $link = isset($var['link']) ? $var['link'] : 1;
  $link_options = !empty($var['link_options']) ? $var['link_options'] : array();
  $title = '';
  if (!empty($visitor->data['company'])) {
    $title .= (($title) ? ', ' : '') . t('Company') . ': ' . $visitor->data['company'];
  }
  if (!empty($visitor->data['title'])) {
    $title .= (($title) ? ', ' : '') . t('Title') . ': ' . $visitor->data['title'];
  }
  $link_options += array(
    'attributes' => array(
      'title' => $title,
    ),
  );
  $name = (!empty($visitor->name)) ? $visitor->name : '(not set)';
  if ($link && !empty($visitor->vid)) {
    return l($name, 'visitor/' . $visitor->vid, $link_options);
  }
  else {
    return $name;
  }
}

function theme_intel_table_visitor_email($vars) {
  $visitor = $vars['visitor'];
  $link_options = !empty($var['link_options']) ? $var['link_options'] : array();
  $title = '';
  if (!empty($visitor->data['website'])) {
    $title .= (($title) ? ', ' : '') . t('Website') . ': ' . $visitor->data['website'];
  }
  if (!empty($visitor->data['phone'])) {
    $title .= (($title) ? ', ' : '') . t('Phone') . ': ' . $visitor->data['phone'];
  }
  $link_options += array(
    'attributes' => array(
      'title' => $title,
    ),
  );
  return l($visitor->email, 'mailto:' . $visitor->email, $link_options);
}

function theme_intel_table_visitor_location($vars) {
  $visitor = $vars['visitor'];
  $link_options = !empty($var['link_options']) ? $var['link_options'] : array();
  $output = '';
  if (!empty($visitor->data['location'])) {
    $output .= '<div title="' . $visitor->data['location']['city'] . ', ' . $visitor->data['location']['region'] . '">';
    $output .= $visitor->data['location']['country'];
    $output .= '</div>';
  }
  return $output;
}



