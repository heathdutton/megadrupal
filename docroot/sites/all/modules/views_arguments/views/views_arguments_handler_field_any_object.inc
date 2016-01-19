<?php

class views_arguments_handler_field_any_object extends views_handler_field {

  public $objects = NULL;
  public $hook_function = NULL;


  /**
   * Overrides views_handler_field::query().
   */
  function query() {
    parent::query();
  }


  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    $object = $this->get_object($values);
    if (!$object) {
      return '';
    }

    $context = array(
      'handler' => $this,
      'values' => $values,
    );

    $output = '';
    if ($function = $this->get_hook_function()) {
      $output = $function($object, $context);
    }

    return $output;
  }


  /**
   * Finds and remember this field's hook function, if it exists.
   */
  protected function get_hook_function() {
    if ($this->hook_function === NULL) {
      $this->hook_function = FALSE;

      $view = $this->view;
      $display = $view->display_handler->options['defaults']['fields'] ? 'default' : $view->current_display;
      $field = $this->options['id'];
      $hook = 'views_arguments_any_field__' . $view->name . '__' . $display . '__' . $field;

      $modules = module_implements($hook);
      if ($modules) {
        $module = end($modules);

        if (function_exists($function = $module . '_' . $hook)) {
          $this->hook_function = $function;
        }
      }
    }

    return $this->hook_function;
  }


  /**
   * Returns this row's this field's object.
   */
  protected function get_object($values) {
    $objects = $this->get_objects();

    $id = $this->get_value($values);
    return @$this->objects[$id];
  }


  /**
   * Fetches and stores all rows' this field's objects.
   */
  protected function get_objects() {
    if ($this->objects === NULL) {
      // this->get_value() would use field_alias, but we want ALL records.
      $alias = $this->field_alias;

      // Get ids from all rows' values.
      $ids = array_filter(array_map(function($record) use ($alias) {
        return $record->$alias;
      }, $this->view->result));

      // Load objects from views data configured type.
      $this->objects = $this->load_multiple($ids);
    }

    return $this->objects;
  }


  protected function load_multiple($ids) {
    // Loading entities is easy.
    $entity_type = @$this->definition['entity type'];
    if ($entity_type) {
      return entity_load($entity_type, $ids);
    }

    // No entities, so load directly from base table.
    return db_select($this->options['table'], 'x')
      ->fields('x')
      ->condition($this->field_alias, $ids)
      ->execute()
      ->fetchAllAssoc($this->field_alias);
  }


}
