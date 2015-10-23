(function ($) {

// Behavior to initialize cookie.
Drupal.behaviors.initCookiebar = {
  attach: function(context) {
    // Workaround to set an xml:lang attribute if it is not set, because the RO
    // Cookie Opt-In library needs it to function properly.
    $('html').once('ro-cookie', function() {
      if (typeof $(this).attr('xml:lang') === 'undefined') {
        $(this).attr('xml:lang', $(this).attr('lang'));
      }
    });

    $.rocookies.init();
    var settings = {
      'cookiename': Drupal.settings['ro_cookie_opt_in']['cookiename'],
      'cookievalues':
      {
        'accept': Drupal.settings['ro_cookie_opt_in']['cookievalues']['accept'],
        'deny': Drupal.settings['ro_cookie_opt_in']['cookievalues']['deny']
      },
      'cookieurl': Drupal.settings['ro_cookie_opt_in']['cookieurl'],  // This option is a custom setting used in the callback.

      'callback': function(result)
      {
        // If visitor agreed, we need to document this by setting a cookie and logging an URL with a unique code for legal reasons.
        // If the visitor disagreed, we just set a 'deny' cookie and request the image (cookieless!) without a unique code.
        var agent = navigator.userAgent.hashCode(),
            now = new Date(),
            timestamp = Date.UTC(now.getFullYear(), now.getMonth(), now.getDate(), now.getHours(), now.getMinutes(), now.getSeconds(), now.getMilliseconds()),
            uniqueid = timestamp + agent.toString(),
            lifespan = $.rocookiebar.settings['lifespan'] || 5*365,
            consent = $.rocookiebar.settings['cookievalues'][result],
            cookielog = new Image();

        if (result == "accept")
        {
          consent = consent + "." + uniqueid;

          // Add statistics code here, e.g. google analytics startup, comscore, or others.
          // This code is only triggered once, on the page where the visitor agrees to accept cookies.
        }

        // Remember choice in cookie.
        $.rocookies.create($.rocookiebar.settings['cookiename'],consent,$.rocookiebar.settings['lifespan']);

        // Fetch an image to log visitor choice on server, with a unique code if visitor agreed.
        cookielog.src = $.rocookiebar.settings['cookieurl'] + "?" + $.rocookiebar.settings['cookiename'] + "=" + consent;
      }
    };

    settings[Drupal.settings['ro_cookie_opt_in']['language']] = {
      'question': // Base texts for question screen
      {
        'title': Drupal.settings['ro_cookie_opt_in']['question']['title'],
        'intro': Drupal.settings['ro_cookie_opt_in']['question']['intro'],
        'learn-more-links': Drupal.settings['ro_cookie_opt_in']['question']['learn-more-links']
      },
      'change': // Base texts for change of preference
      {
        'title': Drupal.settings['ro_cookie_opt_in']['change']['title'],
        'intro': Drupal.settings['ro_cookie_opt_in']['change']['intro'],
        'learn-more-links': Drupal.settings['ro_cookie_opt_in']['change']['learn-more-links']
      },
      'accept': // All texts for 'agree' option: button, explanations, thank-you text, etc.
      {
        'button': Drupal.settings['ro_cookie_opt_in']['accept']['button'],
        'extras': Drupal.settings['ro_cookie_opt_in']['accept']['extras'],
        'title': Drupal.settings['ro_cookie_opt_in']['accept']['title'],
        'intro': Drupal.settings['ro_cookie_opt_in']['accept']['intro'],
        'current': Drupal.settings['ro_cookie_opt_in']['accept']['current'],
        'learn-more-links': Drupal.settings['ro_cookie_opt_in']['accept']['learn-more-links']
      },
      'deny': // All texts for 'disagree' option: button, explanations, thank-you text, etc.
      {
        'button': Drupal.settings['ro_cookie_opt_in']['deny']['button'],
        'extras': Drupal.settings['ro_cookie_opt_in']['deny']['extras'],
        'title': Drupal.settings['ro_cookie_opt_in']['deny']['title'],
        'intro': Drupal.settings['ro_cookie_opt_in']['deny']['intro'],
        'current': Drupal.settings['ro_cookie_opt_in']['deny']['current'],
        'learn-more-links': Drupal.settings['ro_cookie_opt_in']['deny']['learn-more-links']
      },
      'close': Drupal.settings['ro_cookie_opt_in']['close'] // Close link on thank you screens
    };

    $.rocookiebar.init(settings);
    // If the user has already made a decision do not show the cookiebar.
    if (typeof $.rocookies[$.rocookiebar.settings['cookiename']] === 'undefined') {
      $.rocookiebar.build();
    }
  }
};

Drupal.ro_cookie_opt_in = {};

Drupal.ro_cookie_opt_in.cookiesAllowed = function() {
  var allowed = false;
  if(typeof $.rocookies[$.rocookiebar.settings['cookiename']] !== 'undefined' && $.rocookies[$.rocookiebar.settings['cookiename']].indexOf(Drupal.settings['ro_cookie_opt_in']['cookievalues']['accept']) === 0 ) {
    allowed = true;
  }
  return allowed;
}

}(jQuery));
