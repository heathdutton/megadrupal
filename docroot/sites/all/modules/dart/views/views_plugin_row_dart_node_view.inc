<?php
/**
 * @file
 * Contains the node view row style plugin.
 */

module_load_include('inc', 'views', 'modules/node.views');
module_load_include('inc', 'views', 'modules/node/views_plugin_row_node_view');

/**
 * Plugin which inserts ads into a node view.
 */
class views_plugin_row_dart_node_view extends views_plugin_row_node_view {

  /**
   * Define the available options in the options form.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['ad_position'] = array('default' => array());
    $options['ad_position_type'] = array('default' => 'fixed');
    $options['ad_position_interval'] = array('default' => 0);

    return $options;
  }

  /**
   * Construct the options form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    if (!libraries_get_path('postscribe') && !libraries_get_path('writecapture')) {
      drupal_set_message(t('The postscribe and writeCapture jQuery library were not detected.  If one of them is not included on the page, your site is liable to break while using this feature.'), 'error', FALSE);
    }

    $default_values = $this->options['ad_position'];

    $ad_position_options = array(0 => t('<None>'));
    for ($i = 1; $i < 16; $i++) {
      $ad_position_options[$i] = $i;
    }

    $ad_source_options = array('' => t('<None>'));
    foreach (module_invoke('dart', 'block_info') as $block_id => $block_data) {
      $ad_source_options[$block_id] = $block_data['info'];
    }

    $form['ad_position_type'] = array(
      '#title' => t('Type of DART ad placement.'),
      '#type' => 'select',
      '#options' => array(
        'fixed' => t('Fixed'),
        'interval' => t('Interval - every nth row'),
      ),
      '#default_value' => $this->options['ad_position_type'],
      '#required' => TRUE,
    );

    $form['ad_position_interval'] = array(
      '#title' => t('Select ad position interval.'),
      '#type' => 'select',
      '#options' => $ad_position_options,
      '#default_value' => $this->options['ad_position_interval'],
      '#states' => array(
        'visible' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'interval'),
        ),
        'required' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'interval'),
        ),
      ),
      '#description' => t('e.g. select 3 to display an ad in every 3rd row.'),
    );
    $form['ad_position_interval_source'] = array(
      '#title' => t('Select ad position interval source.'),
      '#type' => 'select',
      '#options' => $ad_source_options,
      '#default_value' => $this->options['ad_position_interval_source'],
      '#states' => array(
        'visible' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'interval'),
        ),
        'required' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'interval'),
        ),
      ),
    );

    $form['ad_position'] = array(
      '#title' => t('DART Ad Placement'),
      '#description' => t('The following settings will apply for each page of results.'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#states' => array(
        'visible' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'fixed'),
        ),
      ),
    );

    $form['ad_position'][0] = array(
      '#title' => t('Ad 1'),
      '#type' => 'fieldset',
    );
    $form['ad_position'][0]['position'] = array(
      '#title' => t('Position'),
      '#type' => 'select',
      '#options' => $ad_position_options,
      '#default_value' => $default_values[0]['position'],
      '#states' => array(
        'required' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'fixed'),
        ),
      ),
    );
    $form['ad_position'][0]['source'] = array(
      '#title' => t('Source'),
      '#type' => 'select',
      '#options' => $ad_source_options,
      '#default_value' => $default_values[0]['source'],
      '#states' => array(
        'required' => array(
          ':input[name="row_options[ad_position_type]"]' => array('value' => 'fixed'),
        ),
      ),
    );

    $form['ad_position'][1] = array(
      '#title' => t('Ad 2'),
      '#type' => 'fieldset',
    );
    $form['ad_position'][1]['position'] = array(
      '#title' => t('Position'),
      '#type' => 'select',
      '#options' => $ad_position_options,
      '#default_value' => $default_values[1]['position']
    );
    $form['ad_position'][1]['source'] = array(
      '#title' => t('Source'),
      '#type' => 'select',
      '#options' => $ad_source_options,
      '#default_value' => $default_values[1]['source']
    );

    $form['ad_position'][2] = array(
      '#title' => t('Ad 3'),
      '#type' => 'fieldset',
    );
    $form['ad_position'][2]['position'] = array(
      '#title' => t('Position'),
      '#type' => 'select',
      '#options' => $ad_position_options,
      '#default_value' => $default_values[2]['position']
    );
    $form['ad_position'][2]['source'] = array(
      '#title' => t('Source'),
      '#type' => 'select',
      '#options' => $ad_source_options,
      '#default_value' => $default_values[2]['source']
    );
  }

  /**
   * Called before the view row is rendered.
   */
  function pre_render($values) {
    parent::pre_render($values);
  }

  /**
   * Render the view row and return the HTML output.
   */
  function render($row) {

    // If this is a row with node data, simply return the rendered node.
    if (isset($row->nid) && is_numeric($row->nid)) {
      return parent::render($row);
    }
    // If this row contains ad data, return the rendered ad.
    else if (isset($row->is_dart_ad) && $row->is_dart_ad &&
        isset($row->source) && $row->source) {
      return _dart_ads_tag($row->source);
    }

    // Something's wrong if we get here.  Return an empty string.
    return '';
  }
}
