<?php

namespace Drupal\quizz_matching;

use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_question\QuestionHandler;

/**
 * Extension of QuizQuestion.
 */
class MatchingQuestion extends QuestionHandler {

  protected $body_field_title = 'Instruction';
  protected $base_table = 'quiz_matching_question';

  public function __construct(Question $question) {
    parent::__construct($question);
    if (empty($this->question->match)) {
      $this->question->match = array();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function onSave($is_new = FALSE) {
    // Update or insert the question properties
    db_merge('quiz_matching_question_settings')
      ->key(array('qid' => $this->question->qid, 'vid' => $this->question->vid))
      ->fields(array('choice_penalty' => $this->question->choice_penalty))
      ->execute();

    // Loop through each question-answer combination
    foreach ($this->question->match as $match) {
      $match['feedback'] = (isset($match['feedback'])) ? $match['feedback'] : '';
      // match_id is not so it is a new question.
      if ($is_new || empty($match['match_id']) || $this->question->revision || isset($this->question->node_export_drupal_version)) {
        if (!empty($match['question']) && !empty($match['answer'])) {
          db_insert('quiz_matching_question')
            ->fields(array(
                'qid'      => $this->question->qid,
                'vid'      => $this->question->vid,
                'question' => $match['question'],
                'answer'   => $match['answer'],
                'feedback' => $match['feedback'],
            ))
            ->execute();
        }
      }
      // match_id is set, user may remove or update existing question.
      else {
        if (empty($match['question']) && empty($match['answer'])) {
          // remove sub question.
          db_delete('quiz_matching_question')
            ->condition('match_id', $match['match_id'])
            ->execute();
        }
        else {
          // update sub question.
          db_update('quiz_matching_question')
            ->fields(array(
                'question' => $match['question'],
                'answer'   => $match['answer'],
                'feedback' => $match['feedback'],
            ))
            ->condition('match_id', $match['match_id'])
            ->execute();
        }
      }
    }
  }

  /**
   * Implementation of validateNode
   *
   * @see QuizQuestion#validateNode($form)
   */
  public function validate(array &$form) {
    // No validation is required
    // The first two pairs are required in the form, if there are other errors we forgive them
  }

  /**
   * {@inheritdoc}
   */
  public function delete($single_revision = FALSE) {
    $key = $single_revision ? 'vid' : 'qid';
    $id = $this->question->{$key};

    db_delete('quiz_matching_question_settings')
      ->condition($key, $id)
      ->execute();

    $sql = "DELETE ap";
    $sql .= " FROM {quizz_matching_answer} ap";
    $sql .= " INNER JOIN {quiz_matching_question} qp ON ap.match_id = qp.match_id";
    $sql .= " WHERE qp.{$key} = :id";
    db_query($sql, array(':id' => $id));

    parent::delete($single_revision);
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if (isset($this->properties)) {
      return $this->properties;
    }
    $props = parent::load();

    $res_a = db_query(
      'SELECT choice_penalty FROM {quiz_matching_question_settings} WHERE qid = :qid AND vid = :vid', array(
        ':qid' => $this->question->qid,
        ':vid' => $this->question->vid
      ))->fetchAssoc();

    if (is_array($res_a)) {
      $props = array_merge($props, $res_a);
    }

    $query = db_query('SELECT match_id, question, answer, feedback FROM {quiz_matching_question} WHERE qid = :qid AND vid = :vid', array(
        ':qid' => $this->question->qid,
        ':vid' => $this->question->vid
    ));
    while ($result = $query->fetch()) {
      $props['match'][] = array(
          'match_id' => $result->match_id,
          'question' => $result->question,
          'answer'   => $result->answer,
          'feedback' => $result->feedback,
      );
    }
    $this->properties = $props;
    return $props;
  }

  public function view() {
    $content = parent::view();

    list($matches, $select_option) = $this->getSubquestions();
    $subquestions = array();
    if ($this->viewCanRevealCorrect()) {
      foreach ($matches as $match) {
        $subquestions[] = array(
            'question' => $match['question'],
            'correct'  => $match['answer'],
            'feedback' => $match['feedback']
        );
      }
    }
    else {
      // shuffle the answer column
      foreach ($matches as $match) {
        $sub_qs[] = $match['question'];
        $sub_as[] = $match['answer'];
      }
      shuffle($sub_as);
      foreach ($sub_qs as $i => $sub_q) {
        $subquestions[] = array(
            'question' => $sub_q,
            'random'   => $sub_as[$i],
        );
      }
    }
    $content['answers'] = array(
        '#markup' => theme('quizz_matching_match_question_view', array('subquestions' => $subquestions)),
        '#weight' => 2,
    );
    return $content;
  }

  /**
   * Implementation of getAnsweringForm
   *
   * @see QuizQuestion#getAnsweringForm($form_state, $result_id)
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $form = parent::getAnsweringForm($form_state, $result_id);

    if (isset($result_id)) {
      // The question has already been answered. We load the answers
      $response = new MatchingResponse($result_id, $this->question);
      $responses = $response->getResponse();
    }

    list($matches, $select_option) = $this->getSubquestions();
    //$form['#theme'] = 'quizz_matching_subquestion_form';
    foreach ($matches as $match) {
      $form[$match['match_id']] = array(
          '#title'   => $match['question'],
          '#type'    => 'select',
          '#options' => $this->customShuffle($select_option),
      );

      if ($responses) {
        // If this question already has been answered
        $form[$match['match_id']]['#default_value'] = $responses[$match['match_id']];
      }
    }

    if ($this->question->getQuestionType()->getConfig('quiz_matching_shuffle_options', TRUE)) {
      $form = $this->customShuffle($form);
    }

    $form['scoring_info'] = array(
        '#prefix' => '<p><em>',
        '#markup' => t('You lose points by selecting incorrect options. You may leave an option blank to avoid losing points.'),
        '#suffix' => '</em></p>',
    );
    return $form;
  }

  /**
   * Question response validator.
   */
  public function validateAnsweringForm(array &$form, array &$form_state = NULL) {
    $question_answer = $form_state['values']['question'][$this->question->qid]['answer'];
    foreach ($question_answer as $value) {
      if ($value != 'def') {
        return TRUE;
      }
    }
    form_set_error('', t('You need to match at least one of the items.'));
  }

  /**
   * Shuffles an array, but keeps the keys, and makes sure the first element is the default element
   *
   * @param $array
   *  Array to be shuffled
   * @return
   *  A shuffled version of the array with $array['def'] = '' as the first element
   */
  private function customShuffle(array $array = array()) {
    $new_array = array();
    $new_array['def'] = '';
    while (count($array)) {
      $element = array_rand($array);
      $new_array[$element] = $array[$element];
      unset($array[$element]);
    }
    return $new_array;
  }

  /**
   * Helper function to fetch subquestions
   *
   * @return
   *  Array with two arrays, matches and selected options
   */
  public function getSubquestions() {
    $matches = $select_option = array();
    $query = db_query('SELECT match_id, question, answer, feedback FROM {quiz_matching_question} WHERE qid = :qid AND vid = :vid', array(':qid' => $this->question->qid, ':vid' => $this->question->vid));
    while ($result = $query->fetch()) {
      $matches[] = array(
          'match_id' => $result->match_id,
          'question' => $result->question,
          'answer'   => $result->answer,
          'feedback' => $result->feedback,
      );
      $select_option[$result->match_id] = $result->answer;
    }
    return array($matches, $select_option);
  }

  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm($form_state)
   */
  public function getCreationForm(array &$form_state = NULL) {
    // Get the nodes settings, users settings or default settings
    $default_settings = $this->getDefaultAltSettings();

    $form['settings'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Settings'),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
    );
    $form['settings']['choice_penalty'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Penalty for guessing'),
        '#description'   => t('Subtract 1 point from the users score for each incorrect match. Scores cannot go below 0.'),
        '#default_value' => $default_settings['choice_penalty'],
        '#parents'       => array('choice_penalty'),
    );

    $form['match'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Answer'),
        '#weight'      => -4,
        '#tree'        => TRUE,
        '#theme'       => 'quizz_matching_question_form',
        '#description' => t('Write your pairs in the question and answer columns. For the user the question will be fixed and the answers will be shown as alternatives in a dropdown box.'),
    );

    for ($i = 1; $i <= $this->question->getQuestionType()->getConfig('quiz_matching_form_size', 5); ++$i) {
      $form['match'][$i] = array(
          '#type'  => 'fieldset',
          '#title' => t('Question ' . $i),
      );
      $form['match'][$i]['match_id'] = array(
          '#type'          => 'value',
          '#default_value' => isset($this->question->match[$i - 1]['match_id']) ? $this->question->match[$i - 1]['match_id'] : ''
      );
      $form['match'][$i]['question'] = array(
          '#type'          => 'textarea',
          '#rows'          => 2,
          '#default_value' => isset($this->question->match[$i - 1]['question']) ? $this->question->match[$i - 1]['question'] : '',
          '#required'      => $i < 3,
      );
      $form['match'][$i]['answer'] = array(
          '#type'          => 'textarea',
          '#rows'          => 2,
          '#default_value' => isset($this->question->match[$i - 1]['answer']) ? $this->question->match[$i - 1]['answer'] : '',
          '#required'      => $i < 3,
      );

      $form['match'][$i]['feedback'] = array(
          '#type'          => 'textarea',
          '#rows'          => 2,
          '#default_value' => isset($this->question->match[$i - 1]['feedback']) ? $this->question->match[$i - 1]['feedback'] : ''
      );
    }
    return $form;
  }

  /**
   * Helper function provding the default settings for the creation form.
   *
   * @return
   *  Array with the default settings
   */
  private function getDefaultAltSettings() {
    // If the node is being updated the default settings are those stored in the node
    if (isset($this->question->qid)) {
      $settings['choice_penalty'] = $this->question->choice_penalty;
    }
    // The user is creating his first matching node
    else {
      $settings['choice_penalty'] = 0;
    }
    return $settings;
  }

  /**
   * Implementation of getMaximumScore
   *
   * @see QuizQuestion#getMaximumScore()
   */
  public function getMaximumScore() {
    $to_return = 0;
    foreach ($this->question->match as $match) {
      if (empty($match['question']) || empty($match['answer'])) {
        continue;
      }
      $to_return++;
    }
    // The maximum score = the number of sub-questions
    return $to_return;
  }

  /**
   * Get the correct answers for this question
   *
   * @return
   *  Array of correct answers
   */
  public function getCorrectAnswer() {
    $correct_answers = array();
    $query = db_query('SELECT match_id, question, answer, feedback FROM {quiz_matching_question} WHERE qid = :qid AND vid = :vid', array(':qid' => $this->question->qid, ':vid' => $this->question->vid));
    while ($result = $query->fetch()) {
      $correct_answers[$result->match_id] = array(
          'match_id' => $result->match_id,
          'question' => $result->question,
          'answer'   => $result->answer,
          'feedback' => $result->feedback,
      );
    }
    return $correct_answers;
  }

  /**
   * {@inheritdoc}
   */
  public function questionTypeConfigForm(QuestionType $question_type) {
    $form = array('#validate' => array('quizz_matching_config_validate'));

    $form['quiz_matching_form_size'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Match Question Size'),
        '#description'   => t('Number of questions allowed to wrap under a matching type question.'),
        '#default_value' => $question_type->getConfig('quiz_matching_form_size', 5),
    );

    $form['quiz_matching_shuffle_options'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Shuffle Matching Questions'),
        '#default_value' => $question_type->getConfig('quiz_matching_shuffle_options', TRUE),
        '#description'   => t('If checked matching questions will be shuffled'),
    );

    return $form;
  }

}
