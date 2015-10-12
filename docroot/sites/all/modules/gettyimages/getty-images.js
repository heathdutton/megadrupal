/**
 * @file
 * Top-level descriptor for Getty Images.
 *
 * Provides structure and utility methods for GettyImages
 * controller and models
 */

var GettyImages;

(function($) {
  var g = GettyImages = $.extend(Drupal.settings.gettyImages, {
    // Omniture / SiteCatalyst.
    s: getty_s,
    // Model classes.
    model: {},
    // View classes.
    view: {},
    // Controller instance.
    controller: null,
    // User object.
    user: null,
    // Frame object.
    frame: null,

    // Pretty arbitrarily defined. Would be better to come out of Drupal.
    sizes: {
      small: {
        label: "Small",
        width: 150,
        height: 150
      },
      medium: {
        label: "Medium",
        width: 640,
        height: 480
      },
      large: {
        label: "Large",
        width: 800,
        height: 600
      }
    },

    // Steal WordPress's template method.
    template: _.memoize(function (id) {
      var compiled,
        options = {
          evaluate:    /<#([\s\S]+?)#>/g,
          interpolate: /\{\{\{([\s\S]+?)\}\}\}/g,
          escape:      /\{\{([^\}]+?)\}\}(?!\})/g,
          variable:    'data'
        };

      return function (data) {
        compiled = compiled || _.template($('#tmpl-' + id).html(), null, options);
        return compiled(data);
      };
    }),

    // Make an AJAX request, attach a nonce to data.
    post: function(action, data) {
      data = data || {};

      return $.post(g.settings.ajax + '/' + action, data);
    }
  });

  /**
   * Useful extensions to base JavaScript classes.
   */

  // Turn a number string into a comma-separated triplet human-readable number.
  Number.prototype.commaString = function() {
    var parts = this.toString().split('.');

    if(parts[0].length == 0) {
      return '';
    }

    var result = parts[0].split('').reverse().join('').match(/(\d{1,3})/g).join(',').split('').reverse().join('');

    if(parts[1]) {
      result += '.' + parts[1];
    }

    return result;
  };

  // Turn "CamelCasedString" into "Camel Cased String".
  String.prototype.reverseCamelGirl = function() {
    return this.replace(/(?!^)(?=[A-Z])/g, ' ');
  };

  // Convert various pipe-wrapped links in text to appropriate links.
  String.prototype.gettyLinkifyText = function() {
    var result = this,
        matches = result.match(/\|(.+?)\|/),
        link;

    while(matches) {
      if(matches[1] == "More information") {
        link = 'http://www.gettyimages.com/Corporate/ReleaseInfo/Release_May_Not_Be_Required_Popup.en-US.pdf';
      }
      else {
        link = 'http://www.gettyimages.com/Corporate/ContactUs.aspx';
      }

      result = result.replace(matches[0], '<a href="' + link + '" target="_getty">' + matches[1] + '</a>');
      matches = result.match(/\|(.+?)\|/);
    }

    return result;
  }
})(jQuery);
