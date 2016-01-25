<?php

/**
 * @file
 * Context reaction plugin for Lazy Loading Ad Slots.
 */

/**
 * Class LazyAdSlotContextReaction
 */
class LazyAdSlotContextReaction extends context_reaction {
  public $lazyadslot_tags = [];

  // Form options.
  function options_form($context) {
    $form = [];
    $values = [];

    // Get existing values for this form.
    $settings = $this->fetch_from_context($context);
    if (!empty($settings['values'])) {
      $values = $settings['values'];
    }

    // Get a list of all the tags.
    $tags = dfp_tag_load_all();
    if (empty($tags)) {
      return [
        '#markup' => t('Before you can use this reaction you need to <a href="/admin/structure/dfp_ads/add">Add a new DFP ad</a>.')
      ];
    }

    $options = [];
    foreach ($tags as $tag) {
      $options[$tag->machinename] = $tag->slot;
    }

    $form['values'] = [
      '#type' => 'fieldset',
    ];
    $form['values']['ad_tag'] = [
      '#type' => 'select',
      '#title' => t('DFP Ad tag'),
      '#description' => t('Select a DFP Ad tag to configure.'),
      '#options' => $options,
      '#default_value' => isset($values['ad_tag']) ? $values['ad_tag'] : [],
    ];
    $form['values']['identifier'] = [
      '#type' => 'textfield',
      '#size' => 20,
      '#title' => t('Identifier(Optional)'),
      '#description' => t('An arbitrary identifier in order to recognize the set of configurations when a bespoke implementation is needed.'),
      '#default_value' => isset($values['identifier']) ? $values['identifier'] : '',
    ];
    $form['values']['onscroll'] = [
      '#type' => 'checkbox',
      '#title' => t('Add on OnScroll'),
      '#description' => t('Load the Ads onScroll or instantly.'),
      '#default_value' => !empty($values['onscroll']),
    ];
    $form['values']['top'] = [
      '#title' => t('Top pixels'),
      '#description' => t('Set the value(px) for when to add the Ad.'),
      '#type' => 'textfield',
      '#size' => 10,
      '#default_value' => isset($values['top']) ? $values['top'] : '',
      '#states' => [
        'invisible' => [':input[name="reactions[plugins][lazyadslot][values][onscroll]"]' => ['checked' => FALSE]],
      ],
    ];
    $form['values']['ad_placement'] = [
      '#title' => t('Ad Placement'),
      '#description' => t('Enter the CSS selector where you wish to ad unit to render. <br/>In case you want to use :nth-child type of selector, please use instead the :nth-of-type selector to achieve expected result.<br/> Only selector which has one element match are supported.'),
      '#type' => 'textarea',
      '#default_value' => isset($values['ad_placement']) ? $values['ad_placement'] : '',
    ];
    $form['values']['attach_how'] = [
      '#type' => 'select',
      '#title' => t('Attach how'),
      '#description' => t('Should the Ad be attached before or after specified selector?<br/>Not Recommended to be used in combination with OnScroll option(high calculation is not implemented).'),
      '#options' => [
        'after' => 'After',
        'before' => 'Before',
      ],
      '#default_value' => isset($values['attach_how']) ? $values['attach_how'] : [],
    ];
    $form['values']['disable_dom_rendering'] = [
      '#type' => 'checkbox',
      '#title' => t('Don\'t render the Ad'),
      '#description' => t('The Ad won\'t be rendered on the page until you call the function lazyLoadAdSlot() in JS.'),
      '#default_value' => !empty($values['disable_dom_rendering']),
    ];
    $form['values']['async_rendering'] = [
      '#type' => 'checkbox',
      '#title' => t('Async rendering'),
      '#description' => t('Wrap the Slot declaration into cmd.push() to be handled asynchronously.'),
      '#default_value' => !empty($values['async_rendering']),
    ];
    return $form;
  }

  /**
   * Options form submit handler.
   *
   * As the context do not provide any validate method, will display a error
   * after all the values are submitted already.
   * (#required in the form element will break module non related contexts.)
   */
  function options_form_submit($values) {
    if (empty($values['values']['ad_placement']) && empty($values['values']['disable_dom_rendering'])) {
      form_set_error('reactions[plugins][lazyadslot][values][ad_placement]', t('Please provide at least one selector.'));
    }
    return $values;
  }

  // Execute context.
  function execute() {
    $i = 0;
    foreach ($this->get_contexts() as $context) {
      if (!isset($context->reactions['lazyadslot'])) {
        continue;
      }

      foreach ($context->reactions['lazyadslot'] as $values) {
        if (!empty($values)) {
          $this->lazyadslot_tags['adslot_' . ++$i] = $values;
        }
      }
    }
  }

}
