<?php
/**
 * @file
 * Freshbook template for embedded feedback widget.
 */
// We must built the query string based on options to insert into the embedded form.
$iframesrc = '';
$iframesrc = $freshdesk_url . '/widgets/feedback_widget/new?';
$iframesrc .= '&widgetType=embedded&screenshot=no';
if (empty($freshdesk_widget_embed_form_search)):
  $iframesrc .= '&searchArea=no';
endif;
if (empty($freshdesk_widget_embed_form_attach)):
  $iframesrc .= '&attachFile=no';
endif;
if (!empty($freshdesk_widget_embed_form_submit_message)):
  $freshdesk_widget_embed_form_submit_message = trim($freshdesk_widget_embed_form_submit_message);
  $iframesrc .= '&submitThanks=' . str_replace(' ', '+', $freshdesk_widget_embed_form_submit_message);
endif;
if (!empty($freshdesk_widget_embed_form_heading)):
  $freshdesk_widget_embed_form_heading = trim($freshdesk_widget_embed_form_heading);
  $iframesrc .= '&formTitle=' . str_replace(' ', '+', $freshdesk_widget_embed_form_heading);
endif;
if ($freshdesk_widget_embed_form_requester == 0):
  $iframesrc .= '&helpdesk_ticket[requester]=' . token_replace('[user:mail]', array('user' => $user));
  $iframesrc .= '&disable[requester]=true';
elseif (!empty($freshdesk_widget_embed_form_requester_value) && valid_email_address($freshdesk_widget_embed_form_requester_value)) :
  $iframesrc .= '&helpdesk_ticket[requester]=' . $freshdesk_widget_embed_form_requester_value;
  $iframesrc .= '&disable[requester]=true';
endif;
?>

<script type="text/javascript" src="https://s3.amazonaws.com/assets.freshdesk.com/widget/freshwidget.js"></script>
<style type="text/css" media="screen, projection">
  @import url(https://s3.amazonaws.com/assets.freshdesk.com/widget/freshwidget.css); 
</style> 
<iframe class="freshwidget-embedded-form" id="freshwidget-embedded-form" src="<?php print $iframesrc; ?>" scrolling="no" height="<?php print $freshdesk_widget_embed_form_height; ?>px" width="100%" frameborder="0" >
</iframe>
