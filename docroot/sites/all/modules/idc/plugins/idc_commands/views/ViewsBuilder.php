<?php

/**
 * @file
 * IDC Command to create new views for the 'views' module from the command line.
 */

$plugin = array(
  'handler class' => 'ViewsBuilder',
  'dependencies' => 'views',
);

/**
 * Class ViewsBuilder
 */
class ViewsBuilder extends IDCCommandBase implements IDCInterface {

  public static $commandName = 'views-builder';

  public $exportView;

  public $featureForExport;

  /**
   * Returns the array that defines the drush command.
   *
   * @return array
   *   The drush command definition.
   */
  public static function getCommandDefinition() {
    return array(
      'description' => 'Command to build a new view from the command-line',
    );
  }



  /**
   * Returns the array that defines the steps of the drush command.
   *
   * Each step is a string that matches an existing method in the IDC command
   * class.
   *
   * @return array
   *   An indexed array containing the drush command steps.
   */
  public function getCommandSteps() {
    return array(
      'choose_machine_name',
      'choose_title',
      'choose_description',
      'choose_node_types',
      'choose_displays',
      'choose_style',
      'choose_fields',
      'export_view',
    );
  }

  public function choose_machine_name($processed = FALSE) {
    if (!$processed) {
      return new IDCStepPrompt('Enter the machine_name for the view');
    }
  }

  public function choose_title($processed = FALSE) {
    if (!$processed) {
      return new IDCStepPrompt('Enter the title of the view');
    }
  }

  public function choose_description($processed = FALSE) {
    if (!$processed) {
      return new IDCStepPrompt('Enter a description for the view title of the view');
    }
  }

  public function choose_node_types($processed = FALSE) {
    if (!$processed) {
      // Choose the allowed node types.
      $node_types = array();
      foreach (node_type_get_types() as $machine_name => $node_type_info) {
        $node_types[$machine_name] = $node_type_info->name;
      }
      return new IDCStepMultipleChoice("Choose the node types allowed for the view", $node_types);
    }
  }

  public function choose_displays($processed = FALSE) {
    // Get list of display types.
    if (!$processed) {
      ctools_include('plugins', 'views');
      $plugins = views_discover_plugins();

      $display_types = array();
      foreach ($plugins['display'] as $machine_name => $display_plugin) {
        // 'Master' display will be always present.
        if ($machine_name == 'default') {
          continue;
        }
        $display_types[$machine_name] = $display_plugin['title'];
      }
      return new IDCStepMultipleChoice("Choose the 'display' types to add to the view", $display_types);
    }
  }


  public function choose_style($processed = FALSE) {
    if (!$processed) {
      $plugins = views_discover_plugins();

      $available_styles = array();
      foreach ($plugins['style'] as $machine_name => $style_plugin) {
        $available_styles[$machine_name] = $style_plugin['title'];
      }
      return new IDCStepSingleChoice("Choose the 'style' to use for the view", $available_styles);
    }
  }

  /**
   * TODO: Add support for relationships on a step prior to this.
   */
  public function choose_fields($processed = FALSE) {
    if (!$processed) {
      $available_fields = array();
      // Add all views "fields" available for nodes.
      $node_views_elements = views_fetch_data('node');
      foreach ($node_views_elements as $machine_name => $element_info) {
        if (isset($element_info['field'])) {
          $available_fields[$machine_name] = outputColours::getColouredOutput($machine_name, 'cyan');
        }
      }

      // Add fields attached to the selected content types via Field API.
      $node_types = $this->getStepResults('choose_node_types');
      $field_map = field_info_field_map();
      foreach ($field_map as $field_name => $field_info) {
        if (isset($field_info['bundles']['node']) && array_intersect($node_types, $field_info['bundles']['node']))
        $available_fields[$field_name] = outputColours::getColouredOutput($field_name, 'yellow');
      }
      return new IDCStepMultipleChoice("Select the fields to include in the view ", $available_fields);
    }
  }

  public function export_view($processed = FALSE) {
    if (!$processed) {
      $options = array(
        'new_feature' => 'Create new feature',
        'edit_feature' => 'Add to existing feature',
        'no' => 'Do not export',
      );
      return new IDCStepSingleChoice('Do you want to export this view into a feature?', $options);
    }
    // TODO: It'd be ideal to transfer control to existing commands, in order
    // to reuse existing prompting logic.
    elseif ($this->getStepResults('export_view') === 'no') {
      $this->exportView = FALSE;
    }
    elseif ($this->getStepResults('export_view') == 'new_feature') {
      $feature_name = new IDCStepPrompt('Enter the name for the feature in which to export the view');
      $this->exportView = TRUE;
      $this->featureForExport = $feature_name->processStep()->getInputResults();
    }
    elseif ($this->getStepResults('export_view') == 'edit_feature') {
      // Editing a feature. Select which.
      $features_list = features_get_features();
      $feature_name = new IDCStepSingleChoice("Select the feature in which to export the view", drupal_map_assoc(array_keys($features_list)));
      $this->exportView = TRUE;
      $this->featureForExport = $feature_name->processStep()->getInputResults();
    }
  }

  /**
   * Executes any final code that should run after all the command steps.
   */
  public function finishExecution() {
    // Create view object and save it.
    $view = $this->createViewObject();

    // Try to save view and log results to the cli.
    try {
      views_save_view($view);
      if (!empty($view->vid)) {
        $this->printMessage(outputColours::getColouredOutput(shellIcons::$checkmark . " " . $view->name . " saved with vid: " . $view->vid, 'green'));
      }

      // Store in feature if needed.
      if ($this->exportView) {
        // Get exportable name.
        if ($this->getStepResults('export_view') == 'new_feature') {
          drush_set_option('destination', 'sites/all/modules/features');
        }
        drush_invoke('features-export', array($this->featureForExport, 'views_view:' . $view->name));
      }

      // Return URL for the user to edit the view.
      global $base_url;
      $this->printMessage(outputColours::getColouredOutput("View can be edited at: " . $base_url . '/admin/structure/views/view/' . $view->name . '/edit', 'green'));
    }
    catch (Exception $e) {
      $this->log(outputColours::getColouredOutput(shellIcons::$errorX . " " . $view->name . " saved with vid " . $view->vid, 'white', 'red'));
    }
  }

  /**
   * Creates a view object
   *
   * @return \view
   */
  private function createViewObject() {
    $view = new view();
    $view->name = $this->getStepResults('choose_machine_name');
    $view->description = $this->getStepResults('choose_title');
    $view->tag = 'default';
    $view->base_table = 'node';
    $view->human_name = $this->getStepResults('choose_title');
    $view->core = 7;
    $view->api_version = '3.0';
    $view->disabled = FALSE;

    // Master Display
    $handler = $view->new_display('default', 'Master', 'default');
    $handler->display->display_options['title'] = $this->getStepResults('choose_title');
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['pager']['type'] = 'full';
    $handler->display->display_options['pager']['options']['items_per_page'] = '10';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['pager']['options']['id'] = '0';
    $handler->display->display_options['pager']['options']['quantity'] = '9';
    $handler->display->display_options['style_plugin'] = $this->getStepResults('choose_style');

    // Retrieve all the 'fields' info for views, to assemble the exact
    // 'field_name' => 'table' map for the available fields.
    ctools_include('admin', 'views');
    $views_fields = views_fetch_fields('node', 'field');
    $selected_fields = $this->getStepResults('choose_fields');

    // Add each selected field to the view.
    foreach ($views_fields as $base_table_and_field_name => $field_info) {
      list($base_table, $field_name) = explode('.', $base_table_and_field_name);

      if (in_array($field_name, $selected_fields)) {
        // TODO: For entity reference / term reference fields, display them
        // as links to the entity.
        $options = array(
          'label' => FALSE,
          'element_label_colon' => FALSE,
        );
        $view->add_item('default', 'field', $base_table, $field_name, $options);
      }
    }

    // TODO: Add filter for the content types chosen.

    // Add the selected displays to the view.
    foreach ($this->getStepResults('choose_displays') as $display_name) {
      $view->new_display($display_name, $display_name, $display_name);
    }

    return $view;
  }

}
