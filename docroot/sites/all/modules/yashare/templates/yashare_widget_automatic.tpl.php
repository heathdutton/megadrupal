<?php
print '<' . $element['#html_tag']
  . ' id="' . $element['#id'] . '"'
  . ' class="yashare yashare-auto-init ' . $element['#yashare_theme'] . ' ' . $element['#yashare_type'] . '"'
  . ' data-yashareType="' . $element['#yashare_type'] . '"'
  . ' data-yashareTheme="' . $element['#yashare_theme'] . '"'
  . ' data-yashareQuickServices="' . implode(',', $element['#quickservices']) . '"'
  . ' data-yasharePopupServices="' . implode(',', $element['#popup_services']) . '"'
  . ' data-yashareTitle="' . $element['#title'] . '"'
  . ' data-yashareDescription="' . $element['#description'] . '"'
  . ' data-yashareLink="' . $element['#link'] . '"'
  . ' data-yashareL10n="' . $element['#l10n'] . '"'
  . ' data-yashareImage="' . $element['#image'] . '"'
  . '></' . $element['#html_tag'] . '>';
?>