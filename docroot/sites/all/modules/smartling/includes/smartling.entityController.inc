<?php

/**
 * @file
 * Smartling entity controller.
 */

/**
 * SmartlingEntityData class.
 */
class SmartlingEntityData extends Entity {

  protected $processor;

  /**
   * Get default label.
   *
   * @return string
   *   Return label.
   */
  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * Get default uri string for smartling entity.
   *
   * @return string
   *   Return default uri string.
   */
  protected function defaultUri() {
    return array('path' => 'admin/config/regional/smartling/entity-data/' . $this->identifier());
  }

}

/**
 * SmartlingEntityDataController class.
 */
class SmartlingEntityDataController extends EntityAPIController {

  /**
   * Create smartling entity object.
   *
   * @param array $values
   *   Entity values.
   *
   * @return object
   *   Return smartling entity object.
   */
  public function create(array $values = array()) {
    $values += array(
      'rid' => '',
      'entity_type' => '',
      'bundle' => '',
      'original_language' => '',
      'target_language' => '',
      'file_name' => '',
      'translated_file_name' => '',
      'progress' => '',
      'submitter' => '',
      'submission_date' => '',
      'download' => '',
      'status' => '',
      'content_hash' => '',
    );

    return parent::create($values);
  }

}
