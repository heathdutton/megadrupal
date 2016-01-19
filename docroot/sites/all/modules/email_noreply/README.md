Are you looking for a way to specify a no-reply email address for all mail coming from your Drupal site without having to muck around with the MTA configuration on your server? You might think, "Oh, I'll just go change the site email address in Drupal." Unfortunately, Drupal sends important notices such as user registration to that email address, so if you change it to a noreply address, you will no longer receive those notices. By default, many contributed modules such as Backup & Migrate and Mollom use this address as well. Additionally, anywhere the [site:mail] token is used, that noreply address will show, which is probably not what you intended.

What you really want is the from field in the email to use the noreply address, right? This module is an extremely simple module that adds a new field to specify a separate no-reply email address for all outgoing mail. Any email that goes out that from your Drupal site will be sent from this noreply address, but you will still receive your important emails at your Site E-mail (site_mail) address.

# Instructions
1. Enable the module
2. Go to admin/config/system/site-information
3. Enter your no-reply email address and submit the form
4. Go to admin/reports/status to be sure that no users have the same email address as the site. Look for the 'Unique Site E-mail' status and make sure it's green.

There is also a token, called [site:noreply-mail].