<?php

/**
 * @file
 * Smartling log.
 */

namespace Drupal\smartling\Wrappers;

/**
 * Class SmartlingEntityDataWrapper.
 */
class SmartlingEntityDataWrapper {

  private $entity;

  protected $file_name_manager;


  protected function buildXmlFileName() {
    return $this->file_name_manager->buildFileName($this);
  }


  protected function buildTranslatedXmlFileName() {
    return $this->file_name_manager->buildTranslatedFileName($this);
  }

  protected function defaultEntity() {
    return (object) array(
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
  }


  /**
   * Initialize.
   */
  public function __construct($file_name_manager) {
    $this->entity = $this->defaultEntity();

    $this->file_name_manager = $file_name_manager;
  }

  public function save() {
    smartling_entity_data_save($this->entity);
    return $this;
  }

  public function setEntity($entity) {
    $this->entity = $entity;
    if (!empty($entity)) {
      //Adds default values to an entity, but doesn't loose a link to the $entity param (as objects are passed by link).
      foreach ($this->defaultEntity() as $k => $v) {
        if (!isset($entity->{$k})) {
          $this->entity->{$k} = $v;
        }
      }
    }
    return $this;
  }

  public function getEntity() {
    return $this->entity;
  }

  public function isEmpty() {
    return empty($this->entity);
  }

  public function getEID() {
    return $this->entity->eid;
  }

  public function setEID($eid) {
    $this->entity->eid = $eid;
    return $this;
  }

  public function getRID() {
    return $this->entity->rid;
  }

  public function setRID($id) {
    $this->entity->rid = $id;
    return $this;
  }

  public function getStatus() {
    return $this->entity->status;

  }

  public function setStatus($status) {
    $this->entity->status = $status;
    return $this;
  }

  public function getFileName($do_save = TRUE) {
    if (empty($this->entity->file_name)) {
      $this->entity->file_name = $this->buildXmlFileName();
      if ($do_save) {
        $this->save();
      }
    }
    return $this->entity->file_name;
  }

  public function getFileTranslatedName($do_save = TRUE) {
    if (empty($this->entity->translated_file_name)) {
      $this->entity->translated_file_name = $this->buildTranslatedXmlFileName();
      if ($do_save) {
        $this->save();
      }
    }
    return $this->entity->translated_file_name;
  }

  public function getTitle() {
    return $this->entity->title;
  }

  public function setTitle($title) {
    $this->entity->title = $title;
    return $this;
  }

  public function getEntityType() {
    return $this->entity->entity_type;
  }

  public function setEntityType($entity_type) {
    $this->entity->entity_type = $entity_type;
    return $this;
  }


  public function getProgress() {
    return $this->entity->progress;
  }

  public function setProgress($progress) {
    $this->entity->progress = $progress;
    return $this;
  }

  public function getBundle() {
    return $this->entity->bundle;
  }

  public function setBundle($bundle) {
    $this->entity->bundle = $bundle;
    return $this;
  }

  public function getOriginalLanguage() {
    return $this->entity->original_language;
  }

  public function setOriginalLanguage($original_language) {
    $this->entity->original_language = $original_language;
    return $this;
  }

  public function getTargetLanguage() {
    return $this->entity->target_language;
  }

  public function setTargetLanguage($target_language) {
    $this->entity->target_language = $target_language;
    return $this;
  }

  public function getDownload() {
    return $this->entity->download;
  }

  public function setDownload($download) {
    $this->entity->download = $download;
    return $this;
  }

  public function getContentHash() {
    return $this->entity->content_hash;
  }

  public function setContentHash($content_hash) {
    $this->entity->content_hash = $content_hash;
    return $this;
  }

  public function getSubmitter() {
    return $this->entity->submitter;
  }

  public function setSubmitter($submitter = 0) {
    if ($submitter == 0) {
      global $user;
      $submitter = $user->uid;
    }

    $this->entity->submitter = $submitter;
    return $this;
  }

  public function getSubmissionDate() {
    return $this->entity->submission_date;
  }

  public function setSubmissionDate($submission_date) {
    $this->entity->submission_date = $submission_date;
    return $this;
  }

  public function getLastModified() {
    return $this->entity->last_modified;
  }

  public function setLastModified($last_modified) {
    $this->entity->last_modified = $last_modified;
    return $this;
  }

  /*
   * Create Smartling entity.
   *
   * @param stdClass $drupal_entity
   *   Drupal content entity.
   * @param string $entity_type
   *   Drupal entity type machine name.
   * @param string $origin_language
   *   Language key of source drupal content entity.
   * @param string $target_language
   *   Target language key for smartling entity.
   * @param array $default_options
   *   Optional params that will override the ones in a function.
   *   This feature is use in interface_translation module to set a bundle for example.
   *
   * @return SmartlingEntityDataWrapper
   */
  public function createFromDrupalEntity($drupal_entity, $entity_type, $origin_language, $target_language, $default_options = array()) {
    global $user;

    $wrapper = entity_metadata_wrapper($entity_type, $drupal_entity);
    $entity_data = $default_options + array(
        'rid' => $wrapper->getIdentifier(),
        'entity_type' => $entity_type,
        'title' => $wrapper->label(),
        'bundle' => $wrapper->getBundle(),
        'original_language' => $origin_language,
        'target_language' => $target_language,
        'progress' => 0,
        'translated_file_name' => FALSE,
        'submitter' => $user->uid,
        'submission_date' => time()
      );
    $entity_data = (object) $entity_data;

    $this->setEntity($entity_data);
    $this->save();

    return $this;
  }

  public function loadSingleByConditions(array $conditions) {
    $entity_type = 'smartling_entity_data';
    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    if ($conditions) {
      foreach ($conditions as $name => $value) {
        $query->propertyCondition($name, $value);
      }
    }

    $query->range(0, 1);
    $result = $query->execute();
    if ($result) {
      $id = key($result[$entity_type]);
      $entities = entity_load($entity_type, array($id));
      $this->setEntity(reset($entities));
    }
    else {
      $this->setEntity(FALSE);
    }

    return $this;
  }

  public function orCreateFromDrupalEntity($drupal_entity, $entity_type, $origin_language, $target_language, $default_options = array()) {
    if (!$this->getEntity()) {
      $this->createFromDrupalEntity($drupal_entity, $entity_type, $origin_language, $target_language, $default_options);
    }
    return $this;
  }

  function setStatusByEvent($event) {
    if (is_null($event)) {
      return;
    }

    $status = $this->getStatus();
    switch ($event) {
      case SMARTLING_STATUS_EVENT_SEND_TO_UPLOAD_QUEUE:
        if (empty($status) || ($status == SMARTLING_STATUS_CHANGE)) {
          $status = SMARTLING_STATUS_IN_QUEUE;
        }
        break;

      case SMARTLING_STATUS_EVENT_UPLOAD_TO_SERVICE:
        $status = SMARTLING_STATUS_IN_TRANSLATE;
        break;

      case SMARTLING_STATUS_EVENT_DOWNLOAD_FROM_SERVICE:
      case SMARTLING_STATUS_EVENT_UPDATE_FIELDS:
        if ($status != SMARTLING_STATUS_CHANGE && $this->getProgress() == 100) {
          $status = SMARTLING_STATUS_TRANSLATED;
        }
        break;

      case SMARTLING_STATUS_EVENT_NODE_ENTITY_UPDATE:
        $status = SMARTLING_STATUS_CHANGE;
        break;

      case SMARTLING_STATUS_EVENT_FAILED_UPLOAD:
        $status = SMARTLING_STATUS_FAILED;
        break;
    }

    $this->setStatus($status);
    return $this;
  }

  public function loadByID($eid) {
    $this->entity = entity_load_single('smartling_entity_data', $eid);
    return $this;
  }

  public function delete() {
    smartling_entity_data_delete($this->entity);
  }
}
