<?php

/**
 * @file
 * Contains the DateEndpointGenerator class.
 *
 * This is intended to be a generic PHP class, with no application-specific
 * logic.
 */

/**
 * Generates HTML options based on a repeating relative date interval.
 *
 * Starting from an origin date, work your way backwards/forwards through time;
 * add a new option and corresponding object pair at every specified interval.
 */
class DateEndpointGenerator {
  /**
   * Build the generator, and override defaults if supplied.
   */
  public function __construct(array $properties = NULL) {
    // Set default property values.
    $this->origin = new DateTime();
    $this->direction = -1;
    $this->intervals = array(
      'Y' => 5,
      'M' => 5,
      'W' => 5,
      'D' => 5,
    );

    // Set defaults for option labels.
    $this->optionLabels = array(
      'Y' => function($datetime) {
      return $datetime->format('Y');
      },
      'M' => function($datetime) {
        return $datetime->format('F');
      },
      'W' => function($datetime, DateEndpointGenerator $generator) {
        $datetime_clone = clone $datetime;
        $datestring = '';
        $datestring .= ($generator->direction > 0) ? $datetime_clone->modify('this sunday')->format('M d') : $datetime_clone->modify('this saturday')->format('M d');
        $datestring .= ' - ';
        $datestring .= ($generator->direction > 0) ? $datetime_clone->modify('next saturday')->format('M d') : $datetime_clone->modify('last sunday')->format('M d');

        // Modify to Monday so we get the correct week number.
        return '(week' . ' ' . $datetime->modify('this monday')->format('W') . ') ' . $datestring;
      },
      'D' => function($datetime) {
        return $datetime->format('l, M d');
      },
    );

    // Set values for optgroup labels.
    $this->optgroupLabels = array(
      'Y' => 'Year',
      'M' => 'Month',
      'W' => 'Week',
      'D' => 'Day',
    );

    // By default, option values are just references to endpoint pairs.
    $this->optionValue = function($interval_id, $date_endpoint_pair) {
      return $interval_id;
    };

    // Set supplied properties, if any.
    if (isset($properties) && is_array($properties)) {
      foreach ($properties as $property => $value) {
        $this->$property = $value;
      }
    }
  }

  /**
   * Generates an array of values and labels for <select> options.
   *
   * @var array
   *   Keyed with a string to identify an endpoint pair, and valued with the
   *   label for that endpoint pair.
   */
  protected $options;

  /**
   * Custom endpoints to insert into the component groups.
   *
   * @var array
   *   Keyed with a string to identify the dateTime component group to be
   *   inserted into, and valued by an array of date endpoints.
   */
  protected $endpoints;

  /**
   * Endpoint object pairs.
   *
   * @var array
   *   Keyed by a component string that matches the value for the options;
   *   valued by an array containing two endpoints.  Grouped by date component.
   */
  protected $objects;

  /**
   * An application-supplied closure function to build the array of options.
   *
   * Used for applications that need the option values populated immediately.
   *
   * @var closure
   */
  public $optionValue;

  /**
   * The date to use as a startpoint for generating option/object pairs.
   *
   * @var: DateTime
   */
  public $origin;

  /**
   * Determines which direction, in time, to generate option/object pairs for.
   *
   * A value of 1 generates option/object pairs later than the origin date; -1
   * generates option/object pairs earlier than the origin date.
   *
   * @var boolean
   */
  public $direction;

  /**
   * Determines how many endpoint pairs of each component will be built.
   *
   * @var array
   *   Keyed by a valid PHP DateTime component, such as 'Y', 'M', or 'D',
   *   and valued by the number of intervals desired for each component.
   */
  public $intervals;

  /**
   * Defines functions used to generate option labels for DateTime components.
   *
   * @var array
   *   Keyed by DateTime component, and valued with a closure that will
   *   format that component into a label.
   */
  public $optionLabels;

  /**
   * Determines the labels used for the select list for each component.
   *
   * @var array
   *   Keyed by a valid PHP DateTime component, such as 'Y', 'M', or 'D',
   *   and valued by the desired label for each component.
   */
  public $optgroupLabels;

  /**
   * Gets the options built by generate().
   *
   * The option keys/values can be controlled with $this->optionBuilder().
   */
  public function getOptions() {
    if (empty($this->options) || empty($this->objects)) {
      $this->generate();
    }
    return $this->options;
  }

  /**
   * Gets the objects built by generate().
   */
  public function getObjects() {
    if (empty($this->objects)) {
      $this->generate();
    }
    return $this->objects;
  }

  /**
   * Generates a custom endpoint pair for a given date and interval component.
   *
   * @param string $interval
   *   Date component string to add an endpoint for.
   * @param dateTime $datetime
   *   Date within the desired endpoint pair.
   *
   * @return string
   *   Represents the option value of the added endpoints.
   */
  public function addEndpoint($interval, dateTime $datetime) {
    $custom_endpoints = $this->getInitialEndpoints($interval, $datetime);
    $this->endpoints[$interval][] = $custom_endpoints;
    $index = count($this->endpoints[$interval]) - 1;
    return $this->optionValue->__invoke($interval . '@' . $index, $custom_endpoints);
  }

  /**
   * Gets the initial endpoints for the initial origin interval.
   *
   * For example, if it's October 21, we want the initial 'M' endpoint
   * to be October 1st - 31st, not October 21 - November 21.
   *
   * @param string $interval
   *   A DateTime component string to return endpoints for.
   * @param dateTime $datetime
   *   Date within the desired endpoint pair.
   */
  protected function getInitialEndpoints($interval, dateTime $datetime) {
    $results = array();
    $endpoint_earlier = clone $datetime;

    // Modify endpoint to respect component boundaries; right now these are
    // hard-coded in this switch statement.
    $endpoint_earlier->setTime(00, 00, 00);
    switch ($interval) {
      case 'Y':
        $endpoint_earlier->setDate($endpoint_earlier->format('Y'), 1, 1);
        break;

      case 'M':
        $endpoint_earlier->setDate($endpoint_earlier->format('Y'), $endpoint_earlier->format('m'), 1);
        break;

      case 'W':
        $endpoint_earlier->add(new DateInterval('P1D'));
        $endpoint_earlier->modify('last sunday');
        break;

      case 'D':
        break;

    }

    $results[] = $endpoint_earlier;

    $endpoint_later = clone $endpoint_earlier;
    $endpoint_later->add(new DateInterval('P1' . $interval));
    // Bring a second closer to the origin, so endpoints don't overlap.
    $endpoint_later->sub(new DateInterval('PT1S'));

    $results[] = $endpoint_later;
    return $results;
  }

  /**
   * Builds the option/object pairs.
   *
   * Endpoint 0 is the closest to the origin; endpoint 1 is the farthest.
   */
  protected function generate() {
    $date_endpoint_options = array();
    $date_endpoint_objects = array();

    foreach ($this->intervals as $interval => $periods) {
      // Check for any date component label overrides.
      $group_label = $this->optgroupLabels[$interval] ? $this->optgroupLabels[$interval] : $interval;
      // Add in custom endpoints for this component.
      if (isset($this->endpoints[$interval])) {
        foreach ($this->endpoints[$interval] as $custom_index => $custom_endpoint_pair) {
          $value_string = $this->optionValue->__invoke($interval . '@' . $custom_index, $custom_endpoint_pair);
          $date_endpoint_objects[$interval . '@' . $custom_index] = $custom_endpoint_pair;
          $date_closest = $custom_endpoint_pair[0];
          $date_endpoint_options['By ' . $group_label][$value_string] = 'Custom ' . $group_label . ': ' . $this->optionLabels[$interval]($date_closest, $this);
        }
      }

      $i = 0;
      $initial_endpoints = $this->getInitialEndpoints($interval, $this->origin);

      // Set initial endpoints based on direction.
      // Point 0 will always be the farthest endpoint in the reverse direction.
      if ($this->direction > 0) {
        $endpoint_init0 = reset($initial_endpoints);
        $endpoint_init1 = end($initial_endpoints);
      }
      elseif ($this->direction < 0) {
        $endpoint_init0 = end($initial_endpoints);
        $endpoint_init1 = reset($initial_endpoints);
      }

      // We have our first endpoint pair: generate an option/object pair for it.
      $endpoint_pair = array($endpoint_init0, $endpoint_init1);
      $value_string = $this->optionValue->__invoke($interval . $i, $endpoint_pair);
      $date_endpoint_objects[$interval . $i] = $endpoint_pair;
      $date_endpoint_options['By ' . $group_label][$value_string] = 'Current ' . $group_label;

      $i++;
      $endpoint1 = clone $endpoint_init1;

      // Generate the rest of the option/endpoint pairs.
      while ($i < $periods) {

        // Get a new copy of the previous far endpoint.
        $endpoint1 = clone $endpoint1;

        // Set the new near endpoint to the last far endpoint.
        $endpoint0 = clone $endpoint1;

        if ($this->direction > 0) {
          $advance_method = 'add';
          $label_endpoint = $endpoint0;
        }
        else {
          $advance_method = 'sub';
          $label_endpoint = $endpoint1;
        }

        // Advance by a second so endpoints don't overlap.
        $endpoint0->$advance_method(new DateInterval('PT1S'));

        // Advance by next interval in the specified direction.
        if ($interval == 'W') {
          // Adding a week only advances to the next week; add 7 days instead.
          $endpoint1->$advance_method(new DateInterval('P7D'));
        }
        else {
          $endpoint1->$advance_method(new DateInterval('P1' . $interval));
        }

        // Generate the option label based on the earliest endpoint.
        $endpoint_pair = array($endpoint0, $endpoint1);
        $value_string = $this->optionValue->__invoke($interval . $i, $endpoint_pair);
        $date_endpoint_options['By ' . $group_label][$value_string]
          = $this->optionLabels[$interval]($label_endpoint, $this);
        $date_endpoint_objects[$interval . $i] = $endpoint_pair;

        $i++;
      }
    }

    $this->options = $date_endpoint_options;
    $this->objects = $date_endpoint_objects;
    return $this;
  }
}
