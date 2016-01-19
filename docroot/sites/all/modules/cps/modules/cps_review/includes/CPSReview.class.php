<?php
/**
 * @file
 * Contains the class for the CPS review entity.
 */

/**
 * Class to handle CPS review entities.
 */
class CPSReview extends Entity {

  /**
   * The database identifier of this CPS review.
   *
   * @var int
   */
  public $review_id = NULL;

  /**
   * The database identifier of the associated changeset, which will be a sha256.
   *
   * @var string
   */
  public $changeset_id = NULL;

  /**
   * The user ID of the author/owner of the CPS review.
   *
   * @var int
   */
  public $uid = NULL;

  /**
   * The current status of this CPS review.
   *
   * @var bool
   */
  public $status = NULL;

  /**
   * {@inheritdoc}
   */
  protected function defaultLabel() {
    return check_plain($this->title);
  }

  /**
   * {@inheritdoc}
   */
  protected function defaultUri() {
    return array('path' => 'cps-review/' . $this->identifier());
  }

  /**
   * Returns the HTML ID used to identify the review on the page.
   *
   * @return string
   *   Returns the build HTML ID.
   */
  public function htmlId() {
    return 'cps-review-' . $this->identifier();
  }

  /**
   * Returns the permalink, where this entity can be found in context.
   *
   * @return array
   *   Returns an array with 'path' and 'options' keys.
   *
   * @see entity_uri()
   */
  public function permalink() {
    $container_entity = entity_load_single($this->container_entity_type, $this->container_entity_id);
    $uri = entity_uri($this->container_entity_type, $container_entity);
    $uri['options']['fragment'] = $this->htmlId();
    drupal_alter('cps_review_permalink', $uri, $this);

    return $uri;
  }
}
