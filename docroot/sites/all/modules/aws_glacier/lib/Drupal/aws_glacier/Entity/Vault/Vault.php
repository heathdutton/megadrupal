<?php
/**
 * Defines \Drupal\aws_glacier\Entity\Vault\Vault.
 */

namespace Drupal\aws_glacier\Entity\Vault;

use Drupal\aws_glacier\Entity\GlacierEntity;
use Drupal\aws_glacier\Exception\ExistsException;
use Drupal\aws_glacier\Exception\VaultException;

/**
 * Class GlacierVault
 */
class Vault extends GlacierEntity {

  /**
   * @var string $VaultName
   * The name of the vault.
   */
  public $VaultName;

  /**
   * @var string $VaultARN
   * The Amazon Resource Name (ARN) of the vault.
   */
  public $VaultARN;

  /**
   * @var string $CreationDate
   * The UTC date when the vault was created. A string representation of ISO 8601 date format,
   * for example, "2012-03-20T17:03:43.221Z".
   */
  public $CreationDate;

  /**
   * @var string $LastInventoryDate
   * The UTC date when Amazon Glacier completed the last vault inventory.
   * A string representation of ISO 8601 date format, for example, "2012-03-20T17:03:43.221Z".
   */
  public $LastInventoryDate;

  /**
   * @var int $NumberOfArchives
   * The number of archives in the vault as of the last inventory date.
   * This field will return null if an inventory has not yet run on the vault,
   * for example, if you just created the vault.
   */
  public $NumberOfArchives;

  /**
   * @var int $SizeInBytes
   * Total size, in bytes, of the archives in the vault as of the last inventory date.
   * This field will return null if an inventory has not yet run on the vault,
   * for example, if you just created the vault.
   */
  public $SizeInBytes;

  /**
   * @var string $permission
   * Permission name for user_access() etc.
   */
  static public $permission = 'adminster_glacier_vault';

  /**
   * Constructer.
   *
   * @param array $values
   *   Property values.
   *
   * @return $this
   */
  public function __construct($values = array()) {
    $values['uniqueProperty'] = 'VaultName';
    parent::__construct($values, 'glacier_vault');
    return $this;
  }

  /**
   * {@inheritDoc}
   */
  public function defaultUri() {
    return array(
      'path' => AWS_GLACIER_ADMIN_PATH . '/vaults/' . $this->id,
    );
  }

  /**
   * @link http://docs.aws.amazon.com/amazonglacier/latest/dev/creating-vaults.html @endlink
   *
   * @see GlacierEntity::validateUniqueProperty()
   */
  public function validateUniqueProperty() {
    if (strlen($this->VaultName) > 255) {
      throw new VaultException(t('The vault name is too long. Names can be between 1 and 255 characters long.'));
    }
    if (!empty($this->VaultName) && !preg_match('/[A-Za-z0-9_\-\.]/', $this->VaultName, $matches)) {
      throw new VaultException(t('The vault name contains not allowed characters. Allowed characters are a–z, A–Z, 0–9, _ (underscore), - (hyphen), and . (period).'));
    }
    try{
      parent::validateUniqueProperty();
    }
    catch (ExistsException $e) {
      throw new VaultException($e->getMessage());
    }
    catch (\Exception $e) {
      throw new $e;
    }
  }

  /**
   * {@inheritDoc}
   * @throws \Exception
   */
  public function save() {
    if (isset($this->is_new) && empty($this->CreationDate)) {
      $command = new Command('CreateVault');
      $command->setVault($this);
      try{
        $command->run();
      }
      catch(\Exception $e) {
        throw $e;
      }
      parent::save();
      self::refresh($this->VaultName);
      return $this;
    }
    return parent::save();
  }

  /**
   * @param string $vaultName
   *
   * @return $this
   *
   * @throws \Exception
   */
  static public function refresh($vaultName) {
    /** @var Vault $vault */
    $vault = new static(array('VaultName' => $vaultName));
    $vault = $vault->loadByUniqueProperty();
    try{
      $command = new Command('DescribeVault');
      $command->setCommand('DescribeVault');
      $command->setVault($vault);
      $command->run();
    }
    catch(\Exception $e) {
      throw $e;
    }
    $data = $command->getData();
    if (is_array($data)) {
      foreach ($data as $property => $value) {
        $vault->{$property} = $value;
      }
      $vault->save();
    }
    return $vault;
  }


  /**
   * @return $this
   */
  public function deleteCommand() {
    try{
      $command = new Command('DeleteVault');
      $command->setVault($this);
      $command->run();
    }
    catch (\Exception $e) {
    }
    return $this;
  }

  /**
   * @return array
   */
  static public function getOptionsList() {
    $query = new \EntityFieldQuery();
    $query->entityCondition('entity_type', 'glacier_vault');
    $results = $query->execute();
    $options = array();
    if (!empty($results['glacier_vault'])) {
      $entities = entity_load('glacier_vault', array_keys($results['glacier_vault']));
      foreach ($entities as $entity) {
        $options[$entity->VaultName] = $entity->VaultName;
      }
    }
    return $options;
  }
}
