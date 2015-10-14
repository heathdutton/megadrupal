# Language Switcher Reminder

This module is an add-on to the Language Switcher Dropdown module; it provides
users with a client-side reminder that the content they're currently viewing is
available in their preferred language as configured in their browser.

---------------------------------------

### Features

* A configurable and translatable reminder message, shown to the user in his or
  her preferred language;
* A close button, also translatable, which explicitly opts users out of
  receiving the reminder message;
* The ability to limit the number of times the reminder message is shown;
* Fully configurable wrapper markup and placement selector for the reminder.

---------------------------------------

### Requirements

* Language Switcher Dropdown: https://drupal.org/project/lang_dropdown
* Variable (7.x-2.x): https://drupal.org/project/variable
* i18n Variable: https://drupal.org/project/i18n

---------------------------------------

### Installation and minimum configuration instructions

1. Download and enable this module and all of its dependencies, in particular,
   ensure that Language Switcher Dropdown is already enabled, configured, and
   placed to your liking.
2. Ensure that your theme does not strip standard language metadata from the
   HTML tag on your site. Check your main html.tpl.php file and ensure that, for
   HTML5 themes, a "lang" attribute exists and for XHTML themes, ensure that an
   "xml:lang" attribute exists.
3. Navigate to /admin/config/user-interface/language-switcher-reminder, and set
   the reminder limit above 0 (for example 3).
   
At this point, you should see the reminder message on pages with translated
content when the language switcher dropdown is shown and there is a mismatch
between page language and browser preference.

---------------------------------------

### Additional configurations

In addition to the above, you will almost certainly want to customize and
localize the provided reminder message and/or close button text. To do so, you
will need to do the following:

1. If you'd like to customize the reminder message or close button, navigate to
   /admin/config/user-interface/language-switcher-reminder and modify either to
   your preference.
2. Navigate to /admin/config/regional/i18n/variable and check "Reminder message"
   and/or "Reminder close text," to enable localization of those values.
3. Return to the first configuration page and you should see a link for each
   language enabled on your site. Click through each language and enter the
   corresponding localized version of the reminder message and/or close button
   and save the form.
4. Clear the site cache (at /admin/config/development/performance), and you
   should see the reminder message, but localized into your browser's preferred
   language.

Also note that you can add an ID of "langdropdown-reminder-close" to any element
(for example, an anchor in the reminder message above) so that, when clicked, it
automatically focuses on and opens the language switcher dropdown.

You can also edit the the wrapper markup and jQuery placement selector to modify
where and how the message is displayed. Only edit these values if you need to
and you know what you are doing (e.g. you are a themer).

---------------------------------------

### For themers/developers

If you'd like to customize any of the interaction involved with the reminder via
custom JavaScript in your theme, the custom event 'lang_dropdown_remind_ready'
can be bound to in order to know exactly when/if the reminder will be displayed.
It is triggered directly before the remider is displayed with the slideDown
jQuery effect.

Example usage would be to add a dynamic padding to the top of the <body> of your
site if you want to set the reminder to be position: fixed and always stickied
to the top of the page. The padding-top would avoid the reminder overpallong the
content of the page at all. Your theme code might look like this:

JAVASCRIPT:
  (function( $ ) {
    $(Drupal.settings.lang_dropdown_remind.prependto).bind('lang_dropdown_remind_ready', function(){
      var $reminder = $('#langdropdown-reminder');
      var $body = $('body');
      var push = parseInt($body.css('margin-top'));
      var bodyStyle = $body.attr('style');
      // If we have an admin menu, remove position: fixed
      if (push > 0) {
        $reminder.css('position', 'absolute');
        $reminder.css('top', push);
      }
      $body.animate({
        paddingTop: "+=" + $reminder.height()
      }, 400);
    });
  })(jQuery);

CSS:
  #langdropdown-reminder {
    position:fixed;
    top: 0;
    z-index:900;
  }

---------------------------------------

### Reporting issues and contributing

Any issues should be reported to the drupal.org issue queue for this module:
https://drupal.org/project/issues/lang_dropdown_remind
