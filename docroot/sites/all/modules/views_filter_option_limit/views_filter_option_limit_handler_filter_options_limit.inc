<?php

/**
 * Filter handler for entityreference fields with option limiting.
 *
 * This handler is declared to Views data by our entityreference behaviour
 * plugin, ViewsFilterOptionLimitBehaviorHandler.
 *
 * Any filters or arguments on the view that are on the relationship for this
 * filter's underlying entityreference field may be selected in the admin
 * options as 'limiting handlers'. The value of these limiting filters and
 * argument handlers will then restrict the options in this filter.
 *
 * For example, if this filter is on the 'Cities' reference field, and each
 * city entity has a 'Country' field, you can add a filter on 'Country', and
 * this will cause the 'Cities' filter to only show options for the cities in
 * the selected countries.
 *
 * When the filter's exposed form is rendered, values are taken from the
 * matching handlers and used to build the Entity Field Query to get the
 * options for the exposed form.
 */
class views_filter_option_limit_handler_filter_options_limit extends views_handler_filter_in_operator {

  function option_definition() {
    $options = parent::option_definition();

    $options['options_limit_filters'] = array('default' => array());
    $options['options_limit_arguments'] = array('default' => array());
    $options['no_limiting_values'] = array('default' => 'all');

    return $options;
  }

  /**
   * If a handler has 'extra options' it will get a little settings widget and
   * another form called extra_options.
   */
  function has_extra_options() {
    return TRUE;
  }

  /**
   * Form for additional settings.
   */
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $table_data = views_fetch_data($this->definition['table']);
    $target_entity_base_table = $table_data[$this->definition['field']]['relationship']['base'];
    //dsm($target_entity_base_table);

    // The name of the relationship should match our field name.
    // (TODO: it probably won't if we are ourselves on a relationship!)
    $relationship = $this->definition['field'];

    // Filters are already loaded on the display handler, because we're a filter.
    $filters = $this->view->display_handler->handlers['filter'];
    // We have to go fetch arguments.
    $arguments = $this->view->display_handler->get_handlers('argument');

    // Remove ourselves from the list of potential filters.
    unset($filters[$this->options['id']]);

    //dsm($filters);
    //dsm($filters['promote']);
    //dsm($arguments);

    $filter_options = array();
    $argument_options = array();

    foreach ($filters as $key => $filter_handler) {
      // The relationship for a handler is only in the options at this stage;
      // it's not actually set up.
      if ($filter_handler->options['relationship'] == $relationship) {
        // Gah what is this nonsense with labels???
        if (isset($filter_handler->definition['title short'])) {
          $label = $filter_handler->definition['title short'];
        }
        else {
          $label = $filter_handler->definition['label'];
        }
        $filter_options[$key] = $label;
      }
    }
    foreach ($arguments as $key => $argument_handler) {
      // The relationship for a handler is only in the options at this stage;
      // it's not actually set up.
      if ($argument_handler->options['relationship'] == $relationship) {
        $argument_options[$key] = $argument_handler->definition['title short'];
      }
    }

    // Show only a message if there are no suitable handlers to select from.
    if (empty($filter_options) && empty($argument_options)) {
      $form['message'] = array(
        '#markup' => t("No filters available. Add the relationship that matches this filter's field, and then either filters or contextual filters on that relationship."),
      );
      return;
    }

    if ($filter_options) {
      $form['options_limit_filters'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Matching filters'),
        '#options' => $filter_options,
        '#description' => t('Select the filters whose values should limit the available values of this filter.'),
        '#default_value' => $this->options['options_limit_filters'],
      );
    }

    if ($argument_options) {
      $form['options_limit_arguments'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Matching contextual filters'),
        '#options' => $argument_options,
        '#description' => t('Select the contextual filters whose values should limit the available values of this filter.'),
        '#default_value' => $this->options['options_limit_arguments'],
      );
    }

    $form['no_limiting_values'] = array(
      '#type' => 'radios',
      '#title' => t("Behavior when no limiting values are present"),
      '#description' => t('Determine what this filter should show for its options when none of the filters or arguments selected above have any values to limit by.'),
      '#options' => array(
        'all' => t('Show all the values for this filter. Warning: for a large result set this could impact performance.'),
        'none' => t('Show no values for this filter.'),
      ),
      '#default_value' => $this->options['no_limiting_values'],
    );
  }

  /**
   * Get the options for this filter.
   *
   * @return
   *   Return the stored values in $this->value_options if someone expects it.
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    // Get the field this filter is for.
    $this->field = field_info_field($this->definition['field_name']);
    // accept_exposed_input() has not yet been run on the handler at this
    // stage, so we need to get it from the view. Furthermore, we can't
    // call accept_exposed_input() ourselves as various other things it
    // expects haven't happened yet.
    $exposed_input = $this->view->get_exposed_input();

    //dsm($this);
    //dsm($form_state);
    //dsm($exposed_input, 'exposed input');

    // We build an EntityFieldQuery, and give it the entity type and bundle
    // conditions based on this filter handler's source field: this gets us the
    // options that are valid for the field. We then apply further conditions
    // to the query based on the limiting filter and argument handlers to
    // restrict the options.
    // TODO: An alternative approach here would be to programmatically build a
    // new view on the base table this filter handler is on, and then add
    // handlers that match the limiting filters and arguments in our options.
    // This would be slower, and may run into issues to do with building a view
    // while another one is being built, but would have the advantage that we
    // could allow limiting filters and arguments to be on relationships that
    // are themselves on this filter handler's relationship. For example, if
    // we're limiting a filter for a node reference, we could use a filter that
    // is on the node author user to restrict the options.
    $query = new ViewsFilterOptionLimitEntityFieldQuery();

    $table_data = views_fetch_data($this->definition['table']);
    $target_entity_base_table = $table_data[$this->definition['field']]['relationship']['base'];
    //dsm($target_entity_base_table);

    $target_entity_type = $this->field['settings']['target_type'];
    $entity_info = entity_get_info($target_entity_type);

    // Add a tag to our query.
    $query->addTag('views_filter_option_limit');

    // Set the target entity type and bundles on the EFQ.
    $query->entityCondition('entity_type', $target_entity_type);
    if (!empty($this->field['settings']['handler_settings']['target_bundles'])) {
      $query->entityCondition('bundle', $this->field['settings']['handler_settings']['target_bundles'], 'IN');
    }

    // Add a dummy propertyCondition, in order to force the generated
    // SelectQuery to have the entity base table.
    $query->propertyCondition($entity_info['entity keys']['id'], -1, '!=');

    // Keep track of how many conditions we add to the EFQ.
    $condition_count = 0;

    // Assemble the handlers we need to consider from the options.
    $limiting_handlers = array();
    foreach ($this->options['options_limit_filters'] as $handler_id) {
      $limiting_handlers[] = array('filter', $handler_id);
    }
    foreach ($this->options['options_limit_arguments'] as $handler_id) {
      $limiting_handlers[] = array('argument', $handler_id);
    }

    // Filter handlers first.
    foreach ($limiting_handlers as $handler_info) {
      // We can't do this in the foreach until PHP 5.5 :(
      list($handler_type, $handler_id) = $handler_info;

      $handler = $this->view->display_handler->get_handler($handler_type, $handler_id);

      // First figure out if this handler has a filtering value that is of
      // interest to us.
      if ($handler_type == 'filter') {
        // Case 1: Filter handler.
        if ($handler->options['exposed']) {
          // Case 1A: Exposed filter handler.
          $exposed_identifier = $handler->options['expose']['identifier'];
          if (!isset($exposed_input[$exposed_identifier])) {
            // If there's nothing for this filter in the exposed filter input,
            // then skip it.
            //dsm('skip nothing in this handler');
            // TODO: consider default values in an exposed filter with no value.
            continue;
          }

          // This ends up as a nested array if the filter takes multiple values
          // but nothing further one seems to be bothered by it!
          $handler_value = $exposed_input[$handler->options['expose']['identifier']];

          // If the exposed filter does not have an actual value set, skip it.
          if ($handler_value == 'All') {
            continue;
          }
        }
        else {
          // Case 1B: Non-exposed filter handler.
          // Grab the value from $handler->value.
          $handler_value = $handler->value;

          // This handler has nothing to say and does not participate.
          if (empty($handler_value)) {
            continue;
          }
        }
        //dsm($handler_value, '$handler_value');
      }
      else {
        // Case 2: Argument handler.
        // Trapdoor which covers the case when we're in the admin UI preview.
        // The view has no args on it at all.
        if (!isset($this->view->argument)) {
          continue;
        }

        // The argument handler doesn't have a value set on it by the view yet,
        // but we can ask it to pretend to process itself to get it.
        $handler_value = $handler->get_value();

        if (!empty($handler->options['break_phrase'])) {
          // This weirdly works on the handler itself, which I hope doesn't
          // have any undesirable effects on the argument handler...
          views_break_phrase($handler_value, $handler);

          $handler_value = $handler->value;

          // TODO: handle the operator this sets on the handler.
        }

        //dsm($handler_value);

        // This handler has nothing to say and does not participate.
        if (empty($handler_value)) {
          continue;
        }
      }

      // If we're still here, then we have a value to work with: we should
      // add it to the EFQ.
      $condition_count++;

      // Handle multiple values.
      $operator = is_array($handler_value) ? 'IN' : '=';

      // Figure out if this is a filter on a FieldAPI field, or just a table
      // field.
      if (isset($handler->definition['field_name'])) {
        // It's a field.
        $type = 'field';
        $query_property_name = $handler->definition['field_name'];

        // For the EFQ fieldCondition() we need the $column, which is the actual
        // database table column with the field name removed.
        // Just trim the length of the field name from the start of the column
        // name, adding 1 for the connecting underscore.
        $query_column_name = substr($handler->definition['field'], strlen($handler->definition['field_name']) + 1);

        $query->fieldCondition($query_property_name, $query_column_name, $handler_value, $operator);
      }
      else {
        // It's a table field, aka an entity property.
        $query_property_name = $handler->real_field;

        $query->propertyCondition($query_property_name, $handler_value, $operator);
      }
    } // Foreach handler.

    //dsm($query);

    $this->value_options = array();

    // If we have added no conditions to the query, then none of the limiting
    // handlers had any values to consider.
    if (empty($condition_count)) {
      // If we're configured to show no options in this case, return an empty
      // options array now.
      if ($this->options['no_limiting_values'] == 'none') {
        return $this->value_options;
      }
      // For the other case, just keep going: our EFQ will return all valid
      // entities for the filter's options.
    }

    // Add the sort option.
    // Straight copy from EntityReference_SelectionHandler_Generic::buildEntityFieldQuery()
    // which we can't call because we want our own class for the EFQ.
    if (!empty($this->field['settings']['handler_settings']['sort'])) {
      $sort_settings = $this->field['settings']['handler_settings']['sort'];
      if ($sort_settings['type'] == 'property') {
        $query->propertyOrderBy($sort_settings['property'], $sort_settings['direction']);
      }
      elseif ($sort_settings['type'] == 'field') {
        list($field, $column) = explode(':', $sort_settings['field'], 2);
        $query->fieldOrderBy($field, $column, $sort_settings['direction']);
      }
    }

    // Execute the query.
    $result = $query->execute();

    // Load the entities and build the array of options using their labels.
    if (!empty($result[$target_entity_type])) {
      if (isset($entity_info['entity keys']['label'])) {
        // If the entity type has a label key, then our slightly doctored
        // EntityFieldQuery class will have loaded the labels in the stub
        // entities. This saves us having to do an entity_load().
        $label_property = $entity_info['entity keys']['label'];
        foreach ($result[$target_entity_type] as $id => $stub_entity) {
          $this->value_options[$id] = $stub_entity->{$label_property};
        }
      }
      else {
        // We have to load all the entities to get the label from each one.
        $entities = entity_load($target_entity_type, array_keys($result[$target_entity_type]));
        foreach ($entities as $id => $entity) {
          $this->value_options[$id] = entity_label($target_entity_type, $entity);
        }
      }
    }

    return $this->value_options;
  }


  /**
   * Form for the filter value.
   */
  function value_form(&$form, &$form_state) {
    // Let the parent method act first.
    parent::value_form($form, $form_state);

    // When a limiting filter changes its value it may result in the exposed
    // filter values for this filter no longer being in the available options
    // which may now be restricted differently. In this case, we doctor the
    // input values to prevent a FormAPI validation error.
    $identifier = $this->options['expose']['identifier'];

    // Our parent has already called get_value_options(), so we can rely on
    // them being set.
    $force_default = FALSE;
    if (!empty($form_state['input'][$identifier])) {
      $input_value = $form_state['input'][$identifier];
      if ($input_value != 'All') {
        if (is_array($input_value)) {
          // The input value is an array: the filter is multi-valued.
          // Get the values that are common to the input values and the option
          // values. Just to complicate things, the input value array has values
          // while the value options array has keys.
          $intersection_options = array_intersect($input_value, array_keys($this->value_options));
          // Set the intersection into the input value. This may be empty.
          $form_state['input'][$identifier] = $intersection_options;
        }
        else {
          // Single-valued filter.
          if (!isset($this->value_options[$input_value])) {
            // Select the default 'Any' value.
            $form_state['input'][$identifier] = 'All';
          }
        }
      }
    }
  }

}

/**
 * Subclass of EntityFieldQuery which returns labels in the stub entities.
 *
 * This allows us to avoid an entity_load() of all the entities for our options,
 * when the entity type has a label entity key defined.
 */
class ViewsFilterOptionLimitEntityFieldQuery extends EntityFieldQuery {

  /**
   * Finishes the query.
   *
   * Adds tags, metaData, range and returns the requested list or count.
   *
   * @param SelectQuery $select_query
   *   A SelectQuery which has entity_type, entity_id, revision_id and bundle
   *   fields added.
   * @param $id_key
   *   Which field's values to use as the returned array keys.
   *
   * @return
   *   See EntityFieldQuery::execute().
   */
  function finishQuery($select_query, $id_key = 'entity_id') {
    // We know that an entity type condition is always set.
    $entity_type = $this->entityConditions['entity_type']['value'];
    $entity_info = entity_get_info($entity_type);

    // If the current entity type doesn't have a label key, just use the parent.
    if (!isset($entity_info['entity keys']['label'])) {
      return parent::finishQuery($select_query, $id_key);
    }

    // Add the label field to the query.
    // Get the alias for our entity type's base table.
    $tables = $select_query->getTables();
    $table_alias = $tables[$entity_info['base table']]['alias'];
    $select_query->addField($table_alias, $entity_info['entity keys']['label'], 'label');

    foreach ($this->tags as $tag) {
      $select_query->addTag($tag);
    }
    foreach ($this->metaData as $key => $object) {
      $select_query->addMetaData($key, $object);
    }
    $select_query->addMetaData('entity_field_query', $this);
    if ($this->range) {
      $select_query->range($this->range['start'], $this->range['length']);
    }
    if ($this->count) {
      return $select_query->countQuery()->execute()->fetchField();
    }
    $return = array();
    foreach ($select_query->execute() as $partial_entity) {
      $bundle = isset($partial_entity->bundle) ? $partial_entity->bundle : NULL;
      $entity = entity_create_stub_entity($partial_entity->entity_type, array($partial_entity->entity_id, $partial_entity->revision_id, $bundle));

      // Add the label to the stub entity.
      $entity->{$entity_info['entity keys']['label']} = $partial_entity->label;

      $return[$partial_entity->entity_type][$partial_entity->$id_key] = $entity;
    }
    return $return;
  }

}
