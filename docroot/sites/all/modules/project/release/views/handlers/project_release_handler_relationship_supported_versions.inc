<?php

/**
 * Views handler for relationships between releases and supported version info.
 */
class project_release_handler_relationship_supported_versions extends views_handler_relationship {
  function init(&$view, &$options) {
    parent::init($view, $options);
    // This relationship only works with {project_release_supported_versions}
    $this->definition['base'] = 'project_release_supported_versions';
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $api_vocabulary = taxonomy_vocabulary_load(variable_get('project_release_api_vocabulary', 0));
    $this->ensure_my_table();
    $major_alias = $this->query->ensure_table('field_data_field_release_version_major');
    if (!empty($api_vocabulary)) {
      $term_alias = $this->query->ensure_table('taxonomy_index');
    }
    $join = new views_join();
    $alias = $this->definition['base'] . '_' . $this->table_alias;
    $join->definition = array(
      'table' => $this->definition['base'],
      'field' => 'nid',
      'left_table' => $this->table_alias,
      'left_field' => $this->field,
      'extra' => $alias . '.major = ' . $major_alias . '.field_release_version_major_value'
      . (!empty($api_vocabulary) ? (' AND ' . $alias . '.tid = ' . $term_alias . '.tid') : '')
      . ' AND ' . $alias . '.supported = 1',
    );
    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }
    $join->construct();
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}
