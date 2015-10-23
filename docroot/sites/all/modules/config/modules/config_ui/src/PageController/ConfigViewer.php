<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\config_ui\PageController;

use Drupal\config\Config;
use Drupal\config\ConfigManager\ConfigManager;
use Drupal\config\Parser\ParserManager;
use Drupal\ghost\Page\PageController;
use Drupal\ghost\UserInterface\Component\ItemTable;

/**
 * Class ConfigViewer
 * @package Drupal\config_ui\PageController
 */
class ConfigViewer extends PageController {

  /**
   * Callback to view admin/config/development/config.
   */
  public function listAll() {
    $configs = ConfigManager::create()->listAllConfig();

    $item_table = ItemTable::init();
    $item_table->setColumns(array(
      'name' => t('Name'),
      'description' => t('Description'),
      'provider' => t('Module'),
      'op' => t('Operations'),
    ));

    foreach ($configs as $config_name => $config) {

      $operations = array(
        l(t('view'), 'admin/config/development/config/view/' . $config_name),
      );

      $item = array(
        'name' => check_plain($config['name']),
        'description' => check_plain($config['description']),
        'provider' => check_plain($config['module']),
        'op' => implode(' | ', $operations),
      );

      $item_table->setItem($config_name, $item);
    }

    $item_table->setEmpty(t('No configurations found'));

    return $item_table->render();
  }

  /**
   * Callback to view admin/config/development/config/view/%config_id.
   */
  public function view($item) {

    drupal_set_title(check_plain($item['name']));

    $output = '<h2>Settings</h2>';

    $output .= $this->renderConfig($item);

    $set = Config::load()->getConfigSet($item['name']);

    foreach ($set['files'] as $key => $file) {
      $output .= '<h2>' . check_plain($set['files'][$key]['name']) . '</h2>';
      $output .= $this->renderConfig($file['config']);
    }

    return $output;
  }

  /**
   * Render some config.
   *
   * @param array $item
   *   The configuration
   *
   * @return string
   *   Rendered readable.
   */
  protected function renderConfig($item) {

    return("<pre>".print_r($item, TRUE)."</pre>");
  }
}
