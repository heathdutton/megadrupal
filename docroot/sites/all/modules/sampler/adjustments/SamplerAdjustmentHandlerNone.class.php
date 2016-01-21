<?php


/**
 * @file
 * Handler class for the none adjustment plugin.
 */

class SamplerAdjustmentHandlerNone implements SamplerAdjustmentHandlerInterface {

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
    return array();
  }

  public function adjustSampleSet($sample_set) {
    // Stub function, required in the interface.
    return $sample_set;
  }

  public function adjustSampleResults($samples) {
    // Stub function, required in the interface.
    return $samples;
  }
}
