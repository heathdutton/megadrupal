<?php if ($this->ssl_enabled && $this->ssl_key) : ?>

  <VirtualHost <?php print "{$ip_address}:{$http_ssl_port}"; ?>>
  <?php if ($this->site_mail) : ?>
    ServerAdmin <?php  print $this->site_mail; ?> 
  <?php endif;?>

    DocumentRoot <?php print $this->root; ?> 
      
    ServerName <?php print $this->uri; ?>

    # Enable SSL handling.
     
    SSLEngine on

    SSLCertificateFile <?php print $ssl_cert; ?>

    SSLCertificateKeyFile <?php print $ssl_cert_key; ?>

<?php
if (sizeof($this->aliases)) {
  foreach ($this->aliases as $alias) {
    print "  ServerAlias " . $alias . "\n";
  }
}
?>

    RewriteEngine on
    # the ? at the end is to remove any query string in the original url
    RewriteRule ^(.*)$ <?php print $this->platform->server->web_disable_url . '/' . $this->uri ?>?

</VirtualHost>
<?php endif; ?>

<?php 
  include(provision_class_directory('Provision_Config_Apache_Site') . '/vhost_disabled.tpl.php');
?>
