/**
 * @file
 * Connect jumplist to their corresponding multipage panes.
 */

(function ($) {

Drupal.FieldGroup = Drupal.FieldGroup || {};
Drupal.FieldGroup.Effects = Drupal.FieldGroup.Effects || {};

Drupal.behaviors.multipageJumplist = {
  attach: function(context) {
    // Not using context here, as the jumplist menu may be outside it.
    var $jumplist = $('.multipage-jumplist');
    $jumplist.once('multipage-jumplist').find('li a').removeClass('active').each(function() {
      var $self = $(this),
          pane_class = Drupal.settings.multipage_jumplist.map[this.id],
          control = $('.' + pane_class, context).data('multipageControl');
      if (control == undefined) {
	return;
      }
      // Attach the reverse links to the jumplist and jumplist item.
      control.$multipage_jumplist = $jumplist;
      control.$multipage_jumplist_item = $self;
      if (control.nextLink.length) {
	control.nextLink.bind('click', Drupal.multipageJumplist.updateActive);
      }
      if (control.previousLink.length) {
	control.previousLink.bind('click', Drupal.multipageJumplist.updateActive);
      }
      $self.data('multipageControl', control);
      $self.click(function(event) {
	$(this).parent().siblings().find('a.active').removeClass('active').end().end().end()
	  .addClass('active')
	  .data('multipageControl').focus();
	return false;
      });
    });
  }
};

Drupal.multipageJumplist = {

  /**
   * Update the 'active' class in jumplist based on visible pane.
   *
   * This will only work if this is executed after the click event handler
   * setup in field_group's multipage.js.
   */
  updateActive: function() {
    var $open = $('.multipage-pane:visible');
    if ($open.length) {
      var control = $open.data('multipageControl');
      control.$multipage_jumplist.find('a.active').removeClass('active');
      control.$multipage_jumplist_item.addClass('active');
    }
  }
};

/**
 * Drupal.FieldGroup.Effects.processHook implementation.
 *
 * This hook implementation exists in order to run some JS and update the
 * 'active' class after field_group.js is finished processing and selected the
 * active pane.
 */
Drupal.FieldGroup.Effects.processMultipageJumplist = {
  execute: function (context, settings, type) {
    Drupal.multipageJumplist.updateActive($('.multipage-jumplist'));
  }
};

})(jQuery);
