<?php

/**
 * Contains CToolsTokenAccess.
 */

class CToolsExpirableTokenAccess extends CToolsTokenAccess {

  /**
   * Expiration date.
   *
   * Name of the URL/POST parameter to check.
   *
   * @var string
   */
  protected $expires_in;

  /**
   * {@inheritdoc}
   */
  public function __construct($conf, $context) {
    parent::__construct($conf, $context);
    $this->expires_in = empty($conf['expires_in']) ? '' : $conf['expires_in'];
  }

  /**
   * {@inheritdoc}
   */
  public function access() {
    // Check expiration.
    try {
      $now = new \DateTime();
      if ($now > $this->getExpirationObject()) {
        // The token is expired.
        return FALSE;
      }
    }
    catch (Exception $e) {
      // If an expiration date cannot be determined, then treat the token as
      // expired.
      return FALSE;
    }
    return parent::access();
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm() {
    $form = parent::settingsForm();
    $form['settings']['expires_in'] = array(
      '#type' => 'interval',
      '#title' => t('Expires in'),
      '#description' => t('Use a format procesable by date. Ex: "tomorrow", "+1 month", etc.'),
      '#default_value' => $this->expires_in,
      '#required' => TRUE,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $summary = parent::settingsSummary();
    $summary .= ' ' . t('Expires in: @date', array(
      '@date' => $this->getExpirationObject()->format('M d Y, h:i'),
    ));
    return $summary;
  }

  /**
   * Helper method to get the expiration \DateTime object.
   *
   * @return \DateTime
   *   The object for the expiration.
   */
  protected function getExpirationObject() {
    ctools_include('export');
    $record = ctools_export_crud_load('access_token_export', $this->variable_name);

    $expiration = new \DateTime();
    $expiration->setTimestamp($record->updated);
    interval_apply_interval($expiration, $this->expires_in);
    return $expiration;
  }

}
