<?php

class EntityListException extends Exception {}

interface EntityListHandler {

  /**
   * Constructor to setup and configure the handler.
   */
  public function __construct(Array $config = array());

  /**
   * Returns what entity type this list lists.
   *
   * @return string
   */
  public function entityType();

  /**
   * Returns the required arguments needed for this list. Required arguments are
   * defined as CTools context keys.
   *
   * It should be in the following form:
   * @code
   *   return array(
   *     'node.nid' => array(
   *       'label' => t('Node NID'),
   *       'required' => TRUE,
   *     ),
   *   );
   * @endcode
   *
   * The key 'node.nid' is a ctools context key. It can be in any of the
   * following formats:
   *   - node
   *   - node.nid
   *   - entity:taxonomy_term
   *   - entity:taxonomy_term.tid
   *
   * @return array
   */
  public function requiredArguments();

  /**
   * Setter for the list's arguments.
   *
   * @return $this
   */
  public function setArguments(Array $arguments = array());

  /**
   * Getter for the list's arguments. Mostly used internally.
   *
   * @return array
   */
  public function getArguments();

  /**
   * Setter for the list's arguments.
   *
   * @return $this
   */
  public function setParameters(Array $parameters = array());

  /**
   * Getter for the list's parameters.
   *
   * @return array
   */
  public function getParameters();

  /**
   * Setter for the list's range.
   *
   * @return $this
   */
  public function setRange($offset = NULL, $limit = NULL);

  /**
   * Getter for the list's range.
   *
   * @return array
   */
  public function getRange();

  /**
   * Build the handler's query.
   */
  public function buildQuery();

  /**
   * Returns the number of entities in this list.
   *
   * @return int
   */
  public function count();

  /**
   * Getter for the cache tags that applies to this list.
   *
   * @return array
   */
  public function cacheTags();

  /**
   * Execute the handler query. This function should return a numeric array of
   * entities, simply keyed by the result index.
   *
   * @see EntityFieldQuery::execute()
   * @return array
   */
  public function execute();

  /**
   * Returns the path to the administrative interface for this list.
   *
   * @return string
   */
  public function adminPath();

  /**
   * Form to configure the handler.
   */
  public function configForm(&$form_state);
}
