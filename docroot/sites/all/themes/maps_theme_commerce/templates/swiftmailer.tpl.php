<?php
  global $base_url;
?>

<table width="100%" height="100%" cellpadding="5px" cellspacing="0" bgcolor="#f4f4f4">
  <tr>
    <td valign="top" align="center" style="background-color:#f0f0f0;font-size:12px;font-family:arial,sans-serif;margin:0;padding:0 0 35px 0;">
      <table width="600" cellpadding="0" cellspacing="0">
        <tr>
          <td style="line-height:18px;font-size:12px;">
            <table cellpadding="0" cellspacing="0" width="100%">
              <tr>
                <td style="background:#41a7ef;">
                  <div class="htmlmail-banner">
                    <a href="<?php print $base_url ?>"><img src="image:<?php print drupal_get_path('theme', 'maps_theme_commerce') . '/img/htmlmail_banner.png'; ?>" alt="MaPS System Commerce" /></a>
                  </div>
                </td>
              </tr>
              <tr>
                <td style="padding: 20px; background: white;">
                  <div class="htmlmail-body">
                    <?php echo $body; ?>
                  </div>
                </td>
              </tr>
              <tr>
                <td style="padding: 20px;">
                  <div>
                    <a href="<?php print $base_url ?>" style="color: #666;"><?php print $base_url ?></a>
                  </div>
                </td>
              </tr>
            </table>
          </td>
        </tr>
      </table>
    </td>
  </tr>
</table>
