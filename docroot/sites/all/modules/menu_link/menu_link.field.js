(function ($) {

/**
 * Automatically fill in a menu link title, if possible.
 */
Drupal.behaviors.menuLinkFieldAutoTitle = {
  attach: function (context) {
    $('.field-type-menu-link .menu-link-item.menu-link-auto-title', context).each(function () {
      // Try to find menu link item elements as well as a entity 'title' field
      // in the form, but play nicely with user permissions and form alterations.
      var $enabled = $('.menu-link-item-enabled', this),
          $link_title = $('.menu-link-item-title', this),
          $title = $(this).closest('form').find('.form-item-title input'),
          required = $link_title.hasClass('required'),
          link_title = $link_title.val();

      // Bail out if we do not have all required fields.
      if (!$link_title.length || !$title.length || !($enabled.length || required)) {
        return;
      }

      // If there is a link title already, mark it as overridden. The user expects
      // that toggling the checkbox twice will take over the node's title.
      if (link_title.length ) {
        if ($enabled.is(':checked')) {
          $link_title.data('menuLinkFieldAutomaticTitleOverridden', true);
        }
        else if (!$enabled.length && required && link_title != $title.val()) {
          $link_title.data('menuLinkFieldAutomaticTitleOverridden', true);
        } 
      }

      // Take over any title change.
      $title.keyup(function () {
        if (!$link_title.data('menuLinkFieldAutomaticTitleOverridden') && ($enabled.length ? $enabled.is(':checked') : required)) {
          $link_title.val($title.val());
          $link_title.val($title.val()).trigger('formUpdated');
        }
      });
      // Whenever the value is changed manually, disable this behavior.
      $link_title.keyup(function () {
        $link_title.data('menuLinkFieldAutomaticTitleOverridden', true);
      });

      // Global trigger on checkbox (do not fill-in a value when disabled).
      $enabled.change(function () {
        if ($enabled.is(':checked')) {
          if (!$link_title.data('menuLinkFieldAutomaticTitleOverridden')) {
            $link_title.val($title.val());
          }
        }
        else {
          $link_title.val('');
          $link_title.removeData('menuLinkFieldAutomaticTitleOverridden');
        }
        $enabled.closest('fieldset.vertical-tabs-pane').trigger('summaryUpdated');
        $enabled.trigger('formUpdated');
      });
    });
  }
};

/**
 * Automatically update summary of fieldsets containging menu link fields.
 * 
 * To enable this behavior enable the Field group module and put your menu link
 * field into a field group.
 * 
 * @link http://drupal.org/project/field_group
 */
Drupal.behaviors.menuLinkFieldAutoFieldsetSummary = {
  attach: function (context) {
    $('fieldset.vertical-tabs-pane .field-type-menu-link', context).each(function () {
      $(this).closest('fieldset.vertical-tabs-pane').drupalSetSummary(function (context) {
        var $item = $('.menu-link-item.menu-link-auto-fieldset-summary:first', context);
        var $enabled = $item.find('.menu-link-item-enabled');
        var $title = $item.find('.menu-link-item-title');
        if (($enabled.length && $enabled.is(':checked')) || $title.val()) {
          return Drupal.checkPlain($title.val());
        }
        else {
          return Drupal.t('Not in menu');
        }
      });
    });
  }
};

})(jQuery);
