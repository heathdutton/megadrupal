<?php

/**
 * @file
 * Provides MaPS integration for MaPS Import module.
 */

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Cache\Object\Profile as CacheProfile;
use Drupal\maps_import\Fetcher\Configuration as ConfigurationFetcher;
use Drupal\maps_import\Fetcher\Objects as ObjectsFetcher;
use Drupal\maps_import\Mapping\Mapper\Object as ObjectMapper;
use Drupal\maps_import\Mapping\Mapper\Media as MediaMapper;
use Drupal\maps_import\Mapping\Library\Library as LibraryMapping;

/**
 * Implements hook_maps_import_entity_presave().
 */
function maps_import_maps_import_entity_presave(\EntityDrupalWrapper $wrapper, $entity_type, $bundle) {
  // Since we may hide some legacy fields if overriden by Title module,
  // the standard behavior of this module will use a non-set value of
  // the legacy field to replace the value of the replacement field.
  // So we need first to restore the legacy field value from the replacement
  // field to avoid such issue.
  if (module_exists('title')) {
    $info = entity_get_info($entity_type);
    $langcode = language_default('language');

    if (!empty($info['field replacement'])) {
      $entity = $wrapper->value();

      foreach ($info['field replacement'] as $legacy_field => $info) {
        if (title_field_replacement_enabled($entity_type, $bundle, $legacy_field)) {
          $entity->{$legacy_field} = NULL;
          title_field_sync_get($entity_type, $entity, $legacy_field, $info, $langcode);
        }
      }
    }
  }

  // The user entity needs some specific process to be saved correctly.
  if ($entity_type === 'user') {
    $account = $wrapper->value();
    if (!empty($account->uid) && !isset($account->original)) {
      $account->original = entity_load_unchanged('user', $account->uid);
    }

    // Set the password.
    require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
    if (!isset($account->pass) && isset($account->original->pass)) {
      $account->pass = $account->original->pass;
    }

    // Do not allow empty password.
    if (empty($account->pass)) {
      $account->pass = user_hash_password(user_password());
    }
    elseif (!isset($account->original->pass) || $account->pass != $account->original->pass) {
      // @todo Add some options in the converter to handle the possible cases.
      $account->pass = user_hash_password($account->pass);
    }

    // Enable the user if there is no value.
    // @todo add some options in the converter to handle that.
    if (!isset($account->status)) {
      $account->status = isset($account->original->status) ? $account->original->status : 1;
    }
  }
}

/**
 * Implements hook_maps_suite_admin_overview().
 */
function maps_import_maps_suite_admin_overview() {
  if ($profiles = CacheProfile::getInstance()->loadAll()) {
    $titles = array();
    foreach ($profiles as $profile) {
      $titles[] = $profile->getTitle();
    }

    $content = format_plural(count($profiles), '@count profile: %list.', '@count profiles: %list', array('%list' => implode(', ', $titles)));
  }
  else {
    $content = t('There is no existing profile yet. Add a !link now.', array('!link' => l(t('new profile'), 'admin/maps-suite/profiles/add')));
  }

  return array(t('Web Services profiles') => $content);
}

/**
 * Implements hook_maps_import_profile_actions().
 */
function maps_import_maps_import_profile_action_links(Profile $profile) {
  $links = array();

  if ($profile->getConfiguration()) {
    $links['profile-object-converters'] = array('title' => t('Object converters overview'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/object');
    $links['profile-object-converter-add'] = array('title' => t('Add object converter'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/object/add');
    $links['profile-media-converters'] = array('title' => t('Media converters overview'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/media');
    $links['profile-media-converter-add'] = array('title' => t('Add media converter'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/media/add');
    $links['profile-languages'] = array('title' => t('Languages mapping'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/languages');
    $links['profile-statuses'] = array('title' => t('Statuses mapping'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/statuses');
    $links['profile-media-types'] = array('title' => t('Media types mapping'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/media_types');
  }

  $links['profile-import'] = array('title' => t('manual import'), 'href' => 'admin/maps-suite/profiles/' . $profile->getName() . '/import');

  return $links;
}

/**
 * Implements hook_maps_import_operations().
 */
function maps_import_maps_import_operations(Profile $profile) {
  $operations = array();

  if (!empty($GLOBALS['drupal_test_info']['test_run_id'])) {
    $instances = array(
      new MapsImportConfigurationFetcherMock($profile),
      new MapsImportObjectsFetcherMock($profile),
      new LibraryMapping($profile),
      new MediaMapper($profile),
      new ObjectMapper($profile),
    );
  }
  else {
    $instances = array(
      new ConfigurationFetcher($profile),
      new ObjectsFetcher($profile),
      new LibraryMapping($profile),
      new MediaMapper($profile),
      new ObjectMapper($profile),
    );
  }

  foreach ($instances as $instance) {
    $classname = get_class($instance);

    $operations[$instance->getType()] = array(
      'title' => $instance->getTitle(),
      'description' => $instance->getDescription(),
      'class' => $classname,
    );
  }

  return $operations;
}

/**
 * Implements hook_maps_import_operation_finished().
 */
function maps_import_maps_import_operation_finished($type) {
  switch ($type) {
    case 'configuration_fetch':
      CacheProfile::getInstance()->clearBinCache();
      break;

    case 'object_mapping':
      if (module_exists('rules')) {
        $args = func_get_args();
        unset($args[0]);
        rules_invoke_event_by_args('maps_import_mapping_completed', $args);
      }
      break;
  }
}
