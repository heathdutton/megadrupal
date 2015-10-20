<?php

/**
 * @file
 * Interface for MaPS Import Converter
 */

namespace Drupal\maps_import\Converter;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Filter\FilterInterface;

/**
 * The Converter interface.
 */
interface ConverterInterface {

  /**
   * Defines the uid scope for a child converter.
   */
  const SCOPE_INHERIT = 0;

  /**
   * Defines the global scope for unique IDs.
   */
  const SCOPE_GLOBAL = 1;

  /**
   * Defines the profile scope for unique IDs.
   */
  const SCOPE_PROFILE = 2;

  /**
   * Class constructor.
   *
   * @param Profile $profile
   *   The MaPS Import Profile instance.
   */
  public function __construct(Profile $profile);

  /**
   * Get all mapping plugins.
   *
   * Each mapping plugin relates to a specific entity.
   */
  public function entityInfo();

  /**
   * Get the converter id.
   *
   * @return int
   *   The converter ID.
   */
  public function getCid();

  /**
   * Set the converter id.
   *
   * @param int $cid
   *   The converter ID.
   */
  public function setCid($cid);

  /**
   * Get the related profile ID.
   *
   * @return int
   *   The profile id.
   */
  public function getPid();

  /**
   * Get the converter profile.
   *
   * @return Drupal\maps_import\Profile\Profile
   *   The converter profile.
   */
  public function getProfile();

  /**
   * Set the converter profile.
   *
   * @param Profile $profile
   *    The converter profile.
   */
  public function setProfile(Profile $profile);

  /**
   * Get the converter title.
   *
   * @return string
   *   The converter title.
   */
  public function getTitle();

  /**
   * Set the converter title.
   *
   * @param string $title
   *    The converter title.
   */
  public function setTitle($title);

  /**
   * Get the converter name.
   *
   * @return string
   *   The converter name.
   */
  public function getName();

  /**
   * Set the converter name.
   *
   * @param string $name
   *    The new name.
   */
  public function setName($name);

  /**
   * Get the converter description.
   *
   * @return string
   *   The converter description.
   */
  public function getDescription();

  /**
   * Set the converter description.
   *
   * @param string $description
   *    The converter description.
   */
  public function setDescription($description);

  /**
   * Get the converter uid.
   *
   * @return string
   *   The converter uid.
   */
  public function getUid();

  /**
   * Set the converter uid.
   *
   * @param string $uid
   *    The converter uid.
   */
  public function setUid($uid);

  /**
   * Get the converter uid scope.
   *
   * @return int
   *   The converter uid scope.
   */
  public function getUidScope();

  /**
   * Set the converter uid scope.
   *
   * @param int $uid_scope
   *    The converter uid scope.
   */
  public function setUidScope($uid_scope);

  /**
   * Get the converter entity type.
   *
   * @return string
   *   The converter entity type.
   */
  public function getEntityType();

  /**
   * Set the converter entity type.
   *
   * @param string $entity_type.
   *    The converter entity type.
   */
  public function setEntityType($entity_type);

  /**
   * Get the converter bundle.
   *
   * @return string
   *   The converter bundle.
   */
  public function getBundle();

  /**
   * Set the converter bundle.
   *
   * @param string $bundle
   *    The converter bundle.
   */
  public function setBundle($bundle);

  /**
   * Get the converter weight.
   *
   * @return int
   *   The converter weight.
   */
  public function getWeight();

  /**
   * Set the converter weight.
   *
   * @param int $weight
   *    The converter weight.
   */
  public function setWeight($weight);

  /**
   * Get the converter options.
   *
   * @return array
   *   The converter options.
   */
  public function getOptions();

  /**
   * Set the converter options.
   *
   * @param array $options
   *    The converter options.
   */
  public function setOptions($options);

  /**
   * Get a specific condition rom its ID.
   *
   * @param int $condition_id
   *   The MaPS Import Filter Condition ID.
   *
   * @return Condition
   */
  public function getCondition($condition_id);

  /**
   * Return the related mapping object.
   *
   * @return MappingInterface
   */
  public function getMapping();

  /**
   * Return the converter type.
   *
   * @return string
   */
  public function getType();

  /**
   * Get the related Filter instance.
   *
   * @return  FilterInterface
   */
  public function getFilter();

  /**
   * Check if the mapping is allowed.
   *
   * @return boolean
   */
  public function isMappingAllowed();

  /**
   * Return the parent ID.
   *
   * @return int
   */
  public function getParentId();

  /**
   * Set the parent converter cid.
   *
   * @param $parent
   *   The parent converter.
   */
  public function setParentId($parentId);

  /**
   * Delete the converter from the database.
   *
   * @param array $options
   *   An associative array that contains:
   *   - mode: (optional) The deletion mode for the entities created by the
   *     converter. It can accept 3 modes:
   *     - unlink: (default) The entities are kept in Drupal but no more associated
   *       with MaPS Suite.
   *     - delete: The entities are totally removed from the system.
   *     - reassign: The entities are reassigned to another converter. Using this,
   *       you should take care of the converter children so they are reassigned too,
   *       otherwise their related entities will be unlinked.
   *   - new_cid: The converter ID that should be used to update the {maps_import_entities}
   *     records when the 'reassign' mode is selected. If this mode is choosen but the
   *     'new_cid' value not provided (or does not match any existing converter), the
   *     mode falls back to 'unlink'.
   *   - excepton: If TRUE, an exception is thrown if the options are not valid (missing
   *     'new_cid' when reassigning the converted entities).
   *   - check_converter: (optional) If TRUE, the converter existence is tested when
   *     the choosen mode is 'reassign'.
   *   - children: An associative array that contains details about converter children
   *     and the way to process them:
   *     - mode: (optional) The global mode for all children. This value can be
   *       overriden in each child declaration.
   *     - items: An array containing information about each specific children:
   *       - name: (required) The converter name. If there is no child converter matching
   *         this value, the rule is ignored.
   *       - mode: (optional) The mode for this chid converter (see above).
   */
  public function delete();

  /**
   * Whether the converter has additional options.
   *
   * @return bool
   */
  public function hasAdditionalOptions();

}
