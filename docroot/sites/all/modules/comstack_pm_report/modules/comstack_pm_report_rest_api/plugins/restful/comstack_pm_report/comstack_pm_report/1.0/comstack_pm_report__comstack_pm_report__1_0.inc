<?php

$plugin = array(
  'label' => t('Report a conversation'),
  'resource' => 'cs-pm-report',
  'name' => 'comstack_pm_report__comstack_pm_report__1_0',
  'entity_type' => 'comstack_pm_report',
  'bundle' => 'comstack_pm_report',
  'description' => t('Report a conversation.'),
  'class' => 'ComstackPMReportResource__1_0',
  'authentication_types' => TRUE,
  'authentication_optional' => FALSE,
  'formatter' => 'comstack_json',
);
