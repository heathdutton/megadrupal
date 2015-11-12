<?php
// @fixme hay un potencial problema. si una regla de visualizacion dice que un pane en concreto no se va a mostrar, tenemos que reflejar eso en los tabs. Ahora mismo los estamos imprimiendo a lo gogo
/**
 * Definición del plugin
 */
$plugin = array(
  'title' => t('As tabs'),
  'description' => t('Renderiza una región como tabs'),
  'render region' => 'preprocess_region_tabs',
  'settings form' => 'region_tabs_settings_form',
  'hook theme' => array(
    'region_tabs' => array(
      'variables' => array(
        'tabs' => NULL,
        'content' => NULL,
        'id' => NULL
      ),
      'template' => 'region-tabs',
    ),
  ),
);

/**
 * Render
 */
function theme_preprocess_region_tabs($vars) {

  // @todo add the possibility to select another js tab system. ie: twitter bootstrap.
  // drupal_add_library('system', 'ui.tabs');
  // De momento todo funciona porque el tpl de este plugin cumple con twitter bootstrap, ergo se crean los tabs.

  return theme('region_tabs', array(
    'tabs' => $vars['settings'],
    'content' => array_values($vars['panes']),
    'id' => $vars['region_id']
  ));

}


/**
 * Settings form callback.
 */
function region_tabs_settings_form($conf, $display, $pid, $type, $form_state) {
  $cnt_panes = count($display->panels[$form_state['pid']]);
  $form = array();

  for ($i = 0; $i < $cnt_panes; $i++) {

    $form['tab_' . $i] = array(
      '#type' => 'textfield',
      '#title' => t('Name for tab ' . $i),
      '#default_value' => (isset($conf['tab_' . $i])) ? $conf['tab_' . $i] : t('Untitled tab'),
    );

  }

  return $form;
}