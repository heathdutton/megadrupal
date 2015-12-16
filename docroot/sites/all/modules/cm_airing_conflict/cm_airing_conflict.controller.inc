<?php

class AiringConflictController extends EntityAPIController {

  public function create(array $values = array()) {
    global $user;
    $values += array(
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $user->uid,
    );
    return parent::create($values);
  }

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $wrapper = entity_metadata_wrapper('airingconflict', $entity);
    
    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}


/**
 * AiringConflict class.
 */
class AiringConflict extends Entity {
  protected function defaultLabel() {
    // This is normally a title, but in this case we are using id 
    return $this->airing_conflict_id;
  }

  protected function defaultUri() {
    return array('path' => 'airing-conflict/' . $this->identifier());
  }
}