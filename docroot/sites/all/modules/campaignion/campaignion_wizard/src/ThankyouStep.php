<?php

namespace Drupal\campaignion_wizard;

use \Drupal\campaignion\Forms\EmbeddedNodeForm;

class ThankyouStep extends WizardStep {
  protected $step = 'thank';
  protected $title = 'Thank you';
  protected $contentType;
  protected $referenceField;
  protected $doubleOptIn;

  public function __construct($wizard) {
    parent::__construct($wizard);
    $parameters = drupal_array_merge_deep(array(
      'thank_you_page' => array(
        'type' => 'thank_you_page',
        'reference' => 'field_thank_you_pages',
      ),
    ), $wizard->parameters);
    $parameters =& $parameters['thank_you_page'];
    $this->contentType = $parameters['type'];
    $this->referenceField = &$wizard->node->{$parameters['reference']};
    $this->doubleOptIn = !empty($wizard->node->nid) && $this->hasDoubleOptIn();
  }

  protected function hasDoubleOptIn() {
    return db_query(
      'SELECT 1 FROM {webform_confirm_email} WHERE nid=:nid AND email_type = :conf_request',
      array(
        ':nid'          => $this->wizard->node->nid,
        ':conf_request' => WEBFORM_CONFIRM_EMAIL_CONFIRMATION_REQUEST,
      )
    )->fetchField();
  }

  protected function setConfirmationRedirect($url) {
    $sql = 'UPDATE {webform_confirm_email} SET redirect_url=:url WHERE nid=:nid AND email_type=:conf_request';
    $args = array(
      ':url'          => $url,
      ':nid'          => $this->wizard->node->nid,
      ':conf_request' => WEBFORM_CONFIRM_EMAIL_CONFIRMATION_REQUEST,
    );
    db_query($sql, $args);
  }

  protected function loadIncludes() {
    module_load_include('pages.inc', 'node');
    module_load_include('inc', 'webform', 'includes/webform.emails');
    module_load_include('inc', 'webform', 'includes/webform.components');
  }

  protected function pageForm(&$form_state, $index, $title, $prefix) {
    $field = &$this->referenceField['und'][$index];

    if (isset($field['node_reference_nid'])) {
      $type = 'node';
      $node = node_load($field['node_reference_nid']);
      $old['redirect_url'] = '';
    }
    else {
      $type = 'redirect';
      $node = $this->wizard->prepareNode($this->contentType);
      if (isset($field['redirect_url']) == FALSE) {
        $old['redirect_url'] = '';
      }
      else {
        $old['redirect_url'] = $field['redirect_url'];
      }
    }

    $form = array(
      '#type'  => 'fieldset',
      '#title' => $title,
    );
    $form['type'] = array(
      '#type'          => 'radio',
      '#title'         => t('Redirect supporters to a URL after the action'),
      '#return_value'  => 'redirect',
      '#default_value' => $type == 'redirect' ? 'redirect' : NULL,
      '#parents'       => array($prefix, 'type'),
    );
    $form['redirect_url'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Redirect URL'),
      '#states'        => array('visible' => array(":input[name=\"${prefix}[type]\"]" => array('value' => 'redirect'))),
      '#default_value' => $old['redirect_url'],
    );
    $form['or2'] = array(
      '#type'   => 'markup',
      '#markup' => '<div class="thank-you-outer-or"><span class="thank-you-line-or"><span class="thank-you-text-or">' . t('or') . '</span></span></div>',
    );
    $form['type3'] = array(
      '#type'          => 'radio',
      '#title'         => t('Create new thank you page'),
      '#return_value'  => 'node',
      '#default_value' => $type == 'node' ? 'node' : NULL,
      '#parents'       => $form['type']['#parents'],
    );
    $embedState = array(
      '#wizard_type' => 'thank_you',
      '#wizard_node' => $this->wizard->node,
    );
    $parents = array($prefix, 'node_form');
    $formObj = new EmbeddedNodeForm($node, $form_state, $parents, $embedState);
    $node_form = array(
      '#type'    => 'container',
      '#states'  => array('visible' => array(":input[name=\"${prefix}[type]\"]" => array('value' => 'node'))),
      '#tree'    => TRUE,
    ) + $formObj->formArray($form);

    $node_form['title']['#required'] = FALSE;
    // don't publish per default
    $node_form['options']['status']['#default_value'] = 0;
    $node_form['options']['promote']['#default_value'] = 0;

    // order the form fields
    $node_form['field_main_image']['#attributes']['class'][] = 'sidebar-narrow-right';
    $node_form['#tree'] = TRUE;
    $node_form['actions']['#access'] = FALSE;

    $form['node_form'] =& $node_form;
    $form['#attributes']['class'][] = 'thank-you-node-wrapper';

    $form['#tree'] = TRUE;
    return $form;
  }

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);

    // check if double opt in was enabled and if yes provide a 2nd thank you page
    $thank_you_class = 'half-left';
    if ($this->doubleOptIn) {
      $form['submission_node'] = $this->pageForm($form_state, 0, t('Submission page'), 'submission_node');
      $form['submission_node']['#attributes']['class'][] = 'half-left';
      $thank_you_class = 'half-right';
      $form['#attributes']['class'][] = 'two-halfs';
    }

    $form['thank_you_node'] = $this->pageForm($form_state, 1, t('Thank you page'), 'thank_you_node');
    $form['thank_you_node']['#attributes']['class'][] = $thank_you_class;

    $form['#tree'] = TRUE;
    $form['wizard_head']['#tree'] = FALSE;
    return $form;
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

  public function validateStep($form, &$form_state) {
    $values =& $form_state['values'];
    $thank_you_pages = array('thank_you_node');
    if ($this->doubleOptIn) {
      $thank_you_pages[] = 'submission_node';
    }
    foreach ($thank_you_pages as $page) {
      if (in_array($values[$page]['type'], array('node', 'redirect')) == FALSE) {
        form_set_error('type', t('You have to create either a thank you page or provide a redirect.'));
      }
      if ($values[$page]['type'] == 'node') {
        $form_state['embedded'][$page]['node_form']['formObject']->validate($form, $form_state);
        if (empty($values[$page]['node_form']['title'])) {
          form_set_error("$page][node_form][title", t('!name field is required.', array('!name' => 'Title')));
        }
      }
      elseif ($values[$page]['type'] == 'redirect') {
        if (empty($values[$page]['redirect_url'])) {
          form_set_error('redirect_url', t('You need to provide either a redirect url or create a thank you page.'));
        }
      }
    }
  }

  public function submitStep($form, &$form_state) {
    $values =& $form_state['values'];
    unset($form_state['values']);
    $action = $this->wizard->node;

    $thank_you_pages = array('thank_you_node' => 1);
    if ($this->doubleOptIn) {
      $thank_you_pages['submission_node'] = 0;
    }

    foreach(array(0,1) as $index) {
      $field = &$this->referenceField[LANGUAGE_NONE][$index];
      $field['node_reference_nid'] = NULL;
      $field['redirect_url'] = NULL;
    }

    foreach($thank_you_pages as $page => $index) {
      $field = &$this->referenceField[LANGUAGE_NONE][$index];
      if ($values[$page]['type'] == 'node') {
        $form_state['values'] =& $values[$page]['node_form'];

        $formObj = $form_state['embedded'][$page]['node_form']['formObject'];
        $formObj->submit($form, $form_state);

        $field['node_reference_nid'] = $formObj->node()->nid;
        $field['redirect_url']       = NULL;
        $path = 'node/' . $form_state['values']['nid'];
        if (count($thank_you_pages) == 1) {
          $action->webform['redirect_url'] = $path;
        }
        else {
          if ($page == 'thank_you_node') {
            $this->setConfirmationRedirect($path);
          }
          else {
            $action->webform['redirect_url'] = $path;
          }
        }
      }
      else {
        $field['node_reference_nid'] = NULL;
        $field['redirect_url']       = $values[$page]['redirect_url'];

        if (count($thank_you_pages) == 1) {
          $action->webform['redirect_url'] = $values[$page]['redirect_url'];
        }
        else {
          if ($page == 'thank_you_node') {
            $this->setConfirmationRedirect($values[$page]['redirect_url']);
          }
          else {
            $action->webform['redirect_url'] = $values[$page]['redirect_url'];
          }
        }
      }
    }

    node_save($action);

    $form_state['values'] =& $values;
  }

  public function status() {
    $thank_you_pages = $this->referenceField[LANGUAGE_NONE];

    $msg = t("After your supporters submitted their filled out form ");

    if (   isset($thank_you_pages[0]['redirect_url'])       == TRUE
        || isset($thank_you_pages[0]['node_reference_nid']) == TRUE) {

      if (isset($thank_you_pages[0]['redirect_url']) == TRUE) {
        $msg .= t(
          "you're redirecting them to !link .",
          array(
            '!link' => l(
              $thank_you_pages[0]['redirect_url'],
              $thank_you_pages[0]['redirect_url']
            )
          )
        );
      }
      else {
        $node = node_load($thank_you_pages[0]['node_reference_nid']);
        $msg .= t('they\'ll see your submission page !node.', array('!node' => l($node->title, 'node/' . $node->nid)));
      }
      $msg .= t("<br>After your supporters clicked the confirmation link ");
    }

    if (isset($thank_you_pages[1]['redirect_url']) == TRUE) {
      $msg .= t(
        "you're redirecting them to !link .",
        array(
          '!link' => l(
            $thank_you_pages[1]['redirect_url'],
            $thank_you_pages[1]['redirect_url']
          )
        )
      );
    }
    else {
      $node = node_load($thank_you_pages[1]['node_reference_nid']);
      $msg .= t('they\'ll see your thank you page !node.', array('!node' => l($node->title, 'node/' . $node->nid)));
    }

    return array(
      'caption' => t('Thank you page'),
      'message' => $msg,
    );
  }
}
