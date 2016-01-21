<?php

/**
 * @file
 * Method plugin to generate sample sets using a random sample strategy.
 *
 * Random sampling returns a random selection of object_id values to sample
 * based on the object_id set returned from the trackObjectIDs() method
 * implemented by a metric.
 *
 * This plugin accepts the following options:
 *   random_object_quantity:
 *     The number of random objects to build the sample set from.  Defaults to
 *     100.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerMethodHandlerRandom',
  ),
);
