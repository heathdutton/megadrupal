<?php


/**
 * @file
 * Adjustment plugin for discarding the lowest/highest values from sample
 * results.
 *
 * This plugin accepts the following options:
 *   discard_lowest_quantity:
 *     The number of the highest values to discard. Defaults to 10.
 *   abort_timestamp:
 *     The number of the lowest values to discard. Defaults to 10.
 */

$plugin = array(
  'handler' => array(
    'class' => 'SamplerAdjustmentHandlerDiscardHighestLowest',
  ),
);

