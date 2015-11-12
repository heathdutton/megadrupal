<?php
/**
 * @file
 * PgbarSource plugin that displays a fixed count.
 */

$plugin = array(
  'label' => t('Manual count'),
  'handler' => array('class' => 'PgbarSourceNull'),
);

class PgbarSourceNull {
  /**
   * Value will be set by the "offset" setting.
   */
  public function getValue($item) {
    return 0;
  }
  /**
   * We don't need any extra configuration.
   */
  public function widgetForm($item) {
    return NULL;
  }
}
