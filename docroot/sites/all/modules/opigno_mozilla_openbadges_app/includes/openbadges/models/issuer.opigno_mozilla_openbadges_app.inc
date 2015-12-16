<?php

/**
 * Class Issuer
 * The issuer model class
 */
class Issuer
{

  /**
   * @var $name String The issuer name
   */
  protected $name;

  /**
   * @var $description String The issuer description
   */
  protected $description;

  /**
   * @var $url String The URL of the issuer website
   */
  protected $url;


  /**
   * The Issuer constructor.
   *
   * @param $name String The issuer name
   * @param $description String The issuer description
   * @param $url String The URL of the issuer website
   */
  function __construct($name, $description, $url)
  {
    $this->name = $name;
    $this->description = $description;
    $this->url = $url;
  }


  /**
   * Return this class in Json format ready to be an issuer file.
   *
   * @return String Json format of this object
   */
  public function toJson()
  {
    return json_encode(array(
      'name'        => $this->name,
      'description' => $this->description,
      'url'         => $this->url
    ));
  }


  /**
   * Check if two issuer objects are equals.
   *
   * @param Issuer $other The other issuer object to check
   *
   * @return bool true if they are equal | false otherwise
   */
  public function isEqual(self $other)
  {
    if (
      $this->name == $other->name
      AND $this->description == $other->description
      AND $this->url == $other->url
    ) {
      return true;
    }
    return false;
  }

}