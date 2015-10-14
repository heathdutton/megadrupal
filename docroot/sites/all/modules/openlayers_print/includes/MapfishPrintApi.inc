<?php

require_once("MapfishPrintException.inc");

class MapfishPrintApi {
  private $url = Null;

  private $apiInfo = array();

  public function __construct($url) {

    // strip of the last part if the url ends with ".json"
    $jsonPos = strrpos($url, ".json");
    if (($jsonPos !== False) && (strlen($url) === ($jsonPos+5))) {
      $slashPos = strrpos($url, '/');
      if ($slashPos !== False) {
        $url = substr($url, 0, $slashPos);
      }
    }

    $this->url = rtrim($url, '/');
    $this->fetchApiInfo();
  }

  private function fetchApiInfo() {
    $infoUrl = $this->url . "/info.json";

    $result = drupal_http_request($infoUrl);
    if (isset($result->error)) {
      throw new MapfishPrintException(t("Could not contact MapFish print service at ") .
          $this->url . ": ".
          " " . $result->error);
    }

    $this->apiInfo = json_decode($result->data, True);
    if ($this->apiInfo === NULL) {
      throw new MapfishPrintException(t("MapFish print service returned invalid JSON data:") .
            " " . $result->data);
    }
  }

  public function isValid() {
    return (!empty($this->apiInfo) && is_array($this->apiInfo));
  }

  private function _getGeneric($key, $nameKey="name") {
    $values = array();
    if ($this->isValid()) {
      if (is_array($this->apiInfo[$key])) {
        foreach($this->apiInfo[$key] as $v) {
          if (!empty($v[$nameKey])) {
            $values[] = $v[$nameKey];
          }
        }
      }
    }
    return $values;
  }

  /** return an array containing all supported output formats */
  public function getOutputFormats() {
    return $this->_getGeneric('outputFormats', 'name');
  }

  /** return an array containing all supported layouts */
  public function getLayouts() {
    return $this->_getGeneric('layouts', 'name');
  }

  /** return an array containing all supported DPI settings */
  public function getDpis() {
    return $this->_getGeneric('dpis', 'name');
  }

  /** return an array containing all supported scale factors */
  public function getScales() {
    return $this->_getGeneric('scales', 'value');
  }

  /** get the scale supported by mapfish which most closely matches
   * the $inScale passed to this method
   */
  protected function getClosestScale($inScale, $scales, $method="nearest") {
    $closest = null;
    if ($method == "next-bigger") {
      arsort($scales);
      foreach($scales as $scale) {
        if($closest == null || ($scale > $inScale)) {
          $closest = $scale;
        }
      }
    }
    elseif ($method == "nearest") {
      // nearest scaling might also use a smaller scalefactor if it will fit better
      foreach($scales as $scale) {
        if($closest == null || abs($inScale - $closest) > abs($scale - $inScale)) {
          $closest = $scale;
        }
      }
    }
    return $closest;
  }

  /**
   * produce a print and return the filename of the generated print.
   */
  public function render($createRequest) {

    // fix up the request and find the closest scale to the unit passed down
    // in the request. According to the documentation should do this itself,
    // but in practice mapfish print will just return an error that the given scale
    // has not been found
    if (isset($createRequest["pages"])) {
      if (is_array($createRequest["pages"])) {
        $scales = $this->getScales();
        for ($i = 0; $i < count($createRequest["pages"]); $i++) {
          if (isset($createRequest["pages"][$i]["scale"])) {
            $createRequest["pages"][$i]["scale"] = $this->getClosestScale($createRequest["pages"][$i]["scale"], $scales);
          }
        }
      }
    }

    $createUrl = $this->url . "/create.json";
    $response = drupal_http_request($createUrl,
      array(
        "method" => "POST",
        "headers" => array(
          "Content-Type" => "application/x-www-form-urlencoded"
        ),
        "data" => "spec=".urlencode(json_encode($createRequest))
      )
    );

    if ($response->error) {
      throw new MapfishPrintException(t("Error during rendering:") . " " . $response->data);
    }

    $data = json_decode($response->data, True);
    if ($data === NULL) {
      throw new MapfishPrintException(("MapFish print service returned invalid JSON data:") .
            " " . $result->data);
    }

    // extract the filename from an downloadurl
    $filename = ltrim(substr($data['getURL'], strlen($this->url)), '/');
    return $filename;
  }

  /** get the full downloadurl for a filename of a generated print */
  public function getDownloadUrl($filename) {
    return $this->url . '/' . $filename;
  }
}

// vim: ft=php tabstop=2 shiftwidth=2
