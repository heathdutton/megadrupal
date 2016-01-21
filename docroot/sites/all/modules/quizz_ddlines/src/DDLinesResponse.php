<?php

namespace Drupal\quizz_ddlines;

use Drupal\quizz\Entity\Answer;
use Drupal\quizz_question\Entity\Question;
use Drupal\quizz_question\ResponseHandler;

class DDLinesResponse extends ResponseHandler {

  protected $base_table = 'quiz_ddlines_answer';

  public function __construct($result_id, Question $question, $input = NULL) {
    parent::__construct($result_id, $question, $input);

    if (NULL === $input) {
      if (($answer = $this->loadAnswerEntity()) && ($input = $answer->getInput())) {
        $this->answer = $input;
      }
    }
    else {
      // Input decoded as JSON: [ {"label_id":x,"hotspot_id":y} ]
      if (($decoded = json_decode($input)) && is_array($decoded)) {
        foreach ($decoded as $answer) {
          $this->answer[$answer->label_id] = $answer->hotspot_id;
        }
      }
    }
  }

  public function onLoad(Answer $answer) {
    $sql = 'SELECT label_id, hotspot_id FROM {quiz_ddlines_answer} WHERE answer_id = :id';
    $query = db_query($sql, array(':id' => $answer->id));
    $input = array();
    while ($row = $query->fetch()) {
      $input[$row->label_id] = $row->hotspot_id;
    }
    $answer->setInput($input);
  }

  /**
   * {@inheritdoc}
   */
  public function save() {
    $answer_id = $this->loadAnswerEntity()->id;
    $insert = db_insert('quiz_ddlines_answer_multi')->fields(array('answer_id', 'label_id', 'hotspot_id'));
    foreach ($this->answer as $key => $value) {
      $insert->values(array($answer_id, $key, $value));
    }
    $insert->execute();
  }

  /**
   * {@inheritdoc}
   */
  public function score() {
    $results = $this->getDragDropResults();

    // Count number of correct answers:
    $correct_count = 0;

    foreach ($results as $result) {
      $correct_count += ($result == QUIZZ_DDLINES_CORRECT) ? 1 : 0;
    }

    return $correct_count;
  }

  public function getFeedbackValues() {
    // Have to do node_load, since quiz does not do this. Need the field_image…
    $img_field = field_get_items('quiz_question_entity', quizz_question_load($this->question->qid), 'field_image');
    $img_rendered = theme('image', array('path' => image_style_url('large', $img_field[0]['uri'])));

    $image_path = base_path() . drupal_get_path('module', 'quizz_ddlines') . '/theme/images/';

    $html = '<h3>' . t('Your answers') . '</h3>';
    $html .= '<div class="icon-descriptions"><div><img src="' . $image_path . 'icon_ok.gif">' . t('Means alternative is placed on the correct spot') . '</div>';
    $html .= '<div><img src="' . $image_path . 'icon_wrong.gif">' . t('Means alternative is placed on the wrong spot, or not placed at all') . '</div></div>';
    $html .= '<div class="quiz-ddlines-user-answers" id="' . $this->question->qid . '">';
    $html .= $img_rendered;
    $html .= '</div>';
    $html .= '<h3>' . t('Correct answers') . '</h3>';
    $html .= '<div class="quiz-ddlines-correct-answers" id="' . $this->question->qid . '">';
    $html .= $img_rendered;
    $html .= '</div>';

    // No form to put things in, are therefore using the js settings instead
    $settings = array();
    $correct_id = "correct-{$this->question->qid}";
    $settings[$correct_id] = json_decode($this->question->ddlines_elements);
    $elements = $settings[$correct_id]->elements;

    // Convert the user's answers to the same format as the correct answers
    $answers = clone $settings[$correct_id];
    // Keep everything except the elements:
    $answers->elements = array();

    $elements_answered = array();

    foreach ($this->answer as $label_id => $hotspot_id) {
      if (!isset($hotspot_id)) {
        continue;
      }

      // Find correct answer:
      $element = array(
          'feedback_wrong'   => '',
          'feedback_correct' => '',
          'color'            => $this->getElementColor($elements, $label_id)
      );

      $label = $this->getLabel($elements, $label_id);
      $hotspot = $this->getHotspot($elements, $hotspot_id);

      if (isset($hotspot)) {
        $elements_answered[] = $hotspot->id;
        $element['hotspot'] = $hotspot;
      }

      if (isset($label)) {
        $elements_answered[] = $label->id;
        $element['label'] = $label;
      }

      $element['correct'] = $this->isAnswerCorrect($elements, $label_id, $hotspot_id);
      $answers->elements[] = $element;
    }

    // Need to add the alternatives not answered by the user.
    // Create dummy elements for these:
    foreach ($elements as $el) {
      if (!in_array($el->label->id, $elements_answered)) {
        $element = array(
            'feedback_wrong'   => '',
            'feedback_correct' => '',
            'color'            => $el->color,
            'label'            => $el->label,
        );
        $answers->elements[] = $element;
      }

      if (!in_array($el->hotspot->id, $elements_answered)) {
        $element = array(
            'feedback_wrong'   => '',
            'feedback_correct' => '',
            'color'            => $el->color,
            'hotspot'          => $el->hotspot,
        );
        $answers->elements[] = $element;
      }
    }

    $settings["answers-{$this->question->qid}"] = $answers;
    $settings['mode'] = 'result';
    $settings['execution_mode'] = $this->question->execution_mode;
    $settings['hotspot']['radius'] = $this->question->hotspot_radius;

    // Image path:
    $settings['quiz_imagepath'] = base_path() . drupal_get_path('module', 'quizz_ddlines') . '/theme/images/';

    drupal_add_js(array('quiz_ddlines' => $settings), 'setting');

    quizz_ddlines_add_js_and_css();

    return array('#markup' => $html);
  }

  private function getElementColor($list, $id) {
    foreach ($list as $element) {
      if ($element->label->id == $id) {
        return $element->color;
      }
    }
  }

  private function getHotspot($list, $id) {
    foreach ($list as $element) {
      if ($element->hotspot->id == $id) {
        return $element->hotspot;
      }
    }
  }

  private function getLabel($list, $id) {
    foreach ($list as $element) {
      if ($element->label->id == $id) {
        return $element->label;
      }
    }
  }

  private function isAnswerCorrect($list, $label_id, $hotspot_id) {
    foreach ($list as $element) {
      if ($element->label->id == $label_id) {
        return ($element->hotspot->id == $hotspot_id);
      }
    }
    return FALSE;
  }

  /**
   * Get a list of the labels, tagged correct, false, or no answer
   */
  private function getDragDropResults() {
    $results = array();

    // Iterate through the correct answers, and check the users answer:
    foreach (json_decode($this->question->ddlines_elements)->elements as $element) {
      $source_id = $element->label->id;

      if (isset($this->answer[$source_id])) {
        $results[$element->label->id] = ($this->answer[$source_id] == $element->hotspot->id) ? QUIZZ_DDLINES_CORRECT : QUIZZ_DDLINES_WRONG;
      }
      else {
        $results[$element->label->id] = 0;
      }
    }

    return $results;
  }

}
