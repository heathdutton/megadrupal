<?php

class AiringController extends EntityAPIController {

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
    $wrapper = entity_metadata_wrapper('airing', $entity);
    //$content['author'] = array('#markup' => t('Created by: !author', array('!author' => $wrapper->uid->name->value(array('sanitize' => TRUE)))));

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }
}


/**
 * Airing class.
 */
class Airing extends Entity {
  protected function defaultLabel() {
    // removed entity defined title to allow Feeds to map w/ default entity processor 
    //return $this->title;
    
    return $this->field_airing_title[LANGUAGE_NONE][0]['value'];
  }

  protected function defaultUri() {
    return array('path' => 'airing/' . $this->identifier());
  }
}