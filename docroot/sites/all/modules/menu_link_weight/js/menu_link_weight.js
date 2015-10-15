/**
 * @file
 * Menu Link Weight Javascript functionality.
 */

(function ($) {
  /**
   * Automatically update the current link title in the menu link weight list.
   */
  Drupal.behaviors.menuLinkWeightAutomaticTitle = {
    attach: function (context) {
      $('fieldset.menu-link-form', context).each(function () {
        var $checkbox = $('.form-item-menu-enabled input', this);
        var $link_title = $('.form-item-menu-link-title input', context);
        var $current_selection = $('.menu-link-weight-link-current', context);
        var $node_title = $(this).closest('form').find('.form-item-title input');
        // If there is no title, take over the title of the link.
        if ($current_selection.html() == '') {
          $current_selection.html($link_title.val().substring(0, 30));
        }
        // Take over any link title change.
        $link_title.keyup(function () {
          $current_selection.html($link_title.val().substring(0, 30));
        });
        // Also update on node title change, as this may update the link title.
        $node_title.keyup(function () {
          $current_selection.html($link_title.val().substring(0, 30));
        });
      });
    }
  };

})(jQuery);
