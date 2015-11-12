/**
 * Display a message highlighting the language drop down when the browser's
 * language preference does not match the current page and a translation exists.
 */
(function($) {
  $(document).ready(function() {
    var maxRepeats = Drupal.settings.lang_dropdown_remind.repeat;
    var repeated = (function() {
      var parts = document.cookie.split('langDropdownReminded=');
      return parts.length == 2 ? parts.pop().split(';').shift() : 0;
    })();

    // Ensure we only show the message as many times as specified
    if (repeated < maxRepeats) {
      Drupal.getLanguagePreference(function(langPref) {
        // Support XHTML and HTML5 language identification methods.
        var docLang = $('html').attr('xml:lang') || $('html').attr('lang');

        // Ignore language locales for now.
        var langOnly = langPref.substr(0, 2);

        // We care if the language preference and document language don't match.
        if (docLang.indexOf(langOnly) == -1) {
          // Note: lang_dropdown 1.x uses the former while 2.x uses the latter.
          var $input = $('#lang-dropdown-form input[name^="' + langOnly + '"], #lang_dropdown_form_language input[name^="' + langOnly + '"]');

          // Furthermore, we only care if an appropriate translation exists.
          if ($input.length !== 0 && $input.attr('value').indexOf('node') == -1) {
            // Create, insert, and display the message.
            var message = Drupal.settings.lang_dropdown_remind.messages[langOnly] || Drupal.settings.lang_dropdown_remind.messages['default'];
            var closeMsg = Drupal.settings.lang_dropdown_remind.close[langOnly] || Drupal.settings.lang_dropdown_remind.close['default'];
            var close = '<a id="langdropdown-reminder-close">' + closeMsg + '</a>';
            var $markup = $('<div id="langdropdown-reminder">' +
              Drupal.settings.lang_dropdown_remind.markup.replace('!message', message).replace('!close_button', close) +
              '</div>');
            $markup.hide();
            $(Drupal.settings.lang_dropdown_remind.prependto).prepend($markup);
            // Triggering a custom event that can be used for custom interaction
            // in a theme.
            $(Drupal.settings.lang_dropdown_remind.prependto).trigger('lang_dropdown_remind_ready');

            $('#langdropdown-reminder').slideDown();

            // Allow something to trigger opening of the language dropdown.
            $('#trigger-langdropdown').click(function() {$('#edit-lang-dropdown-select_child').toggle();});

            // Behavior for the "close" button.
            $('#langdropdown-reminder-close').click(function() {
              $('#langdropdown-reminder').slideUp();

              // Ensure the language switcher dropdown is also gone (otherwise,
              // it would be awkwardly stuck open with no way to close it).
              $('#edit-lang-dropdown-select_child').css('display', '');

              // If a user physically clicked the "close" button, don't ever
              // display the reminder again (for this session).
              document.cookie = 'langDropdownReminded=' + maxRepeats + '; path=/';
            });

            // Increment the repeated value in the cookie.
            document.cookie = 'langDropdownReminded=' + ++repeated + '; path=/';
          }
        }
      });
    }
  });
})(jQuery);
