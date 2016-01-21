<?php
/**
 * @file
 * Drupal RestMini module.
 */


class RestMini {

  /**
   * @var integer
   */
  protected static $errorCodeOffset = 0;

  /**
   * @var array
   */
  protected static $errorCodes = array();

  /**
   * Get error code by name, or code list, or code range.
   *
   * @param string $name
   *   Non-empty: return code by name (defaults to 'unknown')
   *   Default: empty (~ return codes list).
   * @param boolean $range
   *   TRUE: return code range array(N-first, N-last).
   *   Default: FALSE (~ ignore argument).
   *
   * @return integer|array
   */
  public static function errorCode($name = '', $range = FALSE) {
    static $codes;

    if ($name) {
      return static::$errorCodeOffset
        + (array_key_exists($name, static::$errorCodes) ? static::$errorCodes[$name] : static::$errorCodes['unknown']);
    }

    if ($range) {
      return array(
        static::$errorCodeOffset,
        // Range of sub modules should only be 100, to allow for all sub modules within an overall range of 1000.
        static::$errorCodeOffset + 99
      );
    }

    if (!$codes) {
      $codes = static::$errorCodes; // Copy.
      if (($offset = static::$errorCodeOffset)) {
        foreach ($codes as &$code) {
          $code += $offset;
        }
        unset($code); // Iteration ref.
      }
    }

    return $codes;
  }


  /**
   * Must not be overridden as the property is used by methods (of RestMiniService) that are final.
   * Should be final, but PHP doesn't support final properties (only methods).
   *
   * @var array
   */
  protected static $supportedMethods = array('HEAD', 'GET', 'POST', 'PUT', 'DELETE');


  /**
   * Get status text of a status code.
   *
   * Does not take HTTP method into consideration - a 201 as response to PUT should rather be 'Updated' than 'Created'.
   *
   * @param integer $code
   *
   * @return string
   *   If unsupported status $code: 'Unknown Status'.
   */
  final public static function statusText($code) {
    // Stringify code to prevent coercion fault; a string matches any number in a PHP switch.
    switch ('' . $code) {
      case '200':
        return 'OK';
      case '201':
        return 'Created';
      case '204':
        return 'No Content';
      case '301':
        return 'Moved Permanently';
      case '302':
        return 'Found';
      case '304':
        return 'Not Modified';
      case '400':
        return 'Bad Request';
      case '401':
        return 'Unauthorized';
      case '403':
        return 'Forbidden';
      case '404':
        return 'Not Found';
      case '405':
        return 'Method Not Allowed';
      case '406':
        return 'Not Acceptable';
      case '410':
        return 'Gone';
      case '415':
        return 'Unsupported Media Type';
      case '500':
        return 'Internal Server Error';
      case '501':
        return 'Not Implemented';
      case '502':
        return 'Bad Gateway';
      case '503':
        return 'Service Unavailable';
      case '504':
        return 'Gateway Timeout';
    }
    return 'Unknown Status';
  }

  /**
   * Get info about appropriate use of HTTP response status codes to HTTP request methods.
   *
   * @param string $method
   *   Non-empty: list status codes usable by that HTTP method.
   *   Default: empty (~ list all status codes and the HTTP methods that may use them).
   *
   * @return array
   */
  final public static function statusCodes($method = '') {
    static $codes, $all_methods;

    if (!$codes) {
      $all_methods = self::$supportedMethods;
      $codes = array(
        200 => array('HEAD', 'GET', 'DELETE'), // OK.
        201 => array('POST', 'PUT'), // Created|Updated (with payload).
        204 => array('POST', 'PUT', 'DELETE'), // No Content.
        301 => $all_methods, // Moved Permanently.
        302 => $all_methods, // Found.
        304 => array('HEAD', 'GET'),
        400 => $all_methods, // Bad Request.
        401 => $all_methods, // Unauthorized.
        403 => $all_methods, // Forbidden.
        404 => $all_methods, // Not Found.
        405 => $all_methods, // Method Not Allowed.
        406 => $all_methods, // Not Acceptable.
        410 => $all_methods, // Gone.
        415 => array('POST', 'PUT'), // Unsupported Media Type.
        500 => $all_methods, // Internal Server Error.
        501 => $all_methods, // Not Implemented.
        502 => $all_methods, // Bad Gateway.
        503 => $all_methods, // Service Unavailable.
        504 => $all_methods, // Gateway Timeout.
      );
    }
    if (!$method) {
      return $codes;
    }
    $arr = array();
    if (in_array($method, $all_methods, TRUE)) {
      foreach ($codes as $status => $methods) {
        if (in_array($method, $methods)) {
          $arr[$status] = $status;
        }
      }
    }
    return $arr;
  }

  /**
   * Check if a successful status - 200, 201, 204, 304 - and that status and optionally content length matches HTTP method.
   *
   * Content length: consider that neither that neither '0' nor an empty array is an empty response.
   *
   * @param string $method
   *   HEAD|GET|POST|PUT|DELETE.
   * @param integer $status
   *   Integer: check against method.
   *   No argument: return array of successful status code of that method.
   * @param integer $contentLength
   *   Integer: check against method and status.
   *   No argument: don't consider content length.
   *
   * @return boolean|array|NULL
   *   Boolean: whether status (and optionally content length) matches method.
   *   Array: list of successful status code of that method.
   *   NULL: on error.
   */
  final public function statusSuccess($method, $status = 0, $contentLength = 0) {
    $nArgs = func_num_args();

    switch ($method) {
      case 'HEAD':
      case 'GET':
        $codes = array(200, 304);
        break;
      case 'POST':
      case 'PUT':
        $codes = array(201, 204); // @todo: W3C says 200 is fine too.
        break;
      case 'DELETE':
        $codes = array(200, 204);
        break;
      default:
        static::log(
          'restmini',
          'HTTP method not supported',
          NULL,
          $method,
          WATCHDOG_ERROR
        );
        return NULL;
    }

    if ($nArgs == 1) {
      return $codes;
    }

    if (!in_array($status, $codes)) {
      return FALSE;
    }
    if ($nArgs == 2) {
      return TRUE;
    }

    return ($method == 'HEAD' || $status == 204 || $status == 304) ? !$contentLength : !!$contentLength;
  }


  /**
   * Supported validation pattern rules.
   *
   * The 'prepared' (~ prepared/checked by ::validationPattern()) rule is deliberately not supported.
   *
   * @see RestMini::validate()
   *
   * @var array
   */
  private static $validationPatternRulesSupported = array(
    'pattern', // Merge with a(nother) predefined pattern.
    'extends', // (array) Has previously been merged with other predefined pattern(s).
    'title',
    'type',
    'optional',
    'default',
    'except_value',
    'except_type',
    'no_trim',
    'exact_length',
    'max_length',
    'min_length',
    'truncate',
    'min',
    'max',
    'enum', // Cannot be combined with neither check nor regex.
    'check', // Check against a predefined check, see restmini_validation_check().
    'regex',
  );

  /**
   * Supported validation pattern data types.
   *
   * @see RestMini::validate()
   *
   * @var array
   */
  private static $validationPatternTypesSupported = array(
    'string',
    'integer',
    'double',
    'float',
    'array',
    'object',
    'boolean',
    'NULL',
  );

  /**
   * Get a predefined pattern, or prepare a custom pattern, or prepare a custom pattern and merge it with a predefined, or get all predefineds.
   *
   * A validation pattern is a set of validation rules, usable by the ::validate() method.
   *
   *  Supported pattern rules (all optional):
   *  - (str) pattern; merge with a(nother) predefined pattern
   *  - (arr) extends; has previously been merged with other predefined pattern(s)
   *  - (str) title; used for failure message
   *  - (str) type; default string, string|integer|float|array|object|boolean|NULL
   *  - (bool) optional; default not
   *  - (mixed) default; only applicable for unset value and for empty (type:)string
   *  - (mixed) except_value; if that and actual value are exactly the same, then it obsoletes all other rules but 'optional' (cannot be array|object)
   *  - (string) except_type; if that and actual data type stringed are the same, then no type mismatch (only 'array'|'object'|'boolean'|'NULL' allowed)
   *  - (bool) no_trim; default not ~ do trim() a type:string value
   *  - (int) exact_length
   *  - (int) max_length
   *  - (int) min_length
   *  - (int) truncate; default don't, only applicable for type:string
   *  - (int) min;
   *  - (int) max;
   *  - (arr) enum; comparison performed type-safe if type:integer/float (cannot be combined with neither check nor regex)
   *  - (str) check; a predefined check, see restmini_validation_check()
   *  - (str) regex; a custom regular expression
   *
   * @throws Exception
   *   If predefined pattern by $name doesn't exist.
   *   Pattern contains unsupported rule(s).
   *   Pattern contains unsupported data 'type' rule.
   *   Pattern 'enum' rule that isn't array.
   *   Pattern contains 'enum' rule AND 'check' or 'regex' rule.
   *   Pattern contains unsupported 'check' rule.
   *   Pattern contains 'regex' rule, that is too short or missis leading or trailing slash delimiter.
   *   If failing to include this module's *.validation_patterns.inc file (and non-existent patterns cache).
   *
   * @param string $name
   *   Default: empty (~ use $custom, or return all).
   *   Throws exception it non-empty $name doesn't exist.
   *
   * @param array $custom
   *   Overrides predefined by $name, if non-empty $name.
   *   Default: empty (~ get by $name, or return all).
   *
   * @return array
   */
  final public static function validationPattern($name = '', array $custom = array()) {
    static $patterns;
    if (!$patterns) {
      if (!($patterns = cache_get('restmini__validation_patterns')) || !($patterns = $patterns->data)) {
        // Rebuild cache.
        // Get own, default, patterns.
        // Can't use module_load_include() here, because the $validation_patterns variable declared in file
        // would be set in module_load_include()'s scope, not this method's scope.
        include 'restmini.validation_patterns.inc';
        if (!isset($validation_patterns)) {
          throw new Exception('Failed to include validation patterns');
        }
        $patterns =& $validation_patterns;
        // Get from hook_restmini_validation_patterns() implementations.
        $modules = module_implements('restmini_validation_patterns');
        foreach ($modules as $module_name) {
          $function = $module_name . '_restmini_validation_patterns';
          $patterns = array_merge(
            $patterns,
            $function()
          );
        }
        unset($modules);

        module_load_include('inc', 'restmini', 'restmini_validation_check');
        $checks = restmini_validation_check();

        foreach ($patterns as $key => &$pattern) {

          // A pattern containing an unsupported rule could easily result in insufficient validation.
          if (($unsupportedRules = array_diff(array_keys($pattern), self::$validationPatternRulesSupported))) {
            $em = 'Validation pattern[' . $key . '] contains unsupported rule(s)';
            static::log(
              'restmini',
              $em,
              NULL,
              array(
                'pattern' => $pattern,
                'unsupported' => $unsupportedRules
              ),
              WATCHDOG_ERROR
            );

            throw new Exception($em . '[' . join(', ', $unsupportedRules) . ']');
          }

          $prepend = array();
          // Make sure every pattern has an 'optional' rule (default false).
          if (!isset($pattern['optional'])) {
            $prepend['optional'] = FALSE;
          }

          // Make sure every patten has a 'type' (and supported); default 'string'.
          if (empty($pattern['type'])) {
            $prepend['type'] = 'string';
          }
          elseif (!in_array($pattern['type'], self::$validationPatternTypesSupported, TRUE)) {
            $em = 'Validation pattern[' . $key . '] contains unsupported \'type\' rule';
            static::log(
              'restmini',
              $em,
              NULL,
              $pattern,
              WATCHDOG_ERROR
            );

            throw new Exception($em . '[' . $pattern['type'] . ']');
          }
          elseif ($pattern['type'] == 'double') {
            $pattern['type'] = 'float';
          }

          if ($prepend) {
            $pattern = array_merge(
              $prepend,
              $pattern
            );
          }

          if (!empty($pattern['enum'])) {
            // 'enum' rule must be array.
            if (!is_array($pattern['enum'])) {
              $em = 'Validation pattern[' . $key . '] \'enum\' is not array but ' . gettype($pattern['enum']);
              static::log(
                'restmini',
                $em,
                NULL,
                $pattern,
                WATCHDOG_ERROR
              );

              throw new Exception($em);
            }

            // 'enum' cannot be combined with neither 'check' nor 'regex'.
            if (!empty($pattern['check']) || !empty($pattern['regex'])) {
              $em = 'Validation pattern[' . $key . '] cannot contain \'enum\' rule AND \'check\' or \'regex\', saw \''
                . (!empty($pattern['check']) ? 'check' : 'regex') . '\'';
              static::log(
                'restmini',
                $em,
                NULL,
                $pattern,
                WATCHDOG_ERROR
              );

              throw new Exception($em);
            }
          }
          // 'enum' cannot be combined with neither 'check' nor 'regex'.
          else {
            // Make sure 'check' rule is supported.
            if (!empty($pattern['check']) && !isset($checks[$pattern['check']])) {
              $em = 'Validation pattern[' . $key . '] contains unsupported \'check\' rule[' . $pattern['check'] . ']';
              static::log(
                'restmini',
                $em,
                NULL,
                $pattern,
                WATCHDOG_ERROR
              );

              throw new Exception($em);
            }

            // Make sure 'regex' looks like a valid regular expression.
            if (!empty($pattern['regex'])) {
              $regex = $pattern['regex'];
              $em = '';
              if (strlen($regex) < 3) {
                $em = 'is too short to contain leading and trailing slash delimiter plus at least a single character class';
              }
              elseif ($regex{0} !== '/') {
                $em = 'misses leading slash delimiter';
              }
              elseif ($regex{strlen($regex) - 1} !== '/') {
                $em = 'misses trailing slash delimiter';
              }
              if ($em) {
                $em = 'Validation pattern[' . $key . '] \'regex\' rule[' . $regex . '] ' . $em;
                static::log(
                  'restmini',
                  $em,
                  NULL,
                  $pattern,
                  WATCHDOG_ERROR
                );

                throw new Exception($em);
              }
            }
          }

          // Flag that the pattern is ready for use.
          $pattern['prepared'] = TRUE;
        }
        unset($pattern); // Iteration ref.

        cache_set('restmini__validation_patterns', $patterns, 'cache', CACHE_PERMANENT);
      }
    }

    // Work.
    if ($name) {
      if (!array_key_exists($name, $patterns)) {
        $em = 'Validation pattern[' . $name . '] doesn\'t exist';
        static::log(
          'restmini',
          $em,
          NULL,
          array(
            'name' => $name,
            'custom' => $custom
          ),
          WATCHDOG_ERROR
        );

        throw new Exception($em);
      }

      if (!$custom) {
        return $patterns[$name];
      }
    }

    if ($custom) {
      // Does it already extend some predefined pattern?
      $extendsAny = !empty($custom['extends']);

      // The 'pattern' rule means that this custom - or (actually) perhaps predefined - pattern should be merged with a(nother) predefined pattern.
      if (!empty($custom['pattern'])) {
        $extendName = $custom['pattern'];
        // Don't extend the same pattern twice (later).
        unset($custom['pattern']);

        // Don't extend the same pattern twice (now).
        if (!$extendsAny || !in_array($extendName, $custom['extends'])) {
          if (!isset($patterns[$extendName])) {
            $em = 'Custom validation pattern contains \'pattern\' rule[' . $extendName . '], but such predefined pattern doesn\'t exist';
            static::log(
              'restmini',
              $em,
              NULL,
              array(
                'pattern' => $custom,
              ),
              WATCHDOG_ERROR
            );

            throw new Exception($em);
          }

          if (!$name) {
            // If no $name, and no other rules than 'pattern': simply use that predefined pattern'.
            if (empty($custom)) {
              return $patterns[$extendName];
            }

            // If no $name, simply use the 'pattern' rule as $name.
            $name = $extendName;
          }
          elseif ($name != $extendName) {
            // Extend the 'pattern' rule first.
            $custom = self::validationPattern($extendName, $custom);
            // Now that the custom pattern already extends one predefined pattern, we don't have check it (again).
            $custom['extends'][] = $name;

            return array_merge(
              $patterns[$name],
              $custom
            );
          }
          // ...else: The 'pattern' rule is the same as $name, so we simply ignore 'pattern'.
        }
      }

      // Don't check/prepare twice.
      if (empty($custom['prepared'])) {

        if (($unsupportedRules = array_diff(array_keys($custom), self::$validationPatternRulesSupported))) {
          $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
            . ' contains unsupported rule(s)[' . join(', ', $unsupportedRules) . ']';
          static::log(
            'restmini',
            $em,
            NULL,
            array(
              'name' => $name,
              'custom' => $custom,
              'unsupported' => $unsupportedRules
            ),
            WATCHDOG_ERROR
          );

          throw new Exception($em);
        }

        // Secure 'optional' and 'type'.
        $prepend = array();

        // If custom pattern is on it's own, we must secure the 'optional' rule (and default to non-optional).
        if (!$name && !isset($custom['optional'])) {
          $prepend['optional'] = FALSE;
        }

        // 'type' must be supported.
        if (!empty($custom['type'])) {
          if (!in_array($custom['type'], self::$validationPatternTypesSupported, TRUE)) {
            $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
              . ' contains unsupported \'type\' rule[' . $custom['type'] . ']';
            static::log(
              'restmini',
              $em,
              NULL,
              array(
                'name' => $name,
                'custom' => $custom,
              ),
              WATCHDOG_ERROR
            );

            throw new Exception($em);
          }
          elseif ($custom['type'] == 'double') {
            $custom['type'] = 'float';
          }
        }
        // If custom pattern is on it's own, we must secure the 'type' rule.
        elseif (!$name) {
          $custom['type'] = 'string';
        }

        if ($prepend) {
          $custom = array_merge(
            $prepend,
            $custom
          );
        }

        if (!empty($custom['enum'])) {
          // 'enum' rule must be array.
          if (!is_array($custom['enum'])) {
            $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
              . ' \'enum\' is not array but ' . gettype($custom['enum']);
            static::log(
              'restmini',
              $em,
              NULL,
              array(
                'name' => $name,
                'custom' => $custom,
              ),
              WATCHDOG_ERROR
            );

            throw new Exception($em);
          }

          // 'enum' cannot be combined with neither 'check' nor 'regex'.
          if (!empty($custom['check']) || !empty($custom['regex'])) {
            $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
              . ' cannot contain \'enum\' rule AND \'check\' or \'regex\', saw \'' . (!empty($custom['check']) ? 'check' : 'regex') . '\'';
            static::log(
              'restmini',
              $em,
              NULL,
              array(
                'name' => $name,
                'custom' => $custom,
              ),
              WATCHDOG_ERROR
            );

            throw new Exception($em);
          }
        }
        // 'enum' cannot be combined with neither 'check' nor 'regex'.
        else {
          // Make sure 'check' rule is supported.
          if (!empty($custom['check'])) {
            module_load_include('inc', 'restmini', 'restmini_validation_check');
            $checks = restmini_validation_check();

            if (!isset($checks[$custom['check']])) {
              $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
                . ' contains unsupported \'check\' rule[' . $custom['check'] . ']';
              static::log(
                'restmini',
                $em,
                NULL,
                array(
                  'name' => $name,
                  'custom' => $custom,
                ),
                WATCHDOG_ERROR
              );

              throw new Exception($em);
            }
          }

          // Make sure 'regex' looks like a valid regular expression.
          if (!empty($custom['regex'])) {
            $regex = $custom['regex'];
            $em = '';
            if (strlen($regex) < 3) {
              $em = 'is too short to contain leading and trailing slash delimiter plus at least a single character class';
            }
            elseif ($regex{0} !== '/') {
              $em = 'misses leading slash delimiter';
            }
            elseif ($regex{strlen($regex) - 1} !== '/') {
              $em = 'misses trailing slash delimiter';
            }
            if ($em) {
              $em = 'Custom validation pattern' . (!$name ? '' : ', to be merged with predefined pattern[' . $name . '],')
                . ' \'regex\' rule[' . $regex . '] ' . $em;
              static::log(
                'restmini',
                $em,
                NULL,
                array(
                  'name' => $name,
                  'custom' => $custom,
                ),
                WATCHDOG_ERROR
              );

              throw new Exception($em);
            }
          }
        }

        // Flag that the pattern is ready for use.
        $custom['prepared'] = TRUE;
      }

      if (!$name) {
        return $custom;
      }

      if (!$extendsAny) {
        $custom['extends'] = array($name);
      }
      else {
        $custom['extends'][] = $name;
      }

      return array_merge(
        $patterns[$name],
        $custom
      );
    }

    return $patterns;
  }

  /**
   * @var array
   */
  protected static $validationFailures = array();

  /**
   * Get validation failures - key-message pairs - recorded by validate().
   *
   * @param boolean $keep
   *   Default: FALSE (~ flush upon retrieval).
   *
   * @return array
   */
  final public static function validationFailures($keep = FALSE) {
    if (!$keep) {
      $a = self::$validationFailures;
      self::$validationFailures = array();
      return $a;
    }
    return self::$validationFailures;
  }

  /**
   * Validates the existence or value of $key in $container, using $pattern as rule set.
   *
   *  Sequence of checks and conversions for non-number types:
   *  1) trim
   *  2) exact_length/max_length/min_length
   *  3) truncate
   *  4) check
   *  5) enum or regex (mutually exclusive)
   *
   *  A string value may get altered:
   *  - trimmed, unless pattern contains 'no_trim' flag
   *  - set, if empty and pattern contains a 'default' value
   *  - truncated, if pattern contains a 'truncate' value
   *  - converted to integer or float, if pattern 'type' is either
   *
   *  Sequence of checks and conversions for integer/float:
   *  1) exact_length/max_length/min_length
   *  2) min/max
   *  3) enum or regex (mutually exclusive)
   *
   *  An integer/float value may get altered:
   *  - cast float to integer (or vice versa) if pattern 'type' is the other
   *  - stringed, if pattern 'type' isn't integer/float
   *
   * Numbers can only be 10-scaled, no support for hex or other.
   * If both integer and float are acceptable types for a value, use float.
   *
   * Array/object type and value is only checked for existance and emptiness, and may use a 'default' if non-existent.
   * So the only rules applicable are: title, type, optional and default.
   *
   * @throws Exception
   *   If $pattern is empty.
   *   If $container isn't array or object.
   *   Propagated: if $pattern is un-prepared and fails ::validationPattern() check.
   *
   * @param array $pattern
   *   Validation pattern; a list of rules and options.
   * @param array|object &$container
   *   By reference, because a string value may get trimmed, truncated or set to a default value.
   * @param string|integer $key
   *   Name or index of the value to validate.
   *
   * @return boolean
   */
  public static function validate(array $pattern, &$container, $key) {
    static $included_check = FALSE;

    if (!$pattern) {
      throw new Exception('Validation pattern is empty');
    }

    // Check/prepare pattern, unless already done.
    if (empty($pattern['prepared'])) {
      // ::validationPattern() may throw exception.
      $pattern = self::validationPattern('', $pattern);
    }

    $required = !$pattern['optional'];
    $expectedType = $pattern['type'];

    // Missing?
    // Has to be checked differently if the container is an object.
    if (is_array($container)) {
      $containerIsObject = FALSE;
      if (!$container || !array_key_exists($key, $container)) {
        // Required?
        if ($required) {
          self::$validationFailures[$key] = $key . ' is missing.';
          return FALSE;
        }
        // A default?
        elseif (array_key_exists('default', $pattern)) {
          $container[$key] = $pattern['default'];
        }
        // Apparantly allowed to be non-existing.
        return TRUE;
      }
      $value = $container[$key];
    }
    elseif (is_object($container)) {
      $containerIsObject = TRUE;
      if (!property_exists($container, $key)) {
        // Required?
        if ($required) {
          self::$validationFailures[$key] = $key . ' is missing.';
          return FALSE;
        }
        // A default?
        elseif (array_key_exists('default', $pattern)) {
          $container->{$key} = $pattern['default'];
        }
        // Apparantly allowed to be non-existing.
        return TRUE;
      }
      $value = $container->{$key};
    }
    else {
      throw new Exception('Validation container ' . gettype($container) . ' is not array or object');
    }

    // Whether value has been changed in any way during evaluation.
    $altered = FALSE;

    // Get and evaluate data type.
    // And leave - after some validation - if not scalar.
    $length = 0;
    $actualString = $actualNumber = $actualInteger = FALSE;
    switch (($actualType = gettype($value))) { // IDE: $value IS defined here.

      case 'string':
        $actualString = TRUE;
        // Trim leading/trailing space chars, unless 'no_trim' flag.
        if (empty($pattern['no_trim'])) {
          $value = trim($value);
          $altered = TRUE;
        }
        // Empty string.
        if ($value === '') {
          // Required?
          if ($required) {
            self::$validationFailures[$key] = $key . ' is empty.';
            return FALSE;
          }
          // A default?
          elseif (array_key_exists('default', $pattern)) {
            if (!$containerIsObject) {
              $container[$key] = $pattern['default'];
            }
            else {
              $container->{$key} = $pattern['default'];
            }
            return TRUE;
          }
        }
        else {
          // Check character set.
          if (!drupal_validate_utf8($value)) {
            self::$validationFailures[$key] = $key . ' is not valid UTF-8.';
            return FALSE;
          }
          $length = drupal_strlen($value);
        }
        break;

      case 'integer':
        $actualNumber = $actualInteger = TRUE;
        $length = strlen('' . $value);
        break;

      case 'double':
      case 'float':
        $actualNumber = TRUE;
        $length = strlen('' . $value);
        break;

      case 'array':
        // That type must be expected, or allowed as exception.
        if ($expectedType != 'array'
          && (!isset($pattern['except_type']) || $pattern['except_type'] != 'array')
        ) {
          self::$validationFailures[$key] = $key . ' is array, should be ' . $expectedType . '.';
          return FALSE;
        }
        if (!$value) {
          // Required?
          if ($required) {
            self::$validationFailures[$key] = $key . ' is empty';
            return FALSE;
          }
          // A default?
          elseif (array_key_exists('default', $pattern)) {
            if (!$containerIsObject) {
              $container[$key] = $pattern['default'];
            }
            else {
              $container->{$key} = $pattern['default'];
            }
          }
        }
        return TRUE;

      case 'object':
        // That type must be expected, or allowed as exception.
        if ($expectedType != 'object'
          && (!isset($pattern['except_type']) || $pattern['except_type'] != 'object')
        ) {
          self::$validationFailures[$key] = $key . ' is object, should be ' . $expectedType . '.';
          return FALSE;
        }
        if (!get_object_vars($value)) {
          // Required?
          if ($required) {
            self::$validationFailures[$key] = $key . ' is empty';
            return FALSE;
          }
          // A default?
          elseif (array_key_exists('default', $pattern)) {
            if (!$containerIsObject) {
              $container[$key] = $pattern['default'];
            }
            else {
              $container->{$key} = $pattern['default'];
            }
          }
        }
        return TRUE;

      case 'boolean':
        if ($expectedType != 'boolean'
          && (!isset($pattern['except_type']) || $pattern['except_type'] != 'boolean')
          && (!isset($pattern['except_value']) || $pattern['except_value'] !== $value)
        ) {
          self::$validationFailures[$key] = $key . ' is boolean, should be ' . $expectedType . '.';
          return FALSE;
        }
        return TRUE;

      case 'NULL':
        if ($expectedType != 'NULL'
          && (!isset($pattern['except_type']) || $pattern['except_type'] != 'NULL')
          && (!array_key_exists('except_value', $pattern) || $pattern['except_value'] !== $value)
        ) {
          if (empty($pattern['allow_null'])) {
            self::$validationFailures[$key] = $key . ' is NULL, should be ' . $expectedType . '.';
            return FALSE;
          }
        }
        return TRUE;

      default:
        // Unsupported actual type; cannot match expected type.
        self::$validationFailures[$key] = $key . ' is ' . $actualType . ', should be ' . $expectedType . '.';
        return FALSE;
    }

    // From here on, the expected and actual data types are string, integer or float.
    // And we do not consider it a failure, if actual type - among these three types - differs from expected type.
    if (!$actualString && !$actualNumber) {
      self::$validationFailures[$key] = $key . ' is ' . $actualType . ', should be ' . $expectedType . '.';
      return FALSE;
    }

    // 'except_value' obsoletes all other rules (but 'optional') if the rule value and the actual value are the same.
    if (isset($pattern['except_value']) && $value === $pattern['except_value']) {
      return TRUE;
    }

    // All types may have length requirements.
    // Algo: exact_length/max_length/min_length MUST be applied before truncate.
    if (!empty($pattern['exact_length'])) {
      if ($length > $pattern['exact_length']) {
        self::$validationFailures[$key] = $key . ' exact length should be ' . $pattern['exact_length'] . ', actual length ' . $length . '.';
        return FALSE;
      }
    }
    else {
      if (!empty($pattern['max_length'])) {
        if ($length > $pattern['max_length']) {
          self::$validationFailures[$key] = $key . ' max length is ' . $pattern['max_length'] . ', actual length ' . $length . '.';
          return FALSE;
        }
      }
      if (!empty($pattern['min_length'])) {
        if ($length < $pattern['min_length']) {
          self::$validationFailures[$key] = $key . ' min length is ' . $pattern['min_length'] . ', actual length ' . $length . '.';
          return FALSE;
        }
      }
    }

    // Numbers.
    // Will become true if number expected AND $value is confirmed as - or converted successfully to - a number.
    $isNumber = FALSE;

    if ($expectedType == 'integer') {
      // Actually a string.
      if (!$actualNumber) {
        if (!$value) {
          $value = 0;
        }
        // Only digits?
        elseif (!ctype_digit($value)) {
          // Not only digits, but it may be a float.
          // filter_var() actually converts, even when given a FILTER_VALIDATE_... flag.
          if (!($value = filter_var($value, FILTER_VALIDATE_FLOAT))
            || $value > 2147483647 || $value < -2147483648
            || !($value = filter_var($value, FILTER_VALIDATE_INT))
          ) {
            self::$validationFailures[$key] = $key . ' is not a 32-bit integer.';
            return FALSE;
          }
          // else... keep $value converted to integer.
        }
        // Only digits, but out of 32-bit range?
        elseif (($value = (float)$value) > 2147483647 || $value < -2147483648) {
          self::$validationFailures[$key] = $key . ' is not a 32-bit integer.';
          return FALSE;
        }
        else {
          $value = (int)$value;
        }
        // No matter which, it got altered.
        $altered = TRUE;
      }
      // Actually a float.
      elseif (!$actualInteger) {
        if ($value > 2147483647 || $value < -2147483648) {
          self::$validationFailures[$key] = $key . ' is not a 32-bit integer.';
          return FALSE;
        }
        else {
          $value = (int)$value;
          $altered = TRUE;
        }
      }
      $isNumber = TRUE;
    }
    elseif ($expectedType == 'float') {
      if (!$actualNumber) {
        if (!$value) {
          $value = 0;
        }
        // filter_var() actually converts, even when given a FILTER_VALIDATE_... flag.
        elseif (!($value = filter_var($value, FILTER_VALIDATE_FLOAT))) {
          self::$validationFailures[$key] = $key . ' is not a float.';
          return FALSE;
        }
        // else... keep $value converted to float.
        $altered = TRUE;
      }
      elseif ($actualInteger) {
        $value = (float)$value;
        $altered = TRUE;
      }
      $isNumber = TRUE;
    }
    // Stringy types.
    else {
      // Cast actual number to string.
      if ($actualNumber) {
        $value = '' . $value;
        $altered = TRUE;
      }

      // Truncate?
      // Algo: truncate MUST be applied before enum/regex.
      if (!empty($pattern['truncate'])) {
        $value = drupal_substr($value, 0, $pattern['truncate']);
        $altered = TRUE;
      }
    }

    // Expected AND confirmed (or converted to) number.
    if ($isNumber) {
      // Min/max.
      if (!empty($pattern['min']) && $value < $pattern['min']) {
        self::$validationFailures[$key] = $key . ' minimum is ' . $pattern['min'] . ', saw ' . $value . '.';
        return FALSE;
      }
      if (!empty($pattern['max']) && $value > $pattern['max']) {
        self::$validationFailures[$key] = $key . ' maximum is ' . $pattern['max'] . ', saw ' . $value . '.';
        return FALSE;
      }
    }

    // enum.
    if (!empty($pattern['enum'])) {
      // Compare type-safe if rule 'type' is integer|float; that will confuse some, but there's no way other way around it.
      if (!in_array($value, $pattern['enum'], $isNumber)) {
        self::$validationFailures[$key] = $key . ' is not '
          . (!empty($pattern['title']) ? $pattern['title'] : 'within range of allowed values') . '.';
        return FALSE;
      }
    }
    // 'enum' cannot be combined with 'check' nor 'regex'.
    else {
      // A predefined 'check'.
      if (!empty($pattern['check'])) {
        if (!$included_check) {
          $included_check = TRUE;
          module_load_include('inc', 'restmini', 'restmini_validation_check');
        }

        if (!restmini_validation_check($pattern['check'], $value)) {
          self::$validationFailures[$key] = $key . ' '
            . (!empty($pattern['title']) ? ('is not ' . $pattern['title']) : 'does not match valid pattern') . '.';
          return FALSE;
        }
      }

      if (!empty($pattern['regex'])) {
        if (!($match = preg_match($pattern['regex'], '' . $value))) {

          // If preg_match() returns FALSE (or NULL?) it's an error, not a validation failure.
          if ($match !== 0) {
            // Don't throw exception - we only want to throw exceptions for obvious mistakes like bad method arguments.
            self::$validationFailures[$key] = $key . ' failed \'regex\' validation, presumably the pattern'
              . (!empty($pattern['title']) ? '\'s' : (' title\'d \'' . $pattern['title'] . '\'s'))
              . ' regex rule is an invalid regular expression, regex ' . $pattern['regex'];

            // But do log the incident.
            static::log(
              'restmini',
              'Validation pattern' . (!empty($pattern['title']) ? '' : (' title\'d \'' . $pattern['title'] . '\'')) . ' contains \'regex\''
                . ' which seems to be invalid regular expression',
              NULL,
              array(
                'pattern' => $pattern,
                'key' => $key,
                'value' => $value
              ),
              WATCHDOG_WARNING
            );
          }

          // Value failed regex validation.
          self::$validationFailures[$key] = $key . ' '
            . (!empty($pattern['title']) ? ('is not ' . $pattern['title']) : 'does not match valid pattern') . '.';
          return FALSE;
        }
      }
    }

    // Passed validation.
    if ($altered) {
      if (!$containerIsObject) {
        $container[$key] = $value;
      }
      else {
        $container->{$key} = $value;
      }
    }
    return TRUE;
  }

  /**
   * Logs to watchdog.
   *
   * Traces Exception, if $exception and the Inspect module exists.
   *
   * @param string $type
   * @param string $message
   * @param Exception|NULL $exception
   * @param mixed $variable
   * @param integer $severity
   */
  protected static function log($type, $message, $exception = NULL, $variable = NULL, $severity = WATCHDOG_ERROR) {
    if (module_exists('inspect')) {
      if ($exception) {
        inspect_trace($exception, array(
          'category' => $type,
          'message' => $message,
          'severity' => $severity,
          'wrappers' => 1,
        ));
      }
      else {
        inspect($variable, array(
          'category' => $type,
          'message' => $message,
          'severity' => $severity,
          'wrappers' => 1,
        ));
      }
    }
    else {
      watchdog(
        $type,
        (!$message && $exception) ? $exception->getMessage() : $message,
        array(),
        $severity
      );
    }
  }

}
