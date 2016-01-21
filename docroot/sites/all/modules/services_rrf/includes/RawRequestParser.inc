<?php

class RawRequestParser implements ServicesParserInterface {
  public function parse(ServicesContextInterface $context) {
    return $context->getRequestBody();
  }
}
