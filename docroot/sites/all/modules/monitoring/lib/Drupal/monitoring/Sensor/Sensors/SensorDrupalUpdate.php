<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorDrupalUpdate.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\Sensor;

/**
 * Monitors for available updates of Drupal core and installed contrib modules.
 *
 * Based on drupal core update module.
 */
class SensorDrupalUpdate extends Sensor {

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    $type = $this->info->getSetting('type');
    $status = $this->calculateStatus($type);

    $result->setStatus($status);

    $available = update_get_available();
    $project_data = update_calculate_project_data($available);

    if ($type == 'core') {
      $this->checkCore($result, $project_data);
    }
    else {
      $this->checkContrib($result, $project_data);
    }
  }

  /**
   * Checks core status and sets sensor status message.
   *
   * @param SensorResultInterface $result
   * @param array $project_data
   */
  protected function checkCore(SensorResultInterface $result, $project_data) {
    $info = $project_data['drupal'];
    $status = $this->getStatusText($info['status']);

    if ($status == 'unknown') {
      $result->addStatusMessage('Core update status unknown');
      // Do not escalate in case the status is unknown.
      $result->setStatus(SensorResultInterface::STATUS_INFO);
    }
    elseif ($status == 'current') {
      $result->addStatusMessage('Core up to date');
    }
    else {
      $result->addStatusMessage('Core (@current) - @status - latest @latest', array(
        '@status' => $status,
        '@current' => isset($info['existing_version']) ? $info['existing_version'] : NULL,
        '@latest' => isset($info['latest_version']) ? $info['latest_version'] : NULL,
      ));
    }
  }

  /**
   * Checks contrib status and sets sensor status message.
   *
   * @param SensorResultInterface $result
   * @param array $project_data
   */
  protected function checkContrib(SensorResultInterface $result, $project_data) {

    unset($project_data['drupal']);
    $uptodate = TRUE;

    foreach ($project_data as $info) {
      // Skip in case the status is current or unknown.
      if ($info['status'] == UPDATE_CURRENT || $info['status'] == UPDATE_UNKNOWN) {
        continue;
      }

      $uptodate = FALSE;

      $status = $this->getStatusText($info['status']);

      if ($status == 'unknown') {
        $result->addStatusMessage('Module @module (@current) - no releases found', array(
          '@module' => $info['info']['name'],
          '@current' => isset($info['existing_version']) ? $info['existing_version'] : NULL,
        ));
      }
      else {
        $result->addStatusMessage('Module @module (@current) - @status - recommended @recommended - latest @latest', array(
          '@module' => $info['info']['name'],
          '@status' => $status,
          '@current' => isset($info['existing_version']) ? $info['existing_version'] : NULL,
          '@recommended' => isset($info['recommended']) ? $info['recommended'] : NULL,
          '@latest' => isset($info['latest_version']) ? $info['latest_version'] : NULL,
        ));
      }
    }

    if ($uptodate) {
      $result->addStatusMessage('All modules up to date');
    }
  }

  /**
   * Gets status text.
   *
   * @param int $status
   *   One of UPDATE_* constants.
   *
   * @return string
   *   Status text.
   */
  protected function getStatusText($status) {
    switch ($status) {
      case UPDATE_NOT_SECURE:
        return 'NOT SECURE';
        break;

      case UPDATE_CURRENT:
        return 'current';
        break;

      case UPDATE_REVOKED:
        return 'version revoked';
        break;

      case UPDATE_NOT_SUPPORTED:
        return 'not supported';
        break;

      case UPDATE_NOT_CURRENT:
        return 'update available';
        break;

      case UPDATE_UNKNOWN:
      case UPDATE_NOT_CHECKED:
      case UPDATE_NOT_FETCHED:
      case UPDATE_FETCH_PENDING:
        return 'unknown';
        break;
    }
  }

  /**
   * Executes the update requirements hook and calculates the status for it.
   *
   * @param string $type
   *   Which types of updates to check for, core or contrib.
   *
   * @return string
   *   One of the SensorResultInterface status constants.
   */
  protected function calculateStatus($type) {
    module_load_include('install', 'update');

    $requirements = update_requirements('runtime');

    $update_info = array();
    if (isset($requirements['update_' . $type])) {
      $update_info = $requirements['update_' . $type];
    }
    $update_info += array(
      'severity' => REQUIREMENT_OK,
    );

    if ($update_info['severity'] == REQUIREMENT_OK) {
      return SensorResultInterface::STATUS_OK;
    }
    elseif ($update_info['severity'] == REQUIREMENT_INFO) {
      return SensorResultInterface::STATUS_INFO;
    }
    // If the level is warning, which is updates available, we do not need to
    // escalate.
    elseif ($update_info['severity'] == REQUIREMENT_WARNING) {
      return SensorResultInterface::STATUS_INFO;
    }
    else {
      return SensorResultInterface::STATUS_CRITICAL;
    }
  }

}
