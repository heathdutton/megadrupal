<?php
namespace Drupal\go1_base\Helper\ContentRender;

/**
 * @todo  Doc & Test for $data['before'], $data['after']
 */
class Process {
  private $data;
  private $args;

  /**
   * @var ProcessCall
   */
  private $caller;

  public function __construct($data, $args) {
    $this->data = $data;
    $this->args = $args ? $args : array();

    if (!empty($data['before'])) {
      $this->caller = new ProcessCall(
        !empty($data['before']) ? $data['before'] : array()
      );
    }
  }

  public function execute() {
    !empty($this->caller) && $this->caller->callBefore();

    foreach (get_class_methods(get_class($this)) as $method) {
      if ('process' === substr($method, 0, 7)) {
        $return = $this->{$method}();
        if (!is_null($return)) {
          return $return;
        }
      }
    }

    throw new \Exception('Unsupported data structure.');
  }

  private function processFunction() {
    if (isset($this->data['function'])) {
      $func = $this->data['function'];
      return call_user_func_array($func, $this->args);
    }
  }

  private function processForm() {
    if (isset($this->data['form'])) {
      $args = array('go1_form', $this->data['form']);
      $args[] = isset($this->data['form arguments']) ? $this->data['form arguments'] : array();
      return call_user_func_array('drupal_get_form', $args);
    }
  }

  private function processController() {
    if (isset($this->data['controller'])) {
      @list($class, $method, $args) = $this->data['controller'];
      $obj = new $class();

      if (empty($args) && !empty($this->data['arguments'])) {
        $args = $this->data['arguments'];
      }

      return call_user_func_array(
        array($obj, $method),
        $this->getControllerArguments($args, $obj)
      );
    }
  }

  private function getControllerArguments($args, $obj) {
    $args = !empty($args) ? $args : array();
    if (empty($args) && method_exists($obj, 'getVariables')) {
      $args = $obj->getVariables();
    }
    return $args;
  }

  private function processTemplate() {
    if (isset($this->data['template']) || isset($this->data['template_file'])) {
      $tpl = isset($this->data['template']) ? $this->data['template'] : $this->data['template_file'];

      return is_string($tpl)
        ? $this->__templateSingle($tpl)
        : $this->__templateMultiple($tpl);
    }
  }

  /**
   * @param string $tpl
   */
  private function __templateSingle($tpl) {
    $tpl = go1_container('helper.real_path')->get($tpl);
    return go1_container('twig')->render($tpl, $this->args);
  }

  private function __templateMultiple($tpls) {
    if (is_array($tpls)) {
      foreach ($tpls as $tpl) {
        $file = go1_container('helper.real_path')->get($tpl);
        if (is_file($file)) {
          return go1_container('twig')->render($file, $this->args);
        }
      }

      throw new \Exception('No template available: ' . print_r($tpls, TRUE));
    }
  }

  private function processTemplateString() {
    $k = isset($this->data['template_string'])
          ? 'template_string'
          : (isset($this->data['content']) ? 'content' : NULL)
    ;

    if (!empty($k)) {
      $tpl = $this->data[$k];
      return trim(go1_container('twig_string')->render($tpl, $this->args));
    }
  }
}
