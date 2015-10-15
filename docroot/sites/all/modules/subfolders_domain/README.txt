Subfolders Domain Module was written by Brice GATO.

Tchap <www.tchap.fr>

INSTALLATION
==============================================================================

0. Install Drush (optional)

1. First of all download and install the Domain Module and enable it

2. Download and install the Subfolders Domain Module and enable it

3. Visit "admin > structure > domains" and add a new subdirectory domain via 
	"Create domain" tab (admin/structure/domain/create)
    Examples :
            - if you want have example.com/site-2 your domain field should look 
			  like this example.com.site-2
            - if you want have example.com/site3 your domain field should look
			  like this example.com.site3
              [Note: Do Not use example.com.site.2 to have example.com/site-2
			  (example.com.site.2 => example.com.site/2)]

4. Via command line, go to the root of your Drupal install

5. Run drush cron 
 (or /usr/bin/php5 
 /path_to_your_drupal/sites/all/modules/subfolders_domain/
 subfolders_domain_cron.php)
	*Using /usr/bin/php5 ignore the PHP Notice error :  Undefined index: 
	REMOTE_ADDR in bootstrap.inc
	[Note: Do not foreget to configure your crontab for Subfolders Domain]
	The Subfolders Domain cron will not run with Drupal's cron.php in web mode
  because this one needs sufficient rights to the creation of new alias on the
  Drupal root. 
	For the configuration I suggest you copy the subfolders_domain_cron.php 
	file outside your Drupal installation and set up another cron job for the 
	Subfolders Domain to run independently.  
	A sample crontab entry to run Subfolders Domain every minute would look 
	like:

	* * * * * /usr/bin/php5 /path_to_your_subfolders_domain_cron.php 
	(outside your Drupal root)

6. Visit "admin > structure > domains" then click on edit your subdirectory 
   domain and configure the hierarchy via "Subfolders settings" tab
    (admin/structure/domain/view/DOMAIN_ID/subfolders)

7. Visit "user > USER_ID > edit" then assign the domains to your user 
        (user/USER_ID/edit)

8. Visit "user > USER_ID > Subfolders Roles" then assign the roles to your 
   user (user/USER_ID/subfolders-domain-roles)
