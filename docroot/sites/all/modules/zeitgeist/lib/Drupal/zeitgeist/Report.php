<?php
/**
 * @file
 * Report page base class for the Zeitgeist module.
 *
 * While usable, the three provided report classes should mostly be considered
 * as examples. For smarter results, build your own page with
 * Statistics::getStatistics() and the Span::* human-oriented duration
 * constants, or use the Views 3 integration as show in
 * zeitgeist_page_results_views(). Either way, this will free you from having to
 * query the database yourself.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Abstract base class for reports.
 */
abstract class Report extends Enum {
  const SIMPLE = 'simple';
  const MULTICOLUMN = 'multicolumn';
  const VIEWS = 'views';

  /**
   * @const Name of the variable for the number of days of the active report.
   */
  const VARDAYS = 'zeitgeist_page_days';

  /**
   * @const Name of the variable for the height of the report pager.
   */
  const VARHEIGHT = 'zeitgeist_page_height';

  /**
   * @var Name of the variable for the type of the active report.
   */
  const VARTYPE = 'zeitgeist_page_type';

  /**
   * @var Drupal\zeitgeist\Report
   */
  protected static $instance;

  /**
   * @var int
   *   The number of days over which to gather statistics for the report.
   */
  public $days;

  /**
   * Singleton: protected constructor.
   *
   * @param int $days
   *   The number of days over which to build a report. 0 means use default.
   */
  protected function __construct($days) {
    $this->days = $days ? $days : variable_get(static::VARDAYS, 30);
  }

  /**
   * Return the page callback for the currently configured Zeitgeist report.
   *
   * @return string
   *   The machine name of the report type to instantiate.
   */
  public static function getActiveReport() {
    $types = static::getConstants();
    $ret = variable_get(static::VARTYPE, reset($types));
    return $ret;
  }

  /**
   * Return the pager height.
   *
   * @return int
   *   The number of lines of each page of the report.
   */
  public function getHeight() {
    $ret = (int) variable_get(static::VARHEIGHT, 20);
    return $ret;
  }

  /**
   * Retrieve the singleton report instance.
   *
   * @param int $days
   *   The number of days for this report. 0 means use configured setting. Only
   *   used when building the instance initially.
   *
   * @return Drupal\zeitgeist\Report
   *   A Report (or child class) instance.
   */
  public static final function instance($days = 0) {
    if (empty(self::$instance)) {
      $class = 'Drupal\zeitgeist\Report' . drupal_ucfirst(self::getActiveReport());
      self::$instance = new $class($days);
    }

    return self::$instance;
  }

  /**
   * Page builder method for the Zeitgeist report.
   */
  public abstract function page();

  /**
   * Form helper: settings for the Zeitgeist report pages.
   *
   * @return array
   *   A hash containing:
   *   - form: the sub-form array for the module settings form.
   *   - menu_rebuild: a flag mentioning whether the menu should be rebuilt.
   */
  public function settings() {
    $ret = array(
      '#type'          => 'fieldset',
      '#title'         => t('Default zeitgeist page'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );
    $ret[static::VARHEIGHT]  = array(
      '#type'          => 'textfield',
      '#title'         => t('Height of the zeitgeist pager'),
      '#default_value' => $this->getHeight(),
      '#size'          => 4,
      '#maxlength'     => 4,
      '#element_validate' => array('element_validate_integer_positive'),
    );
    $ret[static::VARDAYS] = array(
      '#type'          => 'textfield',
      '#title'         => t('Days on the <a href="!link">Zeitgeist report</a> page', array(
        '!link' => check_url(url(ZGPATHREPORT)),
      )),
      '#size'          => 4,
      '#maxlength'     => 4,
      '#default_value' => $this->days,
      '#element_validate' => array('element_validate_integer_positive'),
    );

    $page_types = static::getConstants();
    $ret[static::VARTYPE] = array(
      '#type'          => 'radios',
      '#title'         => t('Report page type'),
      '#description'   => t('Note: the "views" default report ignores Zeitgeist settings and uses the View configuration instead.'),
      '#default_value' => $this->getActiveReport(),
      '#options'       => array_combine($page_types, $page_types),
    );

    return $ret;
  }
}
