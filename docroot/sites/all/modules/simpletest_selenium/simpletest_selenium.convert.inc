<?php

/**
 * @file
 * Provide a conversion routine for Selenium IDE PHP output to SimpleTest.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Convert Selenium IDE PHP output to SimpleTest format.
 *
 * @param $string
 *   Selenium IDE PHP ouput to convert.
 * @return
 *   SimpleTest format of Selenium IDE PHP test.
 */
function simpletest_selenium_convert($string) {
  $converter = new simpletest_selenium_converter($string);
  return $converter->convert();
}

/**
 * Converts Selenium IDE PHP output to SimpleTest format.
 */
class simpletest_selenium_converter {

  /**
   * Lines of Selenium test.
   *
   * @var array
   */
  protected $lines = array();

  /**
   * Selenium method to convert.
   *
   * @var array
   */
  protected $methods = array(
    'open',
    'click',
    'isTextPresent',
    'isElementPresent',
    'getTitle',
    'getText',
    'type',
  );

  /**
   * Converted Selenium calls as SimpleTest calls.
   *
   * @var array
   */
  protected $calls = array();

  /**
   * Internal associative array of form values.
   *
   * @var array
   */
  protected $edit = array();

  /**
   * Create a converted using Selenium IDE PHP output.
   *
   * @param $string
   *   Selenium IDE PHP ouput to convert.
   */
  public function __construct($string) {
    $this->lines = explode("\n", $string);
  }

  /**
   * Peform conversion.
   *
   * @return
   *   SimpleTest format of Selenium IDE PHP test.
   */
  public function convert() {
    // Cycle through each line of input and determine if the line can be
    // converted by checking if any of the supported methods were found.
    foreach ($this->lines as $line) {
      foreach ($this->methods as $method) {
        if (strpos($line, '$this->' . $method) !== FALSE) {
          // Call the appropriate conversion method.
          $handler = 'convert_' . $method;
          $this->$handler($line);
        }
      }
    }

    // Format the SimpleTest calls.
    return $this->to_string();
  }

  protected function convert_open($line) {
    if (preg_match('/\$this\->open\("(.*?)"\)/', $line, $match)) {
      $url = $match[1] == '/' ? '' : $match[1];
      $this->add_call('drupalGet', array("'" . $url . "'"));
    }
  }

  protected function convert_click($line) {
    if (preg_match('/\$this\->click\("link=(.*?)"\)/', $line, $match)) {
      $this->add_call('clickLink', array("t('" . $match[1] . "')"));
    }
    elseif (preg_match('/\$this\->click\("(.*?)"\)/', $line, $match)) {
      $out = str_replace("\n", "\n    ", '$edit = ' . var_export($this->edit, TRUE) . ';');
      $this->edit = array();

      $this->add_call('drupalPost', array('NULL', '$edit', "t('" . $match[1] . "')", $out));
    }
  }

  protected function convert_isTextPresent($line) {
    if (preg_match('/\$this\->isTextPresent\("(.*?)"\)/', $line, $match)) {
      $this->add_call('assertText', array("t('" . $match[1] . "')"));
    }
  }

  protected function convert_isElementPresent($line) {
    if (preg_match('/\$this\->isElementPresent\("(.*?)"\)/', $line, $match)) {
      $this->add_call('assertTrue', array('$this->xpath("' . $match[1] . '")'));
    }
  }

  protected function convert_getTitle($line) {
    if (preg_match('/\$this\->assertEquals\("(.*?)", \$this\->getTitle\(\)\)/', $line, $match)) {
      $this->add_call('assertTitle', array("t('" . $match[1] . "')"));
    }
    elseif (preg_match('/if \("(.*?)" == \$this\->getTitle\(\)\)/', $line, $match)) {
      $this->add_call('assertTitle', array("t('" . $match[1] . "')"));
    }
  }

  protected function convert_getText($line) {
    $wrap = array('trim(html_entity_decode(strip_tags(', ')))');
    if (preg_match('/\$this\->assertEquals\("(.*?)", \$this\->getText\("(.*?)"\)\)/', $line, $match)) {
      $this->add_call('assertEqual', array("t('" . $match[1] . "')", $wrap[0] . '$this->xpath("' . $match[2] . '")' . $wrap[1]));
    }
    elseif (preg_match('/if \("(.*?)" == \$this\->getText\("(.*?)"\)\)/', $line, $match)) {
      $this->add_call('assertEqual', array("t('" . $match[1] . "')", $wrap[0] . '$this->xpath("' . $match[2] . '")' . $wrap[1]));
    }
  }

  protected function convert_type($line) {
    if (preg_match('/\$this\->type\("(.*?)", "(.*?)"\)/', $line, $match)) {
      $this->edit[$match[1]] = $match[2];
    }
  }

  protected function add_call($method, array $arguments) {
    $this->calls[] = array('method' => $method, 'arguments' => $arguments);
  }

  /**
   * Format the SimpleTest calls as a full test case.
   *
   * @return
   *   SimpleTest test case.
   */
  protected function to_string() {
    $lines = array();
    foreach ($this->calls as $call) {
      if (in_array($call['method'], array('drupalGet', 'drupalPost', 'clickLink'))) {
        $lines[] = '';

        // Print fourth parameter before drupalPost().
        if ($call['method'] == 'drupalPost') {
          $lines[] = $call['arguments'][3];
          unset($call['arguments'][3]);
        }
      }

      $lines[] = '$this->' . $call['method'] . '(' . implode(', ', $call['arguments']) . ');';
    }

    return "<?php

/**
 * @file
 */

class Example extends DrupalWebTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Example',
      'description' => '...',
      'group' => 'Example',
    );
  }

  protected function testExample() {
    " . trim(implode("\n    ", $lines)) . "
  }
}
";
  }
}
