<?php

/**
 * @file
 *  Definition of SocialGroupPlugin class.
 */

/**
 * Base class for a social group.
 */
abstract class SocialGroupPlugin {
  
  private $storage_name = 'social_groups';
  private $group_info;
  
  /**
   * Get all groups.
   * 
   * @return array. 
   */
  private function getGroups() {
    $groups = variable_get($this->storage_name, array());
    return $groups;
  }
  
  /**
   * Set groups.
   * 
   * @param array groups.
   */
  private function setGroups($groups = array()) {
    variable_set($this->storage_name, $groups);
  }
  
  
  /**
   * Definition a group.
   *  
   * @param string $group_name
   *  Machine name of the group.
   * @param mixed $store_id 
   *  Id of the storage.
   */
  protected function __construct($group_info, $store_id = NULL) {
    $this->group_info = $group_info;
    if ($store_id) {
      $this->storage_name .= '_' . $store_id;
    }
  }
  
  /**
   * Get group's info.
   * 
   * @return string
   */
  final protected function getInfo() {
    return $this->group_info;
  }
  
  /**
   * Retreive data from a social group. 
   */
  abstract protected function retreiveData();
  
  /**
   * Theming function.
   * 
   * @param array $data 
   *  Social group data.
   */
  abstract protected function theme($data);
  
  /**
   * Get url of API.
   */
  abstract protected function getUrl();
  
  /**
   * Get url of the social group.
   */
  abstract protected function getGroupUrl();

      /**
   * Save the group.
   * 
   * @param array $params 
   *  Group's params.
   */
  final public function setGroup($params = array()) {
    $check = $this->validate($params);
    if ($check) {
      return $check;
    }
    $name = $this->getName();
    $groups = $this->getGroups();
    $groups[$name] = $params;
    $this->setGroups($groups);
  }
  
  /**
   * Get the group.
   * 
   * @return array.
   */
  final public function getGroup() {
    $name = $this->getName();
    $groups = $this->getGroups();
    if (isset($groups[$name])) {
      return $groups[$name];
    }
  }
  
  /**
   * Get group's name.
   * 
   * @return string
   */
  final public function getName() {
    return $this->group_info['name'];
  }
  
  /**
   * Get form element.
   * 
   * @return array. 
   */
  public function getForm() {
    $info = $this->getInfo();
    $group = $this->getGroup();
    $items = array();
    $items[$info['name']] = array(
      '#type' => 'textfield',
      '#title' => $info['title'],
      '#description' => !empty($info['description']) ? $info['description'] : NULL,
      '#default_value' => $group[$info['name']],
    );
    
    return $items;
  }
  
  /**
   * Validate callabck.
   * 
   * @param mixed $param
   * @return boolean 
   */
  public function validate($params) {
    $name = $this->getName();
    if (!empty($params[$name]) && !is_numeric($params[$name])) {
      $result = array(
        'field' => $name, 
        'message' => t('This field should be a numeric.')
      );
      return $result;
    }
  }
  
  /**
   * Get social grop data.
   * 
   * @return html
   */
  public function getData() {
    $data = $this->retreiveData();
    return $this->theme($data);
  }
  
  /**
   * Check is empty group or not. 
   */
  abstract public function isEmpty();
  
}