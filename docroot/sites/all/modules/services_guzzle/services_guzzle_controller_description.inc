<?php

abstract class ServicesGuzzleControllerDescription {
  protected $resource_name;
  protected $controller_name;
  protected $controller;

  public function __construct($resource_name, $controller_name, $controller) {
    $this->resource_name = $resource_name;
    $this->controller_name = $controller_name;
    $this->controller = $controller;
  }

  abstract public function getHttpMethod();
  abstract public function getUri();
}

class ServicesGuzzleControllerDescriptionAction extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'POST';
  }

  public function getUri() {
    return $this->resource_name . '/' . $this->controller_name;
  }
}

class ServicesGuzzleControllerDescriptionTargetedAction extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'POST';
  }

  public function getUri() {
    return $this->resource_name . '/{id}/' . $this->controller_name;
  }
}

class ServicesGuzzleControllerDescriptionRelationship extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'GET';
  }

  public function getUri() {
    return $this->resource_name . '/{id}/' . $this->controller_name;
  }
}

class ServicesGuzzleControllerDescriptionOperationIndex extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'GET';
  }

  public function getUri() {
    return $this->resource_name;
  }
}

class ServicesGuzzleControllerDescriptionOperationCreate extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'POST';
  }

  public function getUri() {
    return $this->resource_name;
  }
}

class ServicesGuzzleControllerDescriptionOperationRetrieve extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'GET';
  }

  public function getUri() {
    return $this->resource_name . '/{id}';
  }
}

class ServicesGuzzleControllerDescriptionOperationUpdate extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'PUT';
  }

  public function getUri() {
    return $this->resource_name . '/{id}';
  }
}

class ServicesGuzzleControllerDescriptionOperationDelete extends ServicesGuzzleControllerDescription {
  public function getHttpMethod() {
    return 'DELETE';
  }

  public function getUri() {
    return $this->resource_name . '/{id}';
  }
}