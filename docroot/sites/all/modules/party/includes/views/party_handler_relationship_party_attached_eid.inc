<?php
/**
 * @file Relationship for party data sets, relating parties to the entities in a set.
 */

class party_handler_relationship_party_attached_eid extends views_handler_relationship {
  /**
   * Defines default values for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['data_set'] = array('default' => NULL);
    $options['delta'] = array('default' => 'all');

    return $options;
  }

  /**
   * Extends the relationship's basic options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Get all defined data sets.
    $sets = party_get_data_set_info();
    $options = array();
    foreach ($sets as $set_name => $set) {
      if ($set['entity type'] == $this->definition['entity_type']) {
        // @todo: sort these nicely in some way?
        $options[$set_name] = $set['label'];
      }
    }

    // @todo: this is radios for now as I'm not sure multiple data sets in a
    // relationship makes sense.
    // @todo: this is not needed if there is only one dataset for this entity
    // type BUT if the form element is removed, the handler MUST nonetheless
    // set its dataset otherwise the query will break!
    $form['data_set'] = array(
      '#type' => 'radios',
      '#title' => t('Data set'),
      '#options' => $options,
      '#default_value' => $this->options['data_set'],
      '#description' => t("The data set entities to create the relationship towards."),
      '#required' => TRUE,
    );

    $form['delta'] = array(
      '#type' => 'select',
      '#title' => t('Delta'),
      '#options' => array(
        'all' => t('All'),
        0 => '1',
        1 => '2',
        2 => '3',
        3 => '4',
      ),
      '#default_value' => $this->options['delta'],
      '#description' => t("Choose which attached entity to create the relationship to."),
    );
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Get essential settings and data.
    $data_set_name = $this->options['data_set'];
    $entity_type = $this->definition['entity_type'];
    $entity_info = entity_get_info($entity_type);

    // Add the table that we are defined on.
    // We don't use $this->ensure_my_table() because that calls
    // $query->ensure_table which only allows us to add a table once.
    // Instead, we may need to add the {party_attached_entity} table
    // multiple times, once for each copy of this relationship.
    // This is the same as the way filtering on different taxonomy terms needs
    // to join to taxonomy_term_data multiple times.
    $join_cpae = new views_join();
    $definition_cpae = array(
      'table' => 'party_attached_entity',
      'field' => 'pid',
      'left_table' => 'party',
      'left_field' => 'pid',
      'extra' => array(
        array(
          'field' => 'data_set',
          'value' => $data_set_name,
        ),
      ),
    );
    if ($this->options['delta'] != 'all') {
      $definition_cpae['extra'][] = array(
        'field' => 'delta',
        'value' => $this->options['delta'],
      );
    }
    $join_cpae->definition = $definition_cpae;
    $join_cpae->construct();

    // Make a unique alias for this table.
    // @todo: It would be nice to have either entity type or data set name in
    // this for debugging, but it will cause field aliases to get truncated!
    $alias_cpae = 'party_attached_entity_' . $this->position;

    $alias_cpae = $this->query->add_table('party_attached_entity', $this->relationship, $join_cpae, $alias_cpae);

    // Now add the relationship to the attached entity table.
    $def = array();
    // The relationship joins to the base table given data set's entity type.
    $def['table'] = $entity_info['base table'];
    $def['field'] = $entity_info['entity keys']['id'];
    $def['left_table'] = $alias_cpae;
    $def['left_field'] = 'eid';

    $alias = $def['table'] . '_' . $this->table;

    $join = new views_join();

    $join->definition = $def;
    $join->construct();
    $join->adjusted = TRUE;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}
