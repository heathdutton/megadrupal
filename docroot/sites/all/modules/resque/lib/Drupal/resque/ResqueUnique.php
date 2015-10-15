<?php
/**
 * @file
 * Contains Drupal\resque\Unique.
 */

namespace Drupal\resque;

use UniqueQueueInterface;
use Resque_Event;
use Resque as Php_Resque;

class ResqueUnique extends Resque implements UniqueQueueInterface {
  /**
   * {@inheritdoc}
   */
  public function createUniqueItem($key, $data) {
    $token = NULL;

    // Check to see if class name was specified in the data array.
    if (!empty($data['class_name'])) {
      $this->className = $data['class_name'];
    }
    $queues = module_invoke_all('cron_queue_info');
    drupal_alter('cron_queue_info', $queues);

    // Add the worker callback.
    $data['worker_callback'] = $queues[$this->name]['worker callback'];
    $data['drupal_unique_key'] = $this->name . ':' . $key;
    Resque_Event::listen(
      'beforeEnqueue',
      array('Drupal\resque\UniquePlugin', 'beforeEnqueue')
    );
    $token = Php_Resque::enqueue($this->name, $this->className, $data, TRUE);

    return $token;
  }
}
