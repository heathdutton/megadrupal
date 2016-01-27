<?php
// $Id$

class FieldRulesEntity extends Entity {
  protected function defaultLabel() {
    return $this->nameKey;
  }
  protected function defaultUri() {
    return array('path' => 'admin/structure/types/manage/' . $entity->identifier());
  }

  /**
   * Permanently saves the entity.
   *
   * @see entity_save()
   */
  public function save() {
    return node_type_save($this);
  }
}
