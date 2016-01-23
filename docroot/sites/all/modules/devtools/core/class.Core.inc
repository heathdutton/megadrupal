<?php
namespace devtools;

class Core extends \devtools\Base {
  const module = 'devtools';

  /**
   * Triggers the specified hook in implementing components; aggregates and
   * returns data.
   *
   * TODO: find a way to dynamically pass arguments to the hook callback while preserving
   * references to get rid of the $a[1...10] monstrosity below!
   * 
   * @param  [type] $name           [description]
   * @param  [type] $default_return [description]
   * @return [type]                 [description]
   */
  public static function hook($name, $default_return = NULL, &$a1 = NULL, &$a2 = NULL, &$a3 = NULL, &$a4 = NULL, &$a5 = NULL, &$a6 = NULL, &$a7 = NULL, &$a8 = NULL, &$a9 = NULL, &$a10 = NULL) {
    // build the hook objects
      $hook_classes =& drupal_static('devtools_core');
      $module_path = drupal_get_path('module', static::module);

      if (!isset($hook_classes[static::module]) || empty($hook_classes[static::module])) {
        $hook_classes[static::module] = array();

        foreach (array_keys(devtools_components(static::module)) as $component) {
          $class_pieces = array(
            '',
            static::module,
            'components',
            $component,
            'Hooks'
          );

          $full_class_name = implode('\\', $class_pieces);

          if (class_exists($full_class_name)) {
            $hooksObj = new $full_class_name();
            $hooksObj->setPath($module_path . '/components/' . $component);
            $hooksObj->setComponentsPath($module_path . '/components');
            $hooksObj->setModulePath($module_path);
            $hooksObj->postInit();

            $hook_classes[static::module][$component] = $hooksObj;
          }
        }
      }

    // call the hook in components which declare it
      $return_data = $default_return;

      if (!empty($hook_classes[static::module])) {
        foreach ($hook_classes[static::module] as $component => $hooksObj) {
          $method = 'hook_' . $name;

          if (method_exists($hooksObj, $method)) {
            $return_value = $hooksObj->{$method}($a1, $a2, $a3, $a4, $a5, $a6, $a7, $a8, $a9, $a10);

            /*
              // this approach will not preserve arguments passed by reference; temporarily replaced by
              // the line above

              // removes the hook name & default return vaue from the arguments passed to the
              // hook call and aggregates the returned data
              $args = func_get_args();
              array_shift($args);
              array_shift($args);

              $return_value = call_user_func_array(array($hooksObj, $method), array($args));
            */
           
            if (!is_null($return_value)) {
              if (!is_array($return_data)) {
                $return_data = array();
              }

              $return_data = array_merge_recursive($return_data, $return_value);
            }
          }
        }
      }

      return $return_data;
  }
}