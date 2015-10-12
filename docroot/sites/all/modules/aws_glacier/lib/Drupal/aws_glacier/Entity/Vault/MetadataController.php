<?php
/**
 * @file
 * Defines \Drupal\aws_glacier\Entity\Vault\MetadataController
 */
namespace Drupal\aws_glacier\Entity\Vault;

/**
 * Class GlacierVaultMetadataController
 */
class MetadataController extends \EntityDefaultMetadataController {
  /**
   * {@inheritDoc}
   */
  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];
    $properties['VaultName'] = array(
      'label' => t('Vault name'),
      'description' => t('The name of the vault.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => Vault::$permission,
      'schema field' => 'VaultName',
    );
    $properties['VaultARN'] = array(
      'label' => t('Vault ARN'),
      'description' => t('The Amazon Resource Name (ARN) of the vault.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => Vault::$permission,
      'schema field' => 'VaultARN',
    );
    $properties['CreationDate'] = array(
      'label' => t('Creation date'),
      'description' => t('The UTC date when the vault was created. A string representation of ISO 8601 date format, for example, "2012-03-20T17:03:43.221Z".'),
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'aws_glacier_date_getter',
      'setter permission' => Vault::$permission,
      'schema field' => 'CreationDate',
      'type' => 'date',
    );
    $properties['LastInventoryDate'] = array(
      'label' => t('Last inventory date'),
      'description' => t('The UTC date when Amazon Glacier completed the last vault inventory. A string representation of ISO 8601 date format, for example, "2012-03-20T17:03:43.221Z".'),
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'aws_glacier_date_getter',
      'setter permission' => Vault::$permission,
      'schema field' => 'LastInventoryDate',
      'type' => 'date',
    );
    $properties['NumberOfArchives'] = array(
      'label' => t('Number of archives'),
      'description' => t('The number of archives in the vault as of the last inventory date. This field will return null if an inventory has not yet run on the vault, for example, if you just created the vault.'),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => Vault::$permission,
      'schema field' => 'NumberOfArchives',
    );
    $properties['SizeInBytes'] = array(
      'label' => t('Size in bytes'),
      'description' => t('Total size, in bytes, of the archives in the vault as of the last inventory date. This field will return null if an inventory has not yet run on the vault, for example, if you just created the vault.'),
      'setter callback' => 'entity_property_verbatim_set',
      'getter callback' => 'aws_glacier_sizes_getter',
      'setter permission' => Vault::$permission,
      'schema field' => 'SizeInBytes',
    );
    return $info;
  }
}
