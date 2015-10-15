<?php

namespace Drupal\aws_glacier\Entity\Archive;

use Drupal\aws_glacier\Entity\GlacierEntity;

/**
 * Class Archive
 * @package Drupal\aws_glacier\Entity\Archive
 */
class Archive extends GlacierEntity {

  /**
   * @var string
   * The ID of the archive. (External)
   */
  public $archiveId = 'dummy';

  /**
   * @var string
   * The name of the vault.
   */
  public $vaultName;

  /**
   * @var string
   * The entity where the file was referenced. E.g. node.
   */
  public $entity;

  /**
   * @var string
   * The fieldname where the file was referenced. E.g. node.
   */
  public $field;

  /**
   * @var string
   * The Id of entity where the file was referenced. E.g. node.
   */
  public $entity_id;

  /**
   * @var string
   * The uri of the file.
   */
  public $file_uri;

  /**
   * @var int
   * The Id of the file.
   */
  public $file_id;

  /**
   * @var string
   *
   * Permission for Drupal.
   */
  static public $permission = 'administer_glacier_archive';

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function __construct($values = array()) {
    $values['uniqueProperty'] = 'archiveId';
    parent::__construct($values, 'glacier_archive');
    return $this;
  }

  /**
   * {@inheritDoc}
   */
  public function defaultUri() {
    return array(
      'path' => AWS_GLACIER_ADMIN_PATH . '/archives/' . $this->id,
    );
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function delete() {
    $vault = entity_create('glacier_vault', array('VaultName' => $this->vaultName));
    $command = new Command('DeleteArchive');
    $command->setArchive($this);
    $command->setVault($vault);
    $command->run();
    parent::delete();
    return $this;
  }
}
