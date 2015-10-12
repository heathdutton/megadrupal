<?php

/**
 * @file
 * Contains \Drupal\integration_ui\FormControllers\ResourceSchemaFormController.
 */

namespace Drupal\integration_ui\FormControllers;

use Drupal\integration_ui\AbstractForm;
use Drupal\integration\ResourceSchema\Configuration\ResourceSchemaConfiguration;
use Drupal\integration_ui\FormHelper;

/**
 * Class ResourceSchemaFormController.
 *
 * @package Drupal\integration_ui\FormControllers
 */
class ResourceSchemaFormController extends AbstractForm {

  /**
   * {@inheritdoc}
   */
  public function form(array &$form, array &$form_state, $op) {
    /** @var ResourceSchemaConfiguration $configuration */
    $configuration = $this->getConfiguration($form_state);
    $plugin_manager = $this->getPluginManager($form_state);

    $form['plugin'] = FormHelper::radios(
      t('Resource schema plugin'),
      $plugin_manager->getPluginDefinitions(),
      $configuration->getPlugin()
    );
    $form['settings'] = FormHelper::tree();
    $form['settings']['plugin'] = FormHelper::tree(FALSE);

    $i = 0;
    $rows = array();
    $fields = (array) $configuration->getPluginSetting('fields');
    foreach ($fields as $name => $label) {
      $form['settings']['plugin']['fields'][$name] = FormHelper::hidden($label);

      $row = array();
      $row['name'] = FormHelper::markup($name);
      $row['label'] = FormHelper::markup($label);
      $row['remove_field_' . $i] = FormHelper::stepSubmit(t('Remove'), $name);
      $row['remove_field_' . $i]['#field'] = $name;
      $rows[] = $row;
      $i++;
    }

    $rows[] = array(
      'field_name' => FormHelper::textField(NULL, NULL, FALSE),
      'field_label' => FormHelper::textField(NULL, NULL, FALSE),
      'add_field' => FormHelper::stepSubmit(t('Add'), 'add_field'),
    );

    $header = array(t('Field name'), t('Field label'), '');
    $form['settings']['fields'] = FormHelper::table($header, $rows);
  }

  /**
   * {@inheritdoc}
   */
  public function formSubmit(array $form, array &$form_state) {
    /** @var ResourceSchemaConfiguration $configuration */
    $configuration = $this->getConfiguration($form_state);
    $input = &$form_state['input'];
    $triggering_element = $form_state['triggering_element'];

    switch ($triggering_element['#name']) {

      // Add field to plugin settings.
      case 'add_field':
        if ($input['field_name'] && $input['field_label']) {
          $configuration->settings['plugin']['fields'][$input['field_name']] = $input['field_label'];
        }
        $input['field_name'] = $input['field_label'] = '';
        $form_state['rebuild'] = TRUE;
        break;

      // Remove field from plugin settings.
      case 'remove_field':
        $field_name = $triggering_element['#field'];
        unset($configuration->settings['plugin']['fields'][$field_name]);
        $form_state['rebuild'] = TRUE;
        break;
    }

    // Remove UI-related values from plugin settings.
    foreach (array('fields') as $name) {
      unset($configuration->settings[$name]);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function formValidate(array $form, array &$form_state) {

  }

}
