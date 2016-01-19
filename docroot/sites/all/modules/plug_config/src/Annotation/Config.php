<?php

/**
 * @file
 * Contains \Drupal\plug_config\Annotation\Config.
 */

namespace Drupal\plug_config\Annotation;

use Drupal\Component\Annotation\Plugin;

/**
 * Defines a Config annotation object.
 *
 * @ingroup plug_config_api
 *
 * @Annotation
 */
class Config extends Plugin {

  /**
   * The config type ID.
   *
   * @var string
   */
  public $id;

  /**
   * The human-readable name of the plugin type.
   * @var string
   */
  public $label;

  /**
   * The name of the class that is used to load the objects.
   *
   * The class has to implement the DrupalEntityControllerInterface interface.
   *  Leave blank to use the DrupalDefaultEntityController implementation.
   *
   * @var string
   */
  public $controller_class;

  /**
   * A controller class name for providing the UI. Defaults to
   * EntityDefaultUIController, which implements an admin UI
   * suiting for managing configuration entities. Other provided controllers
   * suiting for content entities are EntityContentUIController or
   * EntityBundleableUIController (which work fine despite the poorly named
   * 'admin ui' key).
   *  For customizing the UI inherit from the default class and override
   *  methods as suiting and specify your class as controller class.
   *
   * @var string
   */
  public $controller_ui_class;

  /**
   * An array of keys as defined by Drupal core. The following
   * additional keys are used by the entity CRUD API:
   *  - name: (optional) The key of the entity property containing the unique,
   *    machine readable name of the entity. If specified, this is used as
   *    identifier of the entity, while the usual 'id' key is still required and
   *    may be used when modules deal with entities generically, or to refer to
   *    the entity internally, i.e. in the database.
   *    If a name key is given, the name is used as entity identifier by the
   *    entity API module, metadata wrappers and entity-type specific hooks.
   *    However note that for consistency all generic entity hooks like
   *    hook_entity_load() are invoked with the entities keyed by numeric id,
   *    while entity-type specific hooks like hook_{entity_type}_load() are
   *    invoked with the entities keyed by name.
   *    Also, just as entity_load_single() entity_load() may be called
   *    with names passed as the $ids parameter, while the results of
   *    entity_load() are always keyed by numeric id. Thus, it is suggested to
   *    make use of entity_load_multiple_by_name() to implement entity-type
   *    specific loading functions like {entity_type}_load_multiple(), as this
   *    function returns the entities keyed by name. See entity_test_get_types()
   *    for an example.
   *    For exportable entities, it is strongly recommended to make use of a
   *    machine name as names are portable across systems.
   *    This option requires the EntityAPIControllerExportable to work.
   *  - module: (optional) A key for the module property used by the entity CRUD
   *    API to save the source module name for exportable entities that have been
   *    provided in code. Defaults to 'module'.
   *  - status: (optional) The name of the entity property used by the entity
   *    CRUD API to save the exportable entity status using defined bit flags.
   *    Defaults to 'status'. See entity_has_status().
   *  - default revision: (optional) The name of the entity property used by
   *    the entity CRUD API to determine if a newly-created revision should be
   *    set as the default revision. Defaults to 'default_revision'.
   *    Note that on entity insert the created revision will be always default
   *    regardless of the value of this entity property.
   *
   * @var array
   */
  public $entity_keys;

  /**
   * A path where the UI should show up as expected by hook_menu().
   * @var string
   */
  public $path;

}
