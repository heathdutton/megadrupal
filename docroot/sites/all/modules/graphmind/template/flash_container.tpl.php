<!-- Container for the SWF object. -->
<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
  id="<?php echo $id; ?>" 
  class="<?php echo $class; ?>"
  width="100%" 
  height="600"
  codebase="http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab">
  
  <param name="movie" value="<?php echo $swf_source; ?>" />
  <param name="quality" value="high" />
  <param name="bgcolor" value="#FFFFFF" />
  <param name="allowScriptAccess" value="sameDomain" />
  <param name="allowFullScreen" value="true" />
  <param name="flashvars" value="<?php echo $flashvars; ?>" />
  <param name="wmode" value="transparent" />
  
  <embed src="<?php echo $swf_source; ?>" 
    quality="high"
    wmode="transparent"
    bgcolor="#FFFFFF"
    width="100%" 
    height="600" 
    name="<?php echo $id; ?>" 
    align="middle"
    play="true"
    loop="false"
    quality="high"
    allowScriptAccess="sameDomain"
    allowFullScreen="true"
    type="application/x-shockwave-flash"
    pluginspage="http://www.adobe.com/go/getflashplayer"
    flashvars="<?php echo $flashvars; ?>" />
  
</object>
