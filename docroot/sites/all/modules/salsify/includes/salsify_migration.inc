<?php

/**
 * @file
 * includes/salsify_migration.inc
 */

/**
 * Parent class for Salsify migrations.
 */
class SalsifyMigration extends Migration {

  /**
   * URL for the Salsify API.
   */
  const BASE_URL = 'https://app.salsify.com/api';

  /**
   * {@inheritdoc}
   */
  protected $highwaterField = array('name' => 'salsify:updated_at');

  /**
   * ID for cached record counts.
   *
   * @var string
   */
  protected $cacheKey;

  /**
   * {@inheritdoc}
   */
  public function __construct($arguments) {
    parent::__construct($arguments);
    $this->cacheKey = "salsify:count:$this->machineName";
  }

  /**
   * {@inheritdoc}
   */
  protected function import() {
    $ready = TRUE;
    // Check for expired or incomplete channel runs.
    foreach ($this->arguments['channels'] as $channel_id => $run) {
      // Replace expired channel runs.
      // @todo: When Salsify gets incremental updates working, add updates as
      // additional URLs.
      if (strtotime($run['started_at']) < REQUEST_TIME - $this->arguments['expiration']) {
        // To create a channel run programmatically, HTTP POST an empty JSON
        // document to: https://app.salsify.com/api/channels/CHANNEL_ID/runs
        // where you would replace CHANNEL_ID with the ID you got from the
        // Salsify app. This call will return a JSON document that contains
        // a channel run ID along with addition information about the
        // channel run. You'll need this channel run ID to poll for status.
        $result = drupal_http_request(url(SalsifyMigrateWizard::BASE_URL . "/channels/$channel_id/runs", array(
          'query' => array(
            'auth_token' => $this->arguments['api_key'],
          ),
        )), array(
          'method' => 'POST',
          'data' => drupal_json_encode(''),
        ));
        if (isset($result->headers['content-type']) && trim(reset(explode(';', $result->headers['content-type'], 2))) == 'application/json') {
          $run = drupal_json_decode($result->data);
          if (isset($run['id']) && !isset($run['error'])) {
            $this->arguments['channels'][$channel_id] = $run;
            cache_set("salsify:channel_run:$channel_id", $run['id'], 'cache', REQUEST_TIME + $this->arguments['expiration']);
          }
        }
        self::displayMessage(t('Replaced expired run for channel %id.', array('%id' => $channel_id)), 'status');
        $ready = FALSE;
        break;
      }
      elseif ($run['status'] != 'completed') {
        $result = drupal_http_request(url(self::BASE_URL . "/channels/$channel_id/runs/$run[id]", array(
          'query' => array(
            'auth_token' => $this->arguments['api_key'],
          ),
        )));
        if (isset($result->headers['content-type']) && trim(reset(explode(';', $result->headers['content-type'], 2))) == 'application/json') {
          $run = drupal_json_decode($result->data);
          if (!isset($run['error'])) {
            $this->arguments['channels'][$channel_id] = $run;
          }
        }
        if ($run['status'] == 'completed') {
          self::displayMessage(t('New run for channel %id is complete.', array('%id' => $channel_id)), 'status');
        }
        else {
          self::displayMessage(t('Checked status of new run for channel %id.', array('%id' => $channel_id)), 'status');
        }
        $ready = FALSE;
        break;
      }
    }
    if ($ready) {
      $return = parent::import();
    }
    else {
      // Save channel runs.
      $arguments = $this->group->getArguments();
      $arguments['channels'] = $this->arguments['channels'];
      MigrateGroup::register($this->group->getName(), $this->group->getTitle(), $arguments);
      // Expire cached record counts.
      foreach (migrate_migrations() as $migration) {
        if ($this->group->getName() == $migration->getGroup()->getName()) {
          cache_clear_all($this->cacheKey, 'cache');
        }
      }
      $return = MigrationBase::RESULT_INCOMPLETE;
    }
    return $return;
  }

  /**
   * Describe special attributes which don't appear in the main attributes list.
   *
   * @return array
   *   A list of descriptions keyed by attribute.
   */
  static public function salsifyAttributes() {
    return array(
      'salsify:created_at' => t('Creation time'),
      'salsify:updated_at' => t('Last update time'),
    );
  }

}

/**
 * Get product IDs from destination values.
 *
 * @param array $destination_values
 *   If there are multiple values, destination values are arrays whose first
 *   value is the product ID and whose second value is the revision ID.
 *
 * @return array
 *   The first value of each destination value.
 */
function salsify_product_ids($destination_values) {
  $return = reset($destination_values);
  if (is_array($return)) {
    $return = array_map('reset', $destination_values);
  }
  return (array) $return;
}
