<?php
/**
 * @file
 * Drush Interactive Commands Step Choice Base class.
 */

/**
 * Class IDCStepChoiceBase
 *
 * Defines a IDC Command Choice Step.
 */
abstract class IDCStepChoiceBase extends IDCStepBase {

  /**
   * @var array Settings array to be used in singleChoice and multipleChoice().
   */
  protected $choiceSettings = array();

  /**
   * @var bool Whether String Filtering is enabled for the choice step.
   */
  protected $stringFilter;

  /**
   * Gets the the String Filter availability value.
   *
   * @return bool
   *   TRUE, if String Filter is enabled, FALSE otherwise.
   */
  public function getStringFilter() {
    return $this->stringFilter;
  }

  /**
   * Sets the value for the String Filter availability for the choice step.
   *
   * @param $stringFilter
   *   The value (TRUE or FALSE) to set for the String Filter on this step.
   */
  public function setStringFilter($stringFilter) {
    $this->stringFilter = $stringFilter;
  }

  /**
   * Process the step. Main entry point of steps for code using step classes.
   *
   * This function can be used to add any pre-processing or post-processing
   * logic to the step, (e.g: before the input is requested to the user, and
   * after it's been requested but step execution hasn't finished yet).
   *
   * @return $this
   */
  public function processStep() {
    $this->preprocessStep();
    $this->requestInput();
    $this->postprocessStep();
    return $this;
  }

  public function filterOptionsByString($filterString) {
    $filtered_options = array();
    foreach ($this->original_options as $key => $label) {
      if (!empty($filterString) && strstr($label, $filterString)) {
        $filtered_options[$key] = $label;
      }
    }
    return $filtered_options;
  }

  /**
   * Adds a value for a given setting to use in singleChoice or multipleChoice.
   *
   * @param string $setting_name
   *   Name of the setting being added. Supported values:
   *    - 'wildcard_values'.
   *    - 'wildcard_regex'.
   * @param string $setting_name
   *   The value to be added to the $setting_name list of values.
   *
   * @return $this
   */
  public function addChoiceSettings($setting_name, $setting_value) {
    // Initialise array for setting_name, if it doesn't exist.
    $this->choiceSettings[$setting_name] = !empty($this->choiceSettings[$setting_name]) ? $this->choiceSettings[$setting_name] : array();
    // If setting value is not present already, add it.
    if (!in_array($setting_value, $this->choiceSettings[$setting_name])) {
      $this->choiceSettings[$setting_name][] = $setting_value;
    }
    return $this;
  }

  protected function preprocessStep() {
    $extra_options = array();
    // Check if string filtering is enabled.
    if ($this->getStringFilter()) {
      // Add it as an extra option of the list.
      $extra_options[__CLASS__ . '::stringFilter'] = '--- OTHER OPTIONS --- Filter by entering a string';
    }

    // Always enable string filtering through the "s:" and "search:" wildcards.
    $this->addChoiceSettings('wildcard_values', 's:');
    $this->addChoiceSettings('wildcard_values', 'search:');

    // Store the original options.
    if (!isset($this->original_options)) {
      $this->original_options = $this->getOptions();
    }

    // Add extra options, if any.
    if (!empty($extra_options)) {
      $this->setOptions(array_merge($this->getOptions(), $extra_options));
    }
  }

  protected function postprocessStep() {
    // Fetch input results.
    $results = $this->getInputResults();

    // Check for wildcard values if the result is a string.
    if (!empty($this->choiceSettings['wildcard_values']) && is_string($results)) {
      foreach ($this->choiceSettings['wildcard_values'] as $wildcard_value) {
        if (strpos($results, $wildcard_value) === 0) {
          switch ($wildcard_value) {
            case 's:':
            case 'search:':
              // Get options filtered by the string entered.
              $filter_string = str_replace($wildcard_value, '', $results);
              if (empty($filter_string)) {
                $this->setOptions($this->original_options)->processStep();
              }
              else {
                $filtered_options = $this->filterOptionsByString($filter_string);
                $this->setOptions($filtered_options)->processStep();
              }
              break;
          }
        }
      }
    }

    // Check for string filtering selection, if it is enabled as an option.
    if ($this->getStringFilter()) {
      if (($results == (__CLASS__ . '::stringFilter')) || (is_array($results) && in_array(__CLASS__ . '::stringFilter', $results))) {
        // Filter string entered. Filter options.
        $filter_choice = new IDCStepPrompt('Enter the string to filter the available options', NULL, FALSE);
        if ($filter_string = $filter_choice->processStep()->getInputResults()) {
          $filtered_options = $this->filterOptionsByString($filter_string);
          $this->setOptions($filtered_options)->processStep();
        }
        // Empty string entered. Render back original options.
        else {
          $this->setOptions($this->original_options)->processStep();
        }
      }
    }
  }

  /**
   * BEHOLD... THE POWER OF COPY AND PASTE.
   *
   * Ask the user to select an item from a list.
   * From a provided associative array, drush_choice will
   * display all of the questions, numbered from 1 to N,
   * and return the item the user selected. "0" is always
   * cancel; entering a blank line is also interpreted
   * as cancelling.
   *
   * NOTE: This function is a copy of drush_choice, from Drush, edited
   * to add more flexibility to the selection process.
   *
   * TODO: Placed here because it's convenient and doesn't seem a very stupid
   * idea anyways, but this (and others) print helpers should be placed in
   * another file of their own, and called when needed.
   *
   * @param $options
   *   A list of questions to display to the user.  The
   *   KEYS of the array are the result codes to return to the
   *   caller; the VALUES are the messages to display on
   *   each line. Special keys of the form '-- something --' can be
   *   provided as separator between choices groups. Separator keys
   *    don't alter the numbering.
   * @param $prompt
   *   The message to display to the user prompting for input.
   * @param $label
   *   Controls the display of each line.  Defaults to
   *   '!value', which displays the value of each item
   *   in the $options array to the user.  Use '!key' to
   *   display the key instead.  In some instances, it may
   *   be useful to display both the key and the value; for
   *   example, if the key is a user id and the value is the
   *   user name, use '!value (uid=!key)'.
   * @param $settings
   *   Array of settings to use by the selection process. Supported settings:
   *    - 'wildcard_values':  Array of values that will be accepted as possible
   *                          options. By default, the method will only accept
   *                          numeric values, to match them to one of the
   *                          options. Through this setting, some values with
   *                          special meaning (e.g: "s:" followed by a string to
   *                          search specific values in the options list) can be
   *                          used. Wildcard values will be only accepted when
   *                          used at the beginning of the string (e.g: "s: +
   *                          string" would be valid, but "somestring s:" won't.
   *
   *    - 'wildcard_regex':   Array of regex that will be used to validate user
   *                          input. That way, when a value entered doesn't
   *                          match a specific user input, it might still be
   *                          accepted as a valid one. This is useful to accept
   *                          a range (eg: "3-14"), or a list (eg: "3,4,7,10,11)
   *                          of values when working with multiple choice lists.
   *
   * @see drush_choice().
   */
  public static function singleChoice($options, $prompt = 'Enter a number.', $label = '!value', $widths = array(), $settings = array()) {
    // Initialise settings.
    $settings = array_merge(array(
      'wildcard_values' => array(),
      'wildcard_regex' => array(),
    ), $settings);

    drush_print(dt($prompt));

    // Preflight so that all rows will be padded out to the same number of columns
    $array_pad = 0;
    foreach ($options as $key => $option) {
      if (is_array($option) && (count($option) > $array_pad)) {
        $array_pad = count($option);
      }
    }

    $rows[] = array_pad(array('[0]', ':', 'Cancel'), $array_pad + 2, '');
    $selection_number = 0;
    foreach ($options as $key => $option) {
      if ((substr($key, 0, 3) == '-- ') && (substr($key, -3) == ' --')) {
        $rows[] = array_pad(array('', '', $option), $array_pad + 2, '');
      }
      else {
        $selection_number++;
        $row = array("[$selection_number]", ':');
        if (is_array($option)) {
          $row = array_merge($row, $option);
        }
        else {
          $row[] = dt($label, array('!number' => $selection_number, '!key' => $key, '!value' => $option));
        }
        $rows[] = $row;
        $selection_list[$selection_number] = $key;
      }
    }
    drush_print_table($rows, FALSE, $widths);
    drush_print_pipe(array_keys($options));

    // If the user specified --choice, then make an
    // automatic selection.  Cancel if the choice is
    // not an available option.
    if (($choice = drush_get_option('choice', FALSE)) !== FALSE) {
      // First check to see if $choice is one of the symbolic options
      if (array_key_exists($choice, $options)) {
        return $choice;
      }
      // Next handle numeric selections
      elseif (array_key_exists($choice, $selection_list)) {
        return $selection_list[$choice];
      }
      return FALSE;
    }

    // If the user specified --no, then cancel; also avoid
    // getting hung up waiting for user input in --pipe and
    // backend modes.  If none of these apply, then wait,
    // for user input and return the selected result.
    if (!drush_get_context('DRUSH_NEGATIVE') && !drush_get_context('DRUSH_AFFIRMATIVE') && !drush_get_context('DRUSH_PIPE')) {
      while ($line = trim(fgets(STDIN))) {
        // Value entered matches an available option. Return that.
        if (array_key_exists($line, $selection_list)) {
          return $selection_list[$line];
        }
        // Value doesn't match. Check if it's a wildcard_value
        foreach ($settings['wildcard_values'] as $wildcard_value) {
          if (strpos($line, $wildcard_value) === 0) {
            return $line;
          }
        }
        // Not a wildcard. Check if value entered passes the wildcard regex.
        foreach ($settings['wildcard_regex'] as $wildcard_regex) {
          // Not caring much about whether the full line has to match the regex,
          // or whether it's enough if just part of it passes it. Return the
          // line and let the caller decide.
          if (preg_match($wildcard_regex, $line)) {
            return $line;
          }
        }
      }
    }
    // We will allow --yes to confirm input if there is only
    // one choice; otherwise, --yes will cancel to avoid ambiguity
    if (drush_get_context('DRUSH_AFFIRMATIVE')  && (count($options) == 1)) {
      return $selection_list[1];
    }
    drush_print(dt('Cancelled'));
    return FALSE;
  }

  /**
   * BEHOLD... THE POWER OF COPY AND PASTE.
   *
   * Ask the user to select multiple items from a list.
   * This is a wrapper around drush_choice, that repeats the selection process,
   * allowing users to toggle a number of items in a list. The number of values
   * can be constrained by both min and max: the user will only be allowed to
   * finalize selection once the minimum number has been selected, and the oldest
   * selected value will "drop off" the list, if they exceed the maximum number.
   *
   * NOTE: This function is a copy of drush_choice_multiple, from Drush, edited
   * to add more flexibility to the selection process.
   *
   * TODO: Placed here because it's convenient and doesn't seem a very stupid
   * idea anyways, but this (and others) print helpers should be placed in
   * another file of their own, and called when needed.
   *
   * @param $options
   *   Same as drush_choice().
   * @param $defaults
   *   This can take 3 forms:
   *   - FALSE: (Default) All options are unselected by default.
   *   - TRUE: All options are selected by default.
   *   - Array of $options keys to be selected by default.
   * @param $prompt
   *   Same as drush_choice().
   * @param $label
   *   Same as drush_choice().
   * @param $mark
   *   Controls how selected values are marked.  Defaults to '!value (selected)'.
   * @param $min
   *   Constraint on minimum number of selections. Defaults to zero. When fewer
   *   options than this are selected, no final options will be available.
   * @param $max
   *   Constraint on minimum number of selections. Defaults to NULL (unlimited).
   *   If the a new selection causes this value to be exceeded, the oldest
   *   previously selected value is automatically unselected.
   * @param $final_options
   *   An array of additional options in the same format as $options.
   *   When the minimum number of selections is met, this array is merged into the
   *   array of options. If the user selects one of these values and the
   *   selection process will complete (the key for the final option is included
   *   in the return value). If this is an empty array (default), then a built in
   *   final option of "Done" will be added to the available options (in this case
   *   no additional keys are added to the return value).
   * @param array $settings
   *   Same as IDCStepChoiceBase::singleChoice().
   * @param array $partial_selections
   *   TODO: &$partial_selections is passed by reference as a quick & dirty way of keeping
   *   partial selections between different calls to the method, so that the user
   *   can select a few options, filter by string, and still keep the selections
   *   made so far.
   *
   * @see IDCStepChoiceBase::singleChoice().
   * @see drush_choice_multiple().
   */
  public static function multipleChoice($options, $defaults = FALSE, $prompt = 'Select some numbers.', $label = '!value', $mark = '!value (selected)', $min = 0, $max = NULL, $final_options = array(), $settings = array(), &$partial_selections = array()) {
    // Initialise settings.
    $settings = array_merge(array(
      'wildcard_values' => array(),
      'wildcard_regex' => array(),
    ), $settings);

    if (isset($settings['accept_csv']) && $settings['accept_csv']) {
      $settings['wildcard_regex'][] = '/^(\d{1,}|\[\d{1,}\-\d{1,}\])/';
    }

    // Combine static defaults with partial selections made so far.
    if (is_array($defaults)) {
      $defaults = array_merge($defaults, $partial_selections);
    }
    $selections = array();

    // Load default selections.
    if (is_array($defaults)) {
      foreach ($defaults as $default_value) {
        if (in_array($default_value, array_keys($options))) {
          $selections[] = $default_value;
        }
      }
    }
    elseif ($defaults === TRUE) {
      $selections = array_keys($options);
    }
    $complete = FALSE;
    $final_builtin = array();
    if (empty($final_options)) {
      $final_builtin['done'] = dt('Done');
    }
    $final_options_keys = array_keys($final_options);
    while (TRUE) {
      $current_options = $options;
      // Mark selections.
      foreach ($selections as $selection) {
        $current_options[$selection] = dt($mark, array('!key' => $selection, '!value' => $options[$selection]));
      }
      // Add final options, if the minimum number of selections has been reached.
      if (count($selections) >= $min) {
        $current_options = array_merge($current_options, $final_options, $final_builtin);
      }
      $toggle = self::singleChoice($current_options, $prompt, $label, array(), $settings);
      if ($toggle === FALSE) {
        return FALSE;
      }
      // Don't include the built in final option in the return value.
      if (count($selections) >= $min && empty($final_options) && $toggle == 'done') {
        return $selections;
      }
      // Toggle the selected value.
      $item = array_search($toggle, $selections);
      if ($item === FALSE) {
        $input_processed = FALSE;
        // Value doesn't match any available option. Check wildcard_values.
        foreach ($settings['wildcard_values'] as $wildcard_value) {
          if (strpos($toggle, $wildcard_value) === 0) {
            return $toggle;
          }
        }
        foreach ($settings['wildcard_regex'] as $wildcard_regex) {
          if (preg_match($wildcard_regex, $toggle)) {
            switch ($wildcard_regex) {
              // Dealing with CSV (or space-separated values).
              case '/^(\d{1,}|\[\d{1,}\-\d{1,}\])/' && isset($settings['accept_csv']) && $settings['accept_csv']:
                $current_options_keys = array_keys($current_options);
                $csv_values = array();
                preg_match_all('/(\d{1,}|\[\d{1,}\-\d{1,}\])/', $toggle, $csv_values);

                // Process each separated value, either as a single number, or
                // as a range of options (e.g: 3,[5-8],9)...
                foreach ($csv_values[0] as $value) {
                  // Single number entered. Select it or unselect it, depending
                  // on its current value.
                  if ((bool) preg_match('/^\d{1,}$/', $value)) {
                    $input_processed = TRUE;
                    $value_selected = in_array($current_options_keys[$value - 1], $selections);
                    if (!$value_selected) {
                      array_unshift($selections, $current_options_keys[$value - 1]);
                    }
                    else if ($value_selected) {
                      unset($selections[array_search($current_options_keys[$value - 1], $selections)]);
                    }
                  }
                  // Range of values entered. Separate and process them. Ranges
                  // are used only for selection, but not to deselect any
                  // options.
                  else {
                    $input_processed = TRUE;
                    $numbers = array();
                    preg_match_all('/\d{1,}/', $value, $numbers);
                    for ($i = $numbers[0][0]; $i <= $numbers[0][1]; $i++) {
                      array_unshift($selections, $current_options_keys[$i - 1]);
                    }
                  }
                }
                break;
              default:
                return $toggle;
            }
          }
        }
        // If no wildcard values or regex were triggered, select the option
        // chosen.
        if ($input_processed === FALSE) {
          array_unshift($selections, $toggle);
        }
      }
      else {
        unset($selections[$item]);
      }

      // Run through all partial selections, and unset those contained on the
      // current $options list, that are not present in the $selections list
      // anymore (because they've been unselected in this iteration).
      if (!empty($partial_selections)) {
        foreach ($partial_selections as $key => $partial_selection) {
          if (in_array($partial_selection, $options) && !in_array($partial_selection, $selections)) {
            unset($partial_selections[$key]);
          }
        }
      }
      // Add the new selections to the partial selections array.
      $partial_selections = array_merge($partial_selections, $selections);

      // If the user selected one of the final options, return.
      if (count($selections) >= $min && in_array($toggle, $final_options_keys)) {
        return $selections;
      }
      // If the user selected too many options, drop the oldest selection.
      if (isset($max) && count($selections) > $max) {
        array_pop($selections);
      }
    }
  }


}
