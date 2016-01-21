<?php

namespace Drupal\quizz_multichoice;

use Drupal\quizz_multichoice\FormDefinition;
use Drupal\quizz_question\Entity\QuestionType;
use Drupal\quizz_question\QuestionHandler;

class MultichoiceQuestion extends QuestionHandler {

  protected $base_table = 'quizz_multichoice_question';
  protected $base_answer_table = 'quizz_multichoice_answer';

  /**
   * Forgive some possible logical flaws in the user input.
   */
  private function forgive() {
    if ($this->question->choice_multi == 1) {
      for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
        $short = &$this->question->alternatives[$i];

        // If the scoring data doesn't make sense, use the data from the
        // "correct" checkbox to set the score data
        if ($short['score_if_chosen'] == $short['score_if_not_chosen'] || !is_numeric($short['score_if_chosen']) || !is_numeric($short['score_if_not_chosen'])) {
          if (!empty($short['correct'])) {
            $short['score_if_chosen'] = 1;
            $short['score_if_not_chosen'] = 0;
          }
          else {
            $short['score_if_chosen'] = -1;
            $short['score_if_not_chosen'] = 0;
            if ($this->question->getQuestionType()->getConfig('multichoice_def_scoring', 0)) {
              $short['score_if_chosen'] = 0;
              $short['score_if_not_chosen'] = 1;
            }
          }
        }
      }
    }
    else {
      // For questions with one, and only one, correct answer, there will be no
      // points awarded for alternatives not chosen.
      for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
        $short = &$this->question->alternatives[$i];
        $short['score_if_not_chosen'] = 0;
        if (isset($short['correct']) && $short['correct'] == 1 && !quizz_valid_integer($short['score_if_chosen'], 1)) {
          $short['score_if_chosen'] = 1;
        }
      }
    }
  }

  /**
   * Warn the user about possible user errors
   */
  private function warn() {
    // Count the number of correct answers
    $num_corrects = 0;
    for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
      $alt = &$this->question->alternatives[$i];
      if ($alt['score_if_chosen'] > $alt['score_if_not_chosen']) {
        $num_corrects++;
      }
    }

    if ($num_corrects == 1 && $this->question->choice_multi == 1 || $num_corrects > 1 && $this->question->choice_multi == 0) {
      $link_options = array();
      if (isset($_GET['destination'])) {
        $link_options['query'] = array('destination' => $_GET['destination']);
      }
      $go_back = l(t('go back'), 'quiz-question/' . $this->question->qid . '/edit', $link_options);
      if ($num_corrects == 1) {
        $msg = t('Your question allows multiple answers. Only one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.', array('!go_back' => $go_back));
        drupal_set_message($msg, 'warning');
      }
      else {
        $msg = t('Your question doesn\'t allow multiple answers. More than one of the alternatives have been marked as correct. If this wasn\'t intended please !go_back and correct it.', array('!go_back' => $go_back));
        drupal_set_message($msg, 'warning');
      }
    }
  }

  /**
   * Run check_markup() on the field of the specified choice alternative
   * @param $alternativeIndex
   *  The index of the alternative in the alternatives array.
   * @param $field
   *  The name of the field we want to check markup on
   * @param $check_user_access
   *  Whether or not to check for user access to the filter we're trying to apply
   * @return HTML markup
   */
  private function checkMarkup($alternativeIndex, $field, $check_user_access = FALSE) {
    $alternative = $this->question->alternatives[$alternativeIndex];
    return check_markup($alternative[$field]['value'], $alternative[$field]['format']);
  }

  /**
   * Implementation of save
   *
   * Stores the question in the database.
   *
   * @param is_new if - if the node is a new node…
   * (non-PHPdoc)
   * @see sites/all/modules/quiz-HEAD/question_types/quiz_question/QuizQuestion#save()
   */
  public function onSave($is_new = FALSE) {
    $is_new = $is_new || $this->question->revision == 1;

    // Before we save we forgive some possible user errors
    $this->forgive();

    // We also add warnings on other possible user errors
    $this->warn();

    if ($is_new) {
      $id = db_insert('quizz_multichoice_question')
        ->fields(array(
            'qid'            => $this->question->qid,
            'vid'            => $this->question->vid,
            'choice_multi'   => $this->question->choice_multi,
            'choice_random'  => $this->question->choice_random,
            'choice_boolean' => $this->question->choice_boolean,
        ))
        ->execute();

      // TODO: utilize the benefit of multiple insert of DBTNG
      for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
        if (drupal_strlen($this->question->alternatives[$i]['answer']['value']) > 0) {
          $this->insertAlternative($i);
        }
      }
    }
    else {
      db_update('quizz_multichoice_question')
        ->fields(array(
            'choice_multi'   => $this->question->choice_multi,
            'choice_random'  => $this->question->choice_random,
            'choice_boolean' => $this->question->choice_boolean,
        ))
        ->condition('qid', $this->question->qid)
        ->condition('vid', $this->question->vid)
        ->execute();

      // We fetch ids for the existing answers belonging to this question
      // We need to figure out if an existing alternative has been changed or deleted.
      $res = db_query(
        'SELECT id
         FROM {quizz_multichoice_alternative}
         WHERE question_qid = :qid AND question_vid = :vid', array(
          ':qid' => $this->question->qid,
          ':vid' => $this->question->vid
      ));

      // We start by assuming that all existing alternatives needs to be deleted
      $ids_to_delete = array();
      while ($res_o = $res->fetch()) {
        $ids_to_delete[] = $res_o->id;
      }

      for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
        $short = $this->question->alternatives[$i];
        if (drupal_strlen($this->question->alternatives[$i]['answer']['value']) > 0) {
          // If new alternative
          if (!is_numeric($short['id'])) {
            $this->insertAlternative($i);
          }
          // If existing alternative
          else {
            $this->updateAlternative($i);
            // Make sure this alternative isn't deleted
            $key = array_search($short['id'], $ids_to_delete);
            $ids_to_delete[$key] = FALSE;
          }
        }
      }

      foreach ($ids_to_delete as $_id) {
        if ($_id) {
          db_delete('quizz_multichoice_alternative')
            ->condition('id', $_id)
            ->execute();
        }
      }
    }
    $this->saveUserSettings();
  }

  function _normalizeAlternative($alternatives) {
    $copy = $alternatives;
    // answer
    if (is_array($alternatives['answer']) && isset($alternatives['answer']['value'])) {
      $copy['answer'] = $alternatives['answer']['value'];
    }
    // answer format
    if (is_array($alternatives['answer']) && isset($alternatives['answer']['format'])) {
      $copy['answer_format'] = $alternatives['answer']['format'];
    }
    // feedback if choosen
    if (is_array($alternatives['feedback_if_chosen']) && isset($alternatives['feedback_if_chosen']['value'])) {
      $copy['feedback_if_chosen'] = $alternatives['feedback_if_chosen']['value'];
    }
    // feedback if choosen foramt
    if (is_array($alternatives['feedback_if_chosen']) && isset($alternatives['feedback_if_chosen']['format'])) {
      $copy['feedback_if_chosen_format'] = $alternatives['feedback_if_chosen']['format'];
    }
    // feedback if not chosen
    if (is_array($alternatives['feedback_if_not_chosen']) && isset($alternatives['feedback_if_not_chosen']['value'])) {
      $copy['feedback_if_not_chosen'] = $alternatives['feedback_if_not_chosen']['value'];
    }
    // feedback if not chosen foramt
    if (is_array($alternatives['feedback_if_not_chosen']) && isset($alternatives['feedback_if_not_chosen']['format'])) {
      $copy['feedback_if_not_chosen_format'] = $alternatives['feedback_if_not_chosen']['format'];
    }
    return $copy;
  }

  /**
   * Helper function. Saves new alternatives
   *
   * @param $i
   *  The alternative index
   */
  private function insertAlternative($i) {
    $alternatives = $this->_normalizeAlternative($this->question->alternatives[$i]);
    db_insert('quizz_multichoice_alternative')
      ->fields(array(
          'answer'                        => $alternatives['answer'],
          'answer_format'                 => $alternatives['answer_format'],
          'feedback_if_chosen'            => $alternatives['feedback_if_chosen'],
          'feedback_if_chosen_format'     => $alternatives['feedback_if_chosen_format'],
          'feedback_if_not_chosen'        => $alternatives['feedback_if_not_chosen'],
          'feedback_if_not_chosen_format' => $alternatives['feedback_if_not_chosen_format'],
          'score_if_chosen'               => $alternatives['score_if_chosen'],
          'score_if_not_chosen'           => $alternatives['score_if_not_chosen'],
          'question_qid'                  => $this->question->qid,
          'question_vid'                  => $this->question->vid,
          'weight'                        => isset($alternatives['weight']) ? $alternatives['weight'] : $i,
      ))
      ->execute();
  }

  /**
   * Helper function. Updates existing alternatives
   *
   * @param int $i The alternative index.
   */
  private function updateAlternative($i) {
    $alternatives = $this->_normalizeAlternative($this->question->alternatives[$i]);
    db_update('quizz_multichoice_alternative')
      ->fields(array(
          'answer'                        => $alternatives['answer'],
          'answer_format'                 => $alternatives['answer_format'],
          'feedback_if_chosen'            => $alternatives['feedback_if_chosen'],
          'feedback_if_chosen_format'     => $alternatives['feedback_if_chosen_format'],
          'feedback_if_not_chosen'        => $alternatives['feedback_if_not_chosen'],
          'feedback_if_not_chosen_format' => $alternatives['feedback_if_not_chosen_format'],
          'score_if_chosen'               => $alternatives['score_if_chosen'],
          'score_if_not_chosen'           => $alternatives['score_if_not_chosen'],
          'weight'                        => isset($alternatives['weight']) ? $alternatives['weight'] : $i,
      ))
      ->condition('id', $alternatives['id'])
      ->condition('question_qid', $this->question->qid)
      ->condition('question_vid', $this->question->vid)
      ->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function validate(array &$form) {
    if (!$this->question->choice_multi) {
      return $this->validateCaseSinglechoice();
    }

    for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
      if (drupal_strlen($this->checkMarkup($i, 'answer'))) {
        return $this->validateCaseMultichoice($i);
      }
    }
  }

  private function validateCaseSinglechoice() {
    for ($i = 0; (isset($this->question->alternatives[$i]) && is_array($this->question->alternatives[$i])); $i++) {
      $length = drupal_strlen($this->checkMarkup($i, 'answer'));
      $correct = $this->question->alternatives[$i]['correct'];
      $length && $correct && ($has_correct = TRUE);
    }

    if (empty($has_correct)) {
      $msg = t('You have not marked any alternatives as correct. If there are no correct alternatives you should allow multiple answers.');
      form_set_error('choice_multi', $msg);
    }
  }

  private function validateCaseMultichoice($i) {
    $short = $this->question->alternatives[$i];
    if ($short['score_if_chosen'] < $short['score_if_not_chosen'] && $short['correct']) {
      $msg = t('The alternative is marked as correct, but gives more points if you don\'t select it.');
      form_set_error("alternatives][$i][score_if_not_chosen", $msg);
    }
    elseif ($short['score_if_chosen'] > $short['score_if_not_chosen'] && !$short['correct']) {
      $msg = t('The alternative is not marked as correct, but gives more points if you select it.');
      form_set_error("alternatives][$i][score_if_chosen", $msg);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function delete($single_revision = FALSE) {
    $key = $single_revision ? 'question_vid' : 'question_qid';
    $id = $single_revision ? $this->question->vid : $this->question->qid;

    db_delete('quizz_multichoice_alternative')
      ->condition($key, $id)
      ->execute();

    parent::delete($single_revision);
  }

  /**
   * {@inheritdoc}
   */
  public function load() {
    if (isset($this->properties) && !empty($this->properties)) {
      return $this->properties;
    }
    $props = parent::load();

    $res_a = db_query(
      'SELECT choice_multi, choice_random, choice_boolean
       FROM {quizz_multichoice_question}
       WHERE qid = :qid AND vid = :vid', array(
        ':qid' => $this->question->qid,
        ':vid' => $this->question->vid
      ))->fetchAssoc();

    if (is_array($res_a)) {
      $props = array_merge($props, $res_a);
    }

    // Load the answers
    $res = db_query(
      'SELECT id, answer, answer_format, feedback_if_chosen, feedback_if_chosen_format,
            feedback_if_not_chosen, feedback_if_not_chosen_format, score_if_chosen, score_if_not_chosen, weight
       FROM {quizz_multichoice_alternative}
       WHERE question_qid = :question_qid AND question_vid = :question_vid
       ORDER BY weight', array(
        ':question_qid' => $this->question->qid,
        ':question_vid' => $this->question->vid));
    $props['alternatives'] = array(); // init array so it can be iterated even if empty
    while ($res_arr = $res->fetchAssoc()) {
      $props['alternatives'][] = array(
          'id'                     => $res_arr['id'],
          'answer'                 => array(
              'value'  => $res_arr['answer'],
              'format' => $res_arr['answer_format'],
          ),
          'feedback_if_chosen'     => array(
              'value'  => $res_arr['feedback_if_chosen'],
              'format' => $res_arr['feedback_if_chosen_format'],
          ),
          'feedback_if_not_chosen' => array(
              'value'  => $res_arr['feedback_if_not_chosen'],
              'format' => $res_arr['feedback_if_not_chosen_format'],
          ),
          'score_if_chosen'        => $res_arr['score_if_chosen'],
          'score_if_not_chosen'    => $res_arr['score_if_not_chosen'],
          'weight'                 => $res_arr['weight'],
      );
    }
    $this->properties = $props;
    return $props;
  }

  public function view() {
    $content = parent::view();
    if ($this->question->choice_random) {
      $this->shuffle($this->question->alternatives);
    }

    $content['answers'] = array(
        '#weight' => 2,
        '#markup' => theme('quizz_multichoice_answer_question_view', array(
            'alternatives' => $this->question->alternatives,
            'show_correct' => $this->viewCanRevealCorrect()
        )),
    );

    return $content;
  }

  /**
   * Generates the question element.
   *
   * This is called whenever a question is rendered, either
   * to an administrator or to a quiz taker.
   */
  public function getAnsweringForm(array $form_state = NULL, $result_id) {
    $element = parent::getAnsweringForm($form_state, $result_id);

    // We use an array looking key to be able to store multiple answers in tries.
    // At the moment all user answers have to be stored in tries. This is
    // something we plan to fix in quiz 5.x.
    $element['#theme'] = 'multichoice_alternative';
    if (isset($result_id)) {
      // This question has already been answered. We load the answer.
      $response = new MultichoiceResponse($result_id, $this->question);
    }

    for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
      $short = $this->question->alternatives[$i];
      $answer_markup = check_markup($short['answer']['value'], $short['answer']['format']);
      if (drupal_strlen($answer_markup) > 0) {
        $element['user_answer']['#options'][$short['id']] = $answer_markup;
      }
    }

    if ($this->question->choice_random) {
      // We save the choice order so that the order will be the same in the answer report
      $element['choice_order'] = array(
          '#type'  => 'hidden',
          '#value' => $this->shuffle($element['user_answer']['#options']),
      );
    }

    if ($this->question->choice_multi) {
      $element['user_answer']['#type'] = 'checkboxes';
      $element['user_answer']['#title'] = t('Choose all that apply');
      if (isset($response)) {
        if (is_array($response->getResponse())) {
          $element['#default_value'] = $response->getResponse();
        }
      }
    }
    else {
      $element['user_answer']['#type'] = 'radios';
      $element['user_answer']['#title'] = t('Choose one');
      if (isset($response)) {
        $selection = $response->getResponse();
        if (is_array($selection)) {
          $element['user_answer']['#default_value'] = array_pop($selection);
        }
      }
    }

    $element['#attached']['js'][] = drupal_get_path('module', 'quizz_multichoice') . '/js/multichoice.js';

    return $element;
  }

  /**
   * Custom shuffle function. It keeps the array key - value relationship intact
   *
   * @param array $array
   * @return unknown_type
   */
  private function shuffle(array &$array) {
    $newArray = array();
    $toReturn = array_keys($array);
    shuffle($toReturn);
    foreach ($toReturn as $key) {
      $newArray[$key] = $array[$key];
    }
    $array = $newArray;
    return $toReturn;
  }

  /**
   * Implementation of getCreationForm
   *
   * @see QuizQuestion#getCreationForm()
   */
  public function getCreationForm(array &$form_state = NULL) {
    $obj = new FormDefinition($this->question);
    return $obj->get($form_state);
  }

  /**
   * Fetches the users default settings from the creation form
   */
  private function saveUserSettings() {
    global $user;

    db_merge('quizz_multichoice_user_settings')
      ->key(array('uid' => $user->uid))
      ->fields(array(
          'choice_random'  => $this->question->choice_random,
          'choice_multi'   => $this->question->choice_multi,
          'choice_boolean' => $this->question->choice_boolean,
      ))
      ->execute();
  }

  /**
   * Implementation of getMaximumScore.
   *
   * @see QuizQuestion#getMaximumScore()
   */
  public function getMaximumScore() {
    if ($this->question->choice_boolean) {
      return 1;
    }

    $max = 0;
    for ($i = 0; isset($this->question->alternatives[$i]); $i++) {
      $short = $this->question->alternatives[$i];
      if ($this->question->choice_multi) {
        $max += max($short['score_if_chosen'], $short['score_if_not_chosen']);
      }
      else {
        $max = max($max, $short['score_if_chosen'], $short['score_if_not_chosen']);
      }
    }
    return max($max, 1);
  }

  /**
   * Question response validator.
   */
  public function validateAnsweringForm(array &$form, array &$form_state = NULL) {
    if (!$this->question->choice_multi && is_null($form_state['values']['question'][$this->question->qid]['answer']['user_answer'])) {
      form_set_error('', t('You must provide an answer.'));
    }
  }

  /**
   * {@inheritdoc}
   */
  function questionTypeConfigForm(QuestionType $question_type) {
    $form = array('#validate' => array('quizz_multichoice_config_validate'));

    $form['multichoice_def_num_of_alts'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Default number of alternatives'),
        '#default_value' => $question_type->getConfig('multichoice_def_num_of_alts', 2),
    );

    $form['multichoice_def_scoring'] = array(
        '#type'          => 'radios',
        '#title'         => t('Default scoring method'),
        '#description'   => t('Choose the default scoring method for questions with multiple correct answers.'),
        '#options'       => array(
            0 => t('Give minus one point for incorrect answers'),
            1 => t("Give one point for each incorrect option that haven't been chosen"),
        ),
        '#default_value' => $question_type->getConfig('multichoice_def_scoring', 0),
    );

    return $form;
  }

}
