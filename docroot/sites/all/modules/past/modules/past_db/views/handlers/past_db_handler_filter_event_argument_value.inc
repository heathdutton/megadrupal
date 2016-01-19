<?php
/**
 * @file
 * Contains a views filter handler.
 */

/**
 * Field handler which shows the event's argument key value.
 *
 * @ingroup views_field_handlers
 */
class past_db_handler_filter_event_argument_value extends views_handler_filter_string {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $definition = parent::option_definition();
    $definition['argument_name'] = array('default' => NULL);
    $definition['data_key'] = array('default' => NULL);
    return $definition;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['argument_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Argument nane'),
      '#default_value' => $this->options['argument_name'],
    );

    $form['data_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Data key'),
      '#default_value' => $this->options['data_key'],
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    $this->ensure_my_table();

    // Add the join for the argument table using a new relationship.
    $join = new views_join();
    $join->definition = array(
      'field' => 'event_id',
      'left_field' => 'event_id',
      'table' => 'past_event_argument',
      'left_table' => 'past_event',
      'type' => 'INNER',
    );
    $join->construct();
    $relationship_alias = $this->query->add_relationship($this->options['id'], $join, 'past_event');

    // Limit to a specific argument, if configured.
    if ($this->options['argument_name']) {
      $this->query->add_where($this->options['group'], $relationship_alias . '.name', $this->options['argument_name'], '=');
    }

    // Join the data table using the specified relationship.
    $join = new views_join();
    $join->definition = array(
      'field' => 'argument_id',
      'left_field' => 'argument_id',
      'table' => 'past_event_data',
      'left_table' => 'past_event_argument',
      'type' => 'INNER',
    );
    $join->construct();
    $this->table_alias = $this->query->add_table('past_event', $relationship_alias, $join);

    // Limit to a specific data key, if configured.
    if ($this->options['data_key']) {
      $this->query->add_where($this->options['group'], $relationship_alias . '.name', $this->options['data_key'], '=');
    }
    parent::query();
  }

}
