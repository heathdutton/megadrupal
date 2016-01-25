<?php

/**
 * @file
 * Definition of synonyms_views_handler_filter_term_tid class.
 */

/**
 * Synonyms friendly taxonomy filter handler.
 */
class synonyms_views_handler_filter_term_tid extends views_handler_filter_term_node_tid {
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['type']['#options']['synonyms_autocomplete'] = t('Synonyms friendly autocomplete');
  }

  function value_form(&$form, &$form_state) {
    $restore_value = $this->options['type'] == 'synonyms_autocomplete';
    if ($restore_value) {
      $this->options['type'] = 'textfield';
    }
    parent::value_form($form, $form_state);

    if ($restore_value) {
      // We need to determine the entity type onto which this field is attached
      // that is used in this view.
      $entity_type_base_table = $this->view->base_table;
      // TODO: it would be nice to consider the existence of relationships, but
      // I just couldn't figure it out at that time.

      $entity_info = entity_get_info();
      $field_entity_type = FALSE;
      $field = field_info_field($this->definition['field_name']);

      foreach ($field['bundles'] as $entity_type => $bundles) {
        if ($entity_info[$entity_type]['base table'] == $entity_type_base_table) {
          $field_entity_type = $entity_type;
          break;
        }
      }

      if (!$field_entity_type) {
        // Seems like we failed to determine the entity type which is used for
        // this field in the view. Well, it's not a fatal fail, we'll just use
        // whatever then.
        $field_entity_type = array_keys($field['bundles']);
        $field_entity_type = $field_entity_type[0];
      }

      // We just grab the first instance of this field within the determined
      // entity type.
      $bundle = $field['bundles'][$field_entity_type][0];

      $instance = field_info_instance($field_entity_type, $field['field_name'], $bundle);
      if ($instance['widget']['type'] == 'synonyms_autocomplete') {
        $widget = $instance['widget']['settings'];
      }
      else {
        $widget = field_info_widget_settings('synonyms_autocomplete');
      }
      $autocomplete_path = $widget['synonyms_autocomplete_path'];
      $size = $widget['size'];

      $form['value']['#autocomplete_path'] = $autocomplete_path . '/' . $this->definition['field_name'] . '/' . $field_entity_type . '/' . $bundle;
      $form['value']['#size'] = $size;
      $form['value']['#auto_creation'] = FALSE;
      $form['value']['#attributes']['class'][] = 'synonyms-autocomplete';
      $form['value']['#attached']['js'][drupal_get_path('module', 'synonyms') . '/js/synonyms-autocomplete.js'] = array();
      $this->options['type'] = 'synonyms_autocomplete';
    }
  }

  function value_validate($form, &$form_state) {
    if ($this->options['type'] == 'synonyms_autocomplete') {
      $values = drupal_explode_tags($form_state['values']['options']['value']);
      $tids = $this->synonyms_validate_term_strings($form['value'], $values);

      if ($tids) {
        $form_state['values']['options']['value'] = $tids;
      }
    }
    else {
      parent::value_validate($form, $form_state);
    }
  }

  function exposed_validate(&$form, &$form_state) {
    if ($this->options['type'] == 'synonyms_autocomplete') {
      if (empty($this->options['exposed'])) {
        return;
      }
      if (empty($this->options['expose']['identifier'])) {
        return;
      }

      $identifier = $this->options['expose']['identifier'];
      $values = drupal_explode_tags($form_state['values'][$identifier]);
      $tids = $this->synonyms_validate_term_strings($form[$identifier], $values);

      if ($tids) {
        $this->validated_exposed_input = $tids;
      }
    }
    else {
      parent::exposed_validate($form, $form_state);
    }
  }

  /**
   * Validate the user string.
   *
   * In a great extend it does the same job as parent::validate_term_strings(),
   * just that this implementation is synonyms-aware.
   *
   * @param $element
   *   The form element which is used, either the views ui or the exposed
   *   filters.
   * @param $values
   *   The taxonomy names/synonyms which will be converted to tids.
   *
   * @return array
   *   The taxonomy ids for all validated terms.
   */
  protected function synonyms_validate_term_strings($element, $values) {
    if (empty($values)) {
      return array();
    }

    $values = array_map('drupal_strtolower', $values);
    $missing = array_flip($values);

    $tids = array();

    $vocabulary = taxonomy_vocabulary_machine_name_load($this->options['vocabulary']);

    // Firstly looking up the entered tags as if they were term names. Then,
    // the remaining tags are looked up as if they were synonyms of terms.
    // Lastly, if any tags are left at this point, we mark form validation
    // error.
    $query = db_select('taxonomy_term_data', 'td');
    $query->fields('td', array('tid', 'name'));
    $query->condition('td.vid', $vocabulary->vid);
    $query->condition('td.name', $values);
    $query->addTag('term_access');
    $result = $query->execute();
    foreach ($result as $term) {
      unset($missing[drupal_strtolower($term->name)]);
      $tids[] = $term->tid;
    }

    $behavior_implementations = synonyms_behavior_get('autocomplete', 'taxonomy_term', $vocabulary->machine_name, TRUE);
    foreach ($behavior_implementations as $behavior_implementation) {
      if (!empty($missing)) {
        $condition = db_or();
        foreach ($missing as $tag => $v) {
          $condition->condition(AbstractSynonymsSynonymsBehavior::COLUMN_PLACEHOLDER, $tag);
        }
        $synonyms = synonyms_synonyms_find_behavior($condition, $behavior_implementation);
        foreach ($synonyms as $synonym) {
          $synonym->synonym = drupal_strtolower($synonym->synonym);
          unset($missing[$synonym->synonym]);
          $tids[] = $synonym->entity_id;
        }
      }
    }

    if (!empty($missing) && !empty($this->options['error_message'])) {
      form_error($element, format_plural(count($missing), 'Unable to find term: @terms', 'Unable to find terms: @terms', array('@terms' => implode(', ', array_keys($missing)))));
    }

    return $tids;
  }
}
