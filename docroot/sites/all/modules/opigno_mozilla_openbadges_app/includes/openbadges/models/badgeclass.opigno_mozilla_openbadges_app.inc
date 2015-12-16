<?php


/**
 * Class BadgeClass
 * The badge model class
 */
class BadgeClass
{
  /**
   * @var String The badge name.
   */
  protected $name;

  /**
   * @var String The badge description.
   */
  protected $description;

  /**
   * @var String The URL of the badge logo.
   */
  protected $imageUrl;

  /**
   * @var String The URL of the badge Criteria.
   */
  protected $criteriaUrl;

  /**
   * @var String The URL of the issuer JSON file.
   */
  protected $issuerFileUrl;


  /**
   * @param $name String The badge name.
   * @param $description String The badge description.
   * @param $imageUrl String The URL of the badge logo.
   * @param $criteriaUrl String The URL of the badge Criteria.
   * @param $issuerFileUrl String The URL of the issuer JSON file.
   */
  function __construct($name, $description, $imageUrl, $criteriaUrl, $issuerFileUrl)
  {
    $this->name = $name;
    $this->description = $description;
    $this->imageUrl = $imageUrl;
    $this->criteriaUrl = $criteriaUrl;
    $this->issuerFileUrl = $issuerFileUrl;
  }


  /**
   * Return a JSON String ready to be saved as a Badge file, according the Openbadge specifications
   *
   * @return String JSON of this object
   */
  public function toJson()
  {
    return json_encode(array(
      'name'        => $this->name,
      'description' => (empty($this->description) ? '' : $this->description),
      'image'       => $this->imageUrl,
      'criteria'    => $this->criteriaUrl,
      'issuer'      => $this->issuerFileUrl
    ));
  }


  /**
   * Check if two badge object are equals
   *
   * @param $other BadgeClass The other badge object to check
   *
   * @return bool true if they are equals | false otherwise
   */
  public function isEqual(self $other)
  {
    if (
      $this->name == $other->name
      AND $this->description == $other->description
      AND $this->imageUrl == $other->imageUrl
      AND $this->criteriaUrl == $other->criteriaUrl
      AND $this->issuerFileUrl == $other->issuerFileUrl
    ) {
      return true;
    }
    return false;
  }

}