<?php
// $Id$
require_once(drupal_get_path('module', 'dail') . "/dail.inc");

class MarkovChain extends dail {

  public function __construct($context = 2){
    $this->context = $context - 1;
  }

  public function load(array $input, $reset = FALSE) {
    $table = array();
    $states = array();

    if (!is_array($input[0])) {
      $input = array($input);
    }

    foreach (range(0, $this->context) as $i) {
      $states[$i] = NULL;
    }

    foreach ($input as $item) {
      if ($reset) {
        foreach (range(0, $this->context) as $i) {
          $states[$i] = NULL;
        }
      }
      foreach ($item as $i) {
        $t = &$table;
        foreach ($states as $state) {
          $t = &$t[$state];
        }
        $t[] = $i;
        array_shift($states);
        $states[] = $i;
      }
    }
    $t = &$table;
    foreach ($states as $state) {
      $t = &$t[$state];
    }
    $t[] = NULL;
    $this->table = $table;
  }

  public function generate($size = NULL) {
    if (!isset($size)) {
      $size = rand($this->low, $this->high);
    }
    dpm($size);
    $table = $this->table;
    $states = array();
    $t = &$table;

    foreach (range(0, $this->context) as $i) {
      $key = array_rand($t, 1);
      $states[$i] = $key;
      $t = &$t[$key];
    }
    $out = array();

    foreach (range(0, $size - 1) as $c) {
      $t = &$table;
      foreach ($states as $state) {
        $t = &$t[$state];
      }
      $key = array_rand($t, 1);
      $new = $t[$key];

      if ($new === NULL) {
        return $out;
      }

      $out[] = $new;
      array_shift($states);
      $states[] = $new;
    }
    return $out;
  }

  function get_table() {
    return $this->table;
  }

  function set_table($table) {
    $this->table = $table;
  }

  function get_range() {
    return array($this->low, $this->high);
  }

  function set_range($low, $high) {
    $this->low = $low;
    $this->high = $high;
  }

  function load_from_view($view, $field) {
    if (is_string($view)) {
      $view = views_get_view($view);
    }

    $field_info = field_read_field($field);
    $und_key = key($field_info['columns']);

    //$field = 'field_data_' . $field . '_node_entity_type';
    $view->execute();
    $items = array();
    foreach ($view->result as $result) {
      $node = node_load($result->nid);
      if (isset($node->{$field}['und'])) {
        $item = array();
        foreach ($node->{$field}['und'] as $value) {
          $item[] = $value[$und_key];
        }
        $items[] = $item;
      }
    }
    $this->load($items);
  }
}
