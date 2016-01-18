<?php


/**
 * @file
 * Adjustment plugin -- selects random X objects per sample.
 *
 * This plugin accepts the following options:
 *   random_object_quantity:
 *     The number of random objects to build the sample set from.  Defaults to
 *     100.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerAdjustmentHandlerRandom',
  ),
);
