/**
 * @file
 * Menu Per User JS file.
 */

(function ($) {

    /**
     * Provide the summary information for the menu item visibility vertical tabs.
     */
    Drupal.behaviors.usermenuselect = {
        attach: function (context) {
            $('.users-autocomplete').hide();
            $('.users-checkbox').hide();
            // Hide Checkbox when click autocomplete.
            $('#edit-active-1').click(function () {
                $('.users-autocomplete').show();
                $('.users-checkbox').hide();
                $('.summary').text('');
            });
            // Hide Autocomplete when click Checkbox.
            $('#edit-active-0').click(function () {
                $('.users-autocomplete').hide();
                $('.users-checkbox').show();
            });
            // Push Selected Value(s) to display selected users.
            $('fieldset#edit-user', context).drupalSetSummary(function (context) {
                var vals = [];
                $('input[type="checkbox"]:checked', context).each(function () {
                    vals.push($.trim($(this).next('label').text()));
                    $('.summary').css("color", "green");
                });
                return vals.join('<br />');
            });
        }
    };
})(jQuery);
