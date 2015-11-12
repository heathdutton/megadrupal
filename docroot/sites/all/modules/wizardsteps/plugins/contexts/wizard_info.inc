<?php

$plugin = array(
  'title' => t('Wizard Info'),
  'description' => t('Contains information about the active CTools wizard form'),
  'context' => 'wizardsteps_wizard_info_context',
  'keyword' => 'wizard_info',
  'context name' => 'wizard_info',
  'no ui' => TRUE,
);

function wizardsteps_wizard_info_context($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('wizard_info');
  $context->plugin = 'wizard_info';

  global $wizardsteps_wizard_info;

  if (!$wizardsteps_wizard_info) {
    return $context;
  }

  $context->data = $wizardsteps_wizard_info;

  return $context;
}
