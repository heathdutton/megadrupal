<?php

/**
 * Variable realm store for Language realms, with support for language heirarchy.
 */
class LanguageHierarchyI18nVariableStoreRealmStore extends VariableStoreRealmStore {

  /**
   * The store of our parent realm, if we have one.
   *
   * @var LanguageHierarchyI18nVariableStoreRealmStore
   */
  protected $parentStore;

  /**
   * Set the store of our realm's parent realm.
   */
  public function setParentStore($parentStore) {
    $this->parentStore = $parentStore;
  }

  public function variable_get($name, $default = NULL) {
    if ($this->parentStore) {
      return parent::variable_get($name, $this->parentStore->variable_get($name, $default));
    }
    else {
      return parent::variable_get($name, $default);
    }
  }

  public function variable_list() {
    if ($this->parentStore) {
      return parent::variable_list() + $this->parentStore->variable_list();
    }
    else {
      return parent::variable_list();
    }
  }
}