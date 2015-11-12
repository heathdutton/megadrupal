<?php

/**
 * @file
 * Class ObtainDate
 *
 * Contains a collection of stackable finders that can be arranged
 * as needed to obtain a date content.
 */

/**
 * {@inheritdoc}
 */
class ObtainDate extends ObtainHtml {


  /**
   * Finder method to find the first .newsRight.
   *
   * @return string
   *   The string that was found
   */
  protected function pluckClassNewsRightLast() {
    $element = $this->queryPath->top('.newsRight')->last();
    $this->setElementToRemove($element);

    return $element->text();
  }


  /**
   * Finder method to find dates by its accompanying text.
   *
   * @return string
   *   The string that was found
   */
  protected function pluckProbableDate() {
    // Selectors to run through.
    $selectors = array(
      '.BottomLeftContent',
      '#dateline',
      'p',
      '.newsLeft',
      '.newsRight',
    );
    // Text strings to search for.
    $search_strings = array(
      "FOR IMMEDIATE RELEASE",
      "NEWS RELEASE SUMMARY",
      "FOR IMMEDIATE  RELEASE",
      "IMMEDIATE RELEASE",
    );
    // Loop through the selectors.
    foreach ($selectors as $selector) {
      // Loop through the search strings.
      foreach ($search_strings as $search_string) {
        // Search for the string.
        $element = HtmlCleanUp::matchText($this->queryPath, $selector, $search_string);

        if (!empty($element)) {
          $text = $element->text();

          // Remove accompanying text and clean string.
          $text = str_replace($search_string, '', $text);
          $text = $this->cleanString($text);
          $valid = $this->validateString($text);

          if ($valid) {
            $this->setElementToRemove($element);
            MigrationMessage::makeMessage("pluckProbableDate| selector: @selector  search string: @search_string", array('@selector' => $selector, '@search_string' => $search_string), WATCHDOG_DEBUG);

            return $text;
          }
        }
      }
    }

    return '';
  }

  /**
   * Method for returning the table cell at row 1, column 1.
   *
   * @return string
   *   The string found.
   */
  protected function pluckTableRow1Col1() {
    $table = $this->queryPath->find("table");
    $text = $this->pluckFromTable($table, 1, 1);

    return $text;
  }

  /**
   * Cleans $text and returns it.
   *
   * @param string $text
   *   Text to clean and return.
   *
   * @return string
   *   The cleaned text.
   */
  public static function cleanString($text) {

    // There are also numeric html special chars, let's change those.
    module_load_include('inc', 'migration_tools', 'includes/migration_tools');
    $text = StringCleanUp::decodehtmlentitynumeric($text);

    // We want out titles to be only digits and ascii chars so we can produce
    // clean aliases.
    $text = StringCleanUp::convertNonASCIItoASCII($text);

    // Checking again in case another process rendered it non UTF-8.
    $is_utf8 = mb_check_encoding($text, 'UTF-8');

    if (!$is_utf8) {
      $text = StringCleanUp::fixEncoding($text);
    }

    // Remove some strings that often accompany dates.
    $remove = array(
      'FOR',
      'IMMEDIATE',
      'RELEASE',
      'NEWS RELEASE SUMMARY –',
      'CONTACT:',
      'Press Release',
      'news',
      'press',
      'release',
      'updated',
      'update',
      'sunday',
      'monday',
      'tuesday',
      'wednesday',
      'thursday',
      'friday',
      'saturday',
      // Intentional mispellings.
      'thurday',
      'wendsday',
      'firday',
    );
    // Replace these with nothing.
    $text = str_ireplace($remove, '', $text);

    $remove = array(
      '.',
      ',',
      "\n",
    );
    // Replace these with spaces.
    $text = str_ireplace($remove, ' ', $text);

    // Fix mispellings and abbreviations.
    $replace = array(
      'septmber' => 'september',
      'arpil' => 'april',
      'febraury' => 'february',
    );
    $text = str_ireplace(array_keys($replace), array_values($replace), $text);

    // Remove multiple spaces.
    $text = preg_replace('/\s{2,}/u', ' ', $text);
    // Remove any text following the 4 digit year.
    $years = range(1995, 2015);
    foreach ($years as $year) {
      $pos = strpos($text, (string) $year);
      if ($pos !== FALSE) {
        $text = substr($text, 0, ($pos + 4));
        break;
      }
    }

    // Remove white space-like things from the ends and decodes html entities.
    $text = StringCleanUp::superTrim($text);

    return $text;
  }

  /**
   * Returns array of month names.
   *
   * @return array
   *   One entry for each month name.
   */
  public static function returnMonthNames() {
    return array(
      'January',
      'February',
      'March',
      'April',
      'May',
      'June',
      'July',
      'August',
      'September',
      'October',
      'November',
      'December',
    );
  }

  /**
   * Evaluates $string for presence of more than 1 element from month array.
   *
   * @param string $string
   *   The string to validate.
   *
   * @return bool
   *   TRUE if more than one Month found in string.  FALSE if not.
   */
  public static function searchStringDoubleMonth($string) {
    $months = self::returnMonthNames();
    $count = 0;
    $bmultiple = FALSE;
    foreach ($months as $month) {
      $lower_month = strtolower($month);
      $lower_string = strtolower($string);
      if (strstr($lower_string, $lower_month)) {
        $count++;
        if ($count > 1) {
          $bmultiple = TRUE;
          break;
        }
      }
    }
    return $bmultiple;
  }

  /**
   * Searches for a date range overlapping months, returns single date.
   *
   * Looks specifically for month names as defined in self::returnMonthNames().
   * In the case of multiple months found in $string, returns the latter date.
   * Example: "March 28 - April 4", returns April 4.
   *
   * @param string $string
   *   The string to clean.
   *
   * @return string
   *   Return string includes only 1 month.
   */
  public static function removeMultipleMonthRange($string) {
    $bmultiple = self::searchStringDoubleMonth($string);
    if ($bmultiple === TRUE) {
      $explode = explode('-', $string);
      if (!empty($explode[1])) {
        $latter_date = $explode[1];
        return $latter_date;
      }
    }

    return $string;
  }

  /**
   * Evaluates $string and if it checks out, returns TRUE.
   *
   * @param string $string
   *   The string to validate.
   *
   * @return bool
   *   TRUE if possibleText can be used as a date.  FALSE if it cant.
   */
  protected function validateString($string) {
    // Run through any evaluations.  If it makes it to the end, it is good.
    // Case race, first to evaluate TRUE aborts the text.
    switch (TRUE) {
      // List any cases below that would cause it to fail validation.
      case empty($string):
      case is_object($string):
      case is_array($string):
      case (strlen($string) < 7):
        // If we can't form a date out of it, it must not be a date.
      case !strtotime($string):
        return FALSE;

      default:
        return TRUE;
    }
  }

}
