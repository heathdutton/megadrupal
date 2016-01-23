<?php

/**
 * @file
 * Handles time based access restriction for group content bundles.
 */

/**
 * The main interface that all handlers have to implement.
 */
interface OgTimeframeHandlerInterface {

  /**
   * Boolean whether group content is open for editing or not.
   *
   * @param object $group
   *   The group entity.
   * @param mixed $node
   *   Optional group content or type.
   * @param string $op
   *   Requested operation. 'create' or 'update'.
   */
  public function isOpen($group, $node, $op);

}

/**
 * Base class implementing OgTimeframeHandlerInterface.
 *
 * This class is not useful at all, therefore the abstract.
 */
abstract class OgTimeframeHandler implements OgTimeframeHandlerInterface {

  /**
   * The group content entity type this handler is a restriction for.
   *
   * @var string
   */
  protected $groupContentType = '';

  /**
   * The group content entity bundle this handler is a restriction for.
   *
   * @var string
   */
  protected $groupContentBundle = '';

  /**
   * The group entity type.
   *
   * @var string
   */
  protected $groupType = '';

  /**
   * The group entity bundle.
   *
   * @var string
   */
  protected $groupBundle = '';

  /**
   * The group entity.
   *
   * @var object
   */
  protected $group = NULL;

  /**
   * The group entity ID.
   *
   * @var int
   */
  protected $groupId;

  /**
   * The collected from date.
   *
   * @var DateObject
   */
  protected $fromDate = NULL;

  /**
   * The collected from date.
   *
   * @var DateObject
   */
  protected $toDate = NULL;

  /**
   * Date precision to measure dates againts.
   *
   * @var string
   */
  protected $measure = 'seconds';

  /**
   * Constructor.
   *
   * @param array $info
   *   The same info provided in hook_og_timeframe_handler_info() as properties.
   */
  public function __construct(array $info) {
    $this->groupType          = $info['group_type'];
    $this->groupBundle        = $info['group_bundle'];
    $this->groupContentType   = $info['group_content_type'];
    $this->groupContentBundle = $info['group_content_bundle'];
  }

  /**
   * {@inheritdoc}
   */
  public function isOpen($group, $node, $op) {
    $this->setGroup($group);

    // Respect date field's granularity by comparing using the smallest possible
    // precision. This means if you set the finest precision to 'days' you will
    // have write access to the content throughout the day. This is the expected
    // behavior when something is "due september 3": you are able to update
    // content till midnight that day.
    $measure = $this->getMeasure();
    $now = new DateObject();
    $from = $this->getFromDate();
    $to = $this->getToDate();
    if ($from and $from->difference($now, $measure, FALSE) < 0) {
      return FALSE;
    }
    if ($to and $now->difference($to, $measure, FALSE) < 0) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Set the group context.
   *
   * @param string $group
   *   The group entity.
   */
  public function setGroup($group) {
    $this->group = $group;
    list($this->groupId, , $this->groupBundle) = entity_extract_ids($this->groupType, $group);
  }

  /**
   * Group member bundle will be open from this date.
   *
   * @return object
   *   DateObject or NULL if no from date is required.
   */
  public function getFromDate() {
    return $this->fromDate;
  }

  /**
   * Group content will be open up to and including this date.
   *
   * @return object
   *   DateObject or NULL if no to date is required.
   */
  public function getToDate() {
    return $this->toDate;
  }

  /**
   * What measure to compare dates with.
   *
   * @see DateObject::difference()
   *
   * @return string
   *   Name of measure. 'seconds', 'days', 'weeks' ...
   */
  public function getMeasure() {
    return $this->measure;
  }

}
