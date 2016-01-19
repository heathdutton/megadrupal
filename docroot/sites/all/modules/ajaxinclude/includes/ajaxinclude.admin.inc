<?php
/**
 * @file
 * Off-loaded admin functions.
 */

/**
 * Off-loaded hook_form_FORM_ID_alter().
 */
function _ajaxinclude_form_alter(&$form, &$form_state, $form_id) {
  $path     = drupal_get_path('module', 'ajaxinclude');
  $block    = block_load($form['module']['#value'], $form['delta']['#value']);
  $settings = ajaxinclude_get_settings($block);

  if ($settings && is_string($settings)) {
    $settings = unserialize($settings);
  }

  $form['#attached']['css'][] = $path . '/css/ajaxinclude.admin.css';

  $form['visibility']['ajaxinclude'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('AJAX Include'),
    '#weight'      => 90,
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
    '#group'       => 'visibility',
    '#tree'        => TRUE,
    '#attributes'  => array('class' => array('form-wrapper--ajaxinclude')),
    '#description' => t("The settings for loading the block via AJAX Include Pattern. Drupal blocks normally want Element content + Method append or replace. Be sure the block.tpl supports Drupal attributes, content_attributes, title_attributes, otherwise may result empty. See <a target='_blank' href='@url'>AJAX Include Pattern</a>.", array('@url' => '//github.com/filamentgroup/Ajax-Include-Pattern/')),
  );

  $form['visibility']['ajaxinclude']['enabled'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Load block content via AJAX Include?'),
    '#default_value' => 0,
  );

  $form['visibility']['ajaxinclude']['element'] = array(
    '#type'          => 'select',
    '#title'         => t('Element'),
    '#options'       => drupal_map_assoc(array('block', 'content', 'title')),
    '#default_value' => 'content',
    '#description'   => t("What selector or element should the Method data-attributes are attached to? Default to content. If the block.tpl has no content_attributes or no title, choose the other selector here. Also adjust the Method accordingly."),
  );

  $form['visibility']['ajaxinclude']['method'] = array(
    '#type'          => 'select',
    '#title'         => t('Method'),
    '#options'       => drupal_map_assoc(array('after', 'before', 'append', 'replace')),
    '#default_value' => 'append',
    '#description'   => t("Method or placement to include the block content, see <a target='_blank' href='@url'>interaction</a>. This is relative to Element. Notice: Avoid <strong>replace</strong> if the AJAX loaded content has interative elements, or the Element itself is <strong>block</strong>, otherwise lost or broken context.", array('@url' => '//filamentgroup.github.io/Ajax-Include-Pattern/test/functional/interaction.html')),
  );

  $form['visibility']['ajaxinclude']['interaction'] = array(
    '#type'         => 'select',
    '#title'        => t('Interaction'),
    '#options'      => drupal_map_assoc(array('click', 'mouseenter')),
    '#empty_option' => t('- None -'),
    '#description'  => t("Include content when a user interacts on the Element. Be sure the Element is visible enough to interact with, such as title. ajaxInclude is blocked from executing on this element until the interaction, see <a target='_blank' href='@url'>interaction</a>.", array('@url' => '//filamentgroup.github.io/Ajax-Include-Pattern/test/functional/interaction.html')),
  );

  $form['visibility']['ajaxinclude']['target'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Target'),
    '#max_length'  => 60,
    '#description' => t("Include AJAX content on a separate element with the custom CSS selector. Be sure to provide the valid '#' or '.' notation, and its markup is in place. The Method will apply to this target instead. Leave empty to use the normal selector Element above."),
  );

  $form['visibility']['ajaxinclude']['minheight'] = array(
    '#type'         => 'textfield',
    '#title'        => t('Min height'),
    '#max_length'   => 4,
    '#description'  => t("Min height for the block element to avoid collapsing ot too short content."),
    '#field_suffix' => "px",
  );

  $form['visibility']['ajaxinclude']['media'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Media-qualified includes'),
    '#max_length'  => 60,
    '#description' => t('Include content based on a particular CSS3 Media Query, e.g.: "(min-width: 30em)" without quotes. Leave empty to make the block content rendered at all devices.'),
  );

  $form['visibility']['ajaxinclude']['class'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Class'),
    '#max_length'  => 60,
    '#description' => t('Additional class for the block wrapper.'),
  );

  $form['visibility']['ajaxinclude']['noscript'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable noscript'),
    '#default_value' => 0,
  );

  $form['visibility']['ajaxinclude']['nocache'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('No cache'),
    '#default_value' => 0,
    '#description'   => t("If checked, the block content will not be cached by browser and server sides."),
  );

  $form['visibility']['ajaxinclude']['noconcat'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Exclude from concatenation'),
    '#default_value' => 0,
    '#description'   => t("If checked, the current block loading is treated as a separate request, and excluded from a single request."),
  );

  $spinners = array(
    'chasing-dots:2'    => 'Chasing dots',
    'cube-grid:9'       => 'Cube grid',
    'double-bounce:2'   => 'Double bounce',
    'pulse:0'           => 'Pulse',
    'rotating-plane:0'  => 'Rotating plane',
    'three-bounce:3'    => 'Three bounce',
    'wandering-cubes:2' => 'Wandering cubes',
    'wave:5'            => 'Wave',
    'wordpress:1'       => 'Wordpress',
  );

  $form['visibility']['ajaxinclude']['preloader'] = array(
    '#type'         => 'radios',
    '#title'        => t('Preloader'),
    '#options'      => $spinners,
    '#empty_option' => t('- None -'),
    '#description'  => t("Select the CSS3 preloader."),
  );

  foreach ($spinners as $spin => $name) {
    list($skin, $count) = array_pad(array_map('trim', explode(":", $spin, 2)), 2, NULL);
    $attributes = array(
      'class'     => array('ajaxinclude-loader', 'ajaxinclude-loader--' . $skin),
      'data-spin' => $count,
      'data-skin' => $skin,
    );
    $spinner = '';
    if ($count) {
      foreach(range(1, $count) as $i){
        $spinner[$i] = '<span class="ai ai' . $i . '"></span>';
      }
      $spinner = implode(" ", $spinner);
    }
    $form['visibility']['ajaxinclude']['preloader'][$spin]['#field_prefix'] = '<span' . drupal_attributes($attributes) . '>' . $spinner . '</span>';
  }

  foreach (element_children($form['visibility']['ajaxinclude']) as $key) {
    if ($key != 'enabled') {
      $form['visibility']['ajaxinclude'][$key]['#states'] = array(
        'visible' => array(
          array(':input[name="ajaxinclude[enabled]"]' => array('checked' => TRUE)),
        ),
      );
    }
    $default = isset($form['visibility']['ajaxinclude'][$key]['#default_value'])
      ? $form['visibility']['ajaxinclude'][$key]['#default_value'] : '';
    $form['visibility']['ajaxinclude'][$key]['#default_value'] = isset($settings[$key]) ? $settings[$key] : $default;
  }

  $form['visibility']['ajaxinclude_old_values'] = array(
    '#type'  => 'hidden',
    '#value' => empty($block->ajaxinclude) ? NULL : $block->ajaxinclude,
  );

  $form['#submit'][] = 'ajaxinclude_form_submit';
}

/**
 * Additional submit callback for block configuration pages.
 */
function ajaxinclude_form_submit($form, &$form_state) {
  $form_id = $form_state['values']['form_id'];
  if (user_access('administer blocks') && ($form_id == 'block_admin_configure' || $form_id == 'block_add_block_form')) {
    if (isset($form_state['values']['ajaxinclude']) && !form_get_errors()) {
      $enabled = (int) $form_state['values']['ajaxinclude']['enabled'];
      $ajaxinclude = array(
        'module' => $form_state['values']['module'],
        'delta'  => $form_state['values']['delta'],
        'roles'  => array_filter($form_state['values']['roles']),
      );

      foreach ($form_state['values']['ajaxinclude'] as $key => $value) {
        $ajaxinclude[$key] = is_numeric($value) ? (int) $value : $value;
      }

      $current_values = serialize($ajaxinclude);
      if ($current_values != $form_state['values']['ajaxinclude_old_values']) {
        db_update('block')
          ->fields(array('ajaxinclude' => empty($enabled) ? NULL : $current_values))
          ->condition('module', $form_state['values']['module'])
          ->condition('delta', $form_state['values']['delta'])
          ->execute();
      }
    }
  }
}
