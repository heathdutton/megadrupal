<?php
/**
 * @file
 * Variable Realm controller.
 */

/**
 * Controller for Language realms, with support for language heirarchy.
 */
class LanguageHierarchyI18nVariableLanguageRealm extends I18nVariableLanguageRealm {

  /**
   * Get a reference to the parent realm's store.
   */
  protected function getParentRealmStore($realm_key) {
    // If we have a parent language, then get those variables and merge them in.
    if ($parent_key = language_hierarchy_get_parent($realm_key)) {
      if ($parent_controller = $this->getParentKeyVariableRealmController($parent_key->language)) {
        return $parent_controller->getStore($parent_key->language);
      }
    }
    else {
      return NULL;
    }
  }

  /**
   * Create the store for this realm, passing in the parent store.
   */
  protected function createStore($realm_key, $variables = NULL) {
    $store = parent::createStore($realm_key, $variables);
    $store->setParentStore($this->getParentRealmStore($realm_key));
    return $store;
  }


  protected $parentKeyRealmController = array();
  /**
   * Get the realm controller belonging to the given key.
   *
   * Note that this result is stored in a instance variable, so it it not
   * safe to call this method with different $key parameters.
   *
   * @param $key
   *   The key of our parent in the language realm.
   *
   * @return VariableRealmControllerInterface
   *   The variable realm controller for the given parent key.
   */
  protected function getParentKeyVariableRealmController($key) {
    if (!isset($this->parentKeyRealmController[$key])) {
      $info = variable_realm_info($this->realm_name);
      $class = !empty($info['controller class']) ? $info['controller class'] : 'VariableRealmDefaultController';
      if ($this->parentKeyRealmController[$key] = class_exists($class) ? new $class($this->realm_name) : FALSE) {
        $this->parentKeyRealmController[$key]->enable($key);
      }
    }
    return $this->parentKeyRealmController[$key];
  }
}
