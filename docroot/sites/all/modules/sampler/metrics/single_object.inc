<?php


/**
 * @file
 * Base plugin for metrics that only have one object.
 */

$plugin = array(
  'title' => t('Single object'),
  'description' => t("Base plugin for single object metrics."),
  'non_metric' => TRUE,
  'handler' => array(
    'class' => 'SamplerSingleObjectMetric',
  ),
);
