<?php
/**
 * @file
 * Contains the Settings handler.
 */

/**
 * Class Qui_Settings_Handler
 */
class Qui_Settings_Handler extends Qui_Query_Builder_Handler {

  public $enabled = TRUE;
  public $name = 'settings';

  /**
   * Sets the report_name_state for other elements to use.
   */
  public function __construct($controller) {
    parent::__construct($controller);
    $this->report_name_state = array('visible' => array(':input[name=settings[name]]' => array('filled' => TRUE)));
    $this->defaults = new stdClass();
    $this->defaults->settings = new stdClass();
  }

  /**
   * Creates settings elements.
   */
  public function build() {

    $this->elements[$this->name] = array(
      '#title' => t(ucfirst($this->name)),
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => !empty($this->getHandler('qui_select_table')->defaults),
      '#tree' => TRUE,
    );

    $settings = &$this->elements[$this->name];

    $settings['name'] = array(
      '#title' => t('Report Name'),
      '#type' => 'textfield',
      '#required' => TRUE,
      '#default_value' => $this->defaults->name,
    );

    $settings['machine_name'] = array(
      '#title' => t('Machine Name'),
      '#type' => 'machine_name',
      '#required' => TRUE,
      '#default_value' => $this->defaults->machine_name,
      '#machine_name' => array(
        'source' => array($this->name, 'name'),
        'label' => t('Machine Name'),
        'exists' => 'qui_report_load_by_machine_name',
      ),
    );

    $settings['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#rows' => 2,
      '#description' => t('Detailed description of what the report shows.'),
      '#default_value' => $this->defaults->description,
      '#states' => $this->report_name_state,
    );

    $settings['advanced'] = array(
      '#type' => 'fieldset',
      '#title' => t('Advanced'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#states' => $this->report_name_state,
    );

    $advanced = &$settings['advanced'];

    $advanced['cache_time'] = array(
      '#title' => t('Cache time'),
      '#type' => 'textfield',
      '#description' => t('Number of seconds to cache query results.<br />Cache can be cleared by passing a ?clear-cache argument.'),
      '#default_value' => $this->defaults->settings->cache_time,
    );

    $token = $this->defaults->settings->access_token;
    if ($this->reportValue('id')) {
      global $base_url;
      $token_link = l("{$base_url}/qui/reports/view/{$this->reportValue('id')}/json?access_token={$token}", "qui/reports/view/{$this->reportValue('id')}/json", array(
        'absolute' => TRUE,
        'query' => array('access_token' => $token),
      ));
    }
    else {
      $token_link = "/qui/reports/view/REPORT_ID/json?access_token={$token}";
    }

    $advanced['access_token'] = array(
      '#title' => t('Access Token'),
      '#type' => 'textfield',
      '#description' => t('Secure token used for accessing the report for anonymous users.<br />Token is passed with ?token=TOKEN in the URL. Example: !token_link <br />Replace "json" with any other formatter you\'d like. Use "view as" on the reports page to see more formats.<br />Recommended: Use HTTPS to ensure the TOKEN is not visible over the network.', array('!token_link' => $token_link)),
      '#default_value' => $this->defaults->settings->access_token,
    );

    $advanced['tags'] = array(
      '#title' => t('Tags'),
      '#type' => 'textfield',
      '#description' => t('Comma separated list of tags. Tagging the report will make it easier to search for.'),
      '#default_value' => implode(', ', $this->defaults->tags),
    );

    $advanced['allowed_roles_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Allowed Roles'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    $advanced['allowed_roles_fieldset']['allowed_roles'] = array(
      '#type' => 'checkboxes',
      '#options' => user_roles(),
      '#description' => t('User roles that are allowed to access this report.'),
      '#default_value' => $this->defaults->settings->allowed_roles,
    );

    $formats = qui_get_formatters();
    if (count($formats) > 0) {
      $settings['allowed_formats'] = array(
        '#type' => 'fieldset',
        '#title' => t('Allowed Formats'),
        '#attributes' => array('class' => array('inline-items')),
        '#tree' => TRUE,
        '#states' => $this->report_name_state,
      );
      foreach ($formats as $format => $info) {
        $settings['allowed_formats'][$format] = array(
          '#type' => 'checkbox',
          '#title' => $info['name'],
          '#attributes' => array('title' => $info['description']),
          '#default_value' => in_array($format, $this->defaults->settings->allowed_formats),
        );
      }
    }
  }

  /**
   * Sets the default values for all settings.
   */
  public function setDefaults() {
    $this->defaults->name = $this->formValue('name', 'settings') ?: $this->reportValue('name') ?: NULL;
    if (!$this->formValue('name', 'settings') && $this->action == 'clone') {
      $this->defaults->name .= ' CLONED';
    }
    $this->defaults->machine_name = $this->formValue('machine_name', 'settings') ?: $this->reportValue('machine_name') ?: NULL;
    $this->defaults->description = $this->formValue('description', 'settings') ?: $this->reportValue('description') ?: NULL;
    $tags = array();
    if ($tags = $this->formValue('tags', 'settings', 'advanced')) {
      $tags = $this->formatTags($tags) ?: array();
    }
    else {
      $tags = $this->reportValue('tags') ?: array();
    }
    $this->defaults->tags = $tags;
    $this->defaults->settings->access_token = $this->formValue('access_token', 'settings', 'advanced') ?: ($this->reportValue('access_token', 'settings') ?: user_password(32));
    $this->defaults->settings->cache_time = $this->formValue('cache_time', 'settings', 'advanced') ?: $this->reportValue('cache_time', 'settings') ?: NULL;
    $this->defaults->settings->allowed_roles = array_filter($this->formValue('allowed_roles', 'settings', 'advanced', 'allowed_roles_fieldset') ?: $this->reportValue('allowed_roles', 'settings') ?: array());
    $this->defaults->settings->allowed_formats = $this->formValue('allowed_formats', 'settings') ? array_keys(array_filter($this->formValue('allowed_formats', 'settings'))) : ($this->reportValue('allowed_formats', 'settings') ?: array());
  }

  /**
   * Validates settings values.
   */
  public function validate() {
    if ($this->action !== 'edit' && qui_report_load_by_machine_name($this->defaults->machine_name)) {
      form_set_error('settings][machine_name', t('The machine-readable name is already in use. It must be unique.'));
    }
  }

  /**
   * Formats a tags string into an array.
   */
  private function formatTags($tags) {
    return array_map('trim', explode(',', $tags));
  }

  /**
   * Saves settings values.
   */
  public function submit() {
    $this->report->name = $this->defaults->name;
    $this->report->machine_name = $this->defaults->machine_name;
    $this->report->tags = $this->defaults->tags;
    $this->report->description = $this->defaults->description;
    $this->report->settings = $this->defaults->settings;
  }

}
