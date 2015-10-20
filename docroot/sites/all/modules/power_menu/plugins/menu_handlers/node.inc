<?php

/**
 * The node plugin....
 */
$plugin = array(
  'title' => t("Node plugin"),
  'description' => t('Sets the active menu trail based on the entity type. '),
  'has edit form' => FALSE,
  'handler' => array(
    'class' => 'PowerMenuNodeHandler',
  ),
);
