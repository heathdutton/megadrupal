(function($) {

  Drupal.behaviors.asafFormWrappers = {
    attach: function(context, options) {
      // If context is form we have to take a parent to find form in context
      context = context.jquery ? context.parent() : context;

      $('form[data-asaf-area-wrapper-id]', context).once('area-wrapper', function() {
        var area = $(this);
        var wrapperId = area.data('asaf-area-wrapper-id');

        if (!area.parent().is('#'+wrapperId)) {
          area.wrap('<div id="' + wrapperId + '" class="asaf-area-wrapper" />')
        }
      });
    }
  };

  Drupal.behaviors.asafSubmitByEnter = {
    attach: function(context, options) {
      if (options.asaf.submitByEnter) {
        $('input.ajax-processed').each(
          function () {
            var $form = $(this).parents('form');

            $form.once('submit-by-enter', function () {
              $form.bind('keydown', function (event) {
                if (event.keyCode == 13 && event.target.tagName.toLocaleLowerCase() == 'input' && event.target.type !== 'submit' && event.target.type !== 'button') {
                  var $firstButton = $('input.ajax-processed:eq(0)');
                  var event = options.ajax[$firstButton.attr('id')]['event'];
                  if (event) {
                    $firstButton.trigger(event);
                  }
                }
              })

            });
          }
        );
      }
    }
  };

  Drupal.ajax = Drupal.ajax || {};
  Drupal.ajax.prototype.commands = Drupal.ajax.prototype.commands || {};

  Drupal.ajax.prototype.commands.asafReload = function (ajax, response, status) {
    var loc = response.window == 'parent' ? parent.location : location;
    loc.reload();
  }

  Drupal.ajax.prototype.commands.asafRedirect = function (ajax, response, status) {

    var loc = response.window == 'parent' ? parent.location : location;
    loc.href = response.href;
  }

})(jQuery);
