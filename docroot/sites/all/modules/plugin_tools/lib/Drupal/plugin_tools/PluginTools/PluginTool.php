<?php
/**
 * @file
 *  The Plugin Tool class
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\plugin_tools\PluginTools;

/**
 * The PluginTool class encapsulates various tools for looking at plugins.
 */
class PluginTool {

  /**
   * Stores information about plugins
   *
   * @var array
   */
  public $plugins = array();

  /**
   * Stores information about plugin types
   *
   * @var array
   */
  public $plugin_types = array();

  /**
   * Load information about plugins
   *
   * This is merely a wrapper around ctools_plugin_get_plugin_type_info(), with
   * the added benefit of loading the required ctools file for us.
   *
   * @param bool $flush
   *  If TRUE, flush the ctools plugin cache
   */
  public function getPluginInfo($flush = FALSE) {
    ctools_include('plugins');
    $this->plugin_types = ctools_plugin_get_plugin_type_info($flush);
  }

  /**
   * Load information about Plugins by module and type
   *
   * This is a wrapper around ctools_get_plugins, with the benefit of loading the
   * relevant ctools include. We store the information locally in case its
   * required for reuse in this operation.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   */
  public function getPlugins($module, $type) {
    ctools_include('plugins');

    $plugins = ctools_get_plugins($module, $type);
    if (!empty($plugins)) {
      if (!isset($this->plugins[$module])) {
        $this->plugins[$module] = array();
      }
      $this->plugins[$module][$type] = $plugins;
    }
  }

  /**
   * Prepare information about a modules plugins for use in a page callback
   *
   * @param string $module
   *  The module providing the plugin
   * @param bool $flush
   *   Whether to flush the Ctools plugin cache.
   *
   * @return bool|string
   *  A human-readable representation of information about a module's plugins,
   * or FALSE if no module plugins exist.
   */
  public function pluginModuleOverview($module, $flush = FALSE){
    if (!isset($this->plugin_types[$module])) {

      $this->getPluginInfo($flush);
      if (!isset($this->plugin_types[$module])) {

        return FALSE;
      }
    }

    $header = array(t('Type'), t('Available plugins'), t('Options'));
    $rows = array();

    foreach ($this->plugin_types[$module] as $type => $info) {
      $count = $this->countPlugins($module, $type);

      $rows[] = array(
        $type,
        ($count == 0 ? t('None') : $count),
        l('List plugins', 'admin/config/system/plugins/' . $module .'/'. $type),
      );
    }

    $vars = array(
      'header' => $header,
      'rows' => $rows,
    );

    return theme('table', $vars);
  }

  /**
   * Prepare information about the plugins available for a type.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   *
   * @return bool|string
   *  A human-readable representation of information about a module's plugins,
   * or FALSE if no module plugins exist.
   */
  public function pluginTypeDetails($module, $type) {
    $output = FALSE;

    if (!$this->pluginTypeExists($module, $type)) {
      return FALSE;
    }

    if (!empty($this->plugin_types[$module][$type])) {
      $output = '<strong>' . t('Plugin settings') . '</strong>';
      drupal_add_css(drupal_get_path('module', 'plugin_tools') . '/plugin_tools.css');

      $type_output = '';
      foreach ($this->plugin_types[$module][$type] as $key => $value) {
        $type_output .= '<div class="table-row">';
        $type_output .= '<div class="table-cell label">' . $key . '</div>';
        if (is_array($value)) {
          $value = print_r($value, TRUE);
        }
        if (empty($value)) {
          $value = '<em>' . t('No value') . '</em>';
        }
        else {
          $value = check_plain($value);
        }
        $type_output .= '<div class="table-cell no-border">' . $value . '</div>';
        $type_output .= '</div>';
      }

      $output .= '<div class="plugin-tools-type-info">' . $type_output . '</div>';

      $output .= '<strong>' . t('Plugin sources') . '</strong>';
      $directories = ctools_plugin_get_directories($this->plugin_types[$module][$type]);
      if (!empty($directories)) {
        $dir_output = '';
        foreach ($directories as $module => $directory) {
          $dir_output .= '<div class="table-row">';
          $dir_output .= '<div class="table-cell label">' . $module . '</div>';
          $dir_output .= '<div class="table-cell no-border">' . check_plain($directory) . '</div>';
          $dir_output .= '</div>';
        }
      }
      else {
        $dir_output = t('No mapped directories providing plugin\'s.');
      }

      $output .= '<div class="plugin-tools-type-info">' . $dir_output . '</div>';
    }

    if (!empty($output)) {
      return $output;
    }

    return FALSE;
  }

  /**
   * Prepare information about the plugins available for a type.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   *
   * @return bool|string
   *  A human-readable representation of information about a module's plugins,
   * or FALSE if no module plugins exist.
   */
  public function pluginList($module, $type) {
    if (!$this->pluginTypeExists($module, $type)) {
      return FALSE;
    }

    $header = array(t('Name'), t('Machine name'), t('Description'), t('Provider'), t('Options'));
    $rows = array();

    if (!empty($this->plugins[$module])) {
      foreach ($this->plugins[$module][$type] as $name => $info) {

        $title = $this->getPluginTitle($module, $type, $name);
        $desc = $this->getPluginDescription($module, $type, $name);
        $provider = plugin_tools_project_human_name($this->getPluginProvider($module, $type, $name));

        $rows[$name] = array(
          $title,
          $name,
          $desc,
          $provider . ' (' . $this->getPluginProvider($module, $type, $name) .')',
          '',
        );
      }

      ksort($rows);
    }


    $vars = array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('No plugin\'s found for this type.')
    );

    return '<div class="plugin-tools-list"><strong>' . t('Plugins') . '</strong> ' . t('(!count found)', array('!count' => count($this->plugins[$module][$type]))) . theme('table', $vars) . '</div>';
  }

  /**
   * Check if a module specifies any plugins
   *
   * @param string $module
   *  The module providing the plugin
   *
   * @return bool
   *  TRUE, if the module specifies plugins, or FALSE
   */
  public function moduleHasPlugins($module) {

    if (!isset($this->plugin_types[$module])) {

      $this->getPluginInfo();

      if (!isset($this->plugin_types[$module])) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Ensure that plugin type exists.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   *
   * @return bool
   *  TRUE, if the plugin type exists, or FALSE
   */
  public function pluginTypeExists($module, $type) {

    if (!isset($this->plugin_types[$module][$type])) {

      $this->getPluginInfo();

      if (!isset($this->plugin_types[$module][$type])) {

        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Ensure that plugin type exists and has plugins declared.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   *
   * @return bool
   *  TRUE, if the plugin type exists, or FALSE
   */
  public function hasDeclaredPlugins($module, $type) {

    if (!isset($this->plugins[$module]) || !isset($this->plugins[$module][$type])) {

      $this->getPlugins($module, $type);
      if (!isset($this->plugins[$module]) || !isset($this->plugins[$module][$type])) {

        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Check if a specific plugin exists
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   * @param string $name
   *  The name of the plugin
   *
   * @return bool
   *  TRUE, if the plugin type exists, or FALSE
   */
  public function pluginExists($module, $type, $name) {

    if (!isset($this->plugins[$module]) || !isset($this->plugins[$module][$type]) || !isset($this->plugins[$module][$type][$name])) {

      $this->getPlugins($module, $type);
      if (!isset($this->plugins[$module]) || !isset($this->plugins[$module][$type]) || !isset($this->plugins[$module][$type][$name])) {

        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Get information about a plugin
   *
   * In normal usage, its more efficient just to use ctools to do this. We retain
   * it mostly for internal use, though it does have the benefit of autoloading
   * ctools for you.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   * @param string $name
   *  The name of the plugin
   *
   * @return bool|array
   *  An array of plugin information, if the plugin type exists, or FALSE
   */
  public function getPlugin($module, $type, $name) {
    if ($this->pluginExists($module, $type, $name)) {
      return $this->plugins[$module][$type][$name];
    }

    return FALSE;
  }

  /**
   * Count the number of plugins of a given type
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   *
   * @return int
   *  The number of plugins found.
   */
  public function countPlugins($module, $type) {
    if ($this->hasDeclaredPlugins($module, $type)) {
      return count($this->plugins[$module][$type]);
    }

    return 0;
  }

  /**
   * Get the title of a plugin.
   *
   * Different plugins represent this in different ways. This function makes a
   * best-guess on the title, and if not found returns the machine name.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   * @param string $name
   *  The name of the plugin
   *
   * @return bool|array
   *  A string representing a title, if the plugin type exists, or FALSE.
   */
  public function getPluginTitle($module, $type, $name) {
    $info = $this->getPlugin($module, $type, $name);
    if (!empty($info)) {
      if (isset($info['title'])) {
        return $info['title'];
      }
      elseif (isset($info['label'])) {
        return $info['label'];
      }
      elseif (isset($info['name'])) {
        return $info['name'];
      }

      return $name;
    }

    return FALSE;
  }

  /**
   * Get the description for a plugin.
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   * @param string $name
   *  The name of the plugin
   *
   * @return bool|string
   *  A string representing the description, if found and if the plugin type
   * exists, or FALSE
   */
  public function getPluginDescription($module, $type, $name) {
    $info = $this->getPlugin($module, $type, $name);
    if (!empty($info)) {
      if (isset($info['description'])) {
        return $info['description'];
      }

      return '';
    }

    return FALSE;
  }

  /**
   * Get the machine name of the provider of a plugin
   *
   * @param string $module
   *  The module providing the plugin
   * @param string $type
   *  The type of plugin
   * @param string $name
   *  The name of the plugin
   *
   * @return bool|array
   *  The machine name of the plugin, if the plugin type exists, or FALSE
   */
  public function getPluginProvider($module, $type, $name) {
    $info = $this->getPlugin($module, $type, $name);
    if (!empty($info)) {
      return $info['module'];
    }

    return FALSE;
  }

}
