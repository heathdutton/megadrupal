<?php

/**
 * @file
 * Please supply a file description.
 */

/**
 * @file
 * Provides Gengo translation plugin controller.
 */
class TMGMTMyGengoTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Implements TMGMTTranslatorUIControllerInterface::reviewDataItemElement().
   */
  public function reviewDataItemElement($form, &$form_state, $data_item_key, $parent_key, array $data_item, TMGMTJobItem $item) {

    /** @var TMGMTRemote $mapping */
    $mapping = NULL;
    foreach ($item->getRemoteMappings() as $value) {
      if ($value->data_item_key == $data_item_key) {
        $mapping = $value;
        break;
      }
    }

    // If no mapping found or mapping is not yet complete return.
    if (empty($mapping) || empty($mapping->remote_identifier_2)) {
      return $form;
    }

    $gengo_job_id = $mapping->remote_identifier_2;
    $target_key = str_replace('][', '|', $data_item_key);

    // For state pending we need to check also for type as empty value will
    // evaluate as 0.
    if ($data_item['#status'] == TMGMT_DATA_ITEM_STATE_REVIEWED || $data_item['#status'] === TMGMT_DATA_ITEM_STATE_PENDING) {
      $form['actions'][$gengo_job_id . '_comment_form'] = array(
        '#type' => 'submit',
        '#value' => '✉',
        '#attributes' => array('title' => t('Add new comment'), 'class' => array($gengo_job_id . '-gengo-id', 'new-comment-button', 'gengo-button')),
        '#submit' => array('tmgmt_mygengo_gengo_action_form_submit'),
        '#name' => $gengo_job_id . '_comment_form',
        '#gengo_action' => 'comment',
        '#gengo_job_id' => $gengo_job_id,
        '#target_key' => $target_key,
        '#ajax' => array(
          'callback' => 'tmgmt_mygengo_review_form_input_pane_ajax',
          'wrapper' => $gengo_job_id . '-input-wrapper',
        ),
      );
    }

    if ($data_item['#status'] == TMGMT_DATA_ITEM_STATE_TRANSLATED) {
      $form['actions'][$gengo_job_id . '_revision_form'] = array(
        '#type' => 'submit',
        '#value' => '✍',
        '#name' => $gengo_job_id . '_revision_form',
        '#attributes' => array('title' => t('Request new revision'), 'class' => array($gengo_job_id . '-gengo-id', 'request-revision-button', 'gengo-button')),
        '#submit' => array('tmgmt_mygengo_gengo_action_form_submit'),
        '#gengo_action' => 'revision',
        '#gengo_job_id' => $gengo_job_id,
        '#target_key' => $target_key,
        '#ajax' => array(
          'callback' => 'tmgmt_mygengo_review_form_input_pane_ajax',
          'wrapper' => $gengo_job_id . '-input-wrapper',
        ),
      );
    }

    $form['below'][$gengo_job_id . '_gengo'] = array(
      '#type' => 'fieldset',
      '#title' => t('Comments for gengo job #%gengo_job_id', array('%gengo_job_id' => $gengo_job_id)),
      '#prefix' => '<div class="gengo-pane" id="' . $gengo_job_id . '-gengo-pane">',
      '#suffix' => '</div>',
    );

    // Input pane.
    $form['below'][$gengo_job_id . '_gengo']['input_wrapper'] = array(
      '#prefix' => '<div class="input-wrapper" id="' . $gengo_job_id . '-input-wrapper">',
      '#suffix' => '</div>',
      '#tree' => FALSE,
    );

    if (!empty($form_state['active_gengo_job_id']) && $form_state['active_gengo_job_id'] == $gengo_job_id) {
      $form['below'][$gengo_job_id . '_gengo']['input_wrapper'] +=
          $this->getCommentForm(tmgmt_ui_review_form_element_ajaxid($parent_key), $gengo_job_id, $form_state['gengo_action'], $mapping);
    }
    // Input pane end.

    // Comments pane.
    $form['below'][$gengo_job_id . '_gengo']['comments_wrapper'] = array(
      '#prefix' => '<div class="comments-wrapper" id="' . $gengo_job_id . '-comments-wrapper">',
      '#suffix' => '</div>',
    );

    $thread = $this->fetchComments($item->getTranslator(), $gengo_job_id, !empty($form_state['submitted_gengo_action']));

    $form['below'][$gengo_job_id . '_gengo']['comments_wrapper']['comments'] = array(
      '#type' => 'markup',
      '#markup' => t('There are no comments for this item yet.'),
      '#tree' => FALSE,
    );

    if (!empty($thread)) {
      $form['below'][$gengo_job_id . '_gengo']['comments_wrapper']['comments']['#markup'] =
          theme('tmgmt_mygengo_comments_thread', array('thread' => $thread, 'gengo_job_id' => $gengo_job_id));
    }

    if (!empty($form_state['submitted_gengo_action'])) {
      $form['below'][$gengo_job_id . '_gengo']['comments_wrapper']['submitted_comment_gengo_id'] = array(
        '#type' => 'hidden',
        '#value' => $gengo_job_id,
      );
      $form['below'][$gengo_job_id . '_gengo']['comments_wrapper']['submitted_gengo_action'] = array(
        '#type' => 'hidden',
        '#value' => $form_state['submitted_gengo_action'],
      );
    }
    // Comments pane end.

    return $form;
  }

  /**
   * Implements TMGMTTranslatorUIControllerInterface::reviewForm().
   */
  public function reviewForm($form, &$form_state, TMGMTJobItem $item) {

    $form['#attached']['js'][] = drupal_get_path('module', 'tmgmt_mygengo') . '/js/tmgmt_mygengo_comments.js';
    $form['#attached']['css'][] = drupal_get_path('module', 'tmgmt_mygengo') . '/css/tmgmt_mygengo_comments.css';

    return $form;
  }

  /**
   * Builds form comment form based on the action.
   *
   * @param string $ajax_id
   *   For ajax id.
   * @param int $gengo_job_id
   *   Gengo job id.
   * @param $action
   *   Action requested by user (comment, revision)
   * @param object $mapping
   *   Data item mapping object.
   *
   * @return array
   *   Built form array.
   */
  protected function getCommentForm($ajax_id, $gengo_job_id, $action, $mapping) {

    $target_key = str_replace('][', '|', $mapping->data_item_key);

    $submit_base = array(
      '#type' => 'submit',
      '#name' => $gengo_job_id . '_submit',
      '#gengo_job_id' => $gengo_job_id,
      '#target_key' => $target_key,
      '#ajax' => array(
        'callback' => 'tmgmt_ui_translation_review_form_ajax',
        'wrapper' => $ajax_id,
      ),
    );

    if ($action == 'revision') {
      $form[$gengo_job_id . '_comment'] = array(
        '#type' => 'textarea',
        '#title' => t('Revision comment'),
        '#description' => t('Provide instructions for the translator.'),
      );
      $form[$gengo_job_id . '_data_item_key'] = array(
        '#type' => 'value',
        '#value' => $mapping->data_item_key,
      );

      $form[$gengo_job_id . '_submit_revision'] = $submit_base + array(
        '#value' => t('Request revision'),
        // Using same validator as for comment.
        '#validate' => array('tmgmt_mygengo_add_comment_form_validate'),
        '#submit' => array('tmgmt_mygengo_add_revision_form_submit'),
      );
    }
    elseif ($action == 'comment') {
      $form[$gengo_job_id . '_comment'] = array(
        '#type' => 'textarea',
        '#title' => t('New comment'),
      );

      $form[$gengo_job_id . '_submit_comment'] = $submit_base + array(
        '#value' => t('Submit comment'),
        '#validate' => array('tmgmt_mygengo_add_comment_form_validate'),
        '#submit' => array('tmgmt_mygengo_add_comment_form_submit'),
      );
    }

    $form[$gengo_job_id . '_cancel'] = array(
      '#type' => 'submit',
      '#name' => $gengo_job_id . '_cancel',
      '#value' => t('Cancel'),
      '#attributes' => array('class' => array( $gengo_job_id . '-gengo-id', 'cancel-comment-button')),
      '#submit' => array('tmgmt_mygengo_gengo_cancel_form_submit'),
      '#gengo_action' => '',
      '#gengo_job_id' => $gengo_job_id,
      '#target_key' => $target_key,
      '#ajax' => array(
        'callback' => 'tmgmt_mygengo_review_form_input_pane_ajax',
        'wrapper' => $gengo_job_id . '-input-wrapper',
      ),
    );

    return $form;
  }

  /**
   * Overrides TMGMTDefaultTranslatorUIController::pluginSettingsForm().
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {

    $form['api_public_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Gengo API Public key'),
      '#default_value' => $translator->getSetting('api_public_key'),
      '#description' => t('Please enter your Gengo API Public key.'),
    );
    $form['api_private_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Gengo API Private key'),
      '#default_value' => $translator->getSetting('api_private_key'),
      '#description' => t('Please enter your Gengo API Private key.'),
    );
    $form['mygengo_auto_approve'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically approve jobs at Gengo side.'),
      '#default_value' => $translator->getSetting('mygengo_auto_approve'),
      '#description' => t('Check to auto approve translated jobs from Gengo. This setting will skip the review process and automatically mark jobs at Gengo side as approved by you.'),
      '#prefix' => '<div class="mygengo-auto-approve">',
      '#suffix' => '</div>',
    );
    $form['use_sandbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use the sandbox'),
      '#default_value' => $translator->getSetting('use_sandbox'),
      '#description' => t('Check to use the testing environment.'),
      '#prefix' => '<div class="mygengo-use-sandbox">',
      '#suffix' => '</div>',
    );
    $form['use_preferred'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use preferred translators'),
      '#default_value' => $translator->getSetting('use_preferred'),
      '#description' => t('Check to use translators from the preferred translators list associated with your Gengo account.'),
    );
    $form['show_remaining_credits_info'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show remaining credit info'),
      '#default_value' => $translator->getSetting('show_remaining_credits_info'),
      '#description' => t('Check to display remaining Gengo credit at the job checkout page.'),
    );

    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

  /**
   * Overrides TMGMTDefaultTranslatorUIController::checkoutSettingsForm().
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    $translator = $job->getTranslator();

    // Set the quality setting from submitted vals - we need this for quote as
    // repetitive change of Quality select will not update the job settings.
    if (isset($form_state['values']['settings']['quality'])) {
      $job->settings['quality'] = $form_state['values']['settings']['quality'];
    }

    // In case quality has not been set yet, init it to default.
    if (empty($job->settings['quality'])) {
      $job->settings['quality'] = 'standard';
    }

    $settings['quality'] = array(
      '#type' => 'select',
      '#title' => t('Quality'),
      '#options' => $this->getAvailableTiersOptions($job),
      '#default_value' => $job->settings['quality'],
      '#description' => t('Choose the level of quality for this translation job.'),
      '#ajax' => array(
        'callback' => 'tmgmt_ui_ajax_callback_translator_select',
        'wrapper' => 'tmgmt-ui-translator-settings',
      ),
    );

    if ($job->settings['quality'] == 'machine') {
      return $settings;
    }

    $quote = $this->getQuoteInfo($job);
    $credit_info = $this->getRemainingCreditInfo($translator);

    $settings['quote'] = array(
      '#type' => 'fieldset',
      '#title' => t('Quote'),
    );

    $settings['quote']['word_count'] = array(
      '#type' => 'item',
      '#title' => t('Word count'),
      '#markup' => isset($quote['sum_word_count']) ? $quote['sum_word_count'] : t('unknown'),
    );
    $settings['quote']['needed_credits'] = array(
      '#type' => 'item',
      '#title' => t('Needed Credits'),
      '#markup' => number_format($quote['sum_credits'], 2) . ' ' . $quote['currency'],
    );
    if ($translator->getSetting('show_remaining_credits_info')) {
      $settings['quote']['remaining_credits'] = array(
        '#type' => 'item',
        '#title' => t('Remaining Credits'),
        '#markup' => $credit_info['credits'] . ' ' . $credit_info['currency'],
      );
    }
    $settings['quote']['eta'] = array(
      '#type' => 'item',
      '#title' => t('ETA'),
      '#markup' => format_date(time() + $quote['highest_eta'], "long"),
    );

    $settings['comment'] = array(
      '#type' => 'textarea',
      '#title' => t('Instructions'),
      '#description' => t('You can provide a set of instructions so that the translator will better understand your requirements.'),
      '#default_value' => !empty($job->settings['comment']) ? $job->settings['comment'] : NULL,
    );

    return $settings;
  }

  /**
   * Implements TMGMTTranslatorUIControllerInterface::checkoutInfo().
   */
  public function checkoutInfo(TMGMTJob $job) {
    $form = array();

    if ($job->isActive()) {
      $form['actions']['poll'] = array(
        '#type' => 'submit',
        '#value' => t('Poll translations'),
        '#submit' => array('_tmgmt_mygengo_poll_submit'),
        '#weight' => -10,
      );
    }

    return $form;
  }

  /**
   * Get a quote from Gengo for the given job.
   *
   * @param TMGMTJob $job
   *   Job for which to get a quote.
   *
   * @return array
   *   Array with the following keys: currency, estimates, highest_eta,
   *   sum_credits, sum_eta, sum_word_count, and unit_price.
   * @throws TMGMTException
   *   In case of error doing request to gengo service.
   */
  protected function getQuoteInfo(TMGMTJob $job) {
    $response = NULL;
    /* @var TMGMTMyGengoTranslatorPluginController $plugin */
    $plugin = $job->getTranslatorController();

    try {
      $response = $plugin->sendJob($job, TRUE);
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt_mygengo', $e);
      drupal_set_message($e->getMessage(), 'error');
    }

    // Setup empty values
    $quote = array(
      'currency' => '',
      'estimated' => FALSE,
      'highest_eta' => 0,
      'sum_credits' => 0,
      'sum_word_count' => 0,
    );

    if (!empty($response->jobs)) {
      $jobs = (array) $response->jobs;

      $quote['currency'] = reset($jobs)->currency;

      // Sum up quotes from each job.
      foreach ($response->jobs as $job) {
        $quote['sum_word_count'] += $job->unit_count;
        $quote['sum_credits'] += $job->credits;

        if ($job->eta > $quote['highest_eta']) {
          $quote['highest_eta'] = $job->eta;
        }
      }
    }

    return $quote;
  }

  /**
   * Gets remaining credit info at gengo account.
   *
   * @param TMGMTTranslator $translator
   *   Translator.
   *
   * @return array
   *   Associative array of currency and credits.
   */
  protected function getRemainingCreditInfo(TMGMTTranslator $translator) {
    $connector = new TMGMTGengoConnector($translator);
    $credit_info = array(
      'credits' => NULL,
      'currency' => NULL,
    );
    try {
      $response = $connector->getRemainingCredit();
      $credit_info['credits'] = $response->credits;
      $credit_info['currency'] = $response->currency;
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt_mygengo', $e);
      drupal_set_message($e->getMessage(), 'error');
    }

    return $credit_info;
  }

  /**
   * Builds quality/tier options for src/tgt language pair of the job.
   *
   * @param TMGMTJob $job
   *   Translation job.
   *
   * @return array
   *   Associative array of tiers with info.
   */
  protected function getAvailableTiersOptions(TMGMTJob $job) {

    $translator = $job->getTranslator();

    $tier_names = array(
      'machine' => t('Machine'),
      'standard' => t('Standard'),
      'pro' => t('Business'),
      'ultra' => t('Ultra'),
      'nonprofit' => t('Nonprofit'),
    );

    $available_tiers = array();
    // Machine translation is always available.
    $available_tiers['machine'] = $tier_names['machine'];

    $connector = new TMGMTGengoConnector($translator);
    $gengo_language_pairs = array();

    try {
      $gengo_language_pairs = $connector->getLanguagePairs($translator);
    }
    catch (TMGMTException $e) {
      watchdog_exception('tmgmt_mygengo', $e);
      drupal_set_message($e->getMessage(), 'error');
    }

    foreach ($gengo_language_pairs as $tier) {
      // Skip if for other language pairs.
      if ($tier->lc_src != $translator->mapToRemoteLanguage($job->source_language) || $tier->lc_tgt != $translator->mapToRemoteLanguage($job->target_language)) {
        continue;
      }

      $available_tiers[$tier->tier] = t('@tier (@cost @currency per word)', array(
        '@tier' => empty($tier_names[$tier->tier]) ? $tier->tier : $tier_names[$tier->tier],
        '@cost' => number_format($tier->unit_price, 2),
        '@currency' => $tier->currency,
      ));
    }

    // @todo Gengo service does not support ultra quality for grouped jobs
    // and we send all jobs as grouped. We need to wait until gengo will support
    // grouped jobs for ultra quality as well.
    if (isset($available_tiers['ultra'])) {
      unset($available_tiers['ultra']);
    }

    return $available_tiers;
  }

  /**
   * Fetches comments from gengo service.
   *
   * @param TMGMTTranslator $translator
   *   Translator plugin.
   * @param $gengo_job_id
   *   Gengo job id for which to fetch comments.
   * @param boolean $reload
   *   Flag to reload cache.
   *
   * @return array
   */
  function fetchComments(TMGMTTranslator $translator, $gengo_job_id, $reload = FALSE) {

    $cid = 'tmgmt_mygengo_comments_' . $gengo_job_id;
    $cache = cache_get($cid, 'cache_tmgmt');

    if (isset($cache->data) && !$reload && $cache->expire > REQUEST_TIME) {
      return $cache->data;
    }

    $connector = new TMGMTGengoConnector($translator);
    $response = NULL;

    try {
      $response = $connector->getComments($gengo_job_id);

      $data = isset($response->thread) ? $response->thread : NULL;
      cache_set($cid, $data, 'cache_tmgmt', REQUEST_TIME + TMGMT_MYGENGO_COMMENTS_CACHE_EXPIRE);
      return $data;
    }
    catch (TMGMTException $e) {
      drupal_set_message($e->getMessage(), 'error');
      watchdog_exception('tmgmt_mygengo', $e);
      return array();
    }
  }
}
