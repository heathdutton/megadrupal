<?php

class TemplateApiMustacheRenderer extends TemplateApiRenderer {
  /**
   * Render the template
   *
   * @param Template
   * @param array $data the data to render in the template
   * @return array a render array
   */
  public function render(Template $template, $data) {
    require_once self::mustache_path();
    $m = new TemplateApiMustache();
    $m->set_data($data);
    return array(
      '#markup' => $m->render($template->content, $data),
    );
  }

  /**
   * Verify anything before rendering
   * @param Template
   * @param array $data the data to render in the template
   * @return boolean
   */
  public function valid(Template $template, $data) {
    // Does the mustache library exist?
    if (file_exists(self::mustache_path())) {
      return TRUE;
    }

    watchdog('template_api', t('Mustache Library was not found'));
    return FALSE;
  }

  public static function mustache_path() {
    return libraries_get_path('mustache') . '/Mustache.php';
  }
}