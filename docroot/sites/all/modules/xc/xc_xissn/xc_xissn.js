/**
 * @file
 * JavaScript functionality needed for XC xISSN module
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

(function ($) {
  $(document).ready(function() {
    var options = {
      'numbers': Drupal.settings.xc_xissn.numbers
    };
    $.get(Drupal.settings.xc_xissn.url, options, function(info) {
      // info = jQuery.parseJSON(data);
      if (info != null) {
        var meta = info.xc_xissn.meta;
        var content = info.xc_xissn.content;
        var tr = $('#' + meta.id);
        if (meta.title != null) {
          var td = $('td.enhancement-title', tr);
          $('td.enhancement-title', tr).html(meta.title);
        }

        if (content != null) {
          var html;
          var actual = tr;
          for (title in content) {
            var tds = Drupal.theme('xc_td', title, 'xc-label')
                    + Drupal.theme('xc_td', content[title]);
            var newTr = Drupal.theme('xc_tr', tds, 'enhancement-content');
            actual = actual.after(newTr).next();
          }
        }
      }
    });
  });
}(jQuery));