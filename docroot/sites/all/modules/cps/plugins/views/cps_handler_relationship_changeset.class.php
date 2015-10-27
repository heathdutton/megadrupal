<?php

/**
 * @file
 * Definition of cps_handler_relationship_changeset.
 */

/**
 * Relationship handler to relate changeset to entity with specific states.
 *
 * @ingroup views_relationship_handlers
 */
class cps_handler_relationship_changeset extends views_handler_relationship  {

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'type');
    $options['state_type'] = array('default' => array());
    $options['states'] = array('default' => array());
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Filter type'),
      '#options' => array(
        'type' => t('State type'),
        'states' => t('States')
      ),
      '#default_value' => $this->options['type'],
    );

    $states = cps_changeset_get_state_labels();
    $form['states'] = array(
      '#type' => 'checkboxes',
      '#title' => t('States'),
      '#options' => $states,
      '#default_value' => $this->options['states'],
      '#description' => t('Choose which states you wish to relate. Only entity revisions in changesets in the given states will be related.'),
      '#states' => array(
        'visible' => array(
          ':input[name="options[type]"]' => array('value' => 'states'),
        ),
      ),
    );

    $types = cps_changeset_get_state_types();
    $form['state_type'] = array(
      '#type' => 'radios',
      '#title' => t('State types'),
      '#options' => $types,
      '#default_value' => $this->options['state_type'],
      '#description' => t('Choose which state type you wish to relate. Only entity revisions in changesets in the given states will be related.'),
      '#states' => array(
        'visible' => array(
          ':input[name="options[type]"]' => array('value' => 'type'),
        ),
      ),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  function query() {
    if ($this->options['type'] == 'states') {
      $states = array_filter($this->options['states']);
    }
    else {
      $all_states = cps_changeset_get_states();
      $states = array();
      foreach ($all_states as $state => $info) {
        if ($info['type'] == $this->options['state_type']) {
          $states[] = $state;
        }
      }
    }

    $this->ensure_my_table();
    $entity_type = $this->definition['entity type'];
    $entity_info = entity_get_info($entity_type);

    if (!empty($states)) {
      // Add the cps_entity + cps_changeset subselect for the entity.
      // This subselect is required because a LEFT JOIN requires the filter
      // to be part of the join and but since what we're filtering on is
      // 2 tables away from our base, we need a subselect to collapse it
      // into 1 table. If we don't, then we can't LEFT join, which is desired
      // to be able to show entities that are not in a changeset that matches
      // the filter.

      $cps_def = array(
        // Table isn't needed for the query, but it's needed by
        // views_join::__construct() otherwise we get a notice.
        'table' => $entity_info['base table'],
        'field' => 'entity_id',
        'left_table' => $this->table_alias,
        'left_field' => $entity_info['entity keys']['id'],
        'type' => $this->options['required'] ? 'INNER' : 'LEFT',
      );

      $query = db_select('cps_entity', 'e');
      $query->addJoin('INNER', 'cps_changeset', 'c', 'c.changeset_id = e.changeset_id');
      $query->condition('c.status', $states);
      $query->condition('e.entity_type', $entity_type);
      $query->fields('e', array('entity_id', 'revision_id', 'changeset_id'));
      $cps_def['table formula'] = $query;

      $cps_join = new views_join();
      $cps_join->definition = $cps_def;
      $cps_join->construct();
      $cps_join->adjusted = TRUE;

      $this->cps_alias = $this->query->add_table('cps_entity_select', $this->relationship, $cps_join);
    }
    else {
      // Just join the table without all the subselect stuff.
      // This just adds the cps_entity table directly as an intermediary.
      $cps_def = array(
        'table' => 'cps_entity',
        'field' => 'entity_id',
        'left_table' => $this->table_alias,
        'left_field' => $entity_info['entity keys']['id'],
        'type' => $this->options['required'] ? 'INNER' : 'LEFT',
        'extra' => array(
          array(
            'field' => 'entity_type',
            'value' => $entity_type,
          ),
        ),
      );

      $cps_join = new views_join();
      $cps_join->definition = $cps_def;
      $cps_join->construct();
      $cps_join->adjusted = TRUE;

      $this->cps_alias = $this->query->add_table('cps_entity', $this->relationship, $cps_join);
    }

    // Now add our changeset table on the previous relationship.
    $alias = 'cps_entity_changeset';

    $changeset_def = array(
      'table' => 'cps_changeset',
      'field' => 'changeset_id',
      'left_table' => $this->cps_alias,
      'left_field' => 'changeset_id',
      'type' => $this->options['required'] ? 'INNER' : 'LEFT',
    );

    $changeset_join = new views_join();
    $changeset_join->definition = $changeset_def;
    $changeset_join->construct();
    $changeset_join->adjusted = TRUE;

    $this->alias = $this->query->add_relationship($alias, $changeset_join, 'cps_changeset', $this->relationship);
  }
}
