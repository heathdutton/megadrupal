<?php
/**
 * @file
 * Custom display extender plugin for Views.
 */

/**
 * The plugin that added additional setting to views edit form.
 *
 * @ingroup views_display_plugins
 */
class views_linker_plugin_display_extender_back extends views_plugin_display_extender {
  /**
   * Provide a form to edit options for this plugin.
   */
  function options_definition_alter(&$options) {

    $options['views_linker_enabled'] = array(
      'default' => 0,
      'bool' => TRUE,
    );

    $options['search_label'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    $options['direct_label'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    $options['back_path'] = array(
      'default' => '',
    );

    $options['views_linker_block_delta'] = array(
      'default' => '',
    );

  }

  function options_summary(&$categories, &$options) {

    $enabled = $this->display->get_option('views_linker_enabled');

    $options['views_linker'] = array(
      'category' => 'other',
      'title' => t('Views linker'),
      'value' => empty($enabled) ? t('Unset') : t('Enabled'),
    );

  }

  function options_form(&$form, &$form_state) {

    parent::options_form($form, $form_state);

    if ($form_state['section'] == 'views_linker') {

      $form['#title'] .= t('Back to results options');
      $enabled = $this->display->get_option('views_linker_enabled');
      $search_label = $this->display->get_option('search_label');
      $direct_label = $this->display->get_option('direct_label');
      $back_path = $this->display->get_option('back_path');

      $form['enabled'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Enable'),
        '#description'   => t('Enable the back to results option for this view display.'),
        '#default_value' => empty($enabled) ? FALSE : $enabled,
      );

      $form['back_path'] = array(
        '#type'          => 'textfield',
        '#required'      => TRUE,
        '#title'         => t('Link Path'),
        '#description'   => t('The path to go back to the search listing, e.g. widgets/directory.'),
        '#default_value' => $back_path,
      );

      $form['search'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Reached As Search Result'),
        '#description' => t('The page has been reached via a search. There is a pre-filtered search to go back to.'),
      );

      $form['search']['search_label'] = array(
        '#type'          => 'textfield',
        '#required'      => TRUE,
        '#title'         => t('Link label'),
        '#description'   => t('Text to display on the link back to results.'),
        '#default_value' => empty($search_label) ? t('Back to results') : $search_label,
      );

      $form['direct'] = array(
        '#type'        => 'fieldset',
        '#title'       => t('Reached Directly'),
        '#description' => t('The page has been reached directly, not via a search. There is no pre-filtered search to go back to, just a page listing all items and options to filter and search.'),
      );

      $form['direct']['direct_label'] = array(
        '#type'          => 'textfield',
        '#required'      => FALSE,
        '#title'         => t('Link label'),
        '#description'   => t('Text to display on the link to a search page. Leave blank to not display a link.'),
        '#default_value' => $direct_label,
      );

      $form['views_linker']['#type'] = 'container';
    }

  }

  function options_submit(&$form, &$form_state) {

    if ($form_state['section'] == 'views_linker') {
      $view_name = $this->view->name;
      $display = $this->view->current_display;
      $settings = $form_state['values'];
      $block_delta = $this->display->get_option('views_linker_block_delta');

      if (!empty($settings['enabled']) && empty($block_delta)) {
        $block_delta = md5($view_name . '::' . $display);
      }

      $this->display->set_option('views_linker_enabled', $settings['enabled']);
      $this->display->set_option('search_label', $settings['search_label']);
      $this->display->set_option('direct_label', $settings['direct_label']);
      $this->display->set_option('back_path', $settings['back_path']);
      $this->display->set_option('views_linker_block_delta', $block_delta);
    }
  }

}
