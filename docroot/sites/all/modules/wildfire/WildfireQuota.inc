<?php
/**
 * @file
 * A quota object for storing a Wildfire system's quota.
 */

class WildfireQuota {

  /**
   * @var int
   *   The number of topup sends remaining.
   */
  protected $topup;

  /**
   * @var int
   *   The number of monthly sends remaining.
   */
  protected $monthly;

  /**
   * @var int
   *   UNIX timestamp for when these data were retrieved from the server.
   */
  protected $lastChecked;

  /**
   * Constructor.
   *
   * @param bool $refresh
   *   If TRUE, will automatically refresh the data from the WildfireHQ server.
   *   If FALSE, default values will be used and WildfireQuota::refresh() should
   *   be called to retrieve these data.
   */
  public function __construct($refresh = TRUE) {
    $this->topup = 0;
    $this->monthly = 0;
    $this->lastChecked = 0;

    if ($refresh) {
      $this->refresh();
    }
  }

  /**
   * Get the number of topup sends remaining.
   *
   * @return int
   *   The number of topup sends remaining.
   */
  public function getTopup() {
    return $this->topup;
  }

  /**
   * Get the number of monthly sends remaining.
   *
   * @return int
   *   The number of monthly sends remaining.
   */
  public function getMonthly() {
    return $this->monthly;
  }

  /**
   * Get the total number of sends remaining.
   *
   * @return int
   *   The total number of sends remaining. This is the sum of the topup sends
   *   and the monthly sends.
   */
  public function getTotal() {
    return $this->topup + $this->monthly;
  }

  /**
   * Get the time that the data were last retrieved from the WildfireHQ server.
   *
   * @return int
   *   UNIX timestamp for when the data were last retrieved.
   */
  public function getLastChecked() {
    return $this->lastChecked;
  }

  /**
   * Refresh this object with fresh data from the WildfireHQ server.
   */
  public function refresh() {
    if (module_exists('wildfire_rpc_jobs')) {
      try {
        module_load_include('inc', 'wildfire', 'wildfire.rpc');
        $quota = wildfire_rpc_send_request(
          'wildfire.system.getQuota',
          // 5sec timeout is *MORE* than enough to get this data.
          array('timeout' => 5,)
        );

        if (!empty($quota['error'])) {
          drupal_set_message(
            t('The quota could not be obtained at this time.')
          );
        }
        else {
          $this->topup = $quota['top-up'];
          $this->monthly = $quota['monthly'];
        }

        // Update the lastChecked timestamp.
        $this->lastChecked = REQUEST_TIME;

      }
      catch (Exception $e) {
        drupal_set_message(
          t(
            'Checking quota raised an error: !message. Please check that you have entered a valid auth key and try again.',
            array(
              '!message' => $e->getMessage(),
            )
          )
        );
      }

    }
  }
}
