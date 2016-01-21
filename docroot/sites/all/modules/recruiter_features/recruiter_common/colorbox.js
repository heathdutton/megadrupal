(function ($) {

Drupal.behaviors.initRecruiterColorbox = {
  attach: function (context, settings) {
    // Modify settings as suiting for the recruiter.
    settings.colorbox.innerHeight = "70%";
    settings.colorbox.innerWidth = 950;
    settings.colorbox.iframe = true;

    var width = $(window).width();
    var uagent = navigator.userAgent.toLowerCase();
    var mobile = false;
    // Detection for the most important mobile devices. Only checking the screen
    // resultion is not enough.
    if (width < settings.colorbox.innerWidth || uagent.match(/(iphone|ipod|ipad|android|webos|blackberry|blackberryplaybook|windowsphone|kindlefire)/)) {
      mobile = true;
    }
    var basepath = window.location.href;
    var https = false;
    if (basepath.substr(0, 5) == "https") {
      https = true;
    }

    $('a, area, input', context)
      .filter('.recruiter-job-link, .recruiter-resume-link')
      .once('init-recruitercolorbox-processed').each(function() {
        // Append the render slim context.
        $(this).attr('href', function (ind, attr) {
          // If the href contains "?", append $, else, append ?
          return /\?/.test(attr) ? attr + '&render=slim' : attr + '?render=slim';
        });
        // First do not open colorbox wihtin mobile devices (hard to handle)
        // and second, do not open http iframes within https.
        if (!mobile && !(https && $(this).attr('href').substr(0, 5) == "http:")) {
          $(this).colorbox(settings.colorbox);
        }
        else {
          $(this).attr('target', '_blank');
        }
      });
    }
  }
})(jQuery);
