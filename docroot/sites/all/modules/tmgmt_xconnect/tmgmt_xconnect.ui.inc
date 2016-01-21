<?php
/**
 * @file
 * X-Connect translator User Interface.
 */

/**
 * X-Connect translator User Interface.
 */
class TmgmtXConnectTranslatorUIController extends TMGMTDefaultTranslatorUIController {
  /**
   * {@inheritdoc}
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {
    // The order.xml settings.
    $settings_order = tmgmt_xconnect_settings_order($translator);
    $form['order'] = array(
      '#type' => 'fieldset',
      '#title' => t('Order'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['order']['client_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Client ID'),
      '#description' => t('The client ID to order the translations for.'),
      '#default_value' => $settings_order['client_id'],
    );
    $form['order']['due_date'] = array(
      '#type' => 'textfield',
      '#title' => t('Due date in number of days'),
      '#description' => t('What is the deadline for the file(s) to be translated. The deadline should be set in number of days from the moment the translation is ordered.'),
      '#default_value' => $settings_order['due_date'],
    );
    $form['order']['issued_by'] = array(
      '#type' => 'textfield',
      '#title' => t('Issued By'),
      '#description' => t('The email address of the, by the translation known, issuer of the translation.'),
      '#default_value' => $settings_order['issued_by'],
    );
    $form['order']['is_confidential'] = array(
      '#type' => 'radios',
      '#title' => t('Confidential'),
      '#description' => t('Is the content for the translation confidential?'),
      '#default_value' => $settings_order['is_confidential'],
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
    );
    $form['order']['needs_confirmation'] = array(
      '#type' => 'radios',
      '#title' => t('Needs confirmation'),
      '#description' => t('Should there be a confirmation sent when the translation is ready?'),
      '#default_value' => $settings_order['needs_confirmation'],
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
    );
    $form['order']['needs_quotation'] = array(
      '#type' => 'radios',
      '#title' => t('Needs quotation'),
      '#description' => t('Should a quotation be created and sent before the translation is performed?'),
      '#default_value' => $settings_order['needs_quotation'],
      '#options' => array(
        0 => t('No'),
        1 => t('Yes'),
      ),
    );

    // The connection settings.
    $settings_connection = tmgmt_xconnect_settings_connection($translator);
    $form['connection'] = array(
      '#type' => 'fieldset',
      '#title' => t('Connection'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $info = t('The X-Connect translator sends the translation requests through a FTP server to the translation service. Provide the connection parameters here:');
    $form['connection']['info'] = array(
      '#markup' => '<p>' . $info . '</p>',
    );
    $form['connection']['protocol'] = array(
      '#type' => 'radios',
      '#title' => t('Connection type'),
      '#description' => t('The protocol to use to connect to the FTP server.'),
      '#options' => array(
        'FTP' => t('FTP'),
        'SFTP' => t('SFTP'),
      ),
      '#default_value' => $settings_connection['protocol'],
    );
    $form['connection']['timeout'] = array(
      '#type' => 'textfield',
      '#title' => t('Timeout'),
      '#description' => t('The FTP server connection timeout in milliseconds (e.g. 100)'),
      '#default_value' => $settings_connection['timeout'],
    );
    $form['connection']['port'] = array(
      '#type' => 'textfield',
      '#title' => t('Port'),
      '#description' => t('The FTP server port to connect to (e.g. 21 for FTP, 22 for SFTP).'),
      '#default_value' => $settings_connection['port'],
    );
    $form['connection']['host'] = array(
      '#type' => 'textfield',
      '#title' => t('Host'),
      '#description' => t('The FTP server to connect to (e.g. ftp://hostname.domain).'),
      '#default_value' => $settings_connection['host'],
    );
    $form['connection']['user'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#description' => t('The username to connect to the FTP server.'),
      '#default_value' => $settings_connection['user'],
    );
    $form['connection']['pass'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#description' => t('The password to connect to the FTP server.'),
      '#default_value' => $settings_connection['pass'],
    );
    $form['connection']['folder_request'] = array(
      '#type' => 'textfield',
      '#title' => t('Request folder'),
      '#description' => t('The folder to store the translation requests on the FTP server (e.g. To_LSP).'),
      '#default_value' => $settings_connection['folder_request'],
    );
    $form['connection']['folder_receive'] = array(
      '#type' => 'textfield',
      '#title' => t('Receive folder'),
      '#description' => t('The folder to receive the translation requests from on the FTP server (e.g. From_LSP).'),
      '#default_value' => $settings_connection['folder_receive'],
    );

    $form['connection']['folder_processed'] = array(
      '#type' => 'textfield',
      '#title' => t('Processed folder'),
      '#description' => t('The folder to put the translation responses on the FTP server to inform the translation service that a translation is processed (e.g. From_LSP/processed).'),
      '#default_value' => $settings_connection['folder_processed'],
    );

    // Cron settings.
    $settings_cron = tmgmt_xconnect_settings_cron($translator);
    $form['cron'] = array(
      '#type' => 'fieldset',
      '#title' => t('Cron'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['cron']['enabled'] = array(
      '#type' => 'radios',
      '#title' => t('Cron status'),
      '#options' => array(
        1 => t('Enabled'),
        0 => t('Disabled'),
      ),
      '#description' => t('Collecting the translated jobs is done during cron (or manual).'),
      '#default_value' => $settings_cron['enabled'],
    );
    $form['cron']['limit'] = array(
      '#type' => 'select',
      '#title' => t('Limit'),
      '#description' => t('Maximum number of files to process during cron run.'),
      '#options' => array(
        0 => t('No limit'),
        1 => 1,
        2 => 2,
        5 => 5,
        10 => 10,
        20 => 20,
        50 => 50,
        100 => 100,
        200 => 200,
        500 => 500,
        1000 => 1000,
      ),
      '#default_value' => $settings_cron['limit'],
    );

    return parent::pluginSettingsForm($form, $form_state, $translator);
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    $form['instructions'] = array(
      '#type' => 'textarea',
      '#title' => t('Instructions'),
      '#description' => t('Add optional instructions for the translator(s).'),
    );
    $form['reference'] = array(
      '#type' => 'textfield',
      '#title' => t('Reference'),
      '#description' => t('Add an optional reference to the translation order, this will be used in communication about the translation request.'),
    );

    return parent::checkoutSettingsForm($form, $form_state, $job);
  }

  /**
   * {@inheritdoc}
   */
  public function checkoutInfo(TMGMTJob $job) {
    // Show the Job settings (if any).
    $form = array();

    $instructions = $job->getSetting('instructions');
    if (!empty($instructions)) {
      $form['instructions'] = array(
        '#type' => 'item',
        '#title' => t('Instructions'),
        '#markup' => check_markup(filter_xss($job->getSetting('instructions'))),
      );
    }

    $reference = $job->getSetting('reference');
    if (!empty($reference)) {
      $form['reference'] = array(
        '#type' => 'item',
        '#title' => t('Reference'),
        '#markup' => filter_xss($job->getSetting('reference')),
      );
    }

    return $form;
  }

}
