<?php

/**
 * @file
 * Contains the default display plugin.
 */

/**
 * A plugin to handle defaults on a view.
 *
 * @ingroup views_display_plugins
 */
class term_reference_references_dialog_plugin_display extends references_dialog_plugin_display {
  function render() {
    $output = theme($this->theme_functions(), array('view' => $this->view));
    // Let's add the data necessary to javascript, so that we can
    // act upon it there.
    $js_result = array();
    $entity_type = $this->get_entity_type();
    // The only safe way to determine the title of each item is to load the entities.
    // We also need to know the specific
    $entity_ids = array();
    foreach ($this->view->result as $row => $result) {
      $entity_ids[] = $result->{$this->view->base_field};
    }
    $entities = entity_load($entity_type, $entity_ids);
    if ($entity_type == 'taxonomy_term') {
      $field_info = explode(':', $this->view->references_dialog['attachable']['name']);
      $field = field_info_field($field_info[1]);
      $instance = field_info_instance($field_info[0], $field_info[1], $field_info[2]);
    }
    foreach ($this->view->result as $result) {
      if (isset($field)) {
        $view_term = _get_referencable_terms_from_view($field, $instance, NULL, 0, array($result->{$this->view->base_field}));
        $title = current($view_term);
      }
      else {
        $title = entity_label($entity_type, $entities[$result->{$this->view->base_field}]);
      }
      $js_result[] = array(
        'entity_id' => $result->{$this->view->base_field},
        'title' => $title,
        'entity_type' => $entity_type,
      );
    }
    drupal_add_js(drupal_get_path('module', 'references_dialog') . '/js/search-reference.js');
    drupal_add_js(array('ReferencesDialog' => array('entities' => $js_result)), 'setting');
    return $output;
  }
}
