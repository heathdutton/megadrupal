<?php
/**
 * @file
 * IDC Implementation to create / edit features.
 */

$plugin = array(
  'handler class' => 'FeaturesBuilder',
  'dependencies' => array(
    'features',
  ),
);

/**
 * Class FeaturesBuilder
 */
class FeaturesBuilder extends IDCCommandBase implements IDCInterface {

  public static $commandName = 'features-builder';

  public $showExportedComponents = FALSE;

  public static function getCommandDefinition() {
    return array(
      'description' => 'Utility to easily build features through interactive steps.',
    );
  }

  public function getCommandSteps() {
    return array(
      'choose_operation',
      'choose_feature',
      'choose_component_source',
      'choose_components',
    );
  }

  public function choose_operation($processed = FALSE) {
    if (!$processed) {
      return new IDCStepSingleChoice('Choose the operation to perform.', drupal_map_assoc(array('create', 'edit')));
    }
  }

  public function choose_feature($processed = FALSE) {
    $prompt_label = 'Choose the feature to edit';
    if (!$processed) {
      // If creating a new feature, get name from user input and set manually
      // the step results. Ideally there will be a IDCStepPrompt class to return
      // a step object as usual.
      if ($this->getStepResults('choose_operation') == 'create') {
        return new IDCStepPrompt('Enter the feature name');
      }
      // Editing a feature. Select which.
      else {
        $features_list = features_get_features();
        return new IDCStepSingleChoice($prompt_label, drupal_map_assoc(array_keys($features_list)));
      }
    }
  }

  public function choose_component_source($processed = FALSE) {
    $prompt_label = 'Choose the component source';
    if (!$processed) {
      $components = _drush_features_component_list();
      return new IDCStepSingleChoice($prompt_label, drupal_map_assoc(array_keys($components)));
    }
  }

  public function choose_components($processed = FALSE) {
    if (!$processed) {
      $prompt_label = 'Select the components to include in the feature.';
      $source = $this->getStepResults('choose_component_source');
      $components = _drush_features_component_list();

      // Exported components just to be shown when showExportedComponents
      // flag is set to TRUE.
      $options = array(
        'exported' => $this->showExportedComponents,
        'provided by' => TRUE,
      );

      $filtered_components = _drush_features_component_filter($components, array($source), $options);
      $selected_components = $this->getSelectedComponents($source);
      if ($filtered_components) {
        $options = array();
        foreach ($filtered_components['components'] as $source => $components) {
          foreach ($components as $name => $value) {
            $options[$source .':'. $name] = $source .':'. $name;
            if (isset($filtered_components['sources'][$source .':'. $name])) {
              $options[$source .':'. $name] .= "                                                       "
                . dt('Provided by') . ': ' . $filtered_components['sources'][$source .':'. $name];
            }
          }
        }
      }

      $options = drupal_map_assoc($options);
      if ($this->showExportedComponents) {
        $options['switch_exported'] = '--- OTHER OPTIONS: Hide EXPORTED components.';
      }
      else {
        $options['switch_exported'] = '--- OTHER OPTIONS: Show EXPORTED components.';
      }

      return new IDCStepMultipleChoice($prompt_label, $options, $selected_components);
    }

    // Step processed.
    else {
      // If user has chosen to hide / show exported components, switch the flag
      // for it, and repeat step. Don't remove any newly added components, just
      // the value from the hide / show option. Would be ideal to have that
      // handled by default (i.e: extra options used for logic, not included in
      // results.
      $components_selected = $this->getStepResults('choose_components');
      if (in_array('switch_exported', $components_selected)) {
        unset($this->stepResults['choose_components'][array_search('switch_exported', $this->stepResults['choose_components'])]);
        $this->showExportedComponents = !$this->showExportedComponents;
        $this->repeatStep();
      }
      else {
        // Ask the user if he wants to add more components from other sources.
        $results = IDCStepChoiceBase::singleChoice(array('Yes', 'No'), 'Do you want to add more components?');
        if ($results === 0) {
          // User wants to add more components. Jump to source selection.
          $this->jumpToStep('choose_component_source');
        }
      }
    }
  }

  public function finishExecution() {
    $command_results = $this->getStepResults();
    $arguments = array();
    $arguments[] = $command_results['choose_feature'];
    $arguments = array_merge($arguments, $command_results['choose_components']);

    // If creating a new feature, set the destination for the export command.
    if ($command_results['choose_operation'] == 'create') {
      $new_feature_path = new IDCStepPrompt('Destination path (from Drupal root) of the exported feature', 'sites/all/modules/features');
      drush_set_option('destination', $new_feature_path->processStep()->getInputResults());
    }
    drush_invoke('features-export', $arguments);
    $this->log('Finished execution.', 'ok');
  }

  /**
   * Overrides IDCCommandBase::setStepResults().
   *
   * Overrides the base method to keep track of all the features components
   * selected in different iterations of the command.
   *
   * @param string $stepName
   *   The name of the step for which to set the passed results.
   * @param $stepResults
   *   The results to set for the given step.
   * @return $this
   */
  public function setStepResults($stepName, $stepResults) {
    // For the 'choose_components' step, add the results to the existing array,
    // instead of overriding the existing, unless the results are for a feature
    // source for which some data was already entered. In that case, the last
    // options chosen always prevail, so that users can remove components that
    // they added in a previous step.
    if ($stepName == 'choose_components') {
      $chosen_source = $this->getStepResults('choose_component_source');
      if (!empty($this->stepResults[$stepName])) {
        foreach ($this->stepResults[$stepName] as $index => $value) {
          $component_fragments = preg_split('/\:/', $value);
          if ($chosen_source == $component_fragments[0]) {
            unset($this->stepResults[$stepName][$index]);
          }
        }
      }
      $this->stepResults[$stepName] = is_array($this->stepResults[$stepName]) ? array_merge($this->stepResults[$stepName], $stepResults) : $stepResults;
    }
    else {
      parent::setStepResults($stepName, $stepResults);
    }
    return $this;
  }

  public function getSelectedComponents($source) {
    $components = $this->getStepResults('choose_components');
    $components = is_array($components) ? $components : array();
    $selected = array();
    foreach ($components as $value) {
      $component_fragments = preg_split('/\:/', $value);
      if ($source == $component_fragments[0]) {
        $selected[] = $value;
      }
    }
    return $selected;
  }
}
