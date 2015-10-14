<?php

/**
 * @file
 * PMB info notice template.
 */

$notice_id = $notice['id'];

switch ($info) {
  case 'title' :
    $template .= (isset($notice['fields']['title']['full']) ? $notice['fields']['title']['full'] : '');
    break;

  case 'link' :
    switch ($notice['notice']['header']['hl']) {
      case 1:
        $link = _pmb_p('catalog/serial/') . $notice_id . '/';
        break;
      case 2:
        $link = _pmb_p('catalog/record/') . $notice_id . '/';
        break;
      default:
      case 0:
        $link = _pmb_p('catalog/record/') . $notice_id . '/';
        break;
    }
    $template .= url($link);
    break;
}
