<?php

 /**
  * @file
  *
  * Code for fetching the weather data.
  */
class Weather {

    function __construct($city, $service) {

        $this->service = $service;

        if ($city == '') {
          $this->setCity();
        }
        else {
          $this->location_id = $city;
        }

        $reg_key = variable_get('weather_key_world_weather_online');

        // One weather
        static $xml_str;

        if (!($cache = cache_get($this->location_id))) {
          if ($this->service == 'yahoo_weather') {
            if (variable_get('weather_display_units') == 0) {
              $xml_str = simplexml_load_file("http://weather.yahooapis.com/forecastrss?w=$this->location_id&u=c");
            }
            else {

              $xml_str = simplexml_load_file("http://weather.yahooapis.com/forecastrss?w=$this->location_id&u");
            }
          }
          elseif ($this->service == 'world_weather_online') {

            $xml_str = simplexml_load_file("http://free.worldweatheronline.com/feed/weather.ashx?q=$this->location_id&format=xml&num_of_days=5&key=$reg_key");
          }
          else {
          }
        }
        else {
          $xml_str = unserialize(cache_get($this->location_id)->data);
        }

        if (!empty($xml_str)) {
          try {
            if ($this->service == 'weather_com') {
            }
            elseif ($this->service == 'world_weather_online') {
              $this->xml = $xml_str;
            }
            else {

              foreach ($xml_str->channel as $channel) {
                $this->general = $channel->children('http://xml.weather.yahoo.com/ns/rss/1.0');
              }
              foreach ($xml_str->channel->item as $item) {
                $this->condition = $item->children('http://xml.weather.yahoo.com/ns/rss/1.0');
              }
            }
       }

        catch (Exception $e) {
        }
      }
    }

    /**
     * Converts the character encoding to utf-8 when necessary.
     */
    public function sxe($url) {

      $xml = file_get_contents(utf8_decode($url));

      foreach ($http_response_header as $header) {

        if (preg_match('#^Content-Type: text/xml; charset=(.*)#i', $header, $m)) {
          switch (drupal_strtolower($m[1])) {
            case 'utf-8':
            break;
            case 'iso-8859-1':
              $xml = utf8_encode($xml);
            break;
            default:
              $xml = iconv($m[1], 'utf-8', $xml);
            break;
          }
        }

        return $xml;
      }
    }

   /**
    * Fetches the temperature data and prepares it for display.
    */
    public function temperature($tmp = NULL) {
      $scale = variable_get('weather_temperature_scale');

      if ($scale == 1) {

        if ($tmp == NULL) {
          $celcius = self::fahrenheitToCelsius($this->todaysForecast()->tmp);
          return $celcius . ' °C';
        }
        else {
          $celcius = self::fahrenheitToCelsius($tmp);
          return $celcius . ' °C';
        }
      }
      else {
        if ($tmp == NULL) {
          return $this->todaysForecast()->tmp . ' °F';
        }
        else {
          return $tmp . ' °F';
        }
      }
    }

    /**
     * Converts fahrenheit to celcius.
     */
    public static function fahrenheitToCelsius($fahrenheit) {
      if ($fahrenheit == 'N/A') {
        return t('Not available');
      }
      $celcius = round(((int)$fahrenheit - 32) / 1.8);
      return $celcius;
    }

    /**
     * Converts celcius to fahrenheit.
     */
    public static function celsiusToFahrenheit($celcius) {
      if ($celcius == 'N/A') {
        return t('Not available');
      }

      $fahrenheit = round(((int)$celcius + 32) * 1.8);
      return $fahrenheit;
    }

    /**
     * Forecast for other cities for yahoo weather.
     */
    public function otherCities() {

        $forecast[0]['cond'] = $this->condition->condition->attributes()->text;
        $forecast[0]['code'] = $this->condition->condition->attributes()->code;
        $forecast[0]['temp'] = $this->condition->condition->attributes()->temp;
        $forecast[0]['wind']['direction'] = $this->general->wind->attributes()->direction;
        $forecast[0]['wind']['speed'] = $this->general->wind->attributes()->speed;
        $forecast[0]['humidity'] = $this->general->atmosphere->attributes()->humidity;
        $forecast[1]['cond'] = $this->condition->forecast[1]->attributes()->text;
        $forecast[1]['code'] = $this->condition->forecast[1]->attributes()->code;
        $forecast[1]['day'] = $this->condition->forecast[1]->attributes()->day;
        $forecast[1]['temp']['high'] = $this->condition->forecast[1]->attributes()->high;
        $forecast[1]['temp']['low'] = $this->condition->forecast[1]->attributes()->low;

        return $forecast;
    }

    /**
     * Creates a custom menu for swithing between local and world weather.
     */
    public function menu() {
      $arg = arg(0);
      if ($arg === 'world_weather')
        return l(t('Weather'), 'weather') . ' | ' . '<strong>' . l(t('World weather') , 'world_weather') . '</strong>';

      if ($arg === 'weather') {
        return '<strong>' . l(t('Weather'), 'weather') . '</strong>' . ' | ' . l(t('World weather') , 'world_weather');
      }
    }

    /**
     * Sets a default city if no city has been set.
     */
    public function setCity($city_name = "Helsinki") {
      $array = taxonomy_vocabulary_get_names();

      $rs = taxonomy_get_term_by_name($city_name);

      $location = taxonomy_term_load($rs->tid);

      if ($this->service == 'weather_com') {
        $this->location_id = $location->field_weather_com['und'][0]['value'];
      }
      elseif ($this->service == 'world_weather_online') {
        $this->location_id = $location->field_world_weather_online['und'][0]['value'];
      }
      else {
        $this->location_id = $location->field_yahoo_weather['und'][0]['value'];
      }
    }
}
