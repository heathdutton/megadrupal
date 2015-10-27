<?php

class BackpackDisplayer
{

  protected $apiRoot;
  protected $userId;


  function __construct($apiRoot)
  {
    $this->setApiRoot($apiRoot);
  }


  /**
   * @return mixed
   */
  public function getApiRoot()
  {
    return $this->apiRoot;
  }

  /**
   * @param mixed $apiRoot
   */
  public function setApiRoot($apiRoot)
  {
    if (!valid_url($apiRoot, true))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The api root given is invalid. Api root given: '. $apiRoot);
      return;
    }
    $this->apiRoot = $apiRoot;
  }

  /**
   * @return mixed
   */
  public function getUserId()
  {
    return $this->userId;
  }

  /**
   * @param mixed $userId
   */
  public function setUserId($userId)
  {
    if (!is_numeric($userId))
    {
      watchdog('Opigno Mozilla Openbadges App', 'The user ID is invalid. UID given: '. $userId);
      return;
    }
    $this->userId = $userId;
  }

}