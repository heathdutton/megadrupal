/**
 * @file
 */

(function ($) {

  "use strict";

  Drupal.ajaxinclude = Drupal.ajaxinclude || {};

  Drupal.behaviors.ajaxinclude = {
    attach: function (context, settings) {

      $(".ajaxinclude", context).once("ajaxinclude", function () {
        var t = $(this),
          id = "#" + t.attr("id"),
          ai = $("[data-ajaxinclude]", context),
          aix = $("[data-exclude]", context),
          ain = $("[data-include]", context),
          act = $("[data-interaction]", context),
          evi = act.data("interaction"),
          actid = "#" + $(".ajaxinclude--interaction", context).attr("id"),
          loader = $(".ajaxinclude-loader", t),
          spin = loader.data("spin") !== "undefined" ? loader.data("spin") : 0,
          minHeight = t.data("minHeight") !== "undefined" ? {minHeight: t.data("minHeight")} : "",
          list = settings.ajaxinclude.list ? {
            proxy: Drupal.settings.basePath + "ajaxinclude?blocks="
          } : {};

        if (spin > 0) {
          for (var i = 1; i <= spin; i++) {
            loader.append('<span class="ai ai' + i + '" />');
          }
        }

        t.css(minHeight).removeAttr("data-min-height");
        aix.ajaxInclude();
        ain.ajaxInclude(list);

        $(".ajaxinclude-loader", actid).addClass("is-hidden");
        act.one(evi, function (e) {
          e.preventDefault();
          e.stopPropagation();

          $(".ajaxinclude-loader", actid).removeClass("is-hidden");
          $(actid).removeClass("ajaxinclude--loaded");

          $(e.target).removeAttr("data-interaction").ajaxInclude({
            headerCallbacks: {
              'X-AjaxInclude-Redirect': function (url) {
                window.location.href = url;
              }
            }
          });
        });

        $("form[data-interaction]").one("submit", function (e) {
          e.preventDefault();
          $(this).removeAttr("data-interaction").ajaxInclude();
        });

        ai.bind("ajaxIncludeResponse", function (e, data) {
          Drupal.detachBehaviors(id);
          Drupal.ajaxinclude.done(id);
          Drupal.attachBehaviors(id);
        });
      });
    }
  };

  Drupal.ajaxinclude.done = function (id) {
    $(id).addClass("ajaxinclude--loaded");
    $(".ajaxinclude-loader", id).addClass("is-hidden");
  };

})(jQuery);
