<?php

/**
 * @file
 * File for TMGMT Moravia translator UI controller.
 *
 * Url: http://www.moravia.com/
 */

/**
 * TMGMT Moravia translator UI controller.
 */
class TMGMTMoraviaTranslatorUIController extends TMGMTDefaultTranslatorUIController {

  /**
   * Get form settings.
   */
  public function pluginSettingsForm($form, &$form_state, TMGMTTranslator $translator, $busy = FALSE) {

    $form['symfonie_token'] = array(
      '#type' => 'textfield',
      '#title' => t('Moravia Symfonie Token'),
      '#default_value' => $translator->getSetting('symfonie_token'),
      '#description' => t('Please enter Authentification Token from Moravia Symfonie Extranet'),
    );
    return parent::pluginSettingsForm($form, $form_state, $translator, $busy);
  }

  /**
   * Get task name.
   *
   * @param TMGMTJob $job
   *   Job.
   *
   * @return string
   *   Task name.
   */
  private function getDefaultTaskName(TMGMTJob $job) {
    $items = $job->getItems();
    foreach ($items as $item) {
      $task_name = '"' . $item->getSourceLabel() . '"';
      break;
    }
    $count_other = count($items) - 1;
    if ($count_other == 1) {
      $task_name .= " and 1 other";
    } if ($count_other > 1) {
      $task_name .= " and " . $count_other - 1  . " others";
    }
    return $task_name;
  }

  /**
   * Check form settings.
   */
  public function checkoutSettingsForm($form, &$form_state, TMGMTJob $job) {
    $form['moravia_project'] = array(
      '#type' => 'select',
      '#title' => t('Symfonie project'),
      '#options' => $job->getTranslator()->getController()->getProjects($job->getTranslator()),
      '#default_value' => $job->getTranslator()->getSetting('export_format'),
      '#description' => t('Please select a Symfonie project.'),
    );

    $form['moravia_task_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Task name'),
      '#default_value' => $this->getDefaultTaskName($job),
      '#description' => t('Please select a task name'),
      '#size' => 160,
    );

    $form['moravia_task_due_date'] = array(
      '#type' => 'fieldset',
      '#title' => t('Deadline') . " - " . drupal_get_user_timezone() . " timezone",
    );

    $form['moravia_task_due_date']['date'] = array(
      '#type' => 'date',
      '#title' => t('Due date'),
      '#default_value' => "",
      '#description' => t('Please select a deadline'),
    );

    $hours_options = array();
    foreach (range(0, 23) as $hour) {
      $hours_options[$hour] = "$hour:00";
    }
    $form['moravia_task_due_date']['time'] = array(
      '#title' => t('Due time'),
      '#default_value' => 17,
      '#type' => 'select',
      '#options' => $hours_options,
    );

    return parent::checkoutSettingsForm($form, $form_state, $job);
  }
}
