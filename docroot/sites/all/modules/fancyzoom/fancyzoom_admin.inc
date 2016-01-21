<?php
/**
 * @file
 * Fancyzoom administration form
 */

function fancyzoom_settings() {
  drupal_set_title(t('FancyZoom Settings'));
  return drupal_get_form('fancyzoom_settings_form');
}
function fancyzoom_settings_form() {
  $form = array(
    array(
      '#type' => 'markup',
      '#markup' => '<div class="form-item">'
        . '<label>' . t('Test FancyZoom') . ':</label> '
        . l('<img src="/misc/druplicon.png" width="32" />',
            'misc/druplicon.png',
            array('html' => 1, 'attributes' => array('title' => t('It Works!')))
          )
        . '</div>'
    ),
    'fancyzoom_zoomTime' => array(
      '#title' => t('Zoom Time'),
      '#type' => 'textfield',
      '#default_value' => _fancyzoom_var('zoomTime'),
      '#required' => 1,
      '#maxlength' => 3,
      '#description' => t('Milliseconds between frames of zoom animation.')
    ),
    'fancyzoom_zoomSteps' => array(
      '#title' => t('Zoom Steps'),
      '#type' => 'textfield',
      '#default_value' => _fancyzoom_var('zoomSteps'),
      '#required' => 1,
      '#maxlength' => 3,
      '#description' => t('Number of zoom animation frames.')
    ),
    'fancyzoom_minBorder' => array(
      '#title' => t('Minimum Border'),
      '#type' => 'textfield',
      '#default_value' => _fancyzoom_var('minBorder'),
      '#required' => 1,
      '#maxlength' => 3,
      '#description' => t('Minimum padding between a zoomed image and the window edges.')
    ),
    'fancyzoom_includeFade' => array(
      '#title' => t('Use Fade Effect'),
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => _fancyzoom_var('includeFade'),
      '#description' => t('Fade the image in / out as it zooms.')
    ),
    'fancyzoom_includeCaption' => array(
      '#title' => t('Show Caption'),
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => _fancyzoom_var('includeCaption'),
      '#description' => t('The link\'s title can appear below the zoomed image as a caption.')
    ),
    'fancyzoom_showClosebox' => array(
      '#title' => t('Show Close Box'),
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => _fancyzoom_var('showClosebox'),
      '#description' => t('The Close Box is drawn over the top-left corner of the image.')
    ),
    'fancyzoom_shadowColor' => array(
      '#title' => t('Shadow Color'),
      '#type' => 'textfield',
      '#default_value' => _fancyzoom_var('shadowColor'),
    ),
    'fancyzoom_scriptType' => array(
      '#title' => t('Script To Use'),
      '#type' => 'select',
      '#options' => array(
        'jquery-min' => t('JQuery Minimized'),
        'jquery' => t('JQuery'),
        'standard-min' => t('Standard Minimized'),
        'standard' => t('Standard')),
      '#default_value' => _fancyzoom_var('scriptType'),
      '#description' => t('Always use the Minimized JQuery version unless you\'re having conflicts.')
    ),
    'fancyzoom_zoomImagesURI' => array(
      '#title' => t('Zoom Images Path (!path...)', array('!path' => "http://$_SERVER[HTTP_HOST]" . base_path())),
      '#type' => 'textfield',
      '#default_value' => _fancyzoom_var('zoomImagesURI'),
      '#required' => 0,
      '#maxlength' => 300,
      '#description' => t('Location of the zoom images. Leave blank for default images. <em>$t = theme folder, $m = module folder</em>')
    )
  );

  if (module_exists('colorpicker')) {
    $form['fancyzoom_shadowColor']['#type'] = 'colorpicker_textfield';
    $form['fancyzoom_shadowColor']['#colorpicker'] = 'shadow_picker';
    $form['shadow_picker'] = array(
      '#type' => 'colorpicker',
      '#description' => t('Select a color in RGB format. Applies only to CSS3-capable browsers.')
    );
  }

  return system_settings_form($form);
}

function fancyzoom_settings_form_validate($form, &$form_state) {

  $v =& $form_state['values'];

  $k = 'fancyzoom_shadowColor';
  if (!preg_match('/^#(?:(?:[a-f\d]{3}){1,2})$/i', $v[$k])) {
    form_set_error($k, t('%val is not a valid hex color', array('%val' => $v[$k])));
  }

  $k = 'fancyzoom_minBorder';
  if ($v[$k] >= 100) {
    form_set_error($k, t('Minimum Border must be less than 100.'));
  }
  elseif ($v[$k] < 0) {
    form_set_error($k, t('Minimum Border must be 0 or higher.'));
  }

  $k = 'fancyzoom_zoomImagesURI';
  $img = $v[$k];
  if (strpos($img, '$m') > -1 && strpos($img, '$t') > -1) {
    form_set_error($k, t('You can use either $t or $m but not both.'));
  }

}

function _fancyzoom_help() {
  $target = array('target' => '_blank');
  $base = base_path();
  $args = array(
    '!url1' => l('Cabel Sasser / Panic Inc.', 'http://www.cabel.name/', $target),
    '!url2' => l('Thinkyhead', 'http://www.thinkyhead.com/design/fancyzoom', $target),
    '!url3' => l('www.fancyzoom.com', 'http://www.fancyzoom.com/', $target + array('style' => 'color:red;text-decoration:underline'))
  );
  return t('<p style="font-size:small"><b>FancyZoom ' . _fancyzoom_version() . '</b><br />Drupal Module &copy; 2014 !url2<br />FancyZoom 1.1 &copy; 2009 !url1<br /><span style="color:red">&gt;&gt;For commercial sites please visit !url3 to purchase a license.&lt;&lt;</span></p>', $args);
}
