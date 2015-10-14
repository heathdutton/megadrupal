<?php

namespace Drupal\campaignion_wizard;

class ConfirmStep extends WizardStep {
  protected $step = 'confirm';
  protected $title = 'Confirm';

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);

    $button = array(
      '#type' => 'submit',
      '#wizard type' => 'next',
      '#value' => t('Edit'),
      '#submit' => array('ctools_wizard_submit'),
      '#executes_submit_callback' => TRUE,
      '#attributes' => array('class' => array('confirm-edit-button')),
      '#limit_validation_errors' => array(),
    );
    $container = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('confirm-edit-wrapper')),
    );
    $form['confirm_container'] = array(
      '#type' => 'container',
    );

    foreach ($this->wizard->stepHandlers as $urlpart => $step) {
      // allow steps to don't produce a status message
      if (! ($status = $step->status()) )
        continue;

      $form['confirm_container']['to_' . $urlpart] = array(
        'button' => array('#next' => $urlpart, '#name' => 'to_' . $urlpart) + $button,
        'caption' => array('#markup' => "<h2>{$status['caption']}</h2>"),
        'description' => array('#markup' => "<p>{$status['message']}</p>"),
      ) + $container;
    }

    $form['confirm_container']['buttons'] = array(
      '#weight' => 1000,
      '#attributes' => array('class' => array('form-submit')),
    ) + $form['wizard_head']['buttons'];
    unset($form['wizard_head']['buttons']);

    // remove/add buttons
    unset($form['confirm_container']['buttons']['previous']);
    $form['confirm_container']['buttons']['return'] = array(
      '#value' => t('Publish now!'),
      '#type' => 'submit',
      '#name' => 'finish',
      '#wizard type' => 'return',
      '#attributes' => array('class' => array('button-finish')),
    );
    $form['confirm_container']['buttons']['schedule'] = array(
      '#type' => 'submit',
      '#value' => t('Schedule publishing'),
      '#name' => 'schedule',
      '#weight' => 1010,
      '#access' => FALSE,
      '#attributes' => array('class' => array('button-finish-other')),
    );
    $form['confirm_container']['buttons']['draft'] = array(
      '#type' => 'submit',
      '#value' => t('Save as draft'),
      '#name' => 'draft',
      '#weight' => 1020,
      '#wizard type' => 'return',
      '#attributes' => array('class' => array('button-finish-other')),
    );

    return $form;
  }

  public function submitStep($form, &$form_state) {
    if (isset($this->wizard->node->nid)) {
      if(isset($form_state['clicked_button']['#name'])) {
        $node = $this->wizard->node;
        $type_name = node_type_get_name($node);
        switch($form_state['clicked_button']['#name']) {
        case 'finish':
          $node->status = 1;
          node_save($node);
          drupal_set_message(t('!type published successfully.', array('!type' => $type_name)), 'status');
          $form_state['redirect'] = 'node/' . $node->nid;
          break;
        case 'draft':
          $node->status = 0;
          node_save($node);
          drupal_set_message(t('!type saved as draft.', array('!type' => $type_name)), 'status');
          $form_state['redirect'] = 'node/' . $node->nid;
          break;
        case 'schedule':
          drupal_set_message(t('Schedule is not implemented yet.'), 'warning');
          break;
        }
      }
    } else {
      drupal_set_message(t('Where is my node? Did you fill out the first step?'), 'error');
      $form_state['redirect'] = ''; // stay on the page, do not redirect
    }
  }
}
