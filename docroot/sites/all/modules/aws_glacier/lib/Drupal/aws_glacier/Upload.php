<?php

namespace Drupal\aws_glacier;

use Drupal\aws_glacier\Entity\Archive\Archive;
use Drupal\aws_glacier\Entity\Vault\Vault;

/**
 * Class Upload
 * @package Drupal\aws_glacier
 */
class Upload extends Command {

  /**
   * @var int
   */
  protected $maxUploadSize;
  /**
   * @var string
   * The name of the vault.
   */
  protected $vaultName;
  /**
   * @var string
   * The optional description of the archive you are uploading.
   */
  protected $archiveDescription;
  /**
   * @var (string|resource|\Guzzle\Http\EntityBodyInterface)
   * The data to upload.
   */
  protected $body;

  /**
   * {@inheritDoc}
   *
   * @return $this
   */
  function __construct() {
    $this->setMaxUploadSize();
    parent::__construct('UploadArchive');
    return $this;
  }

  /**
   * Set maximum upload size.
   * @param int $maxUploadSize (default: 5)
   * In MB.
   *
   * @return $this
   */
  public function setMaxUploadSize($maxUploadSize = 5) {
    $this->maxUploadSize = DRUPAL_KILOBYTE * DRUPAL_KILOBYTE * $maxUploadSize;
    return $this;
  }

  /**
   * @param string $vaultName
   * @return $this
   */
  public function setVaultName($vaultName) {
    $this->vaultName = $vaultName;
    $this->setArgs(array('vaultName' => $vaultName));
    return $this;
  }

  /**
   * @param string $archiveDescription
   * @return $this
   */
  public function setArchiveDescription($archiveDescription) {
    $this->archiveDescription = $archiveDescription;
    $this->setArgs(array('archiveDescription' => $archiveDescription));
    return $this;
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Archive\Archive $archive
   * @return string
   */
  public function getGeneratedArchiveDescription(Archive $archive) {
    $tmp = array();
    $tmp[] = $archive->vaultName;
    $tmp[] = $archive->entity;
    $tmp[] = $archive->entity_id;
    $tmp[] = $archive->field;
    $tmp[] = $archive->file_uri;
    return implode(':', $tmp);
  }

  /**
   * @param mixed $body
   * @return $this
   */
  public function setBody($body) {
    $this->body = $body;
    $this->setArgs(array('body' => $body));
    return $this;
  }

  /**
   * @param $item
   * @param \DrupalQueueInterface $queue
   * @return \Drupal\aws_glacier\Upload|null
   */
  static public function processQueueItem($item, \DrupalQueueInterface $queue) {
    /** @var Archive $archive */
    $archive = $item->data;
    $file = file_load((int) $archive->file_id);
    if (!$file) {
      $queue->deleteItem($item);
      return NULL;
    }
    if (!file_exists($file->uri)) {
      $queue->deleteItem($item);
      return NULL;
    }
    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', $archive->entityType());
    $query->propertyCondition('file_uri', $file->uri);
    $query->count();
    $count = $query->execute();
    // TODO add property to file entity for this
    if ($count) {
      $queue->deleteItem($item);
      return NULL;
    }

    /** @var Vault $vault */
    $vault = entity_create('glacier_vault', array('VaultName' => $archive->vaultName));
    if ($vault->loadByUniqueProperty(TRUE) < 1) {
      try {
        $vault->save();
      }
      catch (\Exception $e) {
        watchdog_exception('aws_glacier', $e);
        $queue->releaseItem($item);
        return NULL;
      }
    }
    /** @var \Drupal\aws_glacier\Upload $Upload */

    $Upload = new static();
    $archive->file_uri = $file->uri;
    if ($file->filesize > $Upload->maxUploadSize) {
      MultipartUpload::generateParts($archive, $item, $queue);
    }
    else {
      $Upload
        ->setArchiveDescription($Upload->getGeneratedArchiveDescription($archive))
        ->setVaultName($archive->vaultName);
      $Upload->setBody(file_get_contents($file->uri))
        ->run();
      $data = $Upload->getData();

      if (!empty($data['archiveId'])) {
        $archive->archiveId = $data['archiveId'];
        try {
          $archive->save();
        }
        catch (\Exception $e) {
          watchdog_exception('aws_glacier', $e);
          $queue->releaseItem($item);
        }
        $queue->deleteItem($item);
      }
      else {
        $queue->releaseItem($item);
      }
    }
    return $Upload;
  }
}
