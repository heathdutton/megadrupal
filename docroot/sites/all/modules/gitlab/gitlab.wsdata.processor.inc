<?php

/**
 * @file
 * Web Service data processor for GitLab User
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 * @author François Xavier Lemieux <flemieux@coldfrontlabs.ca>
 */

class gitlabWsdataProcessor extends wsdata_simple_json_processor {
  // override - Return any error messages or error data
  public function getError() {
    return $this->error;
  }

  function parse($response) {
    if (isset($response->error)) {
      $data = parent::parse($response->data);
      $this->error = array($response->code => $data);
    } else {
      $data = parent::parse($response);
    }
    return $data;
  }
  
  /**
   * Process request result
   */
  function process_result($data) {
    $ret = array();
    $this->addData($data);
    $error = $this->getError();
    if ($error) {
      $error_keys = array_keys($error);
      $ret['error'] = $error;
    } else {
      $data = $this->getData();
      if (!empty($data)) {
        $ret['data'] = $data;
      }
    }
    return $ret;
  }
}
