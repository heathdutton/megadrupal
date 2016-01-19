<?php
/**
 * Argument handler to accept an entity id.
 *
 * @ingroup views_argument_handlers
 */
class similarreferences_handler_argument_id extends views_handler_argument_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['similarreferences'] = array(
      'contains' => array(
        'field_names' => array('default' => array()),
        'include_args' => array('default' => FALSE),
        'require_all' => array('default' => FALSE),
      ),
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    unset($form['not']);

    $fields = array();
    // Use CTools to get the best matching field name.
    ctools_include('fields');
    foreach (field_info_fields() as $id => $field) {
      if ($field['type'] != 'entityreference') {
        // The field is not an entity reference field.
        continue;
      }

      $entity_info = entity_get_info($field['settings']['target_type']);
      if ($entity_info['base table'] != $this->view->base_table) {
        // Field doesn't reference the right base table.
        continue;
      }
      $field_name = $field['field_name'];
      $fields[$field_name] = ctools_field_label($field_name) . ' (' . $field_name . ')';
    }

    $form['similarreferences'] = array(
      '#type' => 'fieldset',
      '#title' => t('Similarity by References'),
      '#collapsible' => FALSE,
    );
    $form['similarreferences']['field_names'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Limit similarity to references from these fields'),
      '#description' => t('Choose entityreference fields that will be used to calculate similarity.'),
      '#options' => $fields,
      '#default_value' => empty($this->options['similarreferences']['field_names']) ? array() : $this->options['similarreferences']['field_names'],
      '#required' => TRUE,
      '#multiple' => TRUE,
    );

    $form['similarreferences']['require_all'] = array(
      '#type' => 'checkbox',
      '#title' => t('Require at least one match with each field selected above'),
      '#description' => t('If selected, an entity will have to match each field selected above at least once. Otherwise, sharing a value with only one of the fields will result in a match.'),
      '#default_value' => empty($this->options['similarreferences']['require_all']) ? FALSE : $this->options['similarreferences']['require_all'],
    );

    $form['similarreferences']['include_args'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include argument entity in results'),
      '#description' => t('If selected, the entity passed as the argument will be included in the view results.'),
      '#default_value' => !empty($this->options['similarreferences']['include_args']),
    );

  }

  function validate_arg($arg) {
    // First run the inherited arg validation.
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }

    // Find the entityreference values on the target entity.
    // These will be the values we compare to when determining similarity.
    $this->related_ids = array();

    // @TODO There are a few cases where the entity type is not the same as the base table.
    $entity_type = $this->view->base_table;

    $entities = entity_load($entity_type, $this->value);
    foreach ($entities as $entity) {
      foreach ($this->options['similarreferences']['field_names'] as $field_name) {
        if ($values = field_get_items($entity_type, $entity, $field_name)) {
          foreach ($values as $value) {
            $this->related_ids[$field_name][] = $value['target_id'];
          }
        }
      }
    }
    $this->view->related_ids = $this->related_ids;

    if (count($this->related_ids) == 0) {
      // There are no related ids ... we need to cancel the query and bail out.
      return FALSE;
    }

    return TRUE;
  }

  /**
   * Helper function to retrieve db data for selected fields.
   */
  function get_field_data() {
    $field_data = array();
    foreach ($this->options['similarreferences']['field_names'] as $field_name => $value) {
      $field = field_info_field($field_name);
      // Make sure we are dealing with SQL storage.
      if ($field['storage']['type'] == 'field_sql_storage') {
        $db_info = $field['storage']['details']['sql']['FIELD_LOAD_CURRENT'];
        $tables = array_keys($db_info);
        $table = array_pop($tables);
        $field_data[$field_name] = array(
          'table' => $table,
          'column' => array_pop($db_info[$table]),
        );
      }
    }
    return $field_data;
  }

  function query($group_by = FALSE) {

    $this->ensure_my_table();

    $db_info = $this->get_field_data();
    foreach ($this->options['similarreferences']['field_names'] as $field_name) {
      if (is_string($field_name)) {
        $table = $db_info[$field_name]['table'];
        $column = $db_info[$field_name]['column'];
        if (array_key_exists($field_name, $this->related_ids)) {
          $alias = $this->query->add_field($table, $column);

          $this->query->add_where('refs', $table . '.' . $column, $this->related_ids[$field_name], 'IN');
          if (empty($this->options['similarreferences']['require_all'])) {
            $this->query->where['refs']['type'] = 'OR';
          }
        }
      }
    }

    // Exclude the current entity.
    if (empty($this->options['include_args'])) {
      $this->query->add_where(0, $this->view->base_table . '.' . $this->view->base_field, $this->value, 'NOT IN');
    }
  }

}
