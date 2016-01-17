<?php
/**
 * @file
 * Relationship from an attached entity to a party.
 */

class party_handler_relationship_party_from_attached_entity extends views_handler_relationship {
  /**
   * Defines default values for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['data_set'] = array('default' => NULL);

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

    // Output the form element.
    $form['data_set'] = array(
      '#type' => 'select',
      '#title' => t('Data set'),
      '#options' => $options,
      '#default_value' => $this->options['data_set'],
      '#description' => t('Optionally filter by a specific data set.'),
      '#required' => FALSE,
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

    $this->ensure_my_table();

    // Join to the party attached entity table.
    $join_pae = new views_join();
    $join_pae->definition = array(
      'table' => 'party_attached_entity',
      'field' => 'eid',
      'left_table' => $this->table_alias,
      'left_field' => $entity_info['entity keys']['id'],
      'extra' => array(
        array(
          'field' => 'entity_type',
          'value' => $entity_type,
        ),
      ),
    );
    if ($this->options['data_set']) {
      $join_pae->definition['extra'][] = array(
        'field' => 'data_set',
        'value' => $this->options['data_set'],
      );
    }
    $join_pae->construct();
    $alias_pae = $this->query->add_relationship('pae_' . $this->relationship, $join_pae, 'party_attached_entity', $this->relationship);

    // Join to the party table.
    $join_party = new views_join();
    $join_party->definition = array(
      'table' => $this->definition['base'],
      'field' => $this->definition['base field'],
      'left_table' => $alias_pae,
      'left_field' => 'pid',
    );
    $join_party->construct();
    $join_party->adjusted = TRUE;
    $this->alias = $this->query->add_relationship('party_' . $this->relationship, $join_party, $this->definition['base'], $this->relationship);
  }
}
