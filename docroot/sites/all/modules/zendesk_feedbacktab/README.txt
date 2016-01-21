README
======
The Zendesk Feedback Tab is essentially a small and portable version of the Help Center or Web portal that you can embed
on your Drupal site. It can be configured to allow your end-users to search your knowledge base, submit a support request,
or live chat with your agents.

https://support.zendesk.com/entries/20990726-Setting-up-your-Feedback-Tab-channel

The submitted feedback or support requests will create tickets in your zendesk support center account.
A zendesk user account will be created for each submission for each unique email address.
An email will be sent to the support/feedback requestor and they will be get email updates
as well they can reply to the ticket via email.

This module will allow you to configure the tab online and use the Drupal permissions to
determine if the tab is shown for anonoymous users, authenticated users or a restricted role only.

Installation is easy:
- Install the module
- Configure the feedback tab from your zendesk account (go to 'Settings - Channels - Feedback Tab')
  * When you complete the setup, click on the button to Preview the code.
  * We need to use part of the generated zendesk tab code to complete the setup on the Drupal site. You only need to copy the code
    that is between (inside) the {} of the Zenbox.init function defintion.
    Example: Only lines that you will need to copy will be like:
      dropboxID:   "20100001",
      url:         "https://yourdomain.zendesk.com",
      tabTooltip:  "Support",
      tabImageURL: "https://assets.zendesk.com/external/zenbox/images/tab_support.png",
      tabColor:    "black",
      tabPosition: "Left"

- On your Drupal site, access the module configuration page  /admin/config/people/zendesk_tab
  * Paste the tab definition code

- Review the permissions to have it appear for the roles you want.



