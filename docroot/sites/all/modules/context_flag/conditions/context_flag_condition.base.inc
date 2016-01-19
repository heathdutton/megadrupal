<?php
/**
 * @file
 * Abstract base class for context_flag conditions to inherit from.
 *
 * Based on recommendation from joachim in ticket labelled
 * "inherit from abstract parent?"
 *
 * @link https://www.drupal.org/node/2273725
 */

/**
 * Provides common tools used by Flag-driven conditions for the Context module.
 */
abstract class ContextFlagBase extends context_condition {
  // Properties that are required to keep Context functioning properly.
  public $plugin = NULL;
  public $title = NULL;
  public $description = NULL;
  // Properties used in context_flag code.
  public $contextSettingName = NULL;
  public $flagType = NULL;
  /**
   * Helper for setting property values in this context_condition instance.
   */
  public function setPropertyValue($property, $value) {
    $this->{$property} = $value;
  }
  /**
   * Helper for determining if there are flags available for this entity type.
   *
   * Used to control help text shown in the context_ui settings form.
   */
  private function hasAvailableFlags() {
    $flags = $this->getFlagsForType();
    if (count($flags) > 0) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  /**
   * Helper function to simplify getting a list of flags for this entity type.
   *
   * @return array
   *   An array of flags available for this entity type.
   */
  public function getFlagsForType() {
    return flag_get_flags($this->flagType);
  }
  /**
   * Returns the list of flags available for this entity type.
   *
   * Naming pattern required in order to comply with existing code from the
   * Context module.
   *
   * @return array
   *   An array of flags available for this type OR an array with a single
   *   item to let users know that there are no flags of this type available.
   *   (Needs to return an array for both cases because of how this is used with
   *   the forms API within the Context module.)
   */
  public function condition_values() {
    $values = array();
    if ($this->hasAvailableFlags()) {
      $flags = $this->getFlagsForType();
      foreach ($flags as $flag) {
        $values[$flag->name] = $flag->title;
      }
    }
    else {
      $values[NULL] = t('<b>Attention:</b> No flags of this type are available. (Please add one and try again.)');
    }
    return $values;
  }
  /**
   * Options form used when configuring a context within the context_ui.
   *
   * Naming pattern required in order to comply with existing code from the
   * Context module.
   *
   * @return array
   *   Returns a forms API structure for additional fields to add to the
   *   configuration form.
   */
  public function options_form($context) {
    if ($this->hasAvailableFlags()) {
      $defaults = $this->fetch_from_context($context, 'options');
      $default_value = FALSE;
      if (isset($defaults[$this->contextSettingName])) {
        $default_value = $defaults[$this->contextSettingName];
      }
      return $this->requireAllForm($this->contextSettingName, $default_value);
    }
    else {
      return $this->noMatchingFlagsForm($this->contextSettingName);
    }
  }
  /**
   * Returns the form for Contexts that requires multiple flags.
   *
   * @return array
   *   A forms API form.
   *
   * @see options_form
   */
  public function requireAllForm($setting, $value = FALSE) {
    $form = array();
    $form[$setting] = array(
      '#title' => t('Require all flags'),
      '#type' => 'checkbox',
      '#description' => t('Trigger this context only when ALL selections are active.'),
      '#default_value' => $value,
    );
    return $form;
  }
  /**
   * Returns the form when there are no flags to use for a Context.
   *
   * @return array
   *   A forms API form.
   *
   * @see options_form
   */
  public function noMatchingFlagsForm($setting) {
    $form = array();
    $form[$setting] = array(
      '#type' => 'item',
      '#title' => t('No matching flags found'),
      '#description' => t('There are no flags of this type available for use.'),
    );
    return $form;
  }
  /**
   * Helper function for the individual condition logic-checking process.
   *
   * Naming pattern required in order to comply with existing code from the
   * Context module.
   *
   * @see contextFlagConditionCheck
   */
  public function execute() {
    $this->contextFlagConditionCheck();
  }
  /**
   * Overrideable default for providing condition-specific logic.
   */
  abstract public function contextFlagConditionCheck();

}
