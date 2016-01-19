<?php

namespace Drupal\at_theming;

/**
 * Usage
 * ````
 *   $template_file = '/path/to/my/template.php';
 *   $view_name = 'my_view';
 *   $display_id = 'default_id';
 *   at_id(new \Drupal\at_theming\ViewRender($template_file, $view_name, $display_id, $arg_1, $arg_2))
 *         ->render();
 * ````
 */
class ViewRender {
  private $template_file;
  private $view_name;
  private $display_id;
  private $view_args;

  public function __construct($template_file, $view_name, $display_id = 'default') {
    $this->template_file = $template_file;
    $this->view_name = $view_name;
    $this->display_id = $display_id;

    $args = func_get_args();
    array_shift($args); // remove $template_file
    array_shift($args); // remove $view_name
    if (count($args)) {
      array_shift($args); // remove $display_id
    }
    $this->view_args = $args;
  }

  public function render() {
    return at_theming_render_template(
      $this->template_file,
      array('items' => $this->getViewResult($this->view_name, $this->display_id, $this->view_args))
    );
  }

  /**
   * Modified version of views_get_view_result().
   */
  private function getViewResult() {
    $view = views_get_view($this->view_name);
    if (is_object($view)) {
      if (is_array($this->view_args)) {
        $view->set_arguments($this->view_args);
      }

      if (is_string($this->display_id)) {
        $view->set_display($this->display_id);

        // Custom: Render clean output!
        if (!empty($view->display[$this->display_id]->display_options['fields'])) {
          foreach (array_keys($view->display[$this->display_id]->display_options['fields']) as $k) {
            $view->display[$this->display_id]->display_options['fields'][$k]['element_default_classes'] = 0;
            $view->display[$this->display_id]->display_options['fields'][$k]['element_type'] = 0;
          }
        }
      }
      else {
        $view->init_display();
      }
      $view->pre_execute();
      $view->execute();
      return $view->result;
    }
    return array();
  }
}
