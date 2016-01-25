<?php

/**
 * @file: Write warnings and errors from libxml to feeds import log
 */

/**
 * An implementation of XSLTCHainLogProxy which writes every libxml warning and
 * error to the feeds importer log. The first error-level message is retained
 * in order to give exception handlers a clue what happened inside the XSLTPipe
 * structure beeing run.
 */
class FeedsXSLTChainLogProxy implements XSLTChainLogProxy {
  private $source;
  private $config;
  private $originalError;
  private $originalPrefix;
  private $originalXslt;
  private $configPrefix;

  public function __construct(FeedsSource $source, $config, $configPrefix) {
    $this->source = $source;
    $this->config = $config;
    $this->configPrefix = $configPrefix;
  }

  /**
   * Send libxml warnings and errors to feeds source log.
   */
  public function log($xmlError, $prefix, $xsltPath) {
    $isError = $xmlError->level > LIBXML_ERR_WARNING;

    if ($isError && !$this->originalError) {
      $this->originalError = $xmlError;
      $this->originalPrefix = $prefix;
      $this->originalXslt = $xsltPath;
    }

    $this->source->log(
      'import', $this->formatXmlError($xmlError, $prefix, $xsltPath), array(),
      $isError ? WATCHDOG_ERROR : WATCHDOG_WARNING);
  }

  public function formatXmlError($xmlError, $prefix, $xsltPath) {
    $differentPrefix = substr_compare($this->configPrefix, $xsltPath, 0,
      drupal_strlen($this->configPrefix));
    if (!$differentPrefix) {
      $xsltPath = drupal_substr($xsltFile, drupal_strlen($this->configPrefix));
    }

    $differentPrefix = substr_compare($this->configPrefix, $xmlError->file, 0,
      drupal_strlen($this->configPrefix));
    if (!$differentPrefix) {
      $xmlPath = drupal_substr($xmlError->file, drupal_strlen($this->configPrefix));
    }
    else {
      $xmlPath = $xmlError->file;
    }

    $msg = t("Message: %message<br/>Chain: %prefix<br/>File: %path Line: %line Column: %col",
      array(
        '%message' => $xmlError->message,
        '%prefix' => $prefix,
        '%path' => $xmlPath,
        '%line' => $xmlError->line,
        '%col' => $xmlError->column,
      ));
    if ($xsltPath) {
      $msg .= '<br/>' . t('XSLT file: %path', array('%path' => $xsltPath));
    }

    return $msg;
  }

  /**
   * Reset original error
   */
  public function resetOriginalError() {
    $this->originalError = NULL;
    $this->originalPrefix = NULL;
    $this->originalXslt = NULL;
  }

  /**
   * Return original error and prefix.
   */
  public function getOriginalError() {
    return array($this->originalError, $this->originalPrefix, $this->originalXslt);
  }
}
