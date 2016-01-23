<?php

/**
 * @file
 * Hooks provided by the Entity Bundle Admin module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide basic bundle admin UI.
 *
 * This is a placeholder for describing further keys for hook_entity_info(),
 * which are introduced by the entity bundle admin module for providing a UI for
 * an entity type's bundles.
 *
 *  -'bundle admin ui': An array containing the following properties:
 *    - 'path': The base path for the admin UI, eg 'admin/structure/TYPE'.
 *      Each bundle will get a path at PATH/manage/BUNDLE.
 *    - 'permission': The permission string to use for access to the admin UI
 *      pages.
 *    - 'controller class' (optional) The name of a class to use as a controller
 *      for the bundle admin UI generation. Defaults to
 *      EntityBundleAdminDefaultUIController.
 *    - 'bundle hook': (optional) The hook to invoke to collect bundle type
 *      definitions. Defaults to hook_entity_bundle_ENTITY_TYPE_bundle_info().
 *
 * @see hook_entity_info()
 */
function entity_operations_hook_entity_info() {
}

/**
 * Declare bundles for an entity.
 *
 * Note that this may not declare a bundle that is of the same name as the
 * entity type, if hook_entity_info() declares no bundles. This is due to the
 * way that entity_get_info() collects data.
 *
 * @return
 *  An array keyed by bundle machine name, whose values are arrays with the
 *  following properties:
 *  - 'label': The bundle label.
 *  - 'description': A description of the bundle, for use in the admin UI.
 *    This is added to the entity info, for modules such as Entity Operations to
 *    make use of.
 * Any further properties are set in the bundle definition in the bundles array
 * for hook_entity_info().
 *
 * @see entity_bundle_admin_get_entity_bundles()
 */
function hook_entity_bundle_ENTITY_TYPE_bundle_info() {
  return array(
    'mybundle' => array(
      'label' => t('My bundle'),
      'description' => t('Description text'),
    ),
  );
}

/**
 * Alter bundle definitions for an entity.
 *
 * @param $bundle_info
 *  The array of bundle info for the entity type.
 */
function hook_entity_bundle_ENTITY_TYPE_bundle_info_alter(&$bundle_info) {
  $bundle_info['mybundle']['label'] = t('Changed label');
}
