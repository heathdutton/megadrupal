<?php
/**
 * Social Account Linking Interface theme.
 * */
$loc = $GLOBALS['base_url'] . '/' . drupal_get_path('module', 'lr_social_login') . '/images/';
$message = t('connected');
drupal_add_js(array('lrsociallogin' => $my_settings), 'setting');
?>
<script type="text/html" id="loginradiuscustom_tmpl">
  <# if(isLinked) { #>

    <div class="lr-linked">
      <div style="width:100%">
		<span title="<#= Name #>" alt="Linked with <#=Name#>">
<img style="margin-right: 5px;"
     src="<?php print $loc?><#= Name.toLowerCase() #>.png">
					</span>

 <span class="lr-linked-label" style="margin-right:4px;"><#= Name #> is
   <# if(<?php
    $value = isset($_SESSION['current_social_provider']) ? $_SESSION['current_social_provider'] : '';
    echo "'" . $value . "'"?> == providerId) { #>
     </span> <span style="color:green"> <?php print t('currently connected') ?>
     <# } else { #>
      </span> <span class="lr-linked-label" style="margin-right:4px;"> <?php print t('connected') ?>
       <# }  #>
				</span>
       <span> <a style="margin-left:10px;cursor: pointer"
                 onclick='return unLinkAccount(\"<#= Name.toLowerCase() #>\",\"<#= providerId #>\")'>Unlink</a></span>
      </div>
    </div>
    </div>
    <# } else {#>
      <div class="lr-unlinked">
        <div class="lr_icons_box">
          <div style="width:100%">
					<span class="lr_providericons lr_<#=Name.toLowerCase()#>"
                          onclick="return $SL.util.openWindow('<#= Endpoint #>&is_access_token=true&ac_linking=1&callback=<?php print $my_settings["location"]; ?>');"
                          title="<#= Name #>" alt="Link with <#=Name#>">
					</span>
          </div>
        </div>
      </div>
      <# } #>
</script>
<div class="lr_account_linking">
  <div id="interfacecontainerdiv" class="interfacecontainerdiv"></div>
  <div style="clear:both"></div>
</div>
<div class="lr-unlinked-data lr_singleglider_200"></div>
<div style="clear:both"></div>
<div class="lr-linked-data lr_singleglider_200"></div>
<div style="clear:both"></div>

<script>
  jQuery(document).ready(function () {
    initializeAccountLinkingRaasForms();
  });
</script>
