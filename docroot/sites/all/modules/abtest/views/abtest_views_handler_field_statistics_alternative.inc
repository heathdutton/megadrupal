<?php

/**
 * @file
 * Field handler for statistics.
 */

/**
 * Field handler for pull info off the ABTestStatistics object.
 */
class abtest_views_handler_field_statistics_alternative extends views_handler_field {
  var $alternative_property = NULL;
  var $percentage = FALSE;

  function construct() {
    parent::construct();

    $this->additional_fields['test_id'] = array('table' => 'abtest_alternatives', 'field' => 'test_id');
    $this->additional_fields['alternative_id'] = array('table' => 'abtest_alternatives', 'field' => 'alternative_id');

    if (!empty($this->definition['alternative property'])) {
      $this->alternative_property = $this->definition['alternative property'];
    }
    if (!empty($this->definition['percentage'])) {
      $this->percentage = $this->definition['percentage'];
    }
  }

  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['test_id'];
  }

  function render($values) {
    $test_id = $values->{$this->aliases['test_id']};
    $alternative_id = $values->{$this->aliases['alternative_id']};

    $value = '0';
    $test = ABTest::getTest($test_id, NULL, array('create' => FALSE));
    if (!is_null($test)) {
      $statistics = $test->getStatistics();
      $alternative = $statistics->getAlternative($alternative_id);
      $property = $this->alternative_property;

      if (isset($alternative->$property)) {
        $value = round($alternative->$property, 2);
      }
    }

    if ($this->percentage) {
      $value .= '%';
    }

    return $value;
  }
}

