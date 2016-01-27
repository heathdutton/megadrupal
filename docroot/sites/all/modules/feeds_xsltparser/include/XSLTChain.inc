<?php
/**
 * @file:   Implementation of a simple XSLT pipeline applying a set of XSLT
 *          stylesheets to the given markup one by one.
 */

class XSLTChain implements XSLTPipe {
  private $xsltProcessors = array();
  private $xsltPaths = array();
  private $xmlMessages = array();
  private $xmlLastRecordErrors = FALSE;
  private $logproxy;
  private $logproxyArg;

  function run($markup, $params = array()) {
    foreach ($this->xsltProcessors as $i => $proc) {
      // parse markup
      $this->beginRecordLibXMLMessages();
      $doc = new DOMDocument();
      $doc->loadXML($markup);
      $ok = $this->endRecordLibXMLMessages();

      if ($doc === FALSE || $ok === FALSE) {
        throw new LibXMLException(t('Failed to load XML while running XSLT chain.'));
      }

      // run stylesheets
      $this->beginRecordLibXMLMessages();
      $proc->setParameter('', $params);
      $markup = $proc->transformToXML($doc);
      $ok = $this->endRecordLibXMLMessages($this->xsltPaths[$i]);

      if ($markup === FALSE || $ok === FALSE) {
        throw new LibXMLException(t('Failed to transform XML using XSLT chain.'));
      }
    }

    return $markup;
  }

  function setLogProxy(XSLTChainLogProxy $logproxy, $logproxyArg = NULL) {
    $this->logproxy = $logproxy;
    $this->logproxyArg = $logproxyArg;
  }


  function addProcessor(XSLTProcessor $proc, $path = NULL) {
    $this->xsltProcessors[] = $proc;
    $this->xsltPaths[] = $path;
  }

  function addProcessorFromPath($path) {
    // Loading a malformed XML document results in a fatal error
    // (LIBXML_ERR_FATAL)
    $this->beginRecordLibXMLMessages();
    $doc = new DOMDocument();
    $doc->load($path, LIBXML_NOCDATA);
    $ok = $this->endRecordLibXMLMessages();

    if ($doc === FALSE || $ok === FALSE) {
      throw new LibXMLException(t('Failed to load XML file at @path.',
        array('@path' => $path)));
    }

    // According to the PHP sourcecode, XSLTProcessor::importStylesheet does
    // return TRUE on success and FALSE on failure at least since 5.2.0.
    // Therefore I rely on this undocumented feature here.
    // Attempting to import an XSLT from an arbritary but well formed XML file
    // results in multiple LIBXML_ERR_ERROR.
    $proc = new XSLTProcessor();
    $this->beginRecordLibXMLMessages();
    $result = $proc->importStylesheet($doc);
    $ok = $this->endRecordLibXMLMessages();

    if ($result === FALSE || $ok === FALSE) {
      throw new LibXMLException(t('Failed to load XSLT stylesheet from @path.',
        array('@path' => $path)));
    }

    $this->addProcessor($proc, $path);
  }

  /**
   * Turns error recording on for until it is turned off using
   * endRecordLibXMLMessages again.
   */
  private function beginRecordLibXMLMessages() {
    $this->xmlLastRecordErrors = libxml_use_internal_errors(TRUE);
  }

  /**
   * Returns false if the recorded messages contain an LibXMLError whose level
   * is above LIBXML_ERR_WARNING, otherwise returns true.
   */
  private function endRecordLibXMLMessages($xsltPath = NULL) {
    $result = TRUE;
    foreach (libxml_get_errors() as $xmlMessage) {
      if ($xmlMessage->level > LIBXML_ERR_WARNING) {
        $result = FALSE;
      }
      if ($this->logproxy) {
        $this->logproxy->log($xmlMessage, $this->logproxyArg, $xsltPath);
      }
    }

    libxml_clear_errors();
    libxml_use_internal_errors($this->xmlLastRecordErrors);

    return $result;
  }
}
