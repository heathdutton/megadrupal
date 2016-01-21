<div id="gConnect">
  <button class="g-signin"
    data-scope="https://www.googleapis.com/auth/plus.login https://www.googleapis.com/auth/userinfo.email"
    data-requestvisibleactions="http://schemas.google.com/AddActivity"
    data-clientId="<?php print $client_id; ?>"
    data-accesstype="offline"
    data-callback="gplus_sync_sign_on_callback"
    data-theme="light"
    data-approvalprompt="force"
    data-cookiepolicy="single_host_origin">
  </button>
</div>