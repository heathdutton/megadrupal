/**
 * @file
 * Custom javascript.
 */

var smartling = smartling || {};
smartling.forms = smartling.forms || {};

(function ($) {

    // Page setup for node add/edit forms.
    smartling.forms.init = function () {
        $("#edit-language").change(updateVerticalTabSummary).change();
        $('#edit-target .form-checkbox').change(updateVerticalTabSummary);
        updateVerticalTabSummary();
    };

    var updateVerticalTabSummary = function () {
        var summaryMessages = [];

        summaryMessages.push(Drupal.t("Target Locales:"));

        var language = $("#edit-language").val();
        var sourceLanguageSet = language != 'und';

        var locales = [];
        var count = false;

        $('#edit-target .form-checkbox').each(function () {
            if ($(this).is(":checked")) {
                count = true;
                locales.push($(this).parent().find('.option').html().trim());
            }
        });

        if (count) {
            locales.forEach(function (entry) {
                summaryMessages.push(entry);
            });
        } else {
            summaryMessages.push('No select Language');
        }

        // Source language set or not
        if (sourceLanguageSet) {
            $('#smartling_fieldset .form-item-target').show();
            $('#edit-submit-to-translate').show();
        }
        else {
            $('#smartling_fieldset .form-item-target').hide();
            $('#edit-submit-to-translate').hide();
        }

        var extraDetails = summaryMessages.slice(1).join(', ');
        extraDetails = extraDetails.length ? '(' + extraDetails + ')' : extraDetails;
        $('#smartling_fieldset').drupalSetSummary(summaryMessages.slice(0, 1) + ' ' + extraDetails);
    };

})(jQuery);

Drupal.behaviors.smartlingSetupStatus = {
    attach: smartling.forms.init
}
