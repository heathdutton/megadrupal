/*global Drupal: false, jQuery: false, Aviary: false, PaintWeb: false, pwlib: false */
/*jslint devel: true, browser: true, maxerr: 50, indent: 2 */
(function ($) {
  "use strict";

  Drupal.imageeditor = Drupal.imageeditor || {editors: {}, uploaders: {}};
  Drupal.imageeditor.buildUrl = function(opt) {
    var url = opt.launch_url;
    var first_attr = true;
    // For non-clean URLs.
    if (url.indexOf('?') > -1) {
      first_attr = false;
    }
    for (var attribute in opt) {
      if (attribute !== 'launch_url') {
        if (first_attr) {
          url += '?' + attribute + '=' + encodeURIComponent(opt[attribute]);
          first_attr = false;
        }
        else {
          url += '&' + attribute + '=' + encodeURIComponent(opt[attribute]);
        }
      }
    }
    return url;
  };
  Drupal.imageeditor.popup = {
    show: function(options, title) {
      title = title || '';
      var popup_window = window.open(Drupal.imageeditor.buildUrl(options), 'imageeditor', 'location=no,menubar=no,resizable=yes,scrollbars=yes,status=no,titlebar=yes,toolbar=no,channelmode=yes,fullscreen=yes');
      popup_window.focus();
    }
  };

  Drupal.imageeditor.initialize = function(options) {
    var editors_sort = [], uploaders_sort = [], editors_html = '', uploaders_html = '', html, data = {};
    $.each(options.editors, function(index, value) {editors_sort.push([index, value]);});
    editors_sort.sort(function(a, b) {return a[1] - b[1];});
    $.each(options.uploaders, function(index, value) {uploaders_sort.push([index, value]);});
    uploaders_sort.sort(function(a, b) {return a[1] - b[1];});

    $.each(editors_sort, function(index, value) {
      if (typeof(options.image) !== 'undefined' || Drupal.settings.imageeditor[value[0]].image_creation) {
        editors_html += Drupal.settings.imageeditor[value[0]].html;
      }
    });
    editors_html = editors_html ? '<span class="editors">' + editors_html + '</span>' : '';

    if (typeof(options.image) !== 'undefined') {
      $.each(uploaders_sort, function(index, value) {
        uploaders_html += Drupal.settings.imageeditor[value[0]].html;
      });
      uploaders_html = uploaders_html ? '<span class="uploaders">' + uploaders_html + '</span>' : '';

      $.each(options.image, function(index, value) {
        data[index] = value;
        if (index === 'url') {
          data['origurl'] = value;
        }
      });
    }
    if (typeof(options.data) !== 'undefined') {
      $.each(options.data, function(index, value) {
        data[index] = value;
      });
    }
    html = '<span class="imageeditor">' + editors_html + uploaders_html + '</span>';
    data.callback = options.callback;

    var $imageeditor_div = $(html), method = options.method || 'after';
    $imageeditor_div.data(data);
    options.$element[method]($imageeditor_div);

    if (editors_html) {
      $.each(Drupal.imageeditor.editors, function(index, value) {
        value.initialize($imageeditor_div);
      });
    }

    if (uploaders_html) {
      $.each(Drupal.imageeditor.uploaders, function(index, value) {
        value.initialize($imageeditor_div);
      });
    }
  };

})(jQuery);
