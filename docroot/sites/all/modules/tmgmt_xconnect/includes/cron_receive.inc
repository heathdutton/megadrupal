<?php
/**
 * @file
 * Contains the functionality to run the translations receive batch.
 */

/**
 * Run the Cron action to receive all processed translations.
 */
function tmgmt_xconnect_cron_receive_run() {
  $translators = tmgmt_xconnect_get_translators();
  if (!$translators) {
    return;
  }

  foreach ($translators as $translator) {
    tmgmt_xconnect_cron_receive_translator($translator);
  }
}

/**
 * Run the Cron action for a given translator.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 */
function tmgmt_xconnect_cron_receive_translator(TMGMTTranslator $translator) {
  $config = tmgmt_xconnect_settings_cron($translator);

  // Check if cron is not disabled for this translator.
  if ((int) $config['enabled'] !== 1) {
    watchdog(
      TMGMT_XCONNECT_WATCHDOG,
      'Cron : Disabled for translator %translator',
      array(
        '%translator' => $translator->label(),
      )
    );
    return;
  }

  // Check limit.
  $limit = (!empty($config['limit']))
    ? (int) $config['limit']
    : 1000000;

  // Get the files ready for translation.
  try {
    $files = tmgmt_xconnect_import_get_remote_files($translator);
  }
  catch (Exception $e) {
    watchdog(
      TMGMT_XCONNECT_WATCHDOG,
      'Cron : Could not scan for translated jobs for %translator : %message.',
      array(
        '%translator' => $translator->label(),
        '%message' => $e->getMessage(),
      ),
      WATCHDOG_ERROR
    );
    return;
  }

  // Log to watchdog if there are no files to process.
  if (!$files) {
    watchdog(
      TMGMT_XCONNECT_WATCHDOG,
      'Cron : No translations to import for translator %translator.',
      array(
        '%translator' => $translator->label(),
      )
    );
    return;
  }

  // Process the files that are ready.
  $count_files = 0;
  foreach ($files as $file_name) {
    // Check the limit to avoid cron time-out.
    if ($count_files >= $limit) {
      watchdog(
        TMGMT_XCONNECT_WATCHDOG,
        'Cron : File cron limit reached, processed %count of %count_total files for translator %translator.',
        array(
          '%translator' => $translator->label(),
          '%count' => $count_files,
          '%count_total' => count($files),
        )
      );
      break;
    }

    tmgmt_xconnect_cron_receive_file($translator, $file_name);
    $count_files++;
  }
}

/**
 * Run the Cron action for a single file.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 * @param string $file_name
 *   The file that is ready to be processed.
 */
function tmgmt_xconnect_cron_receive_file(TMGMTTranslator $translator, $file_name) {
  $result = tmgmt_xconnect_import_remote_file($translator, $file_name);
  $severity = ($result instanceof TmgmtXConnectImportResultSuccess)
    ? WATCHDOG_INFO
    : WATCHDOG_ERROR;

  watchdog(
    TMGMT_XCONNECT_WATCHDOG,
    $result->getMessage(),
    $result->getMessageArguments(),
    $severity
  );
}
