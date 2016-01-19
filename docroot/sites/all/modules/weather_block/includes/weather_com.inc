<?php

class Weather_com_weather extends Weather {

  public function weather_com_forecast($location) {

    $xml_str = new DOMDocument();

    @$xml_str->loadHTMLFile("http://uk.weather.com/weather/10day-" . $location);

    $finder = new DomXPath($xml_str);

    $classname="forecast";
    $nodes = $finder->query("//*[contains(concat(' ', normalize-space(@class), ' '), ' $classname ')]");

    $i = 0;

    foreach ($nodes as $node) {

      if ($node->getElementsByTagName('span')->item(0)->nodeValue == 'Forecast') {

        $object[$i]['image'] = $node->getElementsByTagName('img')->item(0)->getAttribute('src');

        $object[$i]['weather_type'] = trim($node->getElementsByTagName('b')->item(0)->nodeValue);

        $temp = trim($node->getElementsByTagName('p')->item(0)->nodeValue);

        preg_match('/[\-+]?[0-9]+/', substr($temp, 0, strpos($temp, 'Low')), $matches_high);

        preg_match('/[\-+]?[0-9]+/', substr($temp, strpos($temp, 'Low')), $matches_low);

        $object[$i]['temp_low'] = implode($matches_low);

        $object[$i]['temp_high'] = implode($matches_high);

        $i++;
      }
    }

    return $object;
  }

  public function weather_com_current($location) {

    $xml_str = new DOMDocument();

    @$xml_str->loadHTMLFile("http://uk.weather.com/weather/today-" . $location);

    $object['image'] = $xml_str->getElementById('current_box_icon')->getElementsByTagName('img')->item(0)->getAttribute('src');

    preg_match('/[\-+]?[0-9]+/', $xml_str->getElementById('current_box_temp')->nodeValue, $matches);

    $object['temp'] = implode($matches);

    $object['weather_type'] = $xml_str->getElementById('current_box')->getElementsByTagName('div')->item(3)->nodeValue;

    $object['wind'] = $xml_str->getElementById('current_right')->getElementsByTagName('strong')->item(0)->nextSibling->nextSibling->nodeValue;

    $object['humidity'] = str_replace('%', '', $xml_str->getElementById('current_right')->getElementsByTagName('strong')->item(1)->nextSibling->nextSibling->nodeValue);

    preg_match('/[\-+]?[0-9]+/', $xml_str->getElementById('current_right')->getElementsByTagName('strong')->item(3)->nextSibling->nextSibling->nodeValue, $matches2);

    $object['dew_point'] = implode($matches2);

    $object['visibility'] = $xml_str->getElementById('current_right')->getElementsByTagName('strong')->item(4)->nextSibling->nextSibling->nodeValue;

    return $object;
  }
}
