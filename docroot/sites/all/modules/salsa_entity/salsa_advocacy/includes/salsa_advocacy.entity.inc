<?php

/**
 * @file
 * Definition of SalsaEntityAdvocacy class.
 */

/*
 * SalsaEntityAdvocacy class.
 */
class SalsaEntityAdvocacy extends SalsaEntity {

  /**
   * Overrides SalsaEntity::buildContent().
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();

    if ($this->Description) {
      $content['Description'] = array(
        '#type' => 'item',
        '#markup' => $this->Description,
      );
    }

    if (isset($_SESSION['Thank_You_Text'])) {
      $content['Thank_You_Text'] = array(
        '#type' => 'item',
        '#markup' => $_SESSION['Thank_You_Text'],
      );
      unset($_SESSION['Thank_You_Text']);
    }
    else {
      if ($this->Status == 'Inactive') {
        $content['Status_MSG_I'] = array(
          '#type' => 'item',
          '#markup' => t('This action is no longer active.'),
        );
        $content['Status_MSG_II'] = array(
          '#type' => 'item',
          '#markup' => t('To view the current list of our active campaigns click !here', array('!here' => l(t('here'), 'salsa/actions'))),
        );
      }
      elseif (empty($this->Deadline) || REQUEST_TIME < strtotime($this->Deadline)) {
        if (strpos($this->style_path, 'petition') !== FALSE || $this->Style == 'Petition') {
          $content['Form'] = drupal_get_form('salsa_advocacy_petition_form', $this);
        }
        elseif (strpos($this->style_path, 'targeted') !== FALSE || $this->Style == 'Targeted') {
          $content['Form'] = drupal_get_form('salsa_advocacy_targeted_form', $this);
        }
      }
      else {
        $content['Deadline_MSG'] = array(
          '#type' => 'item',
          '#markup' => t('The signature deadline has been reached for this action. Thanks for your interest!'),
        );
      }
    }

    if ($this->Footer) {
      $content['Footer'] = array(
        '#type' => 'item',
        '#markup' => $this->Footer,
      );
    }

    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
}
