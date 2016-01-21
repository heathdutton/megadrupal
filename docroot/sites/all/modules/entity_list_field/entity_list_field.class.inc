<?php

/**
 * @file
 * Entity List Field class file.
 */

class EntityListFieldList extends EntityLister {

  public $config;
  public $delta;
  public $idPrefix;
  public $hostItem;

  /**
   * Construct the list object.
   *
   * @param array $config
   *   The config options for the list, in an associative array. For more
   *   information about the $config array, please see the EntityLister class.
   * @param int $delta
   *   The delta.
   * @param string $id_prefix
   *   The field name.
   * @param array $host_item
   *   When this class is used for a field formatter, this array describes the
   *   item to which the field is attached. See below.
   *
   * The $host_item array:
   *   $host_item[0] The host item's entity_id.
   *   $host_item[1] The host item's revision_id.
   *   $host_item[2] The host item's bundle.
   *   $host_item[3] The host item's entity type.
   */
  public function __construct($config, $delta, $id_prefix, $host_item) {

    $this->config = $config;
    $this->delta = $delta;
    $this->idPrefix = $id_prefix;
    $this->hostItem = $host_item;

  }

  /**
   * Run the query object through filterHost().
   *
   * @param object $query
   *   The query object.
   *
   * @return object
   *   The query object.
   */
  protected function alterQuery($query) {

    return $this->filterHost($query);

  }

  /**
   * Don't allow the container entity to display itself in a list.
   *
   * One would think we do this to avoid an infinite loop, but in fact, if we
   * try to list an entity inside itself it triggers an "unsupported operand"
   * error when fields are attached in the node module.  In any case it's
   * unlikely an application would need an entity to display a list including
   * itself, so we make it impossible here to avoid a fatal error.
   *
   * @param object $query
   *   The query object.
   *
   * @return object
   *   The query object.
   */
  protected function filterHost($query) {

    $type = $this->config['type'];
    $bundle = $this->config['bundle'];

    // Are we listing entities of type foo inside an entity of type foo?
    if ($type == $this->hostItem[3]) {
      // Is the host entity's bundle among the bundles we are listing?
      if (in_array($this->hostItem[2], $bundle)) {
        // Exclude the host entity from the list.
        $query->entityCondition('entity_id', $this->hostItem[0], '!=');
      }
    }

    return $query;

  }

  /**
   * Run the entities through safeLoop().
   *
   * @param array $entities
   *   The entities.
   *
   * @return array
   *   The entities.
   */
  protected function alterEntities($entities) {

    return $this->safeLoop($entities);

  }

  /**
   * Prevent infinite loops.
   *
   * Prevent display of Entity List fields in listed items.  This prevents a
   * scenario where an entity of type foo lists entities of type bar which
   * list entities of type foo, including the host entity.
   *
   * @param array $entities
   *   The entities.
   *
   * @return array
   *   The entities.
   */
  protected function safeLoop($entities) {

    $has = FALSE;

    foreach ($this->bundleFields as $value) {
      // Check if any of the bundles have an Entity List field.
      if ('entity_list_field' == $value['widget']['module']) {
        $fn = $value['field_name'];
        $has[$fn] = $fn;
      }
    }

    if ($has) {
      foreach ($has as $name) {
        // Remove the list fields from the items.
        foreach ($entities as $key => $entity) {
          if (isset($entity->{$name})) {
            unset($entity->{$name});
          }
          $entities[$key] = $entity;
        }
      }
    }

    return $entities;

  }

  /**
   * Activate the ajax pager.
   */
  protected function pager() {

    $path = drupal_get_path('module', 'entity_lister');
    drupal_add_js($path . '/ajax-pager.js');

    $extra = array(
      'host_item' => $this->hostItem,
    );

    $js_settings[$this->delta] = array(
      'config' => $this->config,
      'id_prefix' => $this->idPrefix,
      'list_class' => get_class($this),
      'extra' => $extra,
    );
    drupal_add_js(array('entity_lister' => $js_settings), 'setting');

  }

}
