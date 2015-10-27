<?php

/**
 * @file
 * Contains a
 *
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 * @copyright Copyright(c) 2015 Christopher Skene
 */

namespace Drupal\globals\Controller;

use Drupal\ghost\Page\PageController;
use Drupal\ghost\Page\PageControllerInterface;
use Drupal\globals\Globals;
use Drupal\globals\GlobalItem;

/**
 * Class Main
 *
 * @package Drupal\globals\Controller
 */
class Main extends PageController implements PageControllerInterface {

  /**
   * View callback.
   *
   * @return string
   *   The output
   */
  public function view() {

    drupal_add_css(drupal_get_path('module', 'globals') . '/css/globals.css');

    $output = array();

    $defined_globals = Globals::init()->getGlobalProperties();

    $output[] = '<div class="global-items">';

    $output[] = '<div class="global-items-header">';
    $output[] = '<div>' . t('Name') . '</div>';
    $output[] = '<div>' . t('Description') . '</div>';
    $output[] = '<div>' . t('Default') . '</div>';
    $output[] = '<div>' . t('Value') . '</div>';
    $output[] = '<div>' . t('Operations') . '</div>';
    $output[] = '</div>';

    if (!empty($defined_globals)) {
      foreach ($defined_globals as $key => $global) {

        /* @var GlobalItem $global */

        if ($global->isHidden()) {
          continue;
        }

        $default = '';
        if ($global->getDefaultValue()) {
          $default = '<em>' . $global->getDefaultValue() . '</em>';
        }

        $value = $global->getValue();
        if ($value == $global->getDefaultValue()) {
          $value = $default;
        }

        $output[] = '<div class="global-item global-item-' . $key . '"">';
        $output[] = '<div class="global-name">' . check_plain($global->getName()) . '</div>';
        $output[] = '<div class="global-description">' . filter_xss_admin($global->getDescription()) . '</div>';
        $output[] = '<div class="global-default">' . filter_xss_admin($default) . '</div>';
        $output[] = '<div class="global-value">' . filter_xss_admin($value) . '</div>';
        $output[] = '<div class="global-ops">' . l(t('Edit'), '/admin/structure/globals/edit/' . $key) . '</div>';
        $output[] = '</div>';
      }
    }

    $output[] = '</div>';

    return implode('', $output);
  }
}
