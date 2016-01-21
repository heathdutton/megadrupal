<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorImageMissingStyle.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;

/**
 * Monitors image derivate creation errors from dblog.
 *
 * Displays image derivate with highest occurrence as message.
 */
class SensorImageMissingStyle extends SensorDatabaseAggregator {

  /**
   * The path of the most failed image.
   *
   * @var string
   */
  protected $sourceImagePath;

  /**
   * {@inheritdoc}
   */
  public function buildQuery() {
    // Extends the watchdog query.
    $query = parent::buildQuery();
    $query->addField('watchdog', 'variables');
    $query->groupBy('variables');
    $query->orderBy('records_count', 'DESC');
    return $query;
  }

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    parent::runSensor($result);
    $query_result = $this->fetchObject();
    if (!empty($query_result)) {
      $variables = unserialize($query_result->variables);
      if (isset($variables['%source_image_path'])) {
        $result->addStatusMessage($variables['%source_image_path']);
        $this->sourceImagePath = $variables['%source_image_path'];
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function resultVerbose(SensorResultInterface $result) {

    $verbose = parent::resultVerbose($result);

    // If non found, no reason to query file_managed table.
    if ($result->getValue() == 0) {
      return $verbose;
    }

    // In case we were not able to retrieve this info from the watchdog
    // variables.
    if (empty($this->sourceImagePath)) {
      $message = t('Source image path is empty, cannot query file_managed table');
    }

    $file = db_query('SELECT * FROM file_managed WHERE uri = :uri', array(':uri' => $this->sourceImagePath))->fetchObject();

    if (!empty($file)) {
      $message = t('File managed records: <pre>@file_managed</pre>', array('@file_managed' => var_export(file_usage_list($file), TRUE)));
    }

    if (empty($message)) {
      $message = t('File @file record not found in the file_managed table.', array('@file' => $result->getMessage()));
    }

    $verbose .=  ' ' . $message;

    return $verbose;
  }
}
