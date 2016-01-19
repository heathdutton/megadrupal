<?php
/**
 * @file
 * Context reaction plugin for Taboola escalator.
 */

class taboola_x_context_reaction extends context_reaction {
  /**
   * Form options.
   */
  public function options_form($context) {
    $form = array();

    $form['values'] = array(
      '#type' => 'fieldset',
    );
    $form['values']['enabled'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
    $form['values']['info'] = array(
      '#markup' => t('Taboola-X javascript will be added to the page.'),
    );

    if (is_null(taboola_get_setting('taboola_x_service_path'))) {
      $form['values']['script']['#markup'] .= PHP_EOL . t('Please configure !link.',
        array(
          '!link' => '<a href="/admin/config/system/taboola">Taboola-X Service path</a>',
        )
      );
    }

    return $form;
  }

  /**
   * Execute context.
   */
  public function execute() {
    foreach ($this->get_contexts() as $context) {
      if (isset($context->reactions['taboola_x']['values'])) {

        $service_path = taboola_get_setting('taboola_x_service_path');

        if (is_null($service_path)) {
          drupal_set_message(t('Please configure !link.',
            array(
              '!link' => '<a href="/admin/config/system/taboola">Taboola-X Service path</a>',
            )
          ), 'warning');

          // Early return.
          return;
        }

        if (!path_is_admin(current_path()) && taboola_get_setting('taboola_enabled')) {
          $service_path = check_plain($service_path);
          $js = array(
            "!function (e, f, u) { e.async = 1; e.src = u; f.parentNode.insertBefore(e, f);}",
            "(document.createElement('script'), document.getElementsByTagName('script')[0], '$service_path');",
          );

          // Add inline JS.
          drupal_add_js(implode(PHP_EOL, $js), array(
            'scope' => 'header',
            'type' => 'inline',
            'weight' => 2,
          ));
        }
      }
    }
  }

}
