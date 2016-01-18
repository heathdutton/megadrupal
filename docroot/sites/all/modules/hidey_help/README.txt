Hidey Help by Thinkyhead - http://www.thinkyhead.com/

This is a very simple module that allows users to hide and show the Help block at the top of forms, admin pages, and other places where modules have employed hook_help() to display user assistance.

Installation

Drop the module into your "sites/*/modules" folder and enable the "hide and show help" permission to all the roles that you want to be able to use it. That's it! If you're upgrading from a previous version of the module, be sure not to skip the permissions step!

Usage

When you visit a page with a Help block it will be hidden and a "Show Help" button will appear in its place. Click this button to show help. Its title will change to "Hide Help." Click the "Hide Help" button to hide the help block. Needs no explanation, really. The button text is translatable and works great with String Overrides.