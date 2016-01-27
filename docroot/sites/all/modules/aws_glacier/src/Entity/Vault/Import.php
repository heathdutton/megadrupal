<?php

namespace Drupal\aws_glacier\Entity\Vault;

use Drupal\aws_glacier\Command;


/**
 * Class Import
 * @package Drupal\aws_glacier\Entity\Vault
 */
class Import extends Command {

  /**
   * @var string
   * The maximum number of items returned in the response. If you don't specify a value, the List Vaults operation returns up to 1,000 items.
   */
  protected $limit;

  /**
   * Sets the maximum number of items returned in the response.
   *
   * @param string $limit
   *
   * @return $this
   */
  public function setLimit($limit) {
    if ((int) $limit > 0) {
      $this->limit = $limit;
    }
    $this->setArgs(array('limit' => $limit));
    return $this;
  }

  /**
   * @return string
   */
  public function getLimit() {
    return $this->limit;
  }

  /**
   * @return bool
   */
  public function hasLimit() {
    return !empty($this->limit) ? TRUE : FALSE;
  }

  /**
   * @var string
   * A string used for pagination. The marker specifies the vault ARN after which the listing of vaults should begin.
   */
  private $marker;

  /**
   * @param string $marker
   * @return $this
   */
  private function setMarker($marker) {
    $this->marker = $marker;
    $this->setArgs(array('marker' => $marker));
    return $this;
  }

  /**
   * Has a marker. Means the Vault list returns more than 1000 vaults.
   * @return bool
   */
  private function hasMarker() {
    return !empty($this->marker) ? TRUE : FALSE;
  }

  /**
   * @var array
   * List of vaults.
   *
   * @link http://docs.aws.amazon.com/aws-sdk-php/latest/class-Aws.Glacier.GlacierClient.html#_listVaults @endlink
   */
  private $VaultList = array();

  /**
   * {@inheritDoc}
   * @return $this
   */
  function __construct() {
    parent::__construct('ListVaults');
    return $this;
  }

  /**
   * {@inheritDoc}
   * @return $this
   */
  public function run() {
    parent::run();
    $this->batchImport();
    return $this;
  }

  /**
   * Imports the retrieved vaults via batch process.
   *
   * @return $this
   */
  private function batchImport() {
    $data = $this->getData();
    $this->setMarker($data['Marker']);
    if (!empty($data['VaultList'])) {
      $this->VaultList = $data['VaultList'];
    }
    $operations = $this->getOperations();
    if (!empty($operations)) {
      $batch = array(
        'title' => t('Importing'),
        'operations' => $operations,
        'finished' => 'aws_glacier_ui_vault_import_finished',
      );
      batch_set($batch);
    }
    return $this;
  }

  /**
   * Builds a list of operation for using in a batch process.
   * @return array
   */
  private function getOperations() {
    $operations = array();
    foreach ($this->VaultList as $data) {
      /** @var Vault $vault */
      $vault = entity_create('glacier_vault', $data);
      $vault = $vault->loadByUniqueProperty();
      $operations[] = array('\Drupal\aws_glacier\Entity\Vault\Import::vaultImport', array($vault));
    }
    if ($this->hasMarker() && !$this->hasLimit()) {
      $this->run();
    }
    return $operations;
  }

  /**
   * @param \Drupal\aws_glacier\Entity\Vault\Vault $vault
   * @param $context
   */
  public static function vaultImport(Vault $vault, &$context) {
    $vault->save();
    $context['results'][] = $vault;
    $context['message'] = check_plain($vault->VaultName);
  }
}
