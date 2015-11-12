<?php

/**
 * @file
 * Theme functions for Whos.amung.us module.
 *
 * @author Francisco J. Cruz Romanos <grisendo@gmail.com>
 */

/**
 * Theme function for the Whos.amung.us widget.
 */
function theme_whos_amung_us($variables) {

  $widget = $variables['widget'];
  $conf = $variables['options'];
  $id = $variables['id'];

  switch ($widget) {

    case 'general':
      $conf = variable_get('whos_amung_us_configuration', whos_amung_us_default_form_values());
      if ($conf['widget'] == 'general') {
        drupal_set_message(t('Infinite loop avoided.'));
        drupal_goto('<front>');
      }
      else {
        return theme(
          'whos_amung_us',
          array(
            'widget' => $conf['widget'],
            'options' => $conf,
            'id' => $id,
          )
        );
      }
      break;

    case 'colored':
      $options = ', "' . $conf['background'] . $conf['foreground'] . '"';
      break;

    case 'tab':
      $options = ', "' . $conf['position'] . '"';
      break;

    case 'map':
      $options = ', "' . $conf['width'] . '", "' . $conf['height'] . '", "' . $conf['map'] . '", "' . $conf['marker'] . '"';
      break;

    default:
      $options = '';
      break;

  }

  ob_start();
  $idshort = $id;
  if (isset($variables['api_key']) && $variables['api_key']) {
    $idlong = $variables['api_key'];
  }
  else {
    $idlong = variable_get('whos_amung_us_key', '');
  }
  $output = '<script id="_wau' . $idshort . '">var _wau = _wau || []; _wau.push(["' . $widget . '", "' . $idlong . '", "' . $idshort . '"' . $options . ']);';
  $output .= '(function() {var s=document.createElement("script"); s.async=true;';
  $output .= 's.src="http://widgets.amung.us/' . $conf['widget'] . '.js";';
  $output .= 'document.getElementsByTagName("head")[0].appendChild(s);';
  $output .= '})();</script>';
  return $output;

}
