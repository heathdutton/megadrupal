<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2014 Christopher Skene
 */

namespace Drupal\ghost\UserInterface\Component;


/**
 * Class PluginTable
 * @package Drupal\ghost\UserInterface\Component
 */
class PluginTable extends ItemTable {

  /**
   * Render a table of plugin information.
   *
   * @param array $plugins
   *   An array of plugin information.
   *
   * @return string
   *   A rendered table.
   */
  public function tableElement($plugins) {

    $columns = array(
      'title' => t('Title'),
      'description' => t('Description'),
      'provider' => t('Provider'),
      'visibility' => t('Visibility'),
    );

    $this->setColumns($columns);

    foreach ($plugins as $index => $plugin) {
      $row = array(
        'title' => $plugin['title'],
        'description' => $plugin['description'],
        'provider' => $plugin['module'],
        'visibility' => isset($plugin['hidden']) && $plugin['hidden'] == TRUE ? t('Hidden') : t('Default'),
      );

      $this->setItem($index, $row);
    }

    $this->setEmpty('No plugins found.');
  }


}
