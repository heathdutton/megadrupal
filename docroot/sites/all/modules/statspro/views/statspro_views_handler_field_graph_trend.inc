<?php
// $Id: statspro_views_handler_field_graph_trend.inc,v 1.1.2.1 2010/06/10 17:53:48 rsevero Exp $

/**
 * @file
 *   Views field handler for graph
 *
 */

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/views/statspro_views_handler_field_graph.inc';

class statspro_views_handler_field_graph_trend extends statspro_views_handler_field_graph {
  var $_last_value = -1;

  var $base_path = FALSE;

  /**
   * Table name for max. calculation
   *
   * @var string
   */
  var $base_table = 'statspro';

  var $trend_translations = FALSE;

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['display_mode'] = array(
      '#type' => 'select',
      '#title' => t('Display mode'),
      '#default_value' => $this->value,
      '#options' => array(
        'text' => t('String'),
        'graph' => t('Graph'),
        'trend' => t('Trend'),
      ),
      '#description' => t('Define the display mode for presentation.'),
    );

    $form['graph_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Graph size'),
      '#default_value' => $this->options['graph_size'],
      '#description' => t('Define the maximum width of the graph. The larges value/image will have this size. Only used with display mode <em>graph</em>'),
      '#size' => 10,
      '#field_suffix' => t('px'),
    );
  }

  function render($values) {
    $estimation = $values->{$this->field_alias};
    $rc = '-';
    switch ($this->options['display_mode']) {
      case 'graph':
        if ($estimation) {
          $rc = $this->get_bar_graph($values);
        }
        break;

      case 'trend':
        $mode = $this->_last_value;
        if ($estimation >= 0) {
          $trend = $this->get_current_trend($values);
          if (!empty($trend)) {
            $rc = sprintf(
              '<img src="%s/images/trend_%s.gif" alt="%s" title="%s" />',
              $this->get_base_path(),
              $trend,
              $this->get_trend_translation($trend),
              t('The trend is always relative to the info on the line above.')
            );
          }
        }

        if (empty($rc)) {
          if ($mode == -1) {
            if ($this->_last_value == -1) {
              $this->_last_value = 0;
            }
          }
          else {
            $rc = sprintf(
              '<img src="%s/images/trend_%s.gif" alt="%s" title="%s" />',
              $this->get_base_path(),
              'same',
              t('same'),
              t('The trend is always relative to the info on the line above.')
            );
          }
        }
        break;

      default:
        if ($estimation) {
          if (!empty($this->options['set_precision'])) {
            $estimation = number_format($estimation, $this->options['precision'], $this->options['decimal'], $this->options['separator']);
          }
          $rc = check_plain($this->options['prefix']) . $estimation . check_plain($this->options['suffix']);
        }
        break;
    }

    return $rc;
  }

  function get_current_trend($values) {
    // first run
    if ($this->_last_value == -1) {
      $rc = '';
    }
    elseif ($values->{$this->field_alias} > $this->_last_value) {
      $rc = 'up';
    }
    elseif ($values->{$this->field_alias} == $this->_last_value) {
      $rc = 'same';
    }
    else {
      $rc = 'down';
    }

    // store current value
    $this->_last_value = $values->{$this->field_alias};

    return $rc;
  }

  function get_trend_translation($trend) {
    if (!$this->trend_translations) {
      $this->trend_translations = array(
        'up' => t('up'),
        'same' => t('same'),
        'down' => t('down'),
        '' => '',
      );
    }
    $translation = isset($this->trend_translations[$trend]) ? $this->trend_translations[$trend] : '';
    return $translation;
  }
}
