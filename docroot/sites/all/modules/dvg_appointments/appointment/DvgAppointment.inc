<?php
/**
 * @file
 * Contains the class DvgAppointment.
 */

/**
 * Class DvgAppointment.
 */
class DvgAppointment {
  /**
   * Local appointment id.
   *
   * @var int
   */
  private $id = null;

  /**
   * Start datetime of the appointment.
   *
   * @var DateTime
   */
  private $start;

  /**
   * End datetime of the appointment.
   *
   * @var DateTime
   */
  private $end;

  /**
   * A list of products.
   *
   * @var array
   */
  private $productList;

  /**
   * A list of remote appointment ids.
   *
   * @var array
   */
  private $remoteAppointments;

  /**
   * Associated webform node.
   *
   * @var int
   */
  private $nid;

  /**
   * Associated webform submission.
   *
   * @var int
   */
  private $sid;

  /**
   * Sets the start of the appointment.
   *
   * @param \DateTime $start
   */
  public function setStart(DateTime $start) {
    $this->start = $start;
  }

  /**
   * Sets the appointment end.
   *
   * @param \DateTime $end
   */
  public function setEnd(DateTime $end) {
    $this->end = $end;
  }


  /**
   * Gets the local appointment id.
   *
   * @return int
   *   The local appointment id.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Sets the local appointment id.
   *
   * @param int $local_id
   *   The local appointment id.
   */
  public function setId($local_id) {
    $this->id = $local_id;
  }

  /**
   * Adds a remote appointment to the local appointment.
   *
   * @param mixed $remote_appointment_id
   *   A remote appointment id.
   */
  public function addRemoteAppointmentId($remote_appointment_id) {
    $this->remoteAppointments[$remote_appointment_id] = $remote_appointment_id;
  }

  /**
   * Gets a list of remote appointment ids.
   *
   * @return array
   *   An array of remote appointment ids.
   */
  public function getRemoteAppointmentIds() {
    return array_values($this->remoteAppointments);
  }

  /**
   * Adds a product to the appointment.
   *
   * @param $product_id
   *   The product id.
   * @param int $count
   *   The amount of products of this type in the appointment.
   * @param int $duration
   *   The duration in seconds of one instance of this product.
   */
  public function addProduct($product_id, $count, $duration) {
    if (isset($this->productList[$product_id])) {
      $this->productList[$product_id]['count'] += $count;
    }
    else {
      $this->productList[$product_id] = array(
        'count' => $count,
        'duration' => $duration,
      );
    }
  }

  /**
   * Gets an estimated product duration based on product duration and count.
   *
   * @param mixed $product_id
   *   The product id.
   *
   * @return mixed
   *   The product duration in seconds.
   */
  public function getProductDurationEstimate($product_id) {
    return $this->getProductCount($product_id) * $this->getProductCount($product_id);
  }

  /**
   * Gets an estimated appointment duration based on product duration and count.
   *
   * @return mixed
   *   The estimated appointment duration in seconds.
   */
  public function getAppointmentDurationEstimate() {
    $duration = 0;
    foreach ($this->getProductIds() as $product_id) {
      $duration += $this->getProductCount($product_id) * $this->getProductDuration($product_id);
    }
    return $duration;
  }

  /**
   * Gets the appointment duration based on start and end times.
   *
   * @return int
   *   The appointment duration in seconds.
   */
  public function getAppointmentDuration() {
    if (isset($this->end) && isset($this->start)) {
      return $this->end->format('U') - $this->start->format('U');
    }
  }

  /**
   * Gets a list of product ids included in the appointment.
   *
   * @return array
   *   An array of product ids.
   */
  public function getProductIds() {
    return array_keys($this->productList);
  }

  /**
   * Gets the amount of a particular product in the appointment.
   *
   * @param mixed $product_id
   *   A product id.
   *
   * @return int
   *   The amount of the product selected.
   */
  public function getProductCount($product_id) {
    return $this->productList[$product_id]['count'];
  }

  /**
   * Gets the duration of one instance of the product.
   *
   * @param mixed $product_id
   *   The product id.
   *
   * @return int
   *   The duration in seconds.
   */
  public function getProductDuration($product_id) {
    return $this->productList[$product_id]['duration'];
  }

  /**
   * Gets the start of the appointment.
   *
   * @return DateTime
   *   Appointment start.
   */
  public function getStart() {
    return $this->start;
  }

  /**
   * Gets the end of the appointment.
   *
   * @return DateTime
   *   Appointment end.
   */
  public function getEnd() {
    return $this->end;
  }

  /**
   * Gets the associated webform node.
   *
   * @return int
   *   Webform node id.
   */
  public function getNodeId() {
    return $this->nid;
  }

  /**
   * Sets the associated webform node.
   *
   * @param int $nid
   *   Webform node id.
   */
  public function setNodeId($nid) {
    $this->nid = $nid;
  }

  /**
   * Gets the associated webform submission.
   *
   * @return int
   *   Webform submission id.
   */
  public function getSubmissionId() {
    return $this->sid;
  }

  /**
   * Sets the associated webform submission.
   *
   * @param int $sid
   *   Webform submission id.
   */
  public function setSubmissionId($sid) {
    $this->sid = $sid;
  }

}