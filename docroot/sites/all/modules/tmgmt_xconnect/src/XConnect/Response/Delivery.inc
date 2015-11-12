<?php
/**
 * @file
 * Class that wraps around the response delivery XML.
 */

/**
 * Class that wraps around the delivery meta file from the response archive.
 */
class TmgmtXConnectResponseDelivery {
  /**
   * The XML.
   *
   * @var SimpleXMLElement
   */
  private $xml;

  /**
   * Constructor.
   *
   * @param string $xml
   *   The raw XML as received from the translation service.
   */
  public function __construct($xml) {
    $this->xml = new SimpleXMLElement($xml);
  }

  /**
   * Get the ID.
   *
   * @return string
   *   The DeliveryId property.
   */
  public function getId() {
    return (string) $this->xml->DeliveryId;
  }

  /**
   * Get the Date.
   *
   * @return DateTime
   *   The DeliveryDate property.
   */
  public function getDate() {
    return new DateTime((string) $this->xml->DeliveryDate);
  }

  /**
   * Get the status.
   *
   * @return string
   *   The DeliveryStatus property.
   */
  public function getStatus() {
    return (string) $this->xml->DeliveryStatus;
  }

  /**
   * Get the person who issued the translation.
   *
   * @return string
   *   The IssuedBy property.
   */
  public function getIssuedBy() {
    return (string) $this->xml->IssuedBy;
  }

  /**
   * Get an array with info about the files within the translation delivery.
   *
   * @return array
   *   Array with info for each file.
   */
  public function getFilesInfo() {
    $info = array();
    foreach ($this->xml->DeliveryFiles->DeliveryFile as $file) {
      // Convert the SimpleXmlElement to StdClass object.
      $info[] = json_decode(json_encode($file));
    };

    return $info;
  }

}
