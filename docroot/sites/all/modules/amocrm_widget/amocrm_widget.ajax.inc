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
function amocrm_widget_ajax_command_popup($form) {}
