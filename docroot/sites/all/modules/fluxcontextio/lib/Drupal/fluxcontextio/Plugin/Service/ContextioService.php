<?php

/**
 * @file
 * Contains ContextioService.
 */

namespace Drupal\fluxcontextio\Plugin\Service;

use Drupal\fluxservice\Service\OAuthServiceBase;
use Guzzle\Service\Builder\ServiceBuilder;

/**
 * Service plugin implementation for Contextio.
 */
class ContextioService extends OAuthServiceBase implements ContextioServiceInterface {

  /**
   * Defines the plugin.
   */
  public static function getInfo() {
    return array(
      'name' => 'fluxcontextio',
      'label' => t('Contextio'),
      'description' => t('Provides Contextio integration for fluxkraft.'),
      'icon font class' => 'icon-inbox',
      'icon background color' => '#ffc666'
    );
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array &$form_state) {
    $form = parent::settingsForm($form_state);
    $form['help'] = array(
      '#type' => 'markup',
      '#markup' => t('In the following, you need to provide authentication details
      for communicating with Contextio.<br/>For that, you need to create an application
      in the <a href="https://www.contextio.com/developers/apps/create">Contextio developer home</a>,
      and provide its consumer key and secret below.'),
      '#prefix' => '<p class="fluxservice-help">',
      '#suffix' => '</p>',
      '#weight' => -1,
    );
    return $form;
  }

  /**
   * @todo: should make this a settings.
   */
  public function getPollingInterval() {
    return 300;
  }
}
