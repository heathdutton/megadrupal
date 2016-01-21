<?php
/**
 * @file
 * Encoding helpers for link_api
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\link_api\LinkApi;

use Drupal\link_api\Extras\Encoding;

/**
 * Class EncodingHelper
 * @package Drupal\link_api\LinkApi
 */
class EncodingHelper {

  /**
   * Normalise encoding.
   *
   * @param string $text
   *   The text to normalise.
   * @param string $source
   *   (optional) The source encoding. Defaults to WINDOWS-1252.
   * @param string $destination
   *   (optional) The destination encoding. Defaults to UTF-8, as this is the
   *    most common usage on Drupal sites.
   *
   * @return string
   *   The normalised text.
   */
  public function normaliseEncoding(&$text, $source = 'WINDOWS-1252', $destination = 'UTF-8') {

    // If the content is not UTF8, we assume it's WINDOWS-1252. This fixes
    // bogus character issues. Technically it could be ISO-8859-1 but it's safe
    // to convert this way.
    // http://en.wikipedia.org/wiki/Windows-1252
    $enc = mb_detect_encoding($text, $destination, TRUE);
    if (!$enc) {
      $text = mb_convert_encoding($text, $destination, $source);
    }

    return $text;
  }


  /**
   * We need to strip the Windows CR characters.
   *
   * Because otherwise we end up with &#13; in the output.
   *
   * @link http://technosophos.com/content/querypath-whats-13-end-every-line
   *
   * @param string $text
   *   The text to normalise.
   *
   * @return string
   *   The normalised text.
   */
  public function stripWindowsCr(&$text) {

    return str_replace(chr(13), '', $text);
  }

  /**
   * Fix &nbsp; being converted into Â.
   *
   * @link
   * http://stackoverflow.com/questions/4515117/php-parsing-problem-nbsp-and
   *
   * @param string $text
   *   The text to normalise.
   *
   * @return string
   *   The normalised text.
   */
  public function fixUTF8NonBreaking(&$text) {
    return str_replace("\xc2\xa0", ' ', $text);
  }

  /**
   * Convert any WINDOWS-1252 or ISO-8859-1 text to UTF-8.
   *
   * @param string $text
   *   The text.
   *
   * @return string
   *   The text.
   */
  public function toUTF8(&$text) {
    return Encoding::toUTF8($text);
  }

  /**
   * Fix UTF8.
   *
   * @param string $text
   *   The text.
   *
   * @return string
   *   The text.
   */
  public function fixUTF8(&$text) {
    return Encoding::fixUTF8($text);
  }

  /**
   * Convert HTML entities.
   *
   * @param string $text
   *   The text.
   *
   * @return string
   *   The text.
   */
  public function htmlEntitiesToUtf8(&$text) {
    return mb_convert_encoding($text, "HTML-ENTITIES", "UTF-8");
  }

  /**
   * String conversion.
   *
   * @param string $text
   *   The text.
   *
   * @return string
   *   The text.
   */
  public function stringConverstion(&$text) {
    return iconv('UTF-8', 'ASCII//TRANSLIT', $text);
  }


  /**
   * Remove an arbitrary string.
   *
   * @param string $text
   *   The text to scan.
   * @param string $string
   *   The string to remove.
   *
   * @return string
   *   The text.
   */
  public function removeString(&$text, $string) {

    $text = str_replace($string, '', $text);

    return $text;
  }

  /**
   * Fix MS Word characters.
   *
   * This is not foolproof.
   *
   * @todo clean this up.
   *
   * @link http://www.php.net/manual/en/function.str-replace.php#102465
   *
   * @param string $text
   *   The text.
   *
   * @return string
   *   The text.
   */
  public function msWordFix(&$text) {
    $text = str_replace(chr(130), ',', $text);    // baseline single quote
    $text = str_replace(chr(131), 'NLG', $text);  // florin
    $text = str_replace(chr(132), '"', $text);    // baseline double quote
    $text = str_replace(chr(133), '...', $text);  // ellipsis
    $text = str_replace(chr(134), '**', $text);   // dagger (a second footnote)
    $text = str_replace(chr(135), '***', $text);  // double dagger (a third footnote)
    $text = str_replace(chr(136), '^', $text);    // circumflex accent
    $text = str_replace(chr(137), 'o/oo', $text); // permile
    $text = str_replace(chr(138), 'Sh', $text);   // S Hacek
    $text = str_replace(chr(139), '<', $text);    // left single guillemet
    // $str = str_replace(chr(140), 'OE', $str);   // OE ligature
    $text = str_replace(chr(145), "'", $text);    // left single quote
    $text = str_replace(chr(146), "'", $text);    // right single quote
    // $str = str_replace(chr(147), '"', $str);    // left double quote
    // $str = str_replace(chr(148), '"', $str);    // right double quote
    $text = str_replace(chr(149), '-', $text);    // bullet
    $text = str_replace(chr(150), '-–', $text);    // endash
    $text = str_replace(chr(151), '--', $text);   // emdash
    // $str = str_replace(chr(152), '~', $str);    // tilde accent
    // $str = str_replace(chr(153), '(TM)', $str); // trademark ligature
    $text = str_replace(chr(154), 'sh', $text);   // s Hacek
    $text = str_replace(chr(155), '>', $text);    // right single guillemet
    // $str = str_replace(chr(156), 'oe', $str);   // oe ligature
    $text = str_replace(chr(159), 'Y', $text);    // Y Dieresis
    $text = str_replace('°C', '&deg;C', $text);    // Celcius is used quite a lot so it makes sense to add this in
    $text = str_replace('£', '&pound;', $text);
    $text = str_replace("'", "'", $text);
    $text = str_replace('"', '"', $text);
    $text = str_replace('–', '&ndash;', $text);

    return $text;
  }
}
