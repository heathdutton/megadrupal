<?php

/**
 * @file
 * Hooks provided by the MaPS Import module.
 */

use Drupal\maps_import\Converter\ConverterInterface;
use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface as MapsEntityInterface;
use Drupal\maps_import\Mapping\Target\Drupal\EntityInterface as DrupalEntityInterface;

/**
 * Allow to act on a Drupal entity before saving it.
 *
 * @param \EntityDrupalWrapper $wrapper
 *   The entity metadata wrapper that contains the entity being saved.
 * @param string $entity_type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 */
function hook_maps_import_entity_presave(\EntityDrupalWrapper $wrapper, $entity_type, $bundle) {
  if ($entity_type === 'node' && $bundle === 'article') {
    db_insert('my_table')
      ->fields(array(
        'nid' => $wrapper->getIdentifier(),
        'updated' => REQUEST_TIME,
      ))
      ->execute();
  }
}

/**
 * Allow to act on a Drupal entity before saving it.
 *
 * This hook may be helpful to add extra data to the Drupal entities that were
 * converted, with a full access to the source data from MaPS System®.
 *
 * Notice: keep in mind that similar MaPS System® objects may be processed and
 * converted to some Drupal entities already processed before. So this hook may
 * be called several times with the same converted entities.
 *
 * @param Drupal\maps_import\Mapping\Target\Drupal\EntityInterface $drupalEntity
 *   The instance containing aff entity wrappers that were previously saved.
 * @param Drupal\maps_import\Mapping\Source\MapsSystem\EntityInterface $mapsEntity
 *   The instance containing all MaPS System® source objets.
 * @param string $entity_type
 *   The entity type.
 * @param string $bundle
 *   The entity bundle.
 * @param ConverterInterface $converter
 *   The related converter.
 */
function hook_maps_import_entity_mapping_finished(DrupalEntityInterface $drupalEntity, MapsEntityInterface $mapsEntity, $entity_type, $bundle, ConverterInterface $converter) {
  // Find something in the raw MaPS System® object attributes.
  $attributes = $mapsEntity->getAttributes();

  if (isset($attributes[$myId])) {
    // Get something...
  }

  // Now perform some changes in the converted Drupal entities...
  foreach ($drupalEntity->getEntities() as $entityWrapper) {
    // ...

    $entityWrapper->save();
  }
}

/**
 * List the available operations for a given profile.
 *
 * @param Profile $profile
 *   A MaPS Import profile object.
 *
 * @return array
 *   An associative array, indexed by the operation name, containing the
 *   following keys:
 *   - title: The operation title.
 *   - description: A short description of the operation.
 *   - class: The class name related to the operation. An operation handler
 *     should always implement the Drupal\maps_import\Operation\OperationInterface
 *     interface.
 *
 * @see hook_maps_import_operations_alter()
 */
function hook_maps_import_operations(Profile $profile) {
  return array(
    'my_operation' => array(
      'title' => t('My new operation.'),
      'description' => t('Description of my new operation.'),
      'class' => 'MyOperationClass',
    )
  );
}

/**
 * Alter the list of available operations for a given profile.
 *
 * @param array &$data
 *   The defined operations.
 * @param Profile $profile
 *   A MaPS Import profile object.
 *
 * @see hook_maps_import_operations()
 */
function hook_maps_import_operations_alter(&$data, Profile $profile) {
  if (isset($data['my_operation'])) {
    $data['my_operation']['title'] = t('My new title');
  }
}

/**
 * This hook is invoked when an operation is fully finished.
 *
 * The operation may have run using the Batch API or an other way,
 * like Drush.
 * It is mostly usefull to clear some cached data.
 *
 * @param $type
 *   The type name of the operation.
 */
function hook_maps_import_operation_finished($type) {
  if ($type === 'my_operation') {
    // Clear some cached data.
    CacheProfile::getInstance()->clearBinCache();

    watchdog('my_module', 'My custom message');
  }
}

/**
 * Get a list of available actions for a given profile.
 *
 * This hook gathers actions that are related to a given profile. It
 * is invoked by the CTools API to build a list of links and displayed
 * as a "drop-down like" element in the profiles overview..
 *
 * @param Profile $profile
 *   A MaPS Import profile object.
 *
 * @return array
 *   An associative array containing:
 *   - title: The action title.
 *   - href: The internal path for the given action.
 *   - weight (optional): A integer that defines the position of the
 *     link related to the others.
 *   - html: (optional) Whether or not 'title' is HTML. If set, the title
 *     will not be passed through check_plain().
 *   - attributes: (optional) Attributes for the anchor, or for the <span>
 *     tag used in its place if no 'href' is supplied. If element 'class' is
 *     included, it must be an array of one or more class names.
 *
 * @see theme_links()
 */
function hook_maps_import_profile_action_links(Profile $profile) {
  return array(
    'profile-edit' => array(
      'title' => t('Edit'),
      'href' => 'admin/my-module/edit-profile/' . $profile->getName(),
      'weight' => -10,
    ),
  );
}

/**
 * This hook is useful for doing specific actions after each batch step of the
 * mapping process.
 *
 * @param Profile $profile
 *   A MaPS Import profile object.
 * @param ConverterInterface[] $ids
 *   An array whose keys are processed entity IDs and values are either the
 *   converter object that processed the entity or NULL if none matched.
 */
function hook_maps_import_mapping_finished(Profile $profile, array $ids) {
  foreach ($ids as $entityId => $converter) {
    if (isset($converter) && $converter->getType() === 'my_entity_type') {
      // Perform some action...
      my_module_update_entity($entityId);
    }
  }
}

/**
 * This hook is invoked after a profile has been deleted.
 *
 * It allows, for example, to perform some cleanup in the tables provided by
 * contributed modules.
 *
 * @param Profile $profile
 *   The deleted profile instaance.
 * @param array $options
 *   An associative array that contains:
 *   - converter delete: Flag indicating if the converters that rely on this
 *     profile should be removed. Default to TRUE.
 *   - converter delete mode: (optional) Either 'unlink' or 'delete'. For more
 *     details on these values, see ConverterInterface::delete(). The 'reassign'
 *     mode is not available, so if the converted entities need to be reassigned,
 *     the converters have to be deleted one after another with the necessary
 *     options for reassignement before removing the profile. Default to 'unlink'.
 *   - library delete mode: A value indicating how the library related entities
 *     should be processed. Default to 'unlink'.
 *     - delete terms: All the taxonomy terms that have a correspondence with a
 *       library are deleted.
 *     - delete vocabularies: The all vocabularies that are mapped with a library
 *       are totally removed, including terms.
 *     - unlink: The taxonomy terms are kept, but no more synchronized with MaPS
 *       Suite.
 *
 * @see Profile::delete()
 */
function hook_maps_import_profile_delete(Profile $profile, array $options) {
  db_delete('my_table')
    ->condition('pid', $profile->getPid())
    ->execute();
}

/**
 * This hook is invoked after a converter has been deleted.
 *
 * It allows, for example, to perform some cleanup in the tables provided by
 * contributed modules.
 *
 * @param ConverterInterface $converter
 *   The deleted converter instaance.
 * @param array $options
 *   An associative array that contains:
 *   - mode: The deletion mode for the entities created by the converter. It
 *     can accept 3 modes:
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
 *
 * @see ConverterInterface::delete()
 */
function hook_maps_import_converter_delete(ConverterInterface $converter, array $options) {
  db_delete('my_table')
    ->condition('cid', $converter->getCid())
    ->execute();
}

/**
 * This hook allows to add new actions to the converters.
 *
 * @param \Drupal\maps_import\Converter\ConverterInterface $converter
 *   The related converter.
 *
 * @return array
 *   An array containing the new actions.
 */
function hook_maps_import_converter_actions(ConverterInterface $converter) {
  return array(
    'my-action' => array(
      'href' => 'path/to/my/action',
      'title' => t('my action'),
    ),
  );
}
