<?php
/**
 * @file
 * Base classes for acquisition processes.
 */

/**
 * Interface class for party acquisitions.
 */
interface PartyAcquisitionInterface {

  /**
   * Behavior bit flag to indicate we should do nothing.
   */
  const BEHAVIOR_NOTHING = 0x0;

  /**
   * Behavior bit flag to indicate we should create a new party.
   */
  const BEHAVIOR_CREATE = 0x1;

  /**
   * Behavior bit flag to indicate we should acquire the first.
   */
  const BEHAVIOR_FIRST = 0x2;

  /**
   * Failure code for no values to acquire on.
   */
  const FAIL_NO_VALUES = 0;

  /**
   * Failure code for no matches.
   */
  const FAIL_NO_MATCHES = 1;

  /**
   * Failure code for multiple matches.
   */
  const FAIL_MULTIPLE_MATCHES = 2;

  /**
   * Create or acquire a party based off the given parameters.
   *
   * @param array $values
   *   An array of primary fields to match on. Keys are the primary field and
   *   values are the expected values.
   * @param array $context
   *   An array of contextual information.
   * @param string $method
   *   Optionally pass a variable to be filled with the acquisition method.
   *
   * @return Party|FALSE
   *   The acquired or newly created party or FALSE on a failure.
   */
  public function acquire(array $values, array $context, &$method);

}

/**
 * Base class for acquiring parties.
 */
class PartyAcquisition implements PartyAcquisitionInterface {

  /**
   * Default context for the acquisition process.
   *
   * The following key/value pairs are always available. Calling modules may
   * add additional items specific to their process.
   * - operator: Which operator to use when there are multiple $values. This
   *   can be 'AND' or 'OR'.
   * - name: Optionally provide an identifying name for the process. This
   *   allows hooks to identify specific context.
   * - behavior: Flags indicating what should we do in the case of no or
   *   multiple matches.
   *
   * @var array
   */
  protected $default_context = array(
    'operator' => 'AND',
    'name' => NULL,
    'behavior' => NULL, // Set in PartyAcquisition::setContext().
    'fail' => NULL,
  );

  /**
   * The context of the current operation.
   *
   * @see PartyAcquisition::$default_context.
   *
   * @var array
   */
  protected $context;

  /**
   * Set up the context by merging in defaults.
   *
   * This method may be overridden for classes to add, remove or alter both the
   * default context, but also the processed context.
   *
   * @param array $context
   *   The context we want merged with the defaults. Anything in $context will
   *   take precedence over the defaults.
   */
  protected function setContext($context) {
    $defaults = $this->default_context;
    $defaults['behavior'] = variable_get('party_acquisition_behavior', self::BEHAVIOR_CREATE);
    $this->context = is_array($context) ? $context + $defaults: $defaults;
  }

  /**
   * Invoke hooks. All additional arguments are passed by reference.
   *
   * @param string $hook
   *   The specific hook to fire.
   * @param $args
   *   An array of arguments to be passed to the hook.
   */
  protected function invoke($hook, &$args) {
    // Get the arguments for our invoke.
    $args[] = &$this->context;

    // Fire off our hooks.
    $hook = 'party_acquisition_' . $hook;
    foreach (module_implements($hook) as $module) {
      $function = $module . '_' . $hook;
      call_user_func_array($function, $args);
    }
  }

  /**
   * Create or acquire a party based off the given parameters.
   *
   * @param array $values
   *   An array of party fields to match on. Keys are the field and values are
   *   the expected values. If empty, we will use the given behavior.
   * @param array $context
   *   An array of contextual information. See
   *   PartyAcquisition::$default_context for the standard contexts. Callers
   *   can use this to pass additional information through to hooks.
   * @param string $method
   *   Optionally pass a variable to be filled with the acquisition method.
   *   On a successful acquisition this will contain either 'create' or
   *   'acquire'.
   *
   * @return Party|FALSE
   *   The acquired or newly created party or FALSE on a failure.
   */
  public function acquire(array $values, array $context = NULL, &$method = '') {
    // Check all the keys in $values are valid.
    $schema = drupal_get_schema('party');
    $unknown = array_diff(array_keys($values), array_keys($schema['fields']));
    if (!empty($unknown)) {
      watchdog('party', 'Attempt to acquire on unknown values: %values', array('%values' => implode(', ', $unknown)), WATCHDOG_WARNING);
      return FALSE;
    }

    // Set up our context.
    $this->setContext($context);

    // Set up our values.
    $args = array(&$values, &$this->context);
    $this->invoke('values_alter', $args);

    // Look for a match.
    if (!empty($values)) {
      $party = $this->findMatch($values);
    }
    else {
      $this->context['fail'] = self::FAIL_NO_VALUES;
      $party = FALSE;
    }

    // If we haven't found a match, see if we should create.
    if ($party) {
      $method = 'acquire';
    }
    elseif ($this->context['behavior'] & self::BEHAVIOR_CREATE) {
      $method = 'create';
      $party = party_create();
    }

    // Fire off post acquisition hooks.
    $args = array(&$party, &$method, &$values, &$this->context);
    $this->invoke('post_acquisition', $args);

    // Return our result.
    return $party;
  }

  /**
   * Find a match for the given parameters.
   *
   * @param array $values
   *   An array of party fields to match on. Keys are the field and values are
   *   the expected values.
   *
   * @return Party|FALSE
   *   Return the matched party or FALSE if no valid match could be found.
   */
  protected function findMatch(array $values) {
    $query = $this->buildQuery($values);

    // If we are set to take the first, we don't need to return more than one.
    // Otherwise return 2 matches so we can ignore multiple matches.
    $limit = $this->context['behavior'] & self::BEHAVIOR_FIRST ? 1: 2;
    $query->range(0, $limit);

    // Get hold of our results.
    $results = $query->execute()->fetchCol();

    // If we got a single match we can return a party..
    if (count($results) == 1) {
      return party_load(reset($results));
    }

    // Store something helpful in $context.
    $this->context['fail'] = count($results) ? self::FAIL_MULTIPLE_MATCHES: self::FAIL_NO_MATCHES;

    // Otherwise we have nothing to return.
    return FALSE;
  }

  /**
   * Build the query for finding a party.
   *
   * @param array $values
   *   An array of party fields to match on. Keys are the field and values are
   *   the expected values.
   *
   * @return SelectQuery
   *   A query primed to return the party pids.
   */
  protected function buildQuery(array $values) {
    // Get our base query.
    $query = db_select('party');
    $query->addField('party', 'pid');

    // Add our matching conditions based on the context.
    $match = $this->context['operator'] == 'OR' ? db_or(): db_and();
    foreach ($values as $property => $value) {
      $match->condition("party.{$property}", $value);
    }
    $query->condition($match);

    // We never want to match to hidden parties.
    $query->condition('party.hidden', 0);

    // Allow modules to alter the query.
    $args = array(&$query);
    $this->invoke('query_alter', $args);

    return $query;
  }

}
