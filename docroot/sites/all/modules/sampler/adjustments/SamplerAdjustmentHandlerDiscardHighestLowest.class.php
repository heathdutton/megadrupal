<?php


/**
 * @file
 * Handler class for discard_highest_lowest adjustment plugin.
 */

class SamplerAdjustmentHandlerDiscardHighestLowest implements SamplerAdjustmentHandlerInterface {

  public $highest = array();
  public $lowest = array();

  public function __construct($sampler) {
    $this->sampler = $sampler;

    // Dump in plugin option defaults.
    $this->sampler->options = $this->sampler->options + $this->options();
  }

  public function options() {
    return array(
      'discard_lowest_quantity' => 10,
      'discard_highest_quantity' => 10,
    );
  }

  public function adjustSampleSet($samples) {
    // Stub function, required by the interface.
    return $samples;
  }

  public function adjustSampleResults($samples) {

    // Determine if the sample has single value results, otherwise this is too
    // complicated and we skip it.
    if (is_array($samples) && (current($samples) !== FALSE)) {
      $first_sample = current($samples);
      if (is_array($first_sample->values) && (current($first_sample->values) !== FALSE)) {
        $first_object_values = current($first_sample->values);
        if (count($first_object_values) == 1) {
          foreach ($samples as $sample_key => $sample) {
            // Next, make sure that sample size is greater than the sum of the highest
            // and lowest quantities -- the strategy isn't very useful otherwise.
            if (count($sample->values) > ($this->sampler->options['discard_lowest_quantity'] + $this->sampler->options['discard_highest_quantity'])) {

              // Start an array for this sample to record the discarded values
              // in the object.
              $this->sampler->highestDiscarded[$sample_key] = array();
              $this->sampler->lowestDiscarded[$sample_key] = array();

              $this->highest = array();
              $this->lowest = array();

              // Cycle through the values in the sample.
              foreach ($sample->values as $values_key => $values) {
                if ($value = array_shift($values)) {
                  foreach (array('highest', 'lowest') as $check_key) {
                    // The array of highest/lowest items isn't full yet, so put this item in.
                    if (count($this->$check_key) < $this->sampler->options["discard_{$check_key}_quantity"]) {
                      array_push($this->$check_key, $values_key);
                    }
                    // The array is full, so compare this value against the others that are in
                    // the array.
                    else {
                      // We only want to keep going through this loop if we
                      // haven't had a match yet.  Can't break; out this deep,
                      // so we use this approach.
                      $hit = FALSE;
                      // Loop through the existing list of potential discards.
                      foreach ($this->$check_key as $key => $existing_key) {
                        if (!$hit) {
                          switch ($check_key) {
                            case 'highest':
                              // Current value is higher than this value, so
                              // replace the old object's key with this
                              // object's key.
                              if ($value > $samples[$sample_key]->values[$existing_key][0]) {
                                $this->highest[$key] = $values_key;
                                // print "$value higher than {$samples[$sample_key]->values[$existing_key][0]}<br />";
                                $hit = TRUE;
                              }
                              break;
                            case 'lowest':
                              // Current value is lower than this value, so
                              // replace the old objects's key with this
                              // object's key.
                              if ($value < $samples[$sample_key]->values[$existing_key][0]) {
                                $this->lowest[$key] = $values_key;
                                // print "$value lower than {$samples[$sample_key]->values[$existing_key][0]}<br />";
                                $hit = TRUE;
                              }
                              break;
                          }
                        }
                        else {
                          break;
                        }
                      }
                    }
                  }
                }
              }

              // Now, clean out the objects that we've determined need to be
              // discarded.
              foreach (array('highest', 'lowest') as $discard) {
                foreach ($this->$discard as $discard_key) {
                  // Save the actual value, and throw away the entire values
                  // array for the object.
                  switch ($discard) {
                    case 'highest':
                      array_push($this->sampler->highestDiscarded[$sample_key], $samples[$sample_key]->values[$discard_key][0]);
                      break;
                    case 'lowest':
                      array_push($this->sampler->lowestDiscarded[$sample_key], $samples[$sample_key]->values[$discard_key][0]);
                      break;
                  }
                  unset($samples[$sample_key]->values[$discard_key]);
                }
              }
            }
          }
        }
      }
    }

    $this->sampler->pluginOutput['adjustment_discard_highest_lowest'] = t("Discarded !highest highest values, !lowest lowest values", array('!highest' => $this->sampler->options['discard_highest_quantity'], '!lowest' => $this->sampler->options['discard_lowest_quantity']));

    return $samples;
  }
}
