/**
 * @file
 * Hierarchical select integration.
 */

(function($) {

  Drupal.behaviors.MenuLinkWeightHierarchicalSelect = {
    attach: function (context) {
      // Remove the "has-no-children" class so that even selecting a parent
      // item without any children will trigger an AJAX update (and load an
      // empty menu link weights list).
      $('.hierarchical-select-wrapper .has-no-children', context).removeClass('has-no-children');
    }
  };

  // Use Hierarchical Select AJAX implementation to update the Menu Link Weight
  // fieldset when another option is chosen.
  Drupal.ajax.prototype.commands.menuLinkWeightHierarchicalSelectUpdate = function(ajax, response, status, hsid) {
    $('#menu-link-weight-wrapper').replaceWith($(response.output));
    Drupal.attachBehaviors($('#menu-link-weight-wrapper'));
  };

})(jQuery);
