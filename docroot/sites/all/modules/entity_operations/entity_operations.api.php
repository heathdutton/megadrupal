<?php

/**
 * @file
 * Hooks provided by the Entity Operations module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Provide an entity type via the remote entity API.
 *
 * This is a placeholder for describing further keys for hook_entity_info(),
 * which are introduced by the entity operations API for providing a UI for
 * an entity type.
 *
 *  -'operations ui': An array containing the following properties:
 *    - 'controller class' (optional) The name of a class to use as a controller
 *      for the operations system. Defaults to
 *      EntityOperationsDefaultUIController.
 *    - 'path': The base path for an entity. This should not include the
 *      wildcard or the trailing slash. For example, for nodes this would
 *      be 'node'.
 *      Note that to avoid duplication of logic here and in the entity's URI
 *      callback, the 'entity uri' property may be set to the generic callback
 *      entity_operations_entity_uri(), which makes use of this base path.
 *    - 'menu wildcard': (optional) A menu wildcard for loading the entity. This
 *      defaults to '%entity_object', which should be suitable in most cases.
 *      If set, should be of the form '%foo' where foo_load() is a menu loader
 *      function. For example, for nodes this would be '%node'.
 *
 * Other required properties:
 *  - 'module' must be set.
 *  - 'entity class' must be Entity or a subclass.
 *  - 'access callback' must be a callback that works with entity_access().
 *  - 'admin ui' must be set if using the add or edit operations.
 *    - 'file path' should be set, because it's consumed by another module.
 *    - 'controller' may be set to 'EntityOperationsDefaultAdminUIController'
 *      or a subclass, to override the default admin links on entities with
 *      those for operations.
 *
 * @see hook_entity_info()
 */
function entity_operations_hook_entity_info() {
}

/**
 * Define operations available to entities.
 *
 * @return
 *  An array keyed by entity type, where each item is itself an array of the
 *  operations available for that entity type. This array of operations is
 *  keyed by the operation name which is an arbitrary machine string, and each
 *  item is an array with the following properties:
 *    - 'handler': The name of the handler class for this operation.
 *    - 'provision': An array defining where this operation is made available.
 *      Each key is a provision type (listed further down) and each value may
 *      be one of:
 *      - TRUE: If the operation is to be provided in this channel, and no
 *        further settings need to be specified.
 *      - FALSE: If the operation is to be explicitly not provided in this
 *        channel. This allows overriding of the channels the handler defines.
 *      - An array of values providing further settings for the provision
 *        channel. The values here depend on the particular provision type.
 *      This module recognizes the following keys for the provision; other
 *      modules may make use of further ones.
 *      - 'menu': The operation is available as a menu path, usually as tabs
 *        below the main entity URI. The following further properties may be
 *        specified in an array:
 *        - 'default': (optional) Whether this operation is the default. Only
 *          one operation may be the default. This causes it to respond to the
 *          base URI of the entity, ie 'base_path/%wildcard'. For nodes, the
 *          'view' operation would be the default.
 *        - 'default secondary': (optional) If the operation name contains a
 *          '/', the setting this to TRUE causes this operation to be the
 *          default in the secondary tab set implicitly formed by the first
 *          part of the name. For example, 'foo/bar' and 'foo/biz' form a
 *          secondary tab set at the subpath 'foo', and one of the two should
 *          be marked as default secondary, so that it is output at 'foo' (and
 *          thus there should be no operation defined for 'foo').
 *          Note that it is possible for one such secondary item to also be
 *          marked as 'default', in which case the entity URI will output it.
 *        - 'parent tab title': (optional) Can be set on a secondary-level
 *          operation which has 'default' set to define the tab title for the
 *          containing primary-level tab that is automatically created.
 *        - 'parent weight': (optional) Similarly to 'parent tab title', this
 *          affects the primary-level tab which is automatically created for a
 *          secondary default tab.
 *        - 'menu item': (optional) An array of properties suitable for
 *          hook_menu(), to use for this operation's menu item. These override
 *          those provided by the handler.
 *      - 'views field': The operation is available as a Views field on the
 *        entity's base table, which shows a link to the operation tab. (This
 *        also requires 'menu' to be set.)
 *      - 'entity view': The operation is shown by default in the fieldset of
 *        operation forms returned by
 *        entity_operations_get_entity_operations_fieldset(), and shown on the
 *        entity view if the EntityOperationsOperationEntityViewOperations
 *        handler is used.
 *      - TODO: vbo
 *      - TODO: services
 *      - TODO: admin UI links
 *      Values of a provision type array can be one of:
 *      - TRUE: If the operation is to be provided in this channel, and no
 *        further settings need to be specified.
 *      - FALSE: If the operation is to be explicitly not provided in this
 *        channel. This allows overriding of the channels the handler defines.
 *      - An array of values depending on the channel type.
 */
function hook_entity_operation_info() {
  $info = array(
    'myentity' => array(
      // List of operations this entity type uses.
      'view' => array(
        // The handler class.
        'handler' => 'EntityOperationsOperationEntityView',
        // The places this operation is exposed.
        'provision' => array(
          // Show as an entity tab.
          'menu' => array(
            // Marks this as the default operation tab, that is, the one that
            // shows for the entity's URI.
            'default' => TRUE,
            // Properties for the menu item.
            'menu item' => array(
              // Override the title.
              'title' => 'Foobar',
            ),
          ),
          // A field handler for Views.
          // If there are no further settings, for easier DX it suffices to set
          // a provision item's value to TRUE.
          'views field' => TRUE,
        ),
      ),
      'edit' => array(
        'handler' => 'EntityOperationsOperationEdit',
        'provision' => array(
          'menu' => TRUE,
          'views field' => TRUE,
        ),
      ),
      'publish' => array(
        'handler' => 'EntityOperationsOperationPublish',
        'provision' => array(
          // Show the operation form as an entity tab.
          'menu' => TRUE,
        ),
      ),
    ),
  );
  return $info;
}

/**
 * Alter entity operation definitions.
 *
 * @param $operation_info
 *  The array of data provided by hook_entity_operation_info().
 */
function hook_entity_operation_info_alter(&$operation_info) {
  // Replace the handler for the edit operation.
  $operation_info['myentity']['edit']['handler'] = 'MyEntityCustomHandlerClass';
}

/**
 * Alter entity operations prior to building hook_menu() items.
 *
 * This allows the addition of operations when menu items are being created. In
 * particular, it prevents circularity with Views both defining operations and
 * consuming them.
 *
 * @param $entity_operations
 *  The array of entity operations, taken from hook_entity_info().
 * @param $entity_type
 *  The entity type.
 */
function hook_entity_operations_menu_operations_alter(&$entity_operations, $entity_type) {
}
