<?php

$plugin = array(
  'schema' => 'template',
  'menu' => array(
    'menu prefix' => 'admin/structure',
    'menu item' => 'template',
    'menu title' => 'Templates',
    'menu description' => 'Manage the Site Templates.',
  ),
  'title singular' => t('template'),
  'title singular proper' => t('Template'),
  'title plural' => t('templates'),
  'title plural proper' => t('Templates'),
  'form' => array(
    'settings' => 'templates_form',
  ),
  'handler' => array(
    'class' => 'TemplateAPIExportUI',
    'parent' => 'ctools_export_ui',
  ),
  'access' => 'access template administration pages',
);

/**
 * Defines the template configuration form.
 */
function templates_form(&$form, $form_state) {
  $template = $form_state['item'];
  unset($form['info']);

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#maxlength' => 60,
    '#default_value' => isset($template->label) ? $template->label : '',
    '#weight' => 1
  );

  $form['name'] = array(
    '#title' => t('Unique name for the template'),
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#default_value' => isset($template->name) ? $template->name : '',
    '#machine_name' => array(
      'exists' => 'template_api_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this template. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => 2
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => isset($template->description) ? $template->description : '',
    '#weight' => 3
  );

  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Tags'),
    '#default_value' => isset($template->tags) ? implode(',', $template->tags) : '',
    '#description' => t('A comma seperated list of tags'),
    '#weight' => 4
  );

  $options = array();
  foreach (template_api_get_owner_types() as $owner_type => $type_info) {
    $options[$owner_type] = $type_info['label'];
  }



  if (empty($template->owner_type) && count($options) == 1) {
    $keys = array_keys($options);
    $template->owner_type = array_shift($keys);
  }

  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] = "owner_type") {
    $template->owner_type = $form_state['triggering_element']['#value'];
  }

  array_unshift($options, t('- Select -'));

  $form['owner_type'] = array(
    '#type' => 'select',
    '#title' => t('Owner Type'),
    '#options' => $options,
    '#default_value' => isset($template->owner_type) ? $template->owner_type : '',
    '#description' => t('The Owner Type to use for this template'),
    '#ajax' => array(
      'callback' => 'template_api_owner_type_ajax',
      'wrapper' => 'template-api-owner',
    ),
    '#weight' => 5
  );


  $form['owner_wrapper'] = array(
    '#prefix' => '<div id="template-api-owner">',
    '#suffix' => '</div>',
    '#weight' => 6
  );

  $options = empty($template->owner_type) ? array() :
    template_api_get_owner_type_controller($template->owner_type)->getValues();

  $form['owner_wrapper']['owner'] = array(
    '#type' => 'select',
    '#title' => t('Owner'),
    '#options' => $options,
    '#default_value' => isset($template->owner) ? $template->owner : '',
    '#description' => t('The Owner of the template'),
  );

  $renderers = template_api_get_renderers();
  if (count($renderers) > 1) {
    $options = array();
    foreach($renderers as $renderer) {
      $options[$renderer['name']] = $renderer['label'];
    }
    $form['renderer'] = array(
      '#title' => t('Renderer'),
      '#type' => 'select',
      '#default_value' => isset($template->renderer) ? $template->renderer : '',
      '#options' => $options,
      '#weight' => 8
    );
  }
  else {
    $renderer = reset($renderers);
    $form['renderer'] = array(
      '#type' => 'value',
      '#value' => $renderer['name'],
      '#weight' => 8
    );
  }

  $form['settings-tabs'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 9
  );

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content'),
    '#group' => 'settings-tabs',
    '#tree' => 0,
    '#weight' => 1,
  );
  $form['content']['content'] = array(
    '#title' => t('Content of template'),
    '#type' => 'text_format',
    '#default_value' => isset($template->content) ? $template->content : '',
    '#rows' => 20,
    '#format' => 'filtered_html',
  );

  $form['attached'] = array(
    '#type' => 'container',
    '#title' => t('Attachments'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['attached']['css'] = array(
    '#type' => 'fieldset',
    '#title' => t('CSS'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'settings-tabs',
    '#weight' => 2,
  );

  $form['attached']['css']['css_content'] = array(
    '#title' => t('CSS'),
    '#type' => 'text_format',
    '#default_value' => isset($template->attached['css']) ? $template->attached['css'] : '',
    '#rows' => 10,
    '#format' => 'filtered_html',
  );

  $form['attached']['js'] = array(
    '#type' => 'fieldset',
    '#title' => t('JavaScript'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'settings-tabs',
    '#weight' => 3,
  );

  $form['attached']['js']['js_content'] = array(
    '#title' => t('JavaScript'),
    '#type' => 'text_format',
    '#default_value' => isset($template->attached['js']) ? $template->attached['js'] : '',
    '#rows' => 10,
    '#format' => 'filtered_html',
  );

  $form['attached']['libraries'] = array(
    '#title' => t('Libraries'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'settings-tabs',
    '#weight' => 4,
  );

  // Attached files
  $libraries = array();
  foreach (module_list() as $module) {
    foreach (drupal_get_library($module) as $library_key => $library_data) {
      $libraries["$module:::$library_key"] = $library_data['title'];
    }
  }

  if (!isset($template->attached['libraries'])) {
    $template->attached['libraries'] = array();
  }

  $default_values = array();
  foreach($template->attached['libraries'] as $key => $attached) {
    $default_values[] = "{$attached['module']}:::{$attached['name']}";
  }

  $form['attached']['libraries']['libraries'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Additional libraries'),
    '#options' => $libraries,
    '#default_value' => drupal_map_assoc($default_values),
  );

  $form['inputs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Input Definitions'),
    '#group' => 'settings-tabs',
    '#weight' => 5,
    '#tree' => TRUE,
    '#theme' => 'template_api_template_inputs_form',
  );

  $inputs_types = template_api_get_input_types();
  $options = array();
  foreach($inputs_types as $input_type) {
    $options[$input_type['name']] = $input_type['label'];
  }

  if (!isset($template->inputs)) {
    $template->inputs = array();
  }
  $max_weight = 0;
  foreach ($template->getFlattenedInputs() as $input_key => $input) {
    $form['inputs'][$input_key] = array(
      '#type' => 'container',
      '#title' => $input['label'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );

    $form['inputs'][$input_key]['label'] = array(
      '#title' => t('Label'),
      '#type' => 'textfield',
      '#default_value' => $input['label'],
      '#size' => '20',
    );

    $form['inputs'][$input_key]['type'] = array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#options' => $options,
      '#empty_option' => t('- Select an input type -'),
      '#default_value' => $input['type'],
    );

    $form['inputs'][$input_key]['description'] = array(
      '#title' => t('Description'),
      '#type' => 'textarea',
      '#default_value' => $input['description'],
    );

    if (_template_api_can_field_be_required($form['inputs'][$input_key])) {
      $form['inputs'][$input_key]['required'] = array(
        '#title' => t('Required'),
        '#type' => 'checkbox',
        '#default_value' => isset($input['required']) ? $input['required'] : FALSE,
      );
    }

    $form['inputs'][$input_key]['remove'] = array(
      '#title' => t('Remove this input?'),
      '#type' => 'checkbox',
      '#default_value' => FALSE,
    );

    $form['inputs'][$input_key]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('weight'),
      '#delta' => 50,
      '#default_value' => isset($input['weight']) && !empty($input['weight']) ? $input['weight'] : 0,
      '#title_display' => 'invisible',
    );

    $form['inputs'][$input_key]['depth'] = array(
      '#type' => 'hidden',
      '#title' => t('depth'),
      '#maxlength' => 2,
      '#default_value' => isset($input['depth']) && !empty($input['depth']) ? $input['depth'] : 0,
      '#title_display' => 'invisible',
    );

    $form['inputs'][$input_key]['parent'] = array(
      '#type' => 'hidden',
      '#title' => t('parent'),
      '#default_value' => isset($input['parent']) && !empty($input['parent']) ? $input['parent'] : NULL,
      '#title_display' => 'invisible',
    );

    $form['inputs'][$input_key]['field_id'] = array(
      '#type' => 'hidden',
      '#default_value' => $input_key,
    );

    $form['inputs'][$input_key] += $template->getExtraAdminFields($input['type'], $input);
    $weight = isset($input['weight']) ? $input['weight'] : 0;
    $max_weight = max($max_weight, $weight);
  }

  $form['inputs']['new'] = array(
    '#type' => 'container',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );

  $form['inputs']['new']['key'] = array(
    '#type' => 'textfield',
    '#title' => t('Add new input'),
    '#default_value' => '',
    '#size' => '20',
  );

  $form['inputs']['new']['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => '',
    '#size' => '20',
  );

  $form['inputs']['new']['type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => $options,
    '#empty_option' => t('- Select an input type -'),
    '#default_value' => '',
  );

  $form['inputs']['new']['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
  );

  $form['inputs']['new']['required'] = array(
    '#title' => t('Required'),
    '#type' => 'checkbox',
    '#default_value' => FALSE,
  );

  $max_weight = min($max_weight + 10, 50);
  $form['inputs']['new']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('weight'),
    '#delta' => 50,
    '#default_value' => $max_weight,
    '#title_display' => 'invisible',
  );
  $form['inputs']['new']['depth'] = array(
    '#type' => 'hidden',
    '#title' => t('depth'),
    '#maxlength' => 2,
    '#default_value' => 0,
    '#title_display' => 'invisible',
  );

  $form['inputs']['new']['parent'] = array(
    '#type' => 'hidden',
    '#title' => t('parent'),
    '#default_value' => NULL,
    '#title_display' => 'invisible',
  );

}

/**
 * Theme function for rendering the template configuration form.
 */
function theme_template_api_template_inputs_form($variables) {
  $form = $variables['form'];

  $row_class = 'draggable';
  $row_weight_class = 'template-api-row-weight';
  $row_parent_class = 'template-api-row-parent';
  $row_id_class = 'template-api-row-id';
  $row_depth_class = 'template-api-row-depth';

  $rows = array();
  $header = array('key', 'label', 'type', 'description', 'required', 'remove', 'Order', 'extra');

  foreach (element_children($form) as $index => $key) {
    $form[$key]['label']['#title_display'] = 'invisible';
    $form[$key]['type']['#title_display'] = 'invisible';
    $form[$key]['description']['#title_display'] = 'invisible';
    $form[$key]['remove']['#title_display'] = 'invisible';
    if (isset($form[$key]['required'])) {
      $form[$key]['required']['#title_display'] = 'invisible';
    }

    $form[$key]['weight']['#attributes']['class'] = array($row_weight_class);
    $form[$key]['parent']['#attributes']['class'][] = $row_parent_class;
    $form[$key]['field_id']['#attributes']['class'][] = $row_id_class;
    $form[$key]['depth']['#attributes']['class'][] = $row_depth_class;

    // Only allow parent-child relationships with fieldsets and verticle tabs.
    $leaf_class = '';
    switch ($form[$key]['type']['#default_value']) {
      case 'vertical_tabs':
        $leaf_class = 'tabledrag-root';
      case 'fieldset':
        $leaf_class = '';
        break;
      default:
        $leaf_class = 'tabledrag-leaf';
        break;
    }

    $depth = isset($form[$key]['depth']['#value']) ? $form[$key]['depth']['#value'] : 0;
    $indentation = theme('indentation', array('size' => $depth));

    $draggable_column = array(
      'class' => array($row_weight_class),
      'data' => $indentation . (isset($form[$key]['key']) ? drupal_render($form[$key]['key']) : check_plain($key)),
    );

    $row = array(
      'class' => array($row_class, $leaf_class),
      'data' => array(
        $draggable_column,
        drupal_render($form[$key]['label']),
        drupal_render($form[$key]['type']),
        drupal_render($form[$key]['description']),
        drupal_render($form[$key]['required']),
        drupal_render($form[$key]['remove']),
        drupal_render($form[$key]['parent']) . drupal_render($form[$key]['weight']) . drupal_render($form[$key]['field_id']). drupal_render($form[$key]['depth']),
        drupal_render($form[$key]),
      ),
    );
    $rows[] = $row;
  }

  drupal_add_tabledrag('template-api-input-definitions', 'match', 'parent', $row_parent_class, $row_parent_class, $row_id_class, TRUE, 2);
  drupal_add_tabledrag('template-api-input-definitions', 'depth', 'group', $row_depth_class, NULL, NULL, FALSE);
  drupal_add_tabledrag('template-api-input-definitions', 'order', 'sibling', $row_weight_class, NULL, NULL, FALSE);


//  return drupal_render_children($form);
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'template-api-input-definitions')));
//  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Determines if a given template field can be flagged as required.
 *
 * @param $field
 * The field on the form.
 *
 * @return bool
 * TRUE if the field can be required.  FALSE if it cannot be required.
 */
function _template_api_can_field_be_required($field) {
  switch ($field['type']['#default_value']) {
    case 'vertical_tabs':
    case 'fieldset':
      $can_be_required = FALSE;
      break;
    default:
      $can_be_required = TRUE;
      break;
  }

  return $can_be_required;
}
