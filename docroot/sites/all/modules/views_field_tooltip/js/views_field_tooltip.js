var ViewsFieldTooltip = ViewsFieldTooltip || { Library: {} };

(function ($) {

  Drupal.behaviors.viewsFieldTooltip = {
    attach: function(context) {
      // TODO: the qtip() function is sometimes not found on preview - figure out why.
      if (typeof $.fn.qtip === 'undefined') return;

      var ajaxify = function(qtip, url) {
        return ViewsFieldTooltip.Library.ajaxify(qtip, url);
      }

      // Expand tokens found in qtip settings.
      var expand = function(tokens, qtip) {
        var target = $.extend(true, {}, qtip); // deep clone the settings object to avoid modifying the original
        for (prop in target) {
          if (target[prop] != null && typeof tokens[target[prop]] !== 'undefined') {
            target[prop] = tokens[target[prop]];
          }
          else if (typeof target[prop].match === 'function' && target[prop].match(/^\$\(.*?\)$/)) {
            target[prop] = eval(target[prop]);
          }
          else if (typeof target[prop] === 'object') {
            target[prop] = expand(tokens, target[prop]);
          }
        }
        return target;
      };

      // Loop on tooltip settings for all views and displays:
      // - insert the tooltip theme into the DOM
      // - configure the tooltip triggers
      $.each(Drupal.settings.viewsFieldTooltip, function(view, displays) {
        $.each(displays, function(display, settings) {

          // Set the label tooltips.
          $.each(settings.labels, function(field, tooltip) {
            $('.view-id-' + view + '.view-display-id-' + display + ' .views-label-tooltip-field-' + field)
              .once('views-field-tooltip')
              .append(tooltip);
          });
          $('.view-id-' + view + '.view-display-id-' + display + ' .views-label-tooltip-icon')
            .once('views-field-tooltip')
            .each(function() {
              $(this).qtip(expand({ "$target": $(this) }, settings.qtip));
            });

          // Set the field tooltips.
          $.each(settings.fields, function(row, fields) {
            $row = $('.view-id-' + view + '.view-display-id-' + display + ' .views-field-tooltip-row:eq(' + row + ')');
            $.each(fields, function(field, tooltip) {
              $('.views-field-tooltip-field-' + field, $row)
                .once('views-field-tooltip')
                .append(tooltip.theme);
              qtip = tooltip.qtip || settings.qtip;
              $('.views-field-tooltip-field-' + field + '.views-field-tooltip-text .views-field-tooltip-icon', $row)
                .once('views-field-tooltip')
                .each(function() {
                  $(this).qtip(expand({ "$target": $(this) }, qtip));
                });
              $('.views-field-tooltip-field-' + field + '.views-field-tooltip-ajax .views-field-tooltip-icon', $row)
                .once('views-field-tooltip')
                .each(function() {
                  $(this).qtip(ajaxify(expand({ "$target": $(this) }, qtip), $(this).attr('data-url')));
                });
            });
          });
        });
      });
    }
  };

})(jQuery);
