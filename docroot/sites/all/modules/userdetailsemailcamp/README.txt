
-- SUMMARY --

  This is a UserDetails Emailcamp component module (for use by other modules).
    * It helps admin to get the report of the users and their status without 
	  login to drupal backend via sending mails dialy or monthly using drupal mail.
	* In this way admin can easily get the email-ids of the users to reply them 
	  if necessary without login to admin backend to find the emailids of the users		
    * It uses the mimemail and mailsystem to send mails to admin
	*It send mails as plain text or HTML message format by using mimemail module
	

  For a full description of the module, visit the project page:
    http://drupal.org/project/userdetailsemailcamp

  To submit bug reports and feature suggestions, or to track changes:
    http://drupal.org/project/issues/userdetailsemailcamp


-- REQUIREMENTS --

  Mail System module - http://drupal.org/project/mailsystem
  Mime Mail module - http://drupal.org/project/mimemail	


-- INSTALLATION --

  Hopefully, you know the drill by now :)
  1. Download the module and extract the files.
  2. Upload the entire UserDetails Emailcamp folder into your Drupal 
	 sites/all/modules/ or sites/my.site.folder/modules/ directory if you are 
	running a multi-site installation of Drupal and you want this module to be
    specific to a particular site in your installation.
  3. Enable the UserDetails Emailcamp module by navigating to:
     Administration > Modules
  4. Adjust settings by navigating to:
     Administration > Configuration > UserDetails Emailcamp
  5. If you select daily option mail will send daily when cron ran.if you select 
	 monthly option mail will send to admin monthly on first day of month when 
	 cron ran.
  6. Add the "From address" and "To address" to send mails to admin in the 
	 Configuration page.	


-- USAGE --
	Since some clients with heavy load of users feels difficult to track the users 
	and their status inside the site evryday.This module helps them to track the 
	users list and their status by sending mail to their mail inbox 
	daily or monthly.In this way admin can easily get the email-ids of the users 
    to reply them if necessary without login to admin backend to find the 
	email ids of the users 

   	Once installed, to send the messages in html format need to configure the 
  	mailsystem by specifing MimeMailSystem as the responsible mail system for a 
  	particular message or all mail sent by one module.

  	This can be done through the web by visiting admin/config/system/mailsystem

	To send the message in full html 
	This can be done through the web by visiting admin/config/system/mimemail
	and select E-mail format as "Full Html"

	This module mainly based on hook_cron() where mails will be send when cron ran.
	
  
    You can use the following optional parameters to build the e-mail:
    'plain':
      Boolean, whether to send messages in plaintext-only
	  (optional, default is FALSE).
    'plaintext':
      Plaintext portion of a multipart e-mail (optional).
    'attachments':
      Array of arrays with the path or content, name and MIME type of 
	  the file (optional).
    'headers':
      A keyed array with headers (optional).

    You can set these in $params either before calling drupal_mail() or 
	in hook_mail() and of course hook_mail_alter().


  


-- CREDITS --

  MAINTAINER: chandrasekhar539 

  * Gudivaka Chandrasekhar
	UserDetails Emailcamp 
