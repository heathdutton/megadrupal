<?php

/**
 * @file
 * Definition of SalsaEntityTellAFriend class.
 */

/*
 * SalsaEntityTellAFriend class.
 */
class SalsaEntityTellAFriend extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content['tell_a_friend_form'] = drupal_get_form('salsa_tell_a_friend_form', $this);
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
