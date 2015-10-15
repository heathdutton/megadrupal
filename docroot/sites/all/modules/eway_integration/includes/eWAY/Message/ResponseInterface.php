<?php

namespace eWAY\Message;

interface ResponseInterface {
  public function getStatusCode();
  public function getReasonPhrase();
  public function json(array $config = array());
}
