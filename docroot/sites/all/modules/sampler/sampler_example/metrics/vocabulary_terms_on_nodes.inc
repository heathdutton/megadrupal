<?php

/**
 * @file
 * Metric plugin for measuring number of vocabulary terms on nodes.
 *
 * This metric is meant to be used with the periodic sampling method.  It
 * measures the number of vocubulary terms that are on all nodes within a
 * certain time period,  Each vocabulary is tracked separately.
 *
 * This metric accepts the following options:
 *   object_ids:
 *     An array of vocabulary vids to filter on.
 */

$plugin = array(
  'title' => t('Vocabulary terms on nodes'),
  'description' => t('For each vocabulary, shows how many times a term from that vocabulary was used on a node.'),
  // We're sampling taxonomy vocabularies, so we have to let views know to
  // use the right base table for any views.
  'object_base_table' => 'taxonomy_vocabulary',
  'object_primary_key' => 'vid',
  'default_options' => array(
    'time_unit' => 3600,
    'time_unit_interval' => 3,
  ),
  'data_type' => array(
    'nodes' => 'int',
  ),
  'handler' => array(
    'class' => 'SamplerExampleMetricVocabularyTermsOnNodes',
  ),
);

