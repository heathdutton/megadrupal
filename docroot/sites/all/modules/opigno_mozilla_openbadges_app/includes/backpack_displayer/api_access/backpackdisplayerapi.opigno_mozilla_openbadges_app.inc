<?php


class BackpackDisplayerApi
{

  protected $backpackDisplayer;

  function __construct(BackpackDisplayer $backpackDisplayer)
  {
    $this->backpackDisplayer = $backpackDisplayer;
  }

  public function getUserId($backpackEmail)
  {
    $url = $this->backpackDisplayer->getApiRoot() .'/convert/email';
    $data = array('email' => $backpackEmail);

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_POST, count($data));
    curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($data));
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json'
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    $responseDecoded = json_decode($response);
    if (empty($responseDecoded) || empty($responseDecoded->userId))
      return -1;

    return $responseDecoded->userId;
  }

  public function getCollections()
  {
    $userId = $this->backpackDisplayer->getUserId();
    if (empty( $userId ))
    {
      return false;
    }
    $url = $this->backpackDisplayer->getApiRoot() .'/'. $userId .'/groups.json';

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json'
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    $responseDecoded = json_decode($response);
    if (empty($responseDecoded) || !isset($responseDecoded->groups))
      return false;

    return $responseDecoded->groups;
  }

  public function getBadges($groupId)
  {
    if (!is_numeric($groupId))
    {
      return false;
    }

    $userId = $this->backpackDisplayer->getUserId();
    if (empty( $userId ))
    {
      return false;
    }

    $url = $this->backpackDisplayer->getApiRoot() .'/'. $userId .'/group/'. $groupId .'.json';

    $ch = curl_init();
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, false);
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
      'Content-Type: application/json'
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

    $response = curl_exec($ch);
    curl_close($ch);

    $responseDecoded = json_decode($response);
    if (empty($responseDecoded) || !isset($responseDecoded->badges))
      return false;

    return $responseDecoded;
  }

}