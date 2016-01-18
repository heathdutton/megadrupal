/*
 * to be called from Drupal  behaviors
 */

(function ( $ ) {

  $.fn.webformAjaxSlide = function( options ) {

    // bail if we are not call on an webform ajax wrapper
    if ($(this).attr('id').indexOf("webform-ajax-wrapper") !== 0) {
      return this;
    }

    // These are the defaults.
    var defaults = {
      minHeight: '0',
      wrapperId: 'webform-ajax-slide-wrapper',
      loadingDummyClass: 'webform-ajax-slide-loading-dummy',
      loadingDummyMsg: 'loading',
      onSlideFinished: function () {},
      onSlideBegin: function () {},
      onLastSlideFinished: function () {}
    }
    var settings = $.extend({}, defaults, options );

    var $container = this;
    var minHeight = 0;
    var stepForward = true;
    var maxPageNum = parseInt($('input[name="details\[page_count\]"]').attr('value'));
    var pageNum = parseInt($('input[name="details\[page_num\]"]').attr('value'));
    var finished = (pageNum == maxPageNum);

    // generate an wrapper around the ajax wrapper to
    // prevent elements visually sliding over the page
    var $containerWrapper = $('#' + settings.wrapperId);
    if ($containerWrapper.length < 1) {
      $containerWrapper = $('<div id="'+settings.wrapperId+'">');
      $containerWrapper.css({overflow: 'hidden', position: 'relative'});
      $container.wrap($containerWrapper);
    }

    // generate a dummy div to display while loading
    var $loadingdummy = $('<div class="'+settings.loadingDummyClass+'"><div class="container"><div>' + settings.loadingDummyMsg + '</div></div></div>');

    // need to set an beforeSubmit callback to get the
    // direction from a data attribute from the markup
    var processTriggers = function(triggers) {
      $.each(triggers, function(t) {
        var trigger = triggers[t];
        // leave non webform ajax triggers alone
        if (trigger['callback'] != 'webform_ajax_callback') {
          return;
        }
        trigger['beforeSubmit'] = function(form_values, element_settings, options) {
          options["ajaxSlidingDirection"] = $(trigger.element).data("direction");
        }
      });
    }
    // initialize
    processTriggers(Drupal.ajax);

    // called via ajaxSend()
    var onSend = function(ajaxOptions) {
      var targetPageNum;
      // set container anew (from loaded data)
      var $container = $('*[id^=webform-ajax-wrapper]', document);
      var pageNum = parseInt($('input[name="details\[page_num\]"]').attr('value'));
      var finishingStep = (pageNum == maxPageNum);

      settings.onSlideBegin.call($container, ajaxOptions);

      // after possible tampering of context + options by callback
      var stepForward = ajaxOptions["ajaxSlidingDirection"] == "back" ? false : true;
      var context = ajaxOptions.data;

      //if ($containerWrapper.height() > minHeight) {
      //  minHeight = $containerWrapper.height();
      //  $containerWrapper.css({minHeight: minHeight});
      //}

      // TODO code not working due to _triggering_elemet_value being not fixed
      // to step number (could be anything) -- wait for webform steps rework
      // -----------------------
      // if (ajaxOptions.extraData._triggering_element_name === 'step-btn') {
      //  stepForward = (parseInt(ajaxOptions.extraData._triggering_element_value) < pageNum);
      //  console.log([parseInt(ajaxOptions.extraData._triggering_element_value) , pageNum]);
      //  targetPageNum = parseInt(ajaxOptions.extraData._triggering_element_value);
      //}
      // -----------------------

      // set dummy dimensions to current container dimensions
      $loadingdummy.css({height: $container.height() + 'px', width: $container.width() + 'px'});

      // define the animation and it's "reverse"
      var anim = {};
      var reverseAnim = {};
      // one element needs position: relative
      if (stepForward) {
        $loadingdummy.css({position: 'absolute', right: '-120%'});
        $container.css({position: 'relative', right: '', left: '0%'});
        anim = {left: '-150%'};
        reverseAnim = {right: '0%'};
        $loadingdummy.insertBefore($container);
      } else {
        $loadingdummy.css({position: 'absolute', left: '-120%'});
        $container.css({position: 'relative', right: '0%', left: ''});
        anim = {right: '-150%'};
        reverseAnim = {left: '0%'};
        $loadingdummy.insertBefore($container);
      }

      if (finishingStep) {
        ajaxOptions["onLastSlide"] = true;
      }

      // do the slide!
      // set container overflow to hidden to prevent overlapping¬
      var $containerWrapper = $('#'+settings.wrapperId);
      $containerWrapper.css({overflow: 'hidden'});
      // move dummy in
      $loadingdummy.show().animate(reverseAnim, 800);
      // move container out
      $container.animate(anim, 800, function() {});
    };

    var onSuccess = function(ajaxOptions) {
      var $container = $('*[id^=webform-ajax-wrapper]', document);

      $container.css({left: '', right: '', position: 'absolute', opacity: 0});
      $loadingdummy.css('position', 'relative');

      // to the incoming slide
      $loadingdummy.animate({height: $container.height()}, 200, 'swing', function() {
        $container.css('position', 'relative');
        $loadingdummy.css('position', 'absolute').fadeOut(400);
        $container.animate({opacity: 1}, 400, function() {
          $loadingdummy.hide();
          var $containerWrapper = $('#'+settings.wrapperId);
          $containerWrapper.css({overflow: 'visible'});
        });
      });

      //if ($containerWrapper.height() > minHeight) {
      //  minHeight = $containerWrapper.height();
      //  $containerWrapper.css({minHeight: minHeight});
      //}

      if (ajaxOptions["onLastSlide"]) {
        // call finishing callback
        settings.onLastSlideFinished.call($container, ajaxOptions);
      } else {
        settings.onSlideFinished.call($container, ajaxOptions);
      }
    }

    $(document).ajaxSend(function(e, xhr, ajaxOptions) {
      if (!ajaxOptions.data)
        return;

      onSend(ajaxOptions);
    });

    $(document).ajaxSuccess(function(e, xhr, ajaxOptions) {
      if (!ajaxOptions.data)
        return;

      onSuccess(ajaxOptions);

      // recreate beforeSubmit callback on newly loaded elements
      processTriggers(Drupal.ajax);
    });

    return this;
  }
})(jQuery);


