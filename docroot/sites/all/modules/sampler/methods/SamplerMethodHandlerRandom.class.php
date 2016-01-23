<?php

/**
 * @file
 * Handler class for the random method plugin.
 */

class SamplerMethodHandlerRandom implements SamplerMethodHandlerInterface {

  public $objectIDs = array();
  private $totalObjects = 0;
  private $randomObjectKeys = array();
  private $randomObjectIDs = array();
  private $objectsInSample = 0;
  public $sampler;

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function __get($variable) {
    return $this->$variable;
  }

  public function __set($variable, $value) {
    // Stub function -- we're not allowing any private variables to be set.
  }

  public function options() {
  // Add periodic defaults to the global options.
    return array(
      'random_object_quantity' => 100,
    );
  }

  public function buildSampleSet() {

    $samples = array();
    $this->getMetricObjectIDs();
    if (!empty($this->objectIDs)) {
      $this->selectRandomObjects();

      // Put the random objects in as an option for the sample.
      $this->sampler->options['object_ids'] = $this->randomObjectIDs;

      // Only one sample is taken, and the API will provide the neccesary
      // defaults.
      $samples[] = new stdClass();

      $this->sampler->pluginOutput['method_random'] = t("Sampled !objects_in_sample objects from !total_objects total objects.", array('!objects_in_sample' => $this->objectsInSample, '!total_objects' => $this->totalObjects));
    }
    return $samples;
  }

  /**
   * Retrieve all object IDs that the metric wants to track.
   */
  public function getMetricObjectIDs() {
    $this->objectIDs = $this->sampler->trackObjectIDs();
  }

  /**
   * Chooses a random set of object IDs.
   *
   * @return
   *   An array of object IDs.
   */
  public function selectRandomObjects() {

    $this->totalObjects = count($this->objectIDs);

    // Can't really pick random quantity unless the total number of objects is
    // bigger than the quantity.
    if ($this->totalObjects > $this->sampler->options['random_object_quantity']) {
      $this->randomObjectKeys = array_rand($this->objectIDs, $this->sampler->options['random_object_quantity']);
      // array_rand() returns a single key sometimes, so catch this and wrap it
      // in an array for consistency.
      $this->randomObjectKeys = is_array($this->randomObjectKeys) ? $this->randomObjectKeys : array($this->randomObjectKeys);

      // Build the randomObjectIDs array from the returned random keys.
      $this->randomObjectIDs = array_intersect_key($this->objectIDs, array_flip($this->randomObjectKeys));
      $this->objectsInSample = $this->sampler->options['random_object_quantity'];
    }
    // Not enough objects to pick a random number, so just use them all.
    else {
      $this->randomObjectIDs = $this->objectIDs;
      $this->objectsInSample = $this->totalObjects;
    }
  }
}
