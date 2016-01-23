<?php


/**
 * @file
 * Handler class for the vary_periodic_timestamp adjustment plugin.
 */

class SamplerAdjustmentHandlerVaryPeriodicTimestamp implements SamplerAdjustmentHandlerInterface {

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
    // Add periodic defaults to the global options.
    return array(
      'sample_timestamp_variance' => 3600,
    );
  }

  public function adjustSampleSet($samples) {
    // Only make this adjustment if the periodic sampling method is being used.
    if ($this->sampler->methodPlugin == 'periodic') {
      if (!empty($samples) && is_array($samples)) {
        // Set up minimum and maximum variance seconds.
        $min = 0 - $this->sampler->options['sample_timestamp_variance'];
        $max = $this->sampler->options['sample_timestamp_variance'];
        foreach ($samples as $key => $sample) {
          // Choose and apply the variance.
          $variance = rand($min, $max);
          $samples[$key]->timestamp = $samples[$key]->timestamp + $variance;
        }
      }
    }

    $this->sampler->pluginOutput['adjustment_vary_periodic_timestamp'] = t("Varied periodic sample times by plus/minus !sample_timestamp_variance seconds", array('!sample_timestamp_variance' => $this->sampler->options['sample_timestamp_variance']));

    return $samples;
  }

  public function adjustSampleResults($samples) {
    // Stub function, required in the interface.
    return $samples;
  }
}
