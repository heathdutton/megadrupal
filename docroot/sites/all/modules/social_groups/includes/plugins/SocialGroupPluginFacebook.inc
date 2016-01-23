<?php

/**
 * @file
 *  Definition of SocialGroupPluginFacebook class.
 */

/**
 * Base class for a social group.
 */
class SocialGroupPluginFacebook extends SocialGroupPluginJSON {
  
  public function __construct($group_info, $store_id = NULL) {
    parent::__construct($group_info, $store_id);
  }
  
  public function getData() {
    if (!$this->isEmpty()) {
      $group = $this->getGroup();
      $name = $this->getName();
      $cache_name = $name . '_' . $group[$name];
      $data = cache_get($cache_name, 'cache');
      if (!$data) {
        $data = $this->retreiveData();
        if ($data) {
          cache_set($cache_name, $data, 'cache', 3600);
        }
      }
      else {
        $data = $data->data;
      }

      return $this->theme($data->likes);
    }  
  }
  
  protected function theme($data) {
    $group = array(
      'count' => $data,
      'url' => $this->getGroupUrl(),
    );
    return theme('social_group_facebook', array('group' => $group));
  }
  
  protected function getUrl() {
    if (!$this->isEmpty()) {
      $group = $this->getGroup();
      $info = $this->getInfo();
      return $info['url'] . $group[$this->getName()];
    }  
  }
  
  protected function getGroupUrl() {
    if (!$this->isEmpty()) {
      $group = $this->getGroup();
      $info = $this->getInfo();
      return $info['group_url'] . $group[$this->getName()];
    }  
  }
  
  public function validate($params) {}
  
  public function isEmpty() {
    $group = $this->getGroup();
    if (empty($group[$this->getName()])) {
      return TRUE;
    }
    return FALSE;
  }
}