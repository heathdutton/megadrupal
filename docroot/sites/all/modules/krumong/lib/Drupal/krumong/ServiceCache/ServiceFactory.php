<?php

namespace Drupal\krumong\ServiceCache;
use Drupal\krumong as m;


class ServiceFactory {

  function createService($key, $cache) {
    $method = 'get_' . $key;
    return $this->$method($cache);
  }

  protected function get_devel($cache) {
    return new m\Devel($cache->main, $cache->mainAccessChecked);
  }

  protected function get_mainAccessChecked($cache) {
    if (user_access('access devel information')) {
      return $cache->main;
    }
    else {
      return new m\DoNothing;
    }
  }

  protected function get_main($cache) {
    return new m\Main($cache->requestCache, $cache->stdOut, $cache->cssJs);
  }

  protected function get_cssJs($cache) {
    return new m\Output\CssJs;
  }

  protected function get_requestCache($cache) {
    return new m\Output\SessionRequestCache('krumong_messages', $cache->cssJs);
  }

  protected function get_ajaxMessageSystem($cache) {
    $key = 'krumong_ajax_messages';
    if (!isset($_SESSION[$key])) {
      $_SESSION[$key] = array();
    }
    return new m\Output\AjaxMessageSystem($_SESSION[$key]);
  }

  protected function get_stdOut($cache) {
    return new m\Output\StdOut($cache->cssJs);
  }
}
