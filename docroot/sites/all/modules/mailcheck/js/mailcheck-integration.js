(function ($) {

  // @see https://github.com/mailcheck/mailcheck/wiki/String-Distance-Algorithms
  // Minor refactor by jwalters (jshint warnings)
  var distanceFunction = function optimalStringAlignmentDistance(s, t) {
    var cost,
        d, i, j, m, n;

    // Determine the "optimal" string-alignment distance between s and t
    if (!s || !t) {
      return 99;
    }
    m = s.length;
    n = t.length;

    /* For all i and j, d[i][j] holds the string-alignment distance
     * between the first i characters of s and the first j characters of t.
     * Note that the array has (m+1)x(n+1) values.
     */
     d = [];
     for (i = 0; i <= m; i++) {
      d[i] = [];
      d[i][0] = i;
    }
    for (j = 0; j <= n; j++) {
      d[0][j] = j;
    }

    // Determine substring distances
    cost = 0;
    for (j = 1; j <= n; j++) {
      for (i = 1; i <= m; i++) {
        cost = (s.charAt(i-1) === t.charAt(j-1)) ? 0 : 1;   // Subtract one to start at strings' index zero instead of index one
        d[i][j] = Math.min(d[i][j-1] + 1,                  // insertion
                           Math.min(d[i-1][j] + 1,         // deletion
                                    d[i-1][j-1] + cost));  // substitution

        if(i > 1 && j > 1 && s.charAt(i-1) === t.charAt(j-2) && s.charAt(i-2) === t.charAt(j-1)) {
          d[i][j] = Math.min(d[i][j], d[i-2][j-2] + cost); // transposition
        }
      }
    }

    // Return the strings' distance
    return d[m][n];
  };

  Drupal.behaviors.mailcheck = {
    attach: function(context, settings) {
      // Only add once.
      if(settings.mailcheck.showInDescription === true) {
        // If description doesn't exist, create an empty one.
        if(!$('.mailcheck').parent().find('.description').length) {
          $('.mailcheck').parent().append('<div class="description" />');
        }
        $('.mailcheck').parent().find('.description').prepend('<div class="mailcheck-action" />');
      }

      $('.mailcheck').blur(function() {
        var currentform = this,
            cfAction;
        if(settings.mailcheck.showInDescription === true) {
          cfAction = $(currentform).parent().find('.mailcheck-action');
        }
        else {
          cfAction = $(currentform).parents('form').find('.mailcheck-action');
        }

        $(this).mailcheck({
          domains: settings.mailcheck.domains,
          topLevelDomains: settings.mailcheck.tlds,
          distanceFunction: distanceFunction,
          threshold: {
            domain: 2,
            tld: 1
          },
          suggested: function(element, suggestion) {
            // Add lock gesture.
            if(settings.mailcheck.lock) {
              $(currentform).closest('form').find('input[type="submit"]').click(function() { return false; });
              setTimeout(function() {
                $(currentform).closest('form').find('input[type="submit"]').unbind('click');
              }, 2000);
            }

            // Replace token with userdefined message, and insert it.
            $(cfAction).html(settings.mailcheck.message.replace('!suggestion', '<a class="corrected-mail">' + suggestion.full + '</a>'));

            // Add shake gesture.
            if(settings.mailcheck.shake) {
              shakeEffect(cfAction);
            }

            // Bind click handler to suggestion link
            $(cfAction).find('.corrected-mail').click(function() {
              // Replace with suggestion, trigger change, trigger keyup, and
              // focus mailcheck input.
              $(currentform).val(suggestion.full).change().keyup().focus();
              // Remove message.
              $(cfAction).html('');
            });
          },
          empty: function() {
            // Remove message since there's no suggestion.
            $(cfAction).html('');
          }
        });
      });

      function shakeEffect(el) {
        // First set attributes.
        $(el).css({
          'position' : 'relative',
          'left' : '0px',
        });

        // Then animate.
        $(el).animate({opacity: 0.25}, 100);
        $(el).animate({left: '-=10'},100);
        $(el).animate({left: '+=20'},100);
        $(el).animate({left: '-=20'},100);
        $(el).animate({left: '+=10'},100);
        $(el).animate({opacity: 1.00}, 170);
      }
    }
  };
})(jQuery);
