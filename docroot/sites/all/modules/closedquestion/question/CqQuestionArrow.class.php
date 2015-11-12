<?php

/**
 * @file
 * Implementation of the CqQuestionArrow question type.
 * A CqQuestionArrow question presents the sudent with an image
 * and the assignment to draw arrows in that image.
 */
class CqQuestionArrow extends CqQuestionAbstract {

  /**
   * HTML containing the question-text.
   *
   * @var string
   */
  private $text;

  /**
   * The base-name used for form elements that need to be accessed by
   * javascript.
   *
   * @var string
   */
  private $formElementName;

  /**
   * The url of the image to use as background for the hotspot question.
   *
   * @var string
   */
  private $matchImgUrl;

  /**
   * The width of the background image
   *
   * @var int
   */
  private $matchImgWidth;

  /**
   * The height of the background image
   *
   * @var int
   */
  private $matchImgHeight;

  /**
   * Maximum number of items the student is allowed to point out.
   * Defaults to 1;
   *
   * @var int
   */
  private $maxChoices = 1;

  /**
   * The list of hotspots in the image.
   *
   * @var array of CqHotspot
   */
  private $hotspots = array();

  /**
   * The mappings to check the student's answer against.
   *
   * @var array of CqMapping
   */
  private $mappings = array();

  /**
   * The mappings that have the correct flag set and matched the current answer.
   *
   * @var array of CqMapping
   */
  private $matchedCorrectMappings = array();

  /**
   * The mappings that matched the current answer.
   *
   * @var array of CqMapping
   */
  private $matchedMappings = array();

  /**
   * List of feedback items to use as general hints.
   *
   * @var array of CqFeedback
   */
  private $hints = array();

  /**
   * Constructs a ChemReaction question object
   *
   * @param CqUserAnswerInterface $userAnswer
   *   The CqUserAnswerInterface to use for storing the student's answer.
   * @param object $node
   *   Drupal node object that this question belongs to.
   */
  public function __construct(CqUserAnswerInterface &$userAnswer, &$node) {
    parent::__construct();
    $this->userAnswer = $userAnswer;
    $this->node = $node;
    $this->formElementName = 'cq_arrow_question' . $this->node->nid . '_';
  }

  /**
   * Implements CqQuestionAbstract::getOutput()
   */
  public function getOutput() {
    $this->initialise();
    $retval = drupal_get_form('closedquestion_get_form_for', $this->node);
    $retval['#prefix'] = $this->prefix;
    $retval['#suffix'] = $this->postfix;
    return $retval;
  }

  /**
   * Overrides CqQuestionAbstract::getDraggables()
   */
  public function getDraggables() {
    return $this->draggables;
  }

  /**
   * Overrides CqQuestionAbstract::getHotspots()
   */
  public function getHotspots() {
    return $this->hotspots;
  }

  /**
   * Overrides CqQuestionAbstract::loadXml()
   */
  public function loadXml(DOMElement $dom) {

    parent::loadXml($dom);


    module_load_include('inc.php', 'closedquestion', 'lib/XmlLib');
    module_load_include('php', 'closedquestion', 'question/mapping/FactoryHotspot');

    $this->hotspots = array();
    $this->mappings = array();
    $this->hints = array();
    $this->linestyle = $dom->getAttribute('linestyle') == '' ? 'curved' : $dom->getAttribute('linestyle');
    $this->linenumbering = $dom->getAttribute('linenumbering') !== 'no' ? true : false;
    $this->startarrow = $dom->getAttribute('startarrow') !== 'yes' ? false : true;
    $this->endarrow = $dom->getAttribute('endarrow') !== 'no' ? true : false;


    foreach ($dom->childNodes as $node) {
      $name = drupal_strtolower($node->nodeName);
      switch ($name) {
        case 'text':
          $this->text = cq_get_text_content($node, $this);
          break;

        case 'matchimg':
          $this->matchImgUrl = $node->getAttribute('src');
          $this->matchImgHeight = $node->getAttribute('height');
          $this->matchImgWidth = $node->getAttribute('width');
          $this->maxChoices = $node->getAttribute('maxChoices');
          foreach ($node->childNodes as $child) {
            switch (drupal_strtolower($child->nodeName)) {
              case 'hotspot':
                $hotspot = cq_Hotspot_from_xml($child, $this);
                if (is_object($hotspot)) {
                  if (isset($this->hotspots[$hotspot->getIdentifier()])) {
                    drupal_set_message(t('Hotspot identifier %identifier used more than once!', array('%identifier' => $hotspot->getIdentifier())), 'warning');
                  }
                  $this->hotspots[$hotspot->getIdentifier()] = $hotspot;
                }
                break;
            }
          }
          break;

        case 'mapping':
          $map = new CqMapping();
          $map->generateFromNode($node, $this);
          $this->mappings[] = $map;
          break;

        case 'hint':
          $this->hints[] = CqFeedback::newCqFeedback($node, $this);
          break;

        default:
          if (!in_array($name, $this->knownElements)) {
            drupal_set_message(t('Unknown node: @nodename', array('@nodename' => $node->nodeName)));
          }
          break;
      }
    }
  }

  /**
   * Returns user answer. This function should be used in stead of calling
   *  $this->getUserAnswer()->getAnswer() directly, as the answer needs some basic
   *  filtering.
   * @return string
   */
  public function getUserAnswerAsString() {
    return str_replace('*', '', $this->getUserAnswer()->getAnswer()); //remove # (used to store arrowInversion, but not for feedback)
  }

  /**
   * Implements CqQuestionAbstract::getFeedbackItems()
   */
  public function getFeedbackItems() {
    $tries = $this->userAnswer->getTries();
    $feedback = array();

    // if ($tries == 0) { // if there is no answer, don't check any further.
    //   return $feedback;
    // }
    // The general hints, only of the answer is not correct.
    if (!$this->isCorrect()) {
      foreach ($this->hints as $fb) {
        if ($fb->inRange($tries)) {
          $feedback[] = $fb;
        }
      }
    }

    // The new style mappings.
    if ($this->isCorrect()) {
      foreach ($this->matchedCorrectMappings AS $mapping) {
        $feedback = array_merge($feedback, $mapping->getFeedbackItems($tries));
      }
    }
    else {
      foreach ($this->matchedMappings AS $mapping) {
        $feedback = array_merge($feedback, $mapping->getFeedbackItems($tries));
      }
    }

    // Finally, ask external systems if they want to add extra feedback.
    return array_merge($feedback, $this->fireGetExtraFeedbackItems($this, $tries));
  }

  /**
   * Implements CqQuestionAbstract::checkCorrect()
   */
  public function checkCorrect() {
    $this->matchedMappings = array();
    $this->matchedCorrectMappings = array();

    $correct = FALSE;
    foreach ($this->mappings as $id => $mapping) {
      if ($mapping->evaluate()) {
        if ($mapping->getCorrect() != 0) {
          $correct = TRUE;
          $this->matchedCorrectMappings[] = $mapping;
        }
        else {
          $this->matchedMappings[] = $mapping;
        }
        if ($mapping->stopIfMatch()) {
          break;
        }
      }
      unset($mapping);
    }
    return $correct;
  }

  /**
   * Implements CqQuestionAbstract::getForm()
   */
  public function getForm($formState) {
    $nextlink = '';
    $answer = $this->getUserAnswerAsString();
    $correct = $this->isCorrect();
    $mapName = $this->formElementName . 'map';

    // The question part is themed
    $form['question']['#theme'] = 'closedquestion_question_arrow';
    $form['question']['questionText'] = array(
      '#type' => 'item',
      '#markup' => $this->text,
    );

    // The data needed by the theme function.
    $data = array();
    $data['elementname'] = $this->formElementName;
    $data['mapname'] = $mapName;
    $data['image'] = array(
      "height" => (int) $this->matchImgHeight,
      "width" => (int) $this->matchImgWidth,
      "url" => $this->matchImgUrl,
    );

    $data['linestyle'] = $this->linestyle;
    $data['linenumbering'] = $this->linenumbering;
    $data['startarrow'] = $this->startarrow;
    $data['endarrow'] = $this->endarrow;


    // Handle the hotspots
    foreach ($this->hotspots as $hotspot) {
      $description = $hotspot->getDescription();
      if (!empty($description)) {
        $termId = $this->formElementName . 'term_' . $hotspot->getIdentifier();
        $data['hotspots'][$hotspot->getIdentifier()] = array(
          'termid' => $termId,
          'maphtml' => $hotspot->getMapHtml(),
          'mapdata' => $hotspot->getMapData(),
          'description' => $description,
        );
      }
    }
    $form['question']['data'] = array('#type' => 'value', '#value' => $data);


    // Other elements are not themed by default.
    // This element will be filled by Javascript to contain the answer.
    $form[$this->formElementName . 'answer'] = array(
      '#type' => 'hidden',
      '#default_value' => $answer,
      '#input' => TRUE,
    );

    // Insert standard feedback and submit elements.
    $wrapper_id = 'cq-feedback-wrapper_' . $this->formElementName;
    $this->insertFeedback($form, $wrapper_id);
    $this->insertSubmit($form, $wrapper_id);
    return $form;
  }

  /**
   * Get the answer the student has given for the given target box, or the full
   * answer string if the given identifier is not a target box.
   *
   * @param string $identifier
   *   The target-box number to fetch the answer for.
   *
   * @return string the answer.
   */
  public function getAnswerForChoice($identifier) {
    $answer = $this->getUserAnswerAsString();
    if (is_numeric($identifier)) {
      $part = (int) $identifier;
      $start = strpos($answer, $part);
      $end = strpos($answer, $part + 1, $start);
      $length = max(0, max(drupal_strlen($answer), $end) - $start);
      // not using drupal_substr since we use a strpos generated indexes.
      return substr($aswer, $start, $length);
    }
    else {
      return $answer;
    }
  }

  /**
   * Implements CqQuestionAbstract::submitAnswer()
   */
  public function submitAnswer($form, &$form_state) {
    $newAnswer = $form_state['values'][$this->formElementName . 'answer'];
    $this->userAnswer->setAnswer($newAnswer);
    $correct = $this->isCorrect(TRUE);
    if ($this->userAnswer->answerHasChanged()) {
      if (!$correct) {
        $this->userAnswer->increaseTries();
      }
      $this->userAnswer->store();
    }
  }

  /**
   * Implements CqQuestionAbstract::getAllText()
   */
  public function getAllText() {
    $this->initialise();
    $retval = array();
    $retval['text']['#markup'] = $this->text;

    // Hints
    $retval['hints'] = array(
      '#theme' => 'closedquestion_feedback_list',
      'extended' => TRUE,
    );
    foreach ($this->hints AS $fbitem) {
      $retval['hints']['items'][] = $fbitem->getAllText();
    }

    // Mappings
    $retval['mappings'] = array(
      '#theme' => 'closedquestion_mapping_list',
      'items' => array(),
    );
    foreach ($this->mappings AS $mapping) {
      $retval['mappings']['items'][] = $mapping->getAllText();
    }

    $retval['#theme'] = 'closedquestion_question_general_text';
    return $retval;
  }

}
