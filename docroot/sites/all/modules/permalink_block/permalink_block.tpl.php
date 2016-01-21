<?php
/**
 * @file
 * Template for the Permalink block module.
 */

// Get the title of the current page.
$page_title = drupal_get_title();
// Get the domain.
$host = variable_get('permalink_block_url_name', preg_replace('#^www\.(.+\.)#i', '$1', parse_url($GLOBALS['base_url'], PHP_URL_HOST)));
// Get the site slogan.
$slogan = variable_get("site_slogan");
// If there is no site slogan, we do not need a separator.
$slogan = (empty($slogan)) ? '' : variable_get('permalink_block_separator', ' - ') . $slogan;
// If there is no title (e.g. on front page), use the site name and slogan.
$page_title = (empty($page_title)) ? $host . $slogan : $page_title;
// Prefix the page title with the site name and decode special characters.
$title = htmlspecialchars_decode($page_title);
// Get the url of the current page.
$query = (!empty($_GET['page'])) ? '?page=' . $_GET['page'] : '';
$link = $GLOBALS['base_url'] . base_path() . current_path() . $query;
// Construct a link to the source page.
$encoded = htmlspecialchars(l('replace-me', $link, array(
  'attributes' => array(
    'title' => $host . $slogan,
  ),
)));
// Put back in the page title. We wanted to exlcude it from being encoded.
$html = preg_replace('/replace-me/', $title, $encoded);
// Copy this file first to the theme's template folder before overriding it.
// CUSTOMIZE FROM HERE..
$permalink_text = t('Permalink');
$intro = t('HTML code to copy to your site or email message:');
$help = t('Click below to select, Ctrl + C to copy.');
// ..TILL HERE or use https://drupal.org/project/stringoverrides.
$display = variable_get('permalink_block_show', array(
  'html' => 0,
  'link' => 'link',
));
$linkonly = (gettype($display['link']) == 'integer') ? '' : '<div id="linkonly">' . $link . '</div>';
$copybox = "'copybox'";
$fullhtml = (gettype($display['html']) == 'integer') ? '' : '<div class="intro">' . $intro . '</div><div class="help">' . $help . '</div><br /><div id="copybox" onclick="selectText(' . $copybox . ')">' . $html . '</div>';
$text = $linkonly . $fullhtml;
$textwrapped = '<div id="permalink">' . $text . '</div>';
?>

<?php if(module_exists('beautytips')): ?>
  <div id="permalink">
  <?php
  print l($permalink_text, $GLOBALS['base_url'] . base_path() . current_path(), array(
        'attributes' => array('title' => t("Right-click to 'Copy Link' or copy from below")),
        )); ?> </div>
  <?php
    $options['permalink_block'] = array(
      // See http://drupalcode.org/project/beautytips.git/blob/HEAD:/README.txt.
      'text' => $textwrapped,
      'cssSelect' => 'div#permalink a',
      'ajaxDisableLink' => 1,
      'trigger' => array('mouseover', 'click'),
      'width' => variable_get('permalink_block_popup_width', '500'),
    );
    beautytips_add_beautytips($options);
  ?>

<?php elseif(module_exists('popup') && !module_exists('beautytips')): ?>
  <?php
  module_load_include('inc', 'popup', 'includes/popup.api');
  print popup(array(
    // See http://drupalcode.org/project/popup.git/blob/HEAD:/README.TXT.
    'text' => $textwrapped,
    'title' => $permalink_text,
    'link' => $link,
    'origin' => 'top-left',
    'expand' => 'top-right',
    'effect' => 'fade',
    'width' => variable_get('permalink_block_popup_width', '500'),
    // Can be set to 'hover' or 'click' through the block configuration.
    'activate' => variable_get('permalink_block_activate', 'hover'),
    // Added just in case 'click' is set. No effect on 'hover'.
    'close' => 'TRUE',
  ));
  ?>

<?php elseif (variable_get('permalink_block_suppress_copybox', 0) == 0): ?>
  <div id="permalink"><span class="link"><?php
  print l($permalink_text, $GLOBALS['base_url'] . base_path() . current_path(), array(
        'attributes' => array('title' => t("Right-click to 'Copy Link' or copy from below")),
        )); ?> </span><br />
  <?php
  print $text;
  ?></div>

<?php else: ?>
  <?php
  print l(t('Permalink'), $GLOBALS['base_url'] . base_path() . current_path(), array(
    'attributes' => array(
      'title' => t("Right-click to 'Copy Link': ") . $GLOBALS['base_url'] . base_path() . current_path(),
    ),
  ));
?>
<?php endif; ?>
