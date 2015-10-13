<?php

namespace Drupal\quizz_scale\Form;

use Drupal\quizz_question\Entity\Question;

class ScaleQuestionForm {

  /** @var Question */
  private $question;

  public function __construct(Question $question) {
    $this->question = $question;
  }

  public function get(array &$form_state = NULL) {
    global $user;

    $form = array();

    // Getting presets from the database
    $collections = quizz_scale_collection_controller()->getPresetCollections($this->question->type, $user->uid, TRUE);

    $options = $this->makeOptions($collections);
    $options['d'] = '-'; // Default

    $form['answer'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Answer'),
        '#description' => t('Provide alternatives for the user to answer.'),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
        '#weight'      => -4,
    );
    $form['answer']['#theme'][] = 'quizz_scale_creation_form';
    $form['answer']['presets'] = array(
        '#type'          => 'select',
        '#title'         => t('Presets'),
        '#options'       => $options,
        '#default_value' => 'd',
        '#description'   => t('Select a set of alternatives'),
    );
    $max_num_alts = $this->question->getQuestionType()->getConfig('scale_max_num_of_alts', 10);

    // @TODO: use #attached
    $this->includeJSSettings($collections, $max_num_alts);

    $form['answer']['alternatives'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Alternatives'),
        '#collapsible' => TRUE,
        '#collapsed'   => TRUE,
    );

    for ($i = 0; $i < $max_num_alts; $i++) {
      $form['answer']['alternatives']["alternative$i"] = array(
          '#type'          => 'textfield',
          '#title'         => t('Alternative !i', array('!i' => ($i + 1))),
          '#size'          => 60,
          '#maxlength'     => 256,
          '#default_value' => isset($this->question->{$i}->answer) ? $this->question->{$i}->answer : '',
          '#required'      => $i < 2,
      );
    }

    $form['answer']['alternatives']['save_new_collection'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Save as a new preset'),
        '#description'   => t('Current alternatives will be saved as a new preset'),
        '#default_value' => FALSE,
    );

    $form['answer']['alternatives']['new_collection_label'] = array(
        '#type'   => 'textfield',
        '#title'  => t('Label'),
        '#states' => array(
            'visible' => array(
                ':input[name="save_new_collection"]' => array('checked' => TRUE),
            ),
        ),
    );

    $form['answer']['manage'] = array(
        '#markup' => l(t('Manage presets'), 'admin/structure/quizz-questions/manage/' . $this->question->getQuestionType()->type),
        '#access' => entity_access('update', 'quiz_question_type', $this->question->getQuestionType(), $user),
    );

    return $form;
  }

  /**
   * Makes a javascript constructing an answer collection array.
   * @param $collections
   */
  private function includeJSSettings(array $collections, $max_alternatives) {
    $alternatives = array();
    foreach ($collections as $id => $collection) {
      if (is_array($collection->alternatives)) {
        foreach ($collections[$id]->alternatives as $aid => $answer) {
          $alternatives[$id][$aid] = check_plain($answer);
        }
      }
    }

    drupal_add_js(array(
        'quizz_scale_alternatives' => array(
            'alternatives'     => $alternatives,
            'max_alternatives' => $max_alternatives,
        )
      ), 'setting');
  }

  /**
   * Makes options array for form elements.
   *
   * @param $collections
   *  collections array, from getPresetCollections() for instance…
   * @return
   *  #options array.
   */
  private function makeOptions(array $collections = NULL) {
    $options = array();
    foreach ($collections as $id => $collection) {
      $options[$id] = $collection->label;
    }
    return $options;
  }

}
