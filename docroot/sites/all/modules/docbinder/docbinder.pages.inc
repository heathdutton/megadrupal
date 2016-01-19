<?php
/**
 * @file
 * DocBinder pages.
 */

/**
 * DocBinder Page showing all currently collected documents.
 */
function docbinder_docbinder() {
  if (empty($_SESSION['docbinder']['files'])) {
    $content = t('Sorry, there are no files queued for you to download.');
  }
  else {
    $files = $_SESSION['docbinder']['files'];
    $content = '';
    if (!empty($files)) {
      $options = array(
        'html' => FALSE,
        'query' => array('destination' => current_path()),
        'attributes' => array('class' => array('docbinder-docbinder')),
      );
      $content .= '<table class="docbinder-docbinder-table">';
      foreach ($files as $fid => $file) {
        $item = file_load($fid);
        $link_text = t('Remove from cart');
        $link_options = $options;
        $link_options['attributes']['id'] = 'docbinder-id-' . $item->fid;
        $path = DOCBINDER_PATH_REMOVE . '/' . $item->fid;
        $mime = check_plain($item->filemime);
        $icon_url = file_icon_url((object) $item);
        $icon = '<img class="file-icon" alt="" title="' . $mime . '" src="' . $icon_url . '" /> ';

        $content .= '<tr class="docbinder-docbinder-row" id="docbinder-docbinder-fid-' . $item->fid . '"><td>' . $icon . '</td>'
          . '<td>' . $item->filename . ' (' . format_size($item->filesize) . ')' . '</td>'
          . '<td>' . l($link_text, $path, $link_options) . '</td></tr>';
      }
      $content .= '</table>';
      $options = array();
      $options['attributes']['class'][] = 'button';
      $content .= '<p>' . l(t('Download collection'), 'docbinder/get', $options) . '</p>';
    }
  }
  return $content;
}
