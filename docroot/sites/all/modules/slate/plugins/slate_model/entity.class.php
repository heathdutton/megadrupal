<?php
/**
 * @file
 */


class SlateModelPluginEntity extends SlateModelPlugin {

  public function requiredContexts() {
    return array(new ctools_context_required($this->model['label'], 'entity:' . $this->plugin['entity_type']));
  }

  public function wrapper($data) {
    return new SlateModelPluginEntityWrapperStructure(entity_metadata_wrapper($this->plugin['entity_type'], $data));
  }

}

class SlateModelPluginEntityWrapperBase {

  protected $data;

  public function __construct($data) {
    $this->data = $data;
  }

  public function __call($name, $arguments) {
    if (is_callable(array($this->data, $name))) {
      return call_user_func_array(array($this->data, $name), $arguments);
    }
  }

  public function __get($name) {
    return $this->data->{$name};
  }

  public function __isset($name) {
    if (!isset($this->data->{$name})) {
      return FALSE;
    }

    return $this->data->{$name}->value() !== NULL;
  }
}

class SlateModelPluginEntityWrapperStructure extends SlateModelPluginEntityWrapperBase {

  protected $fields;

  public function __get($name) {
    if (!isset($this->fields[$name])) {
      if ($this->data->{$name} instanceof EntityDrupalWrapper) {
        $this->fields[$name] = new SlateModelPluginEntityWrapperStructure($this->data->{$name});
      }
      else if ($this->data->{$name} instanceof EntityStructureWrapper) {
        $this->fields[$name] = new SlateModelPluginEntityWrapperStructure($this->data->{$name});
      }
      else if ($this->data->{$name} instanceof EntityValueWrapper) {
        $this->fields[$name] = new SlateModelPluginEntityWrapperValue($this->data->{$name});
      }
      else {
        $this->fields[$name] = $this->data->{$name};
      }
    }
    return $this->fields[$name];
  }

}

class SlateModelPluginEntityWrapperValue extends SlateModelPluginEntityWrapperBase {

  public function __toString() {
    return $this->data->value();
  }

}
