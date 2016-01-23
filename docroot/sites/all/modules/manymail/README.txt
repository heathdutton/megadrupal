ManyMail is a mass e-mail module. In short: it allows you to send a ton
of e-mails LIVE from within your website, with a very low risk of it being
marked as spam.

To use ManyMail, make sure you first follow the installation details in
INSTALL.txt. After you've done that, refer to the instructions below for
a brief manual.



Configuring ManyMail
===================================
First, go to admin/config/manymail and select "General configuration" or go
to admin/config/manymail/config directly.

Options
-----------------------------------
Configure how many e-mails you want to send per batch and how long you want
to wait inbetween batches.

IMPORTANT:
Setting the interval too low or the amount too high may result in e-mails
being dropped due to being marked as spam.

Mail defaults
-----------------------------------
You can set some default content on the 'mail defaults' tab, including:
  - The sender
  - (optional) The reply-to address
  - The subject
  - The body
  - The signature

NOTE:
The signature is a sort of company must-have branding. This is best used for
adding a uniform greeting and/or a legal clauses to the likes of "Legal info:
Any information in this e-mail is private, etc". You can still choose whether
or not to include this signature with every e-mail sent.

Server settings
-----------------------------------
Pretty straightforward: add your SMTP settings.



Configuring recipients
===================================
Secondly, go to admin/config/manymail and select "Mass mail targets" or go
to admin/config/manymail/targets directly.

Recipient roles
-----------------------------------
Pretty simple really: Check which roles you want to make available for
sending mass e-mail to. Only people with the permission "Send e-mail to user
roles" will be able to target the approved roles.

Checking the "administrators may override" flag will do exactly that...



Using ManyMail
===================================
Finally, navigate to manymail (as in sitepath/manymail), select a target and
fill in a message. Once you click send, DO NOT leave the page until the sending
is done. Opening a new browser tab, however, is allowed.
