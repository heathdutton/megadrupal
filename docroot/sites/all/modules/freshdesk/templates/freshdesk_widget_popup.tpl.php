<?php
/**
 * @file
 * Freshbook template for popup feedback widget.
 */
// We must built the query string based on options to insert into the JS.
$qsting = '&widgetType=popup';
if (empty($freshdesk_widget_popup_form_responsive)):
  $qsting .= '&responsive=no';
endif;
if (!empty($freshdesk_widget_popup_form_heading)):
  $qsting .= '&formTitle=' . $freshdesk_widget_popup_form_heading;
endif;
if (!empty($freshdesk_widget_popup_form_submit_message)):
  $qsting .= '&submitThanks=' . $freshdesk_widget_popup_form_submit_message;
endif;
if (empty($freshdesk_widget_popup_form_screenshot)):
  $qsting .= '&screenshot=no';
endif;
if (empty($freshdesk_widget_popup_form_attach)):
  $qsting .= '&attachFile=no';
endif;
if (empty($freshdesk_widget_popup_form_search)):
  $qsting .= '&searchArea=no';
endif;
if ($freshdesk_widget_popup_type == 'button'):
  $buttontype = '"buttonType": "image",';
  $buttontype .= '"backgroundImage": "' . $freshdesk_widget_popup_type_attribute . '",';
  $buttontype .= '"buttonText": "Support",';
endif;
if ($freshdesk_widget_popup_type == 'text'):
  $buttontype = '"buttonType": "text",';
  $buttontype .= '"buttonText": "' . $freshdesk_widget_popup_type_attribute . '",';
endif;
?>

FreshWidget.init("", {"queryString": "<?php print $qsting ?>",
  "widgetType": "popup",
  <?php print $buttontype; ?>
  "buttonColor": "white",
  "buttonBg": "#006063",
  "alignment": "<?php print $freshdesk_widget_popup_position; ?>",
  "offset": "<?php print $freshdesk_widget_popup_offset; ?>px",
  "formHeight": "<?php print $freshdesk_widget_popup_form_height; ?>",
  "url": "
  <?php print $freshdesk_url; ?>"
});
