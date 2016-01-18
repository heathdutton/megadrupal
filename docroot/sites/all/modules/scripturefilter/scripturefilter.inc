<?php
/**
 * @file
 * The scripturize functions that do the actual work of finding
 * scripture references and turning them into links.
 */

define('DEFAULT_BIBLE_TRANSLATION', 'NIV');

/**
 * This function breaks up the text into sections not to touch.
 *
 * (HTML-style  * tags, CODE sections, etc.) and sections we can touch.
 * It then runs the sections that can be touched through a function that
 * does the  modifications: scripturizeAddLinks
 */
function scripturefilter_scripturize($text = '', $bible = DEFAULT_BIBLE_TRANSLATION) {
  // Skip everything within a hyperlink, a <pre> block, a <code> block, or a
  // tag we skip tags because something like <img src="blah" alt="John 3:16">
  // should not be messed with.
  $anchor_regex = '<a\s+href.*?<\/a>';
  $pre_regex = '<pre>.*<\/pre>';
  $code_regex = '<code>.*<\/code>';
  $tag_regex = '<(?:[^<>\s]*)(?:\s[^<>]*){0,1}>';
  $split_regex = "/((?:$anchor_regex)|(?:$pre_regex)|(?:$code_regex)|(?:$tag_regex))/i";

  $parsed_text = preg_split($split_regex, $text, -1, PREG_SPLIT_DELIM_CAPTURE);
  $linked_text = '';

  while (list($key, $value) = each($parsed_text)) {
    if (preg_match($split_regex, $value)) {
      // If it is an HTML element or within a link, just leave it as is.
      $linked_text .= $value;
    }
    else {
      // If it's text, parse it for Bible references.
      $linked_text .= scripturefilter_add_links($value, $bible);
    }
  }
  return $linked_text;
}

/**
 * This function replaces the references within a portion of text.
 *
 * It takes a portion of text, and searches for any valid Bible references
 * within it, using regular expression matching. It then surrounds those
 * references with an anchor tag linking to the relevant Bible text
 * online, by passing the groups within that reference to another function
 * called scripturefilterCallback.
 */
function scripturefilter_add_links($text = '', $bible = DEFAULT_BIBLE_TRANSLATION) {
  $volume_regex = '1|2|3|I|II|III|1st|2nd|3rd|First|Second|Third';

  $book_regex  = 'Genesis|Exodus|Leviticus|Numbers|Deuteronomy';
  $book_regex .= '|Joshua|Judges|Ruth|Ezra|Nehemiah|Esther';
  $book_regex .= '|Job|Psalms?|Proverbs?|Ecclesiastes';
  $book_regex .= '|Songs? of Solomon|Song of Songs';
  $book_regex .= '|Isaiah|Jeremiah|Lamentations|Ezekiel|Daniel';
  $book_regex .= '|Hosea|Joel|Amos|Obadiah|Jonah|Micah|Nahum';
  $book_regex .= '|Habakkuk|Zephaniah|Haggai|Zechariah|Malachi';
  $book_regex .= '|Mat+hew|Mark|Luke|John|Acts?|Acts of the Apostles';
  $book_regex .= '|Romans|Galatians|Ephesians|Phil+ippians|Colossians';
  $book_regex .= '|Titus|Philemon|Hebrews|James|Jude|Revelations?';

  $volume_book_regex  = 'Samuel|Kings|Chronicles|Corinthians';
  $volume_book_regex .= '|Thessalonians|Timothy|Peter|John';

  // Split these up from the Perl code because I want to be able to have an
  // optional full stop at the end of just the abbreviations.
  $abbrev_regex  = 'Gen|Ex|Exo|Lev|Num|Nmb|Deut?';
  $abbrev_regex .= '|Josh?|Judg?|Jdg|Rut|Ezr|Neh';
  $abbrev_regex .= '|Est|Jb|Psa?|Pr(?:ov?)?';
  $abbrev_regex .= '|Eccl?|Song?(?: of Sol)?';
  $abbrev_regex .= '|Isa|Jer|Lam|Eze|Dan';
  $abbrev_regex .= '|Hos|Joe|Amo|Oba|Jon|Mic|Nah';
  $abbrev_regex .= '|Hab|Zeph?|Hag|Zech?|Mal';
  $abbrev_regex .= '|Ma?t?t|Mr?k|Lu?k|Jh?n|Jo|Act';
  $abbrev_regex .= '|Rom|Gal|Eph|Phi|Phil|Col|Tim';
  $abbrev_regex .= '|Tit|Phile|Heb|Js|Ja?m|Ju?d|Rev';

  $volume_abbrev_regex = 'Sam|Ki?n|Chr(?:on?)?|Jo|Jh?n';
  $volume_abbrev_regex .= '|Cor|The?|Thess?|Tim|Pe?t';

  $book_regex = '(?:' . $book_regex . ')|(?:' . $abbrev_regex . ')\.?';
  $book_regex_withvolume = '(?:' . $volume_book_regex . ')|(?:' . $volume_abbrev_regex . ')\.?';

  $verse_regex = "\d{1,3}(?::\d{1,3})?(?:\s?(?:[-&,]\s?(?:\d{1,3}:)?\d+))*";

  $translation_regex = 'NIV|NIV1984|NASB|AMP|NLT|KJV|ESV|CEV|NET|NKJV|KJ21|ASV|WE|YLT|DARBY|WYC|NIV-UK|TNIV|MSG|NIRV';

  $passage_regex = '/(?:(?:(' . $book_regex . '))|(?:(' . $volume_regex . ')\s)(' . $book_regex_withvolume . '))\s';
  $passage_regex .= '(' . $verse_regex . ')(?:\s?[,-]?\s?((?:' . $translation_regex . ')|\s?\((?:' . $translation_regex . ')\)))?/';
  $wrapper = new ScriptureFilterCallbackWrapper();
  $wrapper->bible = $bible;
  $text = preg_replace_callback($passage_regex,
    array(
      &$wrapper,
      "scripturefilterCallback",
    ),
     $text);

  return $text;
}

/**
 * A simple class to wrap our callback function.
 *
 * We need this because we need to pass the Bible parameter in.
 * The preg_replace callback is only passed the matched groups.
 */
class ScriptureFilterCallbackWrapper {

  /**
   * A function just to work on a Bible reference, and turn into a link.
   *
   * It is passed the details of the incoming Bible reference to, and also the
   * original reference, so that the reference can be surrounded by an anchor
   * linking to the relevant site which has the text for that verse. It then
   * passes the transformed reference back out so that it can be replaced in the
   * wider body of text from which it came.
   * Two arguments for book; the first will be populated if no volume is given.
   * The second will be populated if there's a volume number.
   */
  public function scripturefilterCallback($matches) {
    $reference = $matches[0];
    $book1 = $matches[1];
    $volume = $matches[2];
    $book2 = $matches[3];
    $verse = $matches[4];
    $translation = isset($matches[5]) ? $matches[5] : NULL;
    $user_translation = $this->bible;

    $book = ($book1 == '') ? $book2 : $book1;
    if ($volume) {
      $volume = str_replace('III', '3', $volume);
      $volume = str_replace('II', '2', $volume);
      $volume = str_replace('I', '1', $volume);
      // Finally, remove st,nd,and rd (presupposes regex is correct).
      $volume = $volume{0};
    }

    if (!$translation) {
      if (!$user_translation) {
        $translation = DEFAULT_BIBLE_TRANSLATION;
      }
      else {
        $translation = $user_translation;
      }
    }
    else {
      // Strip out any parentheses that might have made it this far.
      $translation = trim($translation, ' ()');
    }

    // Sometimes biblegateway.com changes the abbreviation for a translation.
    // This canonicalises them without breaking sites using older abbreviations.
    $trans_replace = array(
      'NIV-UK' => 'NIVUK');
    if (isset($trans_replace[$translation])) {
      $translation = $trans_replace[$translation];
    }

    // If necessary, choose part of the verse reference to pass to the web
    // interfaces. They wouldn't know what to do with John 5:1-2, 5, 10-13 so we
    // just give them John 5:1-2.This doesn't work quite right with something
    // like 1:5,6 - it gets chopped to 1:5 instead of converted to 1:5-6.
    if ($verse) {
      $verse = strtok($verse, ',& ');
    }

    $book = trim($book, '.');

    // Some descriptors for Bible books may be unknown to the websites providing
    // the text. E.g., Bible Gateway doesn't know of a book called "Phi". Some
    // sites may use these, however. Replace them with recognisable
    // abbreviations. To add such replacements, add new entries to this array.
    $book_replace = array(
      'Phi' => 'Phil',
      'Song of Sol' => 'Song of Songs',
      'Js' => 'James');
    if (isset($book_replace[$book])) {
      $book = $book_replace[$book];
    }

    switch ($translation) {
      case 'ESV':
        // Note: the ESV could actually support a mouseover reference
        // we could pull it directly from their site and include it in $title
        // text. See http://www.gnpcb.org/esv/share/services/api/ for more info.
        $link = htmlspecialchars('http://www.gnpcb.org/esv/search/?go=Go&q=');
        $title = 'English Standard Version Bible';
        $link = sprintf('<a href="%s%s" title="%s">%s</a>', $link, htmlentities(urlencode(trim("$volume $book $verse"))), $title, trim($reference));
        break;

      case 'NET':
        $link = htmlspecialchars('http://net.bible.org/passage.php?passage=');
        $title = 'New English Translation';
        $link = sprintf('<a href="%s%s" title="%s">%s</a>', $link, htmlentities(urlencode(trim("$volume $book $verse"))), $title, trim($reference));
        break;

      default:
        $link = "http://biblegateway.com/passage/?search=";
        $translation_suffix = htmlspecialchars('&version=');
        $title = 'Bible Gateway';
        $link = sprintf('<a href="%s%s%s%s" title="%s">%s</a>', $link, htmlentities(urlencode(trim("$volume $book $verse"))), $translation_suffix, $translation, $title, trim($reference));
        break;
    }

    return $link;
  }
}
