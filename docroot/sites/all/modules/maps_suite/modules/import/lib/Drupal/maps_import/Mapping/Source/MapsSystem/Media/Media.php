<?php

/**
 * @file
 * Class that defines operation on MaPS Object's object-media relation.
 */

namespace Drupal\maps_import\Mapping\Source\MapsSystem\Media;

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface;
use Drupal\maps_import\Mapping\Source\MapsSystem\PropertyWrapper;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface as DrupalEntityInterface;
use Drupal\maps_import\Exception\MappingException;
use Drupal\maps_import\Profile\Profile;

abstract class Media extends PropertyWrapper {

  /**
   * Return the media key.
   *
   * @return string
   */
  public function getKey() {
    return 'media:' . $this->media_type;
  }

  /**
   * Whether the property is translatable.
   *
   * @var boolean
   */
  protected $translatable = FALSE;

  /**
   * Whether the property may have multiple values.
   *
   * @var boolean
   */
  protected $multiple = TRUE;

  /**
   * @inheritdoc
   */
  public function extractValues(EntityInterface $entity, $options = array(), ConverterInterface $currentConverter) {
    $medias = $entity->getMedias($this->media_type);

    if (empty($medias)) {
      return array();
    }

    if (!isset($options['media_start_range']) || !isset($options['media_limit_range'])) {
      throw new MappingException('Media ranges are not defined.', 0, array());
    }

    $values = array();
    $values[0] = array();

    for ($i = ((int) $options['media_start_range'] - 1); $i < (int) $options['media_limit_range']; $i++) {
      if (isset($medias[$i])) {
        $values[DrupalEntityInterface::LANGUAGE_NONE][] = $medias[$i]['entity_id'];
      }
    }

    return $values;
  }

  /**
   * @inheritdoc
   */
  public function getGroupLabel() {
    return t('Object media relation');
  }

  /**
   * @inheritdoc
   */
  public function getTranslatedTitle() {
    return 'Media';
  }

  /**
   * Return the available Media classes.
   */
  static public function getMediaClassFromMediaType($mediaType) {
  	$mediaTypes = array(
  	  1 => 'Image',
  	  2 => 'Document',
  	  3 => 'Video',
  	  4 => 'Sound',
  	);

  	return 'Drupal\\maps_import\\Mapping\\Source\\MapsSystem\\Media\\' . $mediaTypes[$mediaType];
  }

  /**
   * Load a MaPS System® media entity from the MaPS media type id.
   *
   * @param $mediaType
   *   The MaPS System® media type id.
   * @param $definition
   *   The MaPS System® property definition.
   */
  static public function createMediaFromMediaType($mediaType, array $definition = array()) {
    $className = self::getMediaClassFromMediaType($mediaType);
    return new $className($definition);
  }

  /**
   * @inheritdoc
   */
  public function exists(Profile $profile) {
    $results = $profile->getConfiguration(array(
      'id' => $this->media_type,
      'type' => 'media_type',
    ));

    return count($results) > 0;
  }

}
