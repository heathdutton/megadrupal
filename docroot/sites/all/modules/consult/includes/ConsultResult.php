<?php
/**
 * @file
 * Result class for consult module.
 */

class ConsultResult extends ConsultInterviewSubEntity {

  /**
   * Set the value for a given entity property.
   */
  public function setValue($key, $value) {
    $this->{$key} = $value;
  }

  /**
   * Get question text value.
   *
   * @param bool $use_filter
   *   Whether or not to use the format filter.
   *
   * @return string
   *   The question text value.
   */
  public function getMessageText($use_filter = TRUE) {
    $message_text = '';

    if (!empty($this->message['value'])) {
      $message_text = $this->message['value'];
    }

    if ($use_filter) {
      $message_text = check_markup($message_text, $this->getMessageFormat());
    }

    return $message_text;
  }

  /**
   * Get question text format.
   *
   * @return string
   *   The question text format.
   */
  public function getMessageFormat() {
    $message_format = FILTER_FORMAT_TEXT;

    if (!empty($this->message['format'])) {
      $message_format = $this->message['format'];
    }

    return $message_format;
  }

  /**
   * Get the edit path for this entity.
   *
   * @param string $op
   *   The operation to perform (eg edit or delete).
   *
   * @return string
   *   The Drupal path to edit this entity.
   */
  public function getOperationPath($op) {
    return 'admin/structure/consult/manage/' . $this->interview_name . '/result/' . $op . '/' . $this->identifier();
  }

  /**
   * Get the inherited dependencies assigned to this result.
   *
   * @return array
   *   A list of result entity names.
   */
  public function getInheritedDependencies() {
    $output = array();

    if (!empty($this->inherited_dependencies)) {
      $output = array_values($this->inherited_dependencies);
    }

    return $output;
  }

  /**
   * {@inheritdoc}
   */
  public function getDependencyOutput() {
    $output = array();
    $dependencies = $this->getDependencies(TRUE);
    $inheritance = $this->getInheritedDependencies();

    if (empty($inheritance)) {
      $output = $dependencies;
    }
    // Merge the dependencies for any inheritance.
    else {
      foreach ($inheritance as $inherited_result_name) {
        $result_entity = entity_load_single(CONSULT_RESULT_ENTITY_NAME, $inherited_result_name);
        $inherited_result_dependencies = $result_entity->getDependencies(TRUE);

        foreach ($inherited_result_dependencies as $inherited_dependency) {
          foreach ($dependencies as $dependency) {
            $output[] = array_merge($inherited_dependency, $dependency);
          }
        }
      }
    }

    return $output;
  }
}
