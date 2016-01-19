<?php

namespace Drupal\campaignion_recent_supporters;

class RequestParams {
  protected $params;
  public function __construct(array $params) {
    unset($params['q']);
    $this->params = $params + array(
      'limit' => 10,
      'name_display' => CAMPAIGNION_RECENT_SUPPORTERS_NAME_DISPLAY_DEFAULT,
      'hash' => '',
    );
  }
  public function hashKey() {
    $data = $this->params;
    unset($data['hash']);
    ksort($data);
    return drupal_hmac_base64(http_build_query($data), drupal_get_hash_salt());
  }
  public function isValid() {
    return $this->params['hash'] == $this->hashKey() && isset($this->params['backend']);
  }
  public function getParams() {
    return $this->params;
  }
  public function buildQuery() {
    $params = $this->params;
    $params['hash'] = $this->hashKey();
    return http_build_query($params);
  }
  public function getBackend() {
    $class = $this->params['backend'];
    return new $class();
  }
}
