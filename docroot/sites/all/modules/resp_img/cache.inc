<?php
if (!defined('RESP_IMG_COOKIE')) {
  define('RESP_IMG_COOKIE', 'respimg');
}

class RespImgPageCache extends DrupalDatabaseCache {
  function  set($cid, $data, $expire = CACHE_PERMANENT) {
    if ($this->checkCookie()) {
      $cid .= '_' . $_COOKIE[RESP_IMG_COOKIE];
    }
    parent::set($cid, $data, $expire);
  }

  function  getMultiple(&$cids) {
    if ($this->checkCookie()) {
      foreach ($cids as $key => $cid) {
        $cids[$key] = $cid . '_' . $_COOKIE[RESP_IMG_COOKIE];
      }
    }
    return parent::getMultiple($cids);
  }

  function  clear($cid = NULL, $wildcard = FALSE) {
    if ($this->checkCookie() && !empty($cid) && !$wildcard) {
      if (is_array($cid)) {
        foreach ($cid as $key => $c) {
          $cid[$key] = $c . '_' . $_COOKIE[RESP_IMG_COOKIE];
        }
      }
      else {
        $cid .= '_' . $_COOKIE[RESP_IMG_COOKIE];
      }
    }
    parent::clear($cid, $wildcard);
  }

  function checkCookie() {
    return (isset($_COOKIE[RESP_IMG_COOKIE]) && is_numeric($_COOKIE[RESP_IMG_COOKIE]));
  }
}
