
(function ($) {

  $(document)
    .bind('dart_tag_render', function(event, tag) {
      return tag.replace(/;ord=/, ';' + 'ord=');
    });

  Drupal.dart_tag_ajax = Drupal.dart_tag_ajax || {};

  Drupal.dart_tag_ajax.ads = {
    obj: {},

    init: function() {
      $('.dart-tag')
        .each(function(i, o) {
          try {
            var src = $(this).find('script');
            var s = $(src).filter(':eq(0)').text();
            s = s.replace("Drupal.DART.tag('", '').replace(/'\);$/, '');
            s = JSON.parse(s);
            if (typeof(s) == 'object' && typeof(s.machinename) != 'undefined') {
              Drupal.dart_tag_ajax.ads.obj[s.machinename] = {
                obj: this,
                src: $(src).filter(':eq(1)').attr('src'),
                selector: '.dart-tag-' + s.machinename
              }
            }
          }
          catch (e) {
          };
        })
      .end();
    },

    view_callback: function(target, response) {
      // disable jquery ajax cachebuster
      $.ajaxSetup({ cache: true });
      // return output
      Drupal.DART.settings.writeTags = false;

      $.each(Drupal.dart_tag_ajax.ads.obj, function(i, o) {
        // replace ajax version of ad with initial ad
        var s = '<script type="text/javascript" src="'+ o.src +'"></script>';
        $(o.selector).not('.dart-processed').each(function() {
          var _this = $(this);
          if (typeof(postscribe) == 'function') {
            postscribe(_this, s, function () { _this.addClass('dart-processed'); });
          }
          else if (typeof(_this.writeCapture) == 'function') {
            _this.writeCapture().append(s).addClass('dart-processed');
          }
        });
      });

      var view_classes = $(response.display).attr('class').split(/\s/);
      var view = {};
      for (var v in view_classes) {
        if (/^view-id-(.+)/.test(view_classes[v])) {
          view.name = /^view-id-(.+)/.exec(view_classes[v])[1];
        };
        if (/^view-display-id-(.+)/.test(view_classes[v])) {
          view.display = /^view-display-(.+)/.exec(view_classes[v])[1];
        }
      };
      view.page = $(response.display).children('ul.pager').children('li.pager-current').text();
      // virtual page view
      if (typeof(_gaq) != 'undefined') {
        _gaq.push(['_trackPageview', window.location.pathname +"?view="+ view.name +"-"+ view.display +"&page="+ view.page]);
      }
    },

    refresh: function() {

      Drupal.DART.settings.writeTags = false;
      var ord = Math.floor((Math.random() * 10000000000) + 1);

      $.each(Drupal.dart_tag_ajax.ads.obj, function(i, o) {
        $(o.selector).not('.dart-processed').each(function() {

          // replace ajax version of ad with initial ad
          var s = Drupal.DART.tag(Drupal.DART.ajax[i]);
          s = s.replace(/;ord=\d+/, ';ord='+ord);

          var _this = $(this);
          if (typeof(postscribe) == 'function') {
            postscribe(_this, s, function () { _this.addClass('dart-processed'); });
          }
          else if (typeof(_this.writeCapture) == 'function') {
            _this.writeCapture().append(s).addClass('dart-processed');
          }
        });
      });
    }
  };

  $(document)
    .ready(function(){
      Drupal.dart_tag_ajax.ads.init();
    });

  Drupal.behaviors.DARTajax = {
    attach: function(context) {
      Drupal.dart_tag_ajax.ads.refresh();
    }
  };

})(jQuery)
