<?php
/**
 * @file
 * Sendinblue module campaign admin settings.
 */

/**
 * Display Campaign page of module.
 *
 * @return array
 *   Render array.
 */
function sendinblue_campaign_page() {
  $page_html = '<div id="wrap" class="box-border-box container-fluid">';
  $page_html .= SendinblueManager::generateCampaignLogin() . '</div>';
  $output = array(
    'main_wrapper' => array(
      '#type' => 'markup',
      '#markup' => $page_html,
    ),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'sendinblue') . '/css/admin-setting.css'),
    ),
  );
  return $output;
}
