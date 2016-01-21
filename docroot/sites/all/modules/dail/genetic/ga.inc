<?php

/**
 * @file: ga.inc
 *
 * A basic genetic algorithm class.
 */

/**
 * Load dependencies.
 */
require_once(drupal_get_path('module', 'dail') . "/dail.inc");
require_once(drupal_get_path('module', 'dail') . "/genetic/chromosome.inc");

class ga extends dail {

  /**
   */
  private $initial_population = 100;

  //private $minimum_population = 50;
  //private $maximum_population = 150;
  private $minimum_population = 10;
  private $maximum_population = 10;

  public $chromosomes = array();

  /**
   */
  function __construct($options = array()) {
    parent::__construct();
    $this->type .= '.ga';

    if (isset($options['initial_population']) && is_int($options['initial_population'])) {
      $this->initial_population = $options['initial_population'];
    }
    if (isset($options['fitness_callback']) && is_string($options['fitness_callback'])) {
      $this->fitness_callback = $options['fitness_callback'];
    }
  }

  function create($options = array()) {
    if (isset($options['alphabet']) && is_array($options['alphabet'])) {
      $this->alphabet = $options['alphabet'];
    }
    for ($i = 0; $i < $this->initial_population; $i++) {
      $this->chromosomes[] = new chromosome($options);
    }
  }

  function fitness_callback($function = FALSE) {
    if ($function !== FALSE && is_string($function)) {
      $this->fitness_callback = $function;
    }
    return $this->fitness_callback;
  }

  function fitness() {
    $chromosomes = $this->chromosomes;
    if (!empty($chromosomes)) {
      $callback = $this->fitness_callback;
      if (function_exists($callback)) {
        foreach ($chromosomes as $chromosome) {
          $callback($chromosome);
        }
      }
    }
    else {
      drupal_set_message(t('No chromomsomes found.'), 'error');
    }
  }

  function cull($options = array()) {
    if (isset($options['cull_callback']) && is_string($options['cull_callback'])) {
      $this->cull_callback = $options['cull_callback'];
    }
    if (isset($this->cull_callback) && function_exists($this->cull_callback)) {
      $callback = $this->cull_callback;
      $callback($this, $options);
    }
  }

  function evolve($options = array()) {
    // Sort chromosomes from most fit to least fit
    $this->sort_fitness($options);
    $this->children = array();
    $fitness = $this->fitness;
    // TODO: Make this configurable.
    $parents = sizeof($this->chromosomes) / 2;
    $total = 0;
    foreach ($fitness as $key => $chromosomes) {
      foreach ($chromosomes as $chromosome) {
        // TODO: Make this configurable.
        // 66% of the time we breed with this chromosome
        if (rand(0,2) && ($total++ <= $parents || !rand(0,2))) {
          $parent1 = $chromosome;
          $parent2 = $this->fit_parent();
          $children = $this->breed($parent1, $parent2, $options);
          foreach ($children as $child) {
            $this->children[] = $child;
          }
        }
        else {
          continue;
        }
      }
    }
    $this->chromosomes = NULL;
    foreach ($this->children as $key => $child) {
      $this->chromosomes[$key] = new chromosome(array('nocreate' => TRUE, 'alphabet' => $this->alphabet));
      $this->chromosomes[$key]->chromosome = $child;
      $this->mutate($this->chromosomes[$key]);
    }
    while (sizeof($this->chromosomes) < $this->minimum_population) {
      $this->chromosomes[] = new chromosome(array('alphabet' => $this->alphabet));
    }
    while (sizeof($this->chromosomes) > $this->maximum_population) {
      array_pop($this->chromosomes);
    }
  }

  function fit_parent($options = array()) {
    $fitness = $this->fitness;
    foreach ($fitness as $key => $chromosomes) {
      foreach ($chromosomes as $chromosome) {
        // TODO: make this configurable
        if (rand(0,1)) {
          return $chromosome;
        }
      }
    }
  }

  function sort_fitness($options = array()) {
    $this->fitness = array();
    foreach ($this->chromosomes as $key => $chromosome) {
      // TODO: Optionally enable/disable mutation.
      if (isset($chromosome->fitness)) {
        //$this->mutate($chromosome);
        $this->fitness[$chromosome->fitness][] = $chromosome;
      }
    }
    ksort($this->fitness);
    $best = array_shift($this->fitness);
    if (is_array($best)) {
      array_unshift($this->fitness, $best);
      if (!isset($this->best) || $best[0]->fitness < $this->best->fitness) {
        $this->best = $best[0];
      }
      $this->current = $best;
    }
  }

  function evolving($options = array()) {
    return TRUE;
    $current = isset($this->current[0]) ? $this->current[0] : NULL;
    $best = isset($this->best) ? $this->best : NULL;
    $this->loop = isset($this->loop) ? $this->loop + 1 : 1;
    if ($current == $best) {
      $this->found = isset($this->found) ? $this->found + 1 : 1;
    }
    else {
      $this->found = 0;
    }
    if ($this->found > 30 || $this->loop > 2500) {
      return FALSE;
    }
    return TRUE;
  }

  function breed($parent1, $parent2, $options = array()) {
    if (!is_object($parent1) || !is_object($parent2)) {
      return array();
    }
    $children = array();
    $max1 = count($parent1->chromosome);
    $max2 = count($parent2->chromosome);
    if ($max1 > $max2) {
      $max = $max1;
    }
    else {
      $max = $max2;
    }
    $crossover = rand(0, $max);
    for ($i = 0; $i < $max; $i++) {
      if ($i < $crossover) {
        if (isset($parent1->chromosome[$i])) {
          $child1[] = $parent1->chromosome[$i];
        }
        if (isset($parent2->chromosome[$i])) {
          $child2[] = $parent2->chromosome[$i];
        }
      }
      else {
        if (isset($parent2->chromosome[$i])) {
          $child1[] = $parent2->chromosome[$i];
        }
        if (isset($parent1->chromosome[$i])) {
          $child2[] = $parent1->chromosome[$i];
        }
      }
    }
    return array($child1, $child2);
  }

  function mutate($chromosome, $options = array()) {
    $offset = $this->mutate_offset($options);
    foreach ($chromosome->chromosome as $key => $gene) {
      if (!mt_rand(0, $offset)) {
        if (mt_rand(0, 3)) {
          $chromosome->chromosome[$key] = $chromosome->gene();
          // TODO: Optionally enable/disable this logging.
          $chromosome->mutated[] = array(
            'key' => $key,
            'old' => $gene,
            'new' => $chromosome->chromosome[$key],
          );
        }
        else if (mt_rand(0, 1)) {
          // delete gene
          unset($chromosome->chromosome[$key]);
          $chromosome->mutated[] = array(
            'key' => $key,
            'old' => $gene,
            'new' => '',
          );
        }
        else {
          // add new gene
          $index = max(array_flip($chromosome->chromosome)) + 1;
          $chromosome->chromosome[$index] = $chromosome->gene();
          $chromosome->mutated[] = array(
            'key' => $index,
            'old' => '',
            'new' => $chromosome->chromosome[$index],
          );
        }
        // TODO: Optionally display debug output.
        //drupal_set_message("mutation occured key($key) old($gene) new(" . $chromosome->chromosome[$key] .')');
      }
      // TODO: Optionally re-calculate fitness after a mutation.
    }
  }

  function mutate_offset($options = array()) {
    if (isset($options['mutate_offset']) && is_int($options['mutate_offset'])) {
      $this->mutate_offset = $options['mutate_offset'];
    }
    else {
      $this->mutate_offset = 75;
    }
    return $this->mutate_offset;
  }

}

function ga_dail_class() {
  return (object)array(
    'name' => 'ga',
    'class' => t('Genetic Algorithm'),
    'description' => t('A simple genetic algorithm class.'),
  );
}
