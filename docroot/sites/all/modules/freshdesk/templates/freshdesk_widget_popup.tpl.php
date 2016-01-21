<?php
/**
 * @file
 * Freshbook template for popup feedback widget.
 */
// We must built the query string based on options to insert into the JS.
$qstring = '&widgetType=popup';
if ($freshdesk_widget_popup_form_requester == 0):
  $qstring .= '&helpdesk_ticket[requester]=' . token_replace('[user:mail]', array('user' => $user));
  $qstring .= '&disable[requester]=true';
elseif (!empty($freshdesk_widget_popup_form_requester_value) && valid_email_address($freshdesk_widget_popup_form_requester_value)) :
  $qstring .= '&helpdesk_ticket[requester]=' . $freshdesk_widget_popup_form_requester_value;
  $qstring .= '&disable[requester]=true';
endif;
if (empty($freshdesk_widget_popup_form_responsive)):
  $qstring .= '&responsive=no';
endif;
if (!empty($freshdesk_widget_popup_form_heading)):
  $qstring .= '&formTitle=' . $freshdesk_widget_popup_form_heading;
endif;
if (!empty($freshdesk_widget_popup_form_submit_message)):
  $qstring .= '&submitThanks=' . $freshdesk_widget_popup_form_submit_message;
endif;
if (empty($freshdesk_widget_popup_form_screenshot)):
  $qstring .= '&screenshot=no';
endif;
if (empty($freshdesk_widget_popup_form_attach)):
  $qstring .= '&attachFile=no';
endif;
if (empty($freshdesk_widget_popup_form_search)):
  $qstring .= '&searchArea=no';
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

FreshWidget.init("", {"queryString": "<?php print $qstring ?>",
  "widgetType": "popup",
  <?php print $buttontype; ?>
  "buttonColor": "white",
  "buttonBg": "#006063",
  "alignment": "<?php print $freshdesk_widget_popup_position; ?>",
  "offset": "<?php print $freshdesk_widget_popup_offset; ?>px",
  "formHeight": "<?php print $freshdesk_widget_popup_form_height; ?>px",
  "url": "<?php print $freshdesk_url; ?>",
});
