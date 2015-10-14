<?php

namespace Drupal\campaignion_wizard;

abstract class NodeWizard extends \Drupal\oowizard\Wizard {
  public $node;
  public $parameters;
  protected $levels;
  protected $status;

  public function __construct($parameters = array(), $node = NULL, $type = NULL, $user = NULL) {
    $this->parameters = $parameters;
    foreach ($this->steps as &$class) {
      if ($class[0] != '\\') {
        $class = '\\' . __NAMESPACE__ . '\\' . $class;
      }
    }
    $this->levels = array_flip(array_keys($this->steps));
    $this->status = NULL;

    $this->user = $user ? $user : $GLOBALS['user'];
    $this->node = $node ? $node : $this->prepareNode($type);
    node_object_prepare($this->node);
    parent::__construct($user);
    $this->formInfo['path'] = $node ? "node/{$node->nid}/wizard/%step" : "wizard/{$this->node->type}";

    drupal_set_title(t('Create ' . node_type_get_name($this->node)));
    $this->formInfo += array(
      'show return' => TRUE,
      'return path' => $node ? 'node/' . $this->node->nid : 'node',
    );
    $this->status = !empty($this->node->nid) ? Status::loadOrCreate($this->node->nid) : new Status();
  }

  public function setStep($step) {
    parent::setStep($step);
    if (!$this->status->step || $this->levels[$this->status->step] < $this->levels[$step]) {
      $this->status->step = $step;
    }
  }

  public function wizardForm() {
    $form = parent::wizardForm() + array(
      'wizard_head' => array(),
      'wizard_advanced' => array(),
    );
    $form['wizard_head']['trail'] = $this->trail();

    return $form;
  }

  public function prepareNode($type) {
    $node = (object) array('type' => $type);
    $node->uid  = $this->user->uid;
    $node->name = isset($this->user->name) ? $this->user->name : NULL;
    $node->language = LANGUAGE_NONE;
    $node->title = '';
    $node->sticky = 0;
    return $node;
  }

  public function trailItems() {
    $trail = array();
    $accessible = TRUE;
    $completed = empty($this->status) ? -1 : $this->levels[$this->status->step];
    foreach ($this->stepHandlers as $urlpart => $step) {
      $is_current = $urlpart == $this->currentStep;
      $trail[] = array(
        'url' => strtr($this->formInfo['path'], array('%step' => $urlpart)),
        'title' => $step->getTitle(),
        'accessible' => $accessible = ($accessible && ($this->levels[$urlpart] <= $completed) && $step->checkDependencies()),
        'current' => $urlpart == $this->currentStep,
      );
    }
    return $trail;
  }

  public function submit($form, &$form_state) {
    parent::submit($form, $form_state);
    $this->status->nid = $this->node->nid;
    $this->status->save();
  }
}
