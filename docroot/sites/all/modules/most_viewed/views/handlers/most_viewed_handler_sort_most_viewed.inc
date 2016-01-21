<?php
/**
 * @file
 * Weekend date filter handler.
 */

class most_viewed_handler_sort_most_viewed extends views_handler_sort {

  function option_definition() {
    $options = parent::option_definition();

    $options['days'] = array('default' => '');
    return $options;
  }

  /**
   * Options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['days'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of days'),
      '#description' => t('How many days take to check most viewed content'),
      '#default_value' => $this->options['days'],
    );
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();
    $extra = array();

    if (!empty($this->options['days'])) {
      $timestamp = REQUEST_TIME - ($this->options['days'] * 24 * 60 * 60);
      $extra = "most_viewed_hits.created > $timestamp";
    }

    // join most viewed table.
    $join = new views_join();
    $join->construct('most_viewed_hits', $this->table, 'nid', 'entity_id', $extra, 'LEFT');
    $relation = $this->query->add_relationship('most_viewed_hits', $join, $this->table);


    // Add the field
    $this->query->add_orderby(NULL, "{$relation}.entity_id", $this->options['order'], 'most_viewed', array(
      'count' => TRUE,
    ));
  }
}
