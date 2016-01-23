<?php

/**
 * @file: ga.inc
 *
 * A helper class for the basic genetic algorithm class, strings genes together
 * into a chromosome.
 */

/**
 * Load dependencies.
 */
require_once(drupal_get_path('module', 'dail') . "/dail.inc");

class chromosome extends dail {

  /**
   * By default, our chormoses are simply made up of 1's and 0's.
   */
  private $alphabet = array('0', '1');

  private $min_length = 5;
  private $max_length = 50;

  public $chromosome = array();

  /**
   */
  function __construct($options = array()) {
    parent::__construct();
    $this->type .= '.chromosome';

    if (isset($options['alphabet']) && is_array($options['alphabet'])) {
      $this->alphabet = $options['alphabet'];
    }
    if (isset($options['min_length']) && is_int($options['min_length'])) {
      $this->min_length = $options['min_length'];
    }
    if (isset($options['max_length']) && is_int($options['max_length'])) {
      $this->max_length = $options['max_length'];
    }
    if (!isset($options['nocreate'])) {
      $this->create();
    }
  }

  private function create() {
    for ($i = 0; $i < $this->max_length; $i++) {
      $this->chromosome[] = $this->gene();
      if ($i >= $this->min_length) {
        if (!mt_rand(0, 4)) {
          break;
        }
      }
    }
  }

  function alphabet() {
    return $this->alphabet;
  }

  function fitness($fitness = NULL) {
    if ($fitness != NULL) {
      $this->fitness = $fitness;
    }
    return $this->fitness;
  }

  /**
   * Kill off $this chromosome.
   */
  function cull() {
    $this->fitness = NULL;
    //drupal_set_message('Culled: ' . implode('', $this->chromosome));
  }

  function gene() {
    $max = count($this->alphabet) - 1;
    $gene = mt_rand(0, $max);
    return $this->alphabet[$gene];
  }
}

function chromosome_dail_class() {
  return (object)array(
    'name' => 'chromosome',
    'class' => t('Chromosome'),
    'description' => t('Creates genes to build a chromosome.'),
  );
}
