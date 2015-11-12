<?php
/**
 * @file
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */
$plugin = array(
  'title' => t('Default mailer'),
  'description' => t('Mailer to send a standard email'),
  'class' => 'PluggableMailersMailPlugin',
);

class PluggableMailersMailPlugin
  extends \Drupal\PluggableMailers\PluggableMailPlugin
  implements \Drupal\PluggableMailers\PluggableMailInterface {

  /**
   * Do nothing
   */

}

