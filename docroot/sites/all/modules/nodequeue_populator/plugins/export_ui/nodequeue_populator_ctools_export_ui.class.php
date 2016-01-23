<?php

/**
 * @file
 * Export-ui handler for the nodequeue populator module.
 */

class nodequeue_populator_ctools_export_ui extends ctools_export_ui {
  /**
   * Add group sorting.
   */
  function list_sort_options() {
    $options = parent::list_sort_options();
    $options += array(
      'populator_group' => t('Group'),
      'nodequeue' => t('Nodequeue'),
    );
    return $options;
  }

  /**
   * Add group searching.
   */
  function list_search_fields() {
    $fields = parent::list_search_fields();
    $fields[] = 'populator_group';
    return $fields;
  }

  /**
   * Add group header.
   */
  function list_table_header() {
    $header = parent::list_table_header();
    $ops = array_pop($header);
    $storage = array_pop($header);
    $header[] = array('data' => t('Group'), 'class' => array('ctools-export-ui-populator-group'));
    $header[] = array('data' => t('Nodequeue'), 'class' => array('ctools-export-ui-nodequeue'));
    $header[] = $storage;
    $header[] = $ops;
    return $header;
  }

  /**
   * Add group filtering.
   */
  function list_form(&$form, &$form_state) {
    parent::list_form($form, $form_state);
    $all = array('all' => t('- All -'));
    $populators = ctools_export_crud_load_all('nodequeue_populator');
    $groups = array();
    foreach ($populators as $populator) {
      $groups[$populator->populator_group] = $populator->populator_group;
    }

    $form['top row']['populator_group'] = array(
      '#type' => 'select',
      '#title' => t('Group'),
      '#options' => $all + $groups,
      '#default_value' => 'all',
    );
    $form['top row']['search']['#weight'] = 100;
  }

  /**
   * Handle group filtering.
   */
  function list_filter($form_state, $item) {
    $schema = ctools_export_get_schema($this->plugin['schema']);
    if ($form_state['values']['storage'] != 'all' && $form_state['values']['storage'] != $item->{$schema['export']['export type string']}) {
      return TRUE;
    }

    if ($form_state['values']['disabled'] != 'all' && $form_state['values']['disabled'] != !empty($item->disabled)) {
      return TRUE;
    }

    if ($form_state['values']['populator_group'] != 'all' && $form_state['values']['populator_group'] != $item->populator_group) {
      return TRUE;
    }

    if ($form_state['values']['search']) {
      $search = strtolower($form_state['values']['search']);
      foreach ($this->list_search_fields() as $field) {
        if (strpos(strtolower($item->$field), $search) !== FALSE) {
          $hit = TRUE;
          break;
        }
      }
      if (empty($hit)) {
        $subqueue_value = t('Invalid');
        if ($item->sqid && ($subqueue = subqueue_load($item->sqid))) {
          $subqueue_value = $subqueue->title . ' [' . $subqueue->sqid . ']';
        }
        if (strpos(strtolower($subqueue_value), $search) === FALSE) {
          return TRUE;
        }
      }
    }
  }

  /**
   * Add group to each row.
   */
  function list_build_row($item, &$form_state, $operations) {
    parent::list_build_row($item, $form_state, $operations);

    // Set up sorting.
    $name = $item->{$this->plugin['export']['key']};

    $subqueue_link = t('Invalid');
    if (!empty($item->sqid) && ($subqueue = subqueue_load($item->sqid))) {
      $subqueue_link = l(check_plain($subqueue->title . ' [' . $subqueue->sqid . ']'), "admin/structure/nodequeue/$subqueue->qid/view/$subqueue->sqid");
    }

    // Note: $item->{$schema['export']['export type string']} should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'populator_group':
        $this->sorts[$name] = $item->populator_group;
        break;

      case 'nodequeue':
        $this->sorts[$name] = $subqueue_value;
        break;

    }

    $ops = array_pop($this->rows[$name]['data']);
    $storage = array_pop($this->rows[$name]['data']);
    $this->rows[$name]['data'][] = array('data' => check_plain($item->populator_group), 'class' => array('ctools-export-ui-populator-group'));
    $this->rows[$name]['data'][] = array('data' => $subqueue_link, 'class' => array('ctools-export-ui-nodequeue'));
    $this->rows[$name]['data'][] = $storage;
    $this->rows[$name]['data'][] = $ops;
  }
}
