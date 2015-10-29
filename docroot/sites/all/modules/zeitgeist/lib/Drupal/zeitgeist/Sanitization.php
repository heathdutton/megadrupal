<?php
/**
 * @file
 * The class handling display and recording sanitization.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Point of sanitization.
 */
class Sanitization extends Enum {
  /**
   * @const Do not perform search sanitization.
   */
  const NO = 0;

  /**
   * @const Sanitize on recording.
   */
  const RECORD = 1;

  /**
   * @const Sanitize on display (Drupal default approach).
   */
  const DISPLAY = 2;

  /**
   * @const Default validation type is to validate upon display.
   */
  const DEFVALIDATION = Sanitization::DISPLAY;

  /**
   * @const Name of the variable holding the "Record empty searches" setting.
   */
  const VARRECORDEMPTY = 'zeitgeist_record_empty';

  /**
   * @const Name of configuration variable for handling of HTML search strings.
   */
  const VARVALIDATION = 'zeitgeist_validation';

  /**
   * Delegated implementation of the old settings hook.
   */
  public static function settings() {
    $ret = array(
      '#type'          => 'fieldset',
      '#title'         => t('Search filtering'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );
    $ret[Sanitization::VARVALIDATION] = array(
      '#type'          => 'radios',
      '#title'         => t('How should search strings be validated to prevent spamming ZG blocks ?'),
      '#default_value' => Sanitization::getValidationType(),
      '#options'       => array(
        Sanitization::NO      => t('No validation performed: record and display every search'),
        Sanitization::RECORD  => t('Do not record searches containing HTML'),
        Sanitization::DISPLAY => t('Do not display searches containing HTML, but store them'),
      ),
      '#description'   => t('<p>Recommended value is "Do not record HTML": spammers may abuse ZG by submitting links via scripts.</p>'),
    );
    $ret[Sanitization::VARRECORDEMPTY] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Should Zeitgeist record empty searches ?'),
      '#default_value' => static::isEmptySearchRecorded(),
      '#description'   => t('Zeitgeist normally records empty searches, but some of them can be spurious and not represent actual searches, but just clicks on search links by spiders, and some webmasters can wish to ignore them. Whatever this setting, they are never displayed in the "latest searches" block.'),
    );

    return $ret;
  }

  /**
   * Checks whether the search passes validation (anti-spam).
   *
   * @param string $keys
   *   The search query.
   * @param string $stage
   *   save | view
   *
   * @return bool
   *   Validation result.
   */
  public static function validate($keys, $stage) {
    $option = static::getValidationType();
    $html = ($keys != strip_tags($keys));
    $ret = TRUE;

    switch ($stage) {
      case 'save':
        if ($html && $option == Sanitization::RECORD) {
          $ret = FALSE;
        }
        break;

      case 'view':
        if ($html && $option == Sanitization::DISPLAY) {
          $ret = FALSE;
        }
        break;

      default:
        $ret = TRUE;
    }
    return $ret;
  }

  /**
   * Return the currently configured sanitization type.
   *
   * @return int
   *   self::NO|self::RECORD|self::DISPLAY
   */
  public static function getValidationType() {
    return variable_get(Sanitization::VARVALIDATION, Sanitization::DEFVALIDATION);
  }

  /**
   * Decide whether empty searches should be recorded.
   *
   * @return bool
   *   Are empty searches recorded ?
   */
  public static function isEmptySearchRecorded() {
    $ret = variable_get(Sanitization::VARRECORDEMPTY, 1);
    return $ret;
  }
}
