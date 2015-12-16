<?php

/**
 * The class used for review entities.
 */
class ReviewEntity extends Entity {
  /**
   * Change the default URI from default/id to project/id
   */
  protected function defaultUri() {
    return array('path' => 'review/' . $this->vid);
  }

  public function defaultLabel() {
    if (isset($this->type) && isset($this->eid) && isset($this->uid)) {
      $reviewed_entities = entity_load($this->type, array($this->eid));
      $reviewed_entity = $reviewed_entities[$this->eid];

      $reviewed_entity_wrapper = entity_metadata_wrapper($this->type, $reviewed_entity);

      $account = user_load($this->uid);

      return t('!name review by !user', array(
        '!name' => $reviewed_entity_wrapper->label(),
        '!user' => $account->name,
      ));
    }
    else {
      return t('New review');
    }
  }

  /**
   * Set up the object instance on construction or unserializiation.
   */
  protected function setUp() {
    parent::setUp();

    if (isset($this->type) && isset($this->eid)) {
      $reviewed_entity_wrapper = entity_metadata_wrapper($this->type, $this->eid);
      $this->bundle = $this->type . '__' . $reviewed_entity_wrapper->getBundle();
    }
  }

  public function publish() {
    entity_revision_set_default('review', $this);

    $this->status = 1;
    $this->save();
  }

  public function unpublish() {
    $this->status = 0;
    $this->save();
  }
}
