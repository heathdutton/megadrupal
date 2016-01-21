<?php
/**
 * @file
 * Classes to get the weather information from AEMET
 */
class weather_es_Aemet {
  private $city_code;
  private $name;
  private static $url = 'http://www.aemet.es/xml/municipios/';
  //private static $url = 'http://drupal.local/';
  //private static $url = 'file:///var/www/jose/drupal-6.22/';
  private $dom;
  private $days;
  private $load_ok;

  public function __construct($cit_cod) {
    $this->city_code = $cit_cod;
    $this->dom = new DOMDocument('1.0', 'ISO-8859-15');
    $this->name = '';
    $success = $this->dom->load(self::$url . 'localidad_' . $this->city_code . '.xml');
    if ($success) {
      $this->load_ok = TRUE;
      $this->name = $this->dom->documentElement->getElementsByTagName('nombre')->item(0)->nodeValue;
      $dias = $this->dom->documentElement->getElementsByTagName('prediccion')->item(0)->childNodes;
      $days = array();

      foreach ($dias as $dia) {
        if ($dia->nodeType == XML_ELEMENT_NODE) {
          $date = $dia->getAttributeNode("fecha")->value;
          $datos = $dia->childNodes;
          $rain = array();
          $snow = array();
          $sky = array();
          $wind = array();
          $windSpeed = array();
          $temp = array();
          $sens = array();
          $hum = array();
          $uv = '';

          foreach ($datos as $dato) {
            if ($dato->nodeType == XML_ELEMENT_NODE) {
              if ($dato->nodeName == 'prob_precipitacion') {
                if (isset($dato->getAttributeNode('periodo')->value)) {
                  $rain[$dato->getAttributeNode('periodo')->value] = $dato->nodeValue;
                }
                else{
                  $rain['00-24'] = $dato->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'cota_nieve_prov') {
                if (isset($dato->getAttributeNode('periodo')->value)) {
                  $snow[$dato->getAttributeNode('periodo')->value] = $dato->nodeValue;
                }
                else{
                  $snow['00-24'] = $dato->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'estado_cielo') {
                if (isset($dato->getAttributeNode('periodo')->value)) {
                  $sky[$dato->getAttributeNode('periodo')->value] = array($dato->getAttributeNode('descripcion')->value => $dato->nodeValue);
                }
                else{
                  $sky['00-24'] = array($dato->getAttributeNode('descripcion')->value => $dato->nodeValue);
                }
              }
              elseif ($dato->nodeName == 'viento') {
                if (isset($dato->getAttributeNode('periodo')->value)) {
                  $wind[$dato->getAttributeNode('periodo')->value] = array($dato->getElementsByTagName('direccion')->item(0)->nodeValue => $dato->getElementsByTagName('velocidad')->item(0)->nodeValue);
                }
                else{
                  $wind['00-24'] = array($dato->getElementsByTagName('direccion')->item(0)->nodeValue => $dato->getElementsByTagName('velocidad')->item(0)->nodeValue);
                }
              }
              elseif ($dato->nodeName == 'racha_max') {
                if (isset($dato->getAttributeNode('periodo')->value)) {
                  $gust[$dato->getAttributeNode('periodo')->value] = $dato->nodeValue;
                }
                else{
                  $gust['00-24'] = $dato->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'temperatura') {
                $temp = array(
                  'maxima' => $dato->getElementsByTagName('maxima')->item(0)->nodeValue,
                  'minima' => $dato->getElementsByTagName('minima')->item(0)->nodeValue,
                );
                foreach ($dato->getElementsByTagName('dato') as $d) {
                  $temp[$d->getAttributeNode('hora')->value] = $d->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'sens_termica') {
                $chill = array(
                  'maxima' => $dato->getElementsByTagName('maxima')->item(0)->nodeValue,
                  'minima' => $dato->getElementsByTagName('minima')->item(0)->nodeValue,
                );
                foreach ($dato->getElementsByTagName('dato') as $d) {
                  $chill[$d->getAttributeNode('hora')->value] = $d->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'humedad_relativa') {
                $hum = array(
                  'maxima' => $dato->getElementsByTagName('maxima')->item(0)->nodeValue,
                  'minima' => $dato->getElementsByTagName('minima')->item(0)->nodeValue,
                );
                foreach ($dato->getElementsByTagName('dato') as $d) {
                  $hum[$d->getAttributeNode('hora')->value] = $d->nodeValue;
                }
              }
              elseif ($dato->nodeName == 'uv_max') {
                $uv = $dato->nodeValue;
              }
            }
          }
          $this->days[] = array(
            'date' => $date,
            'rain' => $rain,
            'snow' => $snow,
            'sky' => $sky,
            'wind' => $wind,
            'gust' => $gust,
            'temp' => $temp,
            'chill' => $chill,
            'hum' => $hum,
            'uv' => $uv,
          );
        }
      }
    }
    else {
      $this->load_ok = FALSE;
    }
  }
  public function getName() {
    return $this->name;
  }
  public function getInfo() {
    return $this->days;
  }
  public function isLoadOk() {
    return $this->load_ok;
  }
}
/**
 * Class that gets the cities of a province
 */
class weather_es_citPro{
  private $xprovince_code;
  public $xcities;
  private $xerror_message;
  private static $url = 'http://www.aemet.es/es/eltiempo/prediccion/municipios?p=';
  //private static $url = 'file:///var/www/jose/';
  private static $cit_cod_len = 5;

  public function __construct($province_code) {
    // Can't use DOM because AEMET's files aren't valid XHTML files...
    $this->xprovince_code = $province_code;
    $this->xerror_message = '';
    if ($province_code == '---') {
      $this->xcities = array(
        '00' => t('Any city'),
      );
    }
    else {
      if (function_exists('fsockopen')) {
        $this->cities(drupal_http_request(self::$url . $this->xprovince_code));
      }
      else {
        $this->xerror_message = t('Function fsockopen does not exist, you can\'t use this module...');
      }
    }
  }
  /**
   *
   * @param drupal_http_request $result
   */
  public function cities($result) {
    switch ($result->code) {
      case '301':
      case '302':
      case '307':
        $result = drupal_http_request('http://www.aemet.es' . $result->redirect_url);
      case '200':
      case '304':
        // Get the table
        preg_match('/<select.+localidades_selector.+select>/sU', $result->data, $captured);
        // Pass it to a string
        $data_string = implode(" ", $captured);
        $length = strlen($data_string);
        $position = strpos($data_string, "select>");
        $data_string = substr($data_string, 0, $position - $length);
        $clean_string = preg_replace('/[ \f\r\t\n]+/', ' ', $data_string);
        preg_match_all('/<option.+option>/sU', $clean_string, $captured_option);
        break;
      default:
        $this->xerror_message = t('fsockopen fail to open AEMET.');
    }
    // The first code is [0] => ... so it's no useful
    for ($i = 1; $i < sizeof($captured_option[0]); $i++) {
      preg_match('/id[0-9]{5}/', $captured_option[0][$i], $cit_cod);
      $cit_cod = substr($cit_cod[0], 2, self::$cit_cod_len);
      preg_match('/>.+</', $captured_option[0][$i], $cit_nam);
      $cit_nam_lon = strlen($cit_nam[0]);
      $this->xcities[$cit_cod] = drupal_convert_to_utf8(substr($cit_nam[0], 1, $cit_nam_lon - 2), 'ISO-8859-15');
    }
    unset($result);
  }
  /**
   *
   * @return array [city_code] => city_name
   */
  public function getCities() {
    return $this->xcities;
  }
  /**
   *
   * @return string 'nn' n [0-9]
   */
  public function getProvince() {
    return $this->xprovince_code;
  }
  /**
   *
   * @return bool
   */
  public function error() {
    if ($this->xerror_message === '') {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }
  public function errorMessage() {
    return $this->xerror_message;
  }
}
