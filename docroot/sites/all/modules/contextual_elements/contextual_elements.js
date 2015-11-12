(function ($) {
  /**
   * Provide the summary information for the contextual elements
   * settings vertical tabs.
   */
  Drupal.behaviors.contextualElementsSettingsSummary = {
    attach: function (context) {
      // The drupalSetSummary method required for this behavior is not available
      // on the Blocks administration page, so we need to make sure this
      // behavior is processed only if drupalSetSummary is defined.
      if (typeof jQuery.fn.drupalSetSummary == 'undefined') {
        return;
      }

      $('fieldset#edit-domains', context).drupalSetSummary(function (context) {
        var $ret = '';
        var $method = $('input[name="domain_inc"]:checked', context);
        var $list = $('textarea[name="domains"]', context).val();

        $ret += ($method.val() == 1 ? Drupal.t('Include: ') : Drupal.t('Exclude: '));
        $ret += ($list.length > 0 ? $list.replace(/\n/g, ', ') : Drupal.t('None'));

        return $ret;
      });

      $('fieldset#edit-paths', context).drupalSetSummary(function (context) {
        var $ret = '';
        var $method = $('input[name="page_inc"]:checked', context);
        var $list = $('textarea[name="pages"]', context).val();

        $ret += ($method.val() == 1 ? Drupal.t('Include: ') : Drupal.t('Exclude: '));
        $ret += ($list.length > 0 ? $list.replace(/\n/g, ', ') : Drupal.t('None'));

        return $ret;
      });

      $('fieldset#edit-roles', context).drupalSetSummary(function (context) {
        var $ret = '';
        var $roles = [];
        var $method = $('input[name="role_inc"]:checked', context);

        $ret += ($method.val() == 1 ? Drupal.t('Include: ') : Drupal.t('Exclude: '));

        $('.form-item-roles input:checkbox', context).each(function() {
          if ($(this).is(':checked')) {
            $roles.push($(this).next('label').text().replace(/\ +$/g, ''));
          }
        });

        $ret += ($roles.length > 0 ? $roles.join(', ') : Drupal.t('None'));

        return $ret;
      });

      $('fieldset#edit-users', context).drupalSetSummary(function (context) {
        var $ret = '';
        var $method = $('input[name="users"]:checked', context);

        $ret += ($method.val() == 1 ? Drupal.t('Enabled') : Drupal.t('Disabled'));

        return $ret;
      });
    }
  }
})(jQuery);
