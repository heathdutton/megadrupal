<?php

/**
 * @file
 * ContextIOMessageFetcher related classes.
 */

define('CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM', 1);
define('CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_ALL', 2);

/**
 * Result of ContextIOMessageFetcher::fetch().
 */
class ContextIOMessageFetcherResult extends FeedsFetcherResult {

  /**
   * Constructor.
   *
   * @param array $messages
   *   Property array of messages.
   */
  public function __construct(array $messages) {
    parent::__construct('');
    $this->messages = $messages;
  }

  /**
   * Get a message.
   */
  public function getMessages() {
    return $this->messages;
  }

}

/**
 * Fetches email data from Context.IO.
 */
class ContextIOMessageFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);

    $state = $source->state(FEEDS_FETCH);
    // Initialize batch settings. Add a batch_ prefix to own batch related
    // variable names to avoid name conflicts with Feeds's own variables.
    if (!isset($state->batch_progress)) {
      $batch_size = $this->config['additional_options']['batch_size'];
      if ($source_config['number_of_messages'] < $batch_size) {
        $batch_size = $source_config['number_of_messages'];
      }
      $state->batch_progress = 0;
      $state->batch_messages = $this->getMessages($batch_size, $source_config['offset']);
      $state->batch_offset = $source_config['offset'] + count($state->batch_messages);
      if ($source_config['import_type'] == CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM) {
        $state->batch_total = $source_config['number_of_messages'];
      }
      else {
        try {
          $account = contextio_get_account($this->config['mailbox']['account_id']);
          if (!empty($account['nb_messages'])) {
            $state->batch_total = $account['nb_messages'];
          }
          else {
            watchdog('contextio_feeds_plugins', 'Missing nb_messages property after a contextio_get_account() call.', array(), WATCHDOG_ERROR);
          }
        }
        catch (Exception $e) {
          watchdog('contextio_feeds_plugins', 'An error occurred during a contextio_get_account() call. Error message: @message', array(
            '@message' => $e->getMessage(),
          ), WATCHDOG_ERROR);
        }
      }
    }

    // Try to download the next message batch.
    if (empty($state->batch_messages) && ($state->batch_progress < $state->batch_total)) {
      $batch_size = $this->config['additional_options']['batch_size'];
      if (($state->batch_total - $state->batch_progress) < $batch_size) {
        $batch_size = $state->batch_total - $state->batch_progress;
      }
      $state->batch_messages = $this->getMessages($batch_size, $state->batch_offset);
      $state->batch_offset += count($state->batch_messages);
    }

    // Process messages.
    if (!empty($state->batch_messages)) {
      $messages = $state->batch_messages;
      $state->batch_messages = array();
      $state->batch_progress += count($messages);
      $state->progress($state->batch_total, $state->batch_progress);
      return new ContextIOMessageFetcherResult($messages);
    }

    // The number of the messages that can be imported is lower than the number
    // we wanted to fetch, so override the state progress and finish the import.
    $state->progress($state->batch_progress, $state->batch_progress);
    return new ContextIOMessageFetcherResult(array());
  }

  /**
   * Helper function to fetch messages.
   *
   * @param int $limit
   *   The maximum number of messages to download.
   * @param int $offset
   *   The offset the download process will start at.
   *
   * @return array
   *   An array of messages.
   */
  protected function getMessages($limit, $offset) {
    $filters = array();
    // Add filter options.
    foreach ($this->config['filter_options'] as $key => $value) {
      if (!empty($value)) {
        $filters[$key] = $value;
      }
    }

    // Limit can be lower then the default batch size.
    $filters['limit'] = $limit;
    // Additional options.
    $filters['include_body'] = (int) $this->config['additional_options']['include_body'];
    $filters['include_flags'] = (int) $this->config['additional_options']['include_flags'];
    // Source options.
    $filters['offset'] = $offset;

    $messages = array();

    try {
      $messages_data = contextio_get_messages($this->config['mailbox']['account_id'], $filters);
      if (!empty($messages_data)) {
        foreach ($messages_data as $message) {
          $messages[] = $message;
        }
      }
    }
    catch (Exception $e) {
      watchdog('contextio_feeds_plugins', 'An error occurred during a contextio_get_messages() call. Error message: @message', array(
        '@message' => $e->getMessage(),
      ), WATCHDOG_ERROR);
    }

    return $messages;
  }

  /**
   * Override parent::sourceForm().
   */
  public function sourceForm($source_config) {
    $form = array('#tree' => TRUE);
    $form['email_address'] = array(
      '#type' => 'item',
      '#title' => t('Email address'),
      '#markup' => check_plain($this->config['mailbox']['email_address']),
    );
    $form['import_type'] = array(
      '#type' => 'radios',
      '#title' => t('Import type'),
      '#options' => array(
        CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM => t('Import a specified number of messages'),
        CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_ALL => t('Import all messages'),
      ),
      '#default_value' => CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM,
    );
    $form['number_of_messages'] = array(
      '#type' => 'textfield',
      '#title' => t('Number of messages to import'),
      '#default_value' => 100,
      '#size' => 7,
      '#states' => array(
        'visible' => array(
          ':input[name="feeds[' . get_class($this) . '][import_type]"]' => array(
            'value' => CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM,
          ),
        ),
      ),
    );
    $form['offset'] = array(
      '#type' => 'textfield',
      '#title' => t('Offset'),
      '#default_value' => 0,
      '#description' => t('Start the import process at this offset (zero-based).'),
      '#size' => 7,
    );

    $filter_settings = array();
    // Check non-empty filter settings.
    foreach ($this->config['filter_options'] as $key => $value) {
      if (!empty($value)) {
        switch ($key) {
          case 'subject':
            $filter_settings[] = '<strong>' . t('Subject') . ':</strong> ' . check_plain($value);
            break;

          case 'to':
            $filter_settings[] = '<strong>' . t('To') . ':</strong> ' . check_plain($value);
            break;

          case 'from':
            $filter_settings[] = '<strong>' . t('From') . ':</strong> ' . check_plain($value);
            break;

          case 'cc':
            $filter_settings[] = '<strong>' . t('CC') . ':</strong> ' . check_plain($value);
            break;

          case 'bcc':
            $filter_settings[] = '<strong>' . t('BCC') . ':</strong> ' . check_plain($value);
            break;

          case 'folder':
            $filter_settings[] = '<strong>' . t('Folder') . ':</strong> ' . check_plain($value);
            break;

          case 'date_before':
            $filter_settings[] = '<strong>' . t('Date before') . ':</strong> ' . date('Y-m-d H:i', $value);
            break;

          case 'date_after':
            $filter_settings[] = '<strong>' . t('Date after') . ':</strong> ' . date('Y-m-d H:i', $value);
            break;
        }
      }
    }

    // Show non-empty filter settings.
    if (!empty($filter_settings)) {
      $form['filter_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['filter_settings']['message'] = array(
        '#prefix' => '<div class="description">',
        '#markup' => t('Filter settings are treated as an AND combination.'),
        '#suffix' => '</div>',
      );
      $form['filter_settings']['list'] = array(
        '#prefix' => '<p>',
        '#markup' => theme('item_list', array(
          'items' => $filter_settings,
          'type' => 'ul',
        )),
        '#suffix' => '</p>',
      );
    }

    return $form;
  }

  /**
   * Overrides parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    if ($values['import_type'] == CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM) {
      $number_of_messages = $values['number_of_messages'];
      if (!(is_numeric($number_of_messages) && ((int) $number_of_messages >= 1))) {
        $field_name = 'feeds][' . get_class($this) . '][number_of_messages';
        form_set_error($field_name, t('Number of messages to import must be an integer value and greater than or equal to 1.'));
      }
    }

    $offset = $values['offset'];
    if (!(is_numeric($offset) && ((int) $offset >= 0))) {
      $field_name = 'feeds][' . get_class($this) . '][offset';
      form_set_error($field_name, t('Offset must be an integer value and greater than or equal to 0.'));
    }
  }

  /**
   * Override parent::sourceSave().
   */
  public function sourceSave(FeedsSource $source) {
    // Convert string values to integers.
    if ($source->config[get_class($this)]['import_type'] == CONTEXTIOMESSAGEFETCHER_IMPORT_TYPE_NUM) {
      $number_of_messages = &$source->config[get_class($this)]['number_of_messages'];
      $number_of_messages = (int) $number_of_messages;
    }

    $offset = &$source->config[get_class($this)]['offset'];
    $offset = (int) $offset;
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'mailbox' => array(
        'account_id' => '',
        'email_address' => '',
      ),
      'additional_options' => array(
        'batch_size' => 50,
        'include_body' => 0,
        'include_flags' => 0,
      ),
      'filter_options' => array(
        'subject' => '',
        'to' => '',
        'from' => '',
        'cc' => '',
        'bcc' => '',
        'folder' => '',
        'date_before' => '',
        'date_after' => '',
      ),
    );
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array('#tree' => TRUE);
    $key = variable_get('contextio_key', NULL);
    $secret = variable_get('contextio_secret', NULL);

    if (!empty($key) && !empty($secret)) {
      // Default value of the email address depends on a valid connect token.
      $email_address_default_value = $this->config['mailbox']['email_address'];

      if (!empty($_GET['contextio_token'])) {
        // Get the connect token related account id and save it.
        try {
          $connect_token = contextio_get_connect_token(NULL, $_GET['contextio_token']);
          if (!empty($connect_token['account']['id'])) {
            $config = $this->config;
            $config['mailbox']['account_id'] = $connect_token['account']['id'];
            $this->setConfig($config);
            $this->save();
            drupal_get_messages('status');
            drupal_set_message(t('Your settings have been saved. The new mailbox has been added to your Context.IO account.'));
          }
          else {
            $email_address_default_value = '';
            drupal_set_message(t("The new mailbox creation wasn't successful. Please try again."), 'error');
            watchdog('contextio_feeds_plugins', 'Missing account][id property after contextio_get_connect_token() call.', array(), WATCHDOG_ERROR);
          }
        }
        catch (Exception $e) {
          watchdog('contextio_feeds_plugins', 'An error occurred during a contextio_get_connect_token() call. Error message: @message', array(
            '@message' => $e->getMessage(),
          ), WATCHDOG_ERROR);
        }
      }

      // Add new mailbox warning message container.
      $form['add_new_mailbox_message'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'id' => 'add-new-mailbox-message',
          'style' => 'display: none;',
        ),
      );
      // We set a message but right after that we will clear the message queue.
      drupal_set_message(t(
        'After settings have been saved you will be redirected to grant access to Context.IO to use your email address. Once the
         process is complete you will be redirected back to this page.'
      ), 'warning');
      $form['add_new_mailbox_message']['message'] = array(
        '#markup' => theme('status_messages', array('display' => 'warning')),
      );
      $form['#attached'] = array(
        'js' => array(
          drupal_get_path('module', 'contextio_feeds_plugins') . '/js/message_fetcher_config_form.js',
        ),
      );

      // Mailbox settings.
      $form['mailbox'] = array(
        '#type' => 'fieldset',
        '#title' => t('Mailbox'),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );
      $form['mailbox']['account_id'] = array(
        '#type' => 'value',
        '#value' => $this->config['mailbox']['account_id'],
      );
      $form['mailbox']['email_address'] = array(
        '#type' => 'textfield',
        '#title' => t('Email address'),
        '#description' => t('The mailbox email address messages should be imported from.'),
        '#default_value' => $email_address_default_value,
        '#required' => TRUE,
      );
      $form['mailbox']['add_new_mailbox'] = array(
        '#type' => 'checkbox',
        '#title' => t('Add new mailbox'),
        '#description' => t('Add a new mailbox in a new account holder with the provided email address.'),
        // We always want to reset this field value, so we don't save the value.
        '#default_value' => 0,
      );
      $form['mailbox']['first_name'] = array(
        '#type' => 'textfield',
        '#title' => t('First name'),
        '#description' => t('The first name for the new mailbox account holder.'),
        // We always want to reset this field value, so we don't save the value.
        '#default_value' => '',
        '#states' => array(
          'visible' => array(
            ':input[name="mailbox[add_new_mailbox]"]' => array('checked' => TRUE),
          ),
        ),
      );
      $form['mailbox']['last_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Last name'),
        '#description' => t('The last name for the new mailbox account holder.'),
        // We always want to reset this field value, so we don't save the value.
        '#default_value' => '',
        '#states' => array(
          'visible' => array(
            ':input[name="mailbox[add_new_mailbox]"]' => array('checked' => TRUE),
          ),
        ),
      );

      // Additional options.
      $form['additional_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Additional options'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['additional_options']['batch_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Batch size'),
        '#default_value' => $this->config['additional_options']['batch_size'],
        '#description' => t('The maximum number of messages to fetch per request. The maximum limit is 100.'),
        '#size' => 5,
      );
      $form['additional_options']['include_body'] = array(
        '#type' => 'checkbox',
        '#title' => t('Import message bodies'),
        '#description' => t(
          "If checked message bodies will be imported. Since message bodies must be retrieved from the IMAP server,
          expect a performance hit when setting this parameter."
        ),
        '#default_value' => $this->config['additional_options']['include_body'],
      );
      $form['additional_options']['include_flags'] = array(
        '#type' => 'checkbox',
        '#title' => t('Import message flags'),
        '#description' => t(
          "If checked message flags will be imported. Since message flags must be retrieved from the IMAP server,
          expect a performance hit when setting this parameter."
        ),
        '#default_value' => $this->config['additional_options']['include_flags'],
      );

      // Filter options settings.
      $form['filter_options'] = array(
        '#type' => 'fieldset',
        '#title' => t('Filter options'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      $form['filter_options']['description'] = array(
        '#prefix' => '<div class="description">',
        '#markup' => t(
          "<p>Each of the <strong>To</strong>, <strong>From</strong>, <strong>CC</strong> and <strong>BCC</strong>
          filter options can be set to a comma-separated list of email addresses. These multiple addresses are treated
          as an OR combination.</p><p>You can set more than one filter option at once. Multiple filter options are
          treated as an AND combination.</p>"
        ),
        '#suffix' => '</div>',
      );
      $form['filter_options']['subject'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#default_value' => $this->config['filter_options']['subject'],
        '#description' => t("Get messages whose subject matches this search string. Regular expressions are allowed between / characters."),
      );
      $form['filter_options']['to'] = array(
        '#type' => 'textfield',
        '#title' => t('To'),
        '#default_value' => $this->config['filter_options']['to'],
        '#description' => t('Email address(es) of messages have been sent to.'),
      );
      $form['filter_options']['from'] = array(
        '#type' => 'textfield',
        '#title' => t('From'),
        '#default_value' => $this->config['filter_options']['from'],
        '#description' => t('Email address(es) of messages have been received from.'),
      );
      $form['filter_options']['cc'] = array(
        '#type' => 'textfield',
        '#title' => t('CC'),
        '#default_value' => $this->config['filter_options']['cc'],
        '#description' => t("Email address(es) of a contact CC'ed on the messages."),
      );
      $form['filter_options']['bcc'] = array(
        '#type' => 'textfield',
        '#title' => t('BCC'),
        '#default_value' => $this->config['filter_options']['bcc'],
        '#description' => t("Email address(es) of a contact BCC'ed on the messages."),
      );
      $form['filter_options']['folder'] = array(
        '#type' => 'textfield',
        '#title' => t('Folder'),
        '#default_value' => $this->config['filter_options']['folder'],
        '#description' => t(
          "Filter messages by the folder (or Gmail label). This parameter can be the complete folder name with the appropriate
          hierarchy delimiter for the mail server being queried (eg. Inbox/My folder) or the \"symbolic name\" of the folder
          (eg. \\Starred). The symbolic name refers to attributes used to refer to special use folders in a language-independent
          way. See !rfc_link.",
          array(
            '!rfc_link' => l(t('RFC-6154'), 'http://tools.ietf.org/html/rfc6154'),
          )
        ),
      );
      $date_before_empty = empty($this->config['filter_options']['date_before']);
      $form['filter_options']['date_before'] = array(
        '#type' => 'textfield',
        '#title' => t('Date before'),
        '#default_value' => $date_before_empty ? '' : date('Y-m-d H:i', $this->config['filter_options']['date_before']),
        '#size' => 15,
        '#description' => t('Only include messages after a given timestamp (eg. @date).', array(
          '@date' => date('Y-m-d H:i'),
        )),
      );
      $date_after_empty = empty($this->config['filter_options']['date_after']);
      $form['filter_options']['date_after'] = array(
        '#type' => 'textfield',
        '#title' => t('Date after'),
        '#default_value' => $date_after_empty ? '' : date('Y-m-d H:i', $this->config['filter_options']['date_after']),
        '#size' => 15,
        '#description' => t('Only include messages before a given timestamp (eg. @date).', array(
          '@date' => date('Y-m-d H:i'),
        )),
      );
    }
    else {
      $form['set_up_key_and_secret'] = array(
        '#prefix' => '<p>',
        '#markup' => t("To be able to import email messages from Context.IO at first !set_up_link your customer key and secret.",
          array(
            '!set_up_link' => l(t('set up'), 'admin/config/services/contextio', array(
              'attributes' => array(
                'target' => '_blank',
              ),
            )),
          )
        ),
        '#suffix' => '</p>',
      );
    }

    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if (!empty($values['mailbox'])) {
      if (!empty($values['mailbox']['email_address'])) {
        if (!empty($values['mailbox']['add_new_mailbox'])) {
          try {
            $params = array(
              'callback_url' => url(isset($_GET['q']) ? $_GET['q'] : '<front>', array('absolute' => TRUE)),
              'email' => $values['mailbox']['email_address'],
              'first_name' => $values['mailbox']['first_name'],
              'last_name' => $values['mailbox']['last_name'],
            );
            $connect_token = contextio_add_connect_token(NULL, $params);
            if (!empty($connect_token['success']) && !empty($connect_token['browser_redirect_url'])) {
              // Save browser_redirect_url into values for later usage.
              $values['mailbox']['browser_redirect_url'] = $connect_token['browser_redirect_url'];
            }
            else {
              form_set_error('mailbox][email_address', t('An error occurred during the connect token creation. Please try again.'));
              watchdog('contextio_feeds_plugins', 'Missing or FALSE success property after a contextio_add_connect_token() call.', array(), WATCHDOG_ERROR);
            }
          }
          catch (Exception $e) {
            form_set_error('mailbox][email_address', t('An error occurred during the connect token creation. Message: @message', array(
              '@message' => $e->getMessage(),
            )));
            watchdog('contextio_feeds_plugins', 'An error occurred during a contextio_add_connect_token() call. Error message: @message', array(
              '@message' => $e->getMessage(),
            ), WATCHDOG_ERROR);
          }
        }
        else {
          try {
            // Try to get account details and store its id for later usage.
            $accounts = contextio_accounts_list(array('email' => $values['mailbox']['email_address']));
            if (!empty($accounts[0]['id'])) {
              $values['mailbox']['account_id'] = $accounts[0]['id'];
            }
            else {
              form_set_error('mailbox][email_address', t('No account holder found for the provided mailbox email address. Please try again.'));
              watchdog('contextio_feeds_plugins', 'Missing account][0][id property after a contextio_accounts_list() call.', array(), WATCHDOG_ERROR);
            }
          }
          catch (Exception $e) {
            form_set_error('mailbox][email_address', t('An error occurred while trying to get the Context.IO accounts list. Message: @message', array(
              '@message' => $e->getMessage(),
            )));
            watchdog('contextio_feeds_plugins', 'An error occurred during a contextio_accounts_list() call. Error message: @message', array(
              '@message' => $e->getMessage(),
            ), WATCHDOG_ERROR);
          }
        }
      }

      $batch_size = $values['additional_options']['batch_size'];
      if (!(is_numeric($batch_size) && (((int) $batch_size >= 1) && ((int) $batch_size <= 100)))) {
        form_set_error('additional_options][batch_size', t('Batch size must be an integer value between 1 and 100.'));
      }

      // Check date field values' format.
      $field_names = array('date_before', 'date_after');
      foreach ($field_names as $field_name) {
        if (!empty($values['filter_options'][$field_name])) {
          $date = DateTime::createFromFormat('Y-m-d H:i', $values['filter_options'][$field_name]);
          if (!($date && $date->format('Y-m-d H:i') == $values['filter_options'][$field_name])) {
            form_set_error('filter_options][' . $field_name, t('The allowed date format is YY-MM-DD hh:mm'));
          }
        }
      }
    }
  }

  /**
   * Override parent::configFormSubmit().
   */
  public function configFormSubmit(&$values) {
    if (!empty($values['mailbox'])) {
      // Convert date values to timestamps before saving.
      $field_names = array('date_before', 'date_after');
      foreach ($field_names as $field_name) {
        if (!empty($values['filter_options'][$field_name])) {
          $timestamp = strtotime($values['filter_options'][$field_name]);
          $values['filter_options'][$field_name] = $timestamp;
        }
      }
      // Convert limit and offset values to integers.
      $values['additional_options']['batch_size'] = (int) $values['additional_options']['batch_size'];

      parent::configFormSubmit($values);

      // Redirect the user to the new mailbox setup.
      if (!empty($values['mailbox']['browser_redirect_url'])) {
        // We use drupal_goto() since there is no option here to use the more
        // appropriate $form_state['redirect'].
        drupal_goto($values['mailbox']['browser_redirect_url']);
      }
    }
  }

}
