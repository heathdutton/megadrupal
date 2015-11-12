
(function ($) {
  Drupal.behaviors.lingwo_senses = {
    attach: function (context, settings) {
      function triggerAjax() {
        var id = 'edit-lingwo-senses-refresh',
          ajax_settings = settings.ajax[id],
          event_name = ajax_settings['event'],
          selector = ajax_settings['selector'];

        $(selector).trigger(event_name);
      };

      Drupal.lingwo_entry.setCallback('lingwo_senses', 'language', triggerAjax);

      // bind remove checkbox to trigger AHAH
      $('.lingwo-senses-remove', context).once('lingwo_senses', function () {
        $(this).change(function () {
          var match, selector;
          if (match = (''+this.name).match(/^lingwo_senses\[(\d+)\]/)) {
            selector = "input[name='lingwo_senses[" + match[1] + "][new]']";
            if ($(selector, $(this).closest('form')).val() == '1') {
              triggerAjax();
            }
          }
        });
      });

      /*
       * Setup our UI toggles (Same As, No equivalent, Relationship)
       */

      function makeToggleFunc(checkFunc, toggleSelector, parent) {
        return function (node, anim) {
          var show = checkFunc(node),
            match, selector, res;

          // find the index and use that to locate the element we want to toggle
          if (match = /^lingwo_senses\[(\d+)\]/.exec(node.name+'')) {
            selector = toggleSelector.replace('%', match[1]);
            res = $(selector, $(node).closest('form'));
            if (parent) {
              res = res.parent();
            }
            res[show ? 'show' : 'hide'](anim ? 'fast' : null);
          }
        };
      }

      function checkSelectHasValue(node) {
        return $(':selected', node).val() != '';
      }

      function checkIsChecked(node) {
        return $(node).is(':checked');
      }

      function not(func) {
        return function () {
          return !func.apply(null, arguments);
        };
      }

      var toggles = {
        '.same-as-select': makeToggleFunc(not(checkSelectHasValue), '#edit-lingwo-senses-%-data-translation-wrapper'),
        '.no-equivalent-checkbox': makeToggleFunc(not(checkIsChecked), '#edit-lingwo-senses-%-data-translation-trans-wrapper'),
        '.is-relationship': makeToggleFunc(not(checkSelectHasValue), "input[name='lingwo_senses[%][data][difference]']", true),
      }, selector;

      for(selector in toggles) {
        (function (selector, toggle) {
          $(selector).once('lingwo_senses', function () {
            $(this).change(function (evt) {
              toggle(evt.target, true);
            });
          });
          $(selector, context).each(function () { toggle(this); });
        })(selector, toggles[selector]);
      }
    }
  }
})(jQuery);

