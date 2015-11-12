<?php
/**
 * @file
 * Interview class for consult module.
 */

class ConsultInterview extends Entity {
  /**
   * {@inheritdoc}
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    Entity::__construct($values, CONSULT_INTERVIEW_ENTITY_NAME);
  }

  /**
   * Get all questions attached to this interview.
   *
   * @return array
   *   Questions attached to this interview keyed by name.
   */
  public function getQuestions() {
    $questions = entity_get_controller($this->entityType)->getQuestions($this);
    usort($questions, array($this, 'arraySortEntities'));

    return $questions;
  }

  /**
   * Get all group names of questions.
   *
   * @return array
   *   Group names of all available questions.
   */
  public function getQuestionGroups() {
    $group_names = array();
    $results = $this->getQuestions();

    foreach ($results as $result) {
      $group_names[] = $result->getGroupName();
    }

    $group_names = array_unique($group_names);
    return $group_names;
  }

  /**
   * Get all results attached to this interview.
   *
   * @return array
   *   Results attached to this interview keyed by name.
   */
  public function getResults() {
    $results = entity_get_controller($this->entityType)->getResults($this);
    usort($results, array($this, 'arraySortEntities'));

    return $results;
  }

  /**
   * Get the JavaScript settings object for this interview.
   *
   * @return array
   *   The questions and results in a JavaScript settings array format.
   */
  public function getJsSettings() {
    return entity_get_controller($this->entityType)->getJsSettings($this);
  }

  /**
   * Get all group names of results.
   *
   * @return array
   *   Group names of all available results.
   */
  public function getResultGroups() {
    $group_names = array();
    $results = $this->getResults();

    foreach ($results as $result) {
      $group_names[] = $result->getGroupName();
    }

    $group_names = array_unique($group_names);
    return $group_names;
  }

  /**
   * PHP Array Sort callback for ordering entities by weight.
   */
  public function arraySortEntities($a, $b) {
    $a_weight = $a->getWeight();
    $b_weight = $b->getWeight();
    if ($a_weight == $b_weight) {
      return 0;
    }
    return ($a_weight < $b_weight) ? -1 : 1;
  }

  /**
   * Get the settings for a given result attached to this Interview.
   *
   * @param string $entity_type
   *   The sub entity type.
   * @param string $entity_name
   *   The sub entity name.
   *
   * @return mixed
   *   The settings array.
   */
  public function getSubEntitySetting($entity_type, $entity_name) {
    $setting = NULL;

    if (!empty($this->settings[$entity_type]) && !empty($this->settings[$entity_type][$entity_name])) {
      $setting = $this->settings[$entity_type][$entity_name];
    }

    return $setting;
  }

  /**
   * Specifies the default uri, which is picked up by uri().
   */
  protected function defaultURI() {
    return array(
      'path' => 'consult/' . str_replace('_', '-', $this->identifier()),
    );
  }

  /**
   * Set the sub entity settings tied to this interview.
   *
   * @param ConsultInterviewSubEntity $entity
   *   The sub entity.
   * @param int $weight
   *   The weight to set.
   * @param int $enabled
   *   The enabled state.
   */
  public function setSubEntityValue(ConsultInterviewSubEntity $entity, $weight, $enabled) {
    entity_get_controller($this->entityType)->setSubEntityValue($this, $entity, $weight, $enabled);
  }
}

/**
 * Base class for questions and results attached to interviews.
 */
class ConsultInterviewSubEntity extends Entity {
  /**
   * {@inheritdoc}
   */
  public function __construct(array $values = array(), $entity_type = NULL) {
    if (empty($values['interview_name']) && empty($this->interview_name)) {
      throw new Exception('Cannot create the sub-entity without the consultation name.');
    }

    switch (get_class($this)) {
      case 'ConsultQuestion':
        $this->entityType = CONSULT_QUESTION_ENTITY_NAME;
        break;

      case 'ConsultResult':
        $this->entityType = CONSULT_RESULT_ENTITY_NAME;
        break;
    }

    // Set initial values.
    foreach ($values as $key => $value) {
      $this->$key = $value;
    }

    $this->interview = entity_load_single(CONSULT_INTERVIEW_ENTITY_NAME, $this->interview_name);

    $this->setUp();
  }

  /**
   * Set up the object instance on construction or unserializiation.
   */
  protected function setUp() {
    parent::setUp();

    $extra_settings = $this->interview->getSubEntitySetting($this->entityType(), $this->identifier());

    if ($extra_settings) {
      $this->weight = $extra_settings['weight'];
      $this->enabled = $extra_settings['enabled'];
      $this->shown = $extra_settings['shown'];
    }
  }

  /**
   * Override the subentity save.
   */
  public function save() {
    $is_new = !empty($this->is_new) || empty($this->{$this->idKey});
    parent::save();

    // If the sub entity is new, add values to the main entity.
    $settings = $this->interview->getSubEntitySetting($this->entityType(), $this->identifier());
    if ($is_new && empty($settings)) {
      $this->interview->setSubEntityValue($this, 0, 1);
      $this->interview->save();
    }

    cache_clear_all('js_settings_' . $this->interview->identifier(), 'cache', TRUE);
  }

  /**
   * Get dependencies for this subentity.
   *
   * @param bool $filter
   *   Filter out items with empty values.
   *
   * @return array
   *   The dependencies for this subentity.
   */
  public function getDependencies($filter = FALSE) {
    $output = array();

    if (!empty($this->dependencies)) {
      $output = $this->dependencies;
    }

    if ($filter) {
      foreach ($output as &$dependency) {
        // Remove only empty string values.
        $dependency = array_filter($dependency, 'strlen');
      }
    }

    // Reset the dependency delta keys to ensure output as array in JavaScript.
    $output = array_values($output);

    return $output;
  }

  /**
   * Get the filtered and processed dependency output.
   *
   * @return array
   *   The dependencies.
   */
  public function getDependencyOutput() {
    return $this->getDependencies(TRUE);
  }

  /**
   * Add a new blank dependency to this Entity.
   */
  public function addNewDependency() {
    $current_dependencies = $this->getDependencies();

    $current_dependencies[] = array(
      'name' => '',
      'value' => '',
    );

    $this->dependencies = $current_dependencies;
  }

  /**
   * Get the order weight this sub entity (originally saved to Interview).
   *
   * @return int
   *   The weight as recorded by Drupal.
   */
  public function getWeight() {
    $weight = 0;

    if (!empty($this->weight)) {
      $weight = $this->weight;
    }

    return $weight;
  }

  /**
   * Get the status of this sub entity (originally saved to Interview).
   *
   * @return int
   *   0 for disabled, or 1 for enabled.
   */
  public function isEnabled() {
    $enabled = 0;

    if (!empty($this->enabled)) {
      $enabled = $this->enabled;
    }

    return !empty($enabled);
  }

  /**
   * Get the show type of the sub entity (originally saved to Interview).
   *
   * @return int
   *   0 for hidden by default, or 1 for shown by default.
   */
  public function getShown() {
    $shown = 0;

    if (!empty($this->shown)) {
      $shown = $this->shown;
    }

    return $shown;
  }

  /**
   * Get group name (for organisation).
   *
   * @return string
   *   The name of the group the interview sub element belongs to.
   */
  public function getGroupName() {
    $group_name = '';

    if (!empty($this->group_name)) {
      $group_name = $this->group_name;
    }

    return $group_name;
  }

  /**
   * Get the dependency list of a given dependency delta.
   *
   * @param int $delta
   *   The dependency delta
   *
   * @return mixed
   *   Answers keyed by question name.
   */
  public function getDependency($delta) {
    $dependencies = $this->getDependencies();
    return $dependencies[$delta];
  }

  /**
   * Get the value for a question within a dependency.
   *
   * @param string $question_name
   *   The entity name of the question.
   * @param int $dependency_delta
   *   The delta of the dependency.
   *
   * @return string
   *   The value of the answer for the given question.
   */
  public function getValueForQuestionInDependency($question_name, $dependency_delta) {
    $value = '';

    $dependency = $this->getDependency($dependency_delta);

    if (isset($dependency[$question_name])) {
      $value = $dependency[$question_name];
    }

    return $value;
  }
}
