<?php

namespace Drupal\aws_glacier\Entity\Job;

use Drupal\aws_glacier\Entity\Archive\Archive;
use Drupal\aws_glacier\Entity\GlacierEntity;
use Drupal\aws_glacier\Entity\Vault\Vault;

/**
 * Class Job
 * @package Drupal\aws_glacier\Entity\Job
 */
class Job extends GlacierEntity {

  /**
   * Primary Id.
   * @var int
   */
  public $id = 0;

  /**
   * @var string
   * The ID of the job.
   */
  public $jobId = 'dummy';

  /**
   * @var string
   * The name of the vault.
   */
  public $vaultName;

  /**
   * @var string
   * When initiating a job to retrieve a vault inventory,
   * you can optionally add this parameter to your request to specify the output format.
   * If you are initiating an inventory job and do not specify a Format field,
   * JSON is the default format. Valid Values are "CSV" and "JSON".
   */
  public $Format;

  /**
   * @var string
   * The job type. You can initiate a job to retrieve an archive or get an inventory of a vault.
   * Valid Values are "archive-retrieval" and "inventory-retrieval".
   */
  public $Type;

  /**
   * @var string
   * The ID of the archive that you want to retrieve. This field is required only
   * if Type is set to archive-retrieval. An error occurs if you specify this request parameter
   * for an inventory retrieval job request.
   */
  public $ArchiveId;

  /**
   * @var string
   * The optional description for the job. The description must be less than or equal to 1,024 bytes.
   * The allowable characters are 7-bit ASCII without control codes-specifically,
   * ASCII values 32-126 decimal or 0x20-0x7E hexadecimal.
   */
  public $Description;

  /**
   * @var string $permission
   * Permission name for user_access() etc.
   */
  static public $permission = 'adminster_glacier_job';

  /**
   * @var array $metadata
   */
  protected $metadata;

  /**
   * @param array $metadata
   *
   * @return $this
   */
  public function setMetadata(array $metadata = array()) {
    $this->_setWrapper();
    $this->metadata = $metadata;
    if (!empty($metadata['JobId'])) {
      unset($metadata['JobId']);
    }
    foreach ($metadata as $key => $value) {
      $field = 'field_' . strtolower($key);
      try{
        $this->wrapper->{$field}->set($value);
      }
      catch (\Exception $e) {
        #dpm($e->getMessage());
      }

    }
    return $this;
  }

  /**
   * @var mixed
   * @see entity_metadata_wrapper().
   */
  protected $wrapper;

  /**
   * Constructer.
   *
   * @param array $values
   *   Property values.
   *
   * @return $this
   */
  public function __construct($values = array()) {
    $values['uniqueProperty'] = 'jobId';
    parent::__construct($values, 'glacier_job');
    return $this;
  }

  /**
   * Sets the wrapper from the entity api for this entity.
   *
   * @return $this
   */
  function _setWrapper() {
    $this->wrapper = entity_metadata_wrapper($this->entityType(), $this);
    return $this;
  }

  /**
   * {@inheritDoc}
   */
  public function defaultUri() {
    return array(
      'path' => AWS_GLACIER_ADMIN_PATH . '/jobs/' . $this->id,
    );
  }

  /**
   * @return $this
   * @throws \Exception
   */
  function presave() {
    $vault = $this->_setWrapper()->wrapper->field_vault_ref->value();
    if (empty($this->vaultName)) {
      if ($vault instanceof Vault) {
        $this->vaultName = $vault->VaultName;
      }
      else{
        throw new \Exception(t('The field field_vault_ref is not a Vault'));
      }
    }
    else {
      if (empty($vault)) {
        /** @var \Drupal\aws_glacier\Entity\Vault\Vault $vault */
        $vault = entity_create('glacier_vault', array('VaultName' => $this->vaultName));
        $vault = $vault->loadByUniqueProperty();
        $this->wrapper->field_vault_ref->set((string) $vault->identifier());
      }
    }

    $archive = $this->wrapper->field_archive_ref->value();
    if (empty($this->ArchiveId)) {
      if ($archive instanceof Archive) {
        $this->ArchiveId = $archive->archiveId;
      }
      else if ($this->Type == 'archive-retrieval') {
        throw new \Exception(t('The field field_archive_ref is not a Archive'));
      }
    }
    else {
      if (empty($archive)) {
        /** @var \Drupal\aws_glacier\Entity\Archive\Archive $archive */
        $archive = entity_create('glacier_archive', array('archiveId' => $this->ArchiveId));
        $archive = $archive->loadByUniqueProperty();
        $this->wrapper->field_archive_ref->set((string) $archive->identifier());
      }
    }
    return $this;
  }

  /**
   * {@inheritDoc}
   */
  public function save() {
    $this->presave();
    if (isset($this->is_new) && $this->jobId == 'dummy') {
      $command = new Command('InitiateJob');
      $command->setJob($this->wrapper->value());
      try{
        $command->run();
      }
      catch(\Exception $e) {
        throw $e;
      }
      $data = $command->getData();

      if (is_array($data)) {
        foreach ($data as $property => $value) {
          $this->{$property} = $value;
        }
      }
    }
    try{
      if (empty($this->metadata)) {
        $command = new Command('DescribeJob');
        $command->setJob($this);
        $command->run();
        $this->setMetadata($command->getData());
      }
    }
    catch(\Exception $e) {
      throw $e;
    }
    if ($this->metadata['Completed']) {
      if ($this->Type == 'archive-retrieval') {
        $output = new GetOutput();
        $file = $output->setJob($this)
          ->setArchive($this->wrapper->field_archive_ref->value())
          ->run()
          ->getFileEntity();
        $this->wrapper->field_joboutput_ref->set($file->fid);
      }
    }
    return parent::save();
  }

  /**
   * Loads the Job of a vault inventory.
   *
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $vault
   *
   * @return \Drupal\aws_glacier\Entity\Job\Job
   */
  static public function loadForVault(Vault $vault) {
    $cache = &drupal_static(__METHOD__);
    /** @var \Drupal\aws_glacier\Entity\Job\Job $Job */
    $Job = new static();
    if (empty($cache[$vault->VaultName])) {
      $cache[$vault->VaultName] = $Job;

      $query = new \EntityFieldQuery();
      $query->entityCondition('entity_type', $Job->entityType())
      ->propertyCondition('Type', 'inventory-retrieval')
      ->fieldCondition('field_vault_ref', 'target_id', $vault->identifier(), '=');
      $results = $query->execute();

      // We can not use fieldCondition here.
      if (!empty($results[$Job->entityType()])) {
        $Jobs = entity_load($Job->entityType(), array_keys($results[$Job->entityType()]));
        foreach ($Jobs as $_Job) {
          /** @var \Drupal\aws_glacier\Entity\Job\Job $_Job */
          $_Job->_setWrapper();
          if (isset($_Job->wrapper->field_archive_ref) && !($_Job->wrapper->field_archive_ref->value() instanceof Archive)) {
            $cache[$vault->VaultName] = $_Job;
          }
        }
      }
    }
    return isset($cache[$vault->VaultName]) ? $cache[$vault->VaultName] : $Job;
  }

  /**
   * Loads the Job for an archive retrieval.
   *
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $vault
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   *
   * @return \Drupal\aws_glacier\Entity\Job\Job
   */
  static public function loadForArchive(Vault $vault, Archive $archive) {
    /** @var \Drupal\aws_glacier\Entity\Job\Job $job */
    $job = new static();

    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', $job->entityType())
      ->propertyCondition('Type', 'archive-retrieval')
      ->fieldCondition('field_vault_ref', 'target_id', $vault->identifier(), '=')
      ->fieldCondition('field_archive_ref', 'target_id', $archive->identifier(), '=');
    $results = $query->execute();
    if (!empty($results)) {
      return entity_load_single($job->entityType(), reset($results[$job->entityType()])->id);
    }
    return $job;
  }

  /**
   * The status code can be InProgress, Succeeded, or Failed, and indicates the status of the job.
   *
   * @return null|string
   */
  function getStatusCode() {
    $this->_setWrapper();
    $field = 'field_statuscode';
    if (isset($this->wrapper->{$field}) && ($value = $this->wrapper->{$field}->value())) {
      return t($value);
    }
    return NULL;
  }

  /**
   * The UTC time that the archive retrieval request completed. While the job is in progress, the value will be null.
   * @return null|string
   */
  function getCompletionDate() {
    $this->_setWrapper();
    $field = 'field_completiondate';
    if (isset($this->wrapper->{$field}) && ($value = $this->wrapper->{$field}->value())) {
      return $value;
    }
    return NULL;
  }
}

