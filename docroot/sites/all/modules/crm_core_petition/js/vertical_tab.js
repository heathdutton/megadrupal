(function ($) {
    Drupal.behaviors.cmcp_vertical_tab = {
        attach: function (context) {
            // Petition targets tab
            $('fieldset#edit-group_cmcp_send_target', context).drupalSetSummary(function (context) {
                if ($('.field-name-field-cmcp-target-emails input', context).is(':checked')) {
                    return Drupal.checkPlain($('.field-name-field-cmcp-target-ids input', context).val());
                }
                else {
                    return Drupal.t('No targets');
                }
            });
            // Petition settings tab
            $('fieldset#edit-crm-core-petition', context).drupalSetSummary(function (context) {
                var goal;
                var signatories;
                goal = $('.form-item-goal input', context).val();
                if (!goal) {
                    goal = Drupal.t('no target');
                }
                if ($('.form-item-signatories input', context).is(':checked')) {
                    signatories = Drupal.t('yes');
                }
                else {
                    signatories = Drupal.t('no');
                }
                return Drupal.t('Target: @goal, Show signatories: @signatories', { '@goal': goal, '@signatories': signatories});
            });
        }
    };
})(jQuery);
