

                            Elastic Email
               ==========================================

This module directs outgoing Drupal email through the Elastic Email service,
and can optionally queue messages for delivery at cron time.

Elastic Email is a mail relay service. That is, instead of your website
sending mail via its own SMTP server, outgoing email is directed through the
Elastic Email service and out onto the internet. Learn more at:

    http://elasticemail.com



 Why?
==========================================
Elastic Email is of particular use to cloud-hosted websites. There are two
very prominent reasons to use it:

 - Some cloud services (or rather, their IP address ranges) are blacklisted by
   anti-spam services because of the high volumes of spam emanating from their
   servers. For example: Amazon/AWS. That is to say, if you send email directly
   from AWS, there's a good chance it will be blocked by spam filters.
 - Some servers have restrictions on outgoing ports, i.e. SMTP. Your website may
   simply be unable to use SMTP. Elastic Email uses a REST-style API over HTTPS.
   So, all you need open is port 443, the standard HTTPS port.

But the main reason may be:

- You can be up and running in less than 5 minutes total.
  Really.
  You can even get an account that lets you send 1000 emails before you need to
  pay up. And there's no need to mess with configuring sendmail or postfix on
  your server: all configuration is done from one simple Drupal admin screen.



 How?
=========================================
It's a three-step process:

 1. Sign up for an Elastic Email account. Just go to http://elasticemail.com and
    click on "Get your API Key". You'll need the API Key and your Elastic Email
    username (aka your email address) to configure the module.
 2. Install and enable the elastic_email module.
 3. Configure the elastic_email module with the username and API Key.

And you're done. But, if you need a little more detail:



 1. Sign up
=========================================
Go to http://elasticemail.com, click on "Get your API Key". Save the API Key
somewhere safe.



 2. Install & Enable
=========================================
Unzip the elastic_email-XYZ.zip into your sites/all/modules folder. That is,
when unzipped you should have a folder at sites/all/modules/elastic_email
containing README.txt.

Then, go to your Drupal module admin screen which you'll find at:

    YOUR_SITE/admin/modules

Enable the 'elastic_email' module (it's under the 'Mail' category) and click
'Save configuration'.



 3. Configure
=========================================
Go to the Elastic Email configuration screen, which is at:

    YOUR_SITE/admin/config/system/elastic_email

Then:
 1. Enter your 'Elastic Email username'.
 2. Enter your 'API Key'.
 3. Click the 'Test' button. Fix any errors. Finally:
 4. Click 'Save configuration'. You're done.

Drupal should now be sending email via Elastic Email.



 Other configuration options
=========================================
There are two other options you can set:

 - Queue outgoing messages: Instead of sending email on the user's HTTP
   request thread - thus delaying page execution while the message is forwarded
   to the ElasticEmail service - messages are queued and then delivered when
   the cron job runs. This approach is *strongly* recommended for production
   websites.
 - Log message delivery success: All delivery errors are logged, but if this
   option is checked, then a log message is also generated for successfully
   delivered messages. Logging is via watchdog, so you should be able to view
   the log messages at: YOUR_SITE/admin/reports/dblog



 Notes
=========================================
 - Remember that your 'username' is the email address you used when you created
   your Elastic Email account.
 - You'll need the 'administer site configuration' permission to access the
   Elastic Email configuration screen.
 - All mail sent via Elastic Email is delivered individually to each recipient,
   regardless of whether the recipients are in the To:, CC: or BCC: fields.
   That is, message recipients never see the other recipients.
 - This module works by acting as the 'smtp_library' for Drupal. That is, this
   module implements DrupalMailSystem. Only one module can provide this
   function, so this module may not be compatible with other mail-related
   modules. Disable these other mail-related modules to use this module.



 Known issues
=========================================
 - Does not support sending with attachments.
 - Hasn't been tested with non-ASCII text.



 Acknowledgements & other stuff
=========================================
 - The author is not affiliated with the providers of the Elastic Email service.
   However, they kept it real simple, and it works. Well done, lads.
 - Thanks to Dries and the myriad other Drupal contributors for creating what is
   an incredibly powerful and productive piece of software.