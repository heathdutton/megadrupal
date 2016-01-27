<?php

/**
 * @file
 * Definition of SalsaEntityEvent class.
 */

/*
 * SalsaEntityEvent class.
 */
class SalsaEntityEvent extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content['event_form'] = drupal_get_form('salsa_event_form', $this, $view_mode);
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
