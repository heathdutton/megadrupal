## Introduction 

This module is for developers. It provides cTools plugins for common `_info` types of functionality.

For example, it allows you to define a core Drupal block the same way as you would define a cTools `content_type` plugin or a `context` one to use with Panels.

It aims at clearing a bit the mess that ensues when a site needs to define many such blocks or other `_info` based extensions. I always hated the switch statements that cluttered the `hook_block_*` or `hook_field_formatter_*` implementations.

Installing this module will allow you to specify which type of plugin you want enabled (you may not use all of them so no sense including their files) and then define new plugins that contain in one file (or folder) all the logic for that particular plugin. Very clean and organized.

## Documentation

The module creates a new permission called `Administer Info Plugin Types`. After installing the module and adding it to a responsible user role, navigate to `admin/config/info-plugins` and select which plugin types you want enabled. If you don't yet need to define any custom filters, for instance, keep it disabled so that its file does not get included triggering unnecessary hook implementations. 

The steps to define a new plugin are similar to when you define `content_type` plugins to use with Panels:

First, implement `hook_ctools_plugin_directory()` and specify which folder of your module will contain plugins of this type. Recommended is to do the following which will take care of all the plugin types in one go:

```
/**
 * Implements hook_ctools_plugin_directory().
 */
function module_name_ctools_plugin_directory($module, $plugin) {
  if ($module == 'info_plugins' && in_array($plugin, array_keys(info_plugins_ctools_plugin_type())) ) {
    return 'plugins/' . $plugin;
  }
}
```
 
This way any plugin type defined by the `info_plugins` module will be found inside the `plugins/` folder of your module. 

Second, create the plugin file like you normally would inside the `plugins/[plugin-type-name]/` folder with an `.inc` extension. Do make sure though that if you create a plugin of a certain type, you don't reuse that exact plugin name (file name) for another plugin (of the same type or otherwise) within the same module.

Inside this file, make sure you have a `$plugin` array containing some meta information about the plugin + **all the values with their respective keys you need for the type of _info hook this plugin replaces**. For example, for a Block plugin, you could do something like this:

```
$plugin = array(
  'title' => 'My block tile',
  'cache' => DRUPAL_CACHE_PER_PAGE,
);
```

Basically in here you put the all the info you would normally return inside the `_info` hook.

Third, you'll most likely have to write some callback functions that handle the logic of the plugin. You can reference the callback function names in the `$plugin` array or use the automatically *guessed* function name. Since this is relative to the individual plugin types, please consult the example plugins included in the module found inside the `plugins_example` folder. 
