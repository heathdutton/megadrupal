<?php

namespace Drupal\geoip_language_redirect;

class RedirectUserAgentTest extends \DrupalUnitTestCase {
  protected function createRedirect($agent = 'Mozilla') {
    $values = array(
      'userAgent' => $agent,
    );

    $api = $this->getMock('Drupal', array_merge(array('disableCache', 'redirect', 'setCookie', 'serveFromCache'),array_keys($values)));
    foreach ($values as $method => $value) {
      $api->expects($this->any())->method($method)->will($this->returnValue($value));
    }

    return new RedirectUserAgent($api);
  }
  public function testCommonUserAgentStrings() {
    $strings = array(
      'Mozilla/5.0 (X11; Linux x86_64; rv:17.0) Gecko/20130917 Firefox/17.0 Iceweasel/17.0.9' => NULL,
      'Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)' => FALSE,
      'facebookexternalhit/1.1 (+http://www.facebook.com/externalhit_uatext.php)' => FALSE,
      'Mozilla/4.0 (compatible; MSIE 6.01; Windows NT 6.0)' => NULL,
      'Mozilla/5.0 (X11; Linux i686) AppleWebKit/534.33 (KHTML, like Gecko) Ubuntu/9.10 Chromium/13.0.752.0 Chrome/13.0.752.0 Safari/534.33' => NULL,
    );
    foreach ($strings as $string => $result) {
      $this->assertEqual($result, $this->createRedirect($string)->checkAndRedirect(), $string);
    }
  }
}
