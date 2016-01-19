<?php
/**
 * @file
 * Paginator 3000 - Definition of the Views pager plugin.
 */

/**
 * Paginator 3000 pager plugin handler class.
 */
class Paginator3000ViewsPagerPlugin extends views_plugin_pager_full {

  public function option_definition() {
    $options = parent::option_definition();
    $options['returnorder'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide the form for setting options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['returnorder'] = array(
      '#type' => 'select',
      '#title' => t('Pages order'),
      //'#description' => t(''),
      '#options' => array(
        0 => t('Ascending'),
        1 => t('Descending'),
      ),
      '#default_value' => $this->options['returnorder'],
      '#weight' => 10,
    );
    $form['tags']['#weight'] = 20;
    $form['expose']['#weight'] = 30;
  }

  public function render($input) {
    $pager_theme = views_theme_functions('paginator3000', $this->view, $this->display);
    // The 0, 1, 3, 4 index are correct. See theme_pager documentation.
    $tags = array(
      0 => $this->options['tags']['first'],
      1 => $this->options['tags']['previous'],
      3 => $this->options['tags']['next'],
      4 => $this->options['tags']['last'],
    );
    $output = theme($pager_theme, array(
      'tags' => $tags,
      'element' => $this->options['id'],
      'parameters' => $input,
      'quantity' => $this->options['quantity'],
      'returnorder' => $this->options['returnorder'],
    ));
    return $output;
  }
}
