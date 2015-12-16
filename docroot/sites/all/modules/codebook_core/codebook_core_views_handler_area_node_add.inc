<?php

/**
 * Provides an "area" populated with links to add Codebook content.
 */
class codebook_core_views_handler_area_node_add extends views_handler_area {

  /**
   * Define the options available for this handler.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['codebook_types'] = array(
      'default' => array(),
    );
    $options['prepopulate_reference'] = array(
      'default' => TRUE,
    );

    return $options;
  }

  /**
   * Provide a form for setting option configurations.
   */
  function options_form(&$form, &$form_state) {

    $form['codebook_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Codebook content types'),
      '#options' => $this->codebook_types(),
      '#default_value' => $this->options['codebook_types'],
    );

    $form['prepopulate_reference'] = array(
      '#type' => 'checkbox',
      '#title' => t('Prepopulate Codebook parent reference'),
      '#default_value' => $this->options['prepopulate_reference'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render some output.
   */
  function render($empty = FALSE) {
    $vars['items'] = array();

    // Get the configured types, revert to all if not configured.
    $types = array_filter($this->options['codebook_types']);
    $types = (!empty($types)) ? $types : $this->codebook_types();

    $node_types = node_type_get_names();

    // Add in each type of link.
    foreach ($types as $type => $val) {
      if (node_access('create', $type)) {
        $label = t('Add !type', array('!type' => $node_types[$type]));
        $vars['items'][] = l($label, $this->node_add_path($type), $this->link_options($type));
      }
    }

    if ($vars['items']) {
      $output = theme('item_list', array('items' => $vars['items'], 'attributes' => array('class' => 'codebook-create-links')));
      return $output;
    }
  }

  /**
   * Provides a list of codebook node types.
   */
  protected function codebook_types() {
    // Grab available node types, filter for codebook specific types.
    $types = node_type_get_names();
    $codebook_types = array();
    foreach ($types as $type => $name) {
      if (substr($type, 0, 9) == 'codebook_') {
        $codebook_types[$type] = $name;
      }
    }
    return $codebook_types;
  }

  /**
   * Provides the path to create a node of a given type.
   */
  protected function node_add_path($type) {
    $type = str_replace('_', '-', $type);
    return "node/add/$type";
  }

  /**
   * Creates an options array for a link with the various query params set.
   */
  protected function link_options($type) {
    $options = array();

    if ($this->options['prepopulate_reference']) {

      // Find the entity to reference, currently-viewed node.
      if (($node = menu_get_object()) && ($ref_field = $this->get_reference_field($type))) {
        $options['query'][$ref_field] = $node->nid;
      }
    }

    $options['query']['destination'] = current_path();

    return $options;
  }

  /**
   * Finds the codebook reference field instance on a given node type.
   */
  protected function get_reference_field($type) {
    // Find the field that holds the codebook reference.
    $codebook_refs = array('field_codebook_title', 'field_codebook_chapter');

    // Pluck out the fields we may be intrerested in.
    $fields = array_intersect_key(field_info_field_map(), array_flip($codebook_refs));

    foreach ($fields as $field => $info) {
      // Look for a 'field_codebook_' prefixed entityreference type field.
      if (!empty($info['bundles']['node']) && in_array($type, $info['bundles']['node'])) {
        return $field;
      }
    }
  }

}
