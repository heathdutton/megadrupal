<?php

/**
 * @file
 * Filter display plugin.
 */

$plugin = array(
  'title' => t('Filter'),
  'description' => t('Text is shown/hidden based on conditions'),
  'theme' => array(
    'false' => 'conditional_text_nodisplay',
  ),
  'js' => array(),
  'css' => array(),
);
