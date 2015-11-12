<VirtualHost *:80>
        ServerName <?php echo $alias_info['uri'] ?>

        DocumentRoot <?php echo "{$alias_info['root']}\n" ?>
        <Directory />
                Options FollowSymLinks
                AllowOverride None
        </Directory>
        <Directory <?php echo $alias_info['root'] ?>>
                Options FollowSymLinks MultiViews -Indexes
                AllowOverride All

                Order deny,allow
                allow from all

                # Rewrite for clean URLs
                RewriteEngine on
                RewriteBase /
                RewriteCond %{REQUEST_FILENAME} !-f
                RewriteCond %{REQUEST_FILENAME} !-d
                RewriteRule ^(.*)$ index.php?q=$1 [L,QSA]
        </Directory>

        CustomLog /var/log/apache2/drupal7-<?php echo $alias_name ?>-access.log combined
        ErrorLog /var/log/apache2/drupal7-<?php echo $alias_name ?>-error.log
        # Possible values include: debug, info, notice, warn, error, crit,
        # alert, emerg.
        LogLevel warn

        ServerSignature Off
</VirtualHost>
