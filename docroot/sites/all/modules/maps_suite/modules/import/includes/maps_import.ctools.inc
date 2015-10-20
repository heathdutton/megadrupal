<?php

/**
 * @file
 * Provide integration with CTools.
 */

use Drupal\maps_import\Plugins\CTools\ExportUI\ProfileExportable;
use Drupal\maps_import\Cache\Object\Converter as CacheConverter;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;

/**
 * CTools exportable callback; create an exportable profile.
 *
 * @return ProfileExportable
 *   The exportable profile.
 *
 * @see ctools_export_crud_new()
 */
function maps_import_profile_export_create($set_defaults = TRUE) {
  $profile = new ProfileExportable();
  $schema = ctools_export_get_schema('maps_import_profile');

  if ($set_defaults) {
    // Set some defaults so this data always exists.
    // We don't set the export_type property here, as this object is not saved
    // yet. We do give it NULL so we don't generate notices trying to read it.
    $profile->export_type = NULL;
    $profile->{$schema['export']['export type string']} = t('Local');
  }

  return $profile;
}

/**
 * CTools exportable callback; load an exportable profile.
 *
 * @return ProfileExportable
 *   The exportable profile.
 *
 * @see ctools_export_crud_load()
 */
function maps_import_profile_export_load($name) {
  if ($profile = CacheProfile::getInstance()->loadSingle($name, 'name')) {
    return new ProfileExportable($profile);
  }

  return NULL;
}

/**
 * CTools exportable callback; load multiple exportable objects.
 *
 * @see ctools_export_crud_load_multiple()
 */
function maps_import_profile_export_load_multiple(array $names) {
  $objects = array();

  foreach (CacheProfile::getInstance()->load($names, 'name') as $profile) {
    $objects[$profile->getName()] = new ProfileExportable($profile);
  }

  return $objects;
}

/**
 * CTools exportable callback; load all exportable objects of a given type.
 *
 * @see ctools_export_crud_load_all()
 */
function maps_import_profile_export_load_all($reset = FALSE) {
  $objects = array();

  foreach (CacheProfile::getInstance()->loadAll() as $profile) {
    $objects[$profile->getName()] = new ProfileExportable($profile);
  }

  return $objects;
}

/**
 * CTools exportable callback; save an exportable objects in the database.
 *
 * @see ctools_export_crud_save()
 */
function maps_import_profile_export_save(ProfileExportable $profile) {
  // Now remove the old profile.
  if (!empty($profile->old_profile)) {
    $profile->old_profile->delete(array('converter delete' => FALSE));
  }

  // Save the profile first.
  $record = $profile->toArray();

  if (isset($record['pid'])) {
    unset($record['pid']);
  }

  drupal_write_record('maps_import_profile', $record);
  $profile->setPid($record['pid']);

  // Save profile advanced settings.
  foreach (array('languages', 'statuses', 'media_types') as $key) {
    if (!empty($profile->{$key})) {
      variable_set('maps_import:' . $key . ':' . $profile->getPid(), $profile->{$key});
    }
  }

  // Save the converters.
  foreach ($profile->getConverters() as $name => $converter) {
    if (!$converter->ensureParent()) {
      $profile->removeConverter($name);
      watchdog('Maps Suite', 'Profile import: Retrieving parent converter failed for the converter "@name". Child converter has been ignored.', array('@name' => $name));
      continue;
    }

    $record = $converter->toArray();
    drupal_write_record('maps_import_converter', $record);
    $converter->setCid($record['cid']);

    // Save the filter conditions.
    $conditions = $converter->getFilterConditions();

    while ($conditions) {
      $condition = array_shift($conditions);
      $condition['cid'] = $converter->getCid();

      if (isset($condition['id'])) {
        unset($condition['id']);
      }

      drupal_write_record('maps_import_converter_conditions', $condition);

      if (!empty($condition['children'])) {
        foreach ($condition['children'] as $child_condition) {
          $child_condition['parent_id'] = $condition['id'];
          $conditions[] = $child_condition;
        }
      }
    }

    // Save the mapping items.
    foreach ($converter->getMappingItems() as $item) {
      if (isset($item['id'])) {
        unset($item['id']);
      }

      $item['cid'] = $converter->getCid();
      drupal_write_record('maps_import_mapping_item', $item);
    }
  }

  // Remove the old converters.
  if (!empty($profile->old_profile)) {
    $converter_options = isset($profile->old_converters) ? $profile->old_converters : array();

    foreach (CacheConverter::getInstance()->load(array($profile->old_profile->getPid()), 'pid') as $cid => $converter) {
      $options = !empty($converter_options[$cid]) ? $converter_options[$cid] : array();
      $options['check_converter'] = FALSE;
      $options += array(
        'mode' => 'unlink',
      );

      if (!empty($options['override']) && $options['mode'] === 'reassign') {
        $new_converter = $profile->getConverter($options['override']);

        if ($new_converter && $new_cid = $new_converter->getCid()) {
          $options['new_cid'] = $new_cid;
        }
        else {
          $options['mode'] = 'unlink';
        }
      }

      $converter->delete($options);
    }
  }

  CacheProfile::getInstance()->clearBinCache();
  CacheConverter::getInstance()->clearBinCache();
  return SAVED_NEW;
}

/**
 * CTools exportable callback; delete an exportable objects from the database.
 *
 * @see ctools_export_crud_delete()
 *
 * @todo add some advanced options (see Profile::delete()).
 */
function maps_import_profile_export_delete(ProfileExportable $object) {
  $object->getProfile()->delete();
}

/**
 * CTools exportable callback.
 * 
 * Get the exported code of a single exportable object.
 *
 * @see ctools_export_crud_export()
 * @see ctools_export_object()
 */
function maps_import_profile_export(ProfileExportable $object, $indent = '') {
  return $indent . '$profile = ' . $object->export($indent) . ";\n";
}

/**
 * CTools exportable callback. 
 * 
 * Get the exported code of a single exportable object.
 *
 * @see ctools_export_crud_export()
 */
function maps_import_profile_export_set_status(ProfileExportable $object, $status) {
  $object->setEnabled($status);
  $record = $object->toArray();
  drupal_write_record('maps_import_profile', $record, array('pid'));
}
