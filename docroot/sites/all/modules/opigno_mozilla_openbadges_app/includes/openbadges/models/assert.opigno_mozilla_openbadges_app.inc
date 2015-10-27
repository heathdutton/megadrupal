<?php


/**
 * Class Assert
 * The assert model class
 */
class Assert
{
  /**
   * @var String The unique ID of this assert
   */
  protected $id;

  /**
   * @var String The mail address of the earner
   */
  protected $recipient;

  /**
   * @var String The URL to the JSON file of the badge class
   */
  protected $badgeUrl;

  /**
   * @var String The URL of this JSON file
   */
  protected $assertUrl;

  /**
   * @var String The date when this assert was issued
   */
  protected $issueDate;


  /**
   * @var Array Some salts for hashing the earner mail
   */
  public static $salts = array('deadsea', 'opigno1234', 'thisisasalt', 'lifecoffee', 'pencil', 'mymouseissick');


  /**
   * @param $id String The unique ID of this assert
   * @param $recipient String The mail address of the earner OR the recipient json object
   * @param $badgeUrl String The URL to the JSON file of the badge class
   * @param $assertUrl String The URL of this JSON file
   * @param $issueDate String The date when this assert was issued
   */
  function __construct($id, $recipient, $badgeUrl, $assertUrl, $issueDate)
  {
    $this->id = $id;
    $this->recipient = $recipient;
    $this->badgeUrl = $badgeUrl;
    $this->assertUrl = $assertUrl;
    $this->issueDate = $issueDate;
  }


  /**
   * Return a JSON format of this object, ready to be saved, according the Openbadge specifications.
   *
   * @return string JSON of this object.
   */
  public function toJson()
  {
    // If the recipient is simply a mail, do the trick to get a good recipient
    $goodRecipient = json_decode($this->recipient, true);
    if (empty($goodRecipient))
    {
      $salt = self::$salts[array_rand(self::$salts, 1)]; // Get a random salt from the salts array
      $goodRecipient = array(
        'type' => 'email',
        'hashed' => true,
        'salt' => $salt,
        'identity' => 'sha256$'. hash('sha256', $this->recipient . $salt)
      );
    }

    return json_encode(array(
      'uid' => $this->id,
      'recipient' => $goodRecipient,
      'badge' => $this->badgeUrl,
      'verify' => array(
        'type' => 'hosted',
        'url' => $this->assertUrl
      ),
      'issuedOn' => $this->issueDate
    ));
  }

}
