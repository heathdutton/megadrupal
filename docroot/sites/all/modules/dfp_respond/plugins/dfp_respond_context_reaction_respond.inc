<?php
/**
 * @file
 * Context reaction plugin for DFP Respond.
 */

class dfp_respond_context_reaction_respond extends context_reaction {
  public $dfp_respond_tags = array();

  // Form options.
  function options_form($context) {
    $form = array();
    $values = array();

    // Get existing values for this form.
    $settings = $this->fetch_from_context($context);
    if (!empty($settings['values'])) {
      $values = $settings['values'];
    }

    // Get a list of all DART tags.
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
    $form['values']['container'] = array(
      '#title' => t('Parent Container'),
      '#description' => t('Enter the container name where you wish to ad unit to render. You can use # for id and . for class.'),
      '#type' => 'textfield',
      '#size' => 60,
      '#default_value' => isset($values['container']) ? $values['container'] : '',
    );
    $form['values']['children'] = array(
      '#title' => t('Children'),
      '#description' => t('(Optional) Enter the type of children that exist in the Container. If paragraphs enter p if they are list items ad li.'),
      '#type' => 'textfield',
      '#size' => 60,
      '#default_value' => isset($values['children']) ? $values['children'] : '',
    );
    $form['values']['position'] = array(
      '#title' => t('Position'),
      '#description' => t('This determines the position within the parentChildren where you wish to place the ad unit. If you want an ad to appear after the third child add 3.'),
      '#type' => 'textfield',
      '#size' => 60,
      '#default_value' => isset($values['position']) ? $values['position'] : '',
    );

    return $form;
  }

  // Execute context.
  function execute() {
    $i = 0;
    foreach ($this->get_contexts() as $context_name => $context) {
      if (isset($context->reactions['dfp_respond'])) {
        foreach ($context->reactions['dfp_respond'] as $values) {
          if (!empty($values)) {
            $i++;
            $this->dfp_respond_tags['placement' . $i] = $values;
          }
        }
      }
    }
  }

}
