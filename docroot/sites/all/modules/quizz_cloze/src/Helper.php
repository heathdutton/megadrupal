<?php

namespace Drupal\quizz_cloze;

class Helper {

  function shuffleChoices($choices) {
    $new_array = array();
    $new_array[''] = '';
    while (count($choices)) {
      $element = array_rand($choices);
      $new_array[$element] = $choices[$element];
      unset($choices[$element]);
    }
    return $new_array;
  }

  /**
   * @param string $question
   * @return string
   */
  public function getQuestionChunks($question) {
    $chunks = array();
    while (strlen($question) > 0) {
      $match = FALSE;

      if ($pos = strpos($question, '[')) {
        $substring = substr($question, 0, $pos);
        $question = substr($question, $pos);
        $chunks[] = $substring;
        $match = TRUE;
      }

      if (FALSE !== ($pos = strpos($question, ']'))) {
        $substring = substr($question, 0, $pos + 1);
        $question = preg_replace('/' . preg_quote($substring) . '/', '', $question, 1);
        $chunks[] = $substring;
        $match = TRUE;
      }

      if (!$match) {
        $chunks[] = $question;
        $question = '';
      }
    }

    return $chunks;
  }

  /**
   * @param string $question
   */
  public function getCorrectAnswerChunks($question) {
    $corrects = array();
    foreach ($this->getQuestionChunks($question) as $k => $v) {
      if (FALSE === strpos($v, '[')) {
        continue;
      }
      $answer_chunk = str_replace(array('[', ']'), '', $v);
      $choice = explode(',', $answer_chunk);
      $corrects[$k] = count($choice) == 1 ? $answer_chunk : $choice[0];
    }
    return $corrects;
  }

  public function getUserAnswer($question, $answer) {
    $input = $chunks = $this->getQuestionChunks($question);
    $correct_answer_chunks = $this->getCorrectAnswerChunks($question);
    foreach (array_keys($chunks) as $key) {
      if (isset($answer[$key]) && !empty($answer[$key])) {
        $class = ($this->getCleanText($correct_answer_chunks[$key]) == $this->getCleanText($answer[$key])) ? 'correct' : 'incorrect';
        $class .= ' answer user-answer';
        $input[$key] = '<span class="' . $class . '">' . $answer[$key] . '</span>';
      }
      elseif (isset($answer[$key])) {
        $input[$key] = '<span class="incorrect answer user-answer">' . str_repeat('_', strlen($correct_answer_chunks[$key])) . '</span>';
      }
    }
    return str_replace("\n", "<br/>", implode(' ', $input));
  }

  /**
   * Makes the given text consistent for comparison
   */
  public function getCleanText($text) {
    return drupal_strtolower(trim($text));
  }

}
