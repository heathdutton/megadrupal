<?php

/**
 * @file
 * Field handler for statistics.
 */

/**
 * Field handler for pull info off the ABTestStatistics object.
 */
class abtest_views_handler_field_statistics extends views_handler_field {
  var $statistics_function = NULL;

  function construct() {
    parent::construct();

    $this->additional_fields['test_name'] = array('table' => 'abtest', 'field' => 'test_name');

    if (!empty($this->definition['statistics function'])) {
      $this->statistics_function = $this->definition['statistics function'];
    }
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['test_name'];
  }

  function render($values) {
    $test_name = $values->{$this->field_alias};
    $test = ABTest::getTest($test_name, NULL, array('create' => FALSE));
    $statistics = $test->getStatistics();
    $function = $this->statistics_function;

    if (method_exists($statistics, $function)) {
      return round($statistics->$function(), 2);
    }
  }
}

