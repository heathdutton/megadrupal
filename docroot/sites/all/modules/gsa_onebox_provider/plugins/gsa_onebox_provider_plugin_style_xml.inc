<?php
/**
 * @file
 * Plugin include file for GSA Onebox plugin.
 */

/**
 * Generalized style plugin.
 *
 * @ingroup views_style_plugins
 */
class GSAOneboxProviderPluginStyleXml extends GSAOneboxProviderPluginStyle {

  /**
   * Set options fields and default values.
   *
   * @return array
   *   An array of options information.
   */
  public function option_definition() {
    $options['transform'] = array(
      'default' => FALSE,
      'translatable' => FALSE,
    );
    $options['transform_type'] = array(
      'default' => 'dash',
      'translatable' => FALSE,
    );
    $options['datasource'] = array(
      'default' => '',
      'translatable' => FALSE,
    );
    $options['feedtype'] = array(
      'default' => 'meta',
      'translatable' => FALSE,
    );
    return $options;
  }

  /**
   * Options form mini callback.
   *
   * @param object $form
   *   Form array to add additional fields to.
   * @param object $form_state
   *   State of the form.
   */
  public function options_form(&$form, &$form_state) {
    $form['transform'] = array(
      '#type' => 'checkbox',
      '#title' => t('Transform spaces'),
      '#default_value' => $this->options['transform'],
      '#description' => t('Transform spaces to valid XML in field labels (spaces create invalid XML markup). Note that invalid XML tag characters will always be converted.'),
    );
    $form['transform_type'] = array(
      '#type' => 'select',
      '#title' => t('Transform type'),
      '#default_value' => $this->options['transform_type'],
      '#options' => array(
        'dash' => t('Dash'),
        'underline' => t('Underline'),
        'camel' => t('camelCase'),
        'pascal' => t('PascalCase'),
      ),
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array(
        'edit-style-options-transform' => array(TRUE),
      ),
    );

    // TODO: Support content feeds and incremental feeds.
    $form['feedtype'] = array(
      '#type' => 'select',
      '#title' => t('Feed Type'),
      '#default_value' => $this->options['feedtype'],
      '#options' => array(
        'meta' => t('Metadata and Url'),
      ),
    );
    $form['datasource'] = array(
      '#type' => 'textfield',
      '#title' => t('Data Source:'),
      '#default_value' => $this->options['datasource'],
      '#description' => t('This text is used in the feed header.'),
    );
  }
}
