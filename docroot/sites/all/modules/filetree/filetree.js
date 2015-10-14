(function ($) {

Drupal.behaviors.filetree = {

  attach: function(context, settings) {

    // Collapse the sub-folders.
    $('.filetree .files ul').hide();

    // Expand/collapse sub-folder when clicking parent folder.
    $('.filetree .files li.folder').click(function(e) {
      // A link was clicked, so don't mess with the folders.
      if ($(e.target).is('a')) {
        return;
      }
      // Determine whether or not to use an animation when toggling folders.
      var animation = $(this).parents('.filetree').hasClass('filetree-animation') ? 'fast' : '';
      // If multiple folders are not allowed, collapse non-parent folders.
      if (!$(this).parents('.filetree').hasClass('multi')) {
        $(this).parents('.files').find('li.folder').not($(this).parents()).not($(this)).removeClass('expanded').find('ul:first').hide(animation);
      }
      // Expand.
      if (!$(this).hasClass('expanded')) {
        $(this).addClass('expanded').find('ul:first').show(animation);
      }
      // Collapse.
      else {
        $(this).removeClass('expanded').find('ul:first').hide(animation);
      }
      // Prevent collapsing parent folders.
      return false;
    });

    // Expand/collapse all when clicking controls.
    $('.filetree .controls a').click(function() {
      var animation = $(this).parents('.filetree').hasClass('filetree-animation') ? 'fast' : '';
      if ($(this).hasClass('expand')) {
        $(this).parents('.filetree').find('.files li.folder').addClass('expanded').find('ul:first').show(animation);
      }
      else {
        $(this).parents('.filetree').find('.files li.folder').removeClass('expanded').find('ul:first').hide(animation);
      }
      return false;
    });

  }

};

})(jQuery);
