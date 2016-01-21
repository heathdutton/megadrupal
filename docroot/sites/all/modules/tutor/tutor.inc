<?php

/**
 * @file
 * Classes used by the Mechanical Tutor module.
 */

/**
 * The default class for Tutor questions. Extended to create new questions.
 *
 * The methods you want to override are probably these:
 *   - labelGet()
 *   - parametersGenerate()
 *   - buildQuestion()
 *   - extractAnswer()
 *   - evaluateAnswer()
 *
 * @see TutorQuestion::labelGet()
 * @see TutorQuestion::parametersGenerate()
 * @see TutorQuestion::buildQuestion()
 * @see TutorQuestion::extractAnswer()
 * @see TutorQuestion::evaluateAnswer()
 */
abstract class TutorQuestion {
  /**
   * An array with settings for the question type, if any.
   *
   * May be used to differentiate between different types of questions managed
   * by the same class. Populated on construct.
   * @var array
   */
  protected $settings;

  /**
   * A string identifying the type of question. Set on construct.
   *
   * @var string
   * @see TutorQuestion::questionIdGet()
   */
  protected $questionId;

  /**
   * A string identifying the storage key for question data.
   *
   * @var string
   * @see tutor_data_store()
   * @see tutor_data_load()
   * @see TutorQuestion::storageIdSet()
   * @see TutorQuestion::storageIdGet()
   */
  protected $storageId;

  /**
   * An array with parameters used for this instance of the question.
   *
   * May be changed using parametersSet() and parametersAdd(). Parameters are
   * usually stored by tutor_data_store() for persitance between page loads.
   *
   * @var array
   * @see TutorQuestion::parametersSet()
   * @see TutorQuestion::parametersGet()
   */
  protected $parameters;

  /**
   * The submitted answer to the question. Set by extractAnswer().
   *
   * @var array
   * @see TutorQuestion::extractAnswer()
   */
  public $answer;

  /**
   * The response to the answer submitted to the question.
   *
   * Set on construct, modified by evaluateAnswer().
   * @var TutorQuestionResponse
   * @see TutorQuestion::extractAnswer()
   */
  public $response;

  /**
   * Stores some essential properties for the question.
   *
   * @param $parameters
   *   (optional) If specified (and not FALSE), the question will be built using
   *   the given parameters.
   * @param $settings
   *   (optional) An array with any settings, which may be used by other methods
   *   in the question class.
   */
  function __construct($question_id = array(), $settings = array()) {
    $this->questionId = $question_id;
    $this->settings = $settings;
    $this->response = new TutorQuestionResponse(TUTOR_ANSWER_INCOMPLETE);
  }

  /**
   * Returns the ID for this type of question.
   *
   * @return
   *   An string with the question ID.
   */
  public function questionIdGet() {
    return $this->questionId;
  }

  /**
   * Returns the storage ID for this question instance.
   *
   * @return
   *   An string with the question storage ID.
   */
  public function storageIdGet() {
    return $this->storageId;
  }

  /**
   * Sets the storage ID for this question instance.
   *
   * @param $storage_id
   *   A string with the storage ID to use.
   * @return \TutorQuestion
   *   Returns the question object, to allow chaining.
   */
  public function storageIdSet($storage_id) {
    $this->storageId = $storage_id;
    return $this;
  }

  /**
   * Produces the label of the question.
   *
   * Try to keep question labels short, consise and unique.
   *
   * @return
   *   A string that has been passed through the t() function.
   */
  abstract function labelGet();

  /**
   * Generates a new set of parameters to use for this question.
   *
   * If the question uses parameters to vary its content, this is where to
   * create them. Parameters should be stored in the parameters property, and
   * the actual question object should be returns (to allow chaining).
   *
   * @see TutorQuestion::parameters
   * @see TutorQuestion::extractAnswer()
   * @see TutorQuestion::buildQuestion()
   * @return \TutorQuestion
   */
  abstract function parametersGenerate();

  /**
   * Returns the parameters for this question instance.
   *
   * @return
   *   An array with the question parameters.
   */
  public function parametersGet() {
    return $this->parameters;
  }

  /**
   * Replaces the question parameters.
   *
   * @param $parameters
   *   An array with the new parameters for the question.
   * @return \TutorQuestion
   *   Returns the question object, to allow chaining.
   */
  public function parametersSet($parameters) {
    $this->parameters = $parameters;
    return $this;
  }

  /**
   * Adds new parts to the question parameters.
   *
   * @param $parameters
   *   An array with the parameters to add.
   * @return \TutorQuestion
   *   Returns the question object, to allow chaining.
   */
  public function parametersAdd($parameters) {
    $parameters += $this->parameters;
    $this->parameters = $parameters;
    return $this;
  }

  /**
   * Adds the question-and-answer parts to the question form.
   *
   * Any new form elements used when evaluating the answer must be collected by
   * extractAnswer().
   *
   * @param $form
   *   The form used to present the scripted question. Passed by reference – the
   *   return should be the question object, not the form array.
   * @see TutorQuestion::extractAnswer()
   * @see TutorQuestion::parametersGenerate()
   */
  function buildQuestion(&$form) {
    $form['answer'] = array(
      '#weight' => 80,
      '#tree' => TRUE,
      '#type' => 'container',
      '#attached' => array(
        'css' => array(drupal_get_path('module', 'tutor') . '/tutor.css')
      ),
      '#attributes' => array(
        'class' => array('tutor-inline'),
      ),
      'answer' => array(
        '#weight' => 80,
        '#type' => 'textfield',
        '#required' => TRUE,
        '#title' => t('Answer'),
        '#title_display' => 'invisible',
        // The autocomplete attribute breaks some validation, but should be ok in
        // HTML5. It is useful to have here, since autocomplete dropdowns provided
        // by the browsers can make questions really difficult to read. Also, it
        // might reduce the learning effect if previous answers are displayed.
        '#attributes' => array('autocomplete' => array('off')),
      ),
    );
    return $this;
  }

  /**
   * Extracts the answer from a submitted form, and stores in $this->answer.
   *
   * This default function extracts an answer from the ['answer']['answer'] part
   * of the form array. Override the method to extract more complex answers.
   *
   * @param $form
   *   The scripted question form, as submitted by the user.
   * @return
   *   Returns the question object, to allow chaining.
   * @see TutorQuestion::evaluateAnswer()
   */
  function extractAnswer($form) {
    $this->answer = $form['answer']['answer']['#value'];
    return $this;
  }

  /**
   * Evaluates the answer to the scripted question.
   *
   * This method should evaluate $this->answer against $this->parameters and
   * determine if the submitted answer is correct or not. The method should
   * update $this->response to reflect the evaluation result.
   *
   * @return
   *   Returns the question object, to allow chaining.
   * @see TutorQuestion::extractAnswer()
   */
  abstract function evaluateAnswer();
}

/**
 * A class used for declaring responses to answers.
 *
 * There should be no reason to extend this class. It is only here to make sure
 * that responses are handled correctly – not to increase flexibility.
 */
class TutorQuestionResponse {
  // One of the respnse types declared by the Tutor module.
  var $response_type;
  // A message to display to the user. Translated.
  var $message;

  public function __construct($response_type, $message = FALSE) {
    $default_messages = array(
      TUTOR_ANSWER_CORRECT => t('Correct!'),
      TUTOR_ANSWER_CLOSE => t('Close! Give it another try.'),
      TUTOR_ANSWER_INCOMPLETE => t('Your answer is incomplete. Please check and try again.'),
      TUTOR_ANSWER_WRONG => t('Sorry, wrong answer.'),
      TUTOR_ANSWER_INVALID => t('Your answer seems not ony wrong, but is missing something important or includes something weird. Sorry.'),
      TUTOR_ANSWER_QUIT => t('Ok, new question is generated. (Quitting a question counts as a wrong answer.)'),
    );
    if (!isset($default_messages[$response_type])) {
      watchdog('tutor', 'The response type %response is not valid. Defaulting to TUTOR_ANSWER_WRONG.', array('%response' => $response_type));
      $response_type = TUTOR_ANSWER_WRONG;
    }
    if (!$message) {
      $message = $default_messages[$response_type];
    }

    $this->response_type = $response_type;
    $this->message = $message;
  }
}
