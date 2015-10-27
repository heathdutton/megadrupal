<?php

/**
 * @file
 * Contains the Mixitup style plugin.
 */
class views_plugin_style_mixitup_views extends views_plugin_style {

  /**
   * Set default options.
   */
  function option_definition() {
    $options = parent::option_definition();
    // Get default options.
    $default_options = mixitup_views_default_options(TRUE);
    foreach ($default_options as $option => $default_value) {
      $options[$option] = array(
        'default' => $default_value,
      );
      if (is_int($default_value)) {
        $options[$option]['bool'] = TRUE;
      }
    }

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Add Mixitup options to views form.
    $form['mixitup'] = array(
      '#type' => 'fieldset',
      '#title' => t('MixItUp Animation settings'),
    );
    if (mixitup_views_installed()) {
      $options = $this->options;
      $form['filter_type'] = array(
        '#type' => 'select',
        '#title' => t('Type of filtering'),
        '#options' => array(
          'checkboxes' => t('Checkboxes'),
          'select' => t('Selectboxes')
        ),
        '#default_value' => $options['filter_type'],
        '#description' => t('Select needed field type for filtering'),
      );
      $form['animation_enable'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable Animation'),
        '#default_value' => $options['animation_enable'],
        '#attributes' => array(
          'class' => array('animation_enable'),
        ),
      );
      $form['animation_effects'] = array(
        '#type' => 'textfield',
        '#title' => t('Effects'),
        '#description' => t('The effects for all filter operations as a space-separated string.'),
        '#default_value' => $options['animation_effects'],
      );
      $form['animation_duration'] = array(
        '#type' => 'textfield',
        '#title' => t('Duration'),
        '#description' => t('The duration of the animation in milliseconds.'),
        '#default_value' => $options['animation_duration'],
      );
      $form['animation_easing'] = array(
        '#type' => 'textfield',
        '#title' => t('Easing'),
        '#description' => t('For a full list of accepted values, check out easings.net.'),
        '#default_value' => $options['animation_easing'],
      );
      $form['animation_perspectiveDistance'] = array(
        '#type' => 'textfield',
        '#title' => t('perspectiveDistance'),
        '#description' => t('The perspective value in CSS units applied to the container during animations.'),
        '#default_value' => $options['animation_perspectiveDistance'],
      );
      $form['animation_perspectiveOrigin'] = array(
        '#type' => 'textfield',
        '#title' => t('perspectiveOrigin'),
        '#description' => t('The perspective-origin value applied to the container during animations.'),
        '#default_value' => $options['animation_perspectiveOrigin'],
      );
      $form['animation_queue'] = array(
        '#type' => 'checkbox',
        '#title' => t('Queue'),
        '#description' => t('Enable queuing for all operations received while an another operation is in progress.'),
        '#default_value' => $options['animation_queue'],
        '#attributes' => array('class' => array('animation_queue')),
      );
      $form['animation_queueLimit'] = array(
        '#type' => 'textfield',
        '#title' => t('queueLimit'),
        '#description' => t('The maximum number of operations allowed in the queue at any time.'),
        '#default_value' => $options['animation_queueLimit'],
      );

      foreach (mixitup_views_default_options(TRUE) as $option => $default_value) {
        $form[$option]['#fieldset'] = 'mixitup';
        if ($option != 'animation_enable') {
          $selectors['.animation_enable'] = array('checked' => TRUE);
          if ($option == 'animation_queueLimit') {
            $selectors['.animation_queue'] = array('checked' => TRUE);
          }
          $form[$option]['#states'] = array(
            'visible' => $selectors,
          );
        }
      }
      $view = $form_state['view'];
      $sorts = $view->get_items('sort');

      $form['mixitup_sorting_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('MixItUp Sorting settings'),
      );
      $form['use_sort'] = array(
        '#type' => 'checkbox',
        '#fieldset' => 'mixitup_sorting_settings',
        '#title' => t('Use sorting.'),
        '#description' => t('If you want to add new Sort criteria, add them under views "Sort criteria", at first.'),
        '#default_value' => $options['use_sort'],
        '#attributes' => array(
          'class' => array('use_sort'),
        ),
      );
      if ($sorts) {
        $form['sorts'] = array(
          '#type' => 'div',
          '#fieldset' => 'mixitup_sorting_settings',
        );
        foreach ($sorts as $id => $sort) {
          $sort_id = $sort['table'] . '_' . $sort['field'];
          $form['sorts'][$sort_id] = array(
            '#type' => 'textfield',
            '#title' => t('Label for "!f"', array('!f' => $id)),
            '#description' => t("If you don't want to use it, just make this field empty."),
            '#default_value' => isset($options['sorts'][$sort_id]) ? $options['sorts'][$sort_id] : '',
            '#states' => array(
              'visible' => array(
                '.use_sort' => array('checked' => TRUE),
              ),
            ),
          );
        }
      }

      $form['mixitup_vocab'] = array(
        '#type' => 'fieldset',
        '#title' => t('MixItUp Vocabulary settings'),
      );
      $form['restrict_vocab'] = array(
        '#type' => 'checkbox',
        '#fieldset' => 'mixitup_vocab',
        '#title' => t('Restrict terms to particular vocabulary.'),
        '#default_value' => $options['restrict_vocab'],
        '#attributes' => array(
          'class' => array('restrict_vocab_enable'),
        ),
      );
      $all_vocabs = taxonomy_vocabulary_load_multiple(FALSE);
      $vocabulary_options = array();
      foreach ($all_vocabs as $key_vid => $vocab) {
        $vocabulary_options[$key_vid] = $vocab->name;
      }
      $form['restrict_vocab_ids'] = array(
        '#type' => 'checkboxes',
        '#fieldset' => 'mixitup_vocab',
        '#title' => t('Select vocabularies'),
        '#multiple' => TRUE,
        '#options' => $vocabulary_options,
        '#default_value' => $options['restrict_vocab_ids'],
        '#states' => array(
          'visible' => array(
            '.restrict_vocab_enable' => array('checked' => TRUE),
          ),
        ),
      );
    }
    else {
      // Disable Mixitup.
      $form['mixitup_disabled'] = array(
        '#markup' => t('Please, download !mixitup plugin to your
          libraries directory. For more information read !read. After that, you can use it.', array(
          '!mixitup' => l('MixItUp', 'https://github.com/patrickkunka/mixitup'),
          '!read' => l('README', 'admin/help/mixitup_views', array(
            'absolut' => TRUE,
            'attributes' => array('target' => '_blank'),
          )),
        )),
        '#fieldset' => 'mixitup',
      );
    }
  }

}
