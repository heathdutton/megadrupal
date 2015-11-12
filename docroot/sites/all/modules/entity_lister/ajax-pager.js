/**
 * @file
 * AJAX pager.
 */

(function($) {
  "use strict";
  Drupal.behaviors.entity_lister = {
    attach: function(context, settings) {

      var entity_lister = {

        pager: function(vals, page, id) {

          if (!page) {
            vals.page = '0';
          }
          else {
            vals.page = page;
          }

          $.post('/?q=' + vals.config.pager_path, vals, function(data) {
            $('#' + id).html(data);
            Drupal.attachBehaviors('#' + id);
          });

          if ('1' === vals.config.pager_st) {
            // Scroll up.
            var offset = $('#' + id).offset();
            var st = offset.top - vals.config.pager_st_offset;
            var duration = parseInt(vals.config.pager_st_time, 10);
            $("html, body").animate({scrollTop: st}, {duration: duration});
          }

          return false;
        },

        pagerListen: function(vals, id) {
          $('#' + id + ' .pager a').once().click(function(e){
            e.preventDefault();
            var href = $(this).attr('href');
            var arr = href.split('?');
            var qs = '?' + arr[1];
            var page = parseQS('page', qs);

            if (vals.config.headers.length) {
              vals.order = parseQS('order', qs);
              vals.sort = parseQS('sort', qs);
            }

            entity_lister.pager(vals, page, id);
          });
        }

      };

      var s = settings.entity_lister;

      for (var d = 0; d < s.length; d++) {

        var v = {};

        v.config = s[d].config;
        v.delta = d;
        v.id_prefix = s[d].id_prefix;
        v.list_class = s[d].list_class;
        v.extra = s[d].extra;

        var id = v.id_prefix + '-' + d;

        entity_lister.pagerListen(v, id);

      }

    }
  };

})(jQuery);
