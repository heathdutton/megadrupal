<?php
/**
 * @file
 * SSGA Class implementation
 */

/**
 * Class created for serversidegoogleanalytics
 * http://code.google.com/p/serversidegoogleanalytics/
 *
 * Curl modification by  antares500
 * http://code.google.com/p/serversidegoogleanalytics/issues/detail?id=3
 *
 */

class ssga {

  private $beaconURL = "http://www.google-analytics.com/__utm.gif"; // Beacon
  private $defType = "event";
  private $utmwv = "4.3"; // Analytics version
  private $utmn, // Random number
          $utmhn, // Host name (www.elements.at)
          $utmcs, // Charset
          $utmul, // Language
          $utmdt, // Page title
          $utmhid, // Random number (unique for all session requests)
          $utmp, // Pageview
          $utmac, // Google Analytics account
          $utmt, // Analytics type (event)
          $utmcc; //Cookie related variables

  private $eventString;  // Internal structure of the complete event string

  public function __construct() {
    $this->setUtmhid();
    $this->setCharset();
    $this->setCookieVariables();
    $this->utmhn = $_SERVER['HTTP_HOST']; // Host name (www.elements.at)
    $this->utmcs = 'UTF-8'; // Charset
    $this->utmul = $_SERVER['HTTP_ACCEPT_LANGUAGE']; // Language
  }

  private function setCookieVariables() {
    $today = time();
    $this->utmcc = '__utma=1.' . rand(10000000, 99999999) . "00145214523" . '.' . rand(1000000000, 2147483647) . '.' . $today . '.' . $today . '.15;+__utmz=1.' . $today . '.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none);';
  }

  public function setEvent($category, $action, $label = "", $value = "") {

    $this->eventString =
      "5(" . ((string) $category) . "*" . ((string) $action) .
      ($label ? "*" . ((string) $label ) . ")" : ")") .
      (intval($value) != 0 ? "(" . ((int) intval($value) ) . ")" : '' );

  }

  //get data
  private function getCookieVariables() { return $this->utmcc; }
  private function getEventString()     { return $this->eventString; }
  private function getAnalyticsType()   { return $this->utmt; }
  private function getAccountId()       { return $this->utmac; }
  private function getPageView()        { return $this->utmp; }
  private function getVersion()         { return $this->utmwv; }
  private function getGetUniqueId()     { return $this->utmhid; }
  private function getCharset()         { return $this->utmcs; }
  private function getPageTitle()       { return $this->utmdt; }
  private function getLanguage()        { return $this->utmul; }
  private function getHostName()        { return $this->utmhn; }
  private function getRandomNumber()    { return mt_rand(100000000, 999999999); }

  //set data
  public  function setAccountId($accountId)       { $this->utmac = $accountId; }
  public  function setPageView($pageView = "" )   { $this->utmp   = $pageView; }
  public  function setCharset($charset  = "")     { $this->utmcs = isset($charSet) ? $charset :"UTF-8"; }
  public  function setLanguage($language  = "")   { $this->utmul = isset($language) ? $language:"en-us"; }
  public  function setPageTitle($pageTitle  = "") { $this->utmdt = $pageTitle; }
  public  function setHostName($hostName = "")    { $this->utmhn = $hostName; }
  public  function setVersion($version = "")      { if ($version) $this->utmwv = $version; }
  private  function setAnalyticsType($type = "")  { $this->utmt   = $type?$type:$this->defType; }
  private  function setUtmhid()                   { $this->utmhid= mt_rand(100000000, 999999999); }

  public function create() {

    if ($this->getEventString()) { #Event
      $this->setAnalyticsType();
      $parameters = array(
        'utmwv'  => $this->getVersion(),
        'utmn'   => $this->getRandomNumber(),
        'utmhn'  => $this->getHostName(),
        'utmt'   => 'event',
        'utme'   => $this->getEventString(),
        'utmcs'  => $this->getCharset(),
        'utmul'  => $this->getLanguage(),
        #  "utmdt"  => $this->getPageTitle(),
        'utmhid' => $this->getGetUniqueId(),
        #  "utmp"  => $this->getPageView(),
        'utmac'  => $this->getAccountId(),
        'utmcc'  => $this->getCookieVariables()
      );
    }
    else { #PageView
      $parameters = array(
        'utmwv'  => $this->getVersion(),
        'utmn'   => $this->getRandomNumber(),
        'utmhn'  => $this->getHostName(),
        'utmcs'  => $this->getCharset(),
        'utmul'  => $this->getLanguage(),
        'utmdt'  => $this->getPageTitle(),
        'utmhid' => $this->getGetUniqueId(),
        'utmp'   => $this->getPageView(),
        'utmac'  => $this->getAccountId(),
        'utmcc'  => $this->getCookieVariables()
      );
    }

    $c = curl_init();
    curl_setopt($c, CURLOPT_USERAGENT, $_SERVER['HTTP_USER_AGENT']);
    if ($_SERVER['HTTP_REFERER']) curl_setopt($c, CURLOPT_REFERER,  $_SERVER['HTTP_REFERER']);
    curl_setopt($c, CURLOPT_RETURNTRANSFER,  TRUE);
    curl_setopt($c, CURLOPT_URL, $this->beaconURL . '?' . http_build_query($parameters));
    #ip
    #curl_setopt($c, CURLOPT_INTERFACE, $_SERVER['REMOTE_ADDR'] );
    #curl_setopt($c, CURLOPT_HTTPHEADER, array("REMOTE_ADDR: " . $_SERVER['REMOTE_ADDR'], "HTTP_X_FORWARDED_FOR: " . $_SERVER['REMOTE_ADDR']));

    $o =  curl_exec($c);
    curl_close($c);

    return $o;
  }
}