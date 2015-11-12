<?php
/**
 *  @file
 *  Admin page callbacks for the Personalize Panels module.
 */

/**
 * Page callback to list personalized panel instances in the system.
 */
function personalize_panels_list() {
  $header = array(
    array('data' => t('Pane')),
    array('data' => t('Operations'), 'colspan' => 3),
  );
  $rows = array();

  foreach (personalize_option_set_load_by_type('panel_pane') as $personalized_panel) {
    $tablerow = array(
      array('data' => check_plain($personalized_panel->label)),
      array('data' => l(t('Edit'), "admin/structure/pages/nojs/operation/{$personalized_panel->data['task_name']}/handlers/{$personalized_panel->data['handler_id']}/content")),
      array('data' => l(t('Delete'), "admin/structure/personalize/variations/personalize-panels/manage/{$personalized_panel->osid}/delete")),
    );
    $rows[] = $tablerow;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No personalized panels available.'), 'colspan' => 4));
  }

  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'personalize_panels'),
  );
  return $build;
}
