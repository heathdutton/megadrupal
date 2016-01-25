<?php

/**
 * Interface DropSharkCollector.
 */
interface DropSharkCollector {

  const RESPONSE_NONE = 0;
  const RESPONSE_COMPLETE = 1;
  const RESPONSE_DEFERRED = 2;

  const STATUS_SUCCESS = 0;

  /**
   * DropSharkCollector constructor.
   *
   * @param array $definition
   *   Collector definition obtained from hook_dropshark_collector_info().
   */
  public function __construct($definition);

  /**
   * Collect data.
   *
   * @param string $event
   *   A value indicating an event or type of an event to which the collector
   *   may respond.
   * @param array $data
   *   Optional data necessary for the collector to perform its collection. This
   *   is likely information to indicate which server on which a collection is
   *   being ran, or to indicate which instance of a collection is being ran.
   *
   * @return int
   *   A value representing the state of the check.
   *
   *     - DropSharkCollector::RESPONSE_NONE - No collection occurred.
   *     - DropSharkCollector::RESPONSE_COMPLETE - Collection complete.
   *     - DropSharkCollector::RESPONSE_DEFERRED - Some or all of the processing
   *         has been deferred until the end of the request.
   */
  public function collect($event, $data = array());

  /**
   * Perform any processing upon finalizing the page request.
   *
   * This is generally used for computing metrics after potentially many changes
   * occurred, in order to avoid processing on each change. For instance,
   * counting the number of users per role after hundreds of users were granted
   * the role.
   */
  public function finalize();

}
