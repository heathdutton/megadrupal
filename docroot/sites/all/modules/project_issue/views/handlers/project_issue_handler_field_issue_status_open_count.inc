<?php

/**
 * Views handler for displaying an open issue count.
 */
class project_issue_handler_field_issue_status_open_count extends views_handler_field_field {

  function query($use_groupby = FALSE) {
    $this->get_base_table();

    $params = array();
    if ($use_groupby) {
      // When grouping on a "field API" field (whose "real_field" is set to
      // entity_id), retrieve the minimum entity_id to have a valid entity_id to
      // pass to field_view_field().
      $params = array(
        'function' => 'min',
      );

      $this->ensure_my_table();
    }

    // Get the entity type according to the base table of the field.
    // Then add it to the query as a formula. That way we can avoid joining
    // the field table if all we need is entity_id and entity_type.
    $entity_type = $this->definition['entity_tables'][$this->base_table];
    $entity_info = entity_get_info($entity_type);

    if (isset($this->relationship)) {
      $this->base_table_alias = $this->relationship;
    }
    else {
      $this->base_table_alias = $this->base_table;
    }

    // We always need the base field (entity_id / revision_id).
    if (empty($this->definition['is revision'])) {
      $this->field_alias = $this->query->add_field($this->base_table_alias, $entity_info['entity keys']['id'], '', $params);
    }
    else {
      $this->field_alias = $this->query->add_field($this->base_table_alias, $entity_info['entity keys']['revision'], '', $params);
      $this->aliases['entity_id'] = $this->query->add_field($this->base_table_alias, $entity_info['entity keys']['id'], '', $params);
    }


    // The alias needs to be unique, so we use both the field table and the entity type.
    $entity_type_alias = $this->definition['table'] . '_' . $entity_type . '_entity_type';
    $this->aliases['entity_type'] = $this->query->add_field(NULL, "'$entity_type'", $entity_type_alias);

    $fields = $this->additional_fields;
    // We've already added entity_type, so we can remove it from the list.
    $entity_type_key = array_search('entity_type', $fields);
    if ($entity_type_key !== FALSE) {
      unset($fields[$entity_type_key]);
    }

    if ($use_groupby) {
      // Add the fields that we're actually grouping on.
      $options = array();

      if ($this->options['group_column'] != 'entity_id') {
        $options = array($this->options['group_column'] => $this->options['group_column']);
      }

      $options += is_array($this->options['group_columns']) ? $this->options['group_columns'] : array();


      $fields = array();
      $rkey = $this->definition['is revision'] ? 'FIELD_LOAD_REVISION' : 'FIELD_LOAD_CURRENT';
      // Go through the list and determine the actual column name from field api.
      foreach ($options as $column) {
        if ($column  != 'value') {
          $name = $column;
          if (isset($this->field_info['storage']['details']['sql'][$rkey][$this->table][$column])) {
            $name = $this->field_info['storage']['details']['sql'][$rkey][$this->table][$column];
          }

          $fields[$column] = $name;
        }

      }

      $this->group_fields = $fields;
    }

    // Always add this field to the query, as we're a SQL expression.
    $this->ensure_my_table();
    $open_states = project_issue_open_states();
    $table_name = $this->table_alias;
    // Fetch the field_sql_storage DB column name.
    $column_name = 'value';
    $rkey = $this->definition['is revision'] ? 'FIELD_LOAD_REVISION' : 'FIELD_LOAD_CURRENT';
    if (isset($this->field_info['storage']['details']['sql'][$rkey][$this->table][$column_name])) {
      $column_name = $this->field_info['storage']['details']['sql'][$rkey][$this->table][$column_name];
    }

    // Construct the expressison to add to the query.
    $expression = "IF($table_name.$column_name IN (:open_issue_states), 1, 0)";
    $group_params = array(
      'placeholders' => array(
        ':open_issue_states' => $open_states,
      ),
    );

    // Add grouping, if configured.
    if ($this->options['group_type'] != 'group') {
      $group_params['function'] = $this->options['group_type'];
    }

    // Construct a unique alias, and add the expression to the query.
    $value_expression_alias = $this->definition['table'] . '_' . $entity_type . '_value_expression';
    $this->aliases['value_expression'] = $this->query->add_field(NULL, $expression, $value_expression_alias, $group_params);


    // Add our additional fields to the query.
    $this->add_additional_fields($fields);

    // Filter by language, if field translation is enabled.
    $field = $this->field_info;
    if (field_is_translatable($entity_type, $field) && !empty($this->view->display_handler->options['field_language_add_to_query'])) {
      $column = $this->table_alias . '.language';
      // By the same reason as field_language the field might be LANGUAGE_NONE in reality so allow it as well.
      // @see this::field_language()
      global $language_content;
      $default_language = language_default('language');
      $language = str_replace(array('***CURRENT_LANGUAGE***', '***DEFAULT_LANGUAGE***'),
        array($language_content->language, $default_language),
        $this->view->display_handler->options['field_language']);
      $placeholder = $this->placeholder();
      $language_fallback_candidates = array($language);
      if (variable_get('locale_field_language_fallback', TRUE)) {
        require_once DRUPAL_ROOT . '/includes/language.inc';
        $language_fallback_candidates = array_merge($language_fallback_candidates, language_fallback_get_candidates());
      }
      else {
        $language_fallback_candidates[] = LANGUAGE_NONE;
      }
      $this->query->add_where_expression(0, "$column IN($placeholder) OR $column IS NULL", array($placeholder => $language_fallback_candidates));
    }

    // The revision id inhibits grouping.
    // So, stop here if we're using grouping, or if aren't adding all columns to
    // the query.
    if ($use_groupby || empty($this->definition['add fields to query'])) {
      return;
    }

    $this->add_additional_fields(array('revision_id'));
  }

  function advanced_render($values) {
    // Just fetch the $raw_items from the $values collection.
    $raw_items = array(
      array(
        'rendered' => check_plain($values->{$this->aliases['value_expression']}),
      ),
    );
    // If there are no items, set the original value to NULL.
    if (empty($raw_items)) {
      $this->original_value = NULL;
    }

    if ($this->allow_advanced_render()) {
      $tokens = NULL;
      if (method_exists($this, 'render_item')) {
        $items = array();
        foreach ($raw_items as $count => $item) {
          $value = $this->render_item($count, $item);
          if (is_array($value)) {
            $value = drupal_render($value);
          }
          $this->last_render = $value;
          $this->original_value = $this->last_render;

          $alter = $item + $this->options['alter'];
          $alter['phase'] = VIEWS_HANDLER_RENDER_TEXT_PHASE_SINGLE_ITEM;
          $items[] = $this->render_text($alter);
        }

        $value = $this->render_items($items);
      }
      else {
        $alter = array('phase' => VIEWS_HANDLER_RENDER_TEXT_PHASE_COMPLETELY) + $this->options['alter'];
        $value = $this->render_text($alter);
      }

      if (is_array($value)) {
        $value = drupal_render($value);
      }
      // This happens here so that render_as_link can get the unaltered value of
      // this field as a token rather than the altered value.
      $this->last_render = $value;
    }

    if (empty($this->last_render)) {
      if ($this->is_value_empty($this->last_render, $this->options['empty_zero'], FALSE)) {
        $alter = $this->options['alter'];
        $alter['alter_text'] = 1;
        $alter['text'] = $this->options['empty'];
        $alter['phase'] = VIEWS_HANDLER_RENDER_TEXT_PHASE_EMPTY;
        $this->last_render = $this->render_text($alter);
      }
    }

    return $this->last_render;
  }


}