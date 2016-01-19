<?php
/**
 * @file
 * Defines a custom Views pager plugin.
 *
 * Based on the Pagerer module by mondrake <mondrake@mondrake.org>
 */

/**
 * Pager plugin handler class.
 *
 * Based on Views 'mini pager' handler class. Add 'fragments' options to be
 * passed to url().
 */
class G2MigrateViewsPagerPlugin extends views_plugin_pager_mini {

  /**
   * Return plugin options.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['fragments'] = array(
      'contains' => array(
        'previous' => array('default' => '', 'translatable' => FALSE),
        'current' => array('default' => '', 'translatable' => FALSE),
        'next' => array('default' => '', 'translatable' => FALSE),
      ),
    );
    return $options;
  }

  /**
   * Provide the form for setting options.
   */
  public function options_form(&$form, &$form_state) {
    $form['fragments'] = array (
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#title' => t('URL fragments'),
      '#input' => TRUE,
      '#description' => t('Fragments to be added to pager links.'),
    );

    $form['fragments']['previous'] = array(
      '#type' => 'textfield',
      '#title' => t('Fragment for "previous"-link'),
      '#description' => t('Fragment for "previous"-link'),
      '#default_value' => $this->options['fragments']['previous'],
    );

    $form['fragments']['current'] = array(
      '#type' => 'textfield',
      '#title' => t('Fragment for "current"-link'),
      '#description' => t('Fragment for "current"-link'),
      '#default_value' => $this->options['fragments']['current'],
    );

    $form['fragments']['next'] = array(
      '#type' => 'textfield',
      '#title' => t('Fragment for "next"-link'),
      '#description' => t('Fragment for "next"-link'),
      '#default_value' => $this->options['fragments']['next'],
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render the pager, using theme('g2migrate_photo_gallery_pager', ...).
   *
   * @param array $input
   *   Any extra GET parameters that should be retained, such as exposed
   *   input.
   */
  public function render($input) {
    $output = theme('g2migrate_pager', array(
      'fragments' => $this->options['fragments'],
      'element' => $this->options['id'],
      'parameters' => $input,
    ));
    return $output;
  }

}
