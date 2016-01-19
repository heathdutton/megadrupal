(function ($) {
  var clonePrefix;
  // Add some utility links
  Drupal.behaviors.shortcuts = {
    attach: function(context) {
      Drupal.settings.financeExpenses = Drupal.settings.financeExpenses || { showCloneShortcuts: false };
      if (Drupal.settings.financeExpenses.showCloneShortcuts == true) {
        clonePrefix = Drupal.t('Clone of ');
        $('<div class="shortcuts">' + "\n" +
            "\t" + '<a class="next-week" href="#">Clone to next week</a>' + "\n" +
            "\t" + ' | <a class="next-month" href="#">Clone to next month</a>' + "\n" +
            "\t" + ' | <a class="strip-clone-of" href="#">Erase \"' + clonePrefix + '\"</a>' + "\n" +
          '</div>')
          .insertAfter('.form-item-title input');
        $('.form-item-title .shortcuts .next-month, .form-item-title .shortcuts .next-week').click(
          function() {
            eraseCloneOf($('.form-item-title input'));
            // Add 1 to the first part of the date
            var $effectiveDateObj = $('#edit-field-effective-date-und-0-value-datepicker-popup-0');
            // Without focusing, the component doesn't seem to get initialized
            $effectiveDateObj.focus();
            var effectiveDate = new Date(Date.parse($effectiveDateObj.val()));
            if ($(this).hasClass('next-week')) {
              effectiveDate.setDate(effectiveDate.getDate() + 7);
            }
            if ($(this).hasClass('next-month')) {
              effectiveDate.setMonth(effectiveDate.getMonth() + 1);
            }
            var newEffectiveDate = new Date(Date.parse(effectiveDate.toDateString()));
            $effectiveDateObj.datepicker('setDate', newEffectiveDate);
            $effectiveDateObj.parents('form:eq(0)').submit();
            return false;
          }
        );
        $('.form-item-title .shortcuts .strip-clone-of').click(
          function() {
            eraseCloneOf($('.form-item-title input'));
          }
        )
      }
    }
  }
  function eraseCloneOf($obj) {
    $obj.val($obj.val().replace(clonePrefix, ''));
  }
  Drupal.behaviors.autoCheckSpecial = {
    attach: function(context) {
      $('#edit-field-pay-from input').click(
        function() {
          if ($('#edit-field-pay-from input:checked').length) {
            $('#edit-field-special input').attr('checked', 'checked');
          }
          else {
            $('#edit-field-special input').removeAttr('checked');
          }
        }
      );     
    }
  }
})(jQuery);
