<?php
/**
 * @file
 * Displays a table with some summary metrics around petition.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('Overview metrics table'),
  'description' => t('Displays overview metrics table about petitions stored in the system.'),
  'category' => t('CRM Core Petition'),
  'edit form' => 'cmcp_overview_metrics_form',
  'render callback' => 'cmcp_overview_metrics_render',
  'admin info' => 'cmcp_overview_metrics_admin_info',
  'defaults' => array(
    'text' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Administration info.
 */
function cmcp_overview_metrics_admin_info($subtype, $conf, $contexts) {

  $block = new stdClass();
  $block->title = t('Overview metrics table');
  $block->content = t('Displays overview metrics table about petitions stored in the system.');

  return $block;
}

/**
 * Settings form.
 */
function cmcp_overview_metrics_form($form, &$form_state) {

  return $form;
}

/**
 * Submit handler for settings form.
 */
function cmcp_overview_metrics_form_submit($form, &$form_state) {
}

/**
 * Render callback.
 */
function cmcp_overview_metrics_render($subtype, $conf, $panel_args, $context = NULL) {

  $active_petitions = db_query("
SELECT COUNT(petition.nid)
FROM {node} as petition
WHERE petition.status = 1
");
  $active_petitions = (int) $active_petitions->fetchField();

  $signatures = db_query("
SELECT COUNT(signature.activity_id) AS signatures
FROM {crm_core_activity} signature
WHERE signature.type = 'cmcp_petition_signature'
");
  $signatures = (int) $signatures->fetchField();

  $total_petitions = db_query("
SELECT COUNT(petition.nid)
FROM {node} as petition
WHERE petition.type = 'cmcp_petition' AND petition.status = 1
");
  $total_petitions = (int) $total_petitions->fetchField();

  $header = array(
    t('Active petitions'),
    t('Total signatures'),
    t('Average signatures'),
  );
  $rows = array();

  if (empty($total_petitions)) {
    $average_signatures = 0;
  }
  else {
    $average_signatures = round($signatures / $total_petitions, 1);
  }

  $rows[] = array(
    'data' => array(
      $active_petitions,
      $signatures,
      $average_signatures,
    ),
  );

  $attr = array('class' => array('summary-table'));
  $vars = array('rows' => $rows, 'header' => $header, 'attributes' => $attr);
  $content = '<div class="table-container">' . theme('table', $vars) . '</div>';

  $block = new stdClass();
  $block->title = '';
  $block->content = $content;

  return $block;
}
