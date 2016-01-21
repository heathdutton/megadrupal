<?php

/**
 * @file
 * Custom condition plugin.
 */

$plugin = array(
  'title' => t('Custom values'),
  'description' => t(''),
  'evaluate' => function($expression, $context = NULL) {
    global $user;

    list($type, $option) = $expression;
    if ($context) {
      foreach ($context['types'] as $typedef) {
        if ($typedef['identifier_token_mn'] == $type) {
          foreach ($typedef['options'] as $optdef) {
            if ($optdef['option_name']['name_mn'] == $option) {
              foreach ($optdef['values'] as $role => $value) {
                if ($value && in_array($role, $user->roles)) {
                  return TRUE;
                }
              }
            }
          }
        }
      }
    }

    return FALSE;
  },
  'reason' => function($expression, $context = NULL) {
    list($type, $option) = $expression;

    if ($context) {
      $typedef = head(
        array_filter($context['custom']['types'], function($item) use($type) {
          return $item['identifier_token_mn'] == $type;
        })
      );
      if ($typedef) {
        $optdef = head(
          array_filter($typedef['options'], function($item) use($option) {
            return $item['option_name']['name_mn'] == $option;
          })
        );
        if ($optdef) {
          return t('Users with %type: %option', array(
              '%type' => $typedef['identifier_token'],
              '%option' => $optdef['option_name']['name'],
            ));
        }
      }
    }

    return '';
  },
  'settings' => function(&$form_state, $filter) {
    $form = array();

    $form['#tree'] = TRUE;

    $form['types'] = array(
      '#type' => 'fieldset',
      '#title' => t('Custom values'),
      '#prefix' => '<div id="conditional-text-custom-settings-wrapper">',
      '#suffix' => '</div>',
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'conditional_text') .
            '/plugins/condition/custom/custom.theme.css',
        ),
      ),
      '#attributes' => array(
        'class' => array('conditional-text-custom-plugin-form'),
      ),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );

    if (!isset($form_state['custom']['num_types'])) {
      $form_state['custom']['num_types'] = !isset($filter['types']) ?
        0 : count($filter['types']);
    }

    $roles = user_roles();

    $types = times_map($form_state['custom']['num_types'],
      function($i) use($filter, &$form_state, $roles) {
        $subform = array(
          '#type' => 'fieldset',
          '#prefix' => "<div id=\"conditional-text-custom-types-{$i}-wrapper\">",
          '#suffix' => '</div>',
        );

        if (empty($form_state['custom']['num_type'][$i])) {
          $form_state['custom']['num_type'][$i] =
            empty($filter['types'][$i]['options']) ?
              1 : count($filter['types'][$i]['options']);
        }

        $subform['identifier_token'] = array(
          '#type' => 'textfield',
          '#title' => t('Group'),
          '#description' => t('Enter the displayed name for your custom values group. ' .
                              'Example: Experience level'),
          '#default_value' => isset($filter['types'][$i]['identifier_token']) ?
            $filter['types'][$i]['identifier_token'] : NULL,
          '#element_validate' => array(
            'conditional_text_custom_plugin_identifier_token_validate',
          ),
        );

        $subform['identifier_token_mn'] = array(
          '#type' => 'machine_name',
          '#default_value' =>
            isset($filter['types'][$i]['identifier_token_mn']) ?
              $filter['types'][$i]['identifier_token_mn'] : NULL,
          '#machine_name' => array(
            'exists' =>
              'conditional_text_plugin_custom_identifier_token_exists',
          ),
          '#process' => array(
            'conditional_text_plugin_custom_identifier_token_mn_process',
            'form_process_machine_name',
            'ajax_process_form',
          ),
          '#required' => FALSE,
        );

        $subform['options'] = array(
          '#theme_wrappers' => array(
            'conditional_text_form_table',
          ),
        );

        $subform['options']['header'] = array(
          '#theme_wrappers' => array(
            'conditional_text_form_tr',
          ),
        ) + array_map(function($item) {
            return array(
              '#markup' => check_plain($item),
              '#theme_wrappers' => array(
                'conditional_text_form_th',
              ),
            );
          }, array('' => t('Values')) + $roles);

        $subform['options'] += times_map($form_state['custom']['num_type'][$i],
          function($j) use($i, $filter, $roles) {
            return array(
              '#theme_wrappers' => array(
                'conditional_text_form_tr',
              ),
              'option_name' => array(
                'name' => array(
                  '#type' => 'textfield',
                  '#title' => t('Option @num', array('@num' => $j + 1)),
                  '#title_display' => 'invisible',
                  '#description' => t('Enter a value in this group, ' .
                                      'and choose which roles on your site ' .
                                      'have this value by default. ' .
                                      'Example: Beginner'),
                  '#default_value' =>
                    isset($filter['types'][$i]['options'][$j]['option_name']['name']) ?
                      $filter['types'][$i]['options'][$j]['option_name']['name']: NULL,
                ),
                'name_mn' => array(
                  '#type' => 'machine_name',
                  '#machine_name' => array(
                    'exists' =>
                      'conditional_text_plugin_custom_option_name_exists',
                  ),
                  '#process' => array(
                    'conditional_text_plugin_custom_option_mn_process',
                    'form_process_machine_name',
                    'ajax_process_form',
                  ),
                ),
                '#theme_wrappers' => array(
                  'conditional_text_form_th',
                ),
              ),
              'values' => drupal_map_assoc($roles,
                function($item) use($filter, $i, $j) {
                  return array(
                    '#type' => 'checkbox',
                    '#title' => '',
                    '#title_display' => 'attribute',
                    '#default_value' =>
                      isset($filter['types'][$i]['options'][$j]['values'][$item]) ?
                        $filter['types'][$i]['options'][$j]['values'][$item] : NULL,
                    '#theme_wrappers' => array(
                      'form_element',
                      'conditional_text_form_td',
                    ),
                  );
                }),
            );
          });

        $subform['add'] = array(
          '#type' => 'submit',
          '#value' => t('Add option'),
          '#name' => "add_option_{$i}",
          '#custom_key' => $i,
          '#submit' => array('conditional_text_custom_add_option_submit'),
          '#ajax' => array(
            'callback' => 'conditional_text_custom_add_option_callback',
            'wrapper' => "conditional-text-custom-types-{$i}-wrapper",
          ),
          '#attributes' => array(
            'class' => array('add-option'),
          ),
        );

        if ($form_state['custom']['num_type'][$i] > 1) {
          $subform['remove'] = array(
            '#type' => 'submit',
            '#value' => t('Remove last option'),
            '#name' => "remove_option_{$i}",
            '#custom_key' => $i,
            '#submit' => array('conditional_text_custom_remove_option_submit'),
            '#ajax' => array(
              'callback' => 'conditional_text_custom_add_option_callback',
              'wrapper' => "conditional-text-custom-types-{$i}-wrapper",
            ),
            '#attributes' => array(
              'class' => array('remove-option'),
            ),
          );
        }

        $subform['remove-type'] = array(
          '#type' => 'submit',
          '#value' => t('Remove this type'),
          '#name' => "remove_type_{$i}",
          '#custom_key' => $i,
          '#submit' => array('conditional_text_custom_remove_button_submit'),
          '#ajax' => array(
            'callback' => 'conditional_text_custom_add_button_callback',
            'wrapper' => 'conditional-text-custom-settings-wrapper',
          ),
        );

        return $subform;
      });

    $form['types'] += $types;

    $form['types']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add type'),
      '#submit' => array('conditional_text_custom_add_button_submit'),
      '#ajax' => array(
        'callback' => 'conditional_text_custom_add_button_callback',
        'wrapper' => 'conditional-text-custom-settings-wrapper',
      ),
    );

    return $form;
  },
  'settings include' => drupal_get_path('module', 'conditional_text') .
    '/plugins/condition/custom/form.inc',
  'identifier token' => 'custom',
  'short help' => repeat(t('Custom values condition type: ' .
    '[condition custom GROUP VALUE]')),
  'long help' => repeat(t('Custom values condition type:' .
    '[condition custom GROUP VALUE], where:<ul>
<li>GROUP is the machine name of a custom-defined group for this filter.</li>
<li>VALUE is the machine name of a custom-defined value within the group.</li>
</ul>')),
);
