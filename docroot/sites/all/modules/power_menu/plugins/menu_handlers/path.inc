<?php

/**
 * The path plugin....
 */
$plugin = array(
  'title' => t("Path plugin"),
  'description' => t('Sets the active menu trail based on the path alias.'),
  'has edit form' => TRUE,
  'handler' => array(
    'class' => 'PowerMenuPathHandler',
  ),
);
