<?php

/**
 * @file
 * Metric plugin for counting the number of comments on nodes.
 *
 * This metric is meant to be used with the random sampling method.  In this
 * example, we're calculating how many comments have been posted on a random
 * number of nodes.
 *
 * This metric accepts the following options:
 *   startstamp:
 *     The Unix timestamp to filter node created dates on, only pulling nodes
 *     created after this stamp.
 *   endstamp:
 *     The Unix timestamp to filter node created dates on, only pulling nodes
 *     created before this stamp.
 *   node_type:
 *     Node type to filter on.
 *   object_ids:
 *     An array of nids.  If passed it will restrict the results to nodes in
 *     this list.
 */

$plugin = array(
  'title' => t('Comments per node'),
  'description' => t("Shows how many comments were posted on each node."),
  // Set the method here since the metric is designed for this.
  'method' => 'random',
  // Toss in a default adjustment.
  'adjustment' => 'discard_highest_lowest',
  'data_type' => array(
    'comments' => 'int',
  ),
  'handler' => array(
    'class' => 'SamplerExampleMetricCommentsPerNode',
  ),
);

