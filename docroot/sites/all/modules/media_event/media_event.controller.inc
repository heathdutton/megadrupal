<?php

class MediaEventController extends EntityAPIController {
  public function create(array $values = array()) {
    global $user;
    $values += array(
      'title' => '',
      'description' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }
}


/**
 * MediaEvent class.
 */
class MediaEvent extends Entity {
  protected function defaultLabel() {
    return $this->field_media_event_title[LANGUAGE_NONE][0]['value'];
  }

  protected function defaultUri() {
    return array('path' => 'media-event/' . $this->identifier());
  }
}