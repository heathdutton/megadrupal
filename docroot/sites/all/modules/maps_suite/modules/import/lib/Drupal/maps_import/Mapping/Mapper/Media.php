<?php

/**
 * @file
 * Handle objects mapping operation.
 */

namespace Drupal\maps_import\Mapping\Mapper;

use Drupal\maps_import\Fetcher\Fetcher;
use Drupal\maps_import\Mapping\Source\MapsSystem\Media as MapsSystemMedia;

/**
 * Base class for all mapping operations.
 */
class Media extends Mapper {

  /**
   * @inheritdoc
   */
  public function getTitle() {
    return t('Media mapping');
  }

  /**
   * @inheritdoc
   */
  public function getDescription() {
    return t('The medias mapping operation is related to the profile %profile.', array('%profile' => $this->profile->getTitle()));
  }

  /**
   * @inheritdoc
   */
  public function getType() {
    return 'media_mapping';
  }

  /**
   * @inheritdoc
   */
  protected function getConverterClass() {
    return "Drupal\\maps_import\\Converter\\Media";
  }

  /**
   * @inheritdoc
   */
  protected function getEntityTable() {
    return Fetcher::DB_MEDIA_TABLE;
  }

  /**
   * @inheritdoc
   */
  protected function createEntity(array $entity) {
    return new MapsSystemMedia($entity);
  }

  /**
   * @inheritdoc
   */
  protected function processRun($index = 1, $range = 0) {
    parent::processRun($index, $range);

    // Message displayed under the progressbar.
    $this->context['message'] = t('Mapping media for profile "@title" (@current/@total)', array(
      '@title' => $this->getProfile()->getTitle(),
      '@current' => $this->context['results'][$this->getType()]['processed_op'],
      '@total' => $this->context['results'][$this->getType()]['total_op'],
    ));
  }

}
