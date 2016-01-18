
<div id="parachat-applet">
  <applet codebase="<?php print $codebase; ?>"
   archive="papplet.jar" code="pclient.main.ChatClient.class" height="<?php print $height; ?>" width="<?php print $width; ?>">
  <param name="Net.Site" value="<?php print $site_id; ?>">
  <param name="Net.Room" value="<?php print $room; ?>">
  <param name="cabbase" value="papplet.cab">
  <param name="Ctrl.AutoLogin" value="true">
  <param name="Net.User" value="<?php print $username; ?>">

  <?php if (isset($pass) && strlen($pass) > 0) { // only send pass if this isn't false ?>
  <param name="Net.UserPass" value="<?php print $pass; ?>">
  <param name="Net.Cookie" value="donotcare">';
  <?php } ?>

  <param name=Ad.BrandOn value=false>
  Sorry, your browser is not Java enabled, please visit
  <a href="http://www.parachat.com/faq/java.html">our java support pages</a>
  </applet>
</div>
