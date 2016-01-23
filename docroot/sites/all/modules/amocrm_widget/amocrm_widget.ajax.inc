<?php

/**
 * @file
 * Define useful wrappers for ajax commands.
 */

/**
 * Open amoCRM form in popup.
 */
function amocrm_widget_ajax_command_open_form($form) {
  $form = amocrm_widget_prepare_form($form);

  return array(
    'command' => 'amocrmWidgetOpenForm',
    'form' => $form,
  );
}

/**
 * Reload amoCRM page.
 */
function amocrm_widget_ajax_command_reload() {
  return array(
    'command' => 'amocrmWidgetReload',
  );
}

/**
 * Auto resize widget block.
 */
function amocrm_widget_ajax_command_resize() {
  return array(
    'command' => 'amocrmWidgetResize',
  );
}
