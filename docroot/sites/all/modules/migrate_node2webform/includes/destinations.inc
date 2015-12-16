<?php

/**
 * Destination handler for webform submissions.
 */
class WebformSubmissionD7Destination extends MigrateDestination {
  // The webform node we will use to populate submissions.
  protected $webform_node;

  public function __construct($webform_nid) {
    $this->webform_node = node_load($webform_nid);
  }

  /**
   * @override
   */
  public function __toString() {
    return t('Webform Submission (d7)');
  }

  /**
   * @override
   */
  public static function getKeySchema() {
    return MigrateDestinationTable::getKeySchema('webform_submissions');
  }

  /**
   * @override
   */
  public function fields() {
    $fields = array();
    $fields += $this->getCoreFields();
    $fields += $this->getComponentFields();
    return $fields;
  }

  /**
   * Get core fields of a webform submission.
   *
   * @return
   *   Array of fields.
   */
  public function getCoreFields() {
    return array(
      'nid' => t('The node identifier of a webform.'),
      'uid' => t('The id of the user that completed this submission.'),
      'is_draft' => t('Is this a draft of the submission?'),
      'submitted' => t('Timestamp of when the form was submitted.'),
      'remote_addr' => t('The IP address of the user that submitted the form.'),
    );
  }

  /**
   * Get webform components.
   *
   * @return
   *   Array of fields.
   */
  public function getComponentFields() {
    $fields = array();
    foreach ($this->webform_node->webform['components'] as $component) {
      $fields[$component['form_key']] = $component['name'];
    }
    return $fields;
  }

  /**
   * @override
   *
   * Insert imported submissions.
   */
  public function import(stdClass $valuesD7, stdClass $valuesD6) {
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    // Create submission object.
    $submission = new stdClass;
    foreach ($this->getCoreFields() as $key => $description) {
      $submission->$key = isset($valuesD7->$key) ? $valuesD7->$key : NULL;
    }
    $data = array();
    foreach ($this->webform_node->webform['components'] as $cid => $component) {
      $data[$cid] = isset($valuesD7->$component['form_key']) ? $valuesD7->$component['form_key'] : NULL;
    }
    $submission->data = webform_submission_data($this->webform_node, $data);

    // Insert submission.
    $sid = webform_submission_insert($this->webform_node, $submission);
    if (!empty($sid)) {
      $this->numCreated++;
    }
    return array($sid);
  }

  /**
   * Given a webform and a component machine name, get component ID.
   *
   * @param $node
   *   Webform node.
   * @param $name
   *   Component name.
   *
   * @return
   *   Component ID or FALSE if component does not exist.
   */
  public function getComponentId($node, $name) {
    foreach ($node->webform['components'] as $cid => $component) {
      if ($component['form_key'] == $name) {
        return $cid;
      }
    }
    return FALSE;
  }

  /**
   * @override
   *
   * Delete imported submissions.
   */
  public function rollback(array $key) {
    module_load_include('inc', 'webform', 'includes/webform.submissions');
    $sid = array_pop($key);
    $submission = webform_get_submission($this->webform_node->nid, $sid);
    if (!empty($submission)) {
      webform_submission_delete($this->webform_node, $submission);
    }
  }

}
