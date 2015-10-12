<?php

/**
 * @file
 * Question type, enabling the creation of image drop type of questions.
 * 
 * The main classes for the quiz_sentence_drop question type.
 *
 * These inherit or implement code found in quiz_question.classes.inc.
 *
 * Based on:
 * Other question types in the quiz framework.
 */

/**
 * Extension of QuizQuestion.
 */
class SentenceDropQuestion extends QuizQuestion {

  /**
   * Run check_markup() on the field of the specified choice alternative.
   * 
   * @param int $alternativeIndex
   *   The index of the alternative in the alternatives array.
   * @param varchar $field
   *   The name of the field we want to check markup on.
   * @param Boolean $check_user_access
   *   Whether or not to check for user access to the filter we're trying to 
   * apply.
   * 
   * @return HTML
   *   HTML markup.
   */
  private function checkMarkup($alternativeIndex, $field, $check_user_access = FALSE) {
    $alternative = $this->node->alternatives[$alternativeIndex];
    return check_markup($alternative[$field]['value'], $alternative[$field]['format']);
  }

  /**
   * Implementation of save.
   *
   * Stores the question in the database.
   *
   * @param Boolean $is_new 
   *   if - if the node is a new node...
   * (non-PHPdoc)
   * 
   * @see sites/all/modules/quiz-HEAD/question_types/quiz_question/QuizQuestion#save()
   */
  public function saveNodeProperties($is_new = FALSE) {
    // TODO.
  }

  /**
   * Implementation of validate.
   *
   * QuizQuestion#validate()
   */
  public function validateNode(array &$form) {
    $title = $form['body'][LANGUAGE_NONE][0]['value']['#value'];
    $field_upload_delta = $form['field_sentence_dragdrop_file'][LANGUAGE_NONE]['#file_upload_delta'];
    $uploaded_files = $form['field_sentence_dragdrop_file'][LANGUAGE_NONE];

    $title_array = explode(' ', $title);
    if (isset($title_array) && !empty($title_array)) {
      $title_wildcard = array();
      foreach ($title_array as $key => $val) {
        $val = strtolower($val);
        $pos = strrpos($val, "@");

        $val = trim(strip_tags($val));
//        $val = str_replace('.', '', $val);
        if ($pos === false) {
          // not found...
        } else {
          $val_array = explode('@', $val);
          if (count($val_array) > 2) {
            $val = trim($val, end($val_array));
          }
          $title_wildcard[$val] = $val;
        }
      }
    }

    if ($field_upload_delta > 0) {
      for ($i = 0; $i < $field_upload_delta; $i++) {
        $description_wildcard_val = strtolower($form['field_sentence_dragdrop_file'][LANGUAGE_NONE][$i]['description']['#value']);
        $description_wildcard[$description_wildcard_val] = $description_wildcard_val;
      }
    }

    if (count($title_wildcard) <> $field_upload_delta) {
      form_set_error('count_error', t('There is difference between wildcards in the sentence and image uploaded. Please upload images for all the wildcards available in the sentence.'));
    } else {
      $not_found = array();
      foreach ($title_wildcard as $key => $val) {
        if (!isset($description_wildcard[$val])) {
          $not_found[] = $val;
        }
      }

      if (isset($not_found) && !empty($not_found)) {
        $not_found_str = implode(',', $not_found);
        form_set_error('wildcard_not_found', t('images for %wc wildcards are missing.', array('%wc' => $not_found_str)));
      }
    }
  }

  /**
   * Implementation of delete.
   *
   * @see QuizQuestion#delete()
   */
  public function delete($only_this_version = FALSE) {
    if ($only_this_version) {
      db_delete('quiz_sentence_drop_user_answers')
              ->condition('question_nid', $this->node->nid)
              ->condition('question_vid', $this->node->vid)
              ->execute();
    }
    // Delete all versions of this question.
    else {
      db_delete('quiz_sentence_drop_user_answers')
              ->condition('question_nid', $this->node->nid)
              ->execute();
    }
    parent::delete($only_this_version);
  }

  /**
   * Implementation of getNodeProperties.
   *
   * @see QuizQuestion#getNodeProperties()
   */
  public function getNodeProperties() {
    if (isset($this->nodeProperties) && !empty($this->nodeProperties)) {
      return $this->nodeProperties;
    }
    $props = parent::getNodeProperties();

    return $props;
  }

  /**
   * Implementation of getNodeView.
   *
   * @see QuizQuestion#getNodeView()
   */
  public function getNodeView() {
    $content = parent::getNodeView();

    return $content;
  }

  /**
   * Generates the question form.
   *
   * This is called whenever a question is rendered, either
   * to an administrator or to a quiz taker.
   */
  public function getAnsweringForm(array $form_state = NULL, $rid) {
    $form = parent::getAnsweringForm($form_state, $rid);
    $answer_options = $token = array();
    list($answer_options, $token) = $this->getSubquestions();

    $answer_options = $this->customShuffle($answer_options);
    $token = $this->customShuffle($token);

//    $question_array = explode(' ', strtolower($form['question']['#markup']));
    foreach ($token as $token_key => $token_val) {
      $form['question']['#markup'] = str_replace($token_key, $token_val, strtolower($form['question']['#markup']));
    }
    $data = array('answer_options' => $answer_options, 'token' => $token);

    $form['drag_drop_answer'] = array('#markup' => theme('quiz_sentence_drop_answer_form', array('data' => $data)));
    $form['qsdAnswerCount'] = array(
        '#type' => 'hidden',
        '#value' => '',
        '#name' => 'qsdAnswerCount',
        '#attributes' => array('id' => 'qsdAnswerCount'));
    $form['dropCount'] = array(
        '#type' => 'hidden',
        '#value' => '',
        '#name' => 'dropCount',
        '#attributes' => array('id' => 'dropCount'));
    $form['tries'] = array(
        '#type' => 'hidden',
        '#value' => 0,
    );
    $form['reset'] = array(
        '#type' => 'button',
        '#value' => t('Reset'),
        '#attributes' => array('class' => array('reset_btn')),
        '#id' => 'btnReset',
    );
    return $form;
  }

  /**
   * Helper function to generate question images.
   * 
   * @param array $variables
   *   Holds image related settings.
   *
   * @return HTML
   *   HTML of image.
   */
  private function imageStyle($variables) {
    // Determine the dimensions of the styled image.
    $dimensions = array(
      'width' => '',
      'height' => 60,
    );
    $variables['desc'] = strtolower($variables['desc']);
    image_style_transform_dimensions($variables['style_name'], $dimensions);

    $variables['width'] = $dimensions['width'];
    $variables['height'] = $dimensions['height'];

    $variables['attributes'] = array(
//      'class' => 'droppable dropbox',
        'id' => 'image_' . str_replace('@', '', $variables['desc']),
    );
    $placeholder_id = 'placeholder_' . str_replace("@", "", $variables['desc']);
    // Determine the url for the styled image.
    $variables['path'] = image_style_url($variables['style_name'], $variables['path']);
    return '<span class="img_option droppable dropbox" id="' . $placeholder_id . '">' . theme('image', $variables) . '</span>';
  }

  /**
   * Helper function to fetch subquestions.
   *
   * @return array
   *   Array with two arrays, matches and selected options
   */
  private function getSubquestions() {
    $answer_options = $image = $token = array();
    $count = 1;
    $node_detail = node_load($this->node->nid);

    if (isset($node_detail->field_sentence_dragdrop_file[LANGUAGE_NONE]) && !empty($node_detail->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
      foreach ($node_detail->field_sentence_dragdrop_file[LANGUAGE_NONE] as $key => $val) {
        $style = 'thumbnail';
        $path = $val['uri'];
        $fid = $val['fid'];
        $val['description'] = strtolower($val['description']);
        $data = array('style_name' => $style, 'path' => $path, 'fid' => $fid, 'desc' => $val['description']);

        $from_id = 'from_' . str_replace("@", "", $val['description']);
        $answer_options[$val['description']] = '<span class="draggable" id="' . $from_id . '">' . ucfirst(trim(str_replace('@', '', $val['description']))) . '</span>';

        $token[$val['description']] = $this->imageStyle($data);
        $count++;
      }
    }
    return array($answer_options, $token);
  }

  /**
   * Shuffles an array and makes sure the first element is the default element.
   *
   * @param array $array
   *   Array to be shuffled
   * 
   * @return array
   *   A shuffled version of the array with $array['def'] = '' as the first 
   * element.
   */
  private function customShuffle(array $array = array()) {
    $new_array = array();

    while (count($array)) {
      $element = array_rand($array);
      $new_array[$element] = $array[$element];
      unset($array[$element]);
    }
    return $new_array;
  }

  /**
   * Implementation of getCreationForm.
   *
   * @see QuizQuestion#getCreationForm()
   */
  public function getCreationForm(array &$form_state = NULL) {
    $form = array();

    return $form;
  }

  /**
   * Implementation of getMaximumScore.
   *
   * @see QuizQuestion#getMaximumScore()
   */
  public function getMaximumScore() {
    return 1;
  }

}

/**
 * Extension of QuizQuestionResponse
 */
class SentenceDropResponse extends QuizQuestionResponse {

  /**
   * Constructor.
   */
  public function __construct($result_id, stdClass $question_node, $tries = NULL) {
    parent::__construct($result_id, $question_node, $tries);
  }

  /**
   * Implementation of isValid.
   *
   * @see QuizQuestionResponse#isValid()
   */
  public function isValid() {
    $drop_count = $_POST['dropCount'];
    if (isset($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
      if ($drop_count != count($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
        return t('You must drop all the words on the images.');
      }
    }
    return TRUE;
  }

  /**
   * Implementation of save.
   *
   * @see QuizQuestionResponse#save()
   */
  public function save() {

    if (isset($_POST['dropCount']) && !empty($_POST['dropCount'])) {
      $drop_count = $_POST['dropCount'];
    }
    
    if (isset($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
      if (isset($drop_count) && $drop_count == count($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
        db_insert('quiz_sentence_drop_user_answers')
                ->fields(array(
                    'question_nid' => $this->question->nid,
                    'question_vid' => $this->question->vid,
                    'result_id' => $this->rid,
                    'score' => (int) $this->getScore(),
                ))
                ->execute();
      }
    }
  }

  /**
   * Implementation of delete.
   *
   * @see QuizQuestionResponse#delete()
   */
  public function delete() {
    // TODO.
  }

  /**
   * Implementation of score.
   *
   * @return int
   *   Calculates score for the user.
   *
   * @see QuizQuestionResponse#score()
   */
  public function score() {
    $score = 0;
    if (isset($_POST['qsdAnswerCount']) && !empty($_POST['qsdAnswerCount'])) {
      $answer_count = $_POST['qsdAnswerCount'];
      $dropCount = $_POST['dropCount'];

      $score = 0;

      if (isset($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
        if (isset($this->question->field_sentence_dragdrop_file) && $answer_count == count($this->question->field_sentence_dragdrop_file[LANGUAGE_NONE])) {
          $score = 1;
        }
      }
      if ($answer_count == $dropCount) {
        $score = 1;
      }
    } else {
      $result = db_query('SELECT score FROM {quiz_sentence_drop_user_answers} 
                          WHERE result_id = :result_id AND 
                                question_nid = :question_nid AND
                                question_vid = :question_vid', array(
          ':result_id' => $this->rid,
          ':question_nid' => $this->question->nid,
          ':question_vid' => $this->question->vid))->fetchField();

      if (!$result) {
        return;
      }
      $score = $result;
    }

    return $score;
  }

  /**
   * If all answers in a question is wrong.
   *
   * @return Boolean
   *   TRUE if all answers are wrong. False otherwise.
   */
  public function isAllWrong() {
    return TRUE;
  }

  /**
   * Implementation of getResponse.
   *
   * @see QuizQuestionResponse#getResponse()
   */
  public function getResponse() {
    // TODO.
  }

  /**
   * Implementation of getReportFormResponse.
   *
   * @see getReportFormResponse()
   */
  public function getReportFormResponse($showpoints = TRUE, $showfeedback = TRUE, $allow_scoring = FALSE) {
    $score = (int) $this->getScore();
    $data = array('score' => $score, 'is_skipped' => $this->is_skipped);
    // Return themed report.
    return array('#markup' => theme('quiz_sentence_drop_response', array('data' => $data)));
  }

  /**
   * Run check_markup() on the field of the specified choice alternative.
   *
   * @param String $alternative
   *   String to be checked
   * @param String $format
   *   The input format to be used
   * @param Boolean $check_user_access
   *   Whether or not we are to check the users access to the chosen format
   * 
   * @return HTML
   *   HTML markup
   */
  private function checkMarkup($alternative, $format, $check_user_access = FALSE) {
    // If the string is empty we don't run it through input
    // filters(They might add empty tags).
    if (drupal_strlen($alternative) == 0) {
      return '';
    }
    return check_markup($alternative, $format, $langcode = '' /* TODO Set this variable. */, $check_user_access);
  }
}
