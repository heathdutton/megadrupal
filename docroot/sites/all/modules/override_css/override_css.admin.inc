<?php
/**
 * Menu callback to reset a selector group
 */
function override_css_reset_selector_page($selector, $redirect = FALSE) {
  return drupal_get_form('override_css_reset_selector_form', $selector, $redirect);
}

/**
 * Build a confirm form for to reset a selector group
 */
function override_css_reset_selector_form($form, &$form_state, $selector, $redirect = FALSE) {
  $form['#selector'] = $selector;
  $form['#redirect'] = $redirect;

  $path = $redirect ? variable_get('override_css_settings_path', OVERRIDE_CSS_SETTINGS_PATH) : 'admin/appearance/override_css/list/' . $selector['name'];

  return confirm_form($form, t('Are you sure you want to reset the selector group %title?', array('%title' => $selector['title'])), $path);
}

/**
 * Reset a selector group
 */
function override_css_reset_selector_form_submit($form, &$form_state) {
  $selector = $form['#selector'];
  $redirect = $form['#redirect'];

  override_css_selector_reset($selector);

  override_css_store_css();

  $t_args = array('%title' => $selector['title']);
  drupal_set_message(t('The selector group %title has been reset.', $t_args));

  $path = $redirect ? variable_get('override_css_settings_path', OVERRIDE_CSS_SETTINGS_PATH) : 'admin/appearance/override_css';
  $form_state['redirect'] = $path;
}

/**
 * Set values of css properties
 */
function override_css_selector_value($form, &$form_state, $selector = array(), $redirect = FALSE) {
  drupal_add_library('system', 'farbtastic');

  $available_properties = override_css_properties_list();
  $properties = array_filter($selector['properties']);

  $form = array();

  $form['description'] = array(
    '#markup' => '<em>'. $selector['description'] .'</em>',
  );

  $form['selector'] = array(
    '#type' => 'value',
    '#value' => $selector['name'],
  );

  $form['redirect'] = array(
    '#type' => 'value',
    '#value' => $redirect,
  );

  foreach ( $properties as $property ) {
    $fieldset = $property .'_fieldset';
    $form[$fieldset] = array(
      '#type' => 'fieldset',
      '#title' => $available_properties[$property]['title'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      // '#description' => t('Description'), // @todo
    );

    switch ($available_properties[$property]['type']) {
      case 'border':
        $form[$fieldset][$property .'_picker'] = array(
          '#markup' => '<div id="'. $available_properties[$property]['name'] .'-picker" style="float: right;"></div>',
          '#weight' => 1,
        );
        $form[$fieldset][$property] = array(
          '#type' => 'textfield',
          '#title' => 'Kleur',
          '#default_value' => isset($selector['properties_values'][$property]) ? $selector['properties_values'][$property] : '#ffffff',
          '#description' => '',
          '#size' => 7,
          '#prefix' => "<script type='text/javascript'>
            (function ($) {
              Drupal.behaviors.override_css_". $property ." = {
                attach: function() {
                  $('#". $available_properties[$property]['name'] ."-picker').farbtastic('#edit-". $available_properties[$property]['name'] ."');
                }
              }
            })(jQuery);
          </script>",
          '#weight' => 2,
        );
        $form[$fieldset][$property .'_size'] = array(
          '#title' => 'Dikte',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_size']) ? $selector['properties_values'][$property .'_size'] : 0,
          '#size' => 5,
          '#field_suffix' => 'px',
          '#weight' => 3,
        );
        $border_type = array('solid' => 'Solid', 'dashed' => 'Dashed', 'dotted' => 'Dotted', 'double' => 'Double', 'groove' => 'Groove', 'ridge' => 'Ridge', 'inset' => 'Inset', 'outset' => 'Outset');
        $form[$fieldset][$property .'_type'] = array(
          '#title' => 'Type',
          '#type' => 'select',
          '#options' => $border_type,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_type']) ? ($selector['properties_values'][$property .'_type']) : '',
        );
        break;

      case 'shadow':
        $form[$fieldset][$property .'_horizontal'] = array(
          '#title' => 'Positie horizontaal',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_horizontal']) ? $selector['properties_values'][$property .'_horizontal'] : 0,
          '#size' => 5,
          '#field_suffix' => 'px',
          '#weight' => 3,
        );
        $form[$fieldset][$property .'_vertical'] = array(
          '#title' => 'Positie verticaal',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_vertical']) ? $selector['properties_values'][$property .'_vertical'] : 0,
          '#size' => 5,
          '#field_suffix' => 'px',
          '#weight' => 4,
        );
        $form[$fieldset][$property .'_blur'] = array(
          '#title' => 'Blur',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_blur']) ? $selector['properties_values'][$property .'_blur'] : 0,
          '#size' => 5,
          '#field_suffix' => 'px',
          '#weight' => 5,
        );
        $form[$fieldset][$property .'_spread'] = array(
          '#title' => 'Grootte',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_spread']) ? $selector['properties_values'][$property .'_spread'] : 0,
          '#size' => 5,
          '#field_suffix' => 'px',
          '#weight' => 6,
        );
        $direction = array('' => 'Naar buiten', 'inset' => 'Naar binnen');
        $form[$fieldset][$property .'_direction'] = array(
          '#title' => 'Binnen- of buitenkant',
          '#type' => 'select',
          '#options' => $direction,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_direction']) ? ($selector['properties_values'][$property .'_direction']) : '',
          '#weight' => 7,
        );

      case 'background-color':
        $form[$fieldset][$property .'_opacity'] = array(
          '#title' => 'Percentage doorzichtbaarheid',
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property .'_opacity']) ? $selector['properties_values'][$property .'_opacity'] : 0,
          '#size' => 5,
          '#field_suffix' => '%',
          '#weight' => 3,
        );

      case 'color':
        $default = isset($selector['properties_values'][$property]) ? $selector['properties_values'][$property] : '';

        $form[$fieldset][$property .'_container'] = array(
          '#type' => 'container',
        );
        $form[$fieldset][$property .'_container'][$property .'_picker'] = array(
          '#markup' => '<div id="'. $available_properties[$property]['name'] .'-picker" style="float: right;"></div>',
          '#weight' => 1,
        );
        $form[$fieldset][$property] = array(
          '#type' => 'textfield',
          '#title' => 'Kleur',
          '#default_value' => ($default == '' ? '#ffffff' : $default),
          '#description' => '',
          '#size' => 7,
          '#prefix' => "<script type='text/javascript'>
            (function ($) {
              Drupal.behaviors.override_css_". $property ." = {
                attach: function() {
                  $('#". $available_properties[$property]['name'] ."-picker').farbtastic('#edit-". $available_properties[$property]['name'] ."');
                }
              }
            })(jQuery);
          </script>",
          '#weight' => 2,
        );
        break;

      case 'size':
      case 'radius':
        $form[$fieldset][$property] = array(
          '#title' => $available_properties[$property]['title'],
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property]) ? $selector['properties_values'][$property] : '',
          '#size' => 5,
          '#field_suffix' => $available_properties[$property]['unit'],
        );
        break;

      case 'image':
        $form[$fieldset][$property] = array(
          '#title' => 'Pad naar huidige afbeelding',
          '#type' => 'textfield',
          '#description' => 'Het pad naar het bestand dat u wilt gebruiken.',
          '#default_value' => isset($selector['properties_values'][$property]) ? ($selector['properties_values'][$property]) : '',
        );
        $form[$fieldset][$property .'_upload'] = array(
          '#title' => 'Upload nieuwe afbeelding',
          '#type' => 'file',
          '#maxlength' => 40,
          '#description' => 'Gebruik dit veld om een nieuwe afbeelding te uploaden.',
        );
        $position_horizontal = array('left' => 'Links', 'center' => 'Centreren', 'right' => 'Rechts');
        $form[$fieldset][$property .'_position_horizontal'] = array(
          '#title' => 'Horizontale positie',
          '#type' => 'select',
          '#options' => $position_horizontal,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_position_horizontal']) ? ($selector['properties_values'][$property .'_position_horizontal']) : '',
        );
        $position_vertical = array('top' => 'Bovenkant', 'center' => 'Centreren', 'bottom' => 'Onderkant');
        $form[$fieldset][$property .'_position_vertical'] = array(
          '#title' => 'Verticale positie',
          '#type' => 'select',
          '#options' => $position_vertical,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_position_vertical']) ? ($selector['properties_values'][$property .'_position_vertical']) : '',
        );
        $attachment = array('scroll' => 'Mee scrollen met pagina', 'fixed' => 'Gefixeerd tov de pagina');
        $form[$fieldset][$property .'_attachment'] = array(
          '#title' => 'Attachment',
          '#type' => 'select',
          '#options' => $attachment,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_attachment']) ? ($selector['properties_values'][$property .'_attachment']) : '',
        );
        $repeat = array('repeat' => 'Herhaal horizontaal en verticaal', 'repeat-x' => 'Herhaal enkel in horizontale richting', 'repeat-y' => 'Herhaal enkel in verticale richting', 'no-repeat' => 'Geen herhaling');
        $form[$fieldset][$property .'_repeat'] = array(
          '#title' => 'Herhaling',
          '#type' => 'select',
          '#options' => $repeat,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_repeat']) ? ($selector['properties_values'][$property .'_repeat']) : '',
        );
        $stretch = array('none' => 'Niet', 'horizontal' => 'Volledige breedte', 'vertical' => 'Volledige hoogte', 'both' => 'Volledige hoogte en breedte');
        $form[$fieldset][$property .'_stretch'] = array(
          '#title' => 'Afbeelding uitrekken',
          '#type' => 'select',
          '#options' => $stretch,
          '#description' => '',
          '#default_value' => isset($selector['properties_values'][$property .'_stretch']) ? ($selector['properties_values'][$property .'_stretch']) : 'none',
        );
        break;

      case 'font':
        $form[$fieldset][$property] = array(
          '#title' => 'Lettertype',
          '#type' => 'select',
          '#options' => override_css_font_option_list(),
          '#default_value' => isset($selector['properties_values'][$property]) ? $selector['properties_values'][$property] : '',
          '#suffix' => override_css_font_list_preview(),
        );
        break;

      default:
        $form[$fieldset][$property] = array(
          '#title' => $available_properties[$property]['title'],
          '#type' => 'textfield',
          '#default_value' => isset($selector['properties_values'][$property]) ? $selector['properties_values'][$property] : '',
          '#size' => 25,
        );
        break;
    }

    // Add checkbox to reset/disable property
    foreach ($form[$fieldset] as $key => $element) {
      if (is_array($element)) {
        $element['#states'] = array(
          'invisible' => array(
           ':input[name="' . $property . '_disabled"]' => array('checked' => TRUE),
          ),
        );
      }

      $form[$fieldset][$key] = $element;
    }
    $form[$fieldset][$property . '_disabled'] = array(
      '#type' => 'checkbox',
      '#title' => 'Disable ' .  strtolower($available_properties[$property]['title']),
      '#default_value' => isset($selector['properties_values'][$property .'_disabled']) ? ($selector['properties_values'][$property .'_disabled']) : 0,
      '#weight' => -100,
    );

    // Integration with sliderfield module (if available)
    if (module_exists('sliderfield')) {
      foreach ($form[$fieldset] as $key => $element) {
        if (is_array($element) && $element['#type'] == 'textfield' && isset($element['#field_suffix'])) {
          $element['#type'] = 'slider';

          switch ($element['#field_suffix']) {
            case 'px':
              $element['#min'] = 0;
              $element['#max'] = 40;
              break;
            case '%':
              $element['#min'] = 0;
              $element['#max'] = 100;
              break;
          }

          $element['#display_inputs'] = FALSE;
          $element['#display_values'] = TRUE;
          $element['#display_values_format'] = '%{value}% ' . $element['#field_suffix'];
          $element['#field_suffix'] = '';
          $element['#slider_length'] = '250px';
        }

        $form[$fieldset][$key] = $element;
      }
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $redirect ? variable_get('override_css_settings_path', OVERRIDE_CSS_SETTINGS_PATH) : 'admin/appearance/override_css/list/'. $selector['name'],
  );

  return $form;
}

/**
 * Validate selector group form
 */
function override_css_selector_value_validate($form, &$form_state) {
  $selector = override_css_selector_load($form_state['values']['selector']);

  $available_properties = override_css_properties_list();
  $properties = array_filter($selector['properties']);

  // Integration with sliderfield module needs rewrite of the value
  if (module_exists('sliderfield')) {
    foreach ($form_state['values'] as $key => $element) {
      if (is_array($element) && isset($element['value'])) {
        $form_state['values'][$key] = $element['value'];
      }
    }
  }

  foreach ( $properties as $property ) {
    switch ($available_properties[$property]['type']) {
      case 'border':
        if ($form_state['values'][$property .'_size'] ) {
          if ( !is_numeric($form_state['values'][$property .'_size']) ) {
            form_set_error($property .'_opacity', 'Enkel numerieke waarden zijn toegestaan.');
          }
        }
        break;

      case 'shadow':
        $extra_fields = array('_horizontal', '_vertical', '_blur', '_spread');
        foreach ( $extra_fields as $extra_field ) {
          if ($form_state['values'][$property . $extra_field] ) {
            if ( !is_numeric($form_state['values'][$property . $extra_field]) ) {
              form_set_error($property . $extra_field, 'Enkel numerieke waarden zijn toegestaan.');
            }
          }
        }

      case 'background-color':
        if ($form_state['values'][$property .'_opacity'] ) {
          if ( !is_numeric($form_state['values'][$property .'_opacity']) ) {
            form_set_error($property .'_opacity', 'Enkel numerieke waarden zijn toegestaan.');
          }
          elseif ( $form_state['values'][$property .'_opacity'] < 0 || $form_state['values'][$property .'_opacity'] > 100 ) {
            form_set_error($property .'_opacity', 'Enkel waarden tussen 0 (niet transparant) en 100 (transparant) zijn toegestaan.');
          }
        }

      case 'color':
        if ($form_state['values'][$property] && !preg_match('/^#([a-f0-9]{3}){1,2}$/iD', $form_state['values'][$property]) ) {
          form_set_error($property, 'De kleur is geen geldige hexadecimale css waarde (#xxxxxx).');
        }
        break;

      case 'size':
      case 'radius':
        if ($form_state['values'][$property] && !is_numeric($form_state['values'][$property]) ) {
          form_set_error($property, 'Enkel numerieke waarden zijn toegestaan.');
        }
        break;

      case 'image':
        module_load_include('inc', 'system', 'system.admin');
        //validate given path
        if ($form_state['values'][$property]) {
          $path = _system_theme_settings_validate_path($form_state['values'][$property]);
          if (!$path) {
            form_set_error($property, 'Het pad naar de afbeelding bestaat niet, gebruik het upload formulier om een nieuwe afbeelding up te loaden of geef een correct pad in.');
          }
        }
        // validate upload
        $validators = array('file_validate_is_image' => array());
        $file = file_save_upload($property .'_upload', $validators);
        if (isset($file)) {
          if ($file) {
            $form_state['values'][$property .'_upload'] = $file;
          }
          else {
            form_set_error($property .'_upload', 'De afbeelding kon niet geüpload worden.');
          }
        }
        break;

      default:
        break;
    }
  }
}

/**
 * Submit selector group form
 */
function override_css_selector_value_submit($form, &$form_state) {
  $values = array();

  $selector = override_css_selector_load($form_state['values']['selector']);

  $path = $form['redirect'] ? variable_get('override_css_settings_path', OVERRIDE_CSS_SETTINGS_PATH) : 'admin/appearance/override_css/' . $selector['name'];
  $form_state['redirect'] = $path;

  $available_properties = override_css_properties_list();
  $properties = array_filter($selector['properties']);

  foreach ( $properties as $property ) {
    switch ($available_properties[$property]['type']) {
      case 'border':
        $values[$property] = isset($form_state['values'][$property]) ? $form_state['values'][$property] : '';
        $values[$property .'_size'] = isset($form_state['values'][$property .'_size']) ? $form_state['values'][$property .'_size'] : 0;
        $values[$property .'_type'] = isset($form_state['values'][$property .'_type']) ? $form_state['values'][$property .'_type'] : '';
        break;

      case 'shadow':
        $extra_fields = array('_horizontal', '_vertical', '_blur', '_spread');
        foreach ( $extra_fields as $extra_field ) {
          $values[$property . $extra_field] = isset($form_state['values'][$property . $extra_field]) ? $form_state['values'][$property . $extra_field] : 0;
        }
        $values[$property .'_direction'] = isset($form_state['values'][$property .'_direction']) ? $form_state['values'][$property .'_direction'] : '';

      case 'background-color':
        $values[$property .'_opacity'] = isset($form_state['values'][$property .'_opacity']) ? $form_state['values'][$property .'_opacity'] : 0;

      case 'color':
        $values[$property] = isset($form_state['values'][$property]) ? $form_state['values'][$property] : '';
        break;

      case 'size':
      case 'transparent':
      case 'radius':
      case 'font':
        $values[$property] = isset($form_state['values'][$property]) ? $form_state['values'][$property] : '';
        break;

      case 'image':
        module_load_include('inc', 'system', 'system.admin');
        // Save eerst ingegeven pad
        if (!empty($form_state['values'][$property])) {
          $values[$property] = _system_theme_settings_validate_path($form_state['values'][$property]);
        }
        // Overschrijf pad indien afbeelding is geüpload
        if ($file = $form_state['values'][$property .'_upload']) {
          unset($form_state['values'][$property .'_upload']);
          $destination = 'public://'. OVERRIDE_CSS_FILE_PATH;
          file_prepare_directory($destination, FILE_CREATE_DIRECTORY);
          $filename = file_unmanaged_copy($file->uri, $destination .'/'. $file->filename);
          $values[$property] = $filename;
        }
        // Extra properties for backgrounds
        $values[$property .'_position_horizontal'] = $form_state['values'][$property .'_position_horizontal'];
        $values[$property .'_position_vertical'] = $form_state['values'][$property .'_position_vertical'];
        $values[$property .'_attachment'] = $form_state['values'][$property .'_attachment'];
        $values[$property .'_repeat'] = $form_state['values'][$property .'_repeat'];
        $values[$property .'_stretch'] = $form_state['values'][$property .'_stretch'];
        break;

      default:
        $values[$property] = isset($form_state['values'][$property]) ? $form_state['values'][$property] : '';
        break;
    }

    // Saving disabled setting
    $values[$property .'_disabled'] = $form_state['values'][$property .'_disabled'];
  }

  db_merge('override_css_properties')
    ->fields(array('name' => $selector['name'], 'properties_values' => serialize($values)))
    ->condition('name', $selector['name'], '=')
    ->execute();

  override_css_store_css();

  drupal_set_message(t('Your values for this css selector group has been saved.'));
}

function override_css_preview_css() {
  ctools_include('css');

  $css = ctools_css_disassemble(override_css_generate_css());

  return empty($css) ? 'Er is geen css ingegeven.' :'<div style="border: 1px solid #000; background-color: #eee; padding: 5px;"><pre>'. ctools_css_assemble($css) .'</pre></div>';
}

function override_css_admin_form() {
  $form = array();

  $form['override_css_bundle_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => 'Bundle your css selector groups.',
    '#default_value' => variable_get('override_css_bundle_enabled', FALSE),
  );

  $form['override_css_admin_theme'] = array(
    '#type' => 'checkbox',
    '#title' => 'Override css also in admin theme.',
    '#default_value' => variable_get('override_css_admin_theme', FALSE),
  );

  $form = system_settings_form($form);

  $form['#submit'][] = 'override_css_admin_form_submit';

  return $form;
}

function override_css_admin_form_submit($form, &$form_state) {
  menu_rebuild();
}
