<?php

/**
 * @file
 * Definition of postgis_views_handler_filter_geometry.
 */

/**
 * A handler to provide a filter for geospatial data.
 */
class postgis_views_handler_filter_geometry extends views_handler_filter {

  var $no_operator = TRUE;

  var $groups = array('Relationship Measurement', 'Relationship', 'Measurement');

  var $operations = array(
    'Azimuth'           => array('group' => 0, 'value' => TRUE, 'wkt' => TRUE), 
    'Distance'          => array('group' => 0, 'value' => TRUE, 'wkt' => TRUE), 
    'Distance_Sphere'   => array('group' => 0, 'value' => TRUE, 'wkt' => TRUE), 
    'HausdorffDistance' => array('group' => 0, 'value' => TRUE, 'wkt' => TRUE), 
    'MaxDistance'       => array('group' => 0, 'value' => TRUE, 'wkt' => TRUE),
    'Contains'          => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'ContainsProperly'  => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'CoveredBy'         => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Covers'            => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Crosses'           => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Disjoint'          => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Equals'            => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Intersects'        => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'OrderingEquals'    => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Overlaps'          => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Touches'           => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Within'            => array('group' => 1, 'value' => FALSE, 'wkt' => TRUE),
    'Area'              => array('group' => 2, 'value' => TRUE, 'wkt' => FALSE),
    'Length'            => array('group' => 2, 'value' => TRUE, 'wkt' => FALSE),
    'Perimeter'         => array('group' => 2, 'value' => TRUE, 'wkt' => FALSE),
  );

  var $comparisons = array(
    '<' => array('name' => 'less than', 'value' => TRUE),
    '>' => array('name' => 'greater than', 'value' => TRUE),
    '<=' => array('name' => 'less than or equal to', 'value' => TRUE),
    '>=' => array('name' => 'greater than or equal to', 'value' => TRUE),
    '=' => array('name' => 'equal', 'value' => TRUE),
    '<>' => array('name' => 'not equal', 'value' => TRUE),
    'asc' => array('name' => 'lowest', 'value' => FALSE), 
    'desc' => array('name' => 'highest', 'value' => FALSE)
  );

  function operations_list($type) {
    $list = array();
    if (in_array($type, array('value', 'wkt'))) {
      foreach ($this->operations as $key => $item) {
        if ($item[$type]) {
          $list[] = array('value' => $key);
        }
      }
    } else {
      foreach ($this->operations as $key => $item) {
        $group = t($this->groups[$item['group']]);
        if (!isset($list[$group])) {
          $list[$group] = array();
        }
        $list[$group][$key] = t($key); 
      }
    }
    return $list;
  }

  function comparision_list($type) {
    $list = array();
    if ($type === 'value') {
      foreach ($this->comparisons as $key => $item) {
        if ($item[$type]) {
          $list[] = array('value' => $key);
        }
      }
    } else {
      foreach ($this->comparisons as $key => $item) {
        $list[$key] = t($item['name']);
      }
    }
    return $list;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['operation'] = array('default' => '');
    $options['comparison'] = array('default' => '');
    $options['value'] = array('default' => '');
    $options['wkt'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['operation'] = array(
      '#type' => 'select',
      '#title' => t('Operation'),
      '#options' => $this->operations_list('options'),
      '#default_value' => $this->options['operation'] ? $this->options['operation'] : NULL,
      '#weight' => -2,
    );

    $form['comparison'] = array(
      '#type' => 'select',
      '#title' => t('Comparison Operator'),
      '#options' => $this->comparision_list('options'),
      '#default_value' => $this->options['comparison'] ? $this->options['comparison'] : NULL,
      '#weight' => -1,
      '#states' => array(
        'visible' => array(
          ':input[name="options[operation]"]' => $this->operations_list('value'),
        ),
      ),
    );
  }

  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#default_value' => $this->options['value'] ? $this->options['value'] : NULL,
      '#states' => array(
        'visible' => array(
          ':input[name="options[operation]"]' => $this->operations_list('value'),
          ':input[name="options[comparison]"]' => $this->comparision_list('value'),
        ),
      ),
    );

    $form['wkt'] = array(
      '#type' => 'textfield',
      '#title' => t('WKT'),
      '#default_value' => $this->options['wkt'] ? $this->options['wkt'] : NULL,
      '#states' => array(
        'visible' => array(
          ':input[name="options[operation]"]' => $this->operations_list('wkt'),
        ),
      ),
    );
  }

  function admin_summary() {
    $operation = $this->options['operation'];
    $comparison =$this->options['comparison'];
    $result = array($operation);

    if ($this->operations[$operation]['wkt']) {
      $result[] = $this->options['exposed'] ? t('exposed') : $this->options['wkt'];
    }

    if ($this->operations[$operation]['value']) {
      $result[] = $comparison;
    }

    if ($this->comparisons[$comparison]['value']) {
      $result[] = $this->options['exposed'] ? t('exposed') : $this->value;
    }
    return implode(' ', $result);
  }

  function query() {
    $operation = $this->options['operation'];
    $comparison =$this->options['comparison'];

    if (!in_array($operation, array_keys($this->operations)) ||
        !in_array($comparison, array_keys($this->comparisons))) {
      return;
    }

    $column = db_escape_table($this->definition['field_name'] . '_geometry');
    $args = array(':wkt' => $this->options['wkt'], ':srid' => $this->definition['srid']);
    $this->ensure_my_table();

    if ($this->operations[$operation]['value']) {
      if (in_array($comparison, array('asc', 'desc'))) {
        $snippet = "$this->table_alias.entity_id IN (select entity_id from (
          select entity_id, ST_$operation($column, ST_SetSRID(:wkt, :srid)) 
          from $this->table order by 2 $comparison limit 1) foo ) ";
      } else {
        $snippet = "ST_$operation($column, ST_SetSRID(:wkt, :srid)) $comparison :value";
        $args[':value'] = $this->options['value'];
      }
    } else {
      $snippet = "ST_$operation($column, ST_SetSRID(:wkt, :srid))";
      $args = array(':wkt' => $this->value, ':srid' => $this->definition['srid']);
    }

    $this->query->add_where_expression($this->options['group'], $snippet, $args);
  }
}
