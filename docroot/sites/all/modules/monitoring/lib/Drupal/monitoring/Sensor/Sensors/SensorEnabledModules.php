<?php
/**
 * @file
 * Contains \Drupal\monitoring\Sensor\Sensors\SensorEnabledModules.
 */

namespace Drupal\monitoring\Sensor\Sensors;

use Drupal\monitoring\Result\SensorResultInterface;
use Drupal\monitoring\Sensor\SensorConfigurable;

/**
 * Monitors installed modules.
 */
class SensorEnabledModules extends SensorConfigurable {


  /**
   * {@inheritdoc}
   */
  public function settingsForm($form, &$form_state) {
    $form = parent::settingsForm($form, $form_state);

    module_load_include('inc', 'system', 'system.admin');

    $form['allow_additional'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow additional modules to be enabled'),
      '#description' => t('If checked the additional modules being enabled will not be considered as an error state.'),
      '#default_value' => $this->info->getSetting('allow_additional'),
    );

    // Get current list of modules.
    $modules = system_rebuild_module_data();
    $visible_modules = array();
    $hidden_modules = array();

    foreach ($modules as $module => $module_data) {
      // Skip profiles.
      if (strpos(drupal_get_path('module', $module), 'profiles') === 0) {
        continue;
      }
      // As we also include hidden modules, some might have no name at all,
      // make sure it is set.
      if (!isset($module_data->info['name'])) {
        $module_data->info['name'] = '- No name -';
      }
      if (!empty($module_data->info['hidden'])) {
        $module_data->info['name'] .= ' [' . t('Hidden module') .']';
        $hidden_modules[$module] = $module_data;
      }
      else {
        $visible_modules[$module] = $module_data;
      }
    }

    uasort($visible_modules, 'system_sort_modules_by_info_name');
    uasort($hidden_modules, 'system_sort_modules_by_info_name');

    $default_value = $this->info->getSetting('modules');

    if (empty($default_value)) {
      $default_value = module_list();
    }

    $options = array();
    foreach (array_merge($visible_modules, $hidden_modules) as $module => $module_data) {
      $options[$module] = $module_data->info['name'] . ' (' . $module . ')';
    }

    $form['modules'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t('Modules expected to be enabled'),
      '#description' => t('Check all modules that are supposed to be enabled.'),
      '#default_value' => $default_value,
    );

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function runSensor(SensorResultInterface $result) {
    // Load the info from the system table to display the label.
    $module_data = db_select('system')
      ->fields('system')
      ->execute()
      ->fetchAllAssoc('name');
    $result->setExpectedValue(0);
    $delta = 0;

    $monitoring_enabled_modules = array();
    // Filter out install profile.
    foreach (module_list() as $module) {
      $path_parts = explode('/', drupal_get_path('module', $module));
      if ($path_parts[0] != 'profiles') {
        $monitoring_enabled_modules[$module] = $module;
      }
    }

    $expected_modules = array_filter($this->info->getSetting('modules'));

    // If there are no expected modules, the sensor is not configured, so init
    // the expected modules list as currently enabled modules.
    if (empty($expected_modules)) {
      $expected_modules = $monitoring_enabled_modules;
      $settings = monitoring_sensor_settings_get('monitoring_enabled_modules');
      $settings['modules'] = $monitoring_enabled_modules;
      monitoring_sensor_settings_save('monitoring_enabled_modules', $settings);
    }

    // Check for modules not being installed but expected.
    $non_installed_modules = array_diff($expected_modules, $monitoring_enabled_modules);
    if (!empty($non_installed_modules)) {
      $delta += count($non_installed_modules);
      $non_installed_modules_info = array();
      foreach ($non_installed_modules as $non_installed_module) {
        $info = unserialize($module_data[$non_installed_module]->info);
        $non_installed_modules_info[] = $info['name'] . ' (' . $non_installed_module . ')';
      }
      $result->addStatusMessage('Following modules are expected to be installed: @modules', array('@modules' => implode(', ', $non_installed_modules_info)));
    }

    // In case we do not allow additional modules check for modules installed
    // but not expected.
    $unexpected_modules = array_diff($monitoring_enabled_modules, $expected_modules);
    if (!$this->info->getSetting('allow_additional') && !empty($unexpected_modules)) {
      $delta += count($unexpected_modules);
      $unexpected_modules_info = array();
      foreach($unexpected_modules as $unexpected_module) {
        $info = unserialize($module_data[$unexpected_module]->info);
        $unexpected_modules_info[] = $info['name'] . ' (' . $unexpected_module . ')';
      }
      $result->addStatusMessage('Following modules are NOT expected to be installed: @modules', array('@modules' => implode(', ', $unexpected_modules_info)));
    }

    $result->setValue($delta);
  }
}
