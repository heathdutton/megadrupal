<?php

namespace Drupal\quizz_multichoice;

use Drupal\quizz_question\Entity\Question;

class FormDefinition {

  /** @var Question */
  private $question;

  public function __construct(Question $question) {
    $this->question = $question;
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
      $settings['choice_multi'] = $this->question->choice_multi;
      $settings['choice_random'] = $this->question->choice_random;
      $settings['choice_boolean'] = $this->question->choice_boolean;
    }
    // The user is creating his first multichoice node
    elseif (!$settings = $this->getUserSettings()) {
      $settings['choice_multi'] = 0;
      $settings['choice_random'] = 0;
      $settings['choice_boolean'] = 0;
    }

    return $settings;
  }

  /**
   * Fetches the users default settings for the creation form
   *
   * @return
   *  The users default node settings
   */
  private function getUserSettings() {
    global $user;
    $res = db_query('SELECT choice_multi, choice_boolean, choice_random
            FROM {quizz_multichoice_user_settings}
            WHERE uid = :uid', array(':uid' => $user->uid))->fetchAssoc();
    return $res ? $res : FALSE;
  }

  public function get(&$form_state) {
    $form = array();

    // We add #action to the form because of the use of ajax
    $options = array();
    $get = $_GET;
    unset($get['q']);
    if (!empty($get)) {
      $options['query'] = $get;
    }

    $action = url('quiz-question/add/' . str_replace('_', '-', $this->question->getQuestionType()->type), $options);
    if (isset($this->question->qid)) {
      $action = url('quiz-question/' . $this->question->qid . '/edit', $options);
    }
    $form['#action'] = $action;
    $form['#attached']['js'][] = drupal_get_path('module', 'quizz_multichoice') . '/js/multichoice.js';

    // choice_count might be stored in the form_state after an ajax callback
    if (isset($form_state['values']['op']) && $form_state['values']['op'] == t('Add choice')) {
      $form_state['choice_count'] ++;
    }
    else {
      $form_state['choice_count'] = max($this->question->getQuestionType()->getConfig('multichoice_def_num_of_alts', 2), isset($this->question->alternatives) ? count($this->question->alternatives) : 0);
    }

    $this->getChoiceWrapper($form);
    for ($i = 0; $i < $form_state['choice_count']; $i++) {
      $this->getChoice($form, $i);
    }

    return $form;
  }

  private function getChoiceWrapper(&$form) {
    drupal_add_tabledrag('multichoice-alternatives-table', 'order', 'sibling', 'multichoice-alternative-weight');

    $form['alternatives'] = array(
        '#theme'       => array('multichoice_alternative_creation_table'),
        '#type'        => 'fieldset',
        '#title'       => t('Answer'),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
        '#weight'      => -4,
        '#tree'        => TRUE,
        '#prefix'      => '<div class="clear-block" id="multichoice-alternatives-wrapper">',
        '#suffix'      => '</div>',
        'more'         => array('#limit_validation_errors' => array()) + array(
          '#type'   => 'button',
          '#value'  => t('Add choice'),
          '#ajax'   => array(
              'method'   => 'replace',
              'wrapper'  => 'multichoice-alternatives-wrapper',
              'callback' => 'quizz_multichoice_add_alternative_ajax_callback',
          ),
          '#weight' => 50,
        ),
    );

    // Get the nodes settings, users settings or default settings
    $default_settings = $this->getDefaultAltSettings();

    $form['alternatives']['#theme'][] = 'quizz_multichoice_creation_form';
    $form['alternatives']['settings'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Settings'),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
        '#description' => t('Your settings will be remembered.'),
        '#weight'      => 100,
    );

    $form['alternatives']['settings']['choice_multi'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Multiple answers'),
        '#description'   => t('Allow any number of answers(checkboxes are used). If this box is not checked, one, and only one answer is allowed(radiobuttons are used).'),
        '#default_value' => $default_settings['choice_multi'],
        '#parents'       => array('choice_multi'),
    );

    $form['alternatives']['settings']['choice_random'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Random order'),
        '#description'   => t('Present alternatives in random order when @quiz is being taken.', array('@quiz' => QUIZZ_NAME)),
        '#default_value' => $default_settings['choice_random'],
        '#parents'       => array('choice_random'),
    );

    $form['alternatives']['settings']['choice_boolean'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Simple scoring'),
        '#description'   => t('Give max score if everything is correct. Zero points otherwise.'),
        '#default_value' => $default_settings['choice_boolean'],
        '#parents'       => array('choice_boolean'),
    );
  }

  private function getChoice(&$form, $i) {
    $short = isset($this->question->alternatives[$i]) ? $this->question->alternatives[$i] : NULL;
    $form['alternatives'][$i] = array(
        '#type'        => 'container',
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
      // - The two first alternatives won't be collapsed.
      // - Populated alternatives won't be collapsed
    );

    if (is_array($short)) {
      if ($short['score_if_chosen'] == $short['score_if_not_chosen']) {
        $correct_default = isset($short['correct']) ? $short['correct'] : FALSE;
      }
      else {
        $correct_default = $short['score_if_chosen'] > $short['score_if_not_chosen'];
      }
    }
    else {
      $correct_default = FALSE;
    }
    $form['alternatives'][$i]['correct'] = array(
        '#type'          => 'checkbox',
        '#default_value' => $correct_default,
        '#attributes'    => array(
            'onchange' => 'Multichoice.refreshScores(this, ' . $this->question->getQuestionType()->getConfig('multichoice_def_scoring', 0) . ')'
        ),
    );

    // We add id to be able to update the correct alternatives if the node is updated, without destroying
    // existing answer reports
    $form['alternatives'][$i]['id'] = array(
        '#type'  => 'value',
        '#value' => $short['id'],
    );

    $form['alternatives'][$i]['answer'] = array(
        '#type'          => 'text_format',
        '#default_value' => $short['answer']['value'],
        '#required'      => $i < 2,
        '#format'        => isset($short['answer']['format']) ? $short['answer']['format'] : NULL,
        '#rows'          => 2,
    );

    $form['alternatives'][$i]['advanced'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Advanced options'),
        '#collapsible' => TRUE,
        '#collapsed'   => TRUE,
    );
    $form['alternatives'][$i]['advanced']['feedback_if_chosen'] = array(
        '#type'          => 'text_format',
        '#title'         => t('Feedback if chosen'),
        '#description'   => t('This feedback is given to users who chooses this alternative.'),
        '#parents'       => array('alternatives', $i, 'feedback_if_chosen'),
        '#default_value' => $short['feedback_if_chosen']['value'],
        '#format'        => isset($short['feedback_if_chosen']['format']) ? $short['feedback_if_chosen']['format'] : NULL,
        '#rows'          => 2,
    );

    // We add 'helper' to trick the current version of the wysiwyg module to add
    // an editor to several textareas in the same fieldset
    $form['alternatives'][$i]['advanced']['helper']['feedback_if_not_chosen'] = array(
        '#type'          => 'text_format',
        '#title'         => t('Feedback if not chosen'),
        '#description'   => t('This feedback is given to users who doesn\'t choose this alternative.'),
        '#parents'       => array('alternatives', $i, 'feedback_if_not_chosen'),
        '#default_value' => $short['feedback_if_not_chosen']['value'],
        '#format'        => isset($short['feedback_if_not_chosen']['format']) ? $short['feedback_if_not_chosen']['format'] : NULL,
        '#rows'          => 2,
    );

    $form['alternatives'][$i]['advanced']['score_if_chosen'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Score if chosen'),
        '#size'          => 4,
        '#maxlength'     => 4,
        '#default_value' => isset($this->question->alternatives[$i]['score_if_chosen']) ? $this->question->alternatives[$i]['score_if_chosen'] : 0,
        '#description'   => t("This score is added to the user's total score if the user chooses this alternative."),
        '#attributes'    => array(
            'onkeypress' => 'Multichoice.refreshCorrect(this)',
            'onkeyup'    => 'Multichoice.refreshCorrect(this)',
            'onchange'   => 'Multichoice.refreshCorrect(this)'
        ),
        '#parents'       => array('alternatives', $i, 'score_if_chosen')
    );

    $form['alternatives'][$i]['advanced']['score_if_not_chosen'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Score if not chosen'),
        '#size'          => 4,
        '#maxlength'     => 4,
        '#default_value' => isset($short['score_if_not_chosen']) ? $short['score_if_not_chosen'] : 0,
        '#description'   => t("This score is added to the user's total score if the user doesn't choose this alternative. Only used if multiple answers are allowed."),
        '#attributes'    => array(
            'onkeypress' => 'Multichoice.refreshCorrect(this)',
            'onkeyup'    => 'Multichoice.refreshCorrect(this)',
            'onchange'   => 'Multichoice.refreshCorrect(this)'
        ),
        '#parents'       => array('alternatives', $i, 'score_if_not_chosen')
    );

    $form['alternatives'][$i]['weight'] = array(
        '#type'          => 'textfield',
        '#size'          => 2,
        '#attributes'    => array('class' => array('multichoice-alternative-weight')),
        '#default_value' => isset($this->question->alternatives[$i]['weight']) ? $this->question->alternatives[$i]['weight'] : $i,
    );
  }

}
