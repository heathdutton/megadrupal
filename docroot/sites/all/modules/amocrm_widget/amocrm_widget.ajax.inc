<?php

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
 * Open amoCRM popup.
 */
function amocrm_widget_ajax_command_confirm_popup($callback) {}

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
