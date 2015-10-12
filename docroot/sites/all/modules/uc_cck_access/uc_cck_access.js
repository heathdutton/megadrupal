/**
 * Behavior to dynamically enable/disable dependent controls based on selects & checkboxes settings.
 */

(function ($) {
  Drupal.behaviors.uc_cck_access = {
    attach: function (context, settings) {

      // Enable/disable controls depending on selected delay/duration type
      $('input.ucca_delay_type').each(function() {
        $(this).click(function() {
          processType(this, 'delay');
        });
        processType(this, 'delay');
      });
      $('input.ucca_duration_type').each(function() {
        $(this).click(function() {
          processType(this, 'duration');
        });
        processType(this, 'duration');
      });
      function processType(c, t) {
        if (!$(c).attr('checked')) return;
        v=$(c).attr('value');
        var names = ['period', 'date'];
        for (var i in names) {
          f = names[i];
          d = (v != f);
          p=$(c).parents('fieldset:eq(0)').find('fieldset.' + t + '-' + f + '-fieldset').find('input,select');
          p.each(function() {
            $(this).attr('disabled', d);
          });
        }
      }

      // Enable/disable controls when adding new access on user premium content tab
      $('input.ucca_new_cck_access').each(function() {
        $(this).click(function() {
          processEnable(this);
        });
        processEnable(this);
      });
      function processEnable(c) {
        d = !$(c).attr('checked');
        p=$(c).parents('fieldset:eq(0)').find('input,select').not('.ucca_new_cck_access');
        p.each(function() {
          $(this).attr('disabled', d);
        });
        if (!d) p.each(function() {
          if ($(this).attr('name') == 'delay_type') {
            processType(this, 'delay');
	  }
          if ($(this).attr('name') == 'duration_type') {
            processType(this, 'duration');
	  }
        });
      }
    }
  };
}(jQuery));

