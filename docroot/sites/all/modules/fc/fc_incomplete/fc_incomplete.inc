<?php
/**
 * @file
 * Field Incomplete - Provides a block displaying of what's currently incomplete on an entity - support.
 */


function fc_incomplete_build($entity_type, $entity) {
  $incompletes = new fcIncomplete('', NULL, $entity_type, $entity);
  $incompletes->process();
  return $incompletes;
}

class fcIncomplete extends ArrayObject {
  static protected $field_info;
  public $entity_type, $entity, $name, $complete, $parent, $bundle;

  public function __construct($name, $parent = NULL, $entity_type = NULL, $entity = NULL) {
    if (empty(self::$field_info)) {
      self::$field_info = field_info_fields();
    }

    $this->name = $name;
    $this->parent = $parent;
    $this->entity_type = $entity_type;
    $this->entity = $entity;
    if ($entity_type && $entity) {
      list(,, $this->bundle) = entity_extract_ids($entity_type, $entity);
    }
  }

  public function process() {
    $field_info = field_info_fields();
    list($id, $vid, $bundle) = entity_extract_ids($this->entity_type, $this->entity);
    $instances = field_info_instances($this->entity_type, $bundle);

    // Allow other modules to remove instances from the completeness check
    $options = array(
      'bundle' => $bundle,
      'context' => 'view',
    );
    drupal_alter('fc_instances', $instances, $this->entity_type, $this->entity, $options);
    uasort($instances, array($this, 'sortInstances'));

    foreach ($instances as $field_name => $instance) {
      $settings = $instance['settings']['fc'];
      if (!empty($settings['disable'])) {
        continue;
      }

      // Choose the right plugin for the field type.
      $field = $field_info[$field_name];
      $plugin = fc_get_plugin($field['type']);

      $field_items = field_get_items($this->entity_type, $this->entity, $field_name);
      if (empty($field_items)) {
        $this[$field_name] = new fcIncomplete($field_name, $this);
        if (!empty($settings['fc_allow_empty'])) {
          // Complex fields can be set so if they
          // don't they must be counted as complete.
          $this[$field_name]->complete = TRUE;
        }
        continue;
      }

      if ($function = ctools_plugin_get_function($plugin, 'completeness check')) {
        $incomplete = ctools_plugin_get_function($plugin, 'incomplete process');
        $cardinality = ctools_plugin_get_function($plugin, 'cardinality check');
        $this[$field_name] = $incomplete($this, $function, $cardinality, $field_items, $instance, $field);
      }
    }

    $this->complete = TRUE;
    $iterator = $this->getIterator();
    while($this->complete && $iterator->valid()) {
      $this->complete = $this->complete && $iterator->current()->complete;
      $iterator->next();
    }
  }

  protected function sortInstances($a, $b) {
    $p = $a['widget']['weight'];
    $q = $b['widget']['weight'];
    return $p==$q ? 0 : ($p>$q ? 1 : -1);
  }

  protected function entityType() {
    if (!empty($this->entity_type) && !empty($this->bundle)) {
      return array($this->entity_type, $this->bundle);
    }
    if (!empty($this->parent)) {
      return $this->parent->entityType();
    }
    return array(NULL, NULL);
  }

  public function render($depth = 0) {
    // Don't output if complete
    if ($this->complete) {
      return array();
    }

    list($entity_type, $bundle) = $this->entityType();

    $build = array(
      '#prefix' => '<div class="fc-incomplete-section" style="margin-left:' . $depth . 'em">',
      '#suffix' => '</div>',
      'header' => array(
        '#theme' => 'fc_incomplete_header',
        '#complete' => $this->complete,
        '#name' => $this->name,
        '#entity_type' => $entity_type,
        '#entity' => $this->entity,
        '#bundle' => $bundle,
        '#depth' => $depth,
      ),
      'parts' => array(
        '#prefix' => '<ul>',
        '#suffix' => '</ul>',
      ),
    );

    foreach ((array) $this as $sub) {
      if (is_object($sub) && !$sub->complete) {
        $build['parts'][] = array(
          '#prefix' => '<li>',
          '#suffix' => '</li>',
          'sub-item' => $sub->render($depth + 1),
        );
      }
    }
    return $build;
  }

}