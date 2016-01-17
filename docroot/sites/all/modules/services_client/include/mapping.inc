<?php

interface ServicesClientMappingInterface {

  /**
   * Retrieve condition summary for UI.
   *
   * @return string
   *   UI short summary.
   */
  public function getSummary();

}

interface ServicesClientMappingReaderInterface {

  /**
   * Read data from entity by configuration.
   *
   * @param stdClass $entity
   *   Drupal entity that is processed.
   *
   * @return ServicesClientMappingValue
   *   Value definition.
   */
  public function read($entity);

}

interface ServicesClientMappingFormatterInterface {

  /**
   * Format source value.
   *
   * @param ServicesClientMappingValue $source
   *   Source value gathered by Reader plugin.
   *
   * @return array
   *   Array with formatted value and structure
   *   array('key' => 'name', 'value' => 'value')
   */
  public function format(ServicesclientMappingValue $source);

}

/**
 * Value description that is returned from Reader.
 */
class ServicesClientMappingValue {

  /**
   * Value gathered by reader.
   *
   * @var array
   */
  protected $value;

  /**
   * Flag that holds empty/not empty information.
   *
   * @var bool
   */
  protected $empty;

  /**
   * Constructor.
   *
   * @param array $value
   *   Value that should be carried.
   */
  public function __construct($value = array()) {
    $this->value = $value;
    $this->empty = FALSE;
  }

  /**
   * Set value data.
   *
   * @param array $value
   *   List of values gathered by reader.
   * @return self
   */
  public function setValue($value) {
    $this->value = $value;
    return $this;
  }

  /**
   * Retrieve existing value.
   *
   * @return array
   *   Value.
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Return whether value is empty or not.
   *
   * @return bool
   *   TRUE if empty.
   */
  public function isEmpty() {
    return $this->empty;
  }

  /**
   * Set empty flag of value.
   *
   * @return ServicesClientMappingValue
   */
  public function setEmpty() {
    $this->empty = TRUE;
    return $this;
  }

  /**
   * Remove empty flag of value.
   *
   * @return ServicesClientMappingValue
   */
  public function setNotEmpty() {
    $this->empty = FALSE;
    return $this;
  }
}

/**
 * Mapping plugin wrapper which represents mapping row.
 */
class ServicesClientMappingPlugin extends ServicesClientPlugin {

  protected function getPlugin($name, $config) {
    $reflection = new ReflectionClass($name);
    return $reflection->newInstanceArgs(array($this->event, $config));
  }

  protected function getPluginInstance($type, $name = NULL) {
    $name = !empty($name) ? $name : $this->config[$type];
    if (!empty($name)) {
      $config = isset($this->config[$type . '_config']) ? $this->config[$type . '_config'] : array();
      return $this->getPlugin($name, $config);
    }
    else {
      return NULL;
    }
  }

  public function getReader($name = NULL) {
    return $this->getPluginInstance('reader', $name);
  }

  public function getFormatter($name = NULL) {
    return $this->getPluginInstance('formatter', $name);
  }

  public function configForm(&$form, &$form_state) {
    $form['reader'] = array(
      '#type' => 'select',
      '#title' => t('Reader'),
      '#description' => t('What type of reader should be used for this mapping'),
      '#default_value' => isset($this->config['reader']) ? $this->config['reader'] : NULL,
      '#options' => array('' => '<' . t('select') . '>') + services_client_get_plugins('mapping', TRUE, function ($item) { return $item['type'] == 'reader'; }),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'services_client_plugin_mapping_ajax',
        'wrapper' => 'ajax-wrapper',
      ),
    );

    if (!empty($form_state['values']['reader'])) {
      // AJAX: Plugin was changed, reset configuration
      if (isset($form_state['reader_plugin']) && ($form_state['values']['reader'] != get_class($form_state['reader_plugin']))) {
        $form_state['values']['reader_config'] = array();
      }
      $config = !empty($form_state['values']['reader_config']) ? $form_state['values']['reader_config'] : array();
      $reader = $this->getPlugin($form_state['values']['reader'], $config);
      $reader->configForm($form, $form_state);
      $form_state['reader_plugin'] = $reader;
    }
    elseif (!empty($this->config['reader'])) {
      $config = !empty($this->config['reader_config']) ? $this->config['reader_config'] : array();
      $reader = $this->getPlugin($this->config['reader'], $config);
      $reader->configForm($form, $form_state);
      $form_state['reader_plugin'] = $reader;
    }

    $form['formatter'] = array(
      '#type' => 'select',
      '#title' => t('Formatter'),
      '#description' => t('What type of formatter should be used for this mapping'),
      '#default_value' => isset($this->config['formatter']) ? $this->config['formatter'] : NULL,
      '#options' => array('' => '<' . t('select') . '>') + services_client_get_plugins('mapping', TRUE, function ($item) { return $item['type'] == 'formatter'; }),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'services_client_plugin_mapping_ajax',
        'wrapper' => 'ajax-wrapper',
      ),
    );

    if (!empty($form_state['values']['formatter'])) {
      // AJAX: Plugin was changed, reset configuration
      if (isset($form_state['formatter_plugin']) && ($form_state['values']['formatter'] != get_class($form_state['formatter_plugin']))) {
        $form_state['values']['formatter_config'] = array();
      }
      $config = !empty($form_state['values']['formatter_config']) ? $form_state['values']['formatter_config'] : array();
      $formatter = $this->getPlugin($form_state['values']['formatter'], $config);
      $formatter->configForm($form, $form_state);
      $form_state['formatter_plugin'] = $formatter;
    }
    elseif (!empty($this->config['formatter'])) {
      $config = !empty($this->config['formatter_config']) ? $this->config['formatter_config'] : array();
      $formatter = $this->getPlugin($this->config['formatter'], $config);
      $formatter->configForm($form, $form_state);
      $form_state['formatter_plugin'] = $formatter;
    }

    $wrapped['wrapper'] = array(
      '#tree' => FALSE,
      '#theme_wrappers' => array('container'),
      '#attributes' => array('id' => 'ajax-wrapper'),
    );
    foreach (element_children($form) as $id) {
      $wrapped['wrapper'][$id] = $form[$id];
    }

    $form = $wrapped;
  }

  public function configFormSubmit(&$form, &$form_state) {
    if (isset($form_state['clicked_button']) && $form_state['clicked_button']['#value'] == t('Cancel')) {
      return;
    }

    $this->config['reader'] = $form_state['values']['reader'];
    $this->config['formatter'] = $form_state['values']['formatter'];

    if (!empty($form_state['reader_plugin'])) {
      $form_state['reader_plugin']->configFormSubmit($form, $form_state);
      $this->config['reader_config'] = $form_state['reader_plugin']->getConfiguration();
    }

    if (!empty($form_state['formatter_plugin'])) {
      $form_state['formatter_plugin']->configFormSubmit($form, $form_state);
      $this->config['formatter_config'] = $form_state['formatter_plugin']->getConfiguration();
    }
  }

  public function getSummary() {
    if (isset($this->config['reader']) && isset($this->config['formatter'])) {
      return format_string('!reader ==> !formatter', array(
        '!reader' => $this->getReaderSummary(),
        '!formatter' => $this->getFormatterSummary(),
      ));
    }
    else {
      return '[ ' . t('Not configured') . ' ]';
    }
  }

  /**
   * Get reader summary text.
   *
   * @return string
   *   Reader plugin summary.
   */
  public function getReaderSummary() {
    $reader = $this->getReader();
    return !empty($reader) ? $reader->getSummary() : '<b>NO READER</b>';
  }

  /**
   * Get formatter summary text.
   *
   * @return string
   *   Formatter plugin summary.
   */
  public function getFormatterSummary() {
    $formatter = $this->getFormatter();
    return !empty($formatter) ? $formatter->getSummary() : '<b>NO FORMATTER</b>';
  }
}

abstract class ServicesClientMapperPlugin extends ServicesClientPlugin implements ServicesClientMappingInterface {

  public function __construct($event, $config) {
    parent::__construct($event, $config);
    $this->config += $this->getDefaultConfiguration();
  }

  protected function getDefaultConfiguration() {
    return array();
  }

  public function configForm(&$form, &$form_state) {}

  public function configFormValidate(&$form, &$form_state) {}

  public function configFormSubmit(&$form, &$form_state) {}
}

class ServicesClientPropertyReader extends ServicesClientMapperPlugin implements ServicesClientMappingReaderInterface {

  /**
   * Retrieve default configuration.
   *
   * @return array
   *   Default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'property' => '',
    );
  }

  public function getSummary() {
    if (empty($this->config['property'])) {
      return '[PropertyReader - not configured]';
    }
    else {
      return "\$source-><b>{$this->config['property']}</b>";
    }
  }

  /**
   * Read property data from entity.
   *
   * @param object $entity
   *   Drupal entity object.
   *
   * @return ServicesClientMappingValue
   *   Value definition.
   */
  public function read($entity) {
    $value = new ServicesClientMappingValue();

    if (isset($entity->{$this->config['property']})) {
      if (is_array($entity->{$this->config['property']})) {
        $value->setValue($entity->{$this->config['property']});
      }
      else {
        $value->setValue(array($entity->{$this->config['property']}));
      }
    }
    else {
      $value->setEmpty();
    }

    return $value;
  }

  public function configForm(&$form, &$form_state) {
    $form['reader_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('Property reader'),
      '#tree' => TRUE,
    );
    $form['reader_config']['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#description' => t('Enter property name'),
      '#default_value' => $this->config['property']
    );
  }

  public function configFormSubmit(&$form, &$form_state) {
    $this->config['property'] = $form_state['values']['reader_config']['property'];
  }
}

/**
 * Reads data from drupal field.
 */
class ServicesClientFieldReader extends ServicesClientMapperPlugin implements ServicesClientMappingReaderInterface {

  /**
   * Retrieve default configuration.
   *
   * @return array
   *   Default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'field' => '',
      'language' => LANGUAGE_NONE,
      'property' => '',
      'all_values' => TRUE,
    );
  }

  /**
   * Get plugin configuration summary.
   *
   * @return string
   *   Configuration overview string.
   */
  public function getSummary() {
    if (empty($this->config['field'])) {
      return '[FieldReader - not configured]';
    }
    else {
      $delta = !empty($this->config['all_values']) ? '*' : '0';
      return "\$source-><b>{$this->config['field']}[{$this->config['language']}][{$delta}][{$this->config['property']}]</b>";
    }
  }

  /**
   * Read property from entity.
   *
   * @param object $entity
   *   Entity that is being read.
   *
   * @return array
   */
  public function read($entity) {

    $value = new ServicesClientMappingValue();

    $items = isset($entity->{$this->config['field']}[$this->config['language']]) ? $entity->{$this->config['field']}[$this->config['language']] : NULL;
    if (!empty($items)) {
      // We read only single value
      if ($this->config['all_values'] == FALSE) {
        $items = array(reset($items));
      }
      $out = array();
      foreach ($items as $item) {
        if (isset($item[$this->config['property']])) {
          $out[] = $item[$this->config['property']];
        }
      }
      if (!empty($out)) {
        $value->setValue($out);
      }
      else {
        $value->setEmpty();
      }
    }
    else {
      $value->setEmpty();
    }

    return $value;
  }

  /**
   * Configuration form.
   */
  public function configForm(&$form, &$form_state) {
    $form['reader_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('Property reader'),
      '#tree' => TRUE,
    );
    $form['reader_config']['field'] = array(
      '#type' => 'textfield',
      '#title' => t('Field name'),
      '#description' => t('Enter the name of the field'),
      '#default_value' => $this->config['field']
    );
    $form['reader_config']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Language'),
      '#description' => t('Enter property name'),
      '#default_value' => $this->config['language']
    );
    $form['reader_config']['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#description' => t('Enter property name'),
      '#default_value' => $this->config['property']
    );
    $form['reader_config']['all_values'] = array(
      '#type' => 'checkbox',
      '#tilte' => t('Read all values'),
      '#description' => t('Leave checked if this should read all values from field.'),
      '#default_value' => $this->config['all_values']
    );
  }

  /**
   * Submit handler;
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['field'] = $form_state['values']['reader_config']['field'];
    $this->config['language'] = $form_state['values']['reader_config']['language'];
    $this->config['property'] = $form_state['values']['reader_config']['property'];
    $this->config['all_values'] = $form_state['values']['reader_config']['all_values'];
  }
}

/**
 * Formats data to property.
 */
class ServicesClientPropertyFormatter extends ServicesClientMapperPlugin implements ServicesClientMappingFormatterInterface {

  /**
   * Retrieve default configuration.
   *
   * @return array
   *   Default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'property' => '',
      'multivalue' => 'force_single',
      'empty' => 'no_field',
      'default_value' => '',
    );
  }

  /**
   * Get plugin configuration summary.
   *
   * @return string
   *   Configuration overview string.
   */
  public function getSummary() {
    if (empty($this->config['property'])) {
      return '[PropertyFormatter - not configured]';
    }
    else {
      return "\$object-><b>{$this->config['property']}</b>";
    }
  }

  /**
   * Format source value to destination.
   *
   * @param ServicesClientMappingValue $source
   *   Source value gathered from Reader.
   *
   * @return array
   *   Value specified by 'key' => 'name', 'value' => 'mixed'
   */
  public function format(ServicesClientMappingValue $source) {
    // Handle empty values
    if ($source->isEmpty()) {
      if ($this->config['empty'] == 'no_field') {
        return NULL;
      }
      elseif ($this->config['empty'] == 'null_field') {
        return array(
          'key' => $this->config['property'],
          'value' => NULL,
        );
      }
      elseif ($this->config['empty'] == 'default_value') {
        return array(
          'key' => $this->config['property'],
          'value' => $this->config['default_value'],
        );
      }
    }

    // If single value is forced.
    $value = $source->getValue();
    if ($this->config['multivalue'] == 'force_single') {
      $value = reset($value);
    }

    // Return resulting value.
    return array(
      'key' => $this->config['property'],
      'value' => $value
    );
  }

  public function configForm(&$form, &$form_state) {
    $form['formatter_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('Property formatter'),
      '#tree' => TRUE,
    );
    $form['formatter_config']['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#description' => t('Enter property name'),
      '#default_value' => $this->config['property']
    );
    $form['formatter_config']['multivalue'] = array(
      '#type' => 'select',
      '#title' => t('Multiple values'),
      '#description' => t('How to handle multiple values'),
      '#options' => array(
        'force_single' => t('Always force single simple value of property'),
        'force_array' => t('Always send array even with single value')
      ),
      '#default_value' => $this->config['multivalue']
    );
    $form['formatter_config']['empty'] = array(
      '#type' => 'select',
      '#title' => t('Empty values'),
      '#description' => t('How to handle empty values'),
      '#options' => array(
        'no_field' => t("Don't create any field"),
        'null_field' => t("Create null field."),
        'default_value' => t('Provide default value'),
      ),
      '#default_value' => $this->config['empty']
    );
    $form['formatter_config']['default_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Default value'),
      '#description' => t('Provide default value if reader will provide empty value.'),
      '#default_value' => $this->config['default_value'],
      '#states' => array(
        'visible' => array(
          ':input[name="formatter_config[empty]"]' => array(
            array('value' => 'default_value'),
          ),
        ),
      ),
    );
  }

  public function configFormSubmit(&$form, &$form_state) {
    $this->config['property'] = $form_state['values']['formatter_config']['property'];
    $this->config['multivalue'] = $form_state['values']['formatter_config']['multivalue'];
    $this->config['empty'] = $form_state['values']['formatter_config']['empty'];
    $this->config['default_value'] = $form_state['values']['formatter_config']['default_value'];
  }
}

class ServicesClientFieldFormatter extends ServicesClientMapperPlugin implements ServicesClientMappingFormatterInterface {

  /**
   * Retrieve default configuration.
   *
   * @return array
   *   Default configuration.
   */
  protected function getDefaultConfiguration() {
    return array(
      'field' => '',
      'language' => LANGUAGE_NONE,
      'property' => '',
      'multivalue' => 'all_values',
      'empty' => 'no_field',
      'default_value' => '',
    );
  }

  /**
   * Get pluginc configuration summary.
   *
   * @return string
   *   Configuration overview string.
   */
  public function getSummary() {
    if (empty($this->config['field'])) {
      return '[FieldFormatter - not configured]';
    }
    else {
      $delta = $this->config['multivalue'] == 'force_single' ? '0' : '*';
      return "\$object-><b>{$this->config['field']}[{$this->config['language']}][{$delta}][{$this->config['property']}]</b>";
    }
  }

  /**
   * Retrieve empty value formatting by configuration.
   *
   * @return array|NULL
   *   Formatted empty value.
   */
  protected function formatEmptyValue() {
    if ($this->config['empty'] == 'no_field') {
      return NULL;
    }
    elseif ($this->config['empty'] == 'null_field') {
      return array(
        'key' => $this->config['field'],
        'value' => array($this->config['language'] => array()),
      );
    }
    elseif ($this->config['empty'] == 'null_property') {
      return array(
        'key' => $this->config['field'],
        'value' => NULL,
      );
    }
    elseif ($this->config['empty'] == 'default_value') {
      return array(
        'key' => $this->config['field'],
        'value' => array($this->config['language'] => array(array($this->config['property'] => $this->config['default_value']))),
      );
    }
  }

  /**
   * Format source value array to field.
   *
   * @param array $values
   *   Source values that should be formatted.
   *
   * @return array
   *   Formatted value in format
   */
  protected function formatArray($values) {
    // Make handy shortcuts
    $language = $this->config['language'];
    $field = $this->config['field'];
    $property = $this->config['property'];

    // Build final value
    $out = array();
    foreach ($values as $value) {
      $out[$language][][$property] = $value;
    }

    // Return resulting value.
    return array(
      'key' => $this->config['field'],
      'value' => $out,
    );
  }

  /**
   * Format source value to destination.
   *
   * @param ServicesClientMappingValue $source
   *   Source value gathered from Reader.
   *
   * @return array
   *   Value specified by 'key' => 'name', 'value' => 'mixed'
   */
  public function format(ServicesClientMappingValue $source) {
    // Handle empty values
    if ($source->isEmpty()) {
      return $this->formatEmptyValue();
    }

    // If single value is forced.
    $values = $source->getValue();
    if ($this->config['multivalue'] == 'force_single') {
      $values = array(reset($values));
    }

    return $this->formatArray($values);
  }

  /**
   * Config form.
   */
  public function configForm(&$form, &$form_state) {
    $form['formatter_config'] = array(
      '#type' => 'fieldset',
      '#title' => t('Property reader'),
      '#tree' => TRUE,
    );
    $form['formatter_config']['field'] = array(
      '#type' => 'textfield',
      '#title' => t('Field name'),
      '#description' => t('Enter field name'),
      '#default_value' => $this->config['field']
    );
    $form['formatter_config']['language'] = array(
      '#type' => 'textfield',
      '#title' => t('Language'),
      '#description' => t('Enter field name'),
      '#default_value' => $this->config['language']
    );
    $form['formatter_config']['property'] = array(
      '#type' => 'textfield',
      '#title' => t('Property name'),
      '#description' => t('Enter property name'),
      '#default_value' => $this->config['property']
    );
    $form['formatter_config']['multivalue'] = array(
      '#type' => 'select',
      '#title' => t('Multiple values'),
      '#description' => t('How to handle multiple values'),
      '#options' => array(
        'force_single' => t('Always force single value'),
        'all_values' => t('Send all values available in field')
      ),
      '#default_value' => $this->config['multivalue']
    );
    $form['formatter_config']['empty'] = array(
      '#type' => 'select',
      '#title' => t('Empty values'),
      '#description' => t('How to handle empty values'),
      '#options' => array(
        'no_field' => t("Don't create any field"),
        'null_field' => t("Create null field (deletes remote value)."),
        'default_value' => t('Provide default value'),
      ),
      '#default_value' => $this->config['empty']
    );
    $form['formatter_config']['default_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Default value'),
      '#description' => t('Provide default value if reader will provide empty value.'),
      '#default_value' => $this->config['default_value'],
      '#states' => array(
        'visible' => array(
          ':input[name="formatter_config[empty]"]' => array(
            array('value' => 'default_value'),
          ),
        ),
      ),
    );
  }

  /**
   * Submit configuration form.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['field'] = $form_state['values']['formatter_config']['field'];
    $this->config['language'] = $form_state['values']['formatter_config']['language'];
    $this->config['property'] = $form_state['values']['formatter_config']['property'];
    $this->config['multivalue'] = $form_state['values']['formatter_config']['multivalue'];
    $this->config['empty'] = $form_state['values']['formatter_config']['empty'];
    $this->config['default_value'] = $form_state['values']['formatter_config']['default_value'];
  }
}

/**
 * D6 and D5 field formatter.
 */
class ServicesClientFieldD6Formatter extends ServicesClientFieldFormatter {

  /**
   * Get plugin configuration summary.
   *
   * @return string
   *   Configuration overview string.
   */
  public function getSummary() {
    if (empty($this->config['field'])) {
      return '[FieldD6Formatter - not configured]';
    }
    else {
      $multivalue = $this->config['multivalue'] == 'force_single' ? '0' : '*';
      return "\$object-><b>{$this->config['field']}[{$multivalue}][{$this->config['property']}]</b>";
    }
  }

  /**
   * Retrieve empty value formatting by configuration.
   *
   * @return array|NULL
   *   Formatted empty value.
   */
  protected function formatEmptyValue() {
    if ($this->config['empty'] == 'no_field') {
      return NULL;
    }
    elseif ($this->config['empty'] == 'null_field') {
      return array(
        'key' => $this->config['field'],
        'value' => array(),
      );
    }
    elseif ($this->config['empty'] == 'null_property') {
      return array(
        'key' => $this->config['field'],
        'value' => NULL,
      );
    }
    elseif ($this->config['empty'] == 'default_value') {
      return array(
        'key' => $this->config['field'],
        'value' => array(array($this->config['property'] => $this->config['default_value'])),
      );
    }
  }

  /**
   * Format source value array to field.
   *
   * @param array $values
   *   Source values that should be formatted.
   *
   * @return array
   *   Formatted value in format
   */
  protected function formatArray($values) {
    // Make handy shortcuts
    $field = $this->config['field'];
    $property = $this->config['property'];

    // Build final value
    $out = array();
    foreach ($values as $value) {
      $out[][$property] = $value;
    }

    // Return resulting value.
    return array(
      'key' => $this->config['field'],
      'value' => $out,
    );
  }

  /**
   * Config form.
   */
  public function configForm(&$form, &$form_state) {
    parent::configForm($form, $form_state);
    unset($form['formatter_config']['language']);
  }

  /**
   * Submit config form.
   */
  public function configFormSubmit(&$form, &$form_state) {
    $this->config['field'] = $form_state['values']['formatter_config']['field'];
    $this->config['property'] = $form_state['values']['formatter_config']['property'];
    $this->config['multivalue'] = $form_state['values']['formatter_config']['multivalue'];
    $this->config['empty'] = $form_state['values']['formatter_config']['empty'];
    $this->config['default_value'] = $form_state['values']['formatter_config']['default_value'];
  }
}
