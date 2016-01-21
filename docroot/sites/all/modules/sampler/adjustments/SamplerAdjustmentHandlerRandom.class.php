<?php


/**
 * @file
 * Handler class for the random adjustment plugin.
 */

class SamplerAdjustmentHandlerRandom implements SamplerAdjustmentHandlerInterface {

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
    return array(
      'random_object_quantity' => 100,
    );
  }

  public function adjustSampleSet($samples) {
    // Stub function, required in the interface.
    return $samples;
  }

  public function adjustSampleResults($samples) {
    // Make sure this is a set of single value sample results.
    if (is_array($samples) && (current($samples) !== FALSE)) {
      $first_sample = current($samples);
      if (is_array($first_sample->values) && (current($first_sample->values) !== FALSE)) {
        $first_object_values = current($first_sample->values);
        if (count($first_object_values) == 1) {
          // We're going to repurpose the code in the random method plugin to
          // select random results from each of the computed sample values.  This
          // could admittedly be more efficient, but it's a small amount of code
          // that gets the job done.
          $random_method = $this->sampler->loadPlugin('method', 'random');
          foreach ($samples as $key => $sample) {
            // Load in the computed object_ids to the method plugin.
            $random_method->objectIDs = array_keys($sample->values);
            // Select a random set of object_ids.
            $random_method->selectRandomObjects();
            // Use the randomObjectIDs result to eliminate non-matching
            // object_ids in the existing sample values.
            $samples[$key]->values = array_intersect_key($samples[$key]->values, array_flip($random_method->randomObjectIDs));
          }
          $this->sampler->pluginOutput['adjustment_random'] = t("Reduced all samples to !objects_in_sample random objects maximum.", array('!objects_in_sample' => $this->sampler->options['random_object_quantity']));
        }
      }
    }

    return $samples;
  }
}
