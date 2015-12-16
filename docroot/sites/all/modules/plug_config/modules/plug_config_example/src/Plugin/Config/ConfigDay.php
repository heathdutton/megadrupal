<?php

namespace Drupal\plug_config_example\Plugin\Config;

use Drupal\plug_config\Plugin\Config\DefaultConfigEntityInterface;
use Drupal\plug_config\Plugin\Config\DefaultConfigEntity;

/**
 * Class ConfigDay
 * @package Drupal\plug_config_example\Plugin\Config
 *
 * @Config(
 *   id = "config_day",
 *   label = "Config day",
 *   path = "admin/plug-config-example/days"
 * )
 */
class ConfigDay extends DefaultConfigEntity {

  /**
   * The day of the week.
   *
   * @var string
   */
  public $day;

  /**
   * {@inheritdoc}
   */
  public static function schema() {
    $schema = parent::schema();

    $schema['fields']['day'] = array(
      'description' => 'The day of the week for a item.',
      'type' => 'varchar',
      'length' => 3,
      'not null' => TRUE,
    );

    return $schema;
  }

  /**
   * {@inheritdoc}
   */
  public function form(array $form, array &$form_state, $op = 'edit') {
    $form = parent::form($form, $form_state, $op);

    $form['day'] = array(
      '#type' => 'select',
      '#title' => t('Day of the week'),
      '#options' => array(
        'SUN' => t('Sunday'),
        'MON' => t('Monday'),
        'TUE' => t('Tuesday'),
        'WED' => t('Wednesday'),
        'THU' => t('Thursday'),
        'FRI' => t('Friday'),
        'SAT' => t('Saturday'),
      ),
      '#default_value' => isset($this->day) ? $this->day : 'MON',
    );

    return $form;
  }

}
