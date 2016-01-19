<?php

/**
 * @file
 * Provides a class for using date fields within groups as time frame source.
 */

/**
 * This module's modifications to the OgTimeframeDateFieldHandler.
 */
class OgTimeframeGroupHandler extends OgTimeframeDateFieldHandler {

  /**
   * Provide state to our handler.
   */
  public function __construct($info) {
    parent::__construct($info);
    $this->setFieldInstance($this->groupType, $this->groupBundle, $info['#date_field_name']);
  }

  /**
   * Overrides OgTimeframeHandler::setGroup().
   *
   * Point to the entity where to retrieve the date field from.
   */
  public function setGroup($group) {
    parent::setGroup($group);
    $this->setEntityId($this->groupId);
  }

}
