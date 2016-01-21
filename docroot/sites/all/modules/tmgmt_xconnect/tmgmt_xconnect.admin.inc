<?php
/**
 * @file
 * Administration pages & forms for the X-Connect module.
 */

/**
 * Page callback to perform actions on X-Connect translators.
 *
 * @return array
 *   Render array for the page.
 */
function tmgmt_xconnect_admin_actions() {
  $render_array = array();

  $render_array['translators'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('xconnect-translators-overview'),
    ),
  );

  // Add an overview of x-connect translators.
  $translators = tmgmt_xconnect_get_translators();
  if ($translators) {
    $header = array(
      t('Label'),
      array(
        'data' => t('Operations'),
        'colspan' => 3,
      ),
    );
    $rows = array();
    foreach ($translators as $translator) {
      $row = array(
        check_plain($translator->label()) . '<br><small>' . check_plain($translator->description) . '</small>',
        l(t('Send'), 'admin/tmgmt/x-connect/' . $translator->name . '/request'),
        l(t('Scan'), 'admin/tmgmt/x-connect/' . $translator->name . '/scan'),
        l(t('Receive'), 'admin/tmgmt/x-connect/' . $translator->name . '/receive'),
      );
      $rows[] = $row;
    }

    $render_array['translators']['table'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
    );

    $legend = array(
      t('<strong>Send</strong> will send all unprocessed jobs for the specific translator service.'),
      t('<strong>Scan</strong> will scan the remote service and report about the number of processed jobs that are ready to be picked up.'),
      t('<strong>Receive</strong> will pick up any processed request and import them back into the platform.'),
    );
    $render_array['translators']['legend'] = array(
      '#theme' => 'item_list',
      '#items' => $legend,
    );
  }
  else {
    $render_array['translators']['#attributes']['class'] = array(
      'messages',
      'warning',
    );

    $render_array['translators']['none_message'] = array(
      '#markup' => t('There are no translators that use the X-Connect plugin.'),
    );
    $render_array['translators']['none_link'] = array(
      '#markup' => l(
        t('Configure translators'),
        'admin/config/regional/tmgmt_translator'
      ),
      '#prefix' => '<p>',
      '#postfix' => '</p>',
    );
  }

  return $render_array;
}

/**
 * Menu callback to send out translator requests.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 */
function tmgmt_xconnect_admin_actions_request(TMGMTTranslator $translator) {
  module_load_include('inc', 'tmgmt_xconnect', 'includes/batch_request');
  tmgmt_xconnect_batch_request_run($translator, '/admin/tmgmt/x-connect');
}

/**
 * Menu callback to scan the remote service FTP folder.
 *
 * This will connect to the FTP service and report about the number of
 * translated job files waiting to be imported.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 */
function tmgmt_xconnect_admin_actions_scan(TMGMTTranslator $translator) {
  try {
    $connection = tmgmt_xconnect_connection($translator);

    $files = $connection->scan();
    $count = count($files);

    drupal_set_message(
      t(
        'There are %count processed requests ready to be picked up for the %service service.',
        array(
          '%count' => $count,
          '%service' => $translator->label(),
        )
      )
    );
  }
  catch (Exception $e) {
    drupal_set_message(
      t(
        'There was a problem while scanning for new translations: %message',
        array('%message' => $e->getMessage())
      ),
      'error'
    );
  }

  drupal_goto('/admin/tmgmt/x-connect');
}

/**
 * Menu callback to receive processed translations from the remote service.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 */
function tmgmt_xconnect_admin_actions_receive(TMGMTTranslator $translator) {
  module_load_include('inc', 'tmgmt_xconnect', 'includes/batch_receive');
  tmgmt_xconnect_batch_receive_run($translator, '/admin/tmgmt/x-connect');
}
