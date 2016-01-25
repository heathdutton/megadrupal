<?php

/**
 * @file
 * Context reaction plugin for DFP Respond.
 */
class TaboolaDFPNativeContextReaction extends context_reaction {
  public $taboola_dfp_native_tags = array();

  // Form options.
  function options_form($context) {
    $form = array();
    $values = array();

    // Get existing values for this form.
    $settings = $this->fetch_from_context($context);
    if (!empty($settings['values'])) {
      $values = $settings['values'];
    }

    // Get a list of all the tags.
    $options = array();
    $tags = dfp_tag_load_all();
    if (empty($tags)) {
      $form['message'] = array(
        '#markup' => t('Before you can use this reaction you need to <a href="/admin/structure/dfp_ads/add">Add a new DFP ad</a>.'),
      );
      return $form;
    }

    foreach ($tags as $tag) {
      $options[$tag->machinename] = $tag->slot;
    }

    $form['values'] = array(
      '#type' => 'fieldset',
    );
    $form['values']['ad_tag'] = array(
      '#type' => 'select',
      '#title' => t('DFP Ad tag'),
      '#description' => t('Select a DFP Ad tag to configure.'),
      '#options' => $options,
      '#default_value' => isset($values['ad_tag']) ? $values['ad_tag'] : array(),
    );
    // This value is used in parent module.
    // @see: taboola_page_alter().
    $form['values']['loader_identifier'] = array(
      '#title' => t('Loader Identifier'),
      '#description' => t('The "Loader Identifier" is provided by Taboola per page or page group.'),
      '#type' => 'textfield',
      '#size' => 30,
      '#default_value' => isset($values['loader_identifier']) ? $values['loader_identifier'] : 'article',
      '#required' => TRUE,
    );
    $form['values']['selector'] = array(
      '#title' => t('Selector'),
      '#description' => t('Enter the CSS selector where you wish to ad unit to render. You can use # for id and . for class.'),
      '#type' => 'textfield',
      '#size' => 60,
      '#required' => TRUE,
      '#default_value' => isset($values['selector']) ? $values['selector'] : '',
    );
    $form['values']['position'] = array(
      '#title' => t('Position'),
      '#description' => t('This determines the position within the selector where you wish to place the ad unit. If you want an ad to appear after the third child add 3 or leave i empty if you want it to be the first element.'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => isset($values['position']) ? $values['position'] : '',
    );

    return $form;
  }

  // Execute context.
  function execute() {
    $i = 0;
    foreach ($this->get_contexts() as $context) {
      if (!isset($context->reactions['taboola_dfp_native'])) {
        continue;
      }

      foreach ($context->reactions['taboola_dfp_native'] as $values) {
        if (!empty($values)) {
          $this->taboola_dfp_native_tags['placement' . ++$i] = $values;
        }
      }
    }
  }

}
