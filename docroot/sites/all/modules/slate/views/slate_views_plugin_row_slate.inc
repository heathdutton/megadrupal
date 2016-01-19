<?php
/**
 * @file
 */


class slate_views_plugin_row_slate extends views_plugin_row {

  protected $entity_type, $entities;

  public function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);

    // Initialize the entity-type used.
    $table_data = views_fetch_data($this->view->base_table);
    $this->entity_type = $table_data['table']['entity type'];
    // Set base table and field information as used by views_plugin_row to
    // select the entity id if used with default query class.
    $info = entity_get_info($this->entity_type);
    if (!empty($info['base table']) && $info['base table'] == $this->view->base_table) {
      $this->base_table = $info['base table'];
      $this->base_field = $info['entity keys']['id'];
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['slate'] = array('default' => NULL);

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $entity_info = entity_get_info($this->entity_type);

    $subtype = 'entity:' . $this->entity_type;

    $options = array();

    foreach (slate_get_slates() as $slate => $slate_info) {
      if (count($slate_info['models']) != 1) {
        continue;
      }

      $model = reset($slate_info['models']);

      if ($model['type'] !== $subtype) {
        continue;
      }

      $options[$slate] = $slate_info['name'];
    }

    $form['slate'] = array(
      '#type' => 'select',
      '#title' => 'Slate',
      '#options' => $options,
      '#default_value' => $this->options['slate'],
    );

    return $form;
  }

  public function pre_render($values) {
    if (!empty($values)) {
      list($this->entity_type, $this->entities) = $this->view->query->get_result_entities($values, !empty($this->relationship) ? $this->relationship : NULL, isset($this->field_alias) ? $this->field_alias : NULL);
    }
  }

  function get_value($values, $field = NULL) {
    return isset($this->entities[$this->view->row_index]) ? $this->entities[$this->view->row_index] : FALSE;
  }

  public function render($values) {
    if ($entity = $this->get_value($values)) {
      $slate_info = slate_get_slate($this->options['slate']);

      $model = key($slate_info['models']);

      return slate_render($this->options['slate'], array($model => $entity));
    }
  }

}
