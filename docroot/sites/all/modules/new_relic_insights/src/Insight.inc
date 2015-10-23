<?php

/**
 * @file
 * Defines the Insight entity.
 */

class Insight extends Entity {

  public function __get($property) {
    if (isset($this->entity_data->$property)) {
      return $this->entity_data->$property;
    }
    else {
      return NULL;
    }
  }

  public function __isset($property) {
    return isset($this->entity_data->$property);
  }

  protected function defaultUri() {
    return array('path' => 'insight/' . $this->identifier());
  }

  public function label() {
    return t('Insight @eid (@iid)', array(
      '@eid' => $this->eid,
      '@iid' => $this->remote_id,
    ));
  }

}
