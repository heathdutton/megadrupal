<?php

/**
 * @file: Template for the module output
 */

   print "<div class='forecast_menu'>" . $menu . "</div>";

   if(!isset($location)) {

     $name = $_SESSION['weather_location']['city'];
   }
   else {

     $name = $location->name;
   }

   print "<div class='forecast_location'>" . $name . "</div>";

   print drupal_render($form);

   $i = 0;

   foreach($content as $key => $val) {

     print "<div class='weather_forecast'>";

     if($service == 'yahoo_weather' && $key == 0) {

       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img title='" . $val['cond'] . "' src='" . $val['icon'] . "'></img></div>";
       print "<div class='weather_current'><div class='forecast_temp_current_wrapper'><div class='forecast_temp_current'>" . t('Temperature') . "</div><div class='forecast_temp_current_value'> " . $val['current'] . "</div></div>";
       print "<div class='forecast_wind_wrapper'><div class='forecast_wind'>" . t('Wind') . "</div><div class='forecast_wind_value'>" . $val['wind'] . "</div></div>";
       print "<div class='forecast_humidity_wrapper'><div class='forecast_humidity'>" . t('Humidity') . "</div><div class='forecast_humidity_value'>" . $val['humidity'] . "</div></div></div>";
     }

     if ($service == 'yahoo_weather' && $key == 1) {
       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img title='" . $val['cond'] . "' src='" . $val['icon'] . "'></img></div>";
       print "<div class='forecast_temp_low_wrapper'><div class='forecast_temp_low'>" . t('Low') . "</div><div class='forecast_temp_low_value'> " . $val['low'] . "</div></div>";
       print "<div class='forecast_temp_high_wrapper'><div class='forecast_temp_high'>" . t('High') . "</div><div class='forecast_temp_high_value'> " . $val['high'] . "</div></div>";
     }

     if($service == 'world_weather_online' && $key == 0) {
       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img src='" . $val['icon'] . "'></img></div>";
       print "<div class='weather_current'><div class='forecast_temp_current_wrapper'><div class='forecast_temp_current'>" . t('Temperature') . "</div><div class='forecast_temp_current_value'> " . $val['low'] . "</div></div>";
       print "<div class='forecast_wind_wrapper'><div class='forecast_wind'>" . t('Wind') . "</div><div class='forecast_wind_value'> " . $val['wind'] . "</div></div>";
       print "<div class='forecast_humidity_wrapper'><div class='forecast_humidity'>" . t('Humidity') . "</div><div class='forecast_humidity_value'>" . $val['humidity'] . "</div></div></div>";
     }

     if ($service == 'world_weather_online' && $key != 0) {
       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img src='" . $val['icon'] . "'></img></div>";
       print "<div class='forecast_temp_low_wrapper'><div class='forecast_temp_low'>" . t('Low') . "</div><div class='forecast_temp_low_value'> " . $val['low'] . "</div></div>";
       print "<div class='forecast_temp_high_wrapper'><div class='forecast_temp_high'>" . t('High') . "</div><div class='forecast_temp_high_value'> " . $val['high'] . "</div></div>";
       print "<div class='forecast_wind_wrapper'><div class='forecast_wind'>" . t('Wind') . "</div><div class='forecast_wind_value'> " . $val['wind'] . "</div></div>";
     }

     if($service == 'weather_com' && $key == 0) {
       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img src='" . $val['image'] . "'></img></div>";
       print "<div class='weather_current'><div class='forecast_temp_current_wrapper'><div class='forecast_temp_current'>" . t('Temperature') . "</div><div class='forecast_temp_current_value'> " . $val['temp'] . "</div></div>";
       print "<div class='forecast_wind_wrapper'><div class='forecast_wind'>" . t('Wind') . "</div><div class='forecast_wind_value'> " . $val['wind'] . "</div></div>";
       print "<div class='forecast_humidity_wrapper'><div class='forecast_humidity'>" . t('Humidity') . "</div><div class='forecast_humidity_value'>" . $val['humidity'] . "%</div></div>";
       print "<div class='forecast_dew_point_wrapper'><div class='forecast_dew_point'>" . t('Dew point') . "</div><div class='forecast_dew_point_value'>" . $val['dew_point'] . "</div></div></div>";
     }

     if ($service == 'weather_com' && $key != 0) {
       print "<div class='forecast_day_wrapper'>" . $val['day'] . "</div>";
       print "<div class='forecast_icon'><img src='" . $val['image'] . "'></img></div>";
       print "<div class='forecast_temp_low_wrapper'><div class='forecast_temp_low'>" . t('Low') . "</div><div class='forecast_temp_low_value'> " . $val['temp_low'] . "</div></div>";
       print "<div class='forecast_temp_high_wrapper'><div class='forecast_temp_high'>" . t('High') . "</div><div class='forecast_temp_high_value'> " . $val['temp_high'] . "</div></div>";
     }

     print "</div>";
   }

?>
