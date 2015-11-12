<?php

/**
 * Computed field code for BBCode photo embed/link.
 */
function _pptodrupal_computed_field_forum_code() {
  return '
foreach ($entity->pptodrupal_photo[\'und\'] as $photo_key => $photo) {
  $entity_field[$photo_key][\'value\'] = array(
    \'linked_thumb\' => array(
      \'url\' => url("node/" . $entity->nid, array(\'absolute\' => TRUE)),
      \'picture\' => image_style_url(\'photopost_thumb\', $photo[\'uri\']),
      \'label\' => t(\'Linked thumbnail image\'),
      \'id\' => \'photo-linked-thumbnail-\' . $photo_key,
      \'photo_number\' => $photo_key + 1,
    ),
    \'linked_medium\' => array(
      \'url\' => url("node/" . $entity->nid, array(\'absolute\' => TRUE)),
      \'picture\' => image_style_url(\'photopost_medium\', $photo[\'uri\']),
      \'label\' => t(\'Linked medium image\'),
      \'id\' => \'photo-linked-medium-\' . $photo_key,
      \'photo_number\' => $photo_key + 1,
    ),
    \'medium\' => array(
      \'picture\' => image_style_url(\'photopost_medium\', $photo[\'uri\']),
      \'label\' => t(\'Medium image\'),
      \'id\' => \'photo-medium-\' . $photo_key,
      \'photo_number\' => $photo_key + 1,
    ),
  );
}
';
}

/**
 * Computed field display format for BBCode photo embed/link.
 */
function _pptodrupal_computed_field_forum_display_format() {
  return '
drupal_add_css(drupal_get_path(\'module\', \'vbtodrupal\') . \'/pptodrupal.css\');
drupal_add_js(drupal_get_path(\'module\', \'vbtodrupal\') . \'/pptodrupal.js\');

$form_action = drupal_get_destination();
$form_action = $form_action[\'destination\'];

$display_output = \'<div class="pptodrupal_forum_code">\';

$src = $entity_field_item[\'value\'][\'linked_thumb\'][\'picture\'];
$alt = t(\'Thumbnail of image @photo_number\', array(\'@photo_number\' => $entity_field_item[\'value\'][\'linked_thumb\'][\'photo_number\']));
$height = $width = variable_get(\'pptodrupal_thumb_size\', 160);
$display_output .= \'<div class="pptodrupal_thumb"><img src="\' . $src . \'" alt="\' . $alt . \'" height="\' . $height . \'" width="\' . $width . \'" /></div>\';

$display_output .= \'<div class="pptodrupal_codes">\';

foreach ($entity_field_item[\'value\'] as $photo_type) {
  if (isset($photo_type[\'url\'])) {
    $display_output .= <<<EOD
<form action="{$form_action}"><div>
<label for="{$photo_type[\'id\']}">{$photo_type[\'label\']}:</label>
<input type="text" class="photopost-forum-code" size="50"  value="[url={$photo_type[\'url\']}][img]{$photo_type[\'picture\']}[/img][/url]" id="{$photo_type[\'id\']}" />
</div></form>
EOD;
    }
    else {
      $display_output .= <<<EOD
<form action="{$form_action}"><div>
<label for="{$photo_type[\'id\']}">{$photo_type[\'label\']}:</label>
<input type="text" class="photopost-forum-code" size="50" value="[img]{$photo_type[\'picture\']}[/img]" id="{$photo_type[\'id\']}" />
</div></form>
EOD;
    }
  }

$display_output .= \'</div></div>\';
';
}
