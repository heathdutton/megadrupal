(function ($) {
  Drupal.cacheControl = {};
  Drupal.cacheControl.initialized = false;

  Drupal.cacheControl.componentHandlers = [];
  Drupal.cacheControl.componentsFinishedHandlers = [];

  Drupal.behaviors.cacheControl = {
    attach: function(context, settings) {
      if (Drupal.cacheControl.initialized) {
        //There's no need to ever execute this more than once per pageload
        return;
      }
      Drupal.cacheControl.initialized = true;

      if (settings.cacheControl) {
        var cacheNodeId = settings.cacheControl.cacheNodeId;

        var force = settings.cacheControl.force || false;
        var authenticated = parseInt($.cookie('cacheControlAuthenticated'), 10);

        if (authenticated || force) {
          var path = settings.basePath + "_cache_control/get_components?_q=" + settings.cacheControl.q
          if (settings.cacheControl.query) {
            path += "&" + settings.cacheControl.query;
          }

          $.getJSON(path, function(data) {
            //Inject components into DOM

            if (data.settings) {
              //If some components delivered settings, take them into use.
              jQuery.extend(Drupal.settings, data.settings);
            }

            for (var i in data.components) if (data.components.hasOwnProperty(i)) {
              var elem = $(data.components[i]);
              elem.addClass('cacheControlProcessed');

              if (settings.cacheControl.devMode) {
            	 elem.addClass('cacheControlDev');
              }

              var id = elem.attr("id");

              //Remove the anonymous content to avoid conflicts
              var anonymousElement = $("#" + id + "-anonymous");
              var placeholderElement = $("#" + id);
              Drupal.detachBehaviors(anonymousElement);

              if (placeholderElement.length > 0) {
                anonymousElement.remove();
                Drupal.detachBehaviors(placeholderElement);
                placeholderElement.replaceWith(elem);
              }
              else {
                anonymousElement.replaceWith(elem);
              }
            }

            //Some JS components may need initializing
            for (var b in Drupal.behaviors) if (Drupal.behaviors.hasOwnProperty(b)) {
              if (b !== 'cacheControl' && typeof Drupal.behaviors[b].attach === 'function') {
                Drupal.behaviors[b].attach($('.cacheControlProcessed'), Drupal.settings);
              }
            }

            //Give modules/themes (usually themes) a chance to react to the fact that the page is now completely ready.
            var finishHandlers = Drupal.cacheControl.componentsFinishedHandlers;
            for (var i = 0; i < finishHandlers.length; i++) {
              var handler = finishHandlers[i];
              if (typeof handler == 'function') {
                handler();
              }
            }
          });

        }

        if (!authenticated) {
          //Bring up anonymous content
          $(".cacheControlAnonymous").each(function() {
            var id = $(this).attr('id').replace(/-anonymous$/, '');

            if ($("#" + id).length > 0) {
              $("#" + id).replaceWith($(this));
            }
            $(this).attr('id', id);
            $(this).removeClass('cacheControlAnonymous');

            if (settings.cacheControl.devMode) {
              $(this).addClass('cacheControlDev');
            }
          });

          //If needed, process forms with captcha
          if (settings.cacheControl.processAnonymousCaptchaForms) {
            $("input[name=captcha_response]").each(function() {
              var form = $(this).parents('form');
              if (!form.hasClass('cacheControlProcessedForm')) {
                form.addClass('cacheControlProcessedForm');
                var formBuildId = form.find('input[name=form_build_id]').val();

                //Add a focus event handler that silently and hopefully unobtrusively rebuilds the form in the background
                form.find('*').focus(function() {
                  //store focused element id so that we can restore focus when the form has been re-rendered
                  var focusedId = $(this).attr("id");

                  $.get(Drupal.settings.basePath + "_cache_control/rebuild_form/" + formBuildId, function(data) {
                    var currentContent = $("#" + focusedId).val();
                    var newForm = $(data);
                    form.replaceWith(newForm);

                    //Some JS components may need initializing
                    for (var b in Drupal.behaviors) if (Drupal.behaviors.hasOwnProperty(b)) {
                      if (b !== 'cacheControl' && typeof Drupal.behaviors[b].attach === 'function') {
                        Drupal.behaviors[b].attach(newForm, Drupal.settings);
                      }
                    }

                    //For improved user experience
                    $("#" + focusedId)
                      .val(currentContent)
                      .focus();
                  });
                });
              }
            });
          }

          //Further, track statistics for an anonymous user if so requested
          if (cacheNodeId) {
            $.get(Drupal.settings.basePath + "cache_control_statistics_tracker.php?nid=" + cacheNodeId);
          }
        }
      }
    }
  };
})(jQuery);
