<?php
/**
 * Theme email popup.
 */
$message = variable_get('socialloginandsocialshare_emailrequired_popup_top');

?>
<div class="LoginRadius_overlay LoginRadius_content_IE">
  <div class="popupmain">
    <form id="socialloginandsocialshare-popup-form" method="post" action="">
      <div class="lr-popupheading"> <?php print $message ?></div>
      <div id="popupinner">
        <?php if ($popup_params['msgtype'] == 'warning') : ?>
          <div id="loginRadiusError" class="lr-error">
            <?php print $popup_params['msg'] ?>
          </div>
        <?php
        else :
          ?>
          <div class="lr-noerror">
            <?php print $popup_params['msg'] ?>
          </div>
        <?php endif; ?>
        <div class="emailtext" id="innerp">Enter your email:</div>
        <div><input type="text" name="email" id="email" class="inputtxt"/></div>
      </div>
      <div class="lr-popup-footer">
        <input type="submit" name="socialloginandsocialshare_emailclick" id="socialloginandsocialshare_emailclick"
               value="Submit" class="button colorless"/>
        <input type="submit" name="socialloginandsocialshare_emailclick_cancel"
               id="socialloginandsocialshare_emailclick_cancel" value="Cancel" class="button colorless"/>
      </div>
    </form>
  </div>
</div>
