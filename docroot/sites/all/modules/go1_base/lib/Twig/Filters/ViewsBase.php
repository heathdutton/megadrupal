<?php

namespace Drupal\go1_base\Twig\Filters;

/**
 * Base class for drupalView twig filter, provide basic get/set methods.
 */
abstract class ViewsBase {
  protected $name;
  protected $display_id;
  protected $view;
  protected $arguments = array();
  protected $template;

  public function constructTradition() {
    $arguments = func_get_args();

    $name = array_shift($arguments);
    $display_id = isset($arguments[0]) ? array_shift($arguments) : 'default';

    $this->setName($name);
    $this->setDisplayId($display_id);
    $this->setArguments($arguments);
  }

  public function constructFancy($name, $options) {
    $this->setName($name);

    $loop = array(
      'display_id' => 'default',
      'arguments'  => NULL,
      'template'   => NULL,
      'pager'      => NULL,
    );

    foreach ($loop as $k => $default_value) {
      $method = 'set' . str_replace(' ', '', ucwords(str_replace('_', ' ', $k)));
      $value = isset($options[$k]) ? $options[$k] : $default_value;
      if (!is_null($value)) {
        $this->{$method}($value);
      }
    }
  }

  public function setName($name) {
    $this->name = $name;

    if (!$this->view = views_get_view($name)) {
      throw new \Exception('<!-- View not found: '. $this->name . ' -->');
    }
  }

  public function setDisplayId($display_id) {
    $this->display_id = $display_id;
    $this->view->set_display($display_id);

    if (!$this->view->access($this->display_id)) {
      throw new \Exception('<!-- Access denied: '. $this->name .':'. $this->display_id .' -->');
    }
  }

  public function setArguments($arguments) {
    $this->arguments = $arguments;

    if (is_array($this->arguments)) {
      $this->view->set_arguments($this->arguments);
    }
  }

  public function setTemplate($template) {
    $this->template = go1_container('helper.real_path')->get($template);
  }

  public function setPager($pager) {
    $this->view->display[$this->display_id]->display_options['pager'] = $pager;
    $this->view->display_handler->set_option('pager', $pager);

    // Just init the pager plugin if it's init before
    if (!empty($this->view->query->pager)) {
      $this->view->init_pager();
    }
  }
}
