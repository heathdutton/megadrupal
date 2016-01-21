
(function ($) {

  function peekaboo_load(div, div_id, context, settings, callback) {
    // Compose the callback path, and include a random token to bypass IE caching.
    var uri = Drupal.settings.basePath 
      + "?q=" 
      + div_id.replace(/-/g, "/") 
      + "/" 
      + Math.floor((1000000000 * Math.random())).toString(16);
    // Make the Ajax call and load the result into the div.
    $(div).load(uri, null, function(){

      if ($(div).hasClass("peekaboo-unprocessed")) {
        // Initiate reload process if needed.
        if (settings['peekaboo'][div_id]['peekaboo_reload']) {
          peekaboo_reload(div, div_id, context, settings);
        }
      }

      $(div)
        // Remove the collapsed class from the div.
        .removeClass("peekaboo-collapsed")
        // Add the expanded class to the div.
        .addClass("peekaboo-expanded")
        // Remove the unprocessed class from the div.
        .removeClass("peekaboo-unprocessed")
        // Add the processed class to the div.
        .addClass("peekaboo-processed");

      // If there's a link, handle that.
      $("a#" + div_id + "-link")
        // Remove the collapsed class from the link.
        .removeClass("peekaboo-collapsed")
        // Add the expanded class to the link.
        .addClass("peekaboo-expanded")
        // Remove the unprocessed class from the link.
        .removeClass("peekaboo-unprocessed")
        // Add the processed class to the link.
        .addClass("peekaboo-processed")
        // Update the text.
        .html(settings['peekaboo'][div_id]['peekaboo_link_text_hide']);

      callback.call();

    });

  }

  function peekaboo_reload(div, div_id, context, settings) {
    var delay = settings['peekaboo'][div_id]['peekaboo_reload_delay'] * 1000;
    setTimeout(function () {
      peekaboo_load(div, div_id, context, settings);
      peekaboo_reload(div, div_id, context, settings);
    }, delay);
  }

  Drupal.behaviors.peekaboo = {

    attach: function (context, settings) {
      $("a.peekaboo-link", context).click(function(e) {
        // Prevent the default action of clicking a link.
        e.preventDefault();
        var link = this;
        // Determine what the ID of this link is.
        var link_id = $(link).attr('id');
        // Work out the ID of the div.
        var div_id = link_id.substr(0, (link_id.length - 5));
        var div = $("div#" + div_id);
        if ($(link).hasClass("peekaboo-processed")) {

          $(div)
            // Toggle it.
            .toggle(0)
            // Toggle the expanded class on the div.
            .toggleClass("peekaboo-expanded")
            // Toggle the collapsed class on the div.
            .toggleClass("peekaboo-collapsed");

          // Update the text.
          if ($(link).hasClass("peekaboo-expanded")) {
            $(link).html(settings['peekaboo'][div_id]['peekaboo_link_text']);
          }
          else {
            $(link).html(settings['peekaboo'][div_id]['peekaboo_link_text_hide']);
          }

          $(link)
            // Toggle the expanded class on the link.
            .toggleClass("peekaboo-expanded")
            // Toggle the collapsed class on the link.
            .toggleClass("peekaboo-collapsed");

        }
        else {
          // Add the Ajax spinner after the link.
          $(link).after(
            '<div id="' + link_id + '-throb" class="peekaboo-progress ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>'
          );

          peekaboo_load(div, div_id, context, settings, function() { 
            // Remove the Ajax spinner.
            $("div#" + link_id + "-throb").remove();
            if (settings['peekaboo'][div_id]['peekaboo_link_remove']) {
              // Remove the link.
              $(link).remove();
            }
          });

        }
      });

      $("div.peekaboo-container", context).each(function() {
        var div = this;
        var div_id = $(div).attr('id');
        if (settings['peekaboo'][div_id]['peekaboo_autoload']) {
          var delay = settings['peekaboo'][div_id]['peekaboo_autoload_delay'] * 1000;
          setTimeout(function () {
            peekaboo_load(div, div_id, context, settings);
          }, delay);
        }

      });

    }
  }
})(jQuery);