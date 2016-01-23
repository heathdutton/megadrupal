<?php

/**
 * Controller class for the bundle admin UI.
 *
 * Registers paths with hook_menu(), based on entity bundles.
 */
class EntityBundleAdminDefaultUIController {

  protected $entityType;
  protected $entityInfo;

  public function __construct($entity_type, $entity_info) {
    $this->entityType = $entity_type;
    $this->entityInfo = $entity_info;
  }

  /**
   * Returns items for hook_menu() for an entity's bundles.
   */
  function hook_menu() {
    $items = array();

    $base_path = $this->entityInfo['bundle admin ui']['path'];

    // The base item.
    $items[$base_path] = array(
      'title' => '@entity types',
      'title arguments' => array(
        '@entity' => $this->entityInfo['label'],
      ),
      // No idea if this is valid, but there's no other way to put placeholders
      // into this.
      'description' => format_string('Manage fields on @entity entities.', array(
        '@entity' => $this->entityInfo['label'],
      )),
      'page callback' => 'entity_bundle_admin_types_list_page',
      'page arguments' => array($this->entityType),
      'access arguments' => array($this->entityInfo['bundle admin ui']['permission']),
      'file' => 'entity_bundle_admin.admin.inc',
    );

    $bundles = $this->entityInfo['bundles'];
    foreach ($bundles as $bundle_name => $bundle_info) {
      // The admin page for the bundle field admin UI to hang off.
      // TODO: can we get away with a wildcard?
      foreach ($bundles as $bundle_name => $bundle_info) {
        $items["$base_path/manage/$bundle_name"] = array(
          'title' => $bundle_info['label'],
          'page callback' => 'entity_bundle_admin_types_type_page',
          'page arguments' => array($this->entityType, $bundle_name),
          'access arguments' => array($this->entityInfo['bundle admin ui']['permission']),
          'file' => 'entity_bundle_admin.admin.inc',
        );
        // We need this for Field UI tabs to work.
        $items["$base_path/manage/$bundle_name/view"] = array(
          'title' => 'Info',
          'type' => MENU_DEFAULT_LOCAL_TASK,
        );
      }
    }

    return $items;
  }

}
