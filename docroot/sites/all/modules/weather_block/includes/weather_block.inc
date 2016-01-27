<?php

 /**
  * @file
  *
  * Code for displaying the weather page.
  */

/**
 *
 * Implements hook_form().
 */
function weather_block_page_form() {

  $locations = array();

  $array = taxonomy_vocabulary_get_names();

  if (arg(0) == 'weather') {
    $values = taxonomy_get_tree($array['weather_locations']->vid);
  }
  if (arg(0) == 'world_weather') {
    $values = taxonomy_get_tree($array['weather_locations_foreign']->vid);
  }

  foreach ($values as $tid) {

    // Check whether the taxonomy term is published or not and if it is, add to the menu
    $entity = entity_load('taxonomy_term', array($tid->tid));

    foreach ($entity as $val) {
      if ($val->status != 0) {
        $locations[$tid->tid] = $tid->name;
      }
    }
  }

  if (arg(0) == 'weather') {

    if (variable_get('weather_use_smart_ip') == 0) {

      unset($_SESSION['selected_weather_location']);
    }

    if(arg(1) != NULL) {

      $location = taxonomy_get_term_by_name(arg(1), 'weather_locations');

      reset($location);
      $location = key($location);

      $location = taxonomy_term_load($location);

      if($location != NULL) {

        $location = $location->tid;
      }
      else {

        $location = variable_get('weather_location');
      }

      if($location == NULL) {

        if (isset($_SESSION['selected_weather_location'])) {
          if (taxonomy_term_load($_SESSION['selected_weather_location']) != NULL) {
            $location = taxonomy_term_load($_SESSION['selected_weather_location']);
          }
          else {
            $location = weather_set_location();
            $location = $location->tid;
          }
        }
        else {
          $location = weather_set_location();
          $location = $location->tid;
        }
      }
    }

    else if (isset($_SESSION['selected_weather_location'])) {
      if (taxonomy_term_load($_SESSION['selected_weather_location']) != NULL) {
        $location = $_SESSION['selected_weather_location'];
      }
      else {

        if($_SESSION['selected_weather_location'] != 0 || !isset($_SESSION['selected_weather_location']) || arg(1) != NULL) {  

          $location = weather_set_location();
          $location = $location->tid;
        }
      }
    }
    else {

      $location = weather_set_location();

      if (variable_get('weather_use_smart_ip') == 1) {

        if(isset($_SESSION['smart_ip']['location']['city']) && in_array($_SESSION['smart_ip']['location']['city'], $locations)) {

          $location = $location->tid;
        }
        else {

          if(isset($_SESSION['smart_ip']['location']['city']) && in_array($_SESSION['smart_ip']['location']['city'] . ", " . $_SESSION['smart_ip']['location']['country'], $locations)) {

            $location = $location->tid;
          }
          else {

            $location = 0;
          }
        }
      }
      else {

        $location = $location->tid;
      }
    }
  }

  if (arg(0) == 'world_weather') {

    if (arg(1) == NULL) {

      $location = variable_get('weather_location_world');
    }
    else {

      $location = taxonomy_get_term_by_name(arg(1), 'weather_locations_foreign');

      reset($location);
      $location = key($location);

      $location = taxonomy_term_load($location);

      if($location == NULL) {

        $location = variable_get('weather_location_world');
      }
      else {

        $location = $location->tid;
      }
    }

    if (isset($_SESSION['selected_weather_location_world']) && arg(1) == NULL) {

      if (taxonomy_term_load($_SESSION['selected_weather_location_world']) != NULL) {
        $location = $_SESSION['selected_weather_location_world'];
      }
      else {
        $location = variable_get('weather_location_world');
      }
    }
    else {

      if (arg(1) == NULL) {

        $location = variable_get('weather_location_world');
      }
      else {

        $location = taxonomy_get_term_by_name(arg(1), 'weather_locations_foreign');

        reset($location);
        $location = key($location);

        $location = taxonomy_term_load($location);

        if($location == NULL) {

          $location = variable_get('weather_location_world');
        }
        else {

          $location = $location->tid;
        }
      }
    }
  }

  if(variable_get('weather_use_smart_ip') == 1) {

     if(isset($_SESSION['weather_location']['city'])) {

      $locations[0] = $_SESSION['weather_location']['city'];
    }
   }

  asort($locations);

  $form['weather_location'] = array(
    '#type' => 'select',
    '#title' => t('Select city'),
    '#default_value' => $location,
    '#options' => $locations,
    '#required' => FALSE,
  );

  $form['weather_location_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change location'),
  );

  return $form;
}

/**
 * Implements hook_form_submit().
 */
function weather_block_page_form_submit(&$form, $form_state) {

  if (arg(0) == 'weather') {
    $_SESSION['selected_weather_location'] = $form_state['values']['weather_location'];
  }
  if (arg(0) == 'world_weather') {
    $_SESSION['selected_weather_location_world'] = $form_state['values']['weather_location'];
  }

  $arg1 = arg(1);

  if($arg1 != NULL) {

    global $base_url;

    drupal_goto($base_url . "/" . arg(0));
  } 
}

/**
 * Function for creating the page for a weather display for cities other than the default.
 */
function weather_block_page() {

  if (arg(0) == 'weather') {

    if (arg(1) != NULL) {

      $location = taxonomy_get_term_by_name(arg(1), 'weather_locations');

      reset($location);
      $location = key($location);

      $location = taxonomy_term_load($location);

      if($location == NULL) {

        if (isset($_SESSION['selected_weather_location'])) {
          if (taxonomy_term_load($_SESSION['selected_weather_location']) != NULL) {
            $location = taxonomy_term_load($_SESSION['selected_weather_location']);
          }
          else {
            $location = weather_set_location();
          }
        }
        else {
          $location = weather_set_location();
        }
      }
    }

    else if (isset($_SESSION['selected_weather_location'])) {
      if (taxonomy_term_load($_SESSION['selected_weather_location']) != NULL) {
        $location = taxonomy_term_load($_SESSION['selected_weather_location']);
      }
      else {

        $location = weather_set_location();
      }
    }
    else {

      $location = weather_set_location();
    }
  }

  if (arg(0) == 'world_weather') {

    if (arg(1) != NULL) {

      $location = taxonomy_get_term_by_name(arg(1), 'weather_locations_foreign');

      reset($location);
      $location = key($location);

      $location = taxonomy_term_load($location);

      if($location == NULL) {

        if (isset($_SESSION['selected_weather_location_world'])) {
          if (taxonomy_term_load($_SESSION['selected_weather_location_world']) != NULL) {
            $location = taxonomy_term_load($_SESSION['selected_weather_location_world']);
          }
          else {
            $location = taxonomy_term_load(variable_get('weather_location_world'));
          }
        }
        else {
          $location = taxonomy_term_load(variable_get('weather_location_world'));
        }
      }
    }

    else if (isset($_SESSION['selected_weather_location_world'])) {
      if (taxonomy_term_load($_SESSION['selected_weather_location_world']) != NULL) {
        $location = taxonomy_term_load($_SESSION['selected_weather_location_world']);
      }
      else {
        $location = taxonomy_term_load(variable_get('weather_location_world'));
      }
    }
    else {
      $location = taxonomy_term_load(variable_get('weather_location_world'));
    }
  }

  if (!isset($location_id['und'][0]['value']) && !isset($_SESSION['selected_weather_location']) && arg(0) == 'weather') {

     if (isset($_SESSION['weather_location']['id'])) {

       if (variable_get('weather_service') != 'weather_com') {

         $weather = new Weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
       }
       else {

         $weather = new Weather_com_weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
       }
     }
     else {

       if (arg(1) == NULL) {

         if (variable_get('weather_service') != 'weather_com') {

           if (variable_get('weather_service') == 'yahoo_weather') {

             $id = $location->field_yahoo_weather['und'][0]['value'];
           }
           else {

             $id = $location->field_world_weather_online['und'][0]['value'];
           }

           $weather = new Weather($id, variable_get('weather_service'));
         }
         else {

           $weather = new Weather_com_weather($location->field_weather_com['und'][0]['value'], variable_get('weather_service'));
           $locationid = $location->field_weather_com['und'][0]['value'];
         }
       }
       else {
 
         if (variable_get('weather_service') == 'world_weather_online') {

           $weather = new Weather($location->field_world_weather_online['und'][0]['value'], variable_get('weather_service'));
         }
         else if (variable_get('weather_service') == 'yahoo_weather') {

           $weather = new Weather($location->field_yahoo_weather['und'][0]['value'], variable_get('weather_service'));
         }
         else {

           $term = taxonomy_term_load(variable_get('weather_location'));

           $weather = new Weather_com_weather($location->field_weather_com['und'][0]['value'], variable_get('weather_service'));
           $locationid = $location->field_weather_com['und'][0]['value'];
         }
       }
     }
  }
  else if (!isset($location_id['und'][0]['value']) && !isset($_SESSION['selected_weather_location_world']) && arg(0) == 'world_weather') {

     if (isset($_SESSION['weather_location']['id'])) {

       if (variable_get('weather_service') != 'weather_com') {

         $weather = new Weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
       }
       else {

         $weather = new Weather_com_weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
       }
     }
     else {

       if (arg(1) == NULL) {

         if (variable_get('weather_service') != 'weather_com') {

           if (variable_get('weather_service') == 'yahoo_weather') {

             $id = $location->field_yahoo_weather['und'][0]['value'];
           }
           else {

             $id = $location->field_world_weather_online['und'][0]['value'];
           }

           $weather = new Weather($id, variable_get('weather_service'));
         }
         else {

           $weather = new Weather_com_weather($location->field_weather_com['und'][0]['value'], variable_get('weather_service'));
           $locationid = $location->field_weather_com['und'][0]['value'];
         }
       }
       else {

         if (variable_get('weather_service') == 'world_weather_online') {

           $weather = new Weather($location->field_world_weather_online['und'][0]['value'], variable_get('weather_service'));
         }
         else if (variable_get('weather_service') == 'yahoo_weather') {

           $weather = new Weather($location->field_yahoo_weather['und'][0]['value'], variable_get('weather_service'));
         }
         else {

           $term = taxonomy_term_load(variable_get('weather_location'));

           $weather = new Weather_com_weather($location->field_weather_com['und'][0]['value'], variable_get('weather_service'));
           $locationid = $location->field_weather_com['und'][0]['value'];
         }
       }
     }
  }
  else {

    if (isset($_SESSION['selected_weather_location']) && $_SESSION['selected_weather_location'] == 0 && arg(0) == 'weather') {

      if(variable_get('weather_service') == 'weather_com') {

        $weather = new Weather_com_weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
        $locationid = $_SESSION['weather_location']['id'];
      }
      else {

        $weather = new Weather($_SESSION['weather_location']['id'], variable_get('weather_service'));
      }
    }
    else {

      $services = variable_get('weather_services');

      foreach ($services as $key => $val) {
        if ($key == variable_get('weather_service')) {
          $key = 'field_' . $key;

          $location_id = $location->$key;
        }
      }

      if(variable_get('weather_service') == 'weather_com') {

        $weather = new Weather_com_weather($location_id['und'][0]['value'], variable_get('weather_service'));
        $locationid = $location_id['und'][0]['value'];
      }
      else {

        $weather = new Weather($location_id['und'][0]['value'], variable_get('weather_service'));
      }
    }
  }

  if (variable_get('weather_service') == 'weather_com') {

    if (variable_get('weather_display_units') == 0) {
      $scale = t('&deg;C');
      $s = 0;
    }
    else {
      $scale = t('&deg;F');
      $s = 1;
    }

    $current[] = $weather->weather_com_current($locationid);

    if (variable_get('weather_display_units', 0) == 1) {

      preg_match('/[0-9]+/', check_plain(trim($current[0]['wind'])), $matches);

      $windspeed = implode($matches);

      $windspeed = round($windspeed / 1.609344, 0);

      $windspeed = preg_replace('/[0-9]+/', $windspeed, $current[0]['wind']);

      $current[0]['wind'] = str_replace('kmph', 'mph', $windspeed);
    }

    $unixtime = time();

    $forecast = array();

    $i = 0;

    foreach($weather->weather_com_forecast($locationid) as $key => $val) {

       $day = date('l', $unixtime);

       $unixtime = $unixtime + 86400;

       $forecast[$i] = $val;
       $forecast[$i]['day'] = $day;

       if($s == 1) {

         $forecast[$i]['temp_low'] = $weather->celsiusToFahrenheit($forecast[$i]['temp_low']) . " " . $scale;
         $forecast[$i]['temp_high'] = $weather->celsiusToFahrenheit($forecast[$i]['temp_high']) . " " . $scale;
       }
       else {

         $forecast[$i]['temp_low'] = $forecast[$i]['temp_low'] . " " . $scale;
         $forecast[$i]['temp_high'] = $forecast[$i]['temp_high'] . " " . $scale;
       }

       $i++;
    }

    $content = array_merge($current, $forecast);

    $content[0]['day'] = "<div class='forecast_day_wrapper'>" . t('Current conditions') . "</div>";

    if($s == 1) {

      $content[0]['temp'] = $weather->celsiusToFahrenheit($content[0]['temp']) . " " . $scale;
      $content[0]['dew_point'] = $weather->celsiusToFahrenheit($content[0]['dew_point']) . " " . $scale;
    }
    else {

      $content[0]['temp'] = $content[0]['dew_point'] . " " . $scale;
      $content[0]['dew_point'] = $content[0]['dew_point'] . " " . $scale;
    }
  }
  elseif (variable_get('weather_service') == 'yahoo_weather') {
    $data = $weather->otherCities();

    if (variable_get('weather_display_units') == 0) {
      $scale = t('&deg;C');
    }
    else {
      $scale = t('&deg;F');
    }

    $path = base_path() . drupal_get_path('module', 'weather_block');

    foreach ($data as $key => $val) {
      if (variable_get('weather_use_provider_weather_icons') == 1) {
        $icon = "http://l.yimg.com/a/i/us/we/52/" . $val['code'] . ".gif";
      }
      else {
        $icon = $path . "/weather-icons/" . $val['code'] . ".png";
      }

      $content[$key]['icon'] = check_plain($icon);
      $content[$key]['cond'] = check_plain($val['cond']);

      if ($key == 0) {
        $content[$key]['current'] = check_plain($val['temp']) . " " . $scale;
        $content[$key]['day'] = t('Current weather');

        if (variable_get('weather_display_wind_yahoo_weather', 1) == 1) {

          $imgpath = $path . "/weather-icons/wind/" . $val['wind']['direction'] . ".png";

          $content[$key]['wind'] = "<img src='" . $imgpath . "' />" . t(' at ');

          $content[$key]['wind'] .= " " . round(check_plain($weather->general->wind->attributes()->speed)) . " " . check_plain($weather->general->units->attributes()->speed);
        }

        if (variable_get('weather_display_humidity_yahoo_weather', 1) == 1) {
          $content[$key]['humidity'] = check_plain($val['humidity']) . "%";
        }
      }
      else {

        $content[$key]['low'] = check_plain($val['temp']['low']) . " " . $scale;
        $content[$key]['high'] = check_plain($val['temp']['high']) . " " . $scale;

        switch ($val['day']) {
          case 'Mon':
            $day = 'Monday';
          break;
          case 'Tue':
            $day = 'Tuesday';
          break;
          case 'Wed':
            $day = 'Wednesday';
          break;
          case 'Thu':
            $day = 'Thursday';
          break;
          case 'Fri':
            $day = 'Friday';
          break;
          case 'Sat':
            $day = 'Saturday';
          break;
          case 'Sun':
            $day = 'Sunday';
          break;
        }

        $content[$key]['day'] = t($day);
      }
    }
  }
  elseif (variable_get('weather_service') == 'world_weather_online') {

    if (variable_get('weather_display_units') == 0) {
      $scale = t('&deg;C');
    }
    else {
      $scale = t('&deg;F');
    }

    $content[0]['day'] = "<div class='forecast_day_wrapper'>" . t('Current conditions') . "</div>";

    if (variable_get('weather_display_units') == 0) {
      $deg_low = $weather->xml->current_condition->temp_C;
    }
    else {
      $deg_low = $weather->xml->current_condition->temp_F;
    }

    $content[0]['low'] = check_plain($deg_low) . " " . $scale;

    if (date('G') < 9 || date('G') > 21) {
      $content[0]['icon'] = "http://c358489.r89.cf1.rackcdn.com/images/weather/small/" . check_plain($weather->xml->current_condition->weatherCode) . "_night_sm.png";
    }
    else {
      $content[0]['icon'] = "http://c358489.r89.cf1.rackcdn.com/images/weather/small/" . check_plain($weather->xml->current_condition->weatherCode) . "_day_sm.png";
    }

    $content[0]['humidity'] = check_plain($weather->xml->current_condition->humidity) . "%";

    if (variable_get('weather_display_units', 0) == 0) {
      $wind = round((int) $weather->xml->current_condition->windspeedKmph * 1000 / 3600) . " m/s";
    }
    else {
      $wind = $weather->xml->current_condition->windspeedMiles . " mph";
    }

    $content[0]['wind'] = t(check_plain($weather->xml->current_condition->winddir16Point) . " at " . check_plain($wind));

    $i = 1;

    foreach($weather->xml->weather as $key => $val) {

      $content[$i]['day'] = "<div class='forecast_day_wrapper'>" . t(date('l', strtotime($val->date))) . "</div>";
 
      if (variable_get('weather_display_units') == 0) {
        $deg_low = $val->tempMinC;
      }
      else {
        $deg_low = $val->tempMinF;
      }

      $content[$i]['low'] = check_plain($deg_low) . " " . $scale;

      if (variable_get('weather_display_units') == 0) {
        $deg_high = $val->tempMaxC;
      }
      else {
        $deg_high = $val->tempMaxF;
      }

      $content[$i]['high'] = check_plain($deg_high) . " " . $scale;

      if (variable_get('weather_display_units', 0) == 0) {
        $wind = round((int) $val->windspeedKmph * 1000 / 3600) . " m/s";
      }
      else {
        $wind = $val->windspeedMiles . " mph";
      }

      $content[$i]['wind'] = t(check_plain($val->winddir16Point) . " at " . check_plain($wind));

      $content[$i]['icon'] = "http://c358489.r89.cf1.rackcdn.com/images/weather/small/" . check_plain($val->weatherCode) . "_day_sm.png";

      $i++;
    }
  }
  else {
  }

  $menu = $weather->menu();

  $form = drupal_get_form('weather_block_page_form');

  cache_clear_all();

  return theme('weather_block_local_weather', array('content' => $content, 'menu' => $menu, 'location' => $location, 'form' => $form, 'service' => variable_get('weather_service')));
}
