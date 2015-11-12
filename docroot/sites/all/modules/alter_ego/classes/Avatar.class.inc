<?php

/**
 * The class used for avatar entities
 */
// Ensure that entity classes are loaded.  Class autoloading does not work on enable and unisntall.
module_load_include('inc', 'entity', 'includes/entity');  // Entity Class
module_load_include('inc', 'entity', 'includes/entity.controller');  // EntityAPIController Class
class Avatar extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'avatar');
  }

  /**
   *
   * @see Entity::save()
   * @see entity_save()
   */
  public function save() {
    // Set default language.
    if (empty($this->language)) {
      $this->language = LANGUAGE_NONE;
    }

    if (!empty($this->uid)) {
      // If this user hasn't set a main toon.  Use this one.
      if (alter_ego_get_main_aid($this->uid) === FALSE) {
        $this->is_main = TRUE;
        drupal_static_reset('mmoguild_get_main_aid');
      }
    }
    return parent::save();
  }


  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'avatar/' . $this->aid);
  }


}
