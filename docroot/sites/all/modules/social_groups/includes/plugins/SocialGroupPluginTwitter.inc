<?php

/**
 * @file
 *  Definition of SocialGroupPluginTwitter class.
 */

/**
 * Base class for a social group.
 */
class SocialGroupPluginTwitter extends SocialGroupPlugin {
  
  public function __construct($group_info, $store_id = NULL) {
    parent::__construct($group_info, $store_id);
  }
  
  /**
   * Retreive data from a social group via the "Twitter" module. 
   */
  protected function retreiveData() {
    module_load_include('inc', 'twitter');

    $group = $this->getGroup();
    return twitter_account_load($group[$this->getName()]);
  }
  
  public function getData() {
    if (module_exists('twitter')) {
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

        return $this->theme($data->followers_count);
      }
    }
  }
  
  protected function theme($data) {
    $group = array(
      'count' => $data,
      'url' => $this->getGroupUrl(),
    );
    return theme('social_group_twitter', array('group' => $group));
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
  
  /**
   * Get form element.
   * 
   * @return array. 
   */
  public function getForm() {
    $items = array();
    if (module_exists('twitter')) {
      $info = $this->getInfo();
      $group = $this->getGroup();
      $items[$info['name']] = array(
        '#type' => 'textfield',
        '#title' => $info['title'],
        '#description' => !empty($info['description']) ? $info['description'] : NULL,
        '#default_value' => $group[$info['name']],
      );
    }  

    return $items;
  }
  
  /**
   * Validate callabck.
   * 
   * @param mixed $param
   * @return boolean 
   */
  public function validate($params) {
    if (module_exists('twitter')) {
      module_load_include('inc', 'twitter');

      $name = $this->getName();
      if (!empty($params[$name])) {
        $result = array(
          'field' => $name,
          'message' => t("This account was not found. Please add the Twitter account <a href='!url'>here.</a>",
            array('!url' => url('admin/config/services/twitter'))),
        );    
        $twitter_account = twitter_account_load($params[$name]);
        if (empty($twitter_account)) {
          return $result;      
        }
      }
    }
  }
  
  public function isEmpty() {
    $group = $this->getGroup();
    if (empty($group[$this->getName()])) {
      return TRUE;
    }
    return FALSE;
  }
}