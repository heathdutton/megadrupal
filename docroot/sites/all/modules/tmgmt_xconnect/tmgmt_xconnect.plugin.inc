<?php
/**
 * @file
 * X-Connect translator plugin.
 */

/**
 * X-Connect Translator plugin.
 */
class TmgmtXConnectTranslatorPluginController extends TMGMTDefaultTranslatorPluginController {
  /**
   * {@inheritdoc}
   */
  public function canTranslate(TMGMTTranslator $translator, TMGMTJob $job) {
    // The X-Connect support all kinds of translations.
    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function requestTranslation(TMGMTJob $job) {
    $args = array('%name' => $job->getTranslator()->name);

    try {
      // Order name (file name) based on the Job details.
      $order_name = sprintf(
        'JobID%s_%s_%s',
        $job->tjid,
        $job->source_language,
        $job->target_language
      );

      // Create the order.
      $order = new TmgmtXConnectRequestOrder(
        $order_name,
        $job->source_language,
        tmgmt_xconnect_settings_order($job->getTranslator())
      );
      $order->addTargetLanguage($job->target_language);

      // Order options.
      $order->addInstructions(
        filter_xss($job->getSetting('instructions'))
      );
      $order->setClientReference(
        filter_xss($job->getSetting('reference'))
      );

      // Create the request file.
      $file = new TmgmtXConnectRequestFile(
        $order,
        drupal_realpath(tmgmt_xconnect_directory_request())
      );

      // Add the translation strings, add a separate file for each Job Item.
      $exporter = new TmgmtXConnectExportHtml();
      foreach ($job->getItems() as $item) {
        $file->addFromString(
          $order_name . '_' . $item->tjiid . '.html',
          $exporter->exportJobItem($item)
        );
      }
      $file->finish();

      // Send the request file.
      $connection = tmgmt_xconnect_connection($job->getTranslator());
      $result = $connection->send($file);
    }
    catch (Exception $e) {
      $args['%message'] = $e->getMessage();
      $job->addMessage(
        'There was a problem sending the translation job to %name : %message',
        $args,
        'error'
      );
      return;
    }

    // Update the Job status depending on the send request result.
    if ($result) {
      $job->submitted();
    }
    else {
      $job->addMessage(
        'There was a problem sending the translation job to %name',
        $args,
        'error'
      );
    }
  }

  /**
   * {@inheritdoc}
   */
  public function defaultSettings() {
    return array(
      'order' => array(
        'client_id' => '',
        'due_date' => 0,
        'issued_by' => '',
        'is_confidential' => 0,
        'needs_confirmation' => 1,
        'needs_quotation' => 0,
      ),
      'connection' => array(
        'protocol' => 'FTP',
        'port' => 21,
        'host' => '',
        'user' => '',
        'pass' => '',
        'folder_request' => 'To_LSP',
        'folder_receive' => 'From_LSP',
        'folder_processed' => 'From_LSP/processed',
      ),
      'cron' => array(
        'enabled' => 1,
        'limit' => 0,
      ),
    );
  }

}
