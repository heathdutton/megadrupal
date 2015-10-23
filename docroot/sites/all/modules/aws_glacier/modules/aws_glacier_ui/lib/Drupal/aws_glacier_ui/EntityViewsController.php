<?php

namespace Drupal\aws_glacier_ui;

/**
 * Class EntityViewsController
 * @package Drupal\aws_glacier_ui
 */
class EntityViewsController extends \EntityDefaultViewsController {

  /**
   * {@inheritDoc}
   */
  public function views_data() {
    $data = parent::views_data();
    if (isset($data['glacier_vault'])) {
      $data['glacier_vault']['inventory'] = array(
        'title' => t('Initiate inventory'),
        'help' => t('Allows to initiates a inventory.'),
        'field' => array(
          'handler' => '\Drupal\aws_glacier_ui\Views\Inventory',
        ),
      );
      $data['glacier_vault']['bulkdelete'] = array(
        'title' => t('Bulk delete'),
        'help' => t('Allows to delete all archives of this vault.'),
        'field' => array(
          'handler' => '\Drupal\aws_glacier_ui\Views\BulkDelete',
        ),
      );
      $data['glacier_vault']['CreationDate']['field'] = array(
        'handler' => '\Drupal\aws_glacier_ui\Views\VaultDate',
      );
      $data['glacier_vault']['LastInventoryDate']['field'] = array(
        'handler' => '\Drupal\aws_glacier_ui\Views\VaultDate',
      );
      $data['glacier_vault']['SizeInBytes']['field'] = array(
        'handler' => '\Drupal\aws_glacier_ui\Views\Size',
      );
    }
    if (isset($data['glacier_archive'])) {
      $data['glacier_archive']['restore'] = array(
        'title' => t('Restore'),
        'help' => t('Allows to download the file again.'),
        'field' => array(
          'handler' => '\Drupal\aws_glacier_ui\Views\RestoreFile',
        ),
      );
    }
    return $data;
  }
}
