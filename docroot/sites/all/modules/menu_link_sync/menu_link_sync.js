/**
 * @file
 * Javascript functionality for Menu Link Sync.
 */

(function($) {

  /**
   * Make sure the menu link title stays synchronized with the node title.
   *
   * If no "Menu link title" is defined in the node input form, we will
   * simulate two clicks on the "Provide a menu link" checkbox so we
   * can synchronize the "Menu link title" with the "Node title".
   */
  Drupal.ajax.prototype.commands.menuLinkSyncProcessMenuLinkTitle = function(ajax, response, status) {
    if (!$('.form-item-menu-link-title input').val().length) {
      // Attach behaviors defined in menu.js to menu settings form.
      Drupal.attachBehaviors($('.form-item-menu-enabled input').parents('form'));
      // Simulate two clicks on the "Provide a menu link" checkbox so as to
      // synchronize the "Menu link title" with the "Node title".
      $('.form-item-menu-enabled input').click().change().click().change();
    }
  }

})(jQuery);
