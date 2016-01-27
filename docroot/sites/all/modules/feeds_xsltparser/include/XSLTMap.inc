<?php
/**
 * @file: Apply a map of XSLTPipes keyed by integers or strings
 */

class XSLTMap implements XSLTPipe {
  private $map;
  private $xsltParamKeyName;
  private $preproc;

  public function __construct(Iterator $map, $xsltParamKeyName = '_item', $preproc = NULL) {
    $this->map = $map;
    $this->xsltParamKeyName = $xsltParamKeyName;
    $this->preproc = $preproc;
  }

  public function setXsltParamKeyName($xsltParamKeyName) {
    $this->xsltParamKeyName = $xsltParamKeyName;
  }

  public function setPreprocessor($preproc) {
    $this->preproc = $preproc;
  }

  public function run($markup, $params = array()) {
    // Run preprocessor pipe if configured.
    if ($this->preproc) {
      $markup = $this->preproc->run($markup, $params);
    }

    // If the mapping function itself is implemented as an XSLTPipe, like e.g.
    // XSLTRepeat, run it here on the input.
    if ($this->map instanceof XSLTPipe) {
      $this->map->run($markup, $params);
    }

    $result = array();
    foreach ($this->map as $key => $pipe) {
      $pipe_params = $params + array($this->xsltParamKeyName => $key);
      $result[$key] = $pipe->run($markup, $pipe_params);
    }

    return $result;
  }
}
