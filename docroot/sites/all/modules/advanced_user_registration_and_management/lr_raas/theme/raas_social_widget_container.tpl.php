<?php
/**
 * Theme social links.
 * Diaplay Js interface.
 *
 */
$loc = $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'lr_social_login') . '/images/premium_images/';
?>
<script type="text/html" id="loginradiuscustom_tmpl">

  <div class="lr_icons_box">
    <div style="width:100%">
					<span class="lr_providericons lr_<#=Name.toLowerCase()#>"
                          onclick="return $SL.util.openWindow('<#= Endpoint #>&is_access_token=true&callback=<?php print $my_settings["location"]; ?>');"
                          title="<#= Name #>" alt="Sign in with <#=Name#>">

					</span>
    </div>
  </div>

</script>
<div class="lr_singleglider_200 interfacecontainerdiv"></div>
<div style="clear:both"></div>
<script>
  callSocialInterface();
</script>
