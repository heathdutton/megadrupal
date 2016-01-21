/**
 * @file
 * Defines ajax functions to load minipanels.
 */

(function ($) {
  Drupal.behaviors.panelsMiniAjax = {
    attach: function (context, settings) {
      $(".panel-display a").click( function(event) {
        var href = $(this).prop("href");

        if (typeof href == "undefined") {
          return;
        }

        var args = href.split("?");

        if (args.length == 1) {
          var hash = args[0].split("#");
          args[1] = "";
          if (hash.length > 1 && this.pathname == window.location.pathname) {
            return;
          }
        }

        if (args.length > 1) {
          var hash = args[1].split("#");
          if (hash.length > 1) {
            args[1] = hash[0];
            if (hash[0] == window.location.search.substr(1) && this.pathname == window.location.pathname) {
              return;
            }
          }
        }

        if (this.pathname == window.location.pathname) {
          event.preventDefault();
          Drupal.behaviors.panelsMiniAjax.updateMiniPanels(args[1]);
        } else if (args[0].indexOf("/panels_mini_ajax/ajax/") > -1) {
          event.preventDefault();
          Drupal.behaviors.panelsMiniAjax.updateMiniPanels(args[1]);
        }
      });

      $('.panel-display button[type="submit"]').click(Drupal.behaviors.panelsMiniAjax.handleInput);
      $('.panel-display input[type="submit"]').click(Drupal.behaviors.panelsMiniAjax.handleInput);
      $('.panel-display input').submit(Drupal.behaviors.panelsMiniAjax.handleInput);
    },

    handleInput: function (event) {
      var form = $(this).parents("form").first();
      var action = $(form).attr("action");
      if ((action == window.location.pathname) || (action.indexOf("/panels_mini_ajax/ajax/") > -1)) {
        event.preventDefault();
        args = '';
        var input = form.find("input, select, textarea, datalist, output");
        $(input).each(function (index, element) {
          var name = $(this).attr('name');
          var val = encodeURI($(this).attr('value'));
          if (name != '') {
            if (args != '') {
              args += "&";
            }
            args += name + '=' + val;
          }
        });
        Drupal.behaviors.panelsMiniAjax.updateMiniPanels(args);
      }
    },

    updateMiniPanels: function (args) {
      $.each(Drupal.settings.panelsMiniAjax, function (index, mini_panel) {
        var callback = Drupal.settings.basePath + Drupal.settings.pathPrefix + "panels_mini_ajax/ajax/" + mini_panel + "?" + args;
        var pane = '#mini-panel-' + mini_panel; // + " .pane-content";
        $.get(callback, "", function(data, textStatus) {
          $(pane).fadeTo('slow', 0, function() {
             $(pane).replaceWith(data);
             Drupal.attachBehaviors(pane);
             $(pane).fadeTo('slow', 1);
          });
        });
      });
    }
  };
})(jQuery);
