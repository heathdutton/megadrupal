<?php
/**
 * @file:   Interface for XSLT pipeline log proxy classes.
 */

interface XSLTChainLogProxy {
  public function log($xmlError, $prefix, $xsltPath);
}
