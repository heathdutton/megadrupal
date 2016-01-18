<?php
/**
 * @file:   Interface for an XSLT pipeline classes
 */

interface XSLTPipe {
  public function run($markup, $params = array());
}
