<?php

use \StructuredDynamics\osf\php\api\framework\WebServiceQuery;

class DrupalWebServiceQuery extends WebServiceQuery {
  public function setParameters(array $params) {
    $this->params = $params;
  }

  public function setEndpoint($endpoint) {
    parent::setEndpoint($endpoint);
  }

  public function setAppID($appID) {
    $this->appID = $appID;
  }

  public function setUserID($userID) {
    $this->userID = $userID;
  }

  public function setApiKey($apiKey) {
    $this->apiKey = $apiKey;
  }

  public function setMethodGet() {
    parent::setMethodGet();
  }

  public function setMethodPost() {
    parent::setMethodPost();
  }

  public function setNetwork($network) {
    return parent::setNetwork($network);
  }

  public function setSupportedMimes($mimes) {
    return parent::setSupportedMimes($mimes);
  }

  public function setTimeout($timeout) {
    $this->timeout = $timeout;
  }

  public function displayError() {
    $dqe = new DrupalQuerierExtension();
    $dqe->displayError($this->error);
  }
}
