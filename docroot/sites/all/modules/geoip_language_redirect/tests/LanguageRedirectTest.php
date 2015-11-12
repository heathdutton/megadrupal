<?php

namespace Drupal\geoip_language_redirect;

class LanguageRedirectTest extends \Drupal\Tests\DrupalUnitTestCase {
  protected function createRedirector($overrides = array()) {
    $defaults = array(
      'baseUrl' => 'http://some.baseurl.example.com',
      'referer' => 'http://non-macthing.example.com',
      'readCookie' => '',
      'currentPath' => '/somepath',
      'currentParameters' => array(),
      'currentLanguage' => 'en',
      'checkAccess' => TRUE,
      'defaultLanguage' => 'en',
      'switchLinks' => array(
        'de' => array('href' => '/somepath-de', 'language' => 'de'),
      ),
      'getCountry' => 'AT',
      'getMapping' => array('AT' => 'de', 'GB' => 'en'),
      'userAgent' => 'Mozilla',
    );
    $values = $overrides + $defaults;

    $api = $this->getMock('Drupal', array_merge(array('disableCache', 'redirect', 'setCookie', 'serveFromCache'),array_keys($values)));
    foreach ($values as $method => $value) {
      $api->expects($this->any())->method($method)->will($this->returnValue($value));
    }

    $lr = new LanguageRedirect(
      $api,
      array('RedirectReferer', 'RedirectUserAgent'),
      array('RedirectCookie', 'RedirectCountry')
    );
    return array($api, $lr);
  }
  
  public function testNoRedirectIfRefererMatchesBaseUrl() {
    list($api, $lr) = $this->createRedirector(array(
      'referer' => 'http://some.baseurl.example.com',
    ));
    $api->expects($this->never())->method('redirect');
    $api->expects($this->never())->method('disableCache');
    $lr->hook_boot();
    $lr->hook_language_init();
  }

  public function testRedirectWithCookie() {
    list($api, $lr) = $this->createRedirector(array(
      'readCookie' => 'de',
    ));
    $api->expects($this->once())->method('disableCache');
    $api->expects($this->once())->method('redirect');
    $api->expects($this->once())->method('setCookie')->with($this->equalTo('de'));
    $lr->hook_boot();
    $lr->hook_language_init();
  }

  public function testRedirectWithGeoIp() {
    list($api, $lr) = $this->createRedirector(array(
    ));
    $api->expects($this->once())->method('disableCache');
    $api->expects($this->once())->method('redirect')->with($this->equalTo('/somepath-de'));
    $api->expects($this->once())->method('setCookie')->with($this->equalTo('de'));
    $lr->hook_boot();
    $lr->hook_language_init();
  }

  public function testNoRedirectWithCurrentLanguageInCookie() {
    list($api, $lr) = $this->createRedirector(array(
      'readCookie' => 'en',
    ));
    $api->expects($this->once())->method('disableCache');
    $api->expects($this->never())->method('redirect');
    $lr->hook_boot();
    $lr->hook_language_init();
  }
  
  public function testNoRedirectWithCountryHavingCurrentLanguage() {
    list($api, $lr) = $this->createRedirector(array(
      'getCountry' => 'GB',
    ));
    $api->expects($this->once())->method('disableCache');
    $api->expects($this->never())->method('redirect');
    $lr->hook_boot();
    $lr->hook_language_init();
  }
}
