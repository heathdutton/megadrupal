<?php

/**
 * @file
 * Contains \Drupal\queue_unique\Queue\QueueUISystemSetQueue
 */

namespace Drupal\queue_unique\Queue;

class QueueUISystemSetQueue extends \QueueUISystemQueue implements \QueueUIInterface {

  /**
   * The table name.
   *
   * @var string
   */
  protected $tableName = 'queue_unique';

}
