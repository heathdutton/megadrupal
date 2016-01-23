/**
 * @file
 * Enables Emphasis, updates on page links.
 */
(function ($) {
  Drupal.behaviors.emphasis = {
    attach: function(context, settings) {

      $(document).ready(function() {
        // Activate emphasis.
        var emphasis_selector = Drupal.settings.emphasis.selector;
        var keyCode = Drupal.settings.emphasis.keyCode;
        var keyCodeCount = Drupal.settings.emphasis.keyCodeCount;

        $(emphasis_selector).emphasis({kc: keyCode, kcCount : keyCodeCount});

        // If an emphasis hash is on the current URL.
        var hash = decodeURI(window.location.hash);
        if (hash.indexOf('[') > 0 && hash.indexOf(']') > 0) {

          // Scroll to emphasis selector.
          if (Drupal.settings.emphasis.scroll) {
            $('html, body', context).animate({
              scrollTop: $(emphasis_selector).first().offset().top
            });
          }

          // Update link hashes for various elements on page.
          update_link_hashes();
        }
      });

      // React to AJAX, which may pull in new linked content.
      $(document).ajaxComplete(function() {
        // Update link hashes on page.
        update_link_hashes();
      });

      // React to emphasis changing hash.
      $(document).bind('emphasisHashUpdated', function(event, emphasis) {
        // Update link hashes on page.
        update_link_hashes();
      });

      /**
       * Finds all links on page with current URL and appends hash.
       */
      function update_link_hashes() {

        // Update open graph meta tags on page. We're playing it safe here by
        // modifying the existing URL, in case it differs from our
        // currentBaseURL.
        if (Drupal.settings.emphasis.updateOGMetaTags) {
          var currentMeta = parseUri($('meta[property=og:url]').attr('content'));
          var newMeta = currentMeta.protocol + '://' + currentMeta.host + currentMeta.path + '?' + currentMeta.query + window.location.hash;
          $('meta[property=og:url]').attr('content', newMeta);
        }

        // Extract base URL, without hash.
        var currentBaseURI = parseUri(window.location);
        var currentBaseURL = currentBaseURI.protocol + '://' + currentBaseURI.host + currentBaseURI.path + '?' + currentBaseURI.query;
        // Add new hash.
        var newURL = currentBaseURL + window.location.hash;

        // Update anchors on page.
        if (Drupal.settings.emphasis.updateAnchors) {
          $('a', context).each(function() {

            // Update only anchors that have an href attribute.
            if (typeof $(this).attr('href') == 'undefined') {
              // Continue to next iteration of loop.
              return true;
            }

            // Search for anchors with hrefs that match our currentBaseURL. Again,
            // we play it safe by re-using the anchor's current href, and thereby
            // avoiding stripping off parts of the URL by accident.
            var current_href = parseUri($(this).attr('href'));
            var regex = '(' + currentBaseURL + ')';
            var matches = $(this).attr('href').match(regex);
            if (matches) {
              // Rebuild URL with new hash.
              new_href = current_href.protocol + '://' + current_href.host + current_href.path + '?' + current_href.query + window.location.hash;
              $(this).attr('href', new_href);
              return true;
            }

            // For each anchor, check the URL's query parameters for a string that
            // matches our currentBaseURL. This will take care of share links
            // like socialmedia.com?url=currentBaseURL.
            if (current_href.query.length) {
              var URLQuery = parseQueryString(current_href.query);
              // Iterate over values in query string.
              for (var propertyName in URLQuery) {
                // If a query value matches our current url regex, modify.
                if (URLQuery[propertyName].match(regex)) {
                  var param_url = parseUri(URLQuery[propertyName]);
                  // Generate modified URL.
                  new_param_url = param_url.protocol + '://' + param_url.host + param_url.path + '?' + param_url.query + window.location.hash;

                  // Rebuild URL with new, modified query string.
                  URLQuery[propertyName] = new_param_url;
                  newURLQueryString = buildQueryString(URLQuery);

                  new_href = current_href.protocol + '://' + current_href.host + current_href.path + '?' + newURLQueryString;
                  $(this).attr('href', new_href);
                }
              }
            }
          });
        }

        if (Drupal.settings.emphasis.shareThis && typeof stlib != 'undefined') {
          stlib.data.pageInfo.incomingHash = window.location.hash.substr(1);
          stlib.data.pageInfo.shareHash = window.location.hash.substr(1);
          stButtons.locateElements();
        }
      }

      /**
       * parseUri 1.2.2
       * (c) Steven Levithan <stevenlevithan.com>
       * MIT License
       */
      function parseUri (str) {
        parseUri.options = {
          strictMode: false,
          key: ["source","protocol","authority","userInfo","user","password","host","port","relative","path","directory","file","query","anchor"],
          q:   {
            name:   "queryKey",
            parser: /(?:^|&)([^&=]*)=?([^&]*)/g
          },
          parser: {
            strict: /^(?:([^:\/?#]+):)?(?:\/\/((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?))?((((?:[^?#\/]*\/)*)([^?#]*))(?:\?([^#]*))?(?:#(.*))?)/,
            loose:  /^(?:(?![^:@]+:[^:@\/]*@)([^:\/?#.]+):)?(?:\/\/)?((?:(([^:@]*)(?::([^:@]*))?)?@)?([^:\/?#]*)(?::(\d*))?)(((\/(?:[^?#](?![^?#\/]*\.[^?#\/.]+(?:[?#]|$)))*\/?)?([^?#\/]*))(?:\?([^#]*))?(?:#(.*))?)/
          }
        };

        var o   = parseUri.options,
          m   = o.parser[o.strictMode ? "strict" : "loose"].exec(str),
          uri = {},
          i   = 14;

        while (i--) uri[o.key[i]] = m[i] || "";

        uri[o.q.name] = {};
        uri[o.key[12]].replace(o.q.parser, function ($0, $1, $2) {
          if ($1) {
            uri[o.q.name][$1] = $2;
          }
        });

        return uri;
      }

      /* Parses a query string*/
      function parseQueryString(qs) {
        var nvpair = {};
        var pairs = qs.split('&');
        $.each(pairs, function(i, v){
          var pair = v.split('=');
          nvpair[pair[0]] = decodeURIComponent(pair[1]);
        });
        return nvpair;
      }

      /* Builds a query string */
      function buildQueryString(data) {
         var ret = [];
         for (var d in data) {
            ret.push(encodeURIComponent(d) + "=" + encodeURIComponent(data[d]));
         }
         return ret.join("&");
      }

    }
  };
})(jQuery);
