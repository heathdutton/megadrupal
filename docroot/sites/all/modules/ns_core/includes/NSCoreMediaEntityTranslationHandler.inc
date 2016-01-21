<?php

/**
 * @file
 * Custom entity translation for media.
 */

/**
 * Custom handler for file translation.
 * This is a temporary measure to support an older version of entity translation,
 * since we can't upgrade to the latest version until #1807776 is in D8, and
 * backported to D7.
 */
class NSCoreMediaEntityTranslationHandler extends EntityTranslationDefaultHandler {

  /**
   * @see EntityTranslationDefaultHandler::__construct()
   */
  public function __construct($entity_type, $entity_info, $entity, $entity_id) {
    parent::__construct('file', $entity_info, $entity, $entity_id);
  }
}
